#pragma once
#include "defs.h"
#include "raylib.h"

/*typedef struct Planet {
    Texture2D texture;
    Texture2D atmosphere;
    Texture2D thumbnail;
    Texture2D thumbnailAtmosphere;
    Vector2 pos;
    i32 size;
    Color terrainColor;
    Color* palette;
} Planet;

Planet* genPlanet(i32 imgSize, bool randomizeColors);
void drawPlanet(Planet* planet, f32 scale);
void drawPlanetThumbnail(Vector2 pos, Planet* planet);*/

class Planet {
  public:
    Texture2D texture;
    Texture2D atmosphere;
    Texture2D thumbnail;
    Texture2D thumbnailAtmosphere;
    Vector2 pos;
    i32 size;
    Color terrainColor;
    Color* palette;

    Planet(i32 imgSize, bool randomizeColors);

    void draw(f32 scale);
    void drawThumbnail(Vector2 pos);
};
