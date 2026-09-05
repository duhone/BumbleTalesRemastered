/*
 *  Timer.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/2/09.
 *
 */
export module CR.Game.Timer;

import std;
import std.compat;

export namespace CR::Game::Timer {
	void Initialize();

	void Update();
	float GetTimePassed();
}    // namespace CR::Game::Timer

module :private;

namespace cg = CR::Game;

namespace {
	std::chrono::steady_clock::time_point m_starttime{};
	float m_timePassed{};
}    // namespace

void cg::Timer::Initialize() {
	m_starttime = std::chrono::steady_clock::now();
}

void cg::Timer::Update() {
	auto currenttime = std::chrono::steady_clock::now();
	m_timePassed     = std::chrono::duration<float>(currenttime - m_starttime).count();
	m_starttime      = currenttime;
}

float cg::Timer::GetTimePassed() {
	return m_timePassed;
}