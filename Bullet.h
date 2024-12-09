#pragma once
#include <SFML/Graphics.hpp>


class Bullet {

public:

    // The constructor
    Bullet();

    // Stop the bullet;
    void stop();

    // returns the value of m_in_flyght
    bool isInFlyght();

    // Launch a new bullet
    void shoot(float start_x, float start_y,
               float x_target, float y_target);

    // Tell the calling code where the bullet is in
    // the world
    sf::FloatRect getPosition();

    // Return the actual shape (for drawing)
    sf::RectangleShape getShape();

    // Update the bullet each frame
    void update(float elapsed_time);


private:

    // Where is the bullet?
    sf::Vector2f m_position;

    // What each bullet looks like
    sf::RectangleShape m_bullet_shape;

    // Is this bullet currently whizzing through the air
    bool m_in_flyght =  false;

    // How fast does a bullet travel
    float m_bullet_speed = 1000.0f;

    // What fraction of 1 pixel does the bullet travel,
    // Horizontally and vertically each frame?
    // These values will be derived from m_bulletSpeed
    float m_bullet_distance_x;
    float m_bullet_distance_y;

    // Some boundaries so the bullet doesn't fly forever
    float m_max_x;
    float m_min_x;
    float m_max_y;
    float m_min_y;

};