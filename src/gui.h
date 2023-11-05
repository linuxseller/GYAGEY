#pragma once
#include <math.h>
#include <raylib.h>
#include <stdlib.h>
#define DEFAULT_W 720
#define DEFAULT_H 480
#define SPACING 10*scale
#define FONT_FILENAME "resources/fonts/OpenSans-Regular.ttf"
#define TEXTBPSSPEED 10
#define JET (Color){48,48,48, 255}

typedef struct {
    Rectangle rec;
    char *text;
    Color bdColor;
    Color bgColor;
    Color hoverColor;
    Color hoverColorText;
    int lenPx;
} Button;

#define DRAWBTN(btn){ \
    bool hovered = CheckCollisionPointRec(mousePosition, btn.rec);\
    DrawRectangleRounded(btn.rec, 0.5, 5, (hovered)?btn.hoverColor:btn.bgColor); \
    DrawRectangleRoundedLines(btn.rec, 0.5, 5, 2, btn.bdColor); \
    DrawTextEx(gameFont, btn.text, (Vector2){btn.rec.x+5,btn.rec.y+5}, textSize, 1, (!hovered)?btn.bdColor:btn.hoverColorText); \
}


void ShowErrorFrontend(const char *errText, const char *elaboration);

void GuiSlider(Rectangle bounds, float *value, float minValue, float maxValue);
