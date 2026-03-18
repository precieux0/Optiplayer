#include "decoder.h"
#include <vector>

std::vector<StreamInfo> Decoder::getVideoStreams(const std::string& path) {
    return std::vector<StreamInfo>();
}

std::vector<StreamInfo> Decoder::getAudioStreams(const std::string& path) {
    return std::vector<StreamInfo>();
}

std::vector<StreamInfo> Decoder::getSubtitleStreams(const std::string& path) {
    return std::vector<StreamInfo>();
}

bool Decoder::hasHardwareAcceleration(const std::string& codec) {
    return false;
}
