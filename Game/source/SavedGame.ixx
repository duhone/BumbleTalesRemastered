/*
 *  SavedGame.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.SavedGame;

import CR.Game.Event;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SavedGame {
	  public:
		SavedGame();
		~SavedGame();

		void Save();
		void Load();
		void Reset();

		void SaveOnTerminate();
		CR::Utility::Event OnSaveOnTerminate;

		int GetLevel() const;
		void SetLevel(int _value);

		int GetStars() const;
		void SetStars(int _value);

		bool GetOptionsHintsOn() const;
		void SetOptionsHintsOn(bool _value);

		bool GetOptionsMusicOn() const;
		void SetOptionsMusicOn(bool _value);

		bool GetOptionsSoundOn() const;
		void SetOptionsSoundOn(bool _value);

	  private:
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::SavedGame::SavedGame() {}

cg::SavedGame::~SavedGame() {}

void cg::SavedGame::Save() {}

void cg::SavedGame::Load() {}

void cg::SavedGame::Reset() {}

void cg::SavedGame::SaveOnTerminate() {
	if(OnSaveOnTerminate.Size() > 0) OnSaveOnTerminate();
}

int cg::SavedGame::GetLevel() const {
	return 0;
}

void cg::SavedGame::SetLevel([[maybe_unused]] int _value) {}

int cg::SavedGame::GetStars() const {
	return 0;
}

void cg::SavedGame::SetStars([[maybe_unused]] int _value) {}

bool cg::SavedGame::GetOptionsHintsOn() const {
	return false;
}

void cg::SavedGame::SetOptionsHintsOn([[maybe_unused]] bool _value) {}

bool cg::SavedGame::GetOptionsMusicOn() const {
	return false;
}

void cg::SavedGame::SetOptionsMusicOn([[maybe_unused]] bool _value) {}

bool cg::SavedGame::GetOptionsSoundOn() const {
	return false;
}

void cg::SavedGame::SetOptionsSoundOn([[maybe_unused]] bool _value) {}