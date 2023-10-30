#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CLOGGER_IMPL
#include "libs/clogger.h"
//#define JSONLOG
#define JSON_IMPL
#include "libs/cJsonParser.h"
#define ASSLIST_IMPL
#include "libs/AssList.h"

#define DEFAULT_W 720
#define DEFAULT_H 480
#define SPACING 10*scale
#define FONT_FILENAME "resources/fonts/OpenSans-Regular.ttf"
#define TEXTSPEED 10

#define JET (Color){48,48,48, 255}

int min(int x, int y){
  return (x < y) ? x : y;
}

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

#define ENTRYCHECK(str){ \
    if(strcmp(jsonEntry, str)!=0){ \
        clogger(CLOGGER_ERROR, "scene json format error in %s: got %s\n", str, jsonEntry); \
        exit(1); \
    } \
}

typedef struct {
    int number;
    Llist *characterLl;
    Texture2D *background;
    Llist *voiceLineLl;
    Music *music;
} Scene;

typedef struct {
    char *name;
    char *line;
} VoiceLine;

void GuiSlider(Rectangle bounds, float *value, float minValue, float maxValue){
    float temp = (maxValue - minValue)/2.0f;
    if (value == NULL) value = &temp;
    int sliderValue = (int)(((*value - minValue)/(maxValue - minValue))*(bounds.width));
    Rectangle slider = { bounds.x, bounds.y, 0, bounds.height};
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

Scene *ParseSceneJson(char **gameJson){
    Scene *scene = malloc(sizeof(Scene));

    char *jsonEntry = JsonPEAS(gameJson);
    sscanf(jsonEntry, "%d", &scene->number);

    jsonEntry = JsonPEAS(gameJson);
    ENTRYCHECK("Background")
    Texture2D tmpTxt = LoadTexture(JsonPEAS(gameJson));
    {
    scene->background = malloc(sizeof(Texture2D));
    scene->background->id      = tmpTxt.id;
    scene->background->width   = tmpTxt.width;
    scene->background->height  = tmpTxt.height;
    scene->background->mipmaps = tmpTxt.mipmaps;
    scene->background->format  = tmpTxt.format;
    }

    jsonEntry = JsonPEAS(gameJson);
    ENTRYCHECK("Music")
    Music tmpMusic = LoadMusicStream(JsonPEAS(gameJson));
    {
    scene->music = malloc(sizeof(Music));
    scene->music->stream     =tmpMusic.stream;
    scene->music->frameCount =tmpMusic.frameCount;
    scene->music->looping    =tmpMusic.looping;
    scene->music->ctxType    =tmpMusic.ctxType;
    scene->music->ctxData    =tmpMusic.ctxData;
    }

    jsonEntry = JsonPEAS(gameJson);
    ENTRYCHECK("Characters")
    scene->characterLl = JsonParseStringArrToLl(gameJson);

    jsonEntry = JsonPEAS(gameJson);
    ENTRYCHECK("Frases")
    Llist *lineList = malloc(sizeof(Llist));
    lineList->ptr=NULL;
    lineList->size=0;
    jsonEntry = JsonPEAS(gameJson);
    while(strcmp(jsonEntry, "END")!=0){
        VoiceLine *vl = malloc(sizeof(*vl));
        vl->name = jsonEntry;
        jsonEntry = JsonPEAS(gameJson);
        vl->line = jsonEntry;
        jsonEntry = JsonPEAS(gameJson);
        LlAppend(lineList, vl);
    }
    jsonEntry = JsonPEAS(gameJson);
    scene->voiceLineLl = lineList;
    return scene;
}
//printf("Hello My Froinde!\n"); exit(0);
int main(int argc, char **argv){
    char *gameJsonFileName = "game.json";
    if(argc==2){
        gameJsonFileName = argv[1];
        //clogger(CLOGGER_ERROR, "game json file not provided.\n");
        //exit(1);
    }
    // Raylib initialising.
    int screenWidth = DEFAULT_W;
    int screenHeight = DEFAULT_H;
    int screenHalfX = screenWidth/2;
    int screenHalfY = screenHeight/2;
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(screenWidth, screenHeight, "VisNov game Engine");
    SetTargetFPS(120);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();
    SetExitKey(KEY_F10);
    bool menuOpened     = true;
    bool audioMuted     = true;
    float audioVolume     = 0.5;
    SetMasterVolume((audioMuted)?0:audioVolume);
    bool settingsOpened = false;
    int tmp;
    int framesCounter = 0;
    //-Raylib initialising.-

    // Variables setup.
    bool ending = false;
    // Loading characters info [Name:Image]
    int codepoints[512] = { 0 };
    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;   // Basic ASCII characters
    for (int i = 0; i < 255; i++) codepoints[96 + i] = 0x400 + i;   // Cyrillic characters
    Font gameFont = LoadFontEx(FONT_FILENAME, 32, codepoints, 512);
    char *gameJson = JsonReadWholeFile(gameJsonFileName);
    char *strptrcpy = gameJson; // free later
    char *jsonEntry = JsonPEAS(&gameJson);
    ENTRYCHECK("Game")

    jsonEntry = JsonPEAS(&gameJson);
    ENTRYCHECK("Name")
    char *gameName = JsonPEAS(&gameJson);
    SetWindowTitle(gameName);
    jsonEntry = JsonPEAS(&gameJson);
    ENTRYCHECK("MenuBg")
    Texture2D bgMenu = LoadTexture(JsonPEAS(&gameJson));

    jsonEntry = JsonPEAS(&gameJson);
    ENTRYCHECK("MenuMusic")
    Music menuMusic = LoadMusicStream(JsonPEAS(&gameJson));

    AssList assListObj = AssInit();
    AssList *assList = &assListObj;
    do{
        jsonEntry = JsonPEAS(&gameJson);
        if(strcmp(jsonEntry, "Scenes")==0){
            break;
        }
        Texture2D tmpTxt = LoadTexture(JsonPEAS(&gameJson));
        Texture2D *tmpTxtPtr2 = malloc(sizeof(Texture2D));
        tmpTxtPtr2->id      = tmpTxt.id;
        tmpTxtPtr2->width   = tmpTxt.width;
        tmpTxtPtr2->height  = tmpTxt.height;
        tmpTxtPtr2->mipmaps = tmpTxt.mipmaps;
        tmpTxtPtr2->format  = tmpTxt.format;
        AssAppend(assList, jsonEntry, tmpTxtPtr2);
    } while(true);

    // Loading Scene Info
    int sceneId = 0;
    Llist sceneLlObj = LlInit();
    Llist *sceneLl = &sceneLlObj;
    while(JsonHasNextEntry(gameJson)){
        LlAppend(sceneLl, ParseSceneJson(&gameJson));
    }
    Scene scene = *(Scene *)LlGetAt(sceneLl, sceneId++);
    
    char *nameStrPtr;
    char *textStrPtr;
    int voiceLineId=0;
    
    //-Variables setup.-

    while(!WindowShouldClose()){
        // Global game variables. DO NOT TOUCH UNTIL YOU KNOW WHAT YOU ARE DOING
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        screenHalfX = screenWidth/2;
        screenHalfY = screenHeight/2;
        float scale = fminf((float)screenHeight/DEFAULT_H, (float)screenWidth/DEFAULT_W);
        float textSize = scale*20;
        Vector2 mousePosition = GetMousePosition();
        if(IsKeyPressed(KEY_ESCAPE)){
            settingsOpened = !settingsOpened;
        }
        //-Global game variables.-
        if(settingsOpened){
            tmp = MeasureTextEx(gameFont, "Mute", textSize, 1).x;
            Button muteBtn = (Button){(Rectangle){screenHalfX-tmp/2-100, screenHalfY, SPACING+tmp, SPACING+textSize},
                                        "Mute", BLACK, (audioMuted)?GRAY:WHITE, JET, RAYWHITE, tmp};
            if(CheckCollisionPointRec(mousePosition, muteBtn.rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                SetMasterVolume((audioMuted)?0:audioVolume);
                audioMuted=!audioMuted;
            }
            BeginDrawing();
                ClearBackground(BLACK);
                DRAWBTN(muteBtn)
                GuiSlider((Rectangle){screenHalfX-50, muteBtn.rec.y+SPACING/2, 200, textSize}, &audioVolume, 0, 1);
                SetMasterVolume((audioMuted)?0:audioVolume);
            EndDrawing();
            continue;
        }
        if(menuOpened){
            if(!IsMusicStreamPlaying(menuMusic)){
                PlayMusicStream(menuMusic);
            }
            tmp = MeasureTextEx(gameFont, "Start Game", textSize, 1).x;
            Rectangle startBtnRec = (Rectangle){screenHalfX-tmp/2, screenHalfY, SPACING+tmp, SPACING+textSize};
            Button startBtn = (Button){startBtnRec, "Start Game", BLACK, WHITE, JET, RAYWHITE, tmp};
            tmp = MeasureTextEx(gameFont, "Settings", textSize, 1).x;
            Rectangle settingsBtnRec = (Rectangle){screenHalfX-tmp/2, screenHalfY+startBtn.rec.height+SPACING, SPACING+tmp, SPACING+textSize};
            Button settingsBtn = (Button){settingsBtnRec, "Settings", BLACK, WHITE, JET, RAYWHITE, tmp};

            if(CheckCollisionPointRec(mousePosition, startBtn.rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                sceneId = voiceLineId = 0;
                scene = *(Scene *)LlGetAt(sceneLl, sceneId++);
                VoiceLine *vl = LlGetAt(scene.voiceLineLl, voiceLineId);
                nameStrPtr = vl->name;
                textStrPtr = vl->line;

                menuOpened = false;
                if(audioMuted){
                    SetMasterVolume(0);
                }
                StopMusicStream(menuMusic);
                PlayMusicStream(*scene.music);
            }
            if(CheckCollisionPointRec(mousePosition, settingsBtn.rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                settingsOpened = true;
            }
            UpdateMusicStream(menuMusic);
            BeginDrawing();
                ClearBackground(GREEN);
                float bgscale = fmaxf((float)screenHeight/(bgMenu.height), (float)screenWidth/(bgMenu.width));
                DrawTextureEx(bgMenu, (Vector2){0,0}, 0, bgscale, WHITE);
                Vector2 gameNamePos = (Vector2){screenHalfX-MeasureTextEx(gameFont, gameName, textSize*3, 1).x/2, SPACING*3};
                DrawTextEx(gameFont, gameName, gameNamePos, textSize*3, 1, WHITE);
                DRAWBTN(startBtn)
                DRAWBTN(settingsBtn)
            EndDrawing();
            continue;
        }
        if(ending){
            if(IsKeyPressed(KEY_R)){
                menuOpened=true;
                ending=false;
                continue;
            }
            BeginDrawing();
                ClearBackground(GREEN);
                float bgscale = fmaxf((float)screenHeight/(bgMenu.height), (float)screenWidth/(bgMenu.width));
                DrawTextureEx(bgMenu, (Vector2){0,0}, 0, bgscale, WHITE);
                Vector2 endTextPos = (Vector2){screenHalfX-MeasureTextEx(gameFont, "The End.", textSize*5, 1).x/2, screenHalfY-textSize*5/2};
                DrawTextEx(gameFont, "The End.", endTextPos, textSize*5, 1, WHITE);
            EndDrawing();
            continue;
        } 
        // Game part HERE.
        // Update variables.
        if(IsKeyPressed(KEY_SPACE)){
            if(framesCounter/TEXTSPEED<(int)strlen(textStrPtr)){
                framesCounter=10000;
                goto drawing;
            }
            framesCounter=0;
            voiceLineId++;
            if(voiceLineId>=scene.voiceLineLl->size){
                StopMusicStream(*scene.music);
                if(sceneId+1>sceneLl->size){
                    ending=true;
                    continue;
                }
                scene = *(Scene *)LlGetAt(sceneLl, sceneId++);
                PlayMusicStream(*scene.music);
                voiceLineId = 0;
            }
            VoiceLine *vl = LlGetAt(scene.voiceLineLl, voiceLineId);
            nameStrPtr = vl->name;
            textStrPtr = vl->line;
        }
        Rectangle textRec = (Rectangle){SPACING, 2*screenHeight/3, screenWidth-2*SPACING, screenHeight/3-2*SPACING};
        Rectangle nameRec = (Rectangle){SPACING, 2*screenHeight/3-20*scale-SPACING, MeasureTextEx(gameFont, nameStrPtr, textSize, 1).x+SPACING, textSize+SPACING};
        //-Update variables.-
drawing:
        UpdateMusicStream(*scene.music);
        BeginDrawing();
            ClearBackground(GRAY);
            float bgscale = fmaxf((float)screenHeight/(scene.background->height), (float)screenWidth/(scene.background->width));
            DrawTextureEx(*scene.background, (Vector2){0,0}, 0, bgscale, WHITE);
            // Drawing characters at scene.
            for(int i=0, n = scene.characterLl->size; i<scene.characterLl->size; i++){
                char      *characterName    = LlGetAt(scene.characterLl, i);
                if(strcmp(characterName, "NONE")==0){break;}
                Texture2D  characterTexture = *(Texture2D*)AssGet(assList, characterName);
                float      characterScale   = fminf((float)screenWidth/(2*characterTexture.width), (float)screenHeight/(2*characterTexture.height)); // get scale depending on biggest dimention of texture
                if(strcmp(characterName, nameStrPtr)==0){characterScale*=1.1;}
                Vector2    characterPos     = (Vector2){screenWidth/(2*n)+i*screenWidth/n-characterTexture.width*characterScale/2, screenHeight/4};
                DrawTextureEx(characterTexture, characterPos, 0, characterScale, WHITE);
            }
            //-Drawing characters at scene.-
            DrawRectangleRounded(     nameRec, 0.5, 5,    GRAY);
            DrawRectangleRoundedLines(nameRec, 0.5, 5, 2, BLACK);
            DrawRectangleRounded(     textRec, 0.1, 5,    (Color){200, 200, 200, 150});
            DrawRectangleRoundedLines(textRec, 0.1, 5, 2, BLACK);
            DrawTextEx(gameFont, TextSubtext(textStrPtr, 0, framesCounter/TEXTSPEED), (Vector2){SPACING+SPACING, 2*screenHeight/3+SPACING}, textSize, 1, BLACK);
            DrawTextEx(gameFont, nameStrPtr, (Vector2){nameRec.x+SPACING/2, nameRec.y+SPACING/2}, textSize, 1, BLACK);
        EndDrawing();
        framesCounter+=2;
    }
    CloseWindow();
    // free shit out of RAM
    free(strptrcpy);
    return 0;
}
