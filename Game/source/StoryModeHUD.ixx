/*
 *  StoryModeHUD.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/13/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.StoryModeHUD;

import CR.Engine;

import CR.Game.Constants;
import CR.Game.IRenderable;
import CR.Game.NumberFont;
import CR.Game.ResourceBar;

import std;
import std.compat;

export namespace CR::Game {
	class StoryModeHUD : public IRenderable {
	  public:
		// xLoc/yLoc is the center of the HUD
		StoryModeHUD(int xLoc, int yLoc);
		virtual ~StoryModeHUD();

		// IRenderable
		void Update() override;
		void Render() override;
		void SetPosition(float xLoc, float yLoc) override;
		void PauseAnimation(bool pause) override;

		void SetLevel(int level);
		int GetLevel() const { return m_currLevel; }

		void AddResources(Constants::ResourceType rType, int amount);
		bool ResourcesGathered();
		void PerformLevelUp();

		int GetResourceRequirement() const;

	  private:
		static constexpr int resource_bars = 6;

		glm::vec2 offset{};
		CR::Engine::Graphics::Handles::Sprite hudSprite;
		std::array<ResourceBar, resource_bars> resourceBars;
		NumberFont numberFont;
		int m_currLevel{};

		void SetResourceRequirement(Constants::ResourceType rType, int amount);
		void ResetResourceBars();
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace cecore::Literals;
using namespace cg::Constants;

namespace {
	constexpr glm::vec2 c_hudHalfSize{202.0f, 55.0f};
	constexpr glm::vec2 c_resourceBarHalfSize{50.0f, 10.0f};
	constexpr glm::vec2 c_levelFontHalfSize{15.0f, 18.0f};
	constexpr glm::vec2 c_levelFontOffset{-122.0f, 20.0f};

	// Bar centers relative to the HUD center, indexed by ResourceType.
	constexpr std::array<glm::vec2, 6> c_resourceBarOffsets{glm::vec2{6.0f, 34.0f},       // Leaf
															glm::vec2{6.0f, -38.0f},      // Brick
															glm::vec2{6.0f, -2.0f},       // Wood
															glm::vec2{142.0f, -38.0f},    // Crystal
															glm::vec2{142.0f, 34.0f},     // Diamond
															glm::vec2{142.0f, -2.0f}};    // Gold

	constexpr int c_baseResourceRequirement = 3;
}    // namespace

cg::StoryModeHUD::StoryModeHUD(int xLoc, int yLoc) :
	resourceBars{{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}, numberFont("StoryModeFont_Level"_hash64, 20) {
	cegraph::Sprites::Create("StoryModeHUD"_hash64, hudSprite);
	cegraph::Sprites::SetZOrders(hudSprite, 10);

	SetPosition((float)xLoc, (float)yLoc);
}

cg::StoryModeHUD::~StoryModeHUD() {
	if(hudSprite.isValid()) { cegraph::Sprites::Delete(hudSprite); }
}

void cg::StoryModeHUD::Update() {
	for(auto& bar : resourceBars) { bar.Update(); }
}

void cg::StoryModeHUD::Render() {
	numberFont.RenderNumber(m_currLevel);

	for(auto& bar : resourceBars) { bar.Render(); }
}

void cg::StoryModeHUD::PauseAnimation(bool pause) {
	for(auto& bar : resourceBars) { bar.PauseAnimation(pause); }
}

void cg::StoryModeHUD::ResetResourceBars() {
	for(auto& bar : resourceBars) { bar.ResetValue(); }
}

void cg::StoryModeHUD::SetResourceRequirement(ResourceType rType, int amount) {
	if((int)rType > 5) return;

	resourceBars[rType].SetMaxValue(amount);
}

void cg::StoryModeHUD::AddResources(ResourceType rType, int amount) {
	if((int)rType > 5) return;

	resourceBars[rType].AddToCurrValue(amount);
}

bool cg::StoryModeHUD::ResourcesGathered() {
	for(auto& bar : resourceBars) {
		if(!bar.IsFull()) return false;
	}

	return true;
}

void cg::StoryModeHUD::PerformLevelUp() {
	SetLevel(m_currLevel + 1);
	ResetResourceBars();
}

void cg::StoryModeHUD::SetPosition(float xLoc, float yLoc) {
	offset = {xLoc, yLoc};

	cegraph::Sprites::SetPositions(hudSprite, offset - c_hudHalfSize);

	for(size_t i = 0; i < resourceBars.size(); ++i) {
		const glm::vec2 topLeft = offset + c_resourceBarOffsets[i] - c_resourceBarHalfSize;
		resourceBars[i].SetPosition(topLeft.x, topLeft.y);
	}

	const glm::vec2 fontPos = offset + c_levelFontOffset - c_levelFontHalfSize;
	numberFont.SetPosition(fontPos.x, fontPos.y);
}

void cg::StoryModeHUD::SetLevel(int level) {
	m_currLevel = level;

	for(int i = 0; i < 6; i++) {
		SetResourceRequirement((ResourceType)i, c_baseResourceRequirement + level - 1);
	}
}

int cg::StoryModeHUD::GetResourceRequirement() const {
	return c_baseResourceRequirement + m_currLevel - 1;
}
