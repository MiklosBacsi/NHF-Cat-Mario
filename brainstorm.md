Menu->Level kép X vagy Pipa hogy teljesítve van-e, illetve window.renderText() -> Level 1

Rewrite RigidBody TEST!!!!!!

Textrue egy külön fájlba!
bool isStatic --> nem szabad felszabadítani, majd az osztály fogja

*** To-do: ***
- Enemy Quote!!!
- Level Element:
    - Generic: Mountain, Hill, Grass
    - Cloud
    - Tube (Low, Middle, High)
    - Fish
    - Checkpoint Flag
    - End Flag
    - House
- Level File Config
- Choosing Level from Menu
- Rewrite Tests
- Documentation
- CMake!!!!!!!!!!!!!!!

Entity Quote:
Select the enemy that's the left-most --> give the quote to him,
and if he dies regenerate a new quote for the left-most enemy


Class Level:
numberOfCheckpoints
isLevelCompleted
isCheckpointReached