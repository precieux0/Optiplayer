#pragma once
#include <string>
#include <windows.h>  // IMPORTANT: pour HWND, HFONT, etc.

class Player;  // Forward declaration

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
    HWND hwnd;  // Window handle - changer void* en HWND
    HFONT hRobotoFont;  // Police Roboto
    
    void createWindow();
    void messageLoop();
    void loadRobotoFont();
    void applyTheme(const std::string& theme);
    
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
