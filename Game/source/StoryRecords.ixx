/*
 *  StoryRecords.h
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

import CR.Game.Constants;
import CR.Game.IRenderable;
import CR.Game.NumberFont;
import CR.Game.StoryModeSaveInfo;

import std;
import std.compat;

export namespace CR::Game {
	class StoryRecords : public IRenderable {
	  public:
		StoryRecords();
		virtual ~StoryRecords();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		void LoadFromStorySaveInfo(StoryModeSaveInfo* saveInfo);

	  private:
		StoryModeSaveInfo* saveInfo;
		CR::Engine::Graphics::Handles::Sprite bgSprite;
		NumberFont* smallNumFont;
		NumberFont* specialFont;

		int currentLevel;
		int purchaseCompletion;
		int largestMatch;
		int largestChain;
		std::vector<int> blocksCleared;
		int bronzeMedals;
		int silverMedals;
		int goldMedals;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::StoryRecords::StoryRecords() {
	bgSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookRecordsBase1"));
	cegraph::Sprites::SetZOrder(bgSprite, 30);
	cegraph::Sprites::SetPosition(bgSprite, {0, 0});

	specialFont = new NumberFont(cecore::C_Hash64("ScrapbookFont_Records1"), 20);
	specialFont->SetAlignment(AlignFontRight);

	smallNumFont = new NumberFont(cecore::C_Hash64("ScrapbookFont_Records1"), 20);
	smallNumFont->SetAlignment(AlignFontRight);
}

cg::StoryRecords::~StoryRecords() {
	cegraph::Sprites::Delete(bgSprite);
	delete smallNumFont;
	delete specialFont;
}

void cg::StoryRecords::Update() {}

void cg::StoryRecords::Render() {
	smallNumFont->Begin();

	smallNumFont->SetAlignment(AlignFontLeft);
	smallNumFont->SetPosition(310, 95);
	smallNumFont->RenderNumber(currentLevel);

	smallNumFont->SetPosition(272, 122);
	smallNumFont->RenderNumber(purchaseCompletion);
	specialFont->SetPosition(273, 122);
	specialFont->RenderSpecial(12);
	smallNumFont->SetAlignment(AlignFontRight);
	smallNumFont->SetPosition(286, 122);
	smallNumFont->RenderNumber(90);

	smallNumFont->SetAlignment(AlignFontLeft);
	smallNumFont->SetPosition(310, 148);
	smallNumFont->RenderNumber(largestMatch);

	smallNumFont->SetPosition(310, 174);
	smallNumFont->RenderNumber(largestChain);

	smallNumFont->SetPosition(152, 236);
	smallNumFont->RenderNumber(blocksCleared[Constants::Brick]);
	smallNumFont->SetPosition(152, 267);
	smallNumFont->RenderNumber(blocksCleared[Constants::Wood]);
	smallNumFont->SetPosition(152, 298);
	smallNumFont->RenderNumber(blocksCleared[Constants::Leaf]);
	smallNumFont->SetPosition(152, 329);
	smallNumFont->RenderNumber(blocksCleared[Constants::QTile]);

	smallNumFont->SetPosition(290, 236);
	smallNumFont->RenderNumber(blocksCleared[Constants::Gold]);
	smallNumFont->SetPosition(290, 267);
	smallNumFont->RenderNumber(blocksCleared[Constants::Crystal]);
	smallNumFont->SetPosition(290, 298);
	smallNumFont->RenderNumber(blocksCleared[Constants::Diamond]);
	smallNumFont->SetPosition(290, 329);
	smallNumFont->RenderNumber(blocksCleared[Constants::MTile]);

	smallNumFont->SetPosition(121, 396);
	// smallNumFont->SetAlignment(AlignFontRight);
	// smallNumFont->SetPosition(55, 396);
	smallNumFont->RenderNumber(bronzeMedals);

	smallNumFont->SetPosition(218, 396);
	// smallNumFont->SetPosition(154, 396);
	smallNumFont->RenderNumber(silverMedals);

	smallNumFont->SetPosition(315, 396);
	// smallNumFont->SetPosition(253, 396);
	smallNumFont->RenderNumber(goldMedals);

	smallNumFont->End();
}

void cg::StoryRecords::SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) {}

void cg::StoryRecords::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::StoryRecords::LoadFromStorySaveInfo(StoryModeSaveInfo* a_saveInfo) {
	currentLevel = a_saveInfo->GetLevel();
	// purchaseCompletion = 0;
	largestMatch  = a_saveInfo->GetLargestMatch();
	largestChain  = a_saveInfo->GetLargestChain();
	blocksCleared = a_saveInfo->GetBlocksCleared();
	bronzeMedals  = a_saveInfo->GetBronzeMedals();
	silverMedals  = a_saveInfo->GetSilverMedals();
	goldMedals    = a_saveInfo->GetGoldMedals();

	// Calculate Purchase Completion
	purchaseCompletion = 0;

	BuildingLevels* bLevels = a_saveInfo->GetBuildingLevels();
	purchaseCompletion += bLevels->GetBuildingLevel(TownHall);
	purchaseCompletion += bLevels->GetBuildingLevel(PoliceStation);
	purchaseCompletion += bLevels->GetBuildingLevel(Firehouse);
	purchaseCompletion += bLevels->GetBuildingLevel(Lodge);
	purchaseCompletion += bLevels->GetBuildingLevel(Theatre);
	purchaseCompletion += bLevels->GetBuildingLevel(Greenhouse);
	// delete bLevels;

	CharacterLevels* cLevels = a_saveInfo->GetCharacterLevels();
	purchaseCompletion += cLevels->GetCharacterLevel(Mayor);
	purchaseCompletion += cLevels->GetCharacterLevel(Elder);
	purchaseCompletion += cLevels->GetCharacterLevel(Fireman);
	purchaseCompletion += cLevels->GetCharacterLevel(Dog);
	purchaseCompletion += cLevels->GetCharacterLevel(Baker);
	purchaseCompletion += cLevels->GetCharacterLevel(Teacher);
	purchaseCompletion += cLevels->GetCharacterLevel(Librarian);
	purchaseCompletion += cLevels->GetCharacterLevel(Romeo);
	purchaseCompletion += cLevels->GetCharacterLevel(Felicia);
	purchaseCompletion += cLevels->GetCharacterLevel(Bruiser);
	purchaseCompletion += cLevels->GetCharacterLevel(Marla);
	purchaseCompletion += cLevels->GetCharacterLevel(ChopChop);
	// delete cLevels;
}