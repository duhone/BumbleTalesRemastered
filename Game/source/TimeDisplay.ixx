/*
 *  TimeDisplay.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/18/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.TimeDisplay;

import CR.Engine;

import CR.Game.IRenderable;
import CR.Game.NumberFont;

import std;
import std.compat;

export namespace CR::Game {
	class TimeDisplay : public IRenderable {
	  public:
		// Porting note, before it was defaulting to font score 2. now you have to pass font in
		TimeDisplay(uint64_t fontHash);
		virtual ~TimeDisplay();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation([[maybe_unused]] bool pause) {}

		void SetSeconds(int _value) { m_seconds = _value; }
		// void SetFontAsset(int _asset);

	  private:
		glm::vec2 offset;
		NumberFont* numberFont;
		NumberFont* colonFont;
		int m_seconds;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::TimeDisplay::TimeDisplay(uint64_t fontHash) {
	numberFont = new NumberFont(fontHash, 40);
	numberFont->SetAlignment(AlignFontRight);
	colonFont = new NumberFont(fontHash, 40);
	colonFont->SetAlignment(AlignFontRight);
	m_seconds = 0;
	SetPosition(0, 0);
}

cg::TimeDisplay::~TimeDisplay() {
	delete numberFont;
	delete colonFont;
}

void cg::TimeDisplay::Update() {}

void cg::TimeDisplay::Render() {
	numberFont->Begin();
	int minutes = m_seconds / 60;
	int seconds = m_seconds % 60;

	int xPos = (int)offset.x;
	numberFont->SetMinDigits(0);
	numberFont->SetPosition((float)xPos, offset.y);
	numberFont->RenderNumber(minutes);

	minutes = minutes / 10;
	xPos += numberFont->GetDigitWidth();
	while(minutes != 0) {
		minutes = minutes / 10;
		xPos += numberFont->GetDigitWidth();
	}

	colonFont->SetPosition((float)xPos, offset.y);
	colonFont->RenderSpecial(11);
	xPos += numberFont->GetDigitWidth();
	numberFont->SetMinDigits(2);
	numberFont->SetPosition((float)xPos, offset.y);
	numberFont->RenderNumber(seconds);
	numberFont->End();
}

void cg::TimeDisplay::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;
}

// void cg::TimeDisplay::PauseAnimation(bool pause) {}

/*void cg::TimeDisplay::SetFontAsset(int _asset) {
    numberFont->SetFontAsset(_asset);
    colonFont->SetFontAsset(_asset);
}*/