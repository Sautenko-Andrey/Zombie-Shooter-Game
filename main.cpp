#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"



int main(){

    // Here is the instance of the TextureHolder
    TextureHolder texture_holder;

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
    sf::Vector2f mouse_world_position;

    // Where is the mouse in relation to screen coordinates
    sf::Vector2i mouse_screen_position;

    // Create an instance of the Player class
    Player player;

    // The boundaries of the arena
    sf::IntRect arena;

    // Create the background
    sf::VertexArray background;

    // Load the texture for our vertex array
    // sf::Texture texture_background;
    // texture_background.loadFromFile("graphics/background_sheet.png");

    auto texture_background = TextureHolder::getTexture(
        "graphics/background_sheet.png"
    );

    // Prepare for a horde of zombies
    int num_zombies = 0;
    int num_zombies_alive = 0;

    Zombie *zombies = nullptr;

    // 100 bullets should do
    Bullet bullets[100];
    int current_bullet = 0;
    int bullets_spare = 24;
    int bullets_in_clip = 6;
    int clip_size = 6;
    float fire_rate = 1.0f;

    // When was fire button last pressed?
    sf::Time last_pressed;

    // Hide the mouse pointer and replaceit with a crosshair
    window.setMouseCursorVisible(false);
    sf::Sprite sprite_crosshair;
    sf::Texture texture_crosshair = TextureHolder::getTexture(
        "graphics/crosshair.png"
    );

    sprite_crosshair.setTexture(texture_crosshair);
    sprite_crosshair.setOrigin(25, 25);

    // Create a couple of pickups
    Pickup health_pickup(PickupType::Health);
    Pickup ammo_pickup(PickupType::Ammo);

    // The main game loop
    while(window.isOpen()){

        /*
        *******************************************************************
        Handle input
        *******************************************************************
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
                    // Reloading
                    if(event.key.code == sf::Keyboard::R){
                        if(bullets_spare > clip_size){
                            // Plenty of bullets. Allow reload
                            bullets_in_clip = clip_size;
                            bullets_spare -= clip_size;
                        }
                        else if(bullets_spare > 0){
                            // Only few bullets left
                            bullets_in_clip = bullets_spare;
                            bullets_spare = 0;
                        }
                        else{
                            // More here soon!!!
                        }
                    }
                }
            }
        } // End event polling

        // Handle the player quitting
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }

        // Handle WASD while playing
        if(state == State::PLAYING){

            // Handle the pressing and releasing of WASD keys
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                player.moveUp();
            }
            else{
                player.stopUp();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                player.moveDown();
            }
            else{
                player.stopDown();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                player.moveLeft();
            }
            else{
                player.stopLeft();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                player.moveRight();
            }
            else{
                player.stopRight();
            }

            // Fire a bullet
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(game_time_total.asMilliseconds() -
                   last_pressed.asMilliseconds() > 1000 / fire_rate &&
                   bullets_in_clip > 0)
                {
                    // Path the centre of the player
                    // and the centre of the crosshair
                    // to the shoot function
                    bullets[current_bullet].shoot(
                        player.getCenter().x, player.getCenter().y,
                        mouse_world_position.x, mouse_world_position.y
                    );

                    ++current_bullet;

                    if(current_bullet > 99){
                        current_bullet = 0;
                    }

                    last_pressed = game_time_total;
                    --bullets_in_clip;
                }
            }
        }// End WASD while playing

        // Handle the leveling up state
        if(state == State::LEVELING_UP){

            // Handle the player leveling up
            if(event.key.code == sf::Keyboard::Num1){
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num2){
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num3){
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num4){
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num5){
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num6){
                state = State::PLAYING;
            }

            if(state == State::PLAYING){

                // Prepare the level
                // We will modify the next two lines later
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;

                // Pass the vertex array by reference
                // to the create_background function
                int tile_size = createBackground(background, arena);

                // Spawn the player in the middle of the arena
                player.spawn(arena, resolution, tile_size);

                // Configure the pick-ups
                health_pickup.setArena(arena);
                ammo_pickup.setArena(arena);

                // Create a horde of zombies
                num_zombies = 10;

                // Delete the previously allocated memory (if it exists)
                delete[] zombies;
                zombies = createHorde(num_zombies, arena);
                num_zombies_alive = num_zombies;

                // reset clock so there is not a frame jump
                clock.restart();
            }
        }// End leveling up state

        /*
        *****************************************************************
        UPDATE THE FRAME
        *****************************************************************
        */

        if(state == State::PLAYING){

            // Update the delta time
            sf::Time dt = clock.restart();

            // Update the total game time
            game_time_total += dt;

            // Make a fraction of 1 from the delta time
            float dt_as_seconds = dt.asSeconds();

            // Where is the mouse pointer
            mouse_screen_position = sf::Mouse::getPosition();

            // Convert mouse position to world based 
            // coordinates of main_view
            mouse_world_position = window.mapPixelToCoords(
                sf::Mouse::getPosition(), main_view
            );

            // Set the crosshair to the mouse world location
            sprite_crosshair.setPosition(mouse_world_position);

            // Update the player
            player.update(dt_as_seconds, sf::Mouse::getPosition());

            // Make a note of the players new position
            sf::Vector2f player_position(player.getCenter());

            // Make the view centre the around player
            main_view.setCenter(player.getCenter());

            // Loop through each zombie and update them
            for(int i = 0; i < num_zombies; ++i){
                if(zombies[i].isAlive()){
                    zombies[i].update(dt.asSeconds(), player_position);
                }
            }

            // Update any bullets that are in-flight
            for(int i = 0; i < 100; ++i){
                if(bullets[i].isInFlyght()){
                    bullets[i].update(dt_as_seconds);
                }
            }

            // Update the pickups
            health_pickup.update(dt_as_seconds);
            ammo_pickup.update(dt_as_seconds);

        } // End updating the scene

        /*
        ********************************************************************
        Draw the scene
        ********************************************************************
        */

        if(state == State::PLAYING){

            window.clear();

            // Set the main_view to be displayed in the window
            // and draw everything related to it
            window.setView(main_view);

            // Draw the background
            window.draw(background, &texture_background);

            // Draw the zombies
            for(int i = 0; i < num_zombies; ++i){
                window.draw(zombies[i].getSprite());
            }

            // Draw bullets
            for(int i = 0; i < 100; ++i){
                if(bullets[i].isInFlyght()){
                    window.draw(bullets[i].getShape());
                }
            }

            // Draw the player
            window.draw(player.getSprtite());

            // Draw the pickups if currently spawned
            if(ammo_pickup.isSpawned()){
                window.draw(ammo_pickup.getSprite());
            }

            if(health_pickup.isSpawned()){
                window.draw(health_pickup.getSprite());
            }

            // Draw the crosshair
            window.draw(sprite_crosshair);

        }

        if(state == State::LEVELING_UP){

        }

        if(state == State::PAUSED){
            
        }

        if(state == State::GAME_OVER){
            
        }

        window.display();

    }// End game loop

    delete[] zombies;

    return 0;

}