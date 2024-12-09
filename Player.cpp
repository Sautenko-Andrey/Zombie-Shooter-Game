#include "Player.h"
#include "TextureHolder.h"
#include <cmath>

Player::Player()
: m_sprite(),
  m_texture(),
  m_health{START_HEALTH},
  m_max_health{START_HEALTH},
  m_speed{START_SPEED}
{
    // Associate a texture with the sprite
    // !! Watch this space !!
    // m_texture.loadFromFile("graphics/player.png");
    // m_sprite.setTexture(m_texture);

    m_sprite = sf::Sprite(TextureHolder::getTexture(
        "graphics/player.png"
    ));

    // Set the origin of the sprite to the center,
    // for smooth rotation
    m_sprite.setOrigin(25, 25);
}


void Player::spawn(sf::IntRect arena, sf::Vector2f resolution, int title_size){
    /*
        This function does
        what its name suggests. It will prepare the object ready for use,
        which includes putting it in its starting location (that is, spawning it).
        Notice that it doesn’t return any data, but it does have three arguments.
        It receives an IntRect instance called arena, which will be the size
        and location of the current level; a Vector2f instance, which will 
        contain the screen resolution; and an int, which will hold the size 
        of a background tile.
    */

    // Place the player in the middle of the arena
    m_position.x = static_cast<float>(arena.width / 2);
    m_position.y = static_cast<float>(arena.height / 2);

    // Copy the details of the arena to the player's m_arena
    m_arena.left = arena.left;
    m_arena.width = arena.width;
    m_arena.top = arena.top;
    m_arena.height = arena.height;

    // Remember how big the titles are in this arena
    m_tile_size = title_size;

    // Store the resloution for future use
    m_resolution.x = resolution.x;
    m_resolution.y = resolution.y;
}


void Player::resetPlayerStats(){

    /*
        Once we give the player the ability to level up between waves,
        we will need to be able to take away/reset those abilities at 
        the start of a new game.
    */

    m_speed = START_SPEED;
    m_health = START_HEALTH;
    m_max_health = START_HEALTH;
}


bool Player::hit(sf::Time time_hit){

    /*
        First, the if statement checks to see whether the time that’s 
        passed in as a parameter is 200 milliseconds further ahead than 
        the time stored in m_LastHit. If it is, m_LastHit is updated 
        with the time passed in and m_Health has 10 deducted from its 
        current value.
    */

    if(time_hit.asMilliseconds() - m_last_hit.asMilliseconds() > 200){
        m_last_hit = time_hit;
        m_health -= 10;
        return true;
    }
    else{
        return false;
    }
}


sf::Time Player::getLastHitTime(){

    /*
        This function does just one thing – it returns the time when
        the player was last hit by a zombie. We will use this function 
        when detecting collisions, and it will allow us to make sure 
        that the player isn’t punished too frequently for making
        contact with a zombie.
    */

    return m_last_hit;
}


sf::FloatRect Player::getPosition(){

    /*
        This function returns a FloatRect instance that describes
        the horizontal and vertical floating-point coordinates of the rectangle,
        which contains the player graphic. 
        This is also useful for collision detection.
    */

    return m_sprite.getGlobalBounds();
}


sf::Vector2f Player::getCenter(){

    /*
        This is slightly different from getPosition because it is 
        a Vector2f type and contains just the x and y locations of 
        the very center of the player graphic.
    */

    return m_position;
}


float Player::getRotation(){

    /*
        The code in the main function will sometimes need to know, in 
        degrees, which way the player is currently facing. 3 o’clock is 
        0 degrees and increases clockwise.
    */

    return m_sprite.getRotation();
}


sf::Sprite Player::getSprtite(){

    /*
        This function returns a copy of the sprite 
        that represents the player.
    */

    return m_sprite;
}

void Player::moveLeft(){

    m_left_pressed = true;

}


void Player::moveRight(){

    m_right_pressed = true;

}


void Player::moveUp(){

    m_up_pressed = true;

}


void Player::moveDown(){

    m_down_pressed = true;

}


void Player::stopLeft(){

    m_left_pressed = false;

}


void Player::stopRight(){

    m_right_pressed = false;

}


void Player::stopUp(){

    m_up_pressed = false;

}


void Player::stopDown(){

    m_down_pressed = false;

}


void Player::update(float elapsed_time, sf::Vector2i mouse_position){

    /*
        This will be the only long function of the entire class. 
        It will be called once per frame from main. It will do everything 
        necessary to make sure the player object’s data is updated so that 
        it’s ready for collision detection and drawing. Notice that it 
        returns no data but receives the amount of elapsed time since the 
        last frame, along with a Vector2i instance, which will hold the 
        horizontal and vertical screen location of the mouse 
        pointer/crosshair.
    */

    if(m_up_pressed){
        m_position.y -= m_speed * elapsed_time;
    }

    if(m_down_pressed){
        m_position.y += m_speed * elapsed_time;
    }

    if(m_right_pressed){
        m_position.x += m_speed * elapsed_time;
    }

    if(m_left_pressed){
        m_position.x -= m_speed * elapsed_time;
    }

    m_sprite.setPosition(m_position);

    // Keep the player in the arena
    if(m_position.x > static_cast<float>(m_arena.width - m_tile_size)){
        m_position.x = static_cast<float>(m_arena.width - m_tile_size);
    }

    if(m_position.x < static_cast<float>(m_arena.left + m_tile_size)){
        m_position.x = static_cast<float>(m_arena.left + m_tile_size);
    }

    if(m_position.y > static_cast<float>(m_arena.height - m_tile_size)){
        m_position.y = static_cast<float>(m_arena.height - m_tile_size);
    }

    if(m_position.y < static_cast<float>(m_arena.top + m_tile_size)){
        m_position.y = static_cast<float>(m_arena.top + m_tile_size);
    }

    // Calculate the angle the player is facing
    float angle = static_cast<float>((atan2(static_cast<float>(mouse_position.y) - m_resolution.y / 2,
        static_cast<float>(mouse_position.x) - m_resolution.x / 2) * 180) / 3.141);

    m_sprite.setRotation(angle);

}


void Player::upgradeSpeed(){
    /*
        A function that can be called from the leveling-up screen when 
        the player chooses to make the character move faster.
    */

    m_speed += static_cast<float>((START_SPEED * 0.2));
}


void Player::upgradeHealth(){

    /*
        Another function that can be called from the leveling-up screen 
        when the player chooses to make the character move stronger
    */

    m_max_health += static_cast<float>((START_HEALTH * 0.2));

}


void Player::increaseHealthLevel(int amount){

    /*
        A subtle but important difference regarding the previous function 
        is that this one will increase the amount of health the player has, 
        up to the maximum that’s currently set. This function will be used 
        when the player picks up a health pick-up.
    */

    m_health += static_cast<float>(amount);

    // But not beyond the maximum
    if(m_health > m_max_health){
        m_health = m_max_health;
    }

}


float Player::getHealth(){

    /*
        With the level of health being as dynamic as it is, we need to be 
        able to determine how much health the player has at any given 
        moment. This function returns an int, which holds that value.
    */

    return m_health;
}