//Respawn.h
#pragma once
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "OyunDurum.h"

class DurumYoneticisi;
class Respawn:public OyunDurum
{
private:
    static std::random_device rd;
    static std::mt19937 gen;
    static sf::Clock Saat;
    sf::Clock TusSaat;
    
    DurumYoneticisi* yonetici;


public:
    sf::Font font;
    sf::Text text;
    float dusmeTimer = 0.0f;
    float dusmeHizi = 0.45f; // 0.45 saniyede bir 17 piksel
    sf::VertexArray Tile;
    float bloklukBoyut = 16.f;
    std::vector<std::vector <int>> matris;
    sf::Color Renk;
    float x = 216;
    float y = 100;
    std::vector<sf::VertexArray> Sekilvec;
    std::vector<sf::VertexArray> Doluvec;
    std::vector<std::vector<int>>       Dolu = {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//0
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//1
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//2
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//3
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//4
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//5
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//6
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//7
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//8
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//9
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//10
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//11
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//12
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//13
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//14
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//15
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//16
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//17
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//18
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//19
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //20
    };

    bool Tus = false;
    bool Yerdemi = false;
    float cd = 0.2f;
    int SilinenS = 0;
    sf::Clock Grracd;
    float yatayTimer = 0.0f;
    float yatayCd = 0.15f;

    Respawn(DurumYoneticisi* yonetici);

    
    std::vector<sf::FloatRect> BlokBoundslariniGetir(float offsetX, float offsetY) const;
    // Geliþtirilmiþ çarpýþma kontrolü - offset parametreli
    bool CarpismaKontrol(const std::vector<sf::FloatRect>& engeller, float offsetX, float offsetY) const;
       



    int myrand(int min, int max);

    void DortgenEkle(float px, float py, float gen, float uz);


    void VertexOlustur();

    void ISekilOlustur();

    void OSekilOlustur();

    void TSekilOlustur();

    void SSekilOlustur();

    void ZSekilOlustur();

    void JSekilOlustur();

    void LSekilOlustur();

    void RastgeleSekilOlustur();



    // YENÝ: Tüm engelleri kabul eden hareket fonksiyonu
    void hareketGuvenlý(const std::vector<sf::FloatRect>& duvarlar,
         float dt);

    void Dondur();

    // YENÝ: Güvenli dönüþ
    void DondurGuvenlý(const std::vector<sf::FloatRect>& engeller);

    void TusCd();

    // Sekil.h dosyasý içinde, class Sekil'in public bölümünde:

        // YENÝ: Düzeltilmiþ ve sadeleþtirilmiþ Gravity fonksiyonu
    void gra(std::vector<sf::FloatRect> duvar, float dt);
               



    void Isaretle();








    // YENÝ: Yan çarpýþma kontrolü
    bool yan(std::vector<sf::FloatRect> duvar);
    void Sil();

    void SatirlariKaydir(int silinenSatir);
    void BilgiEkrani();
    void GameOver();
    void Gir() override;
    void Cik() override;
    void Guncelle(float dt) override;
    void Ciz(sf::RenderWindow& pencere) override;
    void GirdiIsle(const std::optional<sf::Event>& event) override;





};