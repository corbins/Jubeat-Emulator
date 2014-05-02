#include "src/game.h"

int main() {
    //Initialize the engines, then prompt the user to select a song.
    Game game_engine = Game();
    game_engine.select_song();

    //Loop until the game engine ends the round.
    while(!game_engine.end) {
	game_engine.frame_update();
    }

    //Print out the score and exit.
    game_engine.score();
    return 0;
}
