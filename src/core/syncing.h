#pragma once

class MagicSync {
public:
    static double detectAVDelay(void* mpv_handle);
    static void correctDelay(void* mpv_handle, double delay);
    static bool needsCorrection(void* mpv_handle);
};
