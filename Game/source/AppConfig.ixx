
export module CR.Game.AppConfig;

import std;
import std.compat;

export namespace CR::Game::AppConfig {
	enum EPlatformType { PHONE, IPAD, IAD, ANDROID };

	EPlatformType PlatformType();
	bool IsIpad();
	bool IsIAd();
	bool IsAndroid();
}    // namespace CR::Game::AppConfig

module :private;

namespace cg = CR::Game;

namespace {
	// hard coded for now.
	cg::AppConfig::EPlatformType m_platformType = cg::AppConfig::EPlatformType::IPAD;
}    // namespace

cg::AppConfig::EPlatformType cg::AppConfig::PlatformType() {
	return m_platformType;
}
bool cg::AppConfig::IsIpad() {
	return m_platformType == cg::AppConfig::EPlatformType::IPAD;
}
bool cg::AppConfig::IsIAd() {
	return m_platformType == cg::AppConfig::EPlatformType::IAD;
}
bool cg::AppConfig::IsAndroid() {
	return m_platformType == cg::AppConfig::EPlatformType::ANDROID;
}
