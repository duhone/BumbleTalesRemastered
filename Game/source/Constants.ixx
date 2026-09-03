export module CR.Game.Constants;

import std;
import std.compat;

export namespace CR::Game::Constants {
	// main menu game states
	constexpr int SPLASH_MENU_STATE     = 0;
	constexpr int HOME_MENU_STATE       = 1;
	constexpr int OPTIONS_MENU_STATE    = 2;
	constexpr int HELP_MENU_STATE       = 3;
	constexpr int MORE_GAMES_MENU_STATE = 4;
	constexpr int SCRAPBOOK_MENU_STATE  = 5;

	constexpr uint32_t MAIN_MENU_STATE   = 0;
	constexpr uint32_t STORY_MODE_STATE  = 1;
	constexpr uint32_t ARCADE_MODE_STATE = 2;
}    // namespace CR::Game::Constants