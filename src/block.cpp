#include "block.h"

block::block(int t, sf::Texture &block_texture){

    for(int i = 0;i < 4;i++){
        tetromino[i].setSize(sf::Vector2f(32,32));
        tetromino[i].move(float((int)figures[t][i] % 2 * 32),float((int)figures[t][i] / 2 * 32));
        tetromino[i].setTexture(&block_texture);
        tetromino[i].setTextureRect(sf::IntRect(t * 32,0,32,32));
    }

    color = t;
}


//sf::Texture block::block_texture;


void block::move(move_direction dir,int o){
    float x = dir == move_direction::left ? -1.f : dir == move_direction::right ? 1.f : 0.f;
    float y = dir == move_direction::down ? 1.f : dir == move_direction::up ? -1.f : 0.f;
    for(auto & i : tetromino){
        i.move(x * (float)o,y * (float)o);
    }
}

void block::rotate() {
    if(color != 6){
        float pX = tetromino[1].getPosition().x;
        float pY = tetromino[1].getPosition().y;
        for(auto & i : tetromino){
            float x = i.getPosition().y - pY;
            float y = i.getPosition().x - pX;
            i.setPosition(pX - x,pY + y);
        }
    }
}

sf::Vector2<float> block::pos(int i) const{
    sf::Vector2<float> pos;
    pos.x = tetromino[i].getPosition().x / 32;
    pos.y = tetromino[i].getPosition().y / 32;
    return pos;
}
