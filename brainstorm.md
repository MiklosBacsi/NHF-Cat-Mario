ProgramData::event !!!!!!!!!!!!!!

Rewrite Class Texture -> SDL_Rect !!!

Files to read:
GameConfig.txt -> Level1.txt, Level2.txt

Class Level:
numberOfCheckpoints
std::vector<Enemy>
std::vector<Block>

Menu:
Lang: ENG, JP, HUN
START
EXIT

std::vector<Button> menuButtons
std::vector<Button> gameButtons
Class Button:

Class LanguageModule:
több nyelv
enum LanguageCommands = Exit, Start, Dialogue1, Dialogue2;
std::string Lang[Language][LanguageCommands]

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
isMalevolnet;


Transition (similiar to Post-Proccess)