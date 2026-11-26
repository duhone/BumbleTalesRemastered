/*
 *  TileSelector.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/24/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.TileSelector;

import CR.Game.Constants;
import CR.Game.IRenderable;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class TileSelector : public IRenderable {
	  public:
		TileSelector(float tileSpacing, glm::vec2 puzzleOffset);
		~TileSelector();

		// Methods
		void Update();
		void Render();
		void SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) {};
		void PauseAnimation(bool pause);

		// Properties
		virtual bool Visible() const { return m_visible; }
		virtual void SetVisible(bool _value) { m_visible = _value; }
		void SetPosition(glm::ivec2 position);
		glm::ivec2 GetPosition() const { return m_position; }

	  private:
		glm::ivec2 m_position{};    // position in the tile matrix
		float m_tileSpacing{};
		glm::vec2 m_puzzleOffset{};
		CR::Engine::Graphics::Handles::Sprite m_selectorSprite;
		bool m_visible{true};
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cecore  = CR::Engine::Core;
namespace cg      = CR::Game;

using namespace cecore::Literals;

cg::TileSelector::TileSelector(float tileSpacing, glm::vec2 puzzleOffset) :
	m_tileSpacing(tileSpacing), m_puzzleOffset(puzzleOffset) {
	cegraph::Sprites::Create("PiecesSelector1"_hash64, m_selectorSprite);
	cegraph::Sprites::SetZOrders(m_selectorSprite, 20);

	SetPosition(m_position);
}

cg::TileSelector::~TileSelector() {
	CR::Engine::Graphics::Sprites::Delete(m_selectorSprite);
}

void cg::TileSelector::SetPosition(glm::ivec2 position) {
	// Return w/no changes if player touched out of puzzle's bounds
	if(position.x > (Constants::c_puzzleXTiles - 1) || position.x < 0 || position.y > (Constants::c_puzzleYTiles - 1) ||
	   position.y < 0)
		return;

	// set the position of the selection sprite
	m_position = position;
	glm::vec2 tileSize{Constants::c_resourceTileWidth, Constants::c_resourceTileHeight};
	cegraph::Sprites::SetPositions(m_selectorSprite,
								   glm::vec2{m_position} * (tileSize + m_tileSpacing) + m_puzzleOffset);
}

void cg::TileSelector::Update() {}

void cg::TileSelector::Render() {
	cegraph::Sprites::SetVisibilities(m_selectorSprite, m_visible);
}

void cg::TileSelector::PauseAnimation(bool pause) {
	cegraph::Sprites::SetAnimationPauses(m_selectorSprite, pause);
}