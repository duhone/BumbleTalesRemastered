/*
 *  SaveInfo.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/26/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

export module CR.Game.SaveInfo;

import std;
import std.compat;

export namespace CR::Game {
	class SaveInfo {
	  public:
		friend class SaveGameManager;

		SaveInfo();
		virtual ~SaveInfo();

		virtual void Save();
		virtual void Load();
		virtual void Reset();

	  protected:
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::SaveInfo::SaveInfo() {}

cg::SaveInfo::~SaveInfo() {}

void cg::SaveInfo::Save() {}

void cg::SaveInfo::Load() {}

void cg::SaveInfo::Reset() {
	Save();
}
