#pragma once
#include <SFML/Graphics.hpp>
#include <map>


class TextureHolder {

public:

    TextureHolder();

    static sf::Texture& getTexture(const std::string &file_name);


private:

    std::map<std::string, sf::Texture> m_textures;

    static TextureHolder *m_s_instance;
};


//**************************************************************** */
class TextureHolderV2 {

public:

    // Restrict copy constructor and assignment operator
    TextureHolderV2(const TextureHolderV2 &orig) = delete;
    TextureHolderV2& operator=(const TextureHolderV2 &orig) = delete;

    static TextureHolderV2& getInstance();

    sf::Texture& getTexture(const std::string &file_name);


private:

    TextureHolderV2(){}   // private constructor

    std::map<std::string, sf::Texture> m_textures;

    std::map<std::string, sf::Texture>& getTexturesContainer();


};