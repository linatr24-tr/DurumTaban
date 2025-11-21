//ArkaP.cpp
#include "ArkaP.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "DurumYönetici.h"
#include "Respawn.h"

    ArkaP::ArkaP(DurumYoneticisi* yonetici) : Tilee(sf::PrimitiveType::Triangles), yonetici(yonetici)
    {

    }

    void ArkaP::GridEkle(float px, float py, float gen, float uz) {

        sf::Vertex v1; //dortgen sol ust k��e
        v1.position = { px, py };
        // v1.texCoords = { 0.f,0.f };
        v1.color = Renk;
        Tilee.append(v1);
        sf::Vertex v2; //dortgen sa� �st k��e
        v2.position = { px + gen,py };
        // v2.texCoords = { bloklukBoyut,0.f };
        v2.color = Renk;
        Tilee.append(v2);
        sf::Vertex v3; //dortgen sol alt kose
        v3.position = { px,py + uz };
        //  v3.texCoords = { 0.f,bloklukBoyut };
        v3.color = Renk;
        Tilee.append(v3); //buras� bir ��gen
        sf::Vertex v4;
        v4.position = { px + gen,py };
        // v4.texCoords = { bloklukBoyut,0.f };
        v4.color = Renk;
        Tilee.append(v4); //dortgen sa� �st 2. ��gen sa� �st k��e
        sf::Vertex v5;
        v5.position = { px + gen,py + uz };
        // v5.texCoords = { bloklukBoyut,bloklukBoyut };
        v5.color = Renk;
        Tilee.append(v5);
        sf::Vertex v6;
        v6.position = { px,py + uz };
        // v6.texCoords = { 0.f,bloklukBoyut };
        v6.color = Renk;
        Tilee.append(v6);
    }
    void ArkaP::VertexOlustur() {

        Tilee.clear();


        for (size_t i = 0; i < 21; i++) {
            for (size_t j = 0; j < 12; j++) {
                if (matris[i][j] == 1) {

                    GridEkle(
                        x + j * (bloklukBoyut + 1),
                        y + i * (bloklukBoyut + 1),
                        bloklukBoyut,
                        bloklukBoyut
                    );
                }
            }
        }

        Sekilvec.push_back(Tilee);
    }
    void ArkaP::Hesapla() {
        for (size_t i = 0; i < 21; i++) {  //alt arkaplan kareleri hesaplar
            for (size_t j = 0; j < 12; j++) {
                if (j == 0) continue;
                if (j == 12) continue;
                if (matris[i][j] == 1) {

                    float px = x + static_cast<float>(j) * (bloklukBoyut + 1);
                    float py = y + static_cast<float>(i) * (bloklukBoyut + 1);
                    sf::FloatRect x({ px, py }, { bloklukBoyut, bloklukBoyut });
                    Engel.push_back(x);
                }
            }
        }
        for (size_t i = 0; i < 21; i++) { //Yandaki arkaplan kareleri hesaplar
            for (size_t j = 0; j < 12; j++) {
                if (j == 0 || j == 11) {
                    if (matris[i][j] == 1) {

                        float px = x + static_cast<float>(j) * (bloklukBoyut + 1);
                        float py = y + static_cast<float>(i) * (bloklukBoyut + 1);
                        sf::FloatRect x({ px, py }, { bloklukBoyut, bloklukBoyut });
                        Engel.push_back(x);
                    }
                }
            }
        }
    }



    
    void ArkaP::Gir() {
        ArkaP::VertexOlustur ();
        ArkaP::Hesapla();
        yonetici->DurumEkle(std::make_unique<Respawn>(yonetici));

    };
    void ArkaP::Cik() {};
    void ArkaP::Guncelle(float dt) {};
    void ArkaP::Ciz(sf::RenderWindow& pencere) {
       // pencere.clear(sf::Color::Black);
        for(auto s: ArkaP::Sekilvec)
        pencere.draw(s);
        
    };
    void ArkaP::GirdiIsle(const std::optional<sf::Event>& event) {};
