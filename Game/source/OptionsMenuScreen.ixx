/*
 *  OptionsMenuScreen.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

export module CR.Game.OptionsMenuScreen;

import CR.Game.OptionsMenuView;
import CR.Game.Constants;
import CR.Game.FSM;
import CR.Game.SaveGameManager;
import CR.Game.SettingsSaveInfo;
import CR.Game.ISaveOnTerminate;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class OptionsMenuScreen : public CR::Utility::IState, public ISaveOnTerminate {
	  public:
		OptionsMenuScreen();
		virtual ~OptionsMenuScreen();

		void OnExitOptionsMenu();
		void OnEraseData();

		// IState
		virtual bool Begin();
		virtual void End();
		virtual int Process();

		// Save
		void OnSaveOnTerminate();

	  private:
		OptionsMenuView* view{nullptr};
		bool m_requestShowHomeMenu{false};
		SettingsSaveInfo* saveInfo{nullptr};
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceaud   = CR::Engine::Audio;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::OptionsMenuScreen::OptionsMenuScreen() {
	m_requestShowHomeMenu = false;
}

cg::OptionsMenuScreen::~OptionsMenuScreen() {
	delete view;
}

// IState
bool cg::OptionsMenuScreen::Begin() {
	saveInfo = GetSaveGameManager().LoadSettingsSaveInfo();

	view = new OptionsMenuView(true);
	view->OnExitClicked += Delegate(this, &OptionsMenuScreen::OnExitOptionsMenu);
	view->OnEraseData += Delegate(this, &OptionsMenuScreen::OnEraseData);
	view->LoadFromSaveInfo(saveInfo);

	GetSaveGameManager().SetCurrentSaveOnTerminate(this);
	return true;
}

void cg::OptionsMenuScreen::End() {
	OnSaveOnTerminate();
	GetSaveGameManager().SetCurrentSaveOnTerminate(nullptr);
	delete view;
	view = nullptr;
}

int cg::OptionsMenuScreen::Process() {
	view->Update();
	view->Render();

	if(m_requestShowHomeMenu) {
		m_requestShowHomeMenu = false;
		return Constants::HOME_MENU_STATE;
	}

	return CR::Utility::IState::UNCHANGED;
}

void cg::OptionsMenuScreen::OnExitOptionsMenu() {
	m_requestShowHomeMenu = true;
}

void cg::OptionsMenuScreen::OnEraseData() {
	GetSaveGameManager().Reset();
	saveInfo = GetSaveGameManager().LoadSettingsSaveInfo();
	view->LoadFromSaveInfo(saveInfo);
}

void cg::OptionsMenuScreen::OnSaveOnTerminate() {
	view->WriteToSaveInfo(saveInfo);
	GetSaveGameManager().SaveSettingsSaveInfo(saveInfo);
}