/*
 *  SpecialSelector.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/3/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.SpecialSelector;

import CR.Engine;

import CR.Game.Constants;
import CR.Game.IRenderable;
import CR.Game.Event;

import std;
import std.compat;

export namespace CR::Game {
	class ResourcePuzzle;

	class SpecialSelector : public IRenderable {
	  public:
		SpecialSelector(ResourcePuzzle* parentPuzzle);
		virtual ~SpecialSelector();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		void SetTilePosition(int x, int y, float tileSpacing, glm::vec2 parentOffset);

		void ShowSelector2();
		void ShowSelector3();

		bool HideTiles() const;
		bool IsSelector3() const { return m_isSelector3; }

		CR::Utility::Event OnDoneAnimating;
		CR::Utility::Event OnExplodeTiles;

		int GetCurrFrame() const;

	  private:
		enum class Type { Selector2, Selector3 };
		void SetupSprite(Type a_type);

		glm::vec2 offset;
		CR::Engine::Graphics::Handles::Sprite selectorSprite;
		ResourcePuzzle* m_parentPuzzle;
		bool m_isSelector3;

		bool tilesExploded;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::SpecialSelector::SpecialSelector(ResourcePuzzle* parentPuzzle) {
	SetupSprite(Type::Selector2);
	m_parentPuzzle = parentPuzzle;
	tilesExploded  = false;
}

cg::SpecialSelector::~SpecialSelector() {
	if(selectorSprite.isValid()) { cegraph::Sprites::Delete(selectorSprite); }
}

void cg::SpecialSelector::Update() {
	if(m_isSelector3) {
		bool animatingDone;
		cegraph::Sprites::AreAnimationsDone(selectorSprite, animatingDone);
		if(!animatingDone) {
			OnDoneAnimating();
			ShowSelector2();
			tilesExploded = false;
		} else {
			if(GetCurrFrame() > 12) {
				if(!tilesExploded) {
					OnExplodeTiles();
					tilesExploded = true;
				}
			}
		}
	}
}

void cg::SpecialSelector::Render() {}

void cg::SpecialSelector::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;

	cegraph::Sprites::SetPositions(selectorSprite, offset);
}

void cg::SpecialSelector::PauseAnimation(bool pause) {
	cegraph::Sprites::SetAnimationPauses(selectorSprite, pause);
}

void cg::SpecialSelector::SetTilePosition(int x, int y, float tileSpacing, glm::vec2 parentOffset) {
	// Return w/no changes if player touched out of puzzle's bounds
	if(x > (Constants::c_puzzleXTiles - 1) || x < 0 || y > (Constants::c_puzzleYTiles - 1) || y < 0) return;

	SetPosition(x * Constants::c_resourceTileWidth + Constants::c_resourceTileWidth / 2 + (x * tileSpacing) +
	                parentOffset.x,
	            y * Constants::c_resourceTileHeight + Constants::c_resourceTileHeight / 2 +
	                (y * tileSpacing) + parentOffset.y);
}

void cg::SpecialSelector::ShowSelector2() {
	SetupSprite(Type::Selector2);
}

void cg::SpecialSelector::ShowSelector3() {
	SetupSprite(Type::Selector3);
}

bool cg::SpecialSelector::HideTiles() const {
	if(GetCurrFrame() > 12)
		return true;
	else
		return false;
}

int cg::SpecialSelector::GetCurrFrame() const {
	uint16_t result;
	cegraph::Sprites::GetCurrentFrames(selectorSprite, result);
	return result;
}

void cg::SpecialSelector::SetupSprite(Type a_type) {
	if(selectorSprite.isValid()) { cegraph::Sprites::Delete(selectorSprite); }
	switch(a_type) {
		case Type::Selector2:
			cegraph::Sprites::Create(cecore::C_Hash64("PiecesSelector2"), selectorSprite);
			cegraph::Sprites::SetPositions(selectorSprite, offset);
			cegraph::Sprites::SetZOrders(selectorSprite, 80);
			m_isSelector3 = false;
			break;
		case Type::Selector3:
			cegraph::Sprites::Create(cecore::C_Hash64("PiecesSelector3"), selectorSprite);
			cegraph::Sprites::SetPositions(selectorSprite, offset);
			cegraph::Sprites::SetZOrders(selectorSprite, 80);
			cegraph::Sprites::StartAnimations(selectorSprite);
			m_isSelector3 = true;
			break;
	}
}