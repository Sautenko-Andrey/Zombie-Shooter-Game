#include "ZombieArena.h"

int createBackground(sf::VertexArray &rVA, sf::IntRect arena){
    /*
        Anything we do to rVA we are really doing 
        to background (in the main function)
    */

    // How big is each tile/texture

    constexpr int TILE_SIZE = 50;
    constexpr int TILE_TYPES = 3;
    constexpr int VERTS_IN_QUAD = 4;

    int world_width = arena.width / TILE_SIZE;
    int world_height = arena.height / TILE_SIZE;

    // What type of primitive are we using?
    rVA.setPrimitiveType(sf::Quads);

    // Set the size of the vertex array
    rVA.resize(world_width * world_height * VERTS_IN_QUAD);

    // Start at the beginning of the vertex array
    int current_vertex = 0;

    for(int w = 0; w < world_width; ++w){
        for(int h = 0; h < world_height; ++h){
            // Position each vertex in the current quad
            rVA[current_vertex + 0].position = sf::Vector2f(
                w * TILE_SIZE, h * TILE_SIZE
            );

            rVA[current_vertex + 1].position = sf::Vector2f(
                (w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE
            );
            
            rVA[current_vertex + 2].position = sf::Vector2f(
                (w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE
            );

            rVA[current_vertex + 3].position = sf::Vector2f(
                (w * TILE_SIZE), (h * TILE_SIZE) + TILE_SIZE
            );

            // Define the position in the texture for current quad
            // either grass, stone, bush or wall

            if(h == 0 || h == world_height - 1 ||
               w == 0 || w == world_width - 1){

                // Use the wall texture
                rVA[current_vertex + 0].texCoords = sf::Vector2f(
                    0, 0 + TILE_TYPES * TILE_SIZE
                );

                rVA[current_vertex + 1].texCoords = sf::Vector2f(
                    TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE
                );

                rVA[current_vertex + 2].texCoords = sf::Vector2f(
                    TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE
                );

                rVA[current_vertex + 3].texCoords = sf::Vector2f(
                    0, TILE_SIZE + TILE_TYPES * TILE_SIZE
                );
            }
            else{
                // Use a random floor texture
                srand((int)time(0) + h * w - h);

                int mOrG = (rand() % TILE_TYPES);
                int vertical_offset = mOrG * TILE_SIZE;

                rVA[current_vertex + 0].texCoords = sf::Vector2f(
                    0, 0 + vertical_offset
                );

                rVA[current_vertex + 1].texCoords = sf::Vector2f(
                    TILE_SIZE, 0 + vertical_offset
                );

                rVA[current_vertex + 2].texCoords = sf::Vector2f(
                    TILE_SIZE, TILE_SIZE + vertical_offset
                );

                rVA[current_vertex + 3].texCoords = sf::Vector2f(
                    0, TILE_SIZE + vertical_offset
                );
            }

            // Position ready for the next four verticies
            current_vertex += VERTS_IN_QUAD;
        }
    }

    return TILE_SIZE;

}