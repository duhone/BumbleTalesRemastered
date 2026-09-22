/*
 *  StoryModeSaveInfo.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/26/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.StoryModeSaveInfo;

import CR.Game.SaveInfo;
import CR.Game.BuildingLevels;
import CR.Game.CharacterLevels;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class StoryModeSaveInfo : public SaveInfo {
	  public:
		StoryModeSaveInfo();
		virtual ~StoryModeSaveInfo();

		int GetLevel() const { return m_level; }
		void SetLevel(int _value) { m_level = _value; }
		int GetStars() const { return m_stars; }
		void SetStars(int _value) { m_stars = _value; }

		int GetStarsReceived() const { return m_starsRecieved; }
		void SetStarsReceived(int _value) { m_starsRecieved = _value; }

		int GetBronzeMedals() const { return m_bronzeMedals; }
		void SetBronzeMedals(int _value) { m_bronzeMedals = _value; }

		int GetSilverMedals() const { return m_silverMedals; }
		void SetSilverMedals(int _value) { m_silverMedals = _value; }

		int GetGoldMedals() const { return m_goldMedals; }
		void SetGoldMedals(int _value) { m_goldMedals = _value; }

		int GetTotalPlayTime() { return m_totalPlayTime; }
		void SetTotalPlayTime(int _value) { m_totalPlayTime = _value; }

		bool GetHasWonGame() { return m_hasWonGame; }
		void SetHasWonGame(bool _value) { m_hasWonGame = _value; }

		int GetLargestChain() const { return m_largestChain; }
		void SetLargestChain(int chain) { m_largestChain = chain; }

		int GetLargestMatch() const { return m_laregestMatch; }
		void SetLargestMatch(int match) { m_laregestMatch = match; }

		bool GetHasOpenedStore() { return m_hasOpenedStore; }
		void SetHasOpenedStore(bool _value) { m_hasOpenedStore = _value; }

		CharacterName GetCurrentCharacter() const { return m_currentCharacter; }
		void SetCurrentCharacter(CharacterName _value) { m_currentCharacter = _value; }

		BuildingLevels* GetBuildingLevels() { return &m_bLevels; }
		void SetBuildingLevels([[maybe_unused]] BuildingLevels* _value) {}

		CharacterLevels* GetCharacterLevels() { return &m_cLevels; }
		void SetCharacterLevels([[maybe_unused]] CharacterLevels* _value) {}

		std::vector<int> GetBlocksCleared() const { return m_blocksCleared; }
		void SetBlocksCleared(std::vector<int>& blocks) { m_blocksCleared = blocks; }

		virtual void Save();
		virtual void Load();
		virtual void Reset();

	  private:
		int m_level;
		int m_stars;
		int m_starsRecieved;
		int m_bronzeMedals;
		int m_silverMedals;
		int m_goldMedals;
		int m_totalPlayTime;
		bool m_hasWonGame;
		int m_largestChain;
		int m_laregestMatch;
		bool m_hasOpenedStore;
		CharacterName m_currentCharacter;
		std::vector<int> m_blocksCleared;

		BuildingLevels m_bLevels;
		CharacterLevels m_cLevels;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::StoryModeSaveInfo::StoryModeSaveInfo() {
	m_blocksCleared.resize(8);
	Reset();
}

cg::StoryModeSaveInfo::~StoryModeSaveInfo() {}

void cg::StoryModeSaveInfo::Save() {
	// always save locally as well, in case we don't have a scoreloop connection next run.
	FILE* file = nullptr;
	file       = fopen("story.sav", "wb");
	CR_ASSERT(file, "Failed to open story save file");

	fwrite(&m_level, 1, sizeof(m_level), file);
	fwrite(&m_stars, 1, sizeof(m_stars), file);
	fwrite(&m_starsRecieved, 1, sizeof(m_starsRecieved), file);
	fwrite(&m_bronzeMedals, 1, sizeof(m_bronzeMedals), file);
	fwrite(&m_silverMedals, 1, sizeof(m_silverMedals), file);
	fwrite(&m_goldMedals, 1, sizeof(m_goldMedals), file);
	fwrite(&m_totalPlayTime, 1, sizeof(m_totalPlayTime), file);
	fwrite(&m_hasWonGame, 1, sizeof(m_hasWonGame), file);
	fwrite(&m_largestChain, 1, sizeof(m_largestChain), file);
	fwrite(&m_laregestMatch, 1, sizeof(m_laregestMatch), file);
	fwrite(&m_hasOpenedStore, 1, sizeof(m_hasOpenedStore), file);
	fwrite(&m_currentCharacter, 1, sizeof(m_currentCharacter), file);
	fwrite(&m_blocksCleared[0], sizeof(int), m_blocksCleared.size(), file);

	m_cLevels.Save(file);
	m_bLevels.Save(file);

	fclose(file);
}

void cg::StoryModeSaveInfo::Load() {
	FILE* file = nullptr;
	file       = fopen("story.sav", "rb");

	if(!file) {
		Reset();
		return;
	}

	fread(&m_level, 1, sizeof(m_level), file);
	fread(&m_stars, 1, sizeof(m_stars), file);
	fread(&m_starsRecieved, 1, sizeof(m_starsRecieved), file);
	fread(&m_bronzeMedals, 1, sizeof(m_bronzeMedals), file);
	fread(&m_silverMedals, 1, sizeof(m_silverMedals), file);
	fread(&m_goldMedals, 1, sizeof(m_goldMedals), file);
	fread(&m_totalPlayTime, 1, sizeof(m_totalPlayTime), file);
	fread(&m_hasWonGame, 1, sizeof(m_hasWonGame), file);
	fread(&m_largestChain, 1, sizeof(m_largestChain), file);
	fread(&m_laregestMatch, 1, sizeof(m_laregestMatch), file);
	fread(&m_hasOpenedStore, 1, sizeof(m_hasOpenedStore), file);
	fread(&m_currentCharacter, 1, sizeof(m_currentCharacter), file);
	fread(&m_blocksCleared[0], sizeof(int), m_blocksCleared.size(), file);

	m_cLevels.Load(file);
	m_bLevels.Load(file);

	fclose(file);
}

void cg::StoryModeSaveInfo::Reset() {
	m_level            = 1;
	m_stars            = 0;
	m_starsRecieved    = 0;
	m_bronzeMedals     = 0;
	m_silverMedals     = 0;
	m_goldMedals       = 0;
	m_totalPlayTime    = 0;
	m_hasWonGame       = 0;
	m_largestChain     = 0;
	m_laregestMatch    = 0;
	m_hasOpenedStore   = 0;
	m_currentCharacter = Mayor;
	memset(&m_blocksCleared[0], 0, m_blocksCleared.size() * sizeof(int));

	m_cLevels.Reset();
	m_bLevels.Reset();
}