#!/usr/bin/env python3
import os
import subprocess
from datetime import datetime

version = "1.0.0"
date = datetime.now().strftime("%Y-%m-%d")

notes = f"""# 🚀 OptiPlayer {version} - {date}

Développé avec ❤️ par OKITAKOY Inc.
Fondateur : Précieux Okitakoy

## ✨ Nouveautés
• Magic Sync 2.0 - Correction automatique A/V
• SmoothPlay - Lecture sans saccades
• Video Restorer - Nettoyage des vieilles vidéos
• Support tous formats (MP4, MKV, AVI, etc.)
• Interface moderne et intuitive

## 📦 Téléchargement
- [OptiPlayer-Setup-{version}.exe](https://optiplayer.onrender.com/download)
- [OptiPlayer-Portable-{version}.zip](https://optiplayer.onrender.com/download)

## 🔒 Confidentialité
OptiPlayer ne collecte aucune donnée personnelle.
100% gratuit, 100% respectueux.

---
*Site officiel: https://optiplayer.onrender.com*
*Contact: support@optiplayer.onrender.com*
"""

with open(f"RELEASE-v{version}.md", "w") as f:
    f.write(notes)

print(f"✅ Release notes generated: RELEASE-v{version}.md")
