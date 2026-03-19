#include "player.h"
#include <mpv/client.h>
#include <iostream>
#include <cmath>

Player::Player() {
    mpv_handle = mpv_create();
    if (!mpv_handle) {
        throw std::runtime_error("Failed to create mpv handle");
    }
    
    // Configuration pour Windows
    mpv_set_option_string(mpv_handle, "vo", "gpu-next");
    mpv_set_option_string(mpv_handle, "ao", "wasapi");
    mpv_set_option_string(mpv_handle, "hwdec", "auto");
    
    // Configuration audio pour permettre le volume > 100%
    mpv_set_option_string(mpv_handle, "volume-max", "200");  // Permet jusqu'à 200%
    mpv_set_option_string(mpv_handle, "volume", "100");      // Volume initial
    
    mpv_initialize(mpv_handle);
}

Player::~Player() {
    if (mpv_handle) {
        mpv_terminate_destroy(mpv_handle);
    }
}

bool Player::loadFile(const std::string& path) {
    const char* args[] = {"loadfile", path.c_str(), NULL};
    mpv_command(mpv_handle, args);
    return true;
}

void Player::play() {
    mpv_set_property_string(mpv_handle, "pause", "no");
}

void Player::pause() {
    mpv_set_property_string(mpv_handle, "pause", "yes");
}

void Player::stop() {
    const char* args[] = {"stop", NULL};
    mpv_command(mpv_handle, args);
}

void Player::seek(double seconds) {
    mpv_set_property(mpv_handle, "time-pos", MPV_FORMAT_DOUBLE, &seconds);
}

void Player::setVolume(int volume) {
    // Limiter entre 0 et 200
    if (volume < 0) volume = 0;
    if (volume > 200) volume = 200;
    
    current_volume = volume;
    
    // mpv utilise le volume en % (0-100 par défaut, mais on a configuré volume-max=200)
    mpv_set_property(mpv_handle, "volume", MPV_FORMAT_INT64, &volume);
    
    std::cout << "🔊 Volume set to: " << volume << "%" << std::endl;
}

int Player::getVolume() {
    int64_t volume = 100;
    mpv_get_property(mpv_handle, "volume", MPV_FORMAT_INT64, &volume);
    return static_cast<int>(volume);
}

void Player::setVolumeBoost(bool enable) {
    boost_enabled = enable;
    if (enable) {
        mpv_set_option_string(mpv_handle, "volume-max", "200");
    } else {
        mpv_set_option_string(mpv_handle, "volume-max", "100");
    }
    // Réappliquer le volume actuel
    setVolume(current_volume);
}

void Player::setSpeed(double speed) {
    mpv_set_property(mpv_handle, "speed", MPV_FORMAT_DOUBLE, &speed);
}

double Player::getDuration() {
    double duration = 0;
    mpv_get_property(mpv_handle, "duration", MPV_FORMAT_DOUBLE, &duration);
    return duration;
}

double Player::getPosition() {
    double pos = 0;
    mpv_get_property(mpv_handle, "time-pos", MPV_FORMAT_DOUBLE, &pos);
    return pos;
}

bool Player::isPlaying() {
    int pause = 1;
    mpv_get_property(mpv_handle, "pause", MPV_FORMAT_FLAG, &pause);
    return !pause;
}

void Player::enableMagicSync(bool enable) {
    magic_sync_enabled = enable;
    if (enable) {
        mpv_set_option_string(mpv_handle, "video-sync", "display-resample");
        mpv_set_option_string(mpv_handle, "video-sync-max-video-change", "5");
    }
}

void Player::fixSync() {
    // Force resync
    mpv_set_option_string(mpv_handle, "video-sync", "audio");
    mpv_set_option_string(mpv_handle, "video-sync", "display-resample");
}
