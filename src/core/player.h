#pragma once
#include <string>
#include <functional>

class Player {
public:
    Player();
    ~Player();
    
    bool loadFile(const std::string& path);
    void play();
    void pause();
    void stop();
    void seek(double seconds);
    
    void setVolume(int volume);
    void setSpeed(double speed);
    
    double getDuration();
    double getPosition();
    bool isPlaying();
    
    // Magic Sync
    void enableMagicSync(bool enable);
    void fixSync();

private:
    void* mpv_handle;
    bool magic_sync_enabled = true;
};
