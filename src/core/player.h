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
    
    // Volume: 0-200 (pour correspondre à VLC)
    void setVolume(int volume);  // 0 = muet, 100 = normal, 200 = x2
    int getVolume();  // Retourne le volume actuel
    void setVolumeBoost(bool enable);  // Activer le boost au-delà de 100%
    
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
    int current_volume = 100;
    bool boost_enabled = true;  // Permet 100-200% par défaut
};
