#pragma once
#include <SFML/Graphics.hpp>



class Zombie {

public:

    // Handle when a bullet hits a zombie
    bool hit();

    // Fid out if the zombie is alive
    bool isAlive();

    // Spawn a new zombie
    void spawn(float start_x, float start_y, int type, int seed);

    // return a rectangle that is the position in the world
    sf::FloatRect getPosition();

    // Get a copy of the Sprite to draw
    sf::Sprite getSprite();

    // Update the zombie each frame
    void update(float elapsed_time, sf::Vector2f player_location);


private:

    // How fast is each zombie type?
    const float BLOATER_SPEED = 40.0f;
    const float CHASER_SPEED = 80.0f;
    const float CRAWLER_SPEED = 20.0f;

    // How tough is each zombie type?
    const float BLOATER_HEALTH = 5.0f;
    const float CHASER_HEALTH = 1.0f;
    const float CRAWLER_HEALTH = 3.0f;

    // Make each zombie vary its speed slightly
    const int MAX_VARRIANCE = 30;
    const int OFFSET = 101 - MAX_VARRIANCE;

    // Where is this zombie?
    sf::Vector2f m_position;

    // A sprite for the zombie
    sf::Sprite m_sprite;

    // How fast can this one run/crawl?
    float m_speed;

    // How much health has it got?
    float m_health;

    // Is it still alive?
    bool m_alive;

    // Set up a zombie (my function)
    void setUpZombie(const std::string &texture_path,
                     float zombie_speed, float zombie_health);

};