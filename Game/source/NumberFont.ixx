/*
 *  NumberFont.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/15/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.NumberFont;

import CR.Engine;

import CR.Game.RepeatedSpriteHelper;

import std;
import std.compat;

export namespace CR::Game {
	enum FontAlignment { AlignFontRight, AlignFontCenter, AlignFontLeft };

	class NumberFont {
	  public:
		NumberFont(uint64_t fontHash, int zPos);
		virtual ~NumberFont();

		void SetPosition(float xLoc, float yLoc);
		void RenderNumber(int _value);
		void RenderSpecial(int _frame);
		void SetMinDigits(int _value);
		int GetMinDigits() const;
		void SetAlignment(FontAlignment _value);
		int GetDigitWidth();
		void Begin();
		void End();
		// void SetFontAsset(int _asset);

	  private:
		glm::vec2 offset;
		uint64_t m_fontHash;
		glm::uvec2 m_fontDimensions;
		// CR::Graphics::Sprite* numberSprite;
		int m_minDigits;
		FontAlignment m_align;
		CR::Game::RepeatedSpriteHelper m_spriteHelper;
		bool m_helperLock;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::NumberFont::NumberFont(uint64_t fontHash, int zPos) :
    m_fontHash(fontHash), m_spriteHelper(fontHash, zPos), m_helperLock(false) {
	// numberSprite = graphics_engine->CreateSprite1(true, zPos);
	// numberSprite->SetImage(fontAsset);
	m_fontDimensions = cegraph::Sprites::GetSize(fontHash);
	m_minDigits      = 0;
	SetPosition(0, 0);
	m_align = AlignFontCenter;
}

cg::NumberFont::~NumberFont() {
	// numberSprite->Release();
}

void cg::NumberFont::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;
}

void cg::NumberFont::Begin() {
	m_spriteHelper.Start();
	m_helperLock = true;
}

void cg::NumberFont::End() {
	m_spriteHelper.End();
	m_helperLock = false;
}

void cg::NumberFont::RenderNumber(int _value) {
	if(!m_helperLock) m_spriteHelper.Start();

	std::stack<int> digits;

	int cValue = _value;

	while(cValue != 0) {
		int digit = cValue % 10;
		cValue    = cValue / 10;
		digits.push(digit);
	}

	while(digits.size() < m_minDigits) { digits.push(0); }

	int xPos = 0;
	if(m_align == AlignFontCenter) {
		if(digits.size() > 0)
			xPos = (int)offset.x - (((int)digits.size() * m_fontDimensions.x) / 2);
		else
			xPos = (int)offset.x - (m_fontDimensions.x) / 2;
	} else if(m_align == AlignFontRight) {
		xPos = (int)offset.x;
	} else if(m_align == AlignFontLeft) {
		if(digits.size() > 0)
			xPos = (int)offset.x - (((int)digits.size() * m_fontDimensions.x));
		else
			xPos = (int)offset.x - m_fontDimensions.x;
	}

	if(digits.size() == 0) {
		m_spriteHelper.NewSprite();
		m_spriteHelper.SetPosition(xPos, (int)offset.y);
		m_spriteHelper.SetFrame(0);
	}

	while(digits.size() > 0) {
		int digit = digits.top();
		digits.pop();

		m_spriteHelper.NewSprite();
		m_spriteHelper.SetPosition(xPos, (int)offset.y);
		m_spriteHelper.SetFrame(digit);

		xPos += m_fontDimensions.x;
	}
	if(!m_helperLock) m_spriteHelper.End();
}

void cg::NumberFont::RenderSpecial(int _frame) {
	m_spriteHelper.SetPosition((int)offset.x, (int)offset.y);
	m_spriteHelper.SetFrame(_frame);
}

void cg::NumberFont::SetMinDigits(int _value) {
	m_minDigits = _value;
}

int cg::NumberFont::GetMinDigits() const {
	return m_minDigits;
}

void cg::NumberFont::SetAlignment(FontAlignment _value) {
	m_align = _value;
}

/* would rather not change. see how its done.
void cg::NumberFont::SetFontAsset(int _asset) {
    numberSprite->SetImage(_asset);
    m_spriteHelper.SetImage(_asset);
}*/

int cg::NumberFont::GetDigitWidth() {
	return m_fontDimensions.x;
}
