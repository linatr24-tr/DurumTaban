// OyunDurum.h
#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
class OyunDurum
{
public:
    virtual ~OyunDurum() = default;
    virtual void Gir() = 0;
    virtual void Cik() = 0;
    virtual void Guncelle(float dt) = 0;
    virtual void Ciz(sf::RenderWindow& pencere) = 0;
    virtual void GirdiIsle(const std::optional<sf::Event>& event) = 0;

};