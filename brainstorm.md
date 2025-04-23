Menu->Level kép X vagy Pipa hogy teljesítve van-e, illetve window.renderText() -> Level 1

Files to read:
GameConfig.txt -> Level1.txt, Level2.txt

Class Quotes

roundedRectangleRGBA() !!!!!!!!!!!!!!!!!!!

Class Level:
numberOfCheckpoints
std::vector<Enemy>
std::vector<Block>

LangMod[Language].getRandomQuote();
struct Quote {
    std::string quote;
    size_t quoteIndex;
};

isPaused
isLoadingLevel
isLoadingGame
isLevelCompleted
isCheckpointReached

class Player {
    Vektor2D poz; // struct Pozition = {float x, float y};
    Vektor2D a;
};

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


Transition (similiar to Post-Proccess) draw (black) rectangle with variable oppacity