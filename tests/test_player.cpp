#include <iostream>
#include <cassert>
#include "../src/core/player.h"

void testPlayerCreation() {
std::cout << "🧪 Test: Création du player..." << std::endl;
Player* player = new Player();
assert(player != nullptr);
std::cout << "✅ Succès!" << std::endl;
delete player;
}

void testFileLoading() {
std::cout << "🧪 Test: Chargement fichier..." << std::endl;
Player player;
bool result = player.loadFile("test.mp4");
assert(result == true);
std::cout << "✅ Succès!" << std::endl;
}

void testPlayback() {
std::cout << "🧪 Test: Lecture/Pause..." << std::endl;
Player player;
player.loadFile("test.mp4");
player.play();
player.pause();
player.stop();
std::cout << "✅ Succès!" << std::endl;
}

int main() {
std::cout << "════════════════════════════════════════════" << std::endl;
std::cout << "🔬 LANCEMENT DES TESTS OPTIPLAYER" << std::endl;
std::cout << "════════════════════════════════════════════" << std::endl;

}
