#include <windows.h>
#include "mainwindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Version Unicode explicite
    MainWindow window;
    window.show();
    return 0;
}
