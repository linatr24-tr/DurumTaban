


#include "Effect.h"
#include "Respawn.h"
#include "ArkaP.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "DurumYönetici.h"

Effect::Effect(DurumYoneticisi* yonetici, const std::vector<int>& satirIndexler, float px, float py)
    : yonetici(yonetici),
    zamanlayici(0.0f),
    suresi(0.5f),
    yanipSonmeSayisi(3),
    mevcutYanip(0),
    gorunur(true),
    silinenSatirIndexler(satirIndexler)
{
    // Tüm silinen satýrlarýn bloklarýný oluþtur
    float bloklukBoyut = 16.0f;

    for (int satirIndex : satirIndexler) {
        for (int j = 0; j < 10; j++) {
            sf::RectangleShape blok({ bloklukBoyut, bloklukBoyut });
            blok.setPosition({ px + (j + 1) * (bloklukBoyut + 1), py + satirIndex * (bloklukBoyut + 1) });
            blok.setFillColor(sf::Color::Red); // Efekt rengi
            satirBloklar.push_back(blok);
        }
    }
}

void Effect::Gir() {
    
    Respawn::efektAktif = true;
}

void Effect::Cik() {
    

    // Efekt bittiðinde tüm satýrlarý gerçekten sil
    if (Respawn::aktifInstance) {
        Respawn::aktifInstance->SatirSilmeyiTamamla(silinenSatirIndexler);
    }
    Respawn::efektAktif = false;

    // Baþka dolu satýr var mý kontrol et
    if (Respawn::aktifInstance) {
        Respawn::aktifInstance->Sil();
    }
}

void Effect::Guncelle(float dt) {
    zamanlayici += dt;

    // Her 0.1 saniyede bir yanýp sönme
    if (zamanlayici >= 0.1f) {
        gorunur = !gorunur;
        zamanlayici = 0.0f;

        if (gorunur) {
            mevcutYanip++;
        }
    }

    // Efekt tamamlandý mý?
    if (mevcutYanip >= yanipSonmeSayisi) {
        yonetici->DurumCikar();
    }
}

void Effect::Ciz(sf::RenderWindow& pencere) {
    if (gorunur) {
        for (auto& blok : satirBloklar) {
            // Alpha deðeri ile solma efekti
            sf::Color renk = blok.getFillColor();
            float alpha = 255.0f * (1.0f - (float)mevcutYanip / yanipSonmeSayisi);
            renk.a = static_cast<unsigned char>(alpha);
            blok.setFillColor(renk);

            pencere.draw(blok);
        }
    }
}

void Effect::GirdiIsle(const std::optional<sf::Event>& event) {
    // Efekt sýrasýnda input almýyoruz
}