#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <fstream>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include "Utils.h"


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

    int score = 0;
    int hi_score = 0;

    // For the home/gameover screen
    sf::Sprite sprite_game_over;
    sf::Texture texture_game_over = TextureHolder::getTexture(
        "graphics/background.png"
    );
    sprite_game_over.setTexture(texture_game_over);
    sprite_game_over.setPosition(0, 0);

    // Create a view for the HUD
    sf::View HUD_view(sf::FloatRect(0, 0, resolution.x, resolution.y));

    // Create a sprite for the ammo icon
    sf::Sprite sprite_ammo_icon;
    sf::Texture texture_ammo_icon = TextureHolder::getTexture(
        "graphics/ammo_icon.png"
    );
    sprite_ammo_icon.setTexture(texture_ammo_icon);
    sprite_ammo_icon.setPosition(20, 650);

    // Load the font
    sf::Font font;
    font.loadFromFile("fonts/zombiecontrol.ttf");

    // Paused
    // sf::Text paused_text;
    // paused_text.setFont(font);
    // paused_text.setCharacterSize(155);
    // paused_text.setFillColor(sf::Color::White);
    // paused_text.setPosition(400, 400);
    // paused_text.setString("Press enter \nto continue");
    auto ptr_paused_text = setUpTextObject(font, 155, sf::Color::White,
                                       sf::Vector2i(300, 250), 
                                       "Press enter \nto continue");

    // Game over
    auto ptr_game_over_text = setUpTextObject(font, 125, sf::Color::White,
                                          sf::Vector2i(150, 630),
                                          "Press Enter to play");

    // Leveling up
    std::stringstream level_up_stream;
    level_up_stream << "1- Increased rate of fire"
                    << "\n2- Increased clip size (next reload)"
                    << "\n3- Increased max health"
                    << "\n4- Increased run speed"
                    << "\n5- More and better health pickups"
                    << "\n6- More and better ammo pickups";
    auto ptr_level_up_text = setUpTextObject(font, 80, sf::Color::White,
                                         sf::Vector2i(50, 250),
                                         level_up_stream.str());

    // Ammo
    auto ptr_ammo_text = setUpTextObject(font, 55, sf::Color::White,
                                     sf::Vector2i(200, 680));

    // Score
    auto ptr_score_text = setUpTextObject(font, 55, sf::Color::White,
                                          sf::Vector2i(20, 0));

    // Load the hi score from a text file
    std::ifstream input_file("gamedata/scores.txt");
    if(input_file.is_open()){
        // >> reads the data
        input_file >> hi_score;
        input_file.close();
    }

    // Hi score
    std::stringstream hi_score_stream;
    hi_score_stream << "Record score: " << hi_score;
    auto ptr_hi_score_text = setUpTextObject(font, 55, sf::Color::White,
                                             sf::Vector2i(900, 0), 
                                             hi_score_stream.str());

    // Zombies remaining
    auto ptr_zombies_remaining_text = setUpTextObject(
        font, 55, sf::Color::White, sf::Vector2i(1050, 700), "Zombies: 100"
    );

    // Wave number
    int wave = 0;
    auto ptr_wave_number_text = setUpTextObject(
        font, 55, sf::Color::White, sf::Vector2i(800, 700), "Wave: 0"
    );

    // Health bar
    sf::RectangleShape health_bar;
    health_bar.setFillColor(sf::Color::Red);
    health_bar.setPosition(450, 700);

    // When did we last update the HUD?
    int frames_since_last_HUD_update = 0;

    // How often (in frames) we should update the hood
    int fps_measurement_frame_interval = 1000;

    // Prepare the hit sound
    sf::SoundBuffer hit_buffer;
    hit_buffer.loadFromFile("sound/hit.wav");
    sf::Sound hit_sound;
    hit_sound.setBuffer(hit_buffer);

    // Prepare the splat sound
    sf::SoundBuffer splat_buffer;
    splat_buffer.loadFromFile("sound/splat.wav");
    sf::Sound splat_sound;
    splat_sound.setBuffer(splat_buffer);

    // Prepare the shoot sound
    sf::SoundBuffer shoot_buffer;
    shoot_buffer.loadFromFile("sound/shoot.wav");
    sf::Sound shoot_sound;
    shoot_sound.setBuffer(shoot_buffer);

    // Prepare the reload sound
    sf::SoundBuffer reload_buffer;
    reload_buffer.loadFromFile("sound/reload.wav");
    sf::Sound reload_sound;
    reload_sound.setBuffer(reload_buffer);

    // Prepare the reload failed sound
    sf::SoundBuffer reload_failed_buffer;
    reload_failed_buffer.loadFromFile("sound/reload_failed.wav");
    sf::Sound reload_failed_sound;
    reload_failed_sound.setBuffer(reload_failed_buffer);


    // Prepare the power up sound
    sf::SoundBuffer powerup_buffer;
    powerup_buffer.loadFromFile("sound/powerup.wav");
    sf::Sound powerup_sound;
    powerup_sound.setBuffer(powerup_buffer);

    // Prepare the pickup sound
    sf::SoundBuffer pickup_buffer;
    pickup_buffer.loadFromFile("sound/pickup.wav");
    sf::Sound pickup_sound;
    pickup_sound.setBuffer(pickup_buffer);

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

                            wave = 0;
                            score = 0;

                            // Prepare the gun and ammo for next game
                            current_bullet = 0;
                            bullets_spare = 24;
                            bullets_in_clip = 6;
                            clip_size = 6;
                            fire_rate = 1;

                            // Reset the player's stats
                            player.resetPlayerStats();
                        }
                if(state == State::PLAYING){
                    // Reloading
                    if(event.key.code == sf::Keyboard::R){
                        if(bullets_spare > clip_size){

                            // Plenty of bullets. Allow reload
                            bullets_in_clip = clip_size;
                            bullets_spare -= clip_size;

                            // Play reload sound
                            reload_sound.play();
                        }
                        else if(bullets_spare > 0){

                            // Only few bullets left
                            bullets_in_clip = bullets_spare;
                            bullets_spare = 0;

                            // Play reload sound
                            reload_sound.play();
                        }
                        else{
                            // More here soon!!!
                            reload_failed_sound.play();
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
                    shoot_sound.play();
                    --bullets_in_clip;
                }
            }
        }// End WASD while playing

        // Handle the leveling up state
        if(state == State::LEVELING_UP){

            // Handle the player leveling up
            if(event.key.code == sf::Keyboard::Num1){
                // Increase fire rate
                ++fire_rate;
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num2){
                // Increase clip size
                clip_size += clip_size;
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num3){
                // Increase health
                player.upgradeHealth();
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num4){
                // Increase speed
                player.upgradeSpeed();
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num5){
                // Upgrade health pickup
                health_pickup.upgrade();
                state = State::PLAYING;
            }

            if(event.key.code == sf::Keyboard::Num6){
                // Upgrade ammo pickup
                ammo_pickup.upgrade();
                state = State::PLAYING;
            }

            if(state == State::PLAYING){
                
                // Increase the wave number
                ++wave;

                // Prepare the level
                // We will modify the next two lines later
                // arena.width = 500;
                // arena.height = 500;
                arena.width = 500 * wave;
                arena.height = 500 * wave;
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
                // num_zombies = 10;
                num_zombies = 5 * wave;

                // Delete the previously allocated memory (if it exists)
                delete[] zombies;
                zombies = createHorde(num_zombies, arena);
                num_zombies_alive = num_zombies;

                // Play the powerup sound
                powerup_sound.play();

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

            // Colission detection
            // Have any zombies been shot?
            for(int i = 0; i < 100; ++i){
                for(int j = 0; j < num_zombies; ++j){
                    if(bullets[i].isInFlyght() && zombies[j].isAlive()){
                        if(bullets[i].getPosition().intersects(zombies[j].getPosition())){
                            // Stop the bullet
                            bullets[i].stop();

                            // Register a hit and see if it was kill
                            if(zombies[j].hit()){
                                // Not just a hit but a kill too
                                score += 10;
                                if(score >= hi_score){
                                    hi_score = score;
                                }

                                --num_zombies_alive;

                                // When all zombies are dead (again)
                                if(num_zombies_alive == 0){
                                    state = State::LEVELING_UP;
                                }

                                splat_sound.play();
                            }
                        }
                    }
                }
            } // End zombie being shot

            // Have any zombies touched the player?
            for(int i = 0; i < num_zombies; ++i){
                if(player.getPosition().intersects(zombies[i].getPosition()) &&
                   zombies[i].isAlive()){

                    if(player.hit(game_time_total)){
                        // More here later !!!
                        hit_sound.play();
                    }

                    if(player.getHealth() <= 0){
                        state = State::GAME_OVER;
                        std::ofstream output_file("gamedata/scores.txt");
                        if(output_file.is_open()){
                            output_file << hi_score;
                            output_file.close();
                        }  
                    }

                }
            } // End player touched

            // Has the player touched health pickup?
            if(player.getPosition().intersects(health_pickup.getPosition()) &&
               health_pickup.isSpawned()){

                player.increaseHealthLevel(health_pickup.gotIt());

                pickup_sound.play();
            }

            // Has the player touched ammo pickup?
            if(player.getPosition().intersects(ammo_pickup.getPosition()) &&
               ammo_pickup.isSpawned()){
                
                bullets_spare += ammo_pickup.gotIt();

                pickup_sound.play();
            }

            // Size up the health bar
            health_bar.setSize(sf::Vector2f(
                player.getHealth() * 3, 50
            ));

            // Increment the number of frames since the previous update
            ++frames_since_last_HUD_update;

            // re-calculate every fps_measurement_frame_interval frames
            if(frames_since_last_HUD_update > fps_measurement_frame_interval){

                // Update game HUD text
                std::stringstream ss_ammo;
                std::stringstream ss_score;
                std::stringstream ss_hi_score;
                std::stringstream ss_wave;
                std::stringstream ss_zombies_alive;

                // Update the ammo text
                ss_ammo << bullets_in_clip << "/" << bullets_spare;
                ptr_ammo_text->setString(ss_ammo.str());

                // Update the score text
                ss_score << "Score: " << score;
                ptr_score_text->setString(ss_score.str());

                // Update the hi score text
                ss_hi_score << "Record Score: " << hi_score;
                ptr_hi_score_text->setString(ss_hi_score.str());

                // Update the wave
                ss_wave << "Wave: " << wave;
                ptr_wave_number_text->setString(ss_wave.str());

                // Update zombies alive
                ss_zombies_alive << "Zombies: " << num_zombies_alive;
                ptr_zombies_remaining_text->setString(ss_zombies_alive.str());
                
                frames_since_last_HUD_update = 0;
            } // End HUD update

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

        // Switch to the HUD view
        window.setView(HUD_view);

        // Draw all the HUD elements
        window.draw(sprite_ammo_icon);
        window.draw(*ptr_ammo_text);
        window.draw(*ptr_score_text);
        window.draw(*ptr_hi_score_text);
        window.draw(health_bar);
        window.draw(*ptr_wave_number_text);
        window.draw(*ptr_zombies_remaining_text);

        if(state == State::LEVELING_UP){
            
            window.draw(sprite_game_over);
            window.draw(*ptr_level_up_text);

        }

        if(state == State::PAUSED){
            window.draw(*ptr_paused_text);
        }

        if(state == State::GAME_OVER){
            window.draw(sprite_game_over);
            window.draw(*ptr_game_over_text);
            window.draw(*ptr_score_text);
            window.draw(*ptr_hi_score_text);
        }

        window.display();

    }// End game loop

    delete[] zombies;

    return 0;

}