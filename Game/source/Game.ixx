/*
 *  Game.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/21/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game;

import CR.Game.Constants;
import CR.Game.FSM;
import CR.Game.MainMenuGameState;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class Game {
	  public:
		Game();
		virtual ~Game();

		void Initialize();
		void Execute();
		void ApplicationTerminated();

	  private:
		CR::Utility::FSM gameStateMachine;
	};
}    // namespace CR::Game

module :private;

namespace ceaud   = CR::Engine::Audio;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::Game::Game() {}

cg::Game::~Game() {}

void cg::Game::Initialize() {
	;
	// saveGameManager = new SaveGameManager();

	// SettingsSaveInfo* saveInfo = saveGameManager->LoadSettingsSaveInfo();
	// ISound::Instance().MuteMusic(!saveInfo->GetOptionsMusicOn());
	// ISound::Instance().MuteSounds(!saveInfo->GetOptionsSoundOn());
	ceaud::setFXVolume(1.0f);
	ceaud::setMusicVolume(0.75f);

	// set up the game states
	gameStateMachine << new MainMenuGameState() /*<< new StoryModeGameState() << new ArcadeModeGameState()*/;
	gameStateMachine.State = Constants::MAIN_MENU_STATE;
}

void cg::Game::Execute() {
	gameStateMachine();
}

void cg::Game::ApplicationTerminated() {
	// saveGameManager->SaveOnTerminate();
}
