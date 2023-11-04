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


void ShowErrorFrontend(const char *errText, const char *elaboration);

void GuiSlider(Rectangle bounds, float *value, float minValue, float maxValue);
