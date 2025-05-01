Menu->Level kép X vagy Pipa hogy teljesítve van-e, illetve window.renderText() -> Level 1

Files to read:
GameConfig.txt -> Level1.txt, Level2.txt

Rewrite RigidBody TEST!!!!!!

Redesign classes!!!!!!!!!!!
class GameObject
static size_t cameraX
Vector2D position
SDL_Rect hitBox
Texture

Class Level:
numberOfCheckpoints
std::vector<Enemy>
std::vector<Block>

Class Block:
enum Block::Texture
currentTexture, nextTexture

isLevelCompleted
isCheckpointReached

When an entity kills a player, a randomly selected message shows up (Japanese/English).
When an entity goes off-screen on the left, it dies.

Player Animations:
namespace Anim {
    enum = { STAND, RUN, IN_AIR, DIE };
}
bool faceRight;

player jump: shortJump, longJump (x idő múlva is nyomva van a W gomb)

lookRight és runRight állapotokban fix időt tölt összesen
(az számít amikor a gomb le van nyomva), majd a másik állapotpa vált

Tubes:
isEnterable
isMalevolent;

Class Level would hold all the data (player too)
(static) Level* GameEngine::currentLevel;

Textrue egy külön fájlba!
bool isStatic --> nem szabad felszabadítani, majd az osztály fogja

3. Beadás - Specifikáció
- Minden osztály és publikus függvény deklaráció!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
- Csak egy zip feltöltése *.h, *.cpp fájlokkal, nincs almappa!!!!!!!!
- argumentum: -c
- TESZTPROGRAM!!!
- Dokumentáció itt még nem kell