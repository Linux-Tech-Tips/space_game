# Space game (Demo so far)

Simple space game, made in C using Raylib

Right now, the game is very early in development, and is basically an incomplete demo

Developed and tested on (GNU) Linux, should work on Windows as well

## TODO

Next goal is: working gameplay demonstration version
 
- Modules
    - game.c/game.h -> only general game data (not player or enemy logic), contains player, enemies and bullets and calls their logic (+ interactions)
    - player.c/player.h -> player data structure and logic
    - bullet.c/bullet.h -> bullet data structure and logic
    - enemy.c/enemy.h -> enemy data structure and logic (varying enemies based on stats/data in the structure)

