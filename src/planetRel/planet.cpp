#include "planet.hpp"
#include "assert.h"
#include "defs.h"
#include "raylib.h"
#include "utils.h"
#include <algorithm>
#include <cstring>
#include <math.h>
#include <stdarg.h>
#include <vector>

Color grassCl = {99 * 1.25, 171 * 1.25, 63 * 1.25, 255};
Color swampcl = {47 * 1.25, 87 * 1.25, 83 * 1.25, 255};
Color water = {98, 164 * 1.25, 184 * 1.25, 255};

f32 globalPlanetScale = 1;
f32 atmosScale = 1.1;

Color averageColors(i32 count, ...) {
    va_list colors;
    va_start(colors, count);

    i32 r = 0;
    i32 g = 0;
    i32 b = 0;
    i32 a = 0;

    for (i32 i = 0; i < count; i++) {
        Color c = va_arg(colors, Color);
        r += c.r;
        g += c.g;
        b += c.b;
        a += c.a;
    }

    va_end(colors);

    r /= count;
    g /= count;
    b /= count;
    a /= count;

    return (Color){r, g, b, a};
}

// Convert RGB to HSL
void getSphericalTextureCoords(i32 x, i32 y, i32 width, i32 height, f32* u,
                               f32* v) {
    // Normalize x and y to [-1, 1]
    f32 nx = 2.0f * x / width - 1.0f;
    f32 ny = 2.0f * y / height - 1.0f;

    // Calculate the spherical coordinates
    f32 theta = atan2f(ny, nx);
    f32 r = sqrtf(nx * nx + ny * ny);
    f32 phi = r * PI / 2.0f; // Adjust this factor for more/less distortion

    // Convert back to texture coordinates
    *u = (theta / (2.0f * PI) + 0.5f) * width;
    *v = (phi / PI) * height;
}

// Function to apply the spherical distortion to the texture
void applySphericalDistortion(Image* image) {
    i32 width = image->width;
    i32 height = image->height;

    // Get pixel data from the image
    Color* pixels = LoadImageColors(*image);

    for (i32 y = 0; y < height; ++y) {
        for (i32 x = 0; x < width; ++x) {
            f32 u, v;
            getSphericalTextureCoords(x, y, width, height, &u, &v);

            // Sample the texture (clamp to edges)
            i32 srcX = (i32)fmaxf(0, fminf(width - 1, u));
            i32 srcY = (i32)fmaxf(0, fminf(height - 1, v));

            // Copy the pixel from the source texture to the distorted
            // texture
            ImageDrawPixel(image, x, y, pixels[srcY * width + srcX]);
        }
    }

    // Update the image with distorted pixel data

    // Free the temporary pixel arrays
    UnloadImageColors(pixels);
}

Image cropImageToCircle(Image img, bool shadow) {
    Color* pixels = LoadImageColors(img);

    for (i32 i = 0; i < img.width * img.height; i++) {
        i32 x = i % img.width;
        i32 y = i / img.width;

        // make pixel transparent if it's outside the circle
        // add shadow to the pixels that are close to the edge

        f32 xd = x - img.width / 2.0;
        f32 yd = y - img.height / 2.0;

        f32 dist = sqrt(pow(xd, 2) + pow(yd, 2));

        if (dist > (f32)img.width / 2 - 0.2) {
            ImageDrawPixel(&img, x, y, BLANK);
        } else if (shadow) {
            yd += 40;
            dist = sqrt(pow(xd, 2) + pow(yd * 0.5, 2));
            f32 param = (dist * dist) / 6;
            ImageDrawPixel(&img, x, y, setColorShadow(pixels[i], param));
        }
    }

    free(pixels);

    return img;
}

Image genAtmosphere(i32 size, Color color) {
    Image img = GenImageGradientRadial(size, size, 0.05f, BLANK, color);

    return img;
}

/*void colorPerlin(Color* pixels, Image* img, i32 imgSize, Color c1, Color
c2, Color c3) { Color cl; for (usize i = 0; i < imgSize * imgSize; i++) {
        i32 x = i % imgSize;
        i32 y = i / imgSize;
        usize diff;

        if (pixels[i].r < 100) {
            diff = 100 - pixels[i].r;
            cl = setColorShadow(c1, diff);
            ImageDrawPixel(img, x, y, cl);
        } else if (pixels[i].r < 150) {
            diff = 150 - pixels[i].r;
            cl = setColorShadow(c3, diff);

            ImageDrawPixel(img, x, y, cl);
        } else if (pixels[i].r < 200) {
            diff = 200 - pixels[i].r;
            cl = setColorShadow(c2, diff);
            ImageDrawPixel(img, x, y, cl);
        } else {
            diff = 255 - pixels[i].r;
            cl = setColorShadow(c2, diff);
            ImageDrawPixel(img, x, y, cl);
        }
    }
}*/

void colorPerlin(Color* pixels, Image* img, i32 imgSize,
                 std::vector<Color> colors, std::vector<i32> thresholds) {
    assert(colors.size() == thresholds.size() + 1 && "Invalid thresholds");

    for (usize i = 0; i < imgSize * imgSize; i++) {
        i32 x = i % imgSize;
        i32 y = i / imgSize;
        usize diff;
        Color cl;

        i32 r = pixels[i].r;
        usize idx = std::min(
            static_cast<usize>(std::distance(
                thresholds.begin(),
                std::lower_bound(thresholds.begin(), thresholds.end(), r))),
            colors.size() - 1);

        if (idx == 0) {
            diff = thresholds[0] - r;
        } else if (idx == thresholds.size()) {
            diff = 255 - r;
        } else {
            diff = thresholds[idx] - r;
        }

        cl = setColorShadow(colors[idx], diff);
        ImageDrawPixel(img, x, y, cl);
    }
}

Planet::Planet(i32 imgSize, bool randomizeColors) {
    i32 randomSeed = GetRandomValue(0, 1000);
    i32 thumbSize = 23;

    Image img = GenImagePerlinNoise(imgSize, imgSize, randomSeed,
                                    randomSeed * 2, 4);
    Image thumbnail = GenImagePerlinNoise(thumbSize, thumbSize, randomSeed,
                                          randomSeed * 2, 4);

    Color* imgPixels = LoadImageColors(img);
    Color* thumbnailPixels = LoadImageColors(thumbnail);
    Color c1 = water;
    Color c2 = grassCl;
    Color c3 = swampcl;
    Color atmoColor = SKYBLUE;

    if (randomizeColors) {
        Color otherColors[3];
        c1 = genRandomColor();
        GenerateAnalogousColors(c1, otherColors, 3);
        c2 = otherColors[0];
        c3 = otherColors[1];
        atmoColor = normalizeColor(otherColors[2]);
    }
    Color acs[4];
    GenerateAnalogousColors(c3, acs, 4);
    std::vector<Color> colors = {c1,     c2,     c3,    acs[0],
                                 acs[1], acs[2], acs[3]};
    std::vector<i32> thresholds = {50, 100, 120, 160, 200, 230};

    colorPerlin(imgPixels, &img, imgSize, colors, thresholds);
    colorPerlin(thumbnailPixels, &thumbnail, thumbSize, colors, thresholds);

    applySphericalDistortion(&img);
    applySphericalDistortion(&thumbnail);
    img = cropImageToCircle(img, true);
    thumbnail = cropImageToCircle(thumbnail, false);

    atmoColor.a -= 50;
    Image atmosphereImg =
        cropImageToCircle(genAtmosphere(imgSize * atmosScale, atmoColor),
                          true); // should be false;
    Image thumbnailAtmos = cropImageToCircle(
        genAtmosphere((thumbSize)*atmosScale, atmoColor), true);

    atmosphere = LoadTextureFromImage(atmosphereImg);
    thumbnailAtmosphere = LoadTextureFromImage(thumbnailAtmos);
    texture = LoadTextureFromImage(img);
    this->thumbnail = LoadTextureFromImage(thumbnail);

    free(imgPixels);
    free(thumbnailPixels);

    Color average = averageColors(3, c1, c2, c3);
    average = normalizeColor(average);

    Color* palette = (Color*)malloc(sizeof(Color) * 4);
    palette[0] = c1;
    palette[1] = c2;
    palette[2] = c3;
    atmoColor.a = 255;
    palette[3] = atmoColor;

    pos = {};
    size = imgSize;
    terrainColor = average;
    this->palette = palette;
}

void Planet::drawThumbnail(v2 pos) {
    Rect srcT = {0, 0, thumbnail.width, thumbnail.height};
    Rect destT = {pos.x, pos.y, thumbnail.width, thumbnail.height};

    Rect srcA = {0, 0, thumbnailAtmosphere.width,
                 thumbnailAtmosphere.height};

    // dest of the atmosphere considering that it is atmosScale times bigger
    // than the planet
    float atmosSize = thumbnail.width * atmosScale;
    Rect destA = {pos.x + (destT.width - atmosSize) / 2,
                  pos.y + (destT.height - atmosSize) / 2, atmosSize,
                  atmosSize};

    DrawTexturePro(thumbnail, srcT, destT, (v2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(thumbnailAtmosphere, srcA, destA, (v2){0, 0}, 0.0f,
                   WHITE);
}

void Planet::draw(f32 scale) {
    f32 planetScale = globalPlanetScale * scale;
    Rect srcP = {0, 0, size, size};
    Rect srcA = {0, 0, size * atmosScale, size * atmosScale};
    Rect destP = {pos.x, pos.y, size * planetScale, size * planetScale};

    // Draw the planet texture
    DrawTexturePro(texture, srcP, destP, (v2){0, 0}, 0.0f, WHITE);

    float atmosSize = size * planetScale * atmosScale;
    Rect destA = {pos.x + (destP.width - atmosSize) / 2,
                  pos.y + (destP.height - atmosSize) / 2, atmosSize,
                  atmosSize};

    DrawTexturePro(atmosphere, srcA, destA, (v2){0, 0}, 0.0f, WHITE);
}

Planet::~Planet() {
    UnloadTexture(texture);
    UnloadTexture(atmosphere);
    UnloadTexture(thumbnail);
    UnloadTexture(thumbnailAtmosphere);
    free(palette);
}
