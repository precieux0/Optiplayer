#include "mainwindow.h"
#include "../core/player.h"
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#pragma comment(lib, "comctl32.lib")

// Initialiser les contrôles communs
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

std::wstring toWide(const std::string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::wstring wstr(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
    return wstr;
}

MainWindow::MainWindow() {
    player = new Player();
    player->enableMagicSync(true);
    player->setVolume(100);  // Volume initial à 100%
    current_volume = 100;
    createWindow();
}

MainWindow::~MainWindow() {
    delete player;
    if (hRobotoFont) DeleteObject(hRobotoFont);
}

void MainWindow::createWindow() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    
    // Initialiser les contrôles communs
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);
    
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"OptiPlayerWindow";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
    
    RegisterClassW(&wc);
    
    hwnd = CreateWindowExW(
        0, L"OptiPlayerWindow", L"OptiPlayer",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720, NULL, NULL, hInstance, this
    );
    
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
}

void MainWindow::createControls() {
    // Police Roboto
    hRobotoFont = CreateFontW(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, L"Roboto"
    );
    
    // Créer le slider de volume
    hVolumeSlider = CreateWindowExW(
        0, TRACKBAR_CLASSW, L"",
        WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_TOOLTIPS,
        1000, 650, 150, 30,
        hwnd, (HMENU)101, GetModuleHandle(NULL), NULL
    );
    
    // Configurer le slider pour 0-200
    SendMessageW(hVolumeSlider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 200));
    SendMessageW(hVolumeSlider, TBM_SETPOS, TRUE, current_volume);
    SendMessageW(hVolumeSlider, TBM_SETTICFREQ, 50, 0);  // Graduations tous les 50
    
    // Label pour afficher le volume
    hVolumeLabel = CreateWindowExW(
        0, L"STATIC", L"🔊 100%",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        1150, 650, 60, 30,
        hwnd, (HMENU)102, GetModuleHandle(NULL), NULL
    );
    
    SendMessageW(hVolumeLabel, WM_SETFONT, (WPARAM)hRobotoFont, TRUE);
}

void MainWindow::updateVolumeDisplay() {
    wchar_t buffer[32];
    wsprintfW(buffer, L"🔊 %d%%", current_volume);
    SetWindowTextW(hVolumeLabel, buffer);
}

void MainWindow::show() {
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    createControls();  // Créer les contrôles après l'affichage
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
    current_volume = volume;
    player->setVolume(volume);
    updateVolumeDisplay();
    
    // Feedback visuel selon le niveau
    if (volume > 150) {
        SetWindowTextW(hVolumeLabel, L"🔊🔊 150%+");
    } else if (volume > 100) {
        SetWindowTextW(hVolumeLabel, L"🔊🔊 100%+");
    } else {
        updateVolumeDisplay();
    }
}

void MainWindow::onVolumeUp() {
    int new_volume = min(current_volume + 10, 200);
    SendMessageW(hVolumeSlider, TBM_SETPOS, TRUE, new_volume);
    onVolumeChange(new_volume);
}

void MainWindow::onVolumeDown() {
    int new_volume = max(current_volume - 10, 0);
    SendMessageW(hVolumeSlider, TBM_SETPOS, TRUE, new_volume);
    onVolumeChange(new_volume);
}

void MainWindow::onMagicSync() {
    player->fixSync();
    MessageBoxW(hwnd, L"✨ Magic Sync activé !", L"OptiPlayer", MB_OK | MB_ICONINFORMATION);
}

void MainWindow::onToggleBoost() {
    static bool boost = true;
    boost = !boost;
    player->setVolumeBoost(boost);
    if (boost) {
        SendMessageW(hVolumeSlider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 200));
        MessageBoxW(hwnd, L"🎵 Mode Boost activé (0-200%)", L"OptiPlayer", MB_OK | MB_ICONINFORMATION);
    } else {
        SendMessageW(hVolumeSlider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
        if (current_volume > 100) {
            current_volume = 100;
            SendMessageW(hVolumeSlider, TBM_SETPOS, TRUE, 100);
            player->setVolume(100);
            updateVolumeDisplay();
        }
        MessageBoxW(hwnd, L"🔊 Mode Normal (0-100%)", L"OptiPlayer", MB_OK | MB_ICONINFORMATION);
    }
}

void MainWindow::setTitle(const std::string& title) {
    std::wstring wtitle = toWide("OptiPlayer - " + title);
    SetWindowTextW(hwnd, wtitle.c_str());
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MainWindow* window = (MainWindow*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    
    switch (msg) {
        case WM_CREATE: {
            HMENU hMenu = CreateMenu();
            
            // File menu
            HMENU hFileMenu = CreatePopupMenu();
            AppendMenuW(hFileMenu, MF_STRING, 1, L"📂 &Open File...\tCtrl+O");
            AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hFileMenu, MF_STRING, 2, L"❌ E&xit\tAlt+F4");
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"📁 &File");
            
            // Playback menu
            HMENU hPlaybackMenu = CreatePopupMenu();
            AppendMenuW(hPlaybackMenu, MF_STRING, 3, L"▶️ &Play/Pause\tSpace");
            AppendMenuW(hPlaybackMenu, MF_STRING, 4, L"⚡ &Magic Sync\tF7");
            AppendMenuW(hPlaybackMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hPlaybackMenu, MF_STRING, 5, L"🔊 Volume +10%\tCtrl+Up");
            AppendMenuW(hPlaybackMenu, MF_STRING, 6, L"🔉 Volume -10%\tCtrl+Down");
            AppendMenuW(hPlaybackMenu, MF_STRING, 7, L"🎵 Toggle Boost (100%/200%)\tCtrl+B");
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hPlaybackMenu, L"🎮 &Playback");
            
            // View menu
            HMENU hViewMenu = CreatePopupMenu();
            AppendMenuW(hViewMenu, MF_STRING, 8, L"🖥️ &Fullscreen\tF11");
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"👁️ &View");
            
            SetMenu(hwnd, hMenu);
            break;
        }
        
        case WM_HSCROLL:
            if (window && (HWND)lParam == window->hVolumeSlider) {
                int volume = SendMessageW(window->hVolumeSlider, TBM_GETPOS, 0, 0);
                window->onVolumeChange(volume);
            }
            break;
        
        case WM_COMMAND:
            if (window) {
                switch (LOWORD(wParam)) {
                    case 1: window->onOpenFile(); break;
                    case 2: PostQuitMessage(0); break;
                    case 3: window->onPlayPause(); break;
                    case 4: window->onMagicSync(); break;
                    case 5: window->onVolumeUp(); break;
                    case 6: window->onVolumeDown(); break;
                    case 7: window->onToggleBoost(); break;
                    case 8: 
                        // Fullscreen (à implémenter)
                        break;
                }
            }
            break;
        
        case WM_KEYDOWN:
            if (window) {
                if (GetKeyState(VK_CONTROL) & 0x8000) {
                    switch (wParam) {
                        case VK_UP: window->onVolumeUp(); break;
                        case VK_DOWN: window->onVolumeDown(); break;
                        case 'B': window->onToggleBoost(); break;
                        case 'O': window->onOpenFile(); break;
                    }
                } else {
                    switch (wParam) {
                        case VK_SPACE: window->onPlayPause(); break;
                        case VK_F7: window->onMagicSync(); break;
                        case VK_F11: 
                            // Fullscreen
                            break;
                    }
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
