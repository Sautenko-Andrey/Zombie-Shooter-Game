#pragma once
#include <SFML/Graphics.hpp>


class Player {

public:

    Player();

    void spawn(sf::IntRect arena, sf::Vector2f resolution, int tilte_size);

    // Call this at the end of every game
    void resetPlayerStats();

    // Handle the player getting hit by a zombie
    bool hit(sf::Time time_hit);

    // How long ago was the player last hit
    sf::Time getLastHitTime();

    // Where is the player
    sf::FloatRect getPosition();

    // Where is the center of the player
    sf::Vector2f getCenter();

    // What angle is the player facing
    float getRotation();

    // Send a copy of the sprite to the main function
    sf::Sprite getSprtite();

    // The next four functions move the player
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    // Stop the player moving in a specific direction
    void stopLeft();
    void stopRight();
    void stopUp();
    void stopDown();

    // We will call this function once every frame
    void update(float elapsed_time, sf::Vector2i mouse_position);

    // Give the player a speed boost
    void upgradeSpeed();

    // Give the player some health
    void upgradeHealth();

    // Increase the max' health the player can have
    void increaseHealthLevel(int amount);

    // How much health has the player currently got?
    int getHealth();

private:

    const float START_SPEED{200.0f};
    const float START_HEALTH{100.0f};

    // Where is the player
    sf::Vector2f m_position;

    // The Sprite
    sf::Sprite m_sprite;

    // And a Texture
    // !! Watch this space - Changes here soon!!
    sf::Texture m_texture;

    // What is the screen resolution
    sf::Vector2f m_resolution;

    // What size is the current area
    sf::IntRect m_arena;

    // How big is each tile of the arena
    int m_tile_size;

    // Which direction(s)the player is moving in
    bool m_up_pressed;
    bool m_down_pressed;
    bool m_left_pressed;
    bool m_right_pressed;

    // How much health has the player got?
    int m_health;

    // What is the max' health the player can have
    int m_max_health;

    // When was the player last hit
    sf::Time m_last_hit;

    // Speed in pixels per second
    float m_speed;

};