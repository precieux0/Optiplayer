#pragma once
#include <string>
#include <vector>

struct StreamInfo {
    std::string codec;
    int index;
    std::string language;
};

class Decoder {
public:
    static std::vector<StreamInfo> getVideoStreams(const std::string& path);
    static std::vector<StreamInfo> getAudioStreams(const std::string& path);
    static std::vector<StreamInfo> getSubtitleStreams(const std::string& path);
    static bool hasHardwareAcceleration(const std::string& codec);
};
