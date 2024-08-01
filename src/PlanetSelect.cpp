#include "PlanetSelect.hpp"
#include "globals.hpp"
#include "raylib.h"
#include <stdio.h>

static std::vector<Planet*> planets;

PlanetSelect::PlanetSelect(i32 planetSize, void (*callback)())
    : GameObject("PlanetSelect", 50) {
    size = planetSize;
    this->callback = callback;
    for (i32 i = 0; i < 3; i++) {
        planets.push_back(new Planet(size, 64));
    }
    f32 ypos = screenHeight / 2.0 - size / 2.0;

    planets[0]->pos = {50, ypos};
    planets[1]->pos = {screenWidth - 50 - size, ypos};
    planets[2]->pos = {screenWidth / 2.0 - size / 2.0, ypos};
}

Rect PlanetSelect::getCollider() { return {0, 0, 0, 0}; }

void PlanetSelect::update() { return; }

void PlanetSelect::destroy() {
    GameObject::destroy();
    callback();
}

void PlanetSelect::render() {
    for (Planet* planet : planets) {
        planet->draw(1);
        v2 pos;
        pos.x = planet->pos.x + size / 2.0;
        pos.y = planet->pos.y + size / 2.0;
        if (CheckCollisionPointCircle(mouse, pos, size / 2.0)) {
            if (IsMouseButtonPressed(0)) {
                currentPlanet = planet;
                destroy();
            }
        }
    }
}
