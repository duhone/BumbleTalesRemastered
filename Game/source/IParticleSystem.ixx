/*
 *  ParticleSystem.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/27/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.IParticleSystem;

import CR.Game.IRenderable;
import CR.Engine;
import std;
import std.compat;

export namespace CR::Game {
	class IParticleSystem : public IRenderable {
	  public:
		~IParticleSystem() override = default;

		void Update() override = 0;
		void Render() override = 0;

		bool IsDead() const { return m_isDead; }

	  protected:
		IParticleSystem() = default;

		bool m_isDead{false};

		float FloatRand(float maxVal);
	};
}    // namespace CR::Game

module :private;

namespace cecore = CR::Engine::Core;

float CR::Game::IParticleSystem::FloatRand(float maxVal) {
	return cecore::Random(0.0f, maxVal);
}
