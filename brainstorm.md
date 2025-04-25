Menu->Level kép X vagy Pipa hogy teljesítve van-e, illetve window.renderText() -> Level 1

Files to read:
GameConfig.txt -> Level1.txt, Level2.txt

Class RigidBody

CameraX or CameraRect


Class Level:
numberOfCheckpoints
std::vector<Enemy>
std::vector<Block>

LangMod[Language].getRandomQuote();
struct Quote {
    std::string quote;
    size_t quoteIndex;
};

isLevelCompleted
isCheckpointReached

class Entity; class Element;

When an entity kills a player, a randomly selected message shows up (Japanese/English).
When an entity goes off-screen on the left, it dies.

Player Animations:
enum = {lookRight, lookLeft, runRight, runLeft, inAirRight, isAirLeft, dieRight, dieLeft};

player jump: shortJump, longJump (x idő múlva is nyomva van a W gomb)

lookRight és runRight állapotokban fix időt tölt összesen
(az számít amikor a gomb le van nyomva), majd a másik állapotpa vált

Tubes:
isEnterable
isMalevolent;

Class Level would hold all the data (player too)
(static) Level* ProgramData::currentLevel;

3. Beadás - Specifikáció
- Minden osztály és publikus függvény deklaráció!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
- Csak egy zip feltöltése *.h, *.cpp fájlokkal, nincs almappa!!!!!!!!
- argumentum: -c
- TESZTPROGRAM!!!
- Dokumentáció itt még nem kell

Muszáj deklarálni beadáshoz:
- Class Rigid (és Vector2D) + TESZT !!!!
- Class Entity + Player + Enemy
- Class Block
- Class Level
- Class Animation
- Struct Quote