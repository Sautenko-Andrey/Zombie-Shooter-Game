#include "Utils.h"

std::shared_ptr<sf::Text> setUpTextObject(const sf::Font &font,
                                          int char_size,
                                          sf::Color color,
                                          sf::Vector2i position,
                                          const std::string &text)
{
    auto sp_text_object = std::make_shared<sf::Text>();

    sp_text_object->setFont(font);
    sp_text_object->setCharacterSize(char_size);
    sp_text_object->setFillColor(color);
    sp_text_object->setPosition(position.x, position.y);
    if(!text.empty()){
        sp_text_object->setString(text);
    }
    

    return sp_text_object;
}
