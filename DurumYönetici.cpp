//DurumYoneticisi.cpp
#include "DurumYönetici.h"

void DurumYoneticisi::DurumEkle(std::unique_ptr<OyunDurum> yeniDurum) {
    if (!durumlar.empty()) {
        durumlar.back()->Cik();
    }
    durumlar.push_back(std::move(yeniDurum));
    durumlar.back()->Gir();
}

void DurumYoneticisi::DurumCikar() {
    if (!durumlar.empty()) {
        durumlar.back()->Cik();
        durumlar.pop_back();
        if (!durumlar.empty()) {
            durumlar.back()->Gir();
        }
    }
}

void DurumYoneticisi::Guncelle( float dt) {
    // Sadece en üstteki durumu güncelle
    if (!durumlar.empty()) {
        durumlar.back()->Guncelle(dt);
    }
}

void DurumYoneticisi::Ciz(sf::RenderWindow& pencere) {
    // TÜM durumları alttan üste doğru çiz!
    for (const auto& durum : durumlar) {
        durum->Ciz(pencere);
        
    }
}

void DurumYoneticisi::GirdiIsle(const std::optional<sf::Event>& event) {
    // Sadece en üstteki durum girdileri işlesin
    if (!durumlar.empty()) {
        durumlar.back()->GirdiIsle(event);
    }
}

bool DurumYoneticisi::Bos() const {
    return durumlar.empty();
}

void DurumYoneticisi::DurumuUsteEkle(std::unique_ptr<OyunDurum> yeniDurum) {
    // Üst durumun Cik() metodunu ÇAĞIRMA - arkaplandaki durum aktif kalsın
   
        if (!durumlar.empty()) {
            durumlar.back()->Cik();
            durumlar.pop_back();  // ← BU EKLENDİ
        }
        durumlar.push_back(std::move(yeniDurum));
        durumlar.back()->Gir();
    
}