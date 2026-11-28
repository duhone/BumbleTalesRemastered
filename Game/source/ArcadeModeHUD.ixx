/*
 *  ArcadeModeHUD.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/2/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.ArcadeModeHUD;

import CR.Engine;

import CR.Game.Constants;
import CR.Game.Event;
import CR.Game.IRenderable;
import CR.Game.NumberFont;
import CR.Game.ResourceBar;
import CR.Game.Timer;
import CR.Game.TimerBar;

import std;
import std.compat;

export namespace CR::Game {
	class ArcadeModeHUD : public IRenderable {
	  public:
		// xLoc/yLoc is the center of the HUD
		ArcadeModeHUD(int xLoc, int yLoc);
		virtual ~ArcadeModeHUD();

		// IRenderable
		void Update() override;
		void Render() override;
		void SetPosition(float xLoc, float yLoc) override;
		void PauseAnimation(bool pause) override;

		void SetLevel(int level);
		int GetLevel() const { return m_currLevel; }

		void SetScore(int score);
		int GetScore() const { return m_score; }
		void AddToScore(int _value) { m_score += _value; }

		void AddResources(Constants::ResourceType rType, int amount);
		bool ResourcesGathered();
		void PerformLevelUp();

		// TimerBar
		void OnTimerExpired();
		Utility::Event TimerExpired;
		float GetCurrTime() const { return timerBar.GetCurrTime(); }

		void Reset();

	  private:
		static constexpr int resource_bars = 6;

		glm::vec2 offset{};
		CR::Engine::Graphics::Handles::Sprite hudSprite;
		std::array<ResourceBar, resource_bars> resourceBars;
		NumberFont numberFont;
		NumberFont scoreFont;
		TimerBar timerBar;
		int m_currLevel{1};
		int m_score{};

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
	constexpr glm::vec2 c_hudHalfSize{253.0f, 60.0f};
	constexpr glm::vec2 c_resourceBarHalfSize{13.0f, 7.0f};
	constexpr glm::vec2 c_levelFontHalfSize{11.0f, 15.0f};
	constexpr glm::vec2 c_scoreFontHalfSize{16.0f, 18.0f};

	// Centers relative to the HUD center.
	constexpr glm::vec2 c_timerBarOffset{-216.0f, 0.0f};
	constexpr glm::vec2 c_levelFontOffset{-142.0f, -34.0f};
	constexpr glm::vec2 c_scoreFontOffset{-80.0f, 30.0f};

	// Bar centers relative to the HUD center, indexed by ResourceType.
	constexpr std::array<glm::vec2, 6> c_resourceBarOffsets{glm::vec2{170.0f, -32.0f},    // Leaf
															glm::vec2{-64.0f, -32.0f},    // Brick
															glm::vec2{54.0f, -32.0f},     // Wood
															glm::vec2{112.0f, -32.0f},    // Crystal
															glm::vec2{228.0f, -32.0f},    // Diamond
															glm::vec2{-6.0f, -32.0f}};    // Gold

	constexpr int c_baseResourceRequirement = 3;
	constexpr int c_baseTime                = 30;
	constexpr int c_minTime                 = 10;
}    // namespace

cg::ArcadeModeHUD::ArcadeModeHUD(int xLoc, int yLoc) :
	resourceBars{{{0, 0, TinyResourceBar},
				  {0, 0, TinyResourceBar},
				  {0, 0, TinyResourceBar},
				  {0, 0, TinyResourceBar},
				  {0, 0, TinyResourceBar},
				  {0, 0, TinyResourceBar}}},
	numberFont("ArcadeModeFont_Level2"_hash64, 20), scoreFont("ArcadeModeFont_HUDScore"_hash64, 20), timerBar(0, 0) {
	cegraph::Sprites::Create("ArcadeModeHUD"_hash64, hudSprite);
	cegraph::Sprites::SetZOrders(hudSprite, 10);

	timerBar.TimerExpired += CR::Utility::Delegate(this, &ArcadeModeHUD::OnTimerExpired);

	scoreFont.SetAlignment(AlignFontRight);

	SetPosition((float)xLoc, (float)yLoc);
}

cg::ArcadeModeHUD::~ArcadeModeHUD() {
	if(hudSprite.isValid()) { cegraph::Sprites::Delete(hudSprite); }
}

void cg::ArcadeModeHUD::Update() {
	timerBar.Update();

	for(auto& bar : resourceBars) { bar.Update(); }

	timerBar.DecreaseTimer(Timer::GetTimePassed());
}

void cg::ArcadeModeHUD::Render() {
	numberFont.RenderNumber(m_currLevel);
	scoreFont.RenderNumber(m_score);
	timerBar.Render();

	for(auto& bar : resourceBars) { bar.Render(); }
}

void cg::ArcadeModeHUD::PauseAnimation(bool pause) {
	for(auto& bar : resourceBars) { bar.PauseAnimation(pause); }
}

void cg::ArcadeModeHUD::ResetResourceBars() {
	for(auto& bar : resourceBars) { bar.ResetValue(); }
}

void cg::ArcadeModeHUD::SetResourceRequirement(ResourceType rType, int amount) {
	if((int)rType > 5) return;

	resourceBars[rType].SetMaxValue(amount);
}

void cg::ArcadeModeHUD::AddResources(ResourceType rType, int amount) {
	if((int)rType > 5) return;

	timerBar.IncreaseTimer(amount * 0.3f);

	resourceBars[rType].AddToCurrValue(amount);
}

bool cg::ArcadeModeHUD::ResourcesGathered() {
	for(auto& bar : resourceBars) {
		if(!bar.IsFull()) return false;
	}

	return true;
}

void cg::ArcadeModeHUD::PerformLevelUp() {
	SetLevel(m_currLevel + 1);
	ResetResourceBars();
	timerBar.ResetTimer();
}

void cg::ArcadeModeHUD::SetPosition(float xLoc, float yLoc) {
	offset = {xLoc, yLoc};

	cegraph::Sprites::SetPositions(hudSprite, offset - c_hudHalfSize);

	const glm::vec2 timerPos = offset + c_timerBarOffset;
	timerBar.SetPosition(timerPos.x, timerPos.y);

	for(size_t i = 0; i < resourceBars.size(); ++i) {
		const glm::vec2 topLeft = offset + c_resourceBarOffsets[i] - c_resourceBarHalfSize;
		resourceBars[i].SetPosition(topLeft.x, topLeft.y);
	}

	const glm::vec2 levelPos = offset + c_levelFontOffset - c_levelFontHalfSize;
	numberFont.SetPosition(levelPos.x, levelPos.y);

	const glm::vec2 scorePos = offset + c_scoreFontOffset - c_scoreFontHalfSize;
	scoreFont.SetPosition(scorePos.x, scorePos.y);
}

void cg::ArcadeModeHUD::SetScore(int score) {
	m_score = score;
}

void cg::ArcadeModeHUD::SetLevel(int level) {
	m_currLevel = level;

	for(int i = 0; i < 6; i++) {
		SetResourceRequirement((ResourceType)i, c_baseResourceRequirement + level - 1);
	}

	// Speed up the timer based on the level
	int baseTime = c_baseTime;
	for(int i = 0; i < level; i++) { baseTime = (int)(baseTime * 0.98); }

	if(baseTime < c_minTime) baseTime = c_minTime;

	timerBar.SetMaxTime((float)baseTime);
}

void cg::ArcadeModeHUD::OnTimerExpired() {
	TimerExpired();
}

void cg::ArcadeModeHUD::Reset() {
	timerBar.ResetTimer();
	m_currLevel = 1;
	SetLevel(m_currLevel);
	m_score = 0;

	ResetResourceBars();
}
