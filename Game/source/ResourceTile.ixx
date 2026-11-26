/*
 *  ResourceTile.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.ResourceTile;

import CR.Engine;

import CR.Game.Constants;
import CR.Game.IRenderable;
import CR.Game.Timer;

import std;
import std.compat;

export namespace CR::Game {
	class ResourceTile : public IRenderable {
	  public:
		ResourceTile(int xLoc, int yLoc, float tileSpacing, glm::vec2 puzzleOffset);
		virtual ~ResourceTile();

		// IRenderable
		void Update() override;
		void Render() override;
		void SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) override {}
		void PauseAnimation(bool pause) override;

		void AnimateSlideIn(glm::ivec2 fromTile, glm::ivec2 toTile, float timeSpan);
		void AnimateSlideTo(glm::ivec2 toTile, float timeSpan);
		void AnimateShakeTo(Constants::AnimationDirection direction, float timeSpan);
		void AnimateSlideToPoint(glm::vec2 toPoint, float timeSpan);
		void AnimateHint();
		void StopAnimateHint();

		bool TilePositionUpToDate() const { return animationStatus == Constants::AnimationOff; }
		Constants::ResourceType GetResourceType() const { return resourceType; }
		void RandomizeType(Constants::ResourceType dropRateBonusType);
		glm::vec2 GetAbsolutePosition() const { return position; }

		void SetVisible(bool visible) { m_visible = visible; }

		// Selector is hidden by Render(); call these after Render() to show it this frame, synced to the
		// SpecialSelector's own Selector2/Selector3 sprite.
		void RenderSelectorSpecial(CR::Engine::Graphics::Handles::Sprite specialSelector);
		void RenderSelectorExplode(CR::Engine::Graphics::Handles::Sprite specialSelector);

	  private:
		glm::vec2 TileCenter(glm::ivec2 tile) const;
		void SetCenter(glm::vec2 center);
		void UpdateHint();
		void ShowSelector(uint64_t spriteHash, CR::Engine::Graphics::Handles::Sprite source);

		float m_tileSpacing{};
		glm::vec2 m_puzzleOffset{};

		Constants::ResourceType resourceType{Constants::Leaf};
		glm::vec2 position{};
		bool m_visible{true};

		// Tile Movement Animation
		Constants::AnimationStatus animationStatus{Constants::AnimationOff};
		glm::vec2 finalAnimPoint{};
		glm::vec2 animDelta{};
		float animTotalTime{};
		float animTimeLeft{};

		bool shakeToAnim{false};
		glm::vec2 shakeToStartPoint{};

		// Hint spin animation
		bool m_hintAnimating{false};
		bool m_hintPaused{false};
		float m_hintTime{};

		CR::Engine::Graphics::Handles::Sprite m_tileSprite;
		CR::Engine::Graphics::Handles::Sprite selectorSprite;
		uint64_t selectorSpriteHash{};
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace cecore::Literals;

namespace {
	constexpr uint8_t c_tileZ     = 5;
	constexpr uint8_t c_selectorZ = 20;

	constexpr glm::vec2 c_tileSize{cg::Constants::c_resourceTileWidth, cg::Constants::c_resourceTileHeight};

	// Hint spin measured from the legacy tiles3.png animation, one entry per frame at its 20fps playback.
	// Clockwise degrees: a small wind-up backwards, a fast forward spin, a slight overshoot, then settle upright.
	constexpr float c_hintFrameRate = 20.0f;
	constexpr std::array c_hintSpinDegrees{0.0f,   -13.0f, -18.5f, 9.0f,   39.0f,  69.5f,  99.5f,  130.0f, 160.0f,
										   190.5f, 221.0f, 251.0f, 281.0f, 311.5f, 341.5f, 367.0f, 360.0f};
}    // namespace

cg::ResourceTile::ResourceTile(int xLoc, int yLoc, float tileSpacing, glm::vec2 puzzleOffset) :
	m_tileSpacing(tileSpacing), m_puzzleOffset(puzzleOffset) {
	cegraph::Sprites::Create("Piecestiles"_hash64, m_tileSprite);
	cegraph::Sprites::SetZOrders(m_tileSprite, c_tileZ);
	cegraph::Sprites::SetRotations(m_tileSprite, 0.0f);
	RandomizeType(Constants::NoTile);

	SetCenter(TileCenter({xLoc, yLoc}));
}

cg::ResourceTile::~ResourceTile() {
	if(m_tileSprite.isValid()) { cegraph::Sprites::Delete(m_tileSprite); }
	if(selectorSprite.isValid()) { cegraph::Sprites::Delete(selectorSprite); }
}

void cg::ResourceTile::Update() {
	if(animationStatus == Constants::AnimationOn) {
		animTimeLeft -= Timer::GetTimePassed();

		if(animTimeLeft <= 0) {
			SetCenter(finalAnimPoint);
			animationStatus = Constants::AnimationOff;

			if(shakeToAnim) {
				shakeToAnim = false;
				AnimateSlideToPoint(shakeToStartPoint, animTotalTime);
			}
		} else {
			SetCenter(finalAnimPoint - animDelta * (animTimeLeft / animTotalTime));
		}
	}

	UpdateHint();
}

void cg::ResourceTile::Render() {
	cegraph::Sprites::SetVisibilities(m_tileSprite, m_visible);
	if(selectorSprite.isValid()) { cegraph::Sprites::SetVisibilities(selectorSprite, false); }
}

void cg::ResourceTile::PauseAnimation(bool pause) {
	m_hintPaused = pause;
}

void cg::ResourceTile::AnimateSlideTo(glm::ivec2 toTile, float timeSpan) {
	AnimateSlideToPoint(TileCenter(toTile), timeSpan);
}

void cg::ResourceTile::AnimateSlideIn(glm::ivec2 fromTile, glm::ivec2 toTile, float timeSpan) {
	SetCenter(TileCenter(fromTile));
	AnimateSlideTo(toTile, timeSpan);
}

void cg::ResourceTile::AnimateShakeTo(Constants::AnimationDirection direction, float timeSpan) {
	if(animationStatus == Constants::AnimationOn) return;

	StopAnimateHint();

	shakeToStartPoint = position;

	glm::vec2 shakeDistance = c_tileSize / 2.0f + m_tileSpacing;
	glm::vec2 newPosition   = position;
	switch(direction) {
		case Constants::AnimateUp:
			newPosition.y -= shakeDistance.y;
			break;
		case Constants::AnimateDown:
			newPosition.y += shakeDistance.y;
			break;
		case Constants::AnimateLeft:
			newPosition.x -= shakeDistance.x;
			break;
		case Constants::AnimateRight:
			newPosition.x += shakeDistance.x;
			break;
	}

	shakeToAnim = true;
	AnimateSlideToPoint(newPosition, timeSpan / 2.0f);
}

void cg::ResourceTile::AnimateSlideToPoint(glm::vec2 toPoint, float timeSpan) {
	if(animationStatus == Constants::AnimationOn) return;

	StopAnimateHint();

	animTotalTime  = timeSpan;
	animTimeLeft   = animTotalTime;
	finalAnimPoint = toPoint;
	animDelta      = toPoint - position;

	animationStatus = Constants::AnimationOn;
}

void cg::ResourceTile::AnimateHint() {
	m_hintAnimating = true;
	m_hintTime      = 0.0f;
}

void cg::ResourceTile::StopAnimateHint() {
	m_hintAnimating = false;
	cegraph::Sprites::SetRotations(m_tileSprite, 0.0f);
}

void cg::ResourceTile::UpdateHint() {
	if(!m_hintAnimating || m_hintPaused) return;

	m_hintTime += Timer::GetTimePassed();
	float frame = m_hintTime * c_hintFrameRate;
	auto index  = (std::size_t)frame;
	if(index + 1 >= c_hintSpinDegrees.size()) {
		StopAnimateHint();
		return;
	}

	float degrees = std::lerp(c_hintSpinDegrees[index], c_hintSpinDegrees[index + 1], frame - (float)index);
	// engine rotation is counter-clockwise
	cegraph::Sprites::SetRotations(m_tileSprite, -glm::radians(degrees));
}

void cg::ResourceTile::RandomizeType(Constants::ResourceType dropRateBonusType) {
	std::array<int, Constants::c_resourceTileTypes> dropRates{16, 32, 48, 64, 80, 96, 98, 100};
	if(dropRateBonusType != Constants::NoTile) {
		int pCount = 0;
		for(int i = 0; i < 6; ++i) {
			pCount += (i == dropRateBonusType) ? 21 : 15;
			dropRates[i] = pCount;
		}
	}

	int dropPercent = cecore::Random(0, 100);
	for(int i = 0; i < Constants::c_resourceTileTypes; ++i) {
		if(dropPercent <= dropRates[i]) {
			resourceType = (Constants::ResourceType)i;
			cegraph::Sprites::SetFrames(m_tileSprite, (uint16_t)resourceType);
			break;
		}
	}
}

void cg::ResourceTile::RenderSelectorSpecial(cegraph::Handles::Sprite specialSelector) {
	ShowSelector("PiecesSelector2"_hash64, specialSelector);
}

void cg::ResourceTile::RenderSelectorExplode(cegraph::Handles::Sprite specialSelector) {
	ShowSelector("PiecesSelector3"_hash64, specialSelector);
}

glm::vec2 cg::ResourceTile::TileCenter(glm::ivec2 tile) const {
	return glm::vec2{tile} * (c_tileSize + m_tileSpacing) + c_tileSize / 2.0f + m_puzzleOffset;
}

void cg::ResourceTile::SetCenter(glm::vec2 center) {
	position = center;

	glm::uvec2 size;
	cegraph::Sprites::GetSizes(m_tileSprite, size);
	cegraph::Sprites::SetPositions(m_tileSprite, position - glm::vec2{size} / 2.0f);
}

void cg::ResourceTile::ShowSelector(uint64_t spriteHash, cegraph::Handles::Sprite source) {
	if(selectorSpriteHash != spriteHash) {
		if(selectorSprite.isValid()) { cegraph::Sprites::Delete(selectorSprite); }
		cegraph::Sprites::Create(spriteHash, selectorSprite);
		cegraph::Sprites::SetZOrders(selectorSprite, c_selectorZ);
		cegraph::Sprites::SetRotations(selectorSprite, 0.0f);
		selectorSpriteHash = spriteHash;
	}

	glm::uvec2 size;
	cegraph::Sprites::GetSizes(selectorSprite, size);
	cegraph::Sprites::SetPositions(selectorSprite, position - glm::vec2{size} / 2.0f);
	cegraph::Sprites::SyncAnimations(source, selectorSprite);
	cegraph::Sprites::SetVisibilities(selectorSprite, true);
}
