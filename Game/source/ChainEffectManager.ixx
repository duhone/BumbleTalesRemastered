/*
 *  ChainEffectManager.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/29/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.ChainEffectManager;

import CR.Game.ChainEffect;

import std;
import std.compat;

export namespace CR::Game {
	class ChainEffectManager {
	  public:
		ChainEffectManager()  = default;
		~ChainEffectManager() = default;

		ChainEffectManager(const ChainEffectManager&)            = delete;
		ChainEffectManager& operator=(const ChainEffectManager&) = delete;

		void Update();
		void Render();

		void AddChainEffect(int amount, int xLoc, int yLoc);

	  private:
		std::vector<ChainEffect> m_chainEffects;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

void cg::ChainEffectManager::Update() {
	for(auto& effect : m_chainEffects) { effect.Update(); }
	std::erase_if(m_chainEffects, [](const ChainEffect& effect) { return effect.IsDead(); });
}

void cg::ChainEffectManager::Render() {
	for(auto& effect : m_chainEffects) { effect.Render(); }
}

void cg::ChainEffectManager::AddChainEffect(int amount, int xLoc, int yLoc) {
	if(amount < 2) { return; }

	m_chainEffects.emplace_back(amount, static_cast<float>(xLoc), static_cast<float>(yLoc));
}
