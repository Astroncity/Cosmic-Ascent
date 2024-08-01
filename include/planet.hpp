#pragma once
#include "defs.h"
#include "raylib.h"
#include "render.hpp"
#include <string>

class Label : public Renderable {
  public:
    static Texture2D background;
    std::string text;

    Label(std::string text);
};

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
    bool hovered;
    bool visited;

    Planet(i32 imgSize, bool randomizeColors);
    ~Planet();

    void draw(f32 scale);
    void drawThumbnail(Vector2 pos);
};
