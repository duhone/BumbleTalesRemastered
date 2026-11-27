/*
 *  BuildingHUD.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/2/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.BuildingHUD;

import CR.Engine;

import CR.Game.BuildingButtonControl;
import CR.Game.BuildingLevels;
import CR.Game.Event;
import CR.Game.IRenderable;
import CR.Game.ResourcePuzzle;

import std;
import std.compat;

export namespace CR::Game {
	class BuildingHUD : public IRenderable {
	  public:
		explicit BuildingHUD(ResourcePuzzle& resourcePuzzle);
		~BuildingHUD() override = default;

		BuildingHUD(const BuildingHUD&)            = delete;
		BuildingHUD& operator=(const BuildingHUD&) = delete;

		// IRenderable
		void Update() override;
		void Render() override;
		void SetPosition(float xLoc, float yLoc) override;
		void PauseAnimation(bool pause) override;

		void SetBuildingLevel(BuildingName bName, int level);
		int GetBuildingLevel(BuildingName bName) const { return m_buildingLevels[bName]; }
		void ResetBuildingDelays();

		void EnableButton(BuildingName bName);
		void OnButtonDown(int tag);
		void OnButtonUp(int tag);
		void OnButtonCancel(int tag);

		CR::Utility::Event OnPowerUsed;

		bool GetCityMasterTrophy() const;
		bool GetIHeartBumblesTrophy() const;

	  private:
		static constexpr int c_buildingCount = 6;
		static constexpr float c_baseDelayTime = 90.0f;

		std::array<BuildingButtonControl, c_buildingCount> m_buildingButtons;
		std::array<int, c_buildingCount> m_buildingLevels{};
		std::array<bool, c_buildingCount> m_buildingUsed{};
		ResourcePuzzle& m_resourcePuzzle;
	};
}    // namespace CR::Game

module :private;

namespace cecore = CR::Engine::Core;
namespace cg     = CR::Game;

using namespace cecore::Literals;

namespace {
	constexpr std::array<uint64_t, 6> c_buildingSprites{"StoryModeBuildingHUDAnim1"_hash64,
														"StoryModeBuildingHUDAnim2"_hash64,
														"StoryModeBuildingHUDAnim3"_hash64,
														"StoryModeBuildingHUDAnim4"_hash64,
														"StoryModeBuildingHUDAnim5"_hash64,
														"StoryModeBuildingHUDAnim6"_hash64};
}    // namespace

cg::BuildingHUD::BuildingHUD(ResourcePuzzle& resourcePuzzle) : m_resourcePuzzle(resourcePuzzle) {
	for(int i = 0; i < c_buildingCount; ++i) {
		BuildingButtonControl& button = m_buildingButtons[i];

		button.SetTag(i);
		button.SetSprite(c_buildingSprites[i]);
		button.SetDelayTime(c_baseDelayTime);
		button.StartCountDown();
		button.OnDown += CR::Utility::Delegate1<BuildingHUD, int>(this, &BuildingHUD::OnButtonDown);
		button.OnUp += CR::Utility::Delegate1<BuildingHUD, int>(this, &BuildingHUD::OnButtonUp);
		button.OnCancel += CR::Utility::Delegate1<BuildingHUD, int>(this, &BuildingHUD::OnButtonCancel);

		// Buildings that have not been built are hidden and do not take input.
		button.SetVisible(false);
		button.Disabled(true);
	}
}

void cg::BuildingHUD::Update() {
	for(BuildingButtonControl& button : m_buildingButtons) { button.Update(); }
}

void cg::BuildingHUD::Render() {
	for(int i = 0; i < c_buildingCount; ++i) {
		m_buildingButtons[i].SetVisible(m_buildingLevels[i] > 0);
		m_buildingButtons[i].Render();
	}
}

void cg::BuildingHUD::SetPosition(float xLoc, float yLoc) {
	const int x = (int)xLoc;
	const int y = (int)yLoc;

	m_buildingButtons[TownHall].SetPosition(x - 8, y - 8);
	m_buildingButtons[PoliceStation].SetPosition(x + 56, y + 70);
	m_buildingButtons[Firehouse].SetPosition(x + 140, y - 10);
	m_buildingButtons[Lodge].SetPosition(x + 200, y + 66);
	m_buildingButtons[Theatre].SetPosition(x + 260, y - 6);
	m_buildingButtons[Greenhouse].SetPosition(x + 334, y + 40);
}

void cg::BuildingHUD::PauseAnimation(bool pause) {
	for(BuildingButtonControl& button : m_buildingButtons) {
		button.CancelIfDown();
		button.PauseAnimation(pause);
	}

	m_resourcePuzzle.CancelBuildingSpecial();
}

void cg::BuildingHUD::SetBuildingLevel(BuildingName bName, int level) {
	int& currentLevel = m_buildingLevels[bName];
	if(currentLevel == level) return;

	BuildingButtonControl& button = m_buildingButtons[bName];
	if(currentLevel == 0) {
		button.SetVisible(true);
		button.ShowBuildAnimation();
	}

	currentLevel = level;
	button.Disabled(level <= 0);
	button.SetVisible(level > 0);

	if(level <= 1)
		button.SetDelayTime(c_baseDelayTime);
	else if(level <= 3)
		button.SetDelayTime(c_baseDelayTime / 1.5f);
	else if(level <= 5)
		button.SetDelayTime(c_baseDelayTime / 2.0f);
}

void cg::BuildingHUD::ResetBuildingDelays() {
	for(int i = 0; i < c_buildingCount; ++i) {
		if(m_buildingLevels[i] > 0) { m_buildingButtons[i].StartCountDown(); }
	}
}

void cg::BuildingHUD::EnableButton(BuildingName bName) {
	m_buildingButtons[bName].Activate();
}

void cg::BuildingHUD::OnButtonDown(int tag) {
	if(m_resourcePuzzle.IsPuzzleAnimating()) {
		m_buildingButtons[tag].Activate();
		return;
	}

	m_resourcePuzzle.SetBuildingSpecialDown((BuildingName)tag, m_buildingLevels[tag]);
}

void cg::BuildingHUD::OnButtonUp(int tag) {
	m_resourcePuzzle.SetBuildingSpecialUp((BuildingName)tag, m_buildingLevels[tag]);
	m_buildingUsed[tag] = true;
	OnPowerUsed();
}

void cg::BuildingHUD::OnButtonCancel([[maybe_unused]] int tag) {
	m_resourcePuzzle.CancelBuildingSpecial();
}

bool cg::BuildingHUD::GetCityMasterTrophy() const {
	return std::ranges::all_of(m_buildingLevels, [](int level) { return level >= 5; });
}

bool cg::BuildingHUD::GetIHeartBumblesTrophy() const {
	return std::ranges::all_of(m_buildingUsed, [](bool used) { return used; });
}
