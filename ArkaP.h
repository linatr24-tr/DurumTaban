//ArkaP.h
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "OyunDurum.h"
class DurumYoneticisi;
class ArkaP: public OyunDurum
{
private:
    DurumYoneticisi* yonetici;

public:

    sf::VertexArray Tile;
    sf::VertexArray Tilee;
    float resG = 32.f;
    float resY = 32.f;
    sf::FloatRect engel;
    sf::Color Renk = sf::Color::Blue;
    float bloklukBoyut = 16;
    float x = 200;
    float y = 100;
    std::vector<sf::VertexArray> Sekilvec;
    static std::vector<sf::FloatRect> Engel;

    int ax = 2;
    int ay = 3;
    unsigned char matris[21][12] = {
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };


    ArkaP(DurumYoneticisi* yonetici);

    void GridEkle(float px, float py, float gen, float uz);
    void VertexOlustur();
    void Hesapla();
    void Gir() override;
    void Cik() override;
    void Guncelle(float dt) override;
    void Ciz(sf::RenderWindow& pencere) override;
    void GirdiIsle(const std::optional<sf::Event>& event) override;




};
