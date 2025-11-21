// MenuDurumu.cpp
#include "MenuDurumu.h"
#include "DurumYönetici.h"
#include "ArkaP.h"


MenuDurumu::MenuDurumu(DurumYoneticisi* yonetici) : yonetici(yonetici), font("arial.ttf"), baslik(font)
, oynaButon(font), cikisButon(font)
{
}

void MenuDurumu::Gir() {
    if (!font.openFromFile("arial.ttf")) {
        // Hata
    }

    
    baslik.setString("TETRIS");
    baslik.setCharacterSize(50);
    baslik.setPosition({ 300.f, 100.f });

    
    oynaButon.setString("> Oyna");
    oynaButon.setCharacterSize(30);
    oynaButon.setPosition({ 300.f, 250.f });

    
    cikisButon.setString("  Cikis");
    cikisButon.setCharacterSize(30);
    cikisButon.setPosition({ 300.f, 300.f });
}

void MenuDurumu::Cik() {
    //yonetici->DurumCikar();

}

void MenuDurumu::Guncelle(float dt) {}

void MenuDurumu::Ciz(sf::RenderWindow& pencere) {
    pencere.clear(sf::Color::Black);
    pencere.draw(baslik);
    pencere.draw(oynaButon);
    pencere.draw(cikisButon);
}

void MenuDurumu::GirdiIsle(const std::optional<sf::Event>& event) {
    if (!event) return;
   

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Up) {
            secilenIndex = 0;
            oynaButon.setString("> Oyna");
            cikisButon.setString("  Cikis");
        }
        else if (keyPressed->code == sf::Keyboard::Key::Down) {
            secilenIndex = 1;
            oynaButon.setString("  Oyna");
            cikisButon.setString("> Cikis");
        }
        else if (keyPressed->code == sf::Keyboard::Key::Enter) {
            if (secilenIndex == 0) {
                
                yonetici->DurumuUsteEkle(std::make_unique<ArkaP>(yonetici));
                
            }
            else {
                while (!yonetici->Bos()) {
                    yonetici->DurumCikar();
                }
            }
        }
    }
}

