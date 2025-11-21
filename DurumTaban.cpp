//DurumTaban.cpp
#include <iostream>
#include <memory>
#include <stack>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "DurumYönetici.h"
#include "MenuDurumu.h"

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");
    window.setFramerateLimit(60);
    
    DurumYoneticisi durumYoneticisi;
    durumYoneticisi.DurumEkle(std::make_unique<MenuDurumu>(&durumYoneticisi));

    sf::Clock clock;
    // Start the game loop
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        


        // Dolu blokları da ekle
      
        // Process events
        while (auto event = window.pollEvent()) {
            
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            durumYoneticisi.GirdiIsle(event);
        }

        durumYoneticisi.Guncelle(dt);

        window.clear();
        durumYoneticisi.Ciz(window);
        window.display();
        
    }
}