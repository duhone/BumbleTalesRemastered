/*
 *  ArcadeModeSaveInfo.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/26/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.ArcadeModeSaveInfo;

import CR.Game.SaveInfo;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class ArcadeModeSaveInfo : public SaveInfo {
	  public:
		ArcadeModeSaveInfo();
		virtual ~ArcadeModeSaveInfo();

		std::vector<int> GetHighScores() const { return m_highScores; }
		void SetHighScores(const std::vector<int>& scores) { m_highScores = scores; }

		std::vector<int> GetHighScoresLevels() const { return m_highScoreLevels; }
		void SetHighScoresLevels(const std::vector<int>& levels) { m_highScoreLevels = levels; }

		std::vector<int> GetBlocksCleared() const { return m_blocksCleared; }
		void SetBlocksCleared(const std::vector<int>& blocks) { m_blocksCleared = blocks; }

		int GetLargestChain() const { return m_largetsChain; }
		void SetLargestChain(int chain) { m_largetsChain = chain; }

		int GetLargestMatch() const { return m_largestMatch; }
		void SetLargestMatch(int match) { m_largestMatch = match; }

		int GetMaxPointsOnOneMove() const { return m_maxPointsOnOneMove; }
		void SetMaxPointsOnOneMove(int points) { m_maxPointsOnOneMove = points; }

		int GetMaxMovesInSession() const { return m_maxMovesInSession; }
		void SetMaxMovesInSession(int moves) { m_maxMovesInSession = moves; }

		int GetMaxPowersUsedInSession() const { return m_maxPowerUsedInSession; }
		void SetMaxPowersUsedInSession(int powers) { m_maxPowerUsedInSession = powers; }

		int GetLongestSessionTime() const { return m_longestSessionTime; }
		void SetLongestSessionTime(int seconds) { m_longestSessionTime = seconds; }

		int GetTotalPlayTime() { return m_totalPlayTime; }
		void SetTotalPlayTime(int _value) { m_totalPlayTime = _value; }

		void Save() override;
		void Load() override;
		void Reset() override;

	  private:
		int m_largetsChain;
		int m_largestMatch;
		int m_maxPointsOnOneMove;
		int m_maxMovesInSession;
		int m_maxPowerUsedInSession;
		int m_longestSessionTime;
		int m_totalPlayTime;
		std::vector<int> m_blocksCleared;
		std::vector<int> m_highScores;
		std::vector<int> m_highScoreLevels;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::ArcadeModeSaveInfo::ArcadeModeSaveInfo() {
	m_blocksCleared.resize(8);
	m_highScores.resize(5);
	m_highScoreLevels.resize(5);
}

cg::ArcadeModeSaveInfo::~ArcadeModeSaveInfo() {}

void cg::ArcadeModeSaveInfo::Save() {
	FILE* file = nullptr;
	file       = fopen("arcade.sav", "wb");
	CR_ASSERT(file, "Failed to open arcade save file");

	fwrite(&m_largetsChain, 1, sizeof(m_largetsChain), file);
	fwrite(&m_largestMatch, 1, sizeof(m_largestMatch), file);
	fwrite(&m_maxPointsOnOneMove, 1, sizeof(m_maxPointsOnOneMove), file);
	fwrite(&m_maxMovesInSession, 1, sizeof(m_maxMovesInSession), file);
	fwrite(&m_maxPowerUsedInSession, 1, sizeof(m_maxPowerUsedInSession), file);
	fwrite(&m_longestSessionTime, 1, sizeof(m_longestSessionTime), file);
	fwrite(&m_totalPlayTime, 1, sizeof(m_totalPlayTime), file);
	fwrite(&m_blocksCleared[0], sizeof(int), m_blocksCleared.size(), file);
	fwrite(&m_highScores[0], sizeof(int), m_highScores.size(), file);
	fwrite(&m_highScoreLevels[0], sizeof(int), m_highScoreLevels.size(), file);

	fclose(file);
}

void cg::ArcadeModeSaveInfo::Load() {
	FILE* file = nullptr;
	file       = fopen("arcade.sav", "rb");
	if(!file) {
		Reset();
		return;
	}

	fread(&m_largetsChain, 1, sizeof(m_largetsChain), file);
	fread(&m_largestMatch, 1, sizeof(m_largestMatch), file);
	fread(&m_maxPointsOnOneMove, 1, sizeof(m_maxPointsOnOneMove), file);
	fread(&m_maxMovesInSession, 1, sizeof(m_maxMovesInSession), file);
	fread(&m_maxPowerUsedInSession, 1, sizeof(m_maxPowerUsedInSession), file);
	fread(&m_longestSessionTime, 1, sizeof(m_longestSessionTime), file);
	fread(&m_totalPlayTime, 1, sizeof(m_totalPlayTime), file);
	fread(&m_blocksCleared[0], sizeof(int), m_blocksCleared.size(), file);
	fread(&m_highScores[0], sizeof(int), m_highScores.size(), file);
	fread(&m_highScoreLevels[0], sizeof(int), m_highScoreLevels.size(), file);

	fclose(file);
}

void cg::ArcadeModeSaveInfo::Reset() {
	m_largetsChain          = 0;
	m_largestMatch          = 0;
	m_maxPointsOnOneMove    = 0;
	m_maxMovesInSession     = 0;
	m_maxPowerUsedInSession = 0;
	m_longestSessionTime    = 0;
	m_totalPlayTime         = 0;

	memset(&m_blocksCleared[0], 0, m_blocksCleared.size() * sizeof(int));
	memset(&m_highScores[0], 0, m_highScores.size() * sizeof(int));
	memset(&m_highScoreLevels[0], 0, m_highScoreLevels.size() * sizeof(int));
}