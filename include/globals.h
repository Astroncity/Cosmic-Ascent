#pragma once
#include "planet.h"
#include "planetTerrain.h"
#include "player.h"

typedef enum GameState {
    MAIN_MENU,
    ON_PLANET,
    IN_SPACE,
    GAME_OVER,
} GameState;

#define screenWidth 480
#define screenHeight 270
extern Planet* currentPlanet;
extern PlanetTerrain* currentTerrain;
extern Player* player;
extern v2 mouse;
extern GameState state;
extern bool quit;
