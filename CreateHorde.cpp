#include "ZombieArena.h"
#include "Zombie.h"

enum class Side{ Left, Rigth, Top, Bottom };


Zombie* createHorde(int num_zombies, sf::IntRect arena){

    Zombie* zombies = new Zombie[num_zombies];

    int max_y = arena.height - 20;

    int min_y = arena.top + 20;

    int max_x = arena.width - 20;

    int min_x = arena.left + 20;

    for(int i = 0; i < num_zombies; ++i){

        // Which side should the zombie spawn
        srand((int)time(0) * i);

        int side = (rand() % 4);

        float x{0.0}, y{0.0};

        switch (side)
        {
        case static_cast<int>(Side::Left):
            x = min_x;
            y = (rand() % max_y) + min_y;
            break;
        
        case static_cast<int>(Side::Rigth):
            x = max_x;
            y = (rand() % max_y) + min_y;
            break;

        case static_cast<int>(Side::Top):
            x = (rand() % max_x) + min_x;
            y = min_y;
            break;

        case static_cast<int>(Side::Bottom):
            x = (rand() % max_x) + min_x;
            y = max_y;
            break;
        }

        // Bloater, crawler or runner
        srand((int)time(0) * i * 2);

        int type = (rand() % 3);

        // Spawn the new zombie into the array
        zombies[i].spawn(x, y, type, i);

    }

    return zombies;

}