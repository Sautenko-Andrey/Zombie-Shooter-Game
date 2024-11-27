#include "Player.h"

void Player::spawn(sf::IntRect arena, sf::Vector2f resolution, int tile_size){
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
}


void Player::resetPlayerStats(){

    /*
        Once we give the player the ability to level up between waves,
        we will need to be able to take away/reset those abilities at 
        the start of a new game.
    */
}


bool Player::hit(sf::Time time_hit){

    /*

    */
}


sf::Time Player::getLastHitTime(){

    /*
        This function does just one thing – it returns the time when
        the player was last hit by a zombie. We will use this function 
        when detecting collisions, and it will allow us to make sure 
        that the player isn’t punished too frequently for making
        contact with a zombie.
    */
}


sf::FloatRect Player::getPosition(){

    /*
        This function returns a FloatRect instance that describes
        the horizontal and vertical floating-point coordinates of the rectangle,
        which contains the player graphic. 
        This is also useful for collision detection.
    */

}