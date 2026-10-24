/*
 *  StarMeter.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/10/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.StarMeter;

import CR.Game.IRenderable;
import CR.Game.RepeatedSpriteHelper;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class StarMeter : public IRenderable {
	  public:
		StarMeter();
		virtual ~StarMeter();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		void SetFilledStars(int _value);
		int GetFilledStars() const { return m_starCount; }
		void SetStarIndicator(int _value);
		int GetStarIndicator() const { return m_starIndicator; }

	  private:
		glm::vec2 offset;
		CR::Engine::Graphics::Handles::Sprite meterSprite;
		int m_starCount;
		int m_starIndicator;
		CR::Game::RepeatedSpriteHelper m_spriteHelper;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::StarMeter::StarMeter() : m_spriteHelper(cecore::C_Hash64("ShopStarMeterIndicator"), 50) {
	meterSprite = cegraph::Sprites::Create(cecore::C_Hash64("ShopStarMeter"));
	cegraph::Sprites::SetZOrder(meterSprite, 50);

	m_starCount     = 0;
	m_starIndicator = 0;
}

cg::StarMeter::~StarMeter() {
	cegraph::Sprites::Delete(meterSprite);
}

// IRenderable
void cg::StarMeter::Update() {}

void cg::StarMeter::Render() {
	int availableStars = m_starIndicator;

	m_spriteHelper.Start();
	for(int i = 0; i < 5; i++) {
		if(i >= m_starCount && availableStars > 0) {
			m_spriteHelper.NewSprite();
			m_spriteHelper.SetPosition(int(offset.x + 53 - (26 * i)), int(offset.y - 3));
			availableStars--;
		}
	}
	m_spriteHelper.End(true);
}

void cg::StarMeter::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;

	cegraph::Sprites::SetPosition(meterSprite, offset);
}

void cg::StarMeter::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::StarMeter::SetFilledStars(int _value) {
	if(_value == -1 || m_starCount == _value) return;

	m_starCount = _value;
	cegraph::Sprites::SetFrame(meterSprite, (uint16_t)m_starCount);
}

void cg::StarMeter::SetStarIndicator(int _value) {
	m_starIndicator = _value;
}