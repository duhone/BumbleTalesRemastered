/*
 *  Trophies.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/9/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

module;

#include <engine/Engine.h>

export module CR.Game.Trophies;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	enum TrophyName {
		CityMasterTrophy,
		WellRoundedTrophy,
		IHeartBumblesTrophy,
		MasterStoryTellerTrophy,
		BrickBreakerTrophy,
		WoodChipperTrophy,
		LeafRakerTrophy,
		GoldMinerTrophy,
		CrystalClearerTrophy,
		GemCollectorTrophy,
		MasterPieceTrophy,
		ResourcefulTrophy,
		OnTheLevelTrophy,
		CenturionTrophy,
		MedalManiacTrophy,
		HeavyMedalTrophy,
		PowerHungryTrophy,
		SimpleChainTrophy,
		ChainReactionTrophy,
		MonsterChainTrophy,
		HighFiveTrophy,
		MegaMatchTrophy,
		TheWildBunchTrophy,
		TimeOnYourHandsTrophy,
		ArcadeNewbieTrophy,
		ArcadeProTrophy,
		ArcadeMasterTrophy,
		TheArtOfScoreTrophy,
		MasterStrategistTrophy,
		GreatStreakTrophy,
		WickedStreakTrophy,
		InsaneStreakTrophy,
		EnergizedTrophy,
		OutOfTheGatesTrophy,
		DareDevilTrophy,
		OldFashionedTrophy,
		NumTrophies
	};
	class Trophies {
	  public:
		Trophies();
		virtual ~Trophies();

		bool GetTrophy(TrophyName trophyName) const;
		void SetTrophy(TrophyName trophyName, bool _value);

		void Save();
		void Load();
		void Reset();

	  private:
		bool m_trophies[NumTrophies];
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::Trophies::Trophies() {
	Reset();
}

cg::Trophies::~Trophies() {}

void cg::Trophies::Reset() {
	memset(m_trophies, 0, sizeof(m_trophies) * sizeof(bool));
}

void cg::Trophies::Load() {
	FILE* file = nullptr;
	file       = fopen("trophies.sav", "rb");
	if(file) {
		fread(m_trophies, 1, sizeof(m_trophies), file);
		fclose(file);
	} else
		Reset();
}

void cg::Trophies::Save() {
	FILE* file = nullptr;
	file       = fopen("trophies.sav", "wb");
	CR_ASSERT(file, "failed to open trophies save");

	fwrite(m_trophies, 1, sizeof(m_trophies), file);

	fclose(file);
}

bool cg::Trophies::GetTrophy(TrophyName trophyName) const {
	return m_trophies[trophyName];
}

void cg::Trophies::SetTrophy(TrophyName trophyName, bool _value) {
	m_trophies[trophyName] = _value;
}
