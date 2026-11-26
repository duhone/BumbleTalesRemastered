/*
 *  CharacterHUD.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/30/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.CharacterHUD;

import CR.Engine;

import CR.Game.CharacterBonus;
import CR.Game.CharacterLevels;
import CR.Game.CharacterStats;
import CR.Game.InputButton;
import CR.Game.IRenderable;

import std;
import std.compat;

export namespace CR::Game {
	class CharacterHUD : public InputButton, public IRenderable {
	  public:
		CharacterHUD();
		virtual ~CharacterHUD();

		// IRenderable
		void Update() override;
		void Render() override;
		void SetPosition(float xLoc, float yLoc) override;
		void PauseAnimation(bool pause) override;

		void SetCharacter(CharacterName charName, int level);
		CharacterName GetCharacter() const { return m_character; }
		CharBonus GetCharacterBonus() const;

	  private:
		CharacterBonus m_characterBonus;
		glm::vec2 offset{};
		CR::Engine::Graphics::Handles::Sprite characterSprite;
		CharacterStats m_charStats;
		CharacterName m_character{NoCharacter};
		int m_level{};
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace cecore::Literals;

cg::CharacterHUD::CharacterHUD() {
	SetSpriteAndBounds(0, 0, "StoryModeCharHUDBase"_hash64, 20);

	cegraph::Sprites::Create("StoryModeCharHUD"_hash64, characterSprite);
	cegraph::Sprites::SetZOrders(characterSprite, 30);

	SetCharacter(Mayor, 1);
}

cg::CharacterHUD::~CharacterHUD() {
	cegraph::Sprites::Delete(characterSprite);
}

void cg::CharacterHUD::Update() {
	InputButton::Update();
}

void cg::CharacterHUD::Render() {
	InputButton::Render();

	const bool hasCharacter = m_character != NoCharacter;
	cegraph::Sprites::SetVisibilities(characterSprite, hasCharacter);
	m_charStats.SetVisible(hasCharacter);
	if(hasCharacter) { m_charStats.Render(); }
}

void cg::CharacterHUD::SetPosition(float xLoc, float yLoc) {
	offset = {xLoc, yLoc};

	InputButton::SetPosition((int)xLoc, (int)yLoc);
	cegraph::Sprites::SetPositions(characterSprite, offset + glm::vec2{8, 3});
	m_charStats.SetPosition(xLoc + 28, yLoc + 106);
}

void cg::CharacterHUD::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::CharacterHUD::SetCharacter(CharacterName charName, int level) {
	if(charName < NoCharacter) { cegraph::Sprites::SetFrames(characterSprite, static_cast<uint16_t>(charName)); }
	m_character = charName;
	m_level     = level;
	m_charStats.SetBonus(m_characterBonus.GetBonus(m_character, m_level));
}

cg::CharBonus cg::CharacterHUD::GetCharacterBonus() const {
	return m_characterBonus.GetBonus(m_character, m_level);
}
