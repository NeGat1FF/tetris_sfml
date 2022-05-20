#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <string>
#include "iconarray.cpp"
#include "blocksarray.cpp"
#include "fontarray.cpp"
#include "block.h"


int getRandomNumber(int min, int max) { // random number generator
    static std::random_device rd;
    static std::ranlux48 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

std::string secondsToString(int seconds) { //Convert seconds to string
    int minutes = seconds / 60;
    int seconds_left = seconds % 60;
    std::string minutes_str = std::to_string(minutes);
    std::string seconds_str = std::to_string(seconds_left);
    if (minutes < 10) {
        minutes_str = "0" + minutes_str;
    }
    if (seconds_left < 10) {
        seconds_str = "0" + seconds_str;
    }
    return minutes_str + ":" + seconds_str;
}

int score = 0;
int level = 1;
int line = 0;
int lines_per_level = 10;
int lines_cleared = 0;
float speed = 0.5;
int seconds = 0;
bool game_over = false;

int field[10][20] = {0};

bool checkCollision(const block &bl) { //Check if block collides with the field
    for (int i = 0; i < 4; i++) {
        if (bl.tetromino[i].getPosition().x < 0 || bl.tetromino[i].getPosition().x > 321 - 32 ||
            bl.tetromino[i].getPosition().y > 640 - 32 || field[(int) bl.pos(i).x][(int) bl.pos(i).y])
            return true;
    }
    return false;
}

void checkLine() { //Check if line is full and clear it
    int k = 19;
    for (int i = 19; i > 0; i--) {
        int count = 0;
        for (auto &j: field) {
            if (j[i]) count++;
            j[k] = j[i];
        }
        if (count < 10) {
            k--;
        } else {
            line++;
            lines_cleared++;
            score += 100;
        }
    }

    if (lines_cleared == lines_per_level) {
        level++;
        if(speed != 0.1) speed -= 0.05;
        lines_cleared = 0;
    }
}

void generateNew(block &bl, block &next) { //Generate new block and next block
    bl.move(move_direction::up);
    for (int i = 0; i < 4; i++) {
        field[(int) bl.pos(i).x][(int) bl.pos(i).y] = bl.color + 1;
    }
    bl = block(next.color);
    bl.move(move_direction::right, 32 * 4 + 1);
    next = block(getRandomNumber(0, 6));
    if (next.color == 0) {
        next.move(move_direction::right, 324); //If next is line piece, align it to the center
    } else {
        next.move(move_direction::right, 324 + 16);
    }
    next.move(move_direction::down, 450);
}

void
moveDown(block &bl, block &next, sf::Clock &clock) { //Move down and check for collision with the floor and other blocks
    if (clock.getElapsedTime().asSeconds() >= speed) {
        bl.move(move_direction::down);
        if (checkCollision(bl)) {
            generateNew(bl, next);
            if (checkCollision(bl)) { //If new block generates in other block, game over
                game_over = true;
            }
        }
        clock.restart();
    }
}

void resetGame(block &bl, block &next) { //Reset game
    score = 0;
    level = 1;
    line = 0;
    lines_per_level = 10;
    lines_cleared = 0;
    speed = 0.5;
    game_over = false;
    for (auto & i : field) {
        for (int & j : i) {
            j = 0;
        }
    }
    bl = block(getRandomNumber(0, 6));
    bl.move(move_direction::right, 32 * 4 + 1);
    next = block(getRandomNumber(0, 6));
    if (next.color == 0) {
        next.move(move_direction::right, 324); //If next is line piece, align it to the center
    } else {
        next.move(move_direction::right, 324 + 16);
    }
    next.move(move_direction::down, 450);
}


int main() {


    sf::RenderWindow window(sf::VideoMode(421, 642), "Tetris!", sf::Style::Close);

    sf::Image icon;
    icon.create(200, 200, iconArray);
    window.setIcon(200, 200, icon.getPixelsPtr());

    sf::Image blocks;
    blocks.create(256, 32, blocksArray);
    block::block_texture.loadFromImage(blocks);

    block bl(getRandomNumber(0, 6));
    bl.move(move_direction::right, 32 * 4 + 1);

    block next_bl(getRandomNumber(0, 6));
    if (next_bl.color == 0) {
        next_bl.move(move_direction::right, 324);
    } else {
        next_bl.move(move_direction::right, 324 + 16);
    }
    next_bl.move(move_direction::down, 450);


    sf::VertexArray lines(sf::LinesStrip, 7);
    lines[0].position = sf::Vector2f(321, 0);
    lines[1].position = sf::Vector2f(1, 0);
    lines[2].position = sf::Vector2f(1, 640);
    lines[3].position = sf::Vector2f(420, 640);
    lines[4].position = sf::Vector2f(420, 0);
    lines[5].position = sf::Vector2f(321, 0);
    lines[6].position = sf::Vector2f(321, 640);

    sf::Font font;
    font.loadFromMemory(BACKTO1982_TTF, BACKTO1982_TTF_len);

    sf::Text time;
    time.setFont(font);
    time.setFillColor(sf::Color::White);
    sf::Rect<float> textRect;


    sf::Clock clock;
    sf::Clock timer;

    while (window.isOpen()) {


        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                bl.move(move_direction::left);
                if (checkCollision(bl))
                    bl.move(move_direction::right);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                bl.move(move_direction::right);
                if (checkCollision(bl))
                    bl.move(move_direction::left);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                bl.move(move_direction::down);
                if (checkCollision(bl))
                    bl.move(move_direction::up);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
                block copy = bl;
                bl.rotate();
                if (checkCollision(bl))
                    bl = copy;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                while (!checkCollision(bl))
                    bl.move(move_direction::down);
                generateNew(bl, next_bl);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                resetGame(bl, next_bl);
                timer.restart();
            }
        }
        window.clear();

        time.setCharacterSize(25);

        time.setString("TIME");
        textRect = time.getLocalBounds();

        time.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        time.setPosition(sf::Vector2f(421 / 2.0f + 321 / 2.0f, 30));

        window.draw(time);


        if(!game_over)seconds = (int)timer.getElapsedTime().asSeconds();
        time.setString(secondsToString(seconds));
        textRect = time.getLocalBounds();

        time.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        time.setPosition(sf::Vector2f(421 / 2.0f + 321 / 2.0f, 70));

        window.draw(time);

        time.setCharacterSize(21);

        time.setString("LEVEL");
        textRect = time.getLocalBounds();

        time.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        time.setPosition(sf::Vector2f(421 / 2.0f + 321 / 2.0f, 140));

        window.draw(time);

        time.setString(std::to_string(level));
        textRect = time.getLocalBounds();

        time.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        time.setPosition(sf::Vector2f(421 / 2.0f + 321 / 2.0f, 180));

        window.draw(time);

        time.setString("LINES");
        textRect = time.getLocalBounds();

        time.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        time.setPosition(sf::Vector2f(421 / 2.0f + 321 / 2.0f, 250));

        window.draw(time);

        time.setString(std::to_string(line));
        textRect = time.getLocalBounds();

        time.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        time.setPosition(sf::Vector2f(421 / 2.0f + 321 / 2.0f, 290));

        window.draw(time);

        time.setString("SCORE");
        textRect = time.getLocalBounds();

        time.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        time.setPosition(sf::Vector2f(421 / 2.0f + 321 / 2.0f, 360));

        window.draw(time);

        time.setString(std::to_string(score));
        textRect = time.getLocalBounds();

        time.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        time.setPosition(sf::Vector2f(421 / 2.0f + 321 / 2.0f, 400));

        window.draw(time);


        if(!game_over){
            moveDown(bl, next_bl, clock);
            checkLine();
        }

        //Draw the field
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 20; j++) {
                if (field[i][j] != 0) {
                    sf::Sprite block;
                    block.setTexture(block::block_texture);
                    block.setPosition((float) i * 32, (float) j * 32);
                    block.setTextureRect(sf::IntRect((field[i][j] - 1) * 32, 0, 32, 32));
                    block.move(1, 0);
                    window.draw(block);
                }
            }
        }

        if(game_over){
            time.setCharacterSize(50);
            time.setString("GAME OVER");
            textRect = time.getLocalBounds();

            time.setOrigin(textRect.left + textRect.width / 2.0f,
                           textRect.top + textRect.height / 2.0f);
            time.setPosition(sf::Vector2f(421 / 2.0f, 621 / 2.0f));

//            sf::RectangleShape gameover(sf::Vector2f(386,56));
//            gameover.setFillColor(sf::Color::Black);
//            gameover.setOrigin(time.getOrigin());
//            gameover.setPosition(time.getPosition());
//            window.draw(gameover);

            window.draw(time);
        }

        //Draw the place where the block will be and the block itself
        block trans = bl;
        for (auto &i: trans.tetromino) {
            i.setTextureRect(sf::IntRect(7 * 32, 0, 32, 32));
        }
        while (!checkCollision(trans) && !game_over) {
            trans.move(move_direction::down);
        }
        trans.move(move_direction::up);
        for (int i = 0; i < 4; i++) {
            window.draw(bl.tetromino[i]);
            window.draw(trans.tetromino[i]);
            window.draw(next_bl.tetromino[i]);

        }

        window.draw(lines);


        window.display();
    }

    return 0;
}