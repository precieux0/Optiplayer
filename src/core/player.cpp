#include "player.h"

// Inclure client.h avec gestion d'erreur
#ifdef USE_SYSTEM_MPV
#include <mpv/client.h>
#else
// Version stub - les déclarations sont dans le fichier généré
#include "client.h"
#endif

#include <iostream>
#include <cstring>

Player::Player() {
    mpv_handle = mpv_create();
    if (!mpv_handle) {
        std::cerr << "⚠️ MPV stub mode: using mock player" << std::endl;
        // En mode stub, on continue quand même
    } else {
        // Configuration
        mpv_set_option_string(mpv_handle, "vo", "gpu-next");
        mpv_set_option_string(mpv_handle, "ao", "wasapi");
        mpv_set_option_string(mpv_handle, "hwdec", "auto");
        mpv_set_option_string(mpv_handle, "volume-max", "200");
        
        mpv_initialize(mpv_handle);
        std::cout << "✅ MPV initialized" << std::endl;
    }
}

Player::~Player() {
    if (mpv_handle) {
        mpv_terminate_destroy(mpv_handle);
    }
}

bool Player::loadFile(const std::string& path) {
    if (!mpv_handle) {
        std::cout << "📁 [STUB] Loading file: " << path << std::endl;
        return true;
    }
    const char* args[] = {"loadfile", path.c_str(), NULL};
    mpv_command(mpv_handle, args);
    return true;
}

void Player::play() {
    if (!mpv_handle) {
        std::cout << "▶️ [STUB] Play" << std::endl;
        return;
    }
    mpv_set_property_string(mpv_handle, "pause", "no");
}

void Player::pause() {
    if (!mpv_handle) {
        std::cout << "⏸️ [STUB] Pause" << std::endl;
        return;
    }
    mpv_set_property_string(mpv_handle, "pause", "yes");
}

void Player::stop() {
    if (!mpv_handle) {
        std::cout << "⏹️ [STUB] Stop" << std::endl;
        return;
    }
    const char* args[] = {"stop", NULL};
    mpv_command(mpv_handle, args);
}

void Player::seek(double seconds) {
    if (!mpv_handle) {
        std::cout << "⏩ [STUB] Seek to: " << seconds << "s" << std::endl;
        return;
    }
    mpv_set_property(mpv_handle, "time-pos", MPV_FORMAT_DOUBLE, &seconds);
}

void Player::setVolume(int volume) {
    if (volume < 0) volume = 0;
    if (volume > 200) volume = 200;
    
    if (!mpv_handle) {
        std::cout << "🔊 [STUB] Volume: " << volume << "%" << std::endl;
        return;
    }
    mpv_set_property(mpv_handle, "volume", MPV_FORMAT_INT64, &volume);
}

int Player::getVolume() {
    if (!mpv_handle) {
        return 100; // Valeur par défaut en mode stub
    }
    int64_t volume = 100;
    mpv_get_property(mpv_handle, "volume", MPV_FORMAT_INT64, &volume);
    return static_cast<int>(volume);
}

void Player::setVolumeBoost(bool enable) {
    if (!mpv_handle) {
        std::cout << (enable ? "🎵 [STUB] Boost ON" : "🔊 [STUB] Boost OFF") << std::endl;
        return;
    }
    if (enable) {
        mpv_set_option_string(mpv_handle, "volume-max", "200");
    } else {
        mpv_set_option_string(mpv_handle, "volume-max", "100");
    }
}

void Player::setSpeed(double speed) {
    if (!mpv_handle) {
        std::cout << "⚡ [STUB] Speed: " << speed << "x" << std::endl;
        return;
    }
    mpv_set_property(mpv_handle, "speed", MPV_FORMAT_DOUBLE, &speed);
}

double Player::getDuration() {
    if (!mpv_handle) {
        return 120.0; // 2 minutes par défaut en mode stub
    }
    double duration = 0;
    mpv_get_property(mpv_handle, "duration", MPV_FORMAT_DOUBLE, &duration);
    return duration;
}

double Player::getPosition() {
    if (!mpv_handle) {
        return 0.0;
    }
    double pos = 0;
    mpv_get_property(mpv_handle, "time-pos", MPV_FORMAT_DOUBLE, &pos);
    return pos;
}

bool Player::isPlaying() {
    if (!mpv_handle) {
        return false;
    }
    int pause = 1;
    mpv_get_property(mpv_handle, "pause", MPV_FORMAT_FLAG, &pause);
    return !pause;
}

void Player::enableMagicSync(bool enable) {
    if (!mpv_handle) {
        std::cout << (enable ? "✨ [STUB] Magic Sync ON" : "✨ [STUB] Magic Sync OFF") << std::endl;
        return;
    }
    if (enable) {
        mpv_set_option_string(mpv_handle, "video-sync", "display-resample");
        mpv_set_option_string(mpv_handle, "video-sync-max-video-change", "5");
    }
}

void Player::fixSync() {
    if (!mpv_handle) {
        std::cout << "✨ [STUB] Fixing sync" << std::endl;
        return;
    }
    mpv_set_option_string(mpv_handle, "video-sync", "audio");
    mpv_set_option_string(mpv_handle, "video-sync", "display-resample");
}
