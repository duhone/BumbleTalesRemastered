/*
 *  TrophySaveInfo.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/9/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

module;

#include <engine/Engine.h>

export module CR.Game.TrophySaveInfo;

import CR.Game.SaveInfo;
import CR.Game.Trophies;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class TrophySaveInfo : public SaveInfo {
	  public:
		TrophySaveInfo();
		virtual ~TrophySaveInfo();

		Trophies* GetTrophies();
		void SetTrophies(Trophies* trophies);

		virtual void Save();
		virtual void Load();
		virtual void Reset();

	  private:
		Trophies m_trophies;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::TrophySaveInfo::TrophySaveInfo() {}

cg::TrophySaveInfo::~TrophySaveInfo() {}

cg::Trophies* cg::TrophySaveInfo::GetTrophies() {
	return &m_trophies;
}

void cg::TrophySaveInfo::SetTrophies([[maybe_unused]] Trophies* trophies) {}

void cg::TrophySaveInfo::Save() {
	m_trophies.Save();
}

void cg::TrophySaveInfo::Load() {
	m_trophies.Load();
}

void cg::TrophySaveInfo::Reset() {
	m_trophies.Reset();
}
