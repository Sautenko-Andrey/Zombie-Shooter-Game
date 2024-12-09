#include "Bullet.h"


Bullet::Bullet(){

    m_bullet_shape.setSize(sf::Vector2f(2, 2));

}


void Bullet::stop(){}


bool Bullet::isInFlyght(){

    /*
        Test whether a bullet is currently in flight or not
    */

}


void Bullet::shoot(float start_x, float start_y,
           float target_x, float target_y){

    // Keep track of the bullet
    m_in_flyght = true;

    m_position.x = start_x;

    m_position.y = start_y;

    // Calculate the gradient of the flight path
    float gradient = (start_x - target_x) / (start_y - target_y);

    // Any gradient less then 1 needs to be negative
    if(gradient < 0){
        gradient *= -1;
    }

    // Calculate the ratio between x and y
    float ratioXY = m_bullet_speed / (1 + gradient);

    // Set the speed horizontally and vertically
    m_bullet_distance_y = ratioXY;
    m_bullet_distance_x = ratioXY * gradient;

    // Point the bullet in the right direction
    if(target_x < start_x){
        m_bullet_distance_x *= -1;
    }

    if(target_y < start_y){
        m_bullet_distance_y *= -1;
    }

    // Set a max range of 1000 pixels
    float range = 1000;

    m_min_x = start_x - range;
    m_max_x = start_x + range;
    m_min_y = start_y - range;
    m_max_y = start_y + range;

    // Position the bullet ready to be drawn
    m_bullet_shape.setPosition(m_position);
}


sf::FloatRect Bullet::getPosition(){}


sf::RectangleShape Bullet::getShape(){}


void Bullet::update(float elapsed_time){}