#include "src/game.h"

int main() {
    Game game_engine = Game();

    game_engine.select_song();

    //Loop until there are no more notes
    while(!game_engine.end) {
	game_engine.frame_update();
    }

    game_engine.score();
    return 0;
}
