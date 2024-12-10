#pragma once
#include <SFML/Graphics.hpp>


enum class PickupType { Health = 1, Ammo };

class Pickup{

public:

    explicit Pickup(PickupType type);

    // Prepare a new pickup
    void setArena(sf::IntRect arena);
    void spawn();

    // Check the position of the pickup
    sf::FloatRect getPosition();

    // Get the sprite for drawing
    sf::Sprite getSprite();

    // Let the pickup update itsef each frame
    void update(float elapsed_time);

    // Is this pickup currently spawned?
    bool isSpawned();

    // Get the godness from the pickup
    int gotIt();

    // Upgrade the value of each pickup
    void upgrade();


private:

    // Start value for health pickups
    const float HEALTH_START_VALUE = 50.0f;
    const float AMMO_START_VALUE = 12.0f;
    const float START_WAIT_TIME = 10.0f;
    const float START_SECONDS_TO_LIVE = 5.0f;

    // The sprite that represents this pickup
    sf::Sprite m_sprite;

    // The arena it exists in
    sf::IntRect m_arena;

    // How much is this pickup worth?
    float m_value;

    // What type of pickup is this?
    PickupType m_type;

    // Handle spawing and disappearing
    bool m_spawned;
    float m_seconds_since_spawn;
    float m_seconds_since_de_spawn;
    float m_seconds_to_live;
    float m_seconds_to_wait;
};