#pragma once
#include "defs.h"
#include "gameobject.hpp"

/*typedef struct FuelMeter {
    f32 fuel;
    f32 currFuel;
    f32 maxFuel;
    Rect rect;
} FuelMeter;

FuelMeter* FuelMeterCreate(f32 x, f32 y, f32 max);
void FuelMeterDestroy(FuelMeter* fuelMeter);*/

class FuelMeter : public GameObject {
  public:
    f32 fuel;
    f32 maxFuel;
    f32 currFuel;
    Rect rect;

    FuelMeter(f32 x, f32 y, f32 max);
    f32 getFuel();
    void render() override;
    void update() override;
    void destroy() override;
    Rect getCollider() override;
};
