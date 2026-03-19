#pragma once
#include <string>

// Version stub pour compilation sans dépendances
class Player {
public:
    Player() {}
    ~Player() {}
    
    bool loadFile(const std::string& path) { return true; }
    void play() {}
    void pause() {}
    void stop() {}
    void seek(double seconds) {}
    void setVolume(int volume) {}
    void setSpeed(double speed) {}
    double getDuration() { return 0; }
    double getPosition() { return 0; }
    bool isPlaying() { return false; }
    void enableMagicSync(bool enable) {}
    void fixSync() {}
};
