/*
 *  MainMenuGameState.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

export module CR.Game.MainMenuGameState;

import CR.Game.Constants;
import CR.Game.SplashScreenMenu;
import CR.Game.FSM;
// #include "HelpMenuScreen.h"
// #include "HomeMenuScreen.h"
// #include "MoreGamesMenuScreen.h"
// #include "OptionsMenuScreen.h"
// #include "ScrapbookMenuScreen.h"

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class MainMenuGameState : public CR::Utility::IState {
	  public:
		MainMenuGameState();
		virtual ~MainMenuGameState();

		void StartStoryMode();
		void StartArcadeMode();

		// IState
		virtual bool Begin();
		virtual void End();
		virtual int Process();

	  private:
		CR::Utility::FSM stateMachine;
		bool m_requestStartStoryMode;
		bool m_requestStartArcadeMode;
		// bool m_requestShowOptionsMenu;
		bool m_showSplashScreen;
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::MainMenuGameState::MainMenuGameState() {
	stateMachine << new SplashScreenMenu() /*<< new HomeMenuScreen(this) << new OptionsMenuScreen(this)
	             << new HelpMenuScreen(this) << new MoreGamesMenuScreen(this)
	             << new ScrapbookMenuScreen(this)*/
	    ;
	stateMachine.State = cg::Constants::SPLASH_MENU_STATE;
	m_showSplashScreen = true;
}

cg::MainMenuGameState::~MainMenuGameState() {}

bool cg::MainMenuGameState::Begin() {
	if(m_showSplashScreen) {
		stateMachine.State = cg::Constants::SPLASH_MENU_STATE;
		m_showSplashScreen = false;
	} else {
		stateMachine.State = cg::Constants::HOME_MENU_STATE;
	}

	m_requestStartStoryMode  = false;
	m_requestStartArcadeMode = false;
	return true;
}

void cg::MainMenuGameState::End() {
	stateMachine.Reset();
}

int cg::MainMenuGameState::Process() {
	stateMachine();

	/* states dont exist yet, pending port
	if(m_requestStartStoryMode) {
	    m_requestStartStoryMode = false;
	    return cg::Constants::STORY_MODE_STATE;
	} else if(m_requestStartArcadeMode) {
	    m_requestStartArcadeMode = false;
	    return cg::Constants::ARCADE_MODE_STATE;
	}*/

	/*commented out in prev repo
	else if (m_requestShowOptionsMenu)
	{
	    m_requestShowOptionsMenu = false;
	    return OPTIONS_MENU_STATE;
	}*/

	return CR::Utility::IState::UNCHANGED;
}

void cg::MainMenuGameState::StartStoryMode() {
	m_requestStartStoryMode = true;
}

void cg::MainMenuGameState::StartArcadeMode() {
	m_requestStartArcadeMode = true;
}