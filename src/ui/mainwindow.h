#pragma once
#include <string>
#include <windows.h>
#include "../core/player.h"

class MainWindow {
public:
    MainWindow();
    ~MainWindow();
    
    void show();
    void hide();
    void setTitle(const std::string& title);
    
    // Callbacks
    void onPlayPause();
    void onOpenFile();
    void onSeek(int position);  // 0-100
    void onVolumeChange(int volume);  // 0-200
    void onVolumeUp();
    void onVolumeDown();
    void onMagicSync();
    void onToggleBoost();

private:
    Player* player;
    HWND hwnd;
    HFONT hRobotoFont;
    HWND hVolumeSlider;
    HWND hVolumeLabel;
    int current_volume = 100;
    
    void createWindow();
    void createControls();
    void messageLoop();
    void updateVolumeDisplay();
    
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
