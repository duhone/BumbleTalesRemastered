/*
 *  SplashScreenMenu.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/13/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.SplashScreenMenu;

import CR.Game.Constants;
import CR.Game.FSM;
import CR.Game.SplashScreenMenuView;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SplashScreenMenu : public CR::Utility::IState {
	  public:
		SplashScreenMenu();
		virtual ~SplashScreenMenu();

		// IState
		virtual bool Begin();
		virtual void End();
		virtual int Process();

		void OnExitSplashScreen();

	  private:
		SplashScreenMenuView* view;
		// MainMenuGameState* mainMenuGameState;
		bool m_requestShowHomeMenu;
	};
}    // namespace CR::Game

module :private;

namespace cecore = CR::Engine::Core;
namespace cg     = CR::Game;

cg::SplashScreenMenu::SplashScreenMenu() {
	// this->mainMenuGameState = mainMenuGameState;
	m_requestShowHomeMenu = false;
}

cg::SplashScreenMenu::~SplashScreenMenu() {}

bool cg::SplashScreenMenu::Begin() {
	view = new SplashScreenMenuView();
	view->ExitSplashScreen += Delegate(this, &SplashScreenMenu::OnExitSplashScreen);
	return true;
}

void cg::SplashScreenMenu::End() {
	delete view;
}

int cg::SplashScreenMenu::Process() {
	view->Update();

	// graphics_engine->BeginFrame();
	view->Render();
	// graphics_engine->EndFrame();

	if(m_requestShowHomeMenu) {
		m_requestShowHomeMenu = false;
		return cg::Constants::HOME_MENU_STATE;
	}

	return CR::Utility::IState::UNCHANGED;
}

void cg::SplashScreenMenu::OnExitSplashScreen() {
	m_requestShowHomeMenu = true;
}
