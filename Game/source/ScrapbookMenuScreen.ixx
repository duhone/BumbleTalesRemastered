/*
 *  ScrapbookMenuScreen.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/4/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

export module CR.Game.ScrapbookMenuScreen;

import CR.Game.ScrapbookMenuView;
import CR.Game.Constants;
import CR.Game.FSM;
import CR.Game.StoryModeSaveInfo;
import CR.Game.ArcadeModeSaveInfo;
import CR.Game.TrophySaveInfo;
import CR.Game.SaveGameManager;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class ScrapbookMenuScreen : public CR::Utility::IState {
	  public:
		ScrapbookMenuScreen();
		virtual ~ScrapbookMenuScreen();

		void OnExitScrapbookMenu();

		// IState
		virtual bool Begin();
		virtual void End();
		virtual int Process();

	  private:
		ScrapbookMenuView* view{};
		bool m_requestShowHomeMenu;
		StoryModeSaveInfo* saveInfo{};
		ArcadeModeSaveInfo* arcadeModeSaveInfo{};
		TrophySaveInfo* trophySaveInfo{};
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceaud   = CR::Engine::Audio;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::ScrapbookMenuScreen::ScrapbookMenuScreen() {
	m_requestShowHomeMenu = false;
}

cg::ScrapbookMenuScreen::~ScrapbookMenuScreen() {
	delete view;
	delete saveInfo;
	delete arcadeModeSaveInfo;
	delete trophySaveInfo;
}

// IState
bool cg::ScrapbookMenuScreen::Begin() {
	saveInfo           = GetSaveGameManager().LoadStoryModeSaveInfo();
	arcadeModeSaveInfo = GetSaveGameManager().LoadArcadeModeSaveInfo();
	trophySaveInfo     = GetSaveGameManager().LoadTrophySaveInfo();

	view = new ScrapbookMenuView();
	view->OnExitClicked += Delegate(this, &ScrapbookMenuScreen::OnExitScrapbookMenu);
	view->LoadFromSaveInfo(saveInfo);
	view->LoadFromArcadeSaveInfo(arcadeModeSaveInfo);
	view->LoadFromTrophySaveInfo(trophySaveInfo);

	return true;
}

void cg::ScrapbookMenuScreen::End() {
	delete view;
	view = nullptr;
	delete saveInfo;
	saveInfo = nullptr;
	delete arcadeModeSaveInfo;
	arcadeModeSaveInfo = nullptr;
	delete trophySaveInfo;
	trophySaveInfo = nullptr;
}

int cg::ScrapbookMenuScreen::Process() {
	view->Update();

	view->Render();

	if(m_requestShowHomeMenu) {
		m_requestShowHomeMenu = false;
		return Constants::HOME_MENU_STATE;
	}

	return CR::Utility::IState::UNCHANGED;
}

void cg::ScrapbookMenuScreen::OnExitScrapbookMenu() {
	m_requestShowHomeMenu = true;
}