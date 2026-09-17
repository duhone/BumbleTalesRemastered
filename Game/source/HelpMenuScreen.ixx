/*
 *  HelpMenuScreen.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/24/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

export module CR.Game.HelpMenuScreen;

import CR.Game.HelpMenuView;
import CR.Game.Constants;
import CR.Game.FSM;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class HelpMenuScreen : public CR::Utility::IState {
	  public:
		HelpMenuScreen();
		virtual ~HelpMenuScreen();

		void OnExitHelpMenu();

		// IState
		virtual bool Begin();
		virtual void End();
		virtual int Process();

	  private:
		HelpMenuView* view;
		bool m_requestShowHomeMenu;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceaud   = CR::Engine::Audio;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::HelpMenuScreen::HelpMenuScreen() {
	m_requestShowHomeMenu = false;
}

cg::HelpMenuScreen::~HelpMenuScreen() {}

// IState
bool cg::HelpMenuScreen::Begin() {
	view = new HelpMenuView(true);
	view->OnExitClicked += Delegate(this, &HelpMenuScreen::OnExitHelpMenu);
	return true;
}

void cg::HelpMenuScreen::End() {
	delete view;
}

int cg::HelpMenuScreen::Process() {
	view->Update();
	view->Render();

	if(m_requestShowHomeMenu) {
		m_requestShowHomeMenu = false;
		return Constants::HOME_MENU_STATE;
	}

	return CR::Utility::IState::UNCHANGED;
}

void cg::HelpMenuScreen::OnExitHelpMenu() {
	m_requestShowHomeMenu = true;
}
