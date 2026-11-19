/*
 *  Particle.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/27/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.IParticle;

import CR.Game.IRenderable;
import CR.Engine;
import std;
import std.compat;

export namespace CR::Game {
	class IParticle : public IRenderable {
	  public:
		~IParticle() override = default;

		void Render() override = 0;
		void Update() override = 0;

		bool IsDead() const { return m_isDead; }

	  protected:
		IParticle() = default;

		bool m_isDead{false};

		float FloatRand(float maxVal);
	};
}    // namespace CR::Game

module :private;

namespace cecore = CR::Engine::Core;

float CR::Game::IParticle::FloatRand(float maxVal) {
	return cecore::Random(0.0f, maxVal);
}
