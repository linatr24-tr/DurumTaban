// DurumYoneticisi.h
#pragma once
#pragma once
#include <memory>
#include <stack>
#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>
#include "OyunDurum.h"

class DurumYoneticisi {
private:
    std::vector<std::unique_ptr<OyunDurum>> durumlar;

public:
    void DurumEkle(std::unique_ptr<OyunDurum> yeniDurum);

    void DurumCikar();
    void DurumuUsteEkle(std::unique_ptr<OyunDurum> yeniDurum);


    void Guncelle(float dt);

    void Ciz(sf::RenderWindow& pencere);

    void GirdiIsle(const std::optional<sf::Event>& event);

    bool Bos() const;
};