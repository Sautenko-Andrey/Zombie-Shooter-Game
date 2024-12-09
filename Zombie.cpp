#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

enum class ZombieType{
    Bloater, Chaser, Crawler
};


bool Zombie::hit(){

    /*
        Call every time the zombie is hit by a bullet. The function 
        can then take the necessary steps, such as taking health from 
        the zombie(reducing the value of m_health) or killing it 
        (setting m_alive to false).
    */

    --m_health;

    if(m_health < 0){
        // dead
        m_alive = false;

        m_sprite.setTexture(
            TextureHolder::getTexture("graphics/blood.png")
        );

        return true;
    }
    
    // Zombie is injured but not dead yet
    return false;
}


bool Zombie::isAlive(){

    /*
        Returns a Boolean that lets the calling code know whether 
        the zombie is alive or dead. We don’t want to perform collision 
        detection or remove health from the player for walking over 
        a blood splat.
    */

    return m_alive;
}

void Zombie::setUpZombie(const std::string &texture_path,
                     float zombie_speed, float zombie_health){

    if(texture_path.empty() || zombie_speed < 0.0 || zombie_health < 0.0){
        // Just get out from the function.
        return;
    }

    m_sprite = sf::Sprite(TextureHolder::getTexture(texture_path));
    m_speed = zombie_speed;
    m_health = zombie_health;
}


void Zombie::spawn(float start_x, float start_y, int type, int seed){

    /*
        Takes a starting position, a type (Crawler, Bloater, or Chaser, 
        represented by an int), as well as a seed to use in some 
        random number generation.
    */

    switch (type)
    {
    case static_cast<int>(ZombieType::Bloater):
        m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/bloater.png"));
        // TEST THIS AS WELL !!!***************************************************
        // TextureHolderV2 &texture_holder = TextureHolderV2::getInstance();
        // m_sprite = sf::Sprite(texture_holder.getTexture(
        //     "graphics/bloater.png"
        // ));
        //***************************************************************** */
        m_speed = BLOATER_SPEED;
        m_health = BLOATER_HEALTH;
        break;

    case static_cast<int>(ZombieType::Chaser):
        setUpZombie("graphics/chaser.png", CHASER_SPEED, CHASER_HEALTH);
        break;

    case static_cast<int>(ZombieType::Crawler):
        setUpZombie("graphics/crawler.png", CRAWLER_SPEED, CRAWLER_HEALTH);
        break;
    }

    // Modify the speed to make the zombie unique
    // Every zombie is unique. Create a speed modifier.
    srand((int)time(0) * seed);
    
    // Somewhere between 80 and 100
    float modifier = (rand() % MAX_VARRIANCE) + OFFSET;

    // Express this as a fraction of 1
    modifier /= 100;   // now euqlas between 0.7 and 1

    m_speed *= modifier;

    // Initialize its location
    m_position.x = start_x;
    m_position.y = start_y;

    // Set its origin to its center
    m_sprite.setOrigin(25, 25);

    // Set its position
    m_sprite.setPosition(m_position);
}


sf::FloatRect Zombie::getPosition(){

    return m_sprite.getGlobalBounds();

}


sf::Sprite Zombie::getSprite(){

    return m_sprite;

}


void Zombie::update(float elapsed_time, sf::Vector2f player_location){

    /*
        It would receive the elapsed time since the last frame, but 
        also, notice that it receives a Vector2f vector called 
        playerLocation. This vector will indeed be the exact coordinates 
        of the center of the player.
        This vector to chase after the player.
    */

    auto player_x = player_location.x;

    auto player_y = player_location.y;

    // Update the zombie position variables
    if(player_x > m_position.x){
        m_position.x += (m_speed * elapsed_time);
    }

    if(player_y > m_position.y){
        m_position.y += (m_speed * elapsed_time);
    }

    if(player_x < m_position.x){
        m_position.x -= m_speed * elapsed_time;
    }

    if(player_y < m_position.y){
        m_position.y -= m_speed * elapsed_time;
    }

    // Move the sprite
    m_sprite.setPosition(m_position);

    // Face the sprite in the correct direction
    auto angle = (
        (atan2(player_y - m_position.y, player_x - m_position.x) * 180) / 3.141
    );

    m_sprite.setRotation(angle);

}