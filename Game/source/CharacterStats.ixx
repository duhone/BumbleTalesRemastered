/*
 *  CharacterStats.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/13/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.CharacterStats;

import CR.Engine;

import CR.Game.CharacterBonus;
import CR.Game.Constants;
import CR.Game.IRenderable;
import CR.Game.RepeatedSpriteHelper;

import std;
import std.compat;

export namespace CR::Game {
	class CharacterStats : public IRenderable {
	  public:
		CharacterStats();
		virtual ~CharacterStats();

		// IRenderable
		void Update() override;
		void Render() override;
		void SetPosition(float xLoc, float yLoc) override;
		void PauseAnimation(bool pause) override;

		void SetBonus(const CharBonus& cBonus);
		void SetVisible(bool visible);

	  private:
		glm::vec2 offset{};
		CharBonus m_cBonus;

		CR::Game::RepeatedSpriteHelper m_spriteHelperStats;
		CR::Game::RepeatedSpriteHelper m_spriteHelperResource;
	};
}    // namespace CR::Game

module :private;

namespace cecore = CR::Engine::Core;
namespace cg     = CR::Game;

using namespace cecore::Literals;

namespace {
	constexpr int c_resourceWidth  = 28;
	constexpr int c_resourceHeight = 26;
	constexpr int c_statSize       = 26;
	constexpr int c_statOffsetX    = 30;
	constexpr int c_columnPitch    = 56;
	constexpr int c_rowPitch       = 30;
}    // namespace

cg::CharacterStats::CharacterStats() :
	m_spriteHelperStats("StoryModeCharHUDStat"_hash64, 30), m_spriteHelperResource("StoryModeCharHUDResource"_hash64, 30) {}

cg::CharacterStats::~CharacterStats() {}

void cg::CharacterStats::Update() {}

void cg::CharacterStats::Render() {
	int xRow = 0;
	int yRow = 0;

	const int resourceX = int(offset.x) - c_resourceWidth / 2;
	const int statX     = int(offset.x) + c_statOffsetX - c_statSize / 2;
	const int resourceY = int(offset.y) - c_resourceHeight / 2;
	const int statY     = int(offset.y) - c_statSize / 2;

	m_spriteHelperStats.Start();
	m_spriteHelperResource.Start();

	for(size_t i = 0; i < m_cBonus.resourceBonus.size(); i++) {
		if(m_cBonus.resourceBonus[i] > 0) {
			m_spriteHelperResource.NewSprite();
			m_spriteHelperResource.SetPosition(resourceX + xRow * c_columnPitch, resourceY + yRow * c_rowPitch);
			m_spriteHelperResource.SetFrame(int(i));

			m_spriteHelperStats.NewSprite();
			m_spriteHelperStats.SetPosition(statX + xRow * c_columnPitch, statY + yRow * c_rowPitch);
			m_spriteHelperStats.SetFrame(m_cBonus.resourceBonus[i] - 1);

			xRow++;
			if(xRow > 1) {
				xRow = 0;
				yRow++;
			}
		}
	}

	if(m_cBonus.dropRateBonusType != Constants::NoTile) {
		m_spriteHelperResource.NewSprite();
		m_spriteHelperResource.SetPosition(resourceX + xRow * c_columnPitch, resourceY + yRow * c_rowPitch);
		m_spriteHelperResource.SetFrame(static_cast<int>(m_cBonus.dropRateBonusType));

		m_spriteHelperStats.NewSprite();
		m_spriteHelperStats.SetPosition(statX + xRow * c_columnPitch, statY + yRow * c_rowPitch);
		m_spriteHelperStats.SetFrame(2);
	}

	m_spriteHelperStats.End();
	m_spriteHelperResource.End();
}

void cg::CharacterStats::SetPosition(float xLoc, float yLoc) {
	offset = {xLoc, yLoc};
}

void cg::CharacterStats::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::CharacterStats::SetBonus(const CharBonus& cBonus) {
	m_cBonus = cBonus;
}

void cg::CharacterStats::SetVisible(bool visible) {
	m_spriteHelperStats.SetVisible(visible);
	m_spriteHelperResource.SetVisible(visible);
}
