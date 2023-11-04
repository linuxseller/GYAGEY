#include "gui.h"
void ShowErrorFrontend(const char *errText, const char *elaboration){
    int screenWidth, screenHeight, screenHalfX, screenHalfY;
    while(!WindowShouldClose()){
        ClearBackground(BLACK);
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        screenHalfX = screenWidth/2;
        screenHalfY = screenHeight/2;
        float scale = fminf((float)screenHeight/DEFAULT_H, (float)screenWidth/DEFAULT_W);
        float textSize = scale*20;
        BeginDrawing();
            DrawText(errText, screenHalfX - MeasureText(errText, textSize)/2, screenHalfY-3*textSize/2, textSize, RED);
            DrawText(elaboration, screenHalfX - MeasureText(elaboration, textSize)/2, screenHalfY+3*textSize/2, textSize, RED);
        EndDrawing();
    }
}
void GuiSlider(Rectangle bounds, float *value, float minValue, float maxValue){
    float temp = (maxValue - minValue)/2.0f;
    if (value == NULL) value = &temp;
    int sliderValue = (int)(((*value - minValue)/(maxValue - minValue))*(bounds.width));
    Rectangle slider = {bounds.x, bounds.y, 0, bounds.height};
    slider.width = (float)sliderValue;
    Vector2 mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, bounds)&& IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        *value = ((maxValue - minValue)*(mousePoint.x - bounds.x))/(float)(bounds.width) + minValue;
        if (*value > maxValue) *value = maxValue;
        else if (*value < minValue) *value = minValue;
    }
    DrawRectangleRec(bounds, RAYWHITE);
    DrawRectangleRec(slider, JET);
}

