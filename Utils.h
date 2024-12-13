#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>


/*
    This function sets up a sf::Text object,
    sets font, character size, color, position on the 
    screen and text. Returns a pointer on completed sf::Text object.
*/
std::shared_ptr<sf::Text> setUpTextObject(const sf::Font &font,
                                          int char_size,
                                          sf::Color color, 
                                          sf::Vector2i position,
                                          const std::string &text = "");

