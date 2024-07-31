#pragma once
#include "defs.h"
#include "gameobject.hpp"

class PlanetSelect : public GameObject {
  public:
    i32 size;
    void (*callback)();
    PlanetSelect(i32 planetSize, void (*callback)());

    Rect getCollider() override;
    void update() override;
    void destroy() override;
    void render() override;
};
