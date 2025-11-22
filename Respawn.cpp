//Respawn.cpp
#include "Respawn.h"
#include "ArkaP.h"
#include <iostream>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include "DurumYönetici.h"
#include "Effect.h"
class ArkaP;
class Effect;
bool Respawn::efektAktif = false;
Respawn* Respawn::aktifInstance = nullptr;  // ← DOĞRU
std::random_device Respawn::rd{};
std::mt19937 Respawn::gen{ Respawn::rd() };
sf::Clock Respawn::Saat{};
std::vector<sf::FloatRect> ArkaP::Engel{};

Respawn::Respawn(DurumYoneticisi* yonetici) : Tile(sf::PrimitiveType::Triangles), yonetici(yonetici), font("arial.ttf"), text(font)
{
    aktifInstance = this; // Bu instance'ı global yap
}


 

    // ÖNEMLİ: Belirtilen pozisyondaki blok bounds'larını hesapla
    std::vector<sf::FloatRect> Respawn::BlokBoundslariniGetir(float offsetX = 0, float offsetY = 0) const {
        std::vector<sf::FloatRect> bloklar;

        for (size_t i = 0; i < matris.size(); i++) {
            for (size_t j = 0; j < matris[i].size(); j++) {
                if (matris[i][j] == 1) {
                    float blokX = x + offsetX + j * (bloklukBoyut + 1);
                    float blokY = y + offsetY + i * (bloklukBoyut + 1);

                    sf::FloatRect blokBounds({ blokX, blokY }, { bloklukBoyut, bloklukBoyut });
                    bloklar.push_back(blokBounds);
                }
            }
        }

        return bloklar;
    }

    // Geliştirilmiş çarpışma kontrolü - offset parametreli
    bool Respawn::CarpismaKontrol(const std::vector<sf::FloatRect>& engeller, float offsetX = 0, float offsetY = 0) const {
        std::vector<sf::FloatRect> benimBloklarim = BlokBoundslariniGetir(offsetX, offsetY);

        for (const auto& benimBlok : benimBloklarim) {
            for (const auto& engel : engeller) {
                if (benimBlok.findIntersection(engel).has_value()) {

                    return true; // Çarpışma var
                }
            }
        }

        return false; // Çarpışma yok
    }

    int Respawn::myrand(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);

    }

    void Respawn::DortgenEkle(float px, float py, float gen, float uz) {

        sf::Vertex v1;
        v1.position = { px, py };
        v1.texCoords = { 0.f,0.f };
        v1.color = Renk;
        Tile.append(v1);

        sf::Vertex v2;
        v2.position = { px + gen,py };
        v2.texCoords = { bloklukBoyut,0.f };
        v2.color = Renk;
        Tile.append(v2);

        sf::Vertex v3;
        v3.position = { px,py + uz };
        v3.texCoords = { 0.f,bloklukBoyut };
        v3.color = Renk;
        Tile.append(v3);

        sf::Vertex v4;
        v4.position = { px + gen,py };
        v4.texCoords = { bloklukBoyut,0.f };
        v4.color = Renk;
        Tile.append(v4);

        sf::Vertex v5;
        v5.position = { px + gen,py + uz };
        v5.texCoords = { bloklukBoyut,bloklukBoyut };
        v5.color = Renk;
        Tile.append(v5);

        sf::Vertex v6;
        v6.position = { px,py + uz };
        v6.texCoords = { 0.f,bloklukBoyut };
        v6.color = Renk;
        Tile.append(v6);
    }

    void Respawn::VertexOlustur() {
        Tile.clear();
        Sekilvec.clear();

        for (size_t i = 0; i < matris.size(); i++) {
            for (size_t j = 0; j < matris[i].size(); j++) {
                if (matris[i][j] == 1) {
                    DortgenEkle(
                        x + j * (bloklukBoyut + 1),
                        y + i * (bloklukBoyut + 1),
                        bloklukBoyut,
                        bloklukBoyut
                    );
                }
            }
        }
        Sekilvec.push_back(Tile);
    }

    void Respawn::ISekilOlustur() {
        matris = { {1}, {1}, {1}, {1} };
        Renk = sf::Color::Cyan;
    }

    void Respawn::OSekilOlustur() {
        matris = { {1, 1}, {1, 1} };
        Renk = sf::Color::Yellow;
    }

    void Respawn::TSekilOlustur() {
        matris = { {0, 1, 0}, {1, 1, 1} };
        Renk = sf::Color::Magenta;
    }

    void Respawn::SSekilOlustur() {
        matris = { {0, 1, 1}, {1, 1, 0} };
        Renk = sf::Color::Green;
    }

    void Respawn::ZSekilOlustur() {
        matris = { {1, 1, 0}, {0, 1, 1} };
        Renk = sf::Color::Red;
    }

    void Respawn::JSekilOlustur() {
        matris = { {1, 0, 0}, {1, 1, 1} };
        Renk = sf::Color::Blue;
    }

    void Respawn::LSekilOlustur() {
        matris = { {0, 0, 1}, {1, 1, 1} };
        Renk = sf::Color(255, 165, 0);
    }

    void Respawn::RastgeleSekilOlustur() {
        int sekilTipi = myrand(0, 6);
        switch (sekilTipi) {
        case 0: ISekilOlustur(); break;
        case 1: OSekilOlustur(); break;
        case 2: TSekilOlustur(); break;
        case 3: SSekilOlustur(); break;
        case 4: ZSekilOlustur(); break;
        case 5: JSekilOlustur(); break;
        case 6: LSekilOlustur(); break;
        default: ISekilOlustur(); break;
        }
    }



    // YENİ: Tüm engelleri kabul eden hareket fonksiyonu
    void Respawn::hareketGuvenlı(const std::vector<sf::FloatRect>& duvarlar,
         float dt) {
        yatayTimer += dt;

        TusCd();
        if (Yerdemi || Tus) return;

        // Tüm engelleri birleştir
        std::vector<sf::FloatRect> tumEngeller = duvarlar;
     //   for (const auto& va : doluBloklar) {
         //   tumEngeller.push_back(va.getBounds());
      //  }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && yatayTimer >= yatayCd) {
            // Sol hareket kontrol
            if (!CarpismaKontrol(tumEngeller, -17.0f, 0)) {
                x -= 17.0f;
                VertexOlustur();
            }
            Tus = true;
            yatayTimer = 0;

            TusSaat.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && yatayTimer >= yatayCd) {
            // Sağ hareket kontrol
            if (!CarpismaKontrol(tumEngeller, 17.0f, 0)) {
                x += 17.0f;
                VertexOlustur();
                yatayTimer = 0;

            }
            Tus = true;
            TusSaat.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && yatayTimer >= yatayCd) {
            // Aşağı hareket kontrol

            if (!CarpismaKontrol(tumEngeller, 0, 12.0f)) {
                y += 17.0f;
                VertexOlustur();
                yatayTimer = 0;

            }


            Tus = true;
            TusSaat.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            DondurGuvenlı(tumEngeller);
            Tus = true;
            TusSaat.restart();
        }
    }

    void Respawn::Dondur() {
        if (Yerdemi) return;

        int satir = matris.size();
        int sutun = matris[0].size();

        std::vector<std::vector<int>> yeniMatris(sutun, std::vector<int>(satir));

        for (int i = 0; i < satir; i++) {
            for (int j = 0; j < sutun; j++) {
                yeniMatris[j][satir - 1 - i] = matris[i][j];
            }
        }

        matris = yeniMatris;
        VertexOlustur();
    }

    // YENİ: Güvenli dönüş
    void Respawn::DondurGuvenlı(const std::vector<sf::FloatRect>& engeller) {
        if (Yerdemi) return;

        // Eski matrisi sakla
        auto eskiMatris = matris;

        int satir = matris.size();
        int sutun = matris[0].size();

        std::vector<std::vector<int>> yeniMatris(sutun, std::vector<int>(satir));

        for (int i = 0; i < satir; i++) {
            for (int j = 0; j < sutun; j++) {
                yeniMatris[j][satir - 1 - i] = matris[i][j];
            }
        }

        matris = yeniMatris;

        // Döndükten sonra çarpışma kontrolü
        if (CarpismaKontrol(engeller, 0, 0)) {
            // Çarpışma varsa geri al
            matris = eskiMatris;
        }

        VertexOlustur();
    }

    void Respawn::TusCd() {
        if (Tus) {
            if (TusSaat.getElapsedTime().asSeconds() >= cd) {
                Tus = false;
                TusSaat.restart();
            }
        }
    }

    // Sekil.h dosyası içinde, class Sekil'in public bölümünde:

        // YENİ: Düzeltilmiş ve sadeleştirilmiş Gravity fonksiyonu
    void Respawn::gra(std::vector<sf::FloatRect> duvar,float dt) {
        if (Yerdemi) return;

        // Main'den gelen delta time'ı (dt) mevcut sayaca ekle
        dusmeTimer += dt;

        // Eğer sayaç, düşme hızını geçtiyse (örneğin 0.45 saniye)
        if (dusmeTimer >= dusmeHizi)
        {
            dusmeTimer = 0.0f; // Sayacı sıfırla

            float step = bloklukBoyut + 1.0f; // Bir adımın boyutu (16 + 1 = 17 piksel)

            // 1. Gelecekteki pozisyonu (bir adım aşağısını) kontrol et
            if (CarpismaKontrol(duvar, 0, step)) {
                // Eğer aşağıda engel varsa:
              Isaretle();              // Bloğu olduğu yere çivile
               RastgeleSekilOlustur();  // Yeni şekil yarat
                VertexOlustur();         // Yeni şekli çiz
                return;                  // Fonksiyondan çık
            }

            // 2. Eğer engel yoksa şekli bir adım aşağı indir
            y += step;
            VertexOlustur(); // Görünümü güncelle
        }
    }



    void Respawn::Isaretle() {
        // Her bloğu ayrı ayrı Doluvec'e ekle ve Dolu matrisine kaydet
        for (size_t i = 0; i < matris.size(); i++) {
            for (size_t j = 0; j < matris[i].size(); j++) {
                if (matris[i][j] == 1) {
                    // Bloğun gerçek koordinatları
                    float blokX = x + j * (bloklukBoyut + 1);
                    float blokY = y + i * (bloklukBoyut + 1);

                    // Dolu matrisindeki pozisyonu hesapla
                    // ArkaP matrisine göre: j=0 sol duvar, j=1-10 oyun alanı, j=11 sağ duvar
                    int satir = static_cast<int>((blokY - 100) / (bloklukBoyut + 1));
                    int sutun = static_cast<int>((blokX - 200) / (bloklukBoyut + 1)); // -1 çünkü j=1'den başlar

                    // Dolu matrisine işaretle (0-20 satır, 0-9 sütun)
                    if (satir >= 0 && satir < static_cast<int>(Dolu.size()) &&
                        sutun >= 0 && sutun < static_cast<int>(Dolu[0].size())) {
                        Dolu[satir][sutun] = 1;

                        // Bloğun matristeki pozisyonundan gerçek ekran koordinatlarını hesapla
                        // ArkaP matrisinde j=1'den başlıyor (j=0 sol duvar)
                        blokX = 200 + (sutun + 1) * (bloklukBoyut + 1);
                        blokY = 100 + satir * (bloklukBoyut + 1);
                    }

                    // Bu blok için ayrı bir VertexArray oluştur
                    sf::VertexArray tekBlok(sf::PrimitiveType::Triangles);

                    // Bloğun rengi (beyaz yap)
                    sf::Color blokRenk = sf::Color::White;

                    // 6 vertex ekle (2 üçgen = 1 kare)
                    sf::Vertex v1;
                    v1.position = { blokX, blokY };
                    v1.color = blokRenk;
                    tekBlok.append(v1);

                    sf::Vertex v2;
                    v2.position = { blokX + bloklukBoyut, blokY };
                    v2.color = blokRenk;
                    tekBlok.append(v2);

                    sf::Vertex v3;
                    v3.position = { blokX, blokY + bloklukBoyut };
                    v3.color = blokRenk;
                    tekBlok.append(v3);

                    sf::Vertex v4;
                    v4.position = { blokX + bloklukBoyut, blokY };
                    v4.color = blokRenk;
                    tekBlok.append(v4);

                    sf::Vertex v5;
                    v5.position = { blokX + bloklukBoyut, blokY + bloklukBoyut };
                    v5.color = blokRenk;
                    tekBlok.append(v5);

                    sf::Vertex v6;
                    v6.position = { blokX, blokY + bloklukBoyut };
                    v6.color = blokRenk;
                    tekBlok.append(v6);

                    // Bu bloğu Doluvec'e ekle
                    Doluvec.push_back(tekBlok);
                }
            }
        }

        // Satır silme kontrolü
        Sil();

        // Şekli yukarı taşı (yeni şekil için)
        x = 216;
        y = 100;
    }

    // YENİ: Blok bazlı çarpma






    // YENİ: Yan çarpışma kontrolü
    bool Respawn::yan(std::vector<sf::FloatRect> duvar) {
        if (Yerdemi) return true;
        return CarpismaKontrol(duvar);
    }


    

void Respawn::Sil() {
        // Eğer efekt aktifse satır silme işlemini yapma
        if (efektAktif) return;

        // Önce tüm dolu satırları bul
        std::vector<int> doluSatirlar;

        for (int i = 19; i >= 0; i--) {
            
            bool satirDolu = true;
            if (i >= Dolu.size()) continue;
            for (int j = 0; j < Dolu[i].size(); j++) {
                if (Dolu[i][j] == 0) {
                    satirDolu = false;
                        break;
                }
            }

            if (satirDolu) {
                doluSatirlar.push_back(i);
            }
        }

        // Eğer dolu satır varsa efekt başlat
        if (!doluSatirlar.empty()) {
            SilinenS += doluSatirlar.size(); // Toplam silinen sayısını güncelle

            // Tüm dolu satırlar için efekt başlat
            yonetici->DurumEkle(std::make_unique<Effect>(yonetici, doluSatirlar, 200, 100));
        }
    }

    // SatirSilmeyiTamamla fonksiyonunu güncelle - DÜZELTİLMİŞ VERSİYON:
void Respawn::SatirSilmeyiTamamla(const std::vector<int>& satirIndexler) {
    // 1. Satırları sırala
    std::vector<int> siraliSatirlar = satirIndexler;
    std::sort(siraliSatirlar.begin(), siraliSatirlar.end());

    // 2. Silinen satırları matriste temizle (0 yap)
    for (int satirIndex : siraliSatirlar) {
        for (int j = 0; j < 10; j++) {
            Dolu[satirIndex][j] = 0;
        }
    }

    // 3. YENİ MATRİS OLUŞTUR VE KAYDIRMA MANTIĞI
    std::vector<std::vector<int>> yeniDolu(21, std::vector<int>(10, 0));

    // DÜZELTME BURADA: 
    // Kaydırma işlemine 20'den (Dolu.size()-1) DEĞİL, 19'dan başlıyoruz.
    // Çünkü 20. satır "Zemin"dir ve oraya blok taşınmamalıdır.
    int hedefSatir = 19;

    // Okumaya da 19. satırdan başlıyoruz
    for (int i = 19; i >= 0; i--) {

        // Bu satırın tamamen boş olup olmadığını kontrol et
        bool bosmu = true;
        for (int j = 0; j < 10; j++) {
            if (Dolu[i][j] == 1) {
                bosmu = false;
                break;
            }
        }

        // Eğer satır boş değilse, hedef satıra kopyala ve hedefi bir yukarı kaydır
        if (!bosmu) {
            for (int j = 0; j < 10; j++) {
                yeniDolu[hedefSatir][j] = Dolu[i][j];
            }
            hedefSatir--;
        }
        // Eğer satır boşsa (silinmişse veya zaten boşsa), kopyalamıyoruz.
        // Böylece yukarıdaki dolu satırlar aşağıya (boşluğa) kaymış oluyor.
    }

    // 4. Eski matrisi yenisiyle değiştir
    Dolu = yeniDolu;

    // 5. Görseli güncelle
    YenidenDoluvecOlustur();
}

    // Yeni yardımcı fonksiyon - Doluvec'i sıfırdan oluştur:
    void Respawn::YenidenDoluvecOlustur() {
        Doluvec.clear();

        float bloklukBoyut = 16.0f;

        for (int i = 0; i < Dolu.size()-1; i++) {
            for (int j = 0; j < Dolu[i].size(); j++) {
                
                if (Dolu[i][j] == 1) {
                    // Bu bloğun ekran koordinatlarını hesapla
                    float blokX = 200 + (j + 1) * (bloklukBoyut + 1);
                    float blokY = 100 + i * (bloklukBoyut + 1);
                    // Bu blok için ayrı bir VertexArray oluştur
                    sf::VertexArray tekBlok(sf::PrimitiveType::Triangles);
                    sf::Color blokRenk = sf::Color::White;

                    // 6 vertex ekle (2 üçgen = 1 kare)
                    sf::Vertex v1;
                    v1.position = { blokX, blokY };
                    v1.color = blokRenk;
                    tekBlok.append(v1);

                    sf::Vertex v2;
                    v2.position = { blokX + bloklukBoyut, blokY };
                    v2.color = blokRenk;
                    tekBlok.append(v2);

                    sf::Vertex v3;
                    v3.position = { blokX, blokY + bloklukBoyut };
                    v3.color = blokRenk;
                    tekBlok.append(v3);

                    sf::Vertex v4;
                    v4.position = { blokX + bloklukBoyut, blokY };
                    v4.color = blokRenk;
                    tekBlok.append(v4);

                    sf::Vertex v5;
                    v5.position = { blokX + bloklukBoyut, blokY + bloklukBoyut };
                    v5.color = blokRenk;
                    tekBlok.append(v5);

                    sf::Vertex v6;
                    v6.position = { blokX, blokY + bloklukBoyut };
                    v6.color = blokRenk;
                    tekBlok.append(v6);

                    Doluvec.push_back(tekBlok);
                }
            }
        }
    }

  

    void Respawn::SatirlariKaydir(int silinenSatir)
    {
        std::vector<sf::VertexArray> yeniDoluvec;

        for (auto& va : Doluvec)
        {
            for (int i = 0; i < va.getVertexCount(); i += 6)
            {
                float minY = va[i].position.y;
                float maxY = va[i].position.y;

                for (int k = 1; k < 6; k++)
                {
                    minY = std::min(minY, va[i + k].position.y);
                    maxY = std::max(maxY, va[i + k].position.y);
                }

                int blokSatiri = (int)((minY - 100) / 17);

                //  Silinen satırdaki blok -> ekleme
                if (blokSatiri == silinenSatir)
                {
                    continue;
                }

                //  Blok silinen satırın üstündeyse 1 satır aşağı indir
                sf::VertexArray yeniVA(sf::PrimitiveType::Triangles);

                for (int k = 0; k < 6; k++)
                {
                    sf::Vertex v = va[i + k];
                    if (blokSatiri < silinenSatir)
                    {
                        v.position.y += 17;
                    }
                    yeniVA.append(v);
                }

                yeniDoluvec.push_back(yeniVA);
            }
        }

        Doluvec = yeniDoluvec;
    }
   
    void Respawn::GameOver() {
        if (Tus)return;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
            Tus = true;
            Doluvec.clear();
            Sekilvec.clear();
            for (size_t i = 0; i < Dolu.size(); i++) {
                for (size_t j = 0; j < Dolu[i].size(); j++) {
                    Dolu[i][j] = 0;
                }
            }
            RastgeleSekilOlustur();
            x = 216;
            y = 100;
            VertexOlustur();
            TusSaat.restart();
            SilinenS = 0;

        }
        bool xx = false;
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < Dolu[i].size(); j++) {

                if (Dolu[i][j] == 1)
                    xx = true;

            }
        }

        if (xx) {
            Doluvec.clear();
            Sekilvec.clear();
            SilinenS = 0;
            for (size_t i = 0; i < Dolu.size(); i++) {
                for (size_t j = 0; j < Dolu[i].size(); j++) {
                    Dolu[i][j] = 0;
                }
            }
            RastgeleSekilOlustur();
            x = 216;
            y = 100;
            VertexOlustur();
        }
    }


    void Respawn::BilgiEkrani() {
        std::string metin = "0";

        metin = std::to_string(SilinenS);
        text.setString(metin);
    }









    

void Respawn::Gir() {
    text.setFillColor(sf::Color::White);
    text.setPosition({ 450.f, 50.f });
    Respawn::RastgeleSekilOlustur();

    Respawn::VertexOlustur();
    // yonetici->DurumEkle(std::make_unique<ArkaP>(yonetici));
}
void Respawn::Cik() {};
void Respawn::Guncelle(float dt) {
    std::vector<sf::FloatRect> tumEngeller = ArkaP::Engel;


    // Dolu blokları da ekle
    for (const auto& doluBlok : Doluvec) {
        // Her dolu şeklin bloklarını ayrı ayrı ekle
        for (size_t i = 0; i < doluBlok.getVertexCount(); i += 6) {
            if (i + 5 < doluBlok.getVertexCount()) {
                sf::Vector2f pos = doluBlok[i].position;
                tumEngeller.push_back(sf::FloatRect({ pos.x, pos.y }, { bloklukBoyut, bloklukBoyut }));
            }
        }
    }
    Respawn::hareketGuvenlı(tumEngeller,dt);
    Respawn::gra(tumEngeller,dt);
    Respawn::GameOver();
    Respawn::BilgiEkrani();
};
void Respawn::Ciz(sf::RenderWindow& pencere) {
   // pencere.clear(sf::Color::Black);
    for (auto s : Respawn::Sekilvec)
        pencere.draw(s);
    for (auto s : Respawn::Doluvec)
        pencere.draw(s);
    pencere.draw(text);

}
void Respawn::GirdiIsle(const std::optional<sf::Event>& event) {}
