// Effect.h - Güncelleme:
#pragma once
#include "OyunDurum.h"
#include <SFML/Graphics.hpp>
#include <vector>
class DurumYoneticisi;

class Effect : public OyunDurum
{
private:
    DurumYoneticisi* yonetici;
    float zamanlayici;
    float suresi;
    int yanipSonmeSayisi;
    int mevcutYanip;
    bool gorunur;
    std::vector<sf::RectangleShape> satirBloklar;
    std::vector<int> silinenSatirIndexler; // TEK satýr yerine ÇOKLU satýr

public:
    Effect(DurumYoneticisi* yonetici, const std::vector<int>& satirIndexler, float px, float py);
    void Gir() override;
    void Cik() override;
    void Guncelle(float dt) override;
    void Ciz(sf::RenderWindow& pencere) override;
    void GirdiIsle(const std::optional<sf::Event>& event) override;
};