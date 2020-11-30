//
//  main.cpp
//  Tile Map Class Development
//
//  Created by Vitality Edge on 11/29/20.
//  Copyright © 2020 Vitality Edge. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <iostream>

#include "TileMap.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(60);
    
    sf::Texture tile_set;
    if(!tile_set.loadFromFile("tile set.png")) return (-1);
    
    TileMap tile_map(sf::Vector2f(640, 640), sf::Vector2f(16, 16));
    
    tile_map.append_tile_set(tile_set);
    
    for(int i = 0; i < 100; i++)
        tile_map.set_cell(i, 4, 24);
    
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color(70, 70, 70));
        
        window.draw(tile_map);
        
        window.display();
    }
    
    return 0;
}
