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

import CR.Game.IRenderable;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class ResourcePuzzle;

	class TileSelector : public IRenderable {
	  public:
		TileSelector(ResourcePuzzle* parentPuzzle);
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

		static const int tile_width  = 44;
		static const int tile_height = 44;

	  private:
		glm::ivec2 m_position;    // position in the tile matrix
		CR::Engine::Graphics::Handles::Sprite m_selectorSprite;
		ResourcePuzzle* m_parentPuzzle;
		bool m_visible;
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cecore  = CR::Engine::Core;
namespace cg      = CR::Game;

cg::TileSelector::TileSelector(ResourcePuzzle* parentPuzzle) {
	m_parentPuzzle = parentPuzzle;
	m_visible      = true;

	m_selectorSprite = cegraph::Sprites::Create(cecore::C_Hash64("PiecesSelector1"));
	cegraph::Sprites::SetZOrders(m_selectorSprite, 80);

	m_position.x = 0;
	m_position.y = 0;
	SetPosition(m_position);
}

cg::TileSelector::~TileSelector() {
	CR::Engine::Graphics::Sprites::Delete(m_selectorSprite);
}

void cg::TileSelector::SetPosition([[maybe_unused]] glm::ivec2 position) {
	// Return w/no changes if player touched out of puzzle's bounds
	/* TODO: ResourcePuzzle not ported yet
	if(position.x > (ResourcePuzzle::x_tiles - 1) || position.x < 0 ||
	   position.y > (ResourcePuzzle::y_tiles - 1) || position.y < 0)
	    return;

	// set the position of te selection sprite
	m_position = position;
	cegraph::Sprites::SetPositions(
	    m_selectorSprite,
	    glm::vec2(m_position.x * tile_width + tile_width / 2 + (m_position.x * m_parentPuzzle->Spacing()) +
	                  m_parentPuzzle->Offset_X(),
	              m_position.y * tile_height + tile_height / 2 + (m_position.y * m_parentPuzzle->Spacing()) +
	                  m_parentPuzzle->Offset_Y()));
	                  */
}

void cg::TileSelector::Update() {}

void cg::TileSelector::Render() {
	cegraph::Sprites::SetVisibilities(m_selectorSprite, m_visible);
}

void cg::TileSelector::PauseAnimation(bool pause) {
	cegraph::Sprites::SetAnimationPauses(m_selectorSprite, pause);
}