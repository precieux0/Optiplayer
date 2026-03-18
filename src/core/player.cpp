#include "player.h"
#include <mpv/client.h>
#include <iostream>

Player::Player() {
    mpv_handle = mpv_create();
    if (!mpv_handle) {
        throw std::runtime_error("Failed to create mpv handle");
    }
    
    mpv_set_option_string(mpv_handle, "vo", "gpu-next");
    mpv_set_option_string(mpv_handle, "ao", "wasapi");
    mpv_set_option_string(mpv_handle, "hwdec", "auto");
    
    mpv_initialize(mpv_handle);
}

Player::~Player() {
    if (mpv_handle) {
        mpv_terminate_destroy(mpv_handle);
    }
}

bool Player::loadFile(const std::string& path) {
    std::string cmd = "loadfile";
    const char* args[] = {cmd.c_str(), path.c_str(), NULL};
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
    mpv_set_property(mpv_handle, "volume", MPV_FORMAT_INT64, &volume);
}

void Player::setSpeed(double speed) {
    mpv_set_property(mpv_handle, "speed", MPV_FORMAT_DOUBLE, &speed);
}

double Player::getDuration() {
    double duration;
    mpv_get_property(mpv_handle, "duration", MPV_FORMAT_DOUBLE, &duration);
    return duration;
}

double Player::getPosition() {
    double pos;
    mpv_get_property(mpv_handle, "time-pos", MPV_FORMAT_DOUBLE, &pos);
    return pos;
}

bool Player::isPlaying() {
    int pause;
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
