#!/bin/bash

echo "════════════════════════════════════════════"
echo "📥 TÉLÉCHARGEMENT DES DÉPENDANCES RÉELLES"
echo "════════════════════════════════════════════"

cd external

# FFmpeg (version complète avec headers et libs)
echo "📦 Téléchargement de FFmpeg..."
curl -L -o ffmpeg.7z https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-full.7z
7z x ffmpeg.7z -offmpeg_temp -y
mv ffmpeg_temp/ffmpeg-* ffmpeg
rm -rf ffmpeg_temp ffmpeg.7z

# Vérifier FFmpeg
if [ -f "ffmpeg/include/libavcodec/avcodec.h" ]; then
    echo "✅ FFmpeg headers: OK"
else
    echo "❌ FFmpeg headers manquants"
    exit 1
fi

# MPV (version complète)
echo "📦 Téléchargement de libmpv..."
curl -L -o mpv.7z https://sourceforge.net/projects/mpv-player-windows/files/libmpv/mpv-dev-x86_64-20230226.7z/download
7z x mpv.7z -ompv_temp -y
mv mpv_temp mpv
rm mpv.7z

# Vérifier MPV
if [ -f "mpv/include/mpv/client.h" ]; then
    echo "✅ MPV headers: OK"
else
    echo "❌ MPV headers manquants"
    exit 1
fi

if [ -f "mpv/mpv-1.lib" ]; then
    echo "✅ MPV library: OK"
else
    echo "❌ MPV library manquante"
    exit 1
fi

cd ..

echo ""
echo "════════════════════════════════════════════"
echo "🎉 TOUTES LES DÉPENDANCES SONT PRÊTES !"
echo "════════════════════════════════════════════"
