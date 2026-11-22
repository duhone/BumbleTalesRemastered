/*
 *  ParticleSystemManager.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/28/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.ParticleSystemManager;

import CR.Game.IParticleSystem;
import CR.Game.IRenderable;

import std;
import std.compat;

export namespace CR::Game {
	class ParticleSystemManager : public IRenderable {
	  public:
		ParticleSystemManager()           = default;
		~ParticleSystemManager() override = default;

		void AddParticleSystem(std::unique_ptr<IParticleSystem> pSystem);

		void Update() override;
		void Render() override;
		void SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) override {}
		void PauseAnimation(bool pause) override;

	  private:
		std::vector<std::unique_ptr<IParticleSystem>> particleSystems;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

void cg::ParticleSystemManager::AddParticleSystem(std::unique_ptr<IParticleSystem> pSystem) {
	particleSystems.push_back(std::move(pSystem));
}

void cg::ParticleSystemManager::Update() {
	for(auto& system : particleSystems) { system->Update(); }
	std::erase_if(particleSystems, [](const std::unique_ptr<IParticleSystem>& system) { return system->IsDead(); });
}

void cg::ParticleSystemManager::Render() {
	for(auto& system : particleSystems) { system->Render(); }
}

void cg::ParticleSystemManager::PauseAnimation(bool pause) {
	for(auto& system : particleSystems) { system->PauseAnimation(pause); }
}
