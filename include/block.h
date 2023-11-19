#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

enum class move_direction {
    up,
    left,
    right,
    down
};

const int figures[7][4] = {
        1,3,5,7, //I
        1,3,2,5, //T
        2,4,6,7, //L
        1,3,5,4, //J
        1,3,2,4, //Z
        0,2,3,5, //S
        0,1,2,3, //O
};



// class of tetris block
class block
{
public:
    explicit block(int t, sf::Texture &block_texture);
    sf::RectangleShape tetromino[4];
    void rotate();
    void move(move_direction dir, int o = 32);
    int color;
    //static sf::Texture block_texture;
    sf::Vector2<float> pos(int i) const;
};

#endif // BLOCK_H
