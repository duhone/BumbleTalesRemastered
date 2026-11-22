/*
 *  CrystalParticleSystem.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/28/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.CrystalParticleSystem;

import CR.Engine;

import CR.Game.Constants;
import CR.Game.CrystalParticle;
import CR.Game.IParticleSystem;

import std;
import std.compat;

export namespace CR::Game {
	class CrystalParticleSystem : public IParticleSystem {
	  public:
		CrystalParticleSystem(glm::vec2 position, Constants::ResourceType rType);
		~CrystalParticleSystem() override = default;

		void Update() override;
		void Render() override;
		void SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) override {}
		void PauseAnimation(bool pause) override;

	  private:
		std::vector<CrystalParticle> particles;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

namespace {
	constexpr int c_particlesPerSide = 16;
	constexpr float c_xSpread        = 60.0f;
	constexpr float c_halfTileWidth  = cg::Constants::c_resourceTileWidth / 2.0f;
	constexpr float c_halfTileHeight = cg::Constants::c_resourceTileHeight / 2.0f;
	constexpr float c_ySpread        = 2.0f * cg::Constants::c_resourceTileHeight;
}    // namespace

cg::CrystalParticleSystem::CrystalParticleSystem(glm::vec2 position, Constants::ResourceType rType) {
	particles.reserve(2 * c_particlesPerSide);

	for(int i = 0; i < c_particlesPerSide; ++i) {
		float xMod = FloatRand(c_xSpread);
		float yMod = FloatRand(c_ySpread);
		particles.emplace_back(glm::vec2{position.x - c_halfTileWidth + xMod, position.y - c_halfTileHeight + yMod},
							   CPStyleLeft, rType);
	}

	for(int i = 0; i < c_particlesPerSide; ++i) {
		float xMod = FloatRand(c_xSpread);
		float yMod = FloatRand(c_ySpread);
		particles.emplace_back(glm::vec2{position.x + c_halfTileWidth - xMod, position.y - c_halfTileHeight + yMod},
							   CPStyleRight, rType);
	}
}

void cg::CrystalParticleSystem::Update() {
	if(particles.empty()) { m_isDead = true; }

	for(auto& particle : particles) { particle.Update(); }
	std::erase_if(particles, [](const CrystalParticle& particle) { return particle.IsDead(); });
}

void cg::CrystalParticleSystem::Render() {
	for(auto& particle : particles) { particle.Render(); }
}

void cg::CrystalParticleSystem::PauseAnimation(bool pause) {
	for(auto& particle : particles) { particle.PauseAnimation(pause); }
}
