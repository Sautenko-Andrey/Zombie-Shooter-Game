#include "TextureHolder.h"



TextureHolder* TextureHolder::m_s_instance = nullptr;

TextureHolder::TextureHolder(){
    if(m_s_instance != nullptr){
        throw std::runtime_error("TextureHolder instance already created");
    }

    m_s_instance = this;
}


sf::Texture& TextureHolder::getTexture(const std::string &file_name){

    // Get a reference to m_textures using m_s_instance
    auto &m = m_s_instance->m_textures;

    // Create an iterator to hold a key-value-pair (kvp)
    // and search for the required kvp
    // using the passed in file name
    auto keyValuePair = m.find(file_name);

    // Did we find a match?
    if(keyValuePair != m.end()){
        // Yes
        // Return the texture,
        // the second part of the kvp, the texture
        return keyValuePair->second;
    }
    else{
        // File name not found
        // Create a new key value pair using the filename
        auto &texture = m[file_name];

        // Load the texture from file in the usual way
        texture.loadFromFile(file_name);

        return texture;
    }

}


/************************************************************ */
TextureHolderV2& TextureHolderV2::getInstance(){
    static TextureHolderV2 texture_holder;
    return texture_holder;
}

std::map<std::string, sf::Texture>& TextureHolderV2::getTexturesContainer()
{
    return m_textures;
}

sf::Texture& TextureHolderV2::getTexture(const std::string file_name){

    auto &textures = getTexturesContainer();

    auto iter = textures.find(file_name);

    if(iter != textures.end()){
        return iter->second;
    }
    else{
        auto &new_texture = textures[file_name];
        new_texture.loadFromFile(file_name);
        return new_texture;
    }
}