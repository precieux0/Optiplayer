
📡 API OptiPlayer

Commandes MPV Supportées

Commande Description
loadfile <path> Charger un fichier
set pause <yes/no> Pause/Play
seek <seconds> Avancer/reculer
set volume <0-100> Changer volume

Événements

```cpp
// Exemple d'utilisation
player.on('file-loaded', () => {
    console.log('Fichier chargé');
});
```

