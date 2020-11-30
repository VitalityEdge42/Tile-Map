//
//  TileMap.h
//  Tile Map Class Development
//
//  Created by Vitality Edge on 11/29/20.
//  Copyright © 2020 Vitality Edge. All rights reserved.
//

#ifndef TileMap_h
#define TileMap_h

#include <SFML/Graphics.hpp>

struct Layout {
    int tile;
    bool flip_x;
    bool flip_y;
    
    Layout()
    {
        
    }
    Layout(int v_tile, bool v_flip_x, bool v_flip_y)
    {
        tile = v_tile;
        flip_x = v_flip_x;
        flip_y = v_flip_y;
    }
};


class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap(sf::Vector2f v_size, sf::Vector2f v_cell_size = sf::Vector2f(64, 64), sf::Vector2i v_chunk_size = sf::Vector2i(10, 10))
    {
        cell_size = v_cell_size;
        
        chunk_size = v_chunk_size;
        
        m_tile_map.create(v_size.x, v_size.y);
        
        m_tile_map.clear(sf::Color(0, 0, 0, 0));
    }
    TileMap(int width, int height)
    {
        m_tile_map.create(width, height);
    }
    void clear()
    {
        m_layout.resize(0);
    }
    int get_cell(int x, int y)
    {
        reallocate_if_needed(x, y);
        
        return m_layout.at(y).at(x).tile;
    }
    int get_cellv(sf::Vector2i position)
    {
        reallocate_if_needed(position.x, position.y);
        
        return m_layout.at(position.y).at(position.x).tile;
    }
    std::vector<sf::Vector2i> get_used_cells()
    {
        std::vector<sf::Vector2i> used_cells;
        
        for(int i = 0; i < m_layout.size(); i++)
        {
            for(int j = 0; j < m_layout.at(i).size(); j++)
            {
                if(m_layout.at(i).at(j).tile != -1)
                {
                    used_cells.push_back(sf::Vector2i(j, i));
                }
            }
        }
        return used_cells;
    }
    std::vector<sf::Vector2i> get_used_cells_by_id(int id)
    {
        std::vector<sf::Vector2i> used_cells;
        
        for(int i = 0; i < m_layout.size(); i++)
        {
            for(int j = 0; j < m_layout.at(i).size(); j++)
            {
                if(m_layout.at(i).at(j).tile == id)
                {
                    used_cells.push_back(sf::Vector2i(j, i));
                }
            }
        }
        return used_cells;
    }
    sf::Vector2i get_used_rect()
    {
        return sf::Vector2i((int)m_layout.at(0).size(), (int)m_layout.size());
    }
    bool is_cell_transposed(int x, int y)
    {
        reallocate_if_needed(x, y);
        
        return m_layout.at(y).at(x).flip_x || m_layout.at(y).at(x).flip_y;
    }
    bool is_cell_transposedv(sf::Vector2i position)
    {
        reallocate_if_needed(position.x, position.y);
        
        return m_layout.at(position.y).at(position.x).flip_x || m_layout.at(position.y).at(position.x).flip_y;
    }
    bool is_cell_x_flipped(int x, int y)
    {
        reallocate_if_needed(x, y);
        
        return m_layout.at(y).at(x).flip_x;
    }
    bool is_cell_y_flipped(int x, int y)
    {
        reallocate_if_needed(x, y);
        
        return m_layout.at(y).at(x).flip_y;
    }
    sf::Vector2i map_to_world(sf::Vector2i map_position, bool ignore_half_ofs)
    {
        return sf::Vector2i(map_position.x * cell_size.x, map_position.y * cell_size.y);
    }
    void set_cell(int x, int y, int tileNumber, bool flip_x = false, bool flip_y = false)
    {
        reallocate_if_needed(x, y);
        
        m_layout.at(y).at(x) = Layout(tileNumber, flip_x, flip_y);
        
        set_tile_map(x, y, tileNumber);
    }
    void set_cellv(sf::Vector2i position, int tileNumber, bool flip_x = false, bool flip_y = false)
    {
        reallocate_if_needed(position.x, position.y);
        
        m_layout.at(position.y).at(position.x) = Layout(tileNumber, flip_x, flip_y);
        
        set_tile_map(position.x, position.y, tileNumber);
    }
    void remove_tile(int x, int y)
    {
        reallocate_if_needed(x, y);
        
        m_layout.at(y).at(x) = Layout(-1, 0, 0);
        
        set_tile_map(x, y, -1, 0, 0);
    }
    sf::Vector2i world_to_map(sf::Vector2i world_position)
    {
        return sf::Vector2i(world_position.x / cell_size.x, world_position.y / cell_size.y);
    }
    void append_tile(sf::Texture &texture)
    {
        m_tiles.push_back((sf::Sprite)texture);
    }
    void append_tile_set(sf::Texture &texture)
    {
        int tile_width = texture.getSize().x / cell_size.x + (texture.getSize().x % (int)cell_size.x != 0);
        int tile_height = texture.getSize().y / cell_size.y + (texture.getSize().y % (int)cell_size.y != 0);
        
        for(int i = 0; i < tile_height; i++)
            for(int j = 0; j < tile_width; j++)
            {
                sf::Sprite tile(texture, sf::IntRect(j * cell_size.x, i * cell_size.y, cell_size.x, cell_size.y));
                
                m_tiles.push_back(tile);
            }
        sf::Sprite tile;
    }
    void print_id_layout()
    {
        for(int i = 0; i < m_layout.size(); i++)
        {
            std::cout << "{";
            for(int j = 0; j < m_layout.at(i).size(); j++)
            {
                std::cout << m_layout.at(i).at(j).tile << ", ";
            }
            std::cout << "}," << std::endl;
        }
    }
    sf::Vector2f cell_size;
    sf::Vector2i chunk_size;
    
private:
    void reallocate_if_needed(int x, int y)
    {
        if(y >= m_layout.size()) // reallocate y axis if out of range
        {
            y = (y / chunk_size.y + 1) * chunk_size.y;
            
            m_layout.resize(y + 1);
        }
        if(x >= m_layout.at(y).size()) // reallocate x axis if out of range
        {
            x = (x / chunk_size.x + 1) * chunk_size.x;
            
            for(int i = 0; i < m_layout.size(); i++) // loop through rows and reallocate its size
            {
                m_layout.at(i).resize(x + 1, Layout(-1, 0, 0));
            }
        }
    }
    void set_tile_map(int x, int y, int tileNumber, bool flip_x = false, bool flip_y = false)
    {
        m_tiles[tileNumber].setPosition(x * cell_size.x, y * cell_size.y);
        
        m_tile_map.draw(m_tiles[tileNumber]);
        
        m_tile_map.display();
    }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        
        states.transform *= getTransform();
        
        target.draw((sf::Sprite)m_tile_map.getTexture(), states);
        
    }
    std::vector<std::vector<Layout>> m_layout;
    std::vector<sf::Sprite> m_tiles;
    sf::RenderTexture m_tile_map;
};

#endif /* TileMap_h */
