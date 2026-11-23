/*
 *  ChainEffect.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/29/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.ChainEffect;

import CR.Engine;

import CR.Game.IRenderable;
import CR.Game.Timer;

import std;
import std.compat;

export namespace CR::Game {
	class ChainEffect : public IRenderable {
	  public:
		ChainEffect(int amount, float xLoc, float yLoc);
		~ChainEffect() override;

		ChainEffect(const ChainEffect&)            = delete;
		ChainEffect& operator=(const ChainEffect&) = delete;
		ChainEffect(ChainEffect&& other) noexcept;
		ChainEffect& operator=(ChainEffect&& other) noexcept;

		void Update() override;
		void Render() override;
		void SetPosition(float xLoc, float yLoc) override;
		void PauseAnimation(bool pause) override;
		bool IsDead() const;

	  private:
		glm::vec2 m_offset{};
		CR::Engine::Graphics::Handles::Sprite m_chainSprite;
		float m_timeLeft{};
		bool m_isDead{};
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace cecore::Literals;

namespace {
	constexpr uint8_t c_chainZ      = 21;
	constexpr int c_maxChainFrame   = 20;
	constexpr float c_lifeTime      = 2.0f;
	constexpr float c_fadeTime      = 1.0f;
	constexpr float c_riseSpeed     = 20.0f;
}    // namespace

cg::ChainEffect::ChainEffect(int amount, float xLoc, float yLoc) {
	cegraph::Sprites::Create("StoryModeChainMatchTags"_hash64, m_chainSprite);
	cegraph::Sprites::SetZOrders(m_chainSprite, c_chainZ);
	cegraph::Sprites::SetRotations(m_chainSprite, 0.0f);

	int frame = std::clamp(amount - 2, 0, c_maxChainFrame);
	cegraph::Sprites::SetFrames(m_chainSprite, static_cast<uint16_t>(frame));

	SetPosition(xLoc, yLoc);
	m_timeLeft = c_lifeTime;
}

cg::ChainEffect::~ChainEffect() {
	if(m_chainSprite.isValid()) { cegraph::Sprites::Delete(m_chainSprite); }
}

cg::ChainEffect::ChainEffect(ChainEffect&& other) noexcept :
	IRenderable(other), m_offset(other.m_offset), m_chainSprite(std::exchange(other.m_chainSprite, {})),
	m_timeLeft(other.m_timeLeft), m_isDead(other.m_isDead) {}

cg::ChainEffect& cg::ChainEffect::operator=(ChainEffect&& other) noexcept {
	if(this != &other) {
		if(m_chainSprite.isValid()) { cegraph::Sprites::Delete(m_chainSprite); }
		m_offset      = other.m_offset;
		m_chainSprite = std::exchange(other.m_chainSprite, {});
		m_timeLeft    = other.m_timeLeft;
		m_isDead      = other.m_isDead;
	}
	return *this;
}

void cg::ChainEffect::Update() {
	float timePassed = Timer::GetTimePassed();

	m_timeLeft -= timePassed;

	if(m_timeLeft <= 0) { m_isDead = true; }

	SetPosition(m_offset.x, m_offset.y - (c_riseSpeed * timePassed));

	if(m_timeLeft <= c_fadeTime) {
		float alpha = std::clamp(m_timeLeft / c_fadeTime, 0.0f, 1.0f);
		cegraph::Sprites::SetColors(m_chainSprite, glm::u8vec4{255, 255, 255, static_cast<uint8_t>(255 * alpha)});
	}
}

void cg::ChainEffect::Render() {
	cegraph::Sprites::SetVisibilities(m_chainSprite, !m_isDead);
}

void cg::ChainEffect::SetPosition(float xLoc, float yLoc) {
	m_offset = glm::vec2{xLoc, yLoc};

	glm::uvec2 size;
	cegraph::Sprites::GetSizes(m_chainSprite, size);
	cegraph::Sprites::SetPositions(m_chainSprite, m_offset - glm::vec2{size} / 2.0f);
}

void cg::ChainEffect::PauseAnimation([[maybe_unused]] bool pause) {}

bool cg::ChainEffect::IsDead() const {
	return m_isDead;
}
