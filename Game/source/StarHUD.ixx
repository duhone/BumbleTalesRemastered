/*
 *  StarHUD.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/21/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.StarHUD;

import CR.Game.IRenderable;
import CR.Game.NumberFont;
import CR.Game.InputButton;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class StarHUD : public InputButton, public IRenderable {
	  public:
		StarHUD();
		virtual ~StarHUD();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		void AddStar();
		void SetStars(int _value);
		int GetStars() const;

		void SetMaxedOut(bool _value) { maxedOut = _value; }

	  private:
		glm::vec2 offset;
		CR::Engine::Graphics::Handles::Sprite starSprite;
		NumberFont* numberFont;
		int m_numStars;
		CR::Engine::Graphics::Handles::Sprite maxTag;
		bool maxedOut;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace CR::Engine::Core::Literals;

cg::StarHUD::StarHUD() {
	SetSpriteAndBounds(0, 0, "StoryModeStarHUD"_hash64, 90);

	starSprite = cegraph::Sprites::Create("StoryModeStarHUDAnim"_hash64);
	cegraph::Sprites::SetZOrders(starSprite, 80);

	numberFont = new NumberFont("StoryModeFont_StarHUD"_hash64, 70);
	m_numStars = 0;

	maxTag = cegraph::Sprites::Create("StoryModeMaxTag"_hash64);
	cegraph::Sprites::SetZOrders(maxTag, 80);

	maxedOut = false;
}

cg::StarHUD::~StarHUD() {
	CR::Engine::Graphics::Sprites::Delete(starSprite);
	CR::Engine::Graphics::Sprites::Delete(maxTag);
	delete numberFont;
}

void cg::StarHUD::Update() {
	if(m_numStars > 0) {
		CR::Engine::Graphics::Sprites::SetAnimationPauses(starSprite, false);
	} else if(m_numStars == 0) {
		CR::Engine::Graphics::Sprites::SetAnimationPauses(starSprite, true);
		CR::Engine::Graphics::Sprites::SetFrames(starSprite, 0);
	}
}

void cg::StarHUD::Render() {
	InputButton::Render();

	numberFont->RenderNumber(m_numStars);
	CR::Engine::Graphics::Sprites::SetVisibilities(maxTag, maxedOut);
	numberFont->SetVisible(!maxedOut);
}
void cg::StarHUD::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;

	InputButton::SetPosition((int)xLoc, (int)yLoc);
	CR::Engine::Graphics::Sprites::SetPositions(starSprite, offset);

	numberFont->SetPosition(xLoc + 30, yLoc + 43);
	CR::Engine::Graphics::Sprites::SetPositions(maxTag, offset);
}

void cg::StarHUD::PauseAnimation(bool pause) {
	CR::Engine::Graphics::Sprites::SetAnimationPauses(starSprite, pause);
}

void cg::StarHUD::AddStar() {
	m_numStars++;
}

void cg::StarHUD::SetStars(int _value) {
	m_numStars = _value;
}

int cg::StarHUD::GetStars() const {
	return m_numStars;
}