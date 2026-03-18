#pragma once
#include <string>
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
    void onSeek(int position);
    void onVolumeChange(int volume);
    void onMagicSync();

private:
    Player* player;
    void* hwnd; // Window handle
    HFONT hRobotoFont; // Police Roboto
    
    void createWindow();
    void messageLoop();
    void loadRobotoFont();
    void applyTheme(const std::string& theme);
    
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
