#include <iostream>
#include <raylib.h>
#include "BytePusher.hpp"

Color GetColor(uint8_t index){
    if (index >= 216) return BLACK;
    uint8_t r6 = index / 36;
    uint8_t g6 = (index / 6) % 6;
    uint8_t b6 = index % 6;
    return Color{r6 * 51, g6 * 51, b6 * 51, 255};
}



int main(int argc, char** argv)
{
    BytePusher bp{};
    bp.LoadROM(argv[1]);
    
    InitWindow(512, 512, "ByteBytePush");
    Image Icon = LoadImage("home.png");
    SetWindowIcon(Icon);
    UnloadImage(Icon);
    
    SetTargetFPS(60); 
    
    Image img = GenImageColor(256, 256, BLACK);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(256);
    AudioStream stream = LoadAudioStream(15360, 32, 1);
    PlayAudioStream(stream);
    
    while (!WindowShouldClose()){
        bp.keypad = 0;
        if (IsKeyDown(KEY_ONE))    bp.keypad |= (1 << 0);   // 1  
        if (IsKeyDown(KEY_TWO))    bp.keypad |= (1 << 1);   // 2  
        if (IsKeyDown(KEY_THREE))  bp.keypad |= (1 << 2);   // 3   
        if (IsKeyDown(KEY_FOUR))   bp.keypad |= (1 << 3);   // C   
        if (IsKeyDown(KEY_Q))      bp.keypad |= (1 << 4);   // 4   
        if (IsKeyDown(KEY_W))      bp.keypad |= (1 << 5);   // 5   
        if (IsKeyDown(KEY_E))      bp.keypad |= (1 << 6);   // 6   
        if (IsKeyDown(KEY_R))      bp.keypad |= (1 << 7);   // D   
        if (IsKeyDown(KEY_A))      bp.keypad |= (1 << 8);   // 7   
        if (IsKeyDown(KEY_S))      bp.keypad |= (1 << 9);   // 8   
        if (IsKeyDown(KEY_D))      bp.keypad |= (1 << 10);  // 9   
        if (IsKeyDown(KEY_F))      bp.keypad |= (1 << 11);  // E   
        if (IsKeyDown(KEY_Z))      bp.keypad |= (1 << 12);  // A   
        if (IsKeyDown(KEY_X))      bp.keypad |= (1 << 13);  // 0   
        if (IsKeyDown(KEY_C))      bp.keypad |= (1 << 14);  // B   
        if (IsKeyDown(KEY_V))      bp.keypad |= (1 << 15);  // F   
        bp.Tick();
        float samples[256];
        for (int i = 0; i < 256; ++i) {
            int8_t s = (int8_t)bp.memory[(bp.audioPage << 8) + i];
            samples[i] = s / 128.0f;
        }
        if (IsAudioStreamProcessed(stream)) {
            UpdateAudioStream(stream, samples, 256);
        }
        
        Color framebuffer[256 * 256];
        uint32_t base = bp.pixelPage << 16;
        for (int i = 0; i < 256 * 256; ++i)
        framebuffer[i] = GetColor(bp.memory[base + i]);
        
        UpdateTexture(tex, framebuffer);

        BeginDrawing();
        
        DrawTextureEx(tex, {0.0f, 0.0f}, 0.0f, 2.0f, WHITE);
        EndDrawing();
    }
    UnloadTexture(tex);
    UnloadAudioStream(stream);
    CloseAudioDevice();

    CloseWindow();
}