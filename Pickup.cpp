#include "Pickup.h"
#include "TextureHolder.h"



Pickup::Pickup(PickupType type) : m_type{type}
{
    // Associate the texture with the sprite
    if(m_type == PickupType::Health){
        // Health
        m_sprite = sf::Sprite(TextureHolder::getTexture(
            "graphics/health_pickup.png"
        ));

        // How much is pickup worth?
        m_value = HEALTH_START_VALUE;
    }
    else{
        // Ammo
        m_sprite = sf::Sprite(TextureHolder::getTexture(
            "graphics/ammo_pickup.png"
        ));

        m_value = AMMO_START_VALUE;
    }

    m_sprite.setOrigin(25, 25);

    m_seconds_to_live = START_SECONDS_TO_LIVE;
    m_seconds_to_wait = START_WAIT_TIME;
}


void Pickup::setArena(sf::IntRect arena){

    // Copy the details of the arena to the pickup's m_arena

    constexpr int value = 50;
    m_arena.left = arena.left + value;
    m_arena.width = arena.width - value;
    m_arena.top = arena.top + value;
    m_arena.height = arena.height - value;

    spawn();

}


void Pickup::spawn(){

    // Spawn at a random location
    srand((int)time(0) / static_cast<int>(m_type));
    int x = (rand() % m_arena.width);

    srand((int)time(0) * static_cast<int>(m_type));
    int y = (rand() % m_arena.height);

    m_seconds_since_spawn = 0.0;

    m_spawned = true;

    m_sprite.setPosition(x, y);

}

sf::FloatRect Pickup::getPosition(){

    return m_sprite.getGlobalBounds();

}


sf::Sprite Pickup::getSprite(){

    return m_sprite;

}


void Pickup::update(float elapsed_time){

    if(m_spawned){
        m_seconds_since_spawn += elapsed_time;
    }
    else{
        m_seconds_since_de_spawn += elapsed_time;
    }

    // Do we need to hide a pickup?
    if(m_seconds_since_spawn > m_seconds_to_live && m_spawned){
        // Remove the pickup and put it somewhere else
        m_spawned = false;
        m_seconds_since_de_spawn = 0;
    }

    // Do we need to spawn the pickup?
    if(m_seconds_since_de_spawn > m_seconds_to_wait && !m_spawned){
        // Spawn the pickup and reset the timer
        spawn();
    }

}


bool Pickup::isSpawned(){

    return m_spawned;

}


int Pickup::gotIt(){

    /*
        This function will be called from main when the player 
        touches/collides with (gets) the pickup.
    */

    m_spawned = false;

    m_seconds_since_de_spawn = 0;

    return m_value;

}


void Pickup::upgrade(){

    if(m_type == PickupType::Health){
        m_value += (HEALTH_START_VALUE * 0.5);
    }
    else{
        m_value += (AMMO_START_VALUE * 0.5);
    }

    // Make them more frequent and last longer
    m_seconds_to_live += (START_SECONDS_TO_LIVE / 10);
    m_seconds_to_wait -= (START_WAIT_TIME / 10);

}