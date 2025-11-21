// MenuDurumu.h
#pragma once
#include "OyunDurum.h"
#include <SFML/Graphics.hpp>

class DurumYoneticisi;

class MenuDurumu : public OyunDurum {
private:
    DurumYoneticisi* yonetici;
    sf::Font font;
    sf::Text baslik;
    sf::Text oynaButon;
    sf::Text cikisButon;
    int secilenIndex = 0;

public:
    MenuDurumu(DurumYoneticisi* yonetici);

    void Gir() override;
    void Cik() override;
    void Guncelle(float dt) override;
    void Ciz(sf::RenderWindow& pencere) override;
    void GirdiIsle(const std::optional<sf::Event>& event) override;
};