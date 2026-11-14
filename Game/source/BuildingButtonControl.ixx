/*
 *  BuildingButtonControl.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/1/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.BuildingButtonControl;

import CR.Game.Event;
import CR.Game.IInputObject;
import CR.Game.Timer;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class BuildingButtonControl : public IInputObject {
	  public:
		BuildingButtonControl();
		virtual ~BuildingButtonControl();

		// IInputObject
		void Update() override;
		void Render() override;
		void Reset() override;
		void FreeResources() override {}

		void PauseAnimation(bool pause);

		void SetDelayTime(float _value);
		void StartCountDown();

		void SetSprite(uint64_t spriteHash);
		void SetPosition(int xLoc, int yLoc);
		void SetVisible(bool visible) { m_visible = visible; }

		void SetTag(int _value) { m_tag = _value; }
		int GetTag() const { return m_tag; }

		void ShowBuildAnimation();
		void CancelIfDown();

		void Activate();
		CR::Utility::Event1<int> OnDown;
		CR::Utility::Event1<int> OnUp;
		CR::Utility::Event1<int> OnCancel;

	  private:
		void SetAutoAnimate(bool autoAnimate);
		void UpdateBounds();
		void UpdateInput();
		glm::vec2 GetButtonCenter() const;

		Rect bounds{};
		glm::ivec2 m_position{0, 0};
		CR::Engine::Graphics::Handles::Sprite buttonSprite;
		float m_delayTime{0.0f};
		float m_currTime{0.0f};
		bool m_active{true};
		bool m_armed{false};
		bool m_autoAnimate{true};
		bool m_visible{true};
		int m_tag{-1};

		bool m_wasPaused{false};

		// Build animation
		bool m_showBuildAnimation{false};
		CR::Engine::Graphics::Handles::Sprite buildSprite;
		float m_fadeAlpha{255.0f};

		CR::Engine::Audio::Handles::SoundFX m_readiedSound;
		CR::Engine::Audio::Handles::SoundFX m_notChargedSound;
	};
}    // namespace CR::Game

module :private;

namespace ceaudio = CR::Engine::Audio;
namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace cecore::Literals;

namespace {
	constexpr uint8_t c_buttonZ = 20;
	constexpr uint8_t c_buildZ  = 30;
	// legacy faded in 3 alpha per frame at 60fps.
	constexpr float c_fadeAlphaPerSecond = 3.0f * 60.0f;
}    // namespace

cg::BuildingButtonControl::BuildingButtonControl() {
	m_readiedSound    = ceaudio::SoundFX::GetHandle("building_readied_1"_hash64);
	m_notChargedSound = ceaudio::SoundFX::GetHandle("building_notcharged_1a"_hash64);
}

cg::BuildingButtonControl::~BuildingButtonControl() {
	if(buttonSprite.isValid()) { cegraph::Sprites::Delete(buttonSprite); }
	if(buildSprite.isValid()) { cegraph::Sprites::Delete(buildSprite); }
}

void cg::BuildingButtonControl::Update() {
	if(!m_active && !Disabled()) {
		// skip the time decrease this time since the game was paused
		if(!m_wasPaused) {
			m_currTime -= Timer::GetTimePassed();

			if(m_currTime <= 0) {
				m_active = true;
				SetAutoAnimate(true);
				ceaudio::SoundFX::Play(m_readiedSound);
			}
		} else {
			m_wasPaused = false;
		}
	}

	UpdateInput();
}

void cg::BuildingButtonControl::UpdateInput() {
	switch(PollClick(bounds)) {
		case ClickEvent::Pressed:
			if(!m_active) {
				ceaudio::SoundFX::Play(m_notChargedSound);
			} else {
				m_armed = true;
				OnDown(m_tag);
			}
			break;
		case ClickEvent::Clicked:
			if(m_armed) {
				m_armed = false;
				StartCountDown();
				OnUp(m_tag);
			}
			break;
		case ClickEvent::Cancelled:
		case ClickEvent::None:
			break;
	}

	// Dragged off, or disabled while held.
	if(m_armed && !IsHeld()) {
		m_armed = false;
		OnCancel(m_tag);
	}
}

void cg::BuildingButtonControl::Render() {
	if(!buttonSprite.isValid()) { return; }

	cegraph::Sprites::SetVisibilities(buttonSprite, m_visible);
	cegraph::Sprites::SetPositions(buttonSprite, glm::vec2{m_position});

	if(m_showBuildAnimation) {
		bool animationDone{};
		cegraph::Sprites::AreAnimationsDone(buildSprite, animationDone);

		if(animationDone) {
			cegraph::Sprites::Delete(buildSprite);
			buildSprite.invalidate();
			m_showBuildAnimation = false;
			m_fadeAlpha          = 255.0f;
		} else {
			glm::uvec2 buildSize;
			cegraph::Sprites::GetSizes(buildSprite, buildSize);
			cegraph::Sprites::SetPositions(buildSprite, GetButtonCenter() - glm::vec2{buildSize} / 2.0f);
			cegraph::Sprites::SetVisibilities(buildSprite, m_visible);

			m_fadeAlpha = std::min(m_fadeAlpha + c_fadeAlphaPerSecond * Timer::GetTimePassed(), 255.0f);
		}

		cegraph::Sprites::SetColors(buttonSprite, glm::u8vec4{255, 255, 255, (uint8_t)m_fadeAlpha});
	}
}

void cg::BuildingButtonControl::Reset() {
	ResetClick();
	m_armed = false;
}

void cg::BuildingButtonControl::SetDelayTime(float _value) {
	m_delayTime = _value;

	if(m_currTime > m_delayTime) m_currTime = m_delayTime;
}

void cg::BuildingButtonControl::StartCountDown() {
	m_active   = false;
	m_currTime = m_delayTime;

	SetAutoAnimate(false);
	if(buttonSprite.isValid()) { cegraph::Sprites::SetFrames(buttonSprite, 0); }
}

void cg::BuildingButtonControl::SetSprite(uint64_t spriteHash) {
	if(buttonSprite.isValid()) { cegraph::Sprites::Delete(buttonSprite); }

	cegraph::Sprites::Create(spriteHash, buttonSprite);
	cegraph::Sprites::SetZOrders(buttonSprite, c_buttonZ);
	cegraph::Sprites::SetAnimationPauses(buttonSprite, !m_autoAnimate);
	cegraph::Sprites::SetPositions(buttonSprite, glm::vec2{m_position});
	cegraph::Sprites::SetVisibilities(buttonSprite, m_visible);

	UpdateBounds();
}

void cg::BuildingButtonControl::SetPosition(int xLoc, int yLoc) {
	m_position = {xLoc, yLoc};

	if(buttonSprite.isValid()) { cegraph::Sprites::SetPositions(buttonSprite, glm::vec2{m_position}); }

	UpdateBounds();
}

void cg::BuildingButtonControl::UpdateBounds() {
	glm::uvec2 size{0, 0};
	if(buttonSprite.isValid()) { cegraph::Sprites::GetSizes(buttonSprite, size); }

	bounds.top    = m_position.y;
	bounds.left   = m_position.x;
	bounds.bottom = (int)size.y;
	bounds.right  = (int)size.x;
}

glm::vec2 cg::BuildingButtonControl::GetButtonCenter() const {
	return glm::vec2{bounds.left, bounds.top} + glm::vec2{bounds.right, bounds.bottom} / 2.0f;
}

void cg::BuildingButtonControl::SetAutoAnimate(bool autoAnimate) {
	m_autoAnimate = autoAnimate;
	if(buttonSprite.isValid()) { cegraph::Sprites::SetAnimationPauses(buttonSprite, !m_autoAnimate); }
}

void cg::BuildingButtonControl::PauseAnimation(bool pause) {
	if(pause) {
		SetAutoAnimate(false);
		m_wasPaused = true;
	} else if(m_active) {
		SetAutoAnimate(true);
	}
}

void cg::BuildingButtonControl::ShowBuildAnimation() {
	if(buildSprite.isValid()) { cegraph::Sprites::Delete(buildSprite); }

	m_showBuildAnimation = true;
	cegraph::Sprites::Create("StoryModeDust"_hash64, buildSprite);
	cegraph::Sprites::SetZOrders(buildSprite, c_buildZ);
	cegraph::Sprites::StartAnimations(buildSprite);

	glm::uvec2 buildSize;
	cegraph::Sprites::GetSizes(buildSprite, buildSize);
	cegraph::Sprites::SetPositions(buildSprite, GetButtonCenter() - glm::vec2{buildSize} / 2.0f);
	cegraph::Sprites::SetVisibilities(buildSprite, m_visible);

	m_fadeAlpha = 0.0f;
	if(buttonSprite.isValid()) {
		cegraph::Sprites::SetColors(buttonSprite, glm::u8vec4{255, 255, 255, 0});
	}
}

void cg::BuildingButtonControl::Activate() {
	m_active = true;
	SetAutoAnimate(true);
	ResetClick();
	m_armed = false;
}

void cg::BuildingButtonControl::CancelIfDown() {
	ResetClick();
	m_armed = false;
}
