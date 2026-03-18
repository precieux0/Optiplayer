#include "mainwindow.h"
#include <windows.h>
#include <commdlg.h>
#include <fstream>
#include <sstream>
#include <string>
#include <dwrite.h>
#include <iostream>

#pragma comment(lib, "dwrite.lib")

// Fonction pour charger le thème QSS
std::string loadTheme() {
    // Essayer différents chemins possibles
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
            std::cout << "✅ Thème chargé depuis: " << path << std::endl;
            return buffer.str();
        }
    }
    
    std::cout << "⚠️  Thème non trouvé, utilisation du style par défaut" << std::endl;
    return "";
}

// Fonction pour enregistrer les polices Roboto dans Windows
bool registerRobotoFonts() {
    HRESULT hr;
    
    // Chemins des polices
    const char* fontPaths[] = {
        "assets/fonts/roboto_regular.ttf",
        "assets/fonts/roboto_medium.ttf",
        "../assets/fonts/roboto_regular.ttf",
        "../assets/fonts/roboto_medium.ttf"
    };
    
    for (const char* path : fontPaths) {
        // Vérifier si le fichier existe
        std::ifstream file(path);
        if (file.good()) {
            // Ajouter la police au système
            int result = AddFontResourceExA(path, FR_PRIVATE, 0);
            if (result != 0) {
                std::cout << "✅ Police chargée: " << path << std::endl;
            } else {
                std::cout << "⚠️  Police non chargée (peut-être déjà présente): " << path << std::endl;
            }
        }
    }
    
    // Forcer la mise à jour des polices
    SendMessageTimeout(HWND_BROADCAST, WM_FONTCHANGE, 0, 0, SMTO_ABORTIFHUNG, 1000, NULL);
    
    return true;
}

// Fonction pour appliquer le thème
void applyTheme(const std::string& theme) {
    // Cette fonction sera appelée par votre framework UI
    // Exemple avec Qt: qApp->setStyleSheet(QString::fromStdString(theme));
    std::cout << "🎨 Thème appliqué (" << theme.length() << " caractères)" << std::endl;
}

MainWindow::MainWindow() {
    std::cout << "=== OPTIPLAYER DÉMARRAGE ===" << std::endl;
    
    // Enregistrer les polices Roboto
    registerRobotoFonts();
    
    // Créer le player
    player = new Player();
    player->enableMagicSync(true);
    
    // Charger et appliquer le thème
    std::string theme = loadTheme();
    if (!theme.empty()) {
        applyTheme(theme);
    }
    
    createWindow();
    
    std::cout << "✅ OptiPlayer prêt avec police Roboto !" << std::endl;
}

// ... le reste du code existant ...

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    switch (msg) {
        case WM_CREATE: {
            // Créer une police Roboto pour la fenêtre
            HFONT hFont = CreateFont(
                16,                    // Hauteur
                0,                     // Largeur
                0,                     // Angle d'échappement
                0,                     // Angle d'orientation
                FW_NORMAL,             // Poids
                FALSE,                 // Italique
                FALSE,                 // Souligné
                FALSE,                 // Barré
                DEFAULT_CHARSET,       // Jeu de caractères
                OUT_DEFAULT_PRECIS,    // Précision de sortie
                CLIP_DEFAULT_PRECIS,   // Précision de clipping
                DEFAULT_QUALITY,       // Qualité
                DEFAULT_PITCH,         // Pas
                L"Roboto"               // Nom de la police
            );
            
            if (hFont) {
                SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
            }
            
            HMENU hMenu = CreateMenu();
            HMENU hFileMenu = CreatePopupMenu();
            AppendMenu(hFileMenu, MF_STRING, 1, L"📂 Open File...\tCtrl+O");
            AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(hFileMenu, MF_STRING, 2, L"❌ Exit\tAlt+F4");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"📁 File");
            
            HMENU hPlaybackMenu = CreatePopupMenu();
            AppendMenu(hPlaybackMenu, MF_STRING, 3, L"▶️ Play/Pause\tSpace");
            AppendMenu(hPlaybackMenu, MF_STRING, 4, L"⚡ Magic Sync\tF7");
            AppendMenu(hPlaybackMenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(hPlaybackMenu, MF_STRING, 5, L"🔊 Volume Up\tCtrl+Up");
            AppendMenu(hPlaybackMenu, MF_STRING, 6, L"🔉 Volume Down\tCtrl+Down");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPlaybackMenu, L"🎮 Playback");
            
            HMENU hViewMenu = CreatePopupMenu();
            AppendMenu(hViewMenu, MF_STRING, 7, L"🎬 Fullscreen\tF11");
            AppendMenu(hViewMenu, MF_STRING, 8, L"🖼️ Mini Player\tCtrl+M");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"👁️ View");
            
            SetMenu(hwnd, hMenu);
            break;
        }
        
        // ... reste du WndProc ...
        
        case WM_CTLCOLORSTATIC: {
            // Appliquer Roboto aux contrôles statiques
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(255, 255, 255));
            SetBkColor(hdcStatic, RGB(30, 30, 47));
            
            HFONT hFont = CreateFont(
                14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH, L"Roboto"
            );
            
            if (hFont) {
                SelectObject(hdcStatic, hFont);
            }
            
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
