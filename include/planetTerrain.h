#pragma once
#include "planet.h"

#define LEVEL_SCALE 1

typedef struct PlanetTerrain {
    Texture2D texture;
} PlanetTerrain;

PlanetTerrain* genPlanetTerrain(Planet* planet);
