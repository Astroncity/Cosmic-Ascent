#include "planet.h"
#include "defs.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

Color grassCl = {99 * 1.25, 171 * 1.25, 63 * 1.25, 255};
Color swampcl = {47 * 1.25, 87 * 1.25, 83 * 1.25, 255};
Color water = {98, 164 * 1.25, 184 * 1.25, 255};

f32 planetScale = 5;
f32 atmosScale = 1.05;

// Convert RGB to HSL
void RGBtoHSL(Color color, f32 *h, f32 *s, f32 *l) {
    f32 r = color.r / 255.0f;
    f32 g = color.g / 255.0f;
    f32 b = color.b / 255.0f;

    f32 max = fmaxf(r, fmaxf(g, b));
    f32 min = fminf(r, fminf(g, b));

    *l = (max + min) / 2.0f;

    if (max == min) {
        *h = *s = 0.0f; // achromatic
    } else {
        f32 d = max - min;
        *s = *l > 0.5f ? d / (2.0f - max - min) : d / (max + min);

        if (max == r) {
            *h = (g - b) / d + (g < b ? 6.0f : 0.0f);
        } else if (max == g) {
            *h = (b - r) / d + 2.0f;
        } else if (max == b) {
            *h = (r - g) / d + 4.0f;
        }

        *h /= 6.0f;
    }
}

// Convert HSL to RGB
Color HSLtoRGB(f32 h, f32 s, f32 l) {
    f32 r, g, b;

    if (s == 0.0f) {
        r = g = b = l; // achromatic
    } else {
        f32 q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        f32 p = 2.0f * l - q;
        f32 t[3] = {h + 1.0f / 3.0f, h, h - 1.0f / 3.0f};

        for (i32 i = 0; i < 3; ++i) {
            if (t[i] < 0.0f)
                t[i] += 1.0f;
            if (t[i] > 1.0f)
                t[i] -= 1.0f;

            if (t[i] < 1.0f / 6.0f) {
                t[i] = p + (q - p) * 6.0f * t[i];
            } else if (t[i] < 1.0f / 2.0f) {
                t[i] = q;
            } else if (t[i] < 2.0f / 3.0f) {
                t[i] = p + (q - p) * (2.0f / 3.0f - t[i]) * 6.0f;
            } else {
                t[i] = p;
            }
        }

        r = t[0];
        g = t[1];
        b = t[2];
    }

    return (Color){(u8)(r * 255.0f), (u8)(g * 255.0f), (u8)(b * 255.0f), 255};
}

// Generate analogous colors
void GenerateAnalogousColors(Color baseColor, Color *analogousColors,
                             i32 count) {
    f32 h, s, l;
    RGBtoHSL(baseColor, &h, &s, &l);

    f32 hueStep = 30.0f / 360.0f; // 30 degrees in hue

    for (i32 i = 0; i < count; ++i) {
        f32 newHue = h + hueStep * (i - count / 2.0);
        if (newHue < 0.0f)
            newHue += 1.0f;
        if (newHue > 1.0f)
            newHue -= 1.0f;

        analogousColors[i] = HSLtoRGB(newHue, s, l);
    }
}

inline Color clampColor(i32 r, i32 g, i32 b, i32 a) {
    Color newColor;
    newColor.r = MIN(MAX(r, 0), 255);
    newColor.g = MIN(MAX(g, 0), 255);
    newColor.b = MIN(MAX(b, 0), 255);
    newColor.a = MIN(MAX(a, 0), 255);

    return newColor;
}

inline Color setColorShadow(Color c, i32 shadow) {
    Color newColor = c;

    shadow /= 2;

    i32 r = (i32)c.r - shadow;
    i32 g = (i32)c.g - shadow;
    i32 b = (i32)c.b - shadow;

    newColor = clampColor(r, g, b, c.a);

    return newColor;
}

Color saturateColor(Color cl, f32 mult) {
    cl.r *= mult;
    cl.g *= mult;
    cl.b *= mult;
    return cl;
}

void getSphericalTextureCoords(i32 x, i32 y, i32 width, i32 height, f32 *u,
                               f32 *v) {
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
void applySphericalDistortion(Image *image) {
    i32 width = image->width;
    i32 height = image->height;

    // Get pixel data from the image
    Color *pixels = LoadImageColors(*image);

    for (i32 y = 0; y < height; ++y) {
        for (i32 x = 0; x < width; ++x) {
            f32 u, v;
            getSphericalTextureCoords(x, y, width, height, &u, &v);

            // Sample the texture (clamp to edges)
            i32 srcX = (i32)fmaxf(0, fminf(width - 1, u));
            i32 srcY = (i32)fmaxf(0, fminf(height - 1, v));

            // Copy the pixel from the source texture to the distorted texture
            ImageDrawPixel(image, x, y, pixels[srcY * width + srcX]);
        }
    }

    // Update the image with distorted pixel data

    // Free the temporary pixel arrays
    UnloadImageColors(pixels);
}

Image cropImageToCircle(Image img, bool shadow) {
    Color *pixels = LoadImageColors(img);

    for (i32 i = 0; i < img.width * img.height; i++) {
        i32 x = i % img.width;
        i32 y = i / img.width;

        // make pixel transparent if it's outside the circle
        // add shadow to the pixels that are close to the edge

        f32 xd = x - img.width / 2.0;
        f32 yd = y - img.height / 2.0;

        f32 dist = sqrt(pow(xd, 2) + pow(yd, 2));

        if (dist > (f32)img.width / 2) {
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

inline Color normalizeColor(Color c) {
    // turn color as bright as possible
    f32 max = MAX(MAX(c.r, c.g), c.b);
    f32 mult = 255 / max;

    c.r *= mult;
    c.g *= mult;
    c.b *= mult;

    return c;
}

Image genAtmosphere(i32 size, Color color) {
    Image img = GenImageGradientRadial(size, size, 0.05f, BLANK, color);

    return img;
}

inline Color genRandomColor() {
    return (Color){GetRandomValue(0, 255), GetRandomValue(0, 255),
                   GetRandomValue(0, 255), 255};
}

Planet genPlanet(i32 imgSize, bool randomizeColors) {
    i32 randomSeed = GetRandomValue(0, 1000);

    // Image img = GenImagePerlinNoise(imgSize, imgSize, 5, 5 * 5, 4);
    Image img =
        GenImagePerlinNoise(imgSize, imgSize, randomSeed, randomSeed * 2, 4);

    Color *pixels = LoadImageColors(img);
    Color c1 = water;
    Color c2 = grassCl;
    Color c3 = swampcl;
    Color cl;
    Color atmoColor = SKYBLUE;

    if (randomizeColors) {
        Color otherColors[3];
        c1 = genRandomColor();
        GenerateAnalogousColors(c1, otherColors, 3);
        c2 = otherColors[0];
        c3 = otherColors[1];
        atmoColor = normalizeColor(otherColors[2]);
    }

    for (usize i = 0; i < imgSize * imgSize; i++) {
        i32 x = i % imgSize;
        i32 y = i / imgSize;
        usize diff;

        if (pixels[i].r < 100) {
            diff = 100 - pixels[i].r;
            cl = setColorShadow(c1, diff);
            ImageDrawPixel(&img, x, y, cl);
        } else if (pixels[i].r < 150) {
            diff = 150 - pixels[i].r;
            cl = setColorShadow(c3, diff);

            ImageDrawPixel(&img, x, y, cl);
        } else if (pixels[i].r < 200) {
            diff = 200 - pixels[i].r;
            cl = setColorShadow(c2, diff);
            ImageDrawPixel(&img, x, y, cl);
        } else {
            diff = 255 - pixels[i].r;
            cl = setColorShadow(c2, diff);
            ImageDrawPixel(&img, x, y, cl);
        }
    }

    Image streched = ImageCopy(img);
    img = cropImageToCircle(img, true);

    applySphericalDistortion(&streched);
    streched = cropImageToCircle(streched, true);

    atmoColor.a -= 50;
    Image atmosphereImg =
        cropImageToCircle(genAtmosphere(imgSize * atmosScale, atmoColor),
                          true); // should be false;

    Texture2D tex = LoadTextureFromImage(img);
    Texture2D atmosphere = LoadTextureFromImage(atmosphereImg);
    Texture2D test = LoadTextureFromImage(streched);

    free(pixels);

    Planet planet = {tex, test, atmosphere, (Vector2){0, 0}, imgSize};

    return planet;
}

void drawPlanet(Planet *planet) {
    Rectangle srcP = {0, 0, planet->size, planet->size};
    Rectangle srcA = {0, 0, planet->size * atmosScale,
                      planet->size * atmosScale};
    Rectangle destP = {planet->pos.x, planet->pos.y, planet->size * planetScale,
                       planet->size * planetScale};

    // Draw the planet texture
    DrawTexturePro(planet->test, srcP, destP, (Vector2){0, 0}, 0.0f, WHITE);

    float atmosSize = planet->size * planetScale * atmosScale;
    Rectangle destA = {planet->pos.x + (destP.width - atmosSize) / 2,
                       planet->pos.y + (destP.height - atmosSize) / 2,
                       atmosSize, atmosSize};

    DrawTexturePro(planet->atmosphere, srcA, destA, (Vector2){0, 0}, 0.0f,
                   WHITE);
}
