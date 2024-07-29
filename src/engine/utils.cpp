#include "utils.h"
#include <math.h>

f32 lerp(f32 a, f32 b, f32 t) { return a + (b - a) * t; }

f32 lerpAngle(float a, float b, float t) {
    float difference = fmod(b - a + 360.0f, 360.0f);
    if (difference > 180.0f) {
        difference -= 360.0f;
    }
    return a + t * difference;
}

f32 clampAngle(f32 angle) {
    if (angle < 0) {
        angle += 360;
    }
    if (angle > 360) {
        angle -= 360;
    }
    return angle;
}

bool lineLineIntersection(v2 p1, v2 p2, v2 p3, v2 p4) {
    // clang-format off
    f32 x1 = p1.x; f32 y1 = p1.y;
    f32 x2 = p2.x; f32 y2 = p2.y;
    f32 x3 = p3.x; f32 y3 = p3.y;
    f32 x4 = p4.x; f32 y4 = p4.y;
    // clang-format on

    f32 uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) /
             ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    f32 uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) /
             ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        return true;
    }

    return false;
}

v2 RotatePoint(v2 point, v2 origin, f32 angle) {
    f32 s = sinf(angle * DEG2RAD);
    f32 c = cosf(angle * DEG2RAD);

    // Translate point back to origin
    point.x -= origin.x;
    point.y -= origin.y;

    // Rotate point
    f32 xnew = point.x * c - point.y * s;
    f32 ynew = point.x * s + point.y * c;

    // Translate point back
    point.x = xnew + origin.x;
    point.y = ynew + origin.y;

    return point;
}

// used in main
void drawScaledWindow(RenderTexture2D target, f32 sw, f32 sh, f32 scale) {
    f32 tw = (f32)target.texture.width;
    f32 th = (f32)target.texture.height;
    Rect rect1 = {0.0f, 0.0f, tw, -th};
    f32 x = (GetScreenWidth() - (sw * scale)) * 0.5f;
    f32 y = (GetScreenHeight() - (sh * scale)) * 0.5f;

    Rect rect2 = {x, y, sw * scale, sh * scale};

    DrawTexturePro(target.texture, rect1, rect2, (v2){0, 0}, 0.0f, WHITE);
}

v2 getScreenMousePos(v2* mouse, f32 scale, i32 sw, i32 sh) {
    v2 mouseOLD = GetMousePosition();
    mouse->x =
        (mouseOLD.x - (GetScreenWidth() - (sw * scale)) * 0.5f) / scale;
    mouse->y =
        (mouseOLD.y - (GetScreenHeight() - (sh * scale)) * 0.5f) / scale;
    *mouse = v2Clamp(*mouse, (v2){0, 0}, (v2){(f32)sw, (f32)sh});

    return *mouse;
}

v2 v2Clamp(v2 vec, v2 min, v2 max) {
    return (v2){MIN(MAX(vec.x, min.x), max.x),
                MIN(MAX(vec.y, min.y), max.y)};
}

f32 getAngleToPoint(v2 a, v2 b) {
    v2 diff;
    diff.x = b.x - a.x;
    diff.y = b.y - a.y;
    return atan2(diff.y, diff.x) * RAD2DEG;
}

void RGBtoHSL(Color color, f32* h, f32* s, f32* l) {
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
            if (t[i] < 0.0f) t[i] += 1.0f;
            if (t[i] > 1.0f) t[i] -= 1.0f;

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

    i32 rn = r * 255.0;
    i32 gn = g * 255.0;
    i32 bn = b * 255.0;

    Color ret = {rn, gn, bn, 255};

    return ret;
}

// Generate analogous colors
void GenerateAnalogousColors(Color baseColor, Color* analogousColors,
                             i32 count) {
    f32 h, s, l;
    RGBtoHSL(baseColor, &h, &s, &l);

    f32 hueStep = 30.0f / 360.0f; // 30 degrees in hue

    for (i32 i = 0; i < count; ++i) {
        f32 newHue = h + hueStep * (i - count / 2.0);
        if (newHue < 0.0f) newHue += 1.0f;
        if (newHue > 1.0f) newHue -= 1.0f;

        analogousColors[i] = HSLtoRGB(newHue, s, l);
    }
}

Color setColorShadow(Color c, i32 shadow) {
    Color newColor = c;

    shadow /= 2;

    i32 r = (i32)c.r - shadow;
    i32 g = (i32)c.g - shadow;
    i32 b = (i32)c.b - shadow;

    newColor = clampColor(r, g, b, c.a);
    f32 h, s, l;
    RGBtoHSL(newColor, &h, &s, &l);
    h *= 1 + shadow / 800.0;
    newColor = HSLtoRGB(h, s, l);
    newColor = clampColor(newColor.r, newColor.g, newColor.b, c.a);
    return newColor;
}

Color saturateColor(Color cl, f32 mult) {
    cl.r *= mult;
    cl.g *= mult;
    cl.b *= mult;
    return cl;
}

Color clampColor(i32 r, i32 g, i32 b, i32 a) {
    Color newColor;
    newColor.r = MIN(MAX(r, 0), 255);
    newColor.g = MIN(MAX(g, 0), 255);
    newColor.b = MIN(MAX(b, 0), 255);
    newColor.a = MIN(MAX(a, 0), 255);

    return newColor;
}

Color normalizeColor(Color c) {
    // turn color as bright as possible
    f32 max = MAX(MAX(c.r, c.g), c.b);
    f32 mult = 255 / max;

    c.r *= mult;
    c.g *= mult;
    c.b *= mult;

    return c;
}

Color genRandomColor() {
    return (Color){GetRandomValue(0, 255), GetRandomValue(0, 255),
                   GetRandomValue(0, 255), 255};
}
