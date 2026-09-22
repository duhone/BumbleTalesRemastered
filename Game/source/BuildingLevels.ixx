/*
 *  BuildingLevels.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/29/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.BuildingLevels;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	enum BuildingName {
		TownHall           = 0,
		PoliceStation      = 1,
		Firehouse          = 2,
		Lodge              = 3,
		Theatre            = 4,
		Greenhouse         = 5,
		NoBuilding         = 6,
		NUM_BUILDING_NAMES = 7
	};
	class BuildingLevels {
	  public:
		BuildingLevels();
		virtual ~BuildingLevels();

		int GetBuildingLevel(BuildingName bName) const { return m_levels[bName]; }
		void SetBuildingLevel(BuildingName bName, int _value) { m_levels[bName] = _value; }

		void Save(FILE* _file);
		void Load(FILE* _file);
		void Reset();

	  private:
		int m_levels[NUM_BUILDING_NAMES];
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::BuildingLevels::BuildingLevels() {
	Reset();
}

cg::BuildingLevels::~BuildingLevels() {}

void cg::BuildingLevels::Save(FILE* _file) {
	fwrite(m_levels, 1, sizeof(m_levels), _file);
}

void cg::BuildingLevels::Load(FILE* _file) {
	fread(m_levels, 1, sizeof(m_levels), _file);
}

void cg::BuildingLevels::Reset() {
	memset(m_levels, 0, sizeof(m_levels));
}
