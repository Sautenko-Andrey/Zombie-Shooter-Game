#include <SFML/Graphics.hpp>
#include "Player.h"




int main(){

    // The game will always be in one of four states
    enum class State {
        PAUSED, LEVELING_UP, GAME_OVER, PLAYING
    };

    // Start with the game over state
    State state = State::GAME_OVER;

    // Get the screen resolution and create a SFML window
    sf::Vector2f resolution;
    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(
        sf::VideoMode(resolution.x, resolution.y),
        "Zombie Arena",
        sf::Style::Fullscreen
    );

    // Create a SFMAL view for the main action
    sf::View main_view(sf::FloatRect(0, 0, resolution.x, resolution.y));

    // Here is our clock for timing everything
    sf::Clock clock;

    // How long the playing state has been active?
    sf::Time game_time_total;

    // Where is the mouse in relation to word coordinates
    sf::Vector2f mouse_word_position;

    // Where is the mouse in relation to screen coordinates
    sf::Vector2f mouse_screen_position;

    // Create an instance of the Player class
    Player player;

    // The boundaries of the arena
    sf::IntRect arena;

    // The main game loop
    while(window.isOpen()){

        /*
        ************
        Handle input
        ************
        */

        // Handle events by polling
        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::KeyPressed){
                // Pause a game while playing
                if(event.key.code == sf::Keyboard::Return &&
                   state == State::PLAYING){
                        state = State::PAUSED;
                    }

                // Restart while paused
                else if(event.key.code == sf::Keyboard::Return &&
                        state == State::PAUSED){
                            state = State::PLAYING;
                            // Reset the clock so there isn't a frame jump
                            clock.restart();
                        }
                // Start a new game while in GAME_OVER state
                else if(event.key.code == sf::Keyboard::Return &&
                        state == State::GAME_OVER){
                            state = State::LEVELING_UP;
                        }
                if(state == State::PLAYING){

                }
            }
        } // End event polling
    }// End game loop

    return 0;

}