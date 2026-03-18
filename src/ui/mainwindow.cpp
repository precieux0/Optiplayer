#include "mainwindow.h"
#include "../core/player.h"
#include <windows.h>
#include <commdlg.h>
#include <fstream>
#include <sstream>
#include <string>
#include <dwrite.h>
#include <iostream>

#pragma comment(lib, "dwrite.lib")

// Version Unicode des fonctions
std::wstring toWide(const std::string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::wstring wstr(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
    return wstr;
}

std::string loadTheme() {
    const char* paths[] = {
        "assets/themes/default.qss",
        "../assets/themes/default.qss",
        "./assets/themes/default.qss",
        "../../assets/themes/default.qss"
    };
    
    for (const char* path : paths) {
        std::ifstream file(path);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }
    }
    return "";
}

bool registerRobotoFonts() {
    const char* fontPaths[] = {
        "assets/fonts/roboto_regular.ttf",
        "assets/fonts/roboto_medium.ttf",
        "../assets/fonts/roboto_regular.ttf",
        "../assets/fonts/roboto_medium.ttf"
    };
    
    for (const char* path : fontPaths) {
        std::ifstream file(path);
        if (file.good()) {
            std::wstring wpath = toWide(path);
            AddFontResourceExW(wpath.c_str(), FR_PRIVATE, 0);
        }
    }
    
    SendMessageTimeoutW(HWND_BROADCAST, WM_FONTCHANGE, 0, 0, SMTO_ABORTIFHUNG, 1000, NULL);
    return true;
}

MainWindow::MainWindow() {
    registerRobotoFonts();
    player = new Player();
    player->enableMagicSync(true);
    createWindow();
}

MainWindow::~MainWindow() {
    delete player;
    if (hRobotoFont) {
        DeleteObject(hRobotoFont);
    }
}

void MainWindow::createWindow() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"OptiPlayerWindow";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClassW(&wc);
    
    hwnd = CreateWindowExW(
        0, L"OptiPlayerWindow", L"OptiPlayer",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720, NULL, NULL, hInstance, this
    );
    
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
}

void MainWindow::show() {
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    messageLoop();
}

void MainWindow::messageLoop() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void MainWindow::onOpenFile() {
    OPENFILENAMEW ofn = {};
    wchar_t filename[MAX_PATH] = {};
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Media Files\0*.mp4;*.mkv;*.avi;*.mov;*.mp3;*.flac\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    
    if (GetOpenFileNameW(&ofn)) {
        std::wstring wfilename(filename);
        std::string filenameA(wfilename.begin(), wfilename.end());
        player->loadFile(filenameA);
        player->play();
        setTitle(filenameA);
    }
}

void MainWindow::onPlayPause() {
    if (player->isPlaying()) {
        player->pause();
    } else {
        player->play();
    }
}

void MainWindow::onSeek(int position) {
    double duration = player->getDuration();
    player->seek(duration * position / 100);
}

void MainWindow::onVolumeChange(int volume) {
    player->setVolume(volume);
}

void MainWindow::onMagicSync() {
    player->fixSync();
}

void MainWindow::setTitle(const std::string& title) {
    std::wstring wtitle = toWide("OptiPlayer - " + title);
    SetWindowTextW(hwnd, wtitle.c_str());
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MainWindow* window = (MainWindow*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    
    switch (msg) {
        case WM_CREATE: {
            // Créer police Roboto
            window->hRobotoFont = CreateFontW(
                16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH, L"Roboto"
            );
            
            // Créer menu
            HMENU hMenu = CreateMenu();
            
            // File menu
            HMENU hFileMenu = CreatePopupMenu();
            AppendMenuW(hFileMenu, MF_STRING, 1, L"&Open File...\tCtrl+O");
            AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hFileMenu, MF_STRING, 2, L"E&xit\tAlt+F4");
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
            
            // Playback menu
            HMENU hPlaybackMenu = CreatePopupMenu();
            AppendMenuW(hPlaybackMenu, MF_STRING, 3, L"&Play/Pause\tSpace");
            AppendMenuW(hPlaybackMenu, MF_STRING, 4, L"&Magic Sync\tF7");
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hPlaybackMenu, L"&Playback");
            
            // View menu
            HMENU hViewMenu = CreatePopupMenu();
            AppendMenuW(hViewMenu, MF_STRING, 5, L"&Fullscreen\tF11");
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"&View");
            
            SetMenu(hwnd, hMenu);
            break;
        }
        
        case WM_COMMAND:
            if (window) {
                switch (LOWORD(wParam)) {
                    case 1: window->onOpenFile(); break;
                    case 2: PostQuitMessage(0); break;
                    case 3: window->onPlayPause(); break;
                    case 4: window->onMagicSync(); break;
                }
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}
