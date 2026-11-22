/*
 *  CrystalParticle.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/27/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.CrystalParticle;

import CR.Engine;

import CR.Game.Constants;
import CR.Game.IParticle;
import CR.Game.Timer;

import std;
import std.compat;

export namespace CR::Game {
	enum CrystalParticleStyle { CPStyleLeft, CPStyleRight };

	class CrystalParticle : public IParticle {
	  public:
		CrystalParticle(glm::vec2 position, CrystalParticleStyle cpStyle, Constants::ResourceType rType);
		~CrystalParticle() override;

		CrystalParticle(const CrystalParticle&)            = delete;
		CrystalParticle& operator=(const CrystalParticle&) = delete;
		CrystalParticle(CrystalParticle&& other) noexcept;
		CrystalParticle& operator=(CrystalParticle&& other) noexcept;

		void Update() override;
		void Render() override;
		void SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) override {}
		void PauseAnimation(bool pause) override;

	  private:
		CR::Engine::Graphics::Handles::Sprite m_sprite;
		glm::vec2 m_velocity{};
		glm::vec2 m_acceleration{};
		glm::vec2 m_position{};

		float maxTimeLeft{};
		float timeLeft{};
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace cecore::Literals;

namespace {
	constexpr float c_animMod    = 20.0f;
	constexpr float c_fadeTime   = 0.5f;
	constexpr uint8_t c_particleZ = 40;

	// Legacy velocities and accelerations are per-frame pixel values at 320x480.
	constexpr float c_scale = 2.0f;

	uint64_t ShatterSpriteHash(cg::Constants::ResourceType rType) {
		switch(rType) {
			case cg::Constants::Leaf:
				return "PiecespieceShatter3"_hash64;
			case cg::Constants::Brick:
				return "PiecespieceShatter1"_hash64;
			case cg::Constants::Wood:
				return "PiecespieceShatter2"_hash64;
			case cg::Constants::Crystal:
				return "PiecespieceShatter4"_hash64;
			case cg::Constants::Diamond:
				return "PiecespieceShatter6"_hash64;
			case cg::Constants::Gold:
				return "PiecespieceShatter5"_hash64;
			default:
				return "PiecespieceShatter0"_hash64;
		}
	}
}    // namespace

cg::CrystalParticle::CrystalParticle(glm::vec2 position, CrystalParticleStyle cpStyle,
									 Constants::ResourceType rType) :
	m_position(position) {
	cegraph::Sprites::Create(ShatterSpriteHash(rType), m_sprite);
	cegraph::Sprites::SetZOrders(m_sprite, c_particleZ);
	cegraph::Sprites::SetRotations(m_sprite, 0.0f);

	maxTimeLeft = 1.0f + FloatRand(1.0f);
	timeLeft    = maxTimeLeft;

	float xMod = (cpStyle == CPStyleLeft) ? -1.0f : 1.0f;

	float yVel = -4.0f;
	float xAcc = FloatRand(0.4f) + 0.1f;
	float yAcc = FloatRand(0.8f) + 0.6f;

	m_velocity     = glm::vec2{0.0f, yVel} * c_scale;
	m_acceleration = glm::vec2{xAcc * xMod, yAcc} * c_scale;

	m_isDead = false;
}

cg::CrystalParticle::~CrystalParticle() {
	if(m_sprite.isValid()) { cegraph::Sprites::Delete(m_sprite); }
}

cg::CrystalParticle::CrystalParticle(CrystalParticle&& other) noexcept :
	IParticle(other), m_sprite(std::exchange(other.m_sprite, {})), m_velocity(other.m_velocity),
	m_acceleration(other.m_acceleration), m_position(other.m_position), maxTimeLeft(other.maxTimeLeft),
	timeLeft(other.timeLeft) {}

cg::CrystalParticle& cg::CrystalParticle::operator=(CrystalParticle&& other) noexcept {
	if(this != &other) {
		if(m_sprite.isValid()) { cegraph::Sprites::Delete(m_sprite); }
		IParticle::operator=(other);
		m_sprite       = std::exchange(other.m_sprite, {});
		m_velocity     = other.m_velocity;
		m_acceleration = other.m_acceleration;
		m_position     = other.m_position;
		maxTimeLeft    = other.maxTimeLeft;
		timeLeft       = other.timeLeft;
	}
	return *this;
}

void cg::CrystalParticle::Update() {
	float timePassed = Timer::GetTimePassed();
	timeLeft -= timePassed;

	if(timeLeft <= 0) {
		m_isDead = true;
		return;
	}

	if(timeLeft <= c_fadeTime) {
		cegraph::Sprites::SetColors(m_sprite, glm::u8vec4{255, 255, 255, (uint8_t)(255 * (timeLeft / c_fadeTime))});
	}

	glm::uvec2 size;
	cegraph::Sprites::GetSizes(m_sprite, size);
	cegraph::Sprites::SetPositions(m_sprite, m_position - glm::vec2{size} / 2.0f);

	m_velocity += m_acceleration * (timePassed * c_animMod);
	m_position += m_velocity;
}

void cg::CrystalParticle::Render() {
	cegraph::Sprites::SetVisibilities(m_sprite, !m_isDead);
}

void cg::CrystalParticle::PauseAnimation(bool pause) {
	cegraph::Sprites::SetAnimationPauses(m_sprite, pause);
}
