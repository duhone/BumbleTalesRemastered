/*
 *  ArcadeRecords.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/16/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.ArcadeRecords;

import CR.Engine;

import CR.Game.IRenderable;
import CR.Game.NumberFont;
import CR.Game.TimeDisplay;
import CR.Game.ArcadeModeSaveInfo;

import std;
import std.compat;

export namespace CR::Game {
	class ArcadeRecords : public IRenderable {
	  public:
		ArcadeRecords();
		virtual ~ArcadeRecords();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		void LoadFromArcadeSaveInfo(ArcadeModeSaveInfo* saveInfo);

	  private:
		ArcadeModeSaveInfo* saveInfo;
		CR::Engine::Graphics::Handles::Sprite bgSprite;
		NumberFont* smallNumFont;
		NumberFont* bigNumFont;

		std::vector<int> highScores;
		std::vector<int> highScoresLevels;
		int largestMatch;
		int largestChain;
		int maxPointsOnSingleMove;
		int highestNumMoves;
		int mostPowersUsed;
		int longestSession;
		TimeDisplay* timeDisplay;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::ArcadeRecords::ArcadeRecords() {
	bgSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookRecordsBase2"));
	cegraph::Sprites::SetZOrder(bgSprite, 30);
	cegraph::Sprites::SetPosition(bgSprite, {0, 0});

	bigNumFont = new NumberFont(cecore::C_Hash64("ScrapbookFont_Records2"), 20);
	bigNumFont->SetAlignment(AlignFontRight);

	smallNumFont = new NumberFont(cecore::C_Hash64("ScrapbookFont_Records1"), 20);
	smallNumFont->SetAlignment(AlignFontRight);

	timeDisplay = new TimeDisplay(cecore::C_Hash64("ScrapbookFont_Records1"));
}

cg::ArcadeRecords::~ArcadeRecords() {
	cegraph::Sprites::Delete(bgSprite);
	delete bigNumFont;
	delete smallNumFont;
	delete timeDisplay;
}

void cg::ArcadeRecords::Update() {}

void cg::ArcadeRecords::Render() {
	smallNumFont->Begin();
	bigNumFont->Begin();

	bigNumFont->SetAlignment(AlignFontRight);
	bigNumFont->SetPosition(50, 135);
	bigNumFont->RenderNumber(highScores[0]);
	bigNumFont->SetPosition(50, 160);
	bigNumFont->RenderNumber(highScores[1]);
	bigNumFont->SetPosition(50, 186);
	bigNumFont->RenderNumber(highScores[2]);
	bigNumFont->SetPosition(50, 212);
	bigNumFont->RenderNumber(highScores[3]);
	bigNumFont->SetPosition(50, 238);
	bigNumFont->RenderNumber(highScores[4]);

	bigNumFont->SetAlignment(AlignFontLeft);
	bigNumFont->SetPosition(310, 135);
	bigNumFont->RenderNumber(highScoresLevels[0]);
	bigNumFont->SetPosition(310, 160);
	bigNumFont->RenderNumber(highScoresLevels[1]);
	bigNumFont->SetPosition(310, 186);
	bigNumFont->RenderNumber(highScoresLevels[2]);
	bigNumFont->SetPosition(310, 212);
	bigNumFont->RenderNumber(highScoresLevels[3]);
	bigNumFont->SetPosition(310, 238);
	bigNumFont->RenderNumber(highScoresLevels[4]);

	smallNumFont->SetAlignment(AlignFontLeft);
	smallNumFont->SetPosition(160, 279);
	smallNumFont->RenderNumber(largestMatch);

	smallNumFont->SetPosition(310, 279);
	smallNumFont->RenderNumber(largestChain);

	smallNumFont->SetPosition(310, 305);
	smallNumFont->RenderNumber(maxPointsOnSingleMove);

	smallNumFont->SetPosition(310, 333);
	smallNumFont->RenderNumber(highestNumMoves);

	smallNumFont->SetPosition(310, 361);
	smallNumFont->RenderNumber(mostPowersUsed);

	timeDisplay->SetPosition(262, 391);
	timeDisplay->Render();

	bigNumFont->End();
	smallNumFont->End();
}

void cg::ArcadeRecords::SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) {}

void cg::ArcadeRecords::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::ArcadeRecords::LoadFromArcadeSaveInfo(ArcadeModeSaveInfo* a_saveInfo) {
	highScores            = a_saveInfo->GetHighScores();
	highScoresLevels      = a_saveInfo->GetHighScoresLevels();
	largestMatch          = a_saveInfo->GetLargestMatch();
	largestChain          = a_saveInfo->GetLargestChain();
	maxPointsOnSingleMove = a_saveInfo->GetMaxPointsOnOneMove();
	highestNumMoves       = a_saveInfo->GetMaxMovesInSession();
	mostPowersUsed        = a_saveInfo->GetMaxPowersUsedInSession();
	longestSession        = a_saveInfo->GetLongestSessionTime();

	timeDisplay->SetSeconds(longestSession);
}