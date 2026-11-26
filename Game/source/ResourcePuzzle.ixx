/*
 *  ResourcePuzzle.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.ResourcePuzzle;

import CR.Engine;

import CR.Game.BuildingLevels;
import CR.Game.ChainEffectManager;
import CR.Game.Constants;
import CR.Game.CrystalParticleSystem;
import CR.Game.Event;
import CR.Game.IRenderable;
import CR.Game.ParticleSystemManager;
import CR.Game.ResourceTile;
import CR.Game.SpecialSelector;
import CR.Game.TileSelector;
import CR.Game.Timer;

import std;
import std.compat;

export namespace CR::Game {
	class ResourcePuzzle : public IRenderable {
	  public:
		// offset and spacing are in remastered (640x960) pixels.
		ResourcePuzzle(glm::vec2 offset, float spacing);
		~ResourcePuzzle() override;

		ResourcePuzzle(const ResourcePuzzle&)            = delete;
		ResourcePuzzle& operator=(const ResourcePuzzle&) = delete;

		// IRenderable
		void Update() override;
		void Render() override;
		void SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) override {}
		void PauseAnimation(bool pause) override;

		glm::vec2 Offset() const { return m_offset; }
		float Spacing() const { return m_tileSpacing; }

		void ResetPuzzle();
		bool IsPuzzleAnimating() const;

		void SetDropRateBonusType(Constants::ResourceType rType) { m_dropRateBonusType = rType; }
		void SetBuildingSpecialDown(BuildingName bName, int level);
		void SetBuildingSpecialUp(BuildingName bName, int level);
		void CancelBuildingSpecial();
		void DropTouchValue() { m_touchActive = false; }
		void SetHintsEnabled(bool _value) { m_hintsEnabled = _value; }

		// Events
		CR::Utility::Event2<Constants::ResourceType, int> ResourcesRetrieved;
		CR::Utility::Event OnIncrementChain;
		CR::Utility::Event MoveMade;

	  private:
		static constexpr int c_tileCount = Constants::c_puzzleXTiles * Constants::c_puzzleYTiles;

		enum MatchOrientation { VerticalMatch, HorizontalMatch };

		struct MatchBounds {
			bool foundMatch{false};
			int x{};
			int y{};
			int lower{};
			int upper{};
			MatchOrientation orientation{VerticalMatch};

			Constants::ResourceType qType{Constants::Gold};
			int qTiles{};
		};

		// Tiles pulled out of the grid, waiting to be reused as new tiles sliding in from the top.
		struct RemovedTiles {
			std::array<ResourceTile*, c_tileCount> tiles{};
			int count{};
			std::array<int, Constants::c_puzzleXTiles> perColumn{};

			void Add(ResourceTile* tile, int column) {
				tiles[count++] = tile;
				++perColumn[column];
			}
		};

		void UpdateInput();
		void OnTouchTile(glm::ivec2 tile);
		glm::ivec2 CellSize() const;

		ResourceTile* GetResourceTile(glm::ivec2 tile) const;
		bool CheckAndRemoveTileMatches();
		MatchBounds FindMatch(glm::ivec2 tile, MatchOrientation orientation) const;
		void CollapseAndRefill(RemovedTiles& removed);
		void SwapTiles(glm::ivec2 tile1, glm::ivec2 tile2);
		bool CanSwapTiles(glm::ivec2 tile1, glm::ivec2 tile2) const;
		void RandomizeTiles();
		void AnimatePuzzleSlideIn();
		bool AnimateShakeTiles(glm::ivec2 tile1, glm::ivec2 tile2);

		bool IsPuzzleValid() const;
		int CountSwapMatches(glm::ivec2 tile) const;
		bool CheckForSwapMatch(glm::ivec2 tile, glm::ivec2 direction) const;
		bool CheckForLineMatch(glm::ivec2 tile, glm::ivec2 direction, const ResourceTile* rTile) const;
		bool CheckForMiddleMatch(glm::ivec2 tile, glm::ivec2 axis, const ResourceTile* rTile) const;

		void OnClearBuildingSpecialTiles();
		void OnExplodeTilesSpecial();

		void FindHintBlock();
		void AnimateHint();
		void StopAnimateHint();
		void PlayResourceSound(Constants::ResourceType rType);

		bool m_isAnimating{false};
		float m_tileSpacing{};
		glm::vec2 m_offset{};

		std::array<CR::Engine::Core::Embedded<ResourceTile>, c_tileCount> m_tileStorage;
		std::array<std::array<ResourceTile*, Constants::c_puzzleYTiles>, Constants::c_puzzleXTiles> m_resourceTiles{};
		std::array<std::array<bool, Constants::c_puzzleYTiles>, Constants::c_puzzleXTiles> m_specialClearTiles{};

		TileSelector m_tileSelector;
		SpecialSelector m_specialSelector;
		ParticleSystemManager m_particleSystemManager;
		ChainEffectManager m_chainManager;

		CR::Engine::Input::Handles::Region m_boardRegion;
		bool m_touchActive{false};
		glm::ivec2 m_touchTile{};

		bool m_hideTileSelectorAfterAnimation{false};
		Constants::ResourceType m_dropRateBonusType{Constants::NoTile};
		bool m_buildingSpecialDown{false};
		bool m_specialSelectorWasShown{false};

		bool m_resettingPuzzle{false};
		CR::Engine::Graphics::Handles::Sprite m_resetBoardSprite;

		float m_currHintTime{};
		glm::ivec2 m_hintOffset{};
		bool m_hintsEnabled{true};

		int m_chainCount{0};
		bool m_touchAllowed{true};

		std::array<std::array<CR::Engine::Audio::Handles::SoundFX, 2>, Constants::c_resourceTileTypes> m_resourceSounds;
		std::array<CR::Engine::Audio::Handles::SoundFX, 3> m_slideSounds;
		std::array<CR::Engine::Audio::Handles::SoundFX, 2> m_invalidSounds;
		CR::Engine::Audio::Handles::SoundFX m_noMovesSound;
		CR::Engine::Audio::Handles::SoundFX m_powerUsedSound;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceaudio = CR::Engine::Audio;
namespace cegraph = CR::Engine::Graphics;
namespace ceinput = CR::Engine::Input;
namespace cg      = CR::Game;

using namespace cecore::Literals;

namespace {
	constexpr float c_animTime   = 0.15f;
	constexpr float c_maxHintTime = 10.0f;

	constexpr glm::ivec2 c_tileSize{cg::Constants::c_resourceTileWidth, cg::Constants::c_resourceTileHeight};

	constexpr uint8_t c_resetBoardZ = 25;
	constexpr glm::vec2 c_resetBoardPosition{0.0f, 136.0f};

	constexpr std::array<cg::Constants::ResourceType, cg::NUM_BUILDING_NAMES> c_buildingResources{
		cg::Constants::Gold,    // TownHall
		cg::Constants::Crystal, // PoliceStation
		cg::Constants::Brick,   // Firehouse
		cg::Constants::Wood,    // Lodge
		cg::Constants::Diamond, // Theatre
		cg::Constants::Leaf,    // Greenhouse
		cg::Constants::NoTile,  // NoBuilding
	};

	bool IsAdjacent(glm::ivec2 tile1, glm::ivec2 tile2) {
		glm::ivec2 diff = glm::abs(tile2 - tile1);
		return diff.x + diff.y == 1;
	}
}    // namespace

cg::ResourcePuzzle::ResourcePuzzle(glm::vec2 offset, float spacing) :
	m_tileSpacing(spacing), m_offset(offset), m_tileSelector(spacing, offset) {
	m_currHintTime = c_maxHintTime;
	m_tileSelector.SetVisible(false);

	for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
		for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
			int index = y * Constants::c_puzzleXTiles + x;
			m_tileStorage[index].emplace(x, y, m_tileSpacing, m_offset);
			m_resourceTiles[x][y] = &*m_tileStorage[index];
		}
	}

	glm::ivec2 boardSize = CellSize() * glm::ivec2{Constants::c_puzzleXTiles, Constants::c_puzzleYTiles};
	m_boardRegion        = ceinput::Regions::create(cecore::Rect2D<int32_t>{glm::ivec2{m_offset}, boardSize});

	ResetPuzzle();

	m_specialSelector.OnDoneAnimating += CR::Utility::Delegate(this, &ResourcePuzzle::OnClearBuildingSpecialTiles);
	m_specialSelector.OnExplodeTiles += CR::Utility::Delegate(this, &ResourcePuzzle::OnExplodeTilesSpecial);

	cegraph::Sprites::Create("StoryModeNoMovesCover"_hash64, m_resetBoardSprite);
	cegraph::Sprites::SetZOrders(m_resetBoardSprite, c_resetBoardZ);
	cegraph::Sprites::SetPositions(m_resetBoardSprite, c_resetBoardPosition);
	cegraph::Sprites::SetVisibilities(m_resetBoardSprite, false);

	m_resourceSounds = {{
		{ceaudio::SoundFX::GetHandle("leaves1"_hash64), ceaudio::SoundFX::GetHandle("leaves2"_hash64)},
		{ceaudio::SoundFX::GetHandle("brick1"_hash64), ceaudio::SoundFX::GetHandle("brick2"_hash64)},
		{ceaudio::SoundFX::GetHandle("wood"_hash64), ceaudio::SoundFX::GetHandle("wood"_hash64)},
		{ceaudio::SoundFX::GetHandle("crystal1"_hash64), ceaudio::SoundFX::GetHandle("crystal2"_hash64)},
		{ceaudio::SoundFX::GetHandle("crystal1"_hash64), ceaudio::SoundFX::GetHandle("crystal2"_hash64)},
		{ceaudio::SoundFX::GetHandle("gold1"_hash64), ceaudio::SoundFX::GetHandle("gold2"_hash64)},
		{ceaudio::SoundFX::GetHandle("mpiece"_hash64), ceaudio::SoundFX::GetHandle("mpiece"_hash64)},
		{ceaudio::SoundFX::GetHandle("mpiece"_hash64), ceaudio::SoundFX::GetHandle("mpiece"_hash64)},
	}};
	m_slideSounds    = {ceaudio::SoundFX::GetHandle("slide1b"_hash64), ceaudio::SoundFX::GetHandle("slide2b"_hash64),
						ceaudio::SoundFX::GetHandle("slide3b"_hash64)};
	m_invalidSounds  = {ceaudio::SoundFX::GetHandle("invalidpiece1b"_hash64),
						ceaudio::SoundFX::GetHandle("invalidpiece2b"_hash64)};
	m_noMovesSound   = ceaudio::SoundFX::GetHandle("nomoves"_hash64);
	m_powerUsedSound = ceaudio::SoundFX::GetHandle("powerused1c"_hash64);
}

cg::ResourcePuzzle::~ResourcePuzzle() {
	cegraph::Sprites::Delete(m_resetBoardSprite);
	ceinput::Regions::release(m_boardRegion);
	for(auto& tile : m_tileStorage) { tile.reset(); }
}

void cg::ResourcePuzzle::Update() {
	UpdateInput();

	bool isAnimating = false;
	m_tileSelector.Update();
	m_specialSelector.Update();
	m_particleSystemManager.Update();
	for(auto& column : m_resourceTiles) {
		for(ResourceTile* tile : column) {
			if(!tile->TilePositionUpToDate()) { isAnimating = true; }
			tile->Update();
		}
	}

	m_isAnimating = isAnimating;

	if(!isAnimating) {
		m_resettingPuzzle = false;
		bool foundMatch   = CheckAndRemoveTileMatches();
		// if a match was found, the puzzle is going into an animated state
		if(foundMatch) { m_isAnimating = true; }

		// only check if the puzzle is valid if a match has not been found
		if(!foundMatch && !IsPuzzleValid()) {
			// Puzzle is not in a valid state, tell user, then shuffle
			ResetPuzzle();
			AnimatePuzzleSlideIn();
		} else if(foundMatch) {
			FindHintBlock();
			m_currHintTime = c_maxHintTime;
		}
	}

	if(!m_isAnimating && m_hideTileSelectorAfterAnimation) {
		m_hideTileSelectorAfterAnimation = false;
		m_tileSelector.SetVisible(false);
	}

	if(!m_isAnimating) {
		m_chainCount   = 0;
		m_touchAllowed = true;
	}

	if(m_hintsEnabled) {
		m_currHintTime -= Timer::GetTimePassed();

		if(m_currHintTime <= 0) {
			AnimateHint();
			m_currHintTime = c_maxHintTime;
		}
	}

	m_chainManager.Update();
}

void cg::ResourcePuzzle::Render() {
	cegraph::Sprites::SetVisibilities(m_resetBoardSprite, m_resettingPuzzle);

	bool hideSpecialTiles = m_specialSelector.IsSelector3() && m_specialSelector.HideTiles();
	for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
		for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
			m_resourceTiles[x][y]->SetVisible(!(hideSpecialTiles && m_specialClearTiles[x][y]));
			m_resourceTiles[x][y]->Render();
		}
	}

	m_tileSelector.Render();

	bool showSpecialSelector = false;
	if(m_buildingSpecialDown) {
		for(const auto& column : m_specialClearTiles) {
			showSpecialSelector = showSpecialSelector || std::ranges::contains(column, true);
		}
	}

	if(showSpecialSelector) {
		// restart the Selector2 animation each time it starts being shown
		if(!m_specialSelectorWasShown && !m_specialSelector.IsSelector3()) { m_specialSelector.ShowSelector2(); }

		for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
			for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
				if(!m_specialClearTiles[x][y]) { continue; }

				if(m_specialSelector.IsSelector3()) {
					m_resourceTiles[x][y]->RenderSelectorExplode(m_specialSelector.GetSprite());
				} else {
					m_resourceTiles[x][y]->RenderSelectorSpecial(m_specialSelector.GetSprite());
				}
			}
		}
	} else if(m_specialSelector.IsSelector3()) {
		// A power was used with no matching tiles on the board, so there is nothing to explode and
		// OnDoneAnimating will never fire. Cancel it, otherwise input stays blocked.
		m_specialSelector.ShowSelector2();
		m_buildingSpecialDown = false;
	}
	m_specialSelectorWasShown = showSpecialSelector;

	m_particleSystemManager.Render();
	m_chainManager.Render();
}

void cg::ResourcePuzzle::PauseAnimation(bool pause) {
	m_tileSelector.PauseAnimation(pause);
	m_particleSystemManager.PauseAnimation(pause);
	m_specialSelector.PauseAnimation(pause);
}

void cg::ResourcePuzzle::UpdateInput() {
	uint32_t state{};
	ceinput::Regions::getStates(m_boardRegion, state);
	if((state & ceinput::Regions::RegionStates::Down) == 0) {
		m_touchActive = false;
		return;
	}

	// Down means the cursor is inside the board, so the offset is always within the 7x7 cells.
	glm::ivec2 cursorOffset{};
	ceinput::Regions::getCursorOffsets(m_boardRegion, cursorOffset);
	// The spacing gap belongs to the tile on its left/top, same as the legacy touch math.
	glm::ivec2 tile = cursorOffset / CellSize();

	// A new press always counts, a held touch only counts when it is dragged onto another tile.
	if((state & ceinput::Regions::RegionStates::Pressed) != 0) {
		m_touchActive = false;
		m_touchTile   = tile;
		OnTouchTile(tile);
	} else if(m_touchActive && tile != m_touchTile) {
		m_touchTile = tile;
		OnTouchTile(tile);
	}
}

glm::ivec2 cg::ResourcePuzzle::CellSize() const {
	return c_tileSize + (int)m_tileSpacing;
}

void cg::ResourcePuzzle::OnTouchTile(glm::ivec2 tile) {
	// no input is allowed during animation
	if(!m_touchAllowed || m_isAnimating || m_specialSelector.IsSelector3() || m_buildingSpecialDown) return;

	m_touchActive = true;

	bool wasVisible        = m_tileSelector.Visible();
	glm::ivec2 oldPosition = m_tileSelector.GetPosition();
	m_tileSelector.SetPosition(tile);
	m_tileSelector.SetVisible(true);

	if(wasVisible && m_tileSelector.GetPosition() != oldPosition) {
		if(m_hintsEnabled) {
			m_currHintTime = c_maxHintTime;
			StopAnimateHint();
		}

		if(CanSwapTiles(oldPosition, m_tileSelector.GetPosition())) {
			SwapTiles(oldPosition, m_tileSelector.GetPosition());
		} else if(AnimateShakeTiles(oldPosition, m_tileSelector.GetPosition())) {
			m_hideTileSelectorAfterAnimation = true;
			m_touchActive                    = false;
		}

		// A move was made or attempted
		MoveMade();
		m_touchAllowed = false;
	}
}

void cg::ResourcePuzzle::ResetPuzzle() {
	bool hasMatches = true;

	RandomizeTiles();

	while(hasMatches) {
		hasMatches = false;
		for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
			for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
				MatchBounds hMatch = FindMatch({x, y}, HorizontalMatch);
				MatchBounds vMatch = FindMatch({x, y}, VerticalMatch);

				if(hMatch.foundMatch) {
					hasMatches = true;
					for(int i = hMatch.lower; i <= hMatch.upper; ++i) {
						m_resourceTiles[i][y]->RandomizeType(m_dropRateBonusType);
					}
				}

				if(vMatch.foundMatch) {
					hasMatches = true;
					for(int i = vMatch.lower; i <= vMatch.upper; ++i) {
						m_resourceTiles[x][i]->RandomizeType(m_dropRateBonusType);
					}
				}
			}
		}

		if(!IsPuzzleValid()) {
			hasMatches = true;
			RandomizeTiles();
		}
	}

	FindHintBlock();
}

void cg::ResourcePuzzle::AnimatePuzzleSlideIn() {
	ceaudio::SoundFX::Play(m_noMovesSound);

	m_resettingPuzzle = true;

	for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
		for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
			m_resourceTiles[x][y]->AnimateSlideIn({x, y - Constants::c_puzzleYTiles}, {x, y},
												  c_animTime * Constants::c_puzzleYTiles);
		}
	}
}

// returns true if a shake took place
bool cg::ResourcePuzzle::AnimateShakeTiles(glm::ivec2 tile1, glm::ivec2 tile2) {
	// if the tiles are not within 1 of eachother, or are diagonal from eachother, return without shaking them
	if(!IsAdjacent(tile1, tile2)) return false;

	glm::ivec2 direction = tile2 - tile1;
	Constants::AnimationDirection animDirection;
	if(direction.x == 1)
		animDirection = Constants::AnimateRight;
	else if(direction.x == -1)
		animDirection = Constants::AnimateLeft;
	else if(direction.y == 1)
		animDirection = Constants::AnimateDown;
	else
		animDirection = Constants::AnimateUp;
	GetResourceTile(tile1)->AnimateShakeTo(animDirection, c_animTime);

	ceaudio::SoundFX::Play(m_invalidSounds[cecore::Random(0, (int)m_invalidSounds.size() - 1)]);

	return true;
}

bool cg::ResourcePuzzle::CheckAndRemoveTileMatches() {
	// Find every match before removing any tiles, so tiles shared by a horizontal and vertical match are
	// still detected by both. Each tile can report at most one match in each orientation.
	std::array<MatchBounds, 2 * c_tileCount> matches;
	int matchCount = 0;
	for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
		for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
			for(MatchOrientation orientation : {HorizontalMatch, VerticalMatch}) {
				MatchBounds match = FindMatch({x, y}, orientation);
				if(match.foundMatch) { matches[matchCount++] = match; }
			}
		}
	}

	// if a match was not found, there is no need to continue
	if(matchCount == 0) return false;

	RemovedTiles removed;
	std::array<int, Constants::c_resourceTileTypes> resourceCount{};    // resources receieved through matches
	glm::vec2 chainPosition{};
	bool playedSound = false;

	for(const MatchBounds& match : std::span{matches.data(), (std::size_t)matchCount}) {
		bool horizontal = match.orientation == HorizontalMatch;
		glm::ivec2 step = horizontal ? glm::ivec2{1, 0} : glm::ivec2{0, 1};
		glm::ivec2 first = horizontal ? glm::ivec2{match.lower, match.y} : glm::ivec2{match.x, match.lower};

		// An earlier overlapping match may have already removed some of these tiles.
		if(const ResourceTile* middle = GetResourceTile(first + step * ((match.upper - match.lower) / 2))) {
			chainPosition = middle->GetAbsolutePosition();
		}

		bool isChain      = true;
		bool removedMatch = false;
		for(int i = 0; i <= match.upper - match.lower; ++i) {
			glm::ivec2 tile            = first + step * i;
			ResourceTile*& resourceTile = m_resourceTiles[tile.x][tile.y];
			if(resourceTile == nullptr) {
				isChain = false;
				continue;
			}

			resourceTile->StopAnimateHint();
			++resourceCount[resourceTile->GetResourceType()];
			m_particleSystemManager.AddParticleSystem(std::make_unique<CrystalParticleSystem>(
				resourceTile->GetAbsolutePosition(), resourceTile->GetResourceType()));
			removed.Add(resourceTile, tile.x);
			resourceTile = nullptr;
			removedMatch = true;
		}

		if(!removedMatch) { continue; }

		if(horizontal) {
			playedSound = true;
			PlayResourceSound(match.qType);
		} else if(!playedSound) {
			PlayResourceSound(match.qType);
		}

		if(isChain) {
			OnIncrementChain();
			++m_chainCount;
			m_chainManager.AddChainEffect(m_chainCount, (int)chainPosition.x, (int)chainPosition.y);
		}

		resourceCount[match.qType] += match.qTiles;
	}

	// Report Resources Retrieved
	for(int i = 0; i < Constants::c_resourceTileTypes; ++i) {
		if(resourceCount[i] > 0) { ResourcesRetrieved((Constants::ResourceType)i, resourceCount[i]); }
	}

	m_tileSelector.SetVisible(false);

	CollapseAndRefill(removed);

	return true;
}

cg::ResourcePuzzle::MatchBounds cg::ResourcePuzzle::FindMatch(glm::ivec2 tile, MatchOrientation orientation) const {
	// Q tiles are wild. A run containing Q tiles is found from its non Q tiles, including from a neighbor
	// of a run made only of Q tiles, so starting from a Q tile can never find anything extra.
	const ResourceTile* resourceTile = GetResourceTile(tile);
	if(resourceTile == nullptr || resourceTile->GetResourceType() == Constants::QTile) return {};

	Constants::ResourceType resourceType = resourceTile->GetResourceType();
	bool horizontal                      = orientation == HorizontalMatch;
	glm::ivec2 step                      = horizontal ? glm::ivec2{1, 0} : glm::ivec2{0, 1};

	int qTiles   = 0;
	auto inMatch = [&](glm::ivec2 checkTile) {
		const ResourceTile* tmpTile = GetResourceTile(checkTile);
		if(tmpTile == nullptr) return false;
		if(tmpTile->GetResourceType() == Constants::QTile) {
			++qTiles;
			return true;
		}
		return tmpTile->GetResourceType() == resourceType;
	};

	glm::ivec2 lowerTile = tile;
	while(inMatch(lowerTile - step)) { lowerTile -= step; }
	glm::ivec2 upperTile = tile;
	while(inMatch(upperTile + step)) { upperTile += step; }

	MatchBounds result;
	result.lower       = horizontal ? lowerTile.x : lowerTile.y;
	result.upper       = horizontal ? upperTile.x : upperTile.y;
	result.foundMatch  = (result.upper - result.lower) >= (Constants::c_tilesRequiredForMatch - 1);
	result.orientation = orientation;
	result.x           = tile.x;
	result.y           = tile.y;
	result.qType       = resourceType;
	result.qTiles      = qTiles;
	return result;
}

void cg::ResourcePuzzle::CollapseAndRefill(RemovedTiles& removed) {
	// Shift down all tiles
	for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
		for(int y = Constants::c_puzzleYTiles - 1; y > 0; --y) {
			if(m_resourceTiles[x][y] != nullptr) { continue; }

			for(int i = y - 1; i >= 0; --i) {
				if(ResourceTile* tmpTile = m_resourceTiles[x][i]) {
					m_resourceTiles[x][i] = nullptr;
					m_resourceTiles[x][y] = tmpTile;
					tmpTile->AnimateSlideTo({x, y}, c_animTime * (y - i));
					break;
				}
			}
		}
	}

	// Use the removed tiles as new tiles, slide them in
	int nextTile = 0;
	for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
		for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
			if(m_resourceTiles[x][y] != nullptr) { continue; }

			ResourceTile* newTile = removed.tiles[nextTile++];
			m_resourceTiles[x][y] = newTile;
			newTile->RandomizeType(m_dropRateBonusType);
			newTile->AnimateSlideIn({x, y - removed.perColumn[x]}, {x, y}, c_animTime * removed.perColumn[x]);
		}
	}
	CR_ASSERT(nextTile == removed.count, "ResourcePuzzle didn't reuse all removed tiles");
}

void cg::ResourcePuzzle::SwapTiles(glm::ivec2 tile1, glm::ivec2 tile2) {
	ResourceTile* rTile1 = GetResourceTile(tile1);
	ResourceTile* rTile2 = GetResourceTile(tile2);
	m_resourceTiles[tile1.x][tile1.y] = rTile2;
	m_resourceTiles[tile2.x][tile2.y] = rTile1;
	rTile1->AnimateSlideTo(tile2, c_animTime);
	rTile2->AnimateSlideTo(tile1, c_animTime);

	ceaudio::SoundFX::Play(m_slideSounds[cecore::Random(0, (int)m_slideSounds.size() - 1)]);
}

bool cg::ResourcePuzzle::CanSwapTiles(glm::ivec2 tile1, glm::ivec2 tile2) const {
	// if the tiles are not within 1 of eachother, or are diagonal from eachother, return false
	if(!IsAdjacent(tile1, tile2)) return false;

	glm::ivec2 direction = tile2 - tile1;
	return CheckForSwapMatch(tile1, direction) || CheckForSwapMatch(tile2, -direction);
}

void cg::ResourcePuzzle::RandomizeTiles() {
	for(auto& column : m_resourceTiles) {
		for(ResourceTile* tile : column) { tile->RandomizeType(m_dropRateBonusType); }
	}
}

bool cg::ResourcePuzzle::IsPuzzleValid() const {
	for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
		for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
			if(CountSwapMatches({x, y}) > 0) return true;
		}
	}

	return false;
}

int cg::ResourcePuzzle::CountSwapMatches(glm::ivec2 tile) const {
	int count = 0;
	for(glm::ivec2 direction : {glm::ivec2{-1, 0}, glm::ivec2{1, 0}, glm::ivec2{0, -1}, glm::ivec2{0, 1}}) {
		if(CheckForSwapMatch(tile, direction)) { ++count; }
	}
	return count;
}

// Would moving the tile one space in direction make a match at its new location.
bool cg::ResourcePuzzle::CheckForSwapMatch(glm::ivec2 tile, glm::ivec2 direction) const {
	const ResourceTile* rTile = GetResourceTile(tile);
	glm::ivec2 target         = tile + direction;
	glm::ivec2 perpendicular{direction.y, direction.x};

	return CheckForLineMatch(target, direction, rTile) || CheckForLineMatch(target, perpendicular, rTile) ||
		   CheckForLineMatch(target, -perpendicular, rTile) || CheckForMiddleMatch(target, perpendicular, rTile);
}

// rTile placed at tile matches the next two tiles in direction.
bool cg::ResourcePuzzle::CheckForLineMatch(glm::ivec2 tile, glm::ivec2 direction, const ResourceTile* rTile) const {
	if(rTile == nullptr) return false;

	Constants::ResourceType rType = rTile->GetResourceType();

	for(int i = 1; i <= 2; ++i) {
		const ResourceTile* t1 = GetResourceTile(tile + direction * i);
		if(t1 != nullptr && rType == Constants::QTile) rType = t1->GetResourceType();

		if(t1 == nullptr || (t1->GetResourceType() != rType && t1->GetResourceType() != Constants::QTile))
			return false;
	}

	return true;
}

// rTile placed at tile matches the tiles on either side of it along axis.
bool cg::ResourcePuzzle::CheckForMiddleMatch(glm::ivec2 tile, glm::ivec2 axis, const ResourceTile* rTile) const {
	if(rTile == nullptr) return false;

	const ResourceTile* t1 = GetResourceTile(tile - axis);
	const ResourceTile* t2 = GetResourceTile(tile + axis);
	if(t1 == nullptr || t2 == nullptr) return false;

	Constants::ResourceType rType  = rTile->GetResourceType();
	Constants::ResourceType t1Type = t1->GetResourceType();
	Constants::ResourceType t2Type = t2->GetResourceType();

	if(rType == Constants::QTile) {
		return t1Type == t2Type || t1Type == Constants::QTile || t2Type == Constants::QTile;
	}
	return (t1Type == rType || t1Type == Constants::QTile) && (t2Type == rType || t2Type == Constants::QTile);
}

cg::ResourceTile* cg::ResourcePuzzle::GetResourceTile(glm::ivec2 tile) const {
	if(tile.x >= 0 && tile.x < Constants::c_puzzleXTiles && tile.y >= 0 && tile.y < Constants::c_puzzleYTiles)
		return m_resourceTiles[tile.x][tile.y];
	else
		return nullptr;
}

bool cg::ResourcePuzzle::IsPuzzleAnimating() const {
	return m_isAnimating || m_specialSelector.IsSelector3();
}

void cg::ResourcePuzzle::SetBuildingSpecialDown(BuildingName bName, int level) {
	int breakBlocks     = 0;
	bool breakAllBlocks = false;

	m_tileSelector.SetVisible(false);

	// determine how many random blocks to mark for breaking
	if(level <= 2)
		breakBlocks = 3;
	else if(level <= 4)
		breakBlocks = 4;
	else
		breakAllBlocks = true;

	Constants::ResourceType rType = c_buildingResources[bName];

	// find all blocks of the specified resource type
	std::array<glm::ivec2, c_tileCount> tilePoints;
	int tileCount = 0;
	for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
		for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
			if(m_resourceTiles[x][y]->GetResourceType() == rType) { tilePoints[tileCount++] = {x, y}; }
		}
	}

	// mark all of the blocks, or a random selection up to the amount required
	int markCount = (breakAllBlocks || tileCount <= breakBlocks) ? tileCount : breakBlocks;
	for(int i = 0; i < markCount; ++i) {
		std::swap(tilePoints[i], tilePoints[cecore::Random(i, tileCount - 1)]);
		m_specialClearTiles[tilePoints[i].x][tilePoints[i].y] = true;
	}

	m_buildingSpecialDown = true;
}

void cg::ResourcePuzzle::SetBuildingSpecialUp([[maybe_unused]] BuildingName bName, [[maybe_unused]] int level) {
	if(!m_specialSelector.IsSelector3()) {
		ceaudio::SoundFX::Play(m_powerUsedSound);
		StopAnimateHint();
	}

	m_specialSelector.ShowSelector3();
}

void cg::ResourcePuzzle::CancelBuildingSpecial() {
	if(!m_specialSelector.IsSelector3()) {
		m_specialClearTiles   = {};
		m_buildingSpecialDown = false;
	}
}

void cg::ResourcePuzzle::OnClearBuildingSpecialTiles() {
	RemovedTiles removed;
	std::array<int, Constants::c_resourceTileTypes> resourceCount{};    // resources receieved through specials

	m_buildingSpecialDown = false;

	for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
		for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
			if(m_specialClearTiles[x][y]) {
				ResourceTile*& resourceTile = m_resourceTiles[x][y];
				++resourceCount[resourceTile->GetResourceType()];
				removed.Add(resourceTile, x);
				resourceTile = nullptr;
			}
		}
	}
	m_specialClearTiles = {};

	if(removed.count == 0) return;

	// Report Resources Retrieved
	for(int i = 0; i < Constants::c_resourceTileTypes; ++i) {
		if(resourceCount[i] > 0) { ResourcesRetrieved((Constants::ResourceType)i, resourceCount[i]); }
	}

	m_tileSelector.SetVisible(false);

	CollapseAndRefill(removed);

	FindHintBlock();
}

void cg::ResourcePuzzle::OnExplodeTilesSpecial() {
	if(!m_buildingSpecialDown) return;

	for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
		for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
			if(m_specialClearTiles[x][y]) {
				m_particleSystemManager.AddParticleSystem(std::make_unique<CrystalParticleSystem>(
					m_resourceTiles[x][y]->GetAbsolutePosition(), m_resourceTiles[x][y]->GetResourceType()));
			}
		}
	}
}

void cg::ResourcePuzzle::FindHintBlock() {
	if(!m_hintsEnabled) return;

	// Pick a random valid move, a tile with several valid moves is proportionally more likely to be chosen.
	int validMoves = 0;
	for(int x = 0; x < Constants::c_puzzleXTiles; ++x) {
		for(int y = 0; y < Constants::c_puzzleYTiles; ++y) {
			int tileMoves = CountSwapMatches({x, y});
			if(tileMoves == 0) { continue; }

			validMoves += tileMoves;
			if(cecore::Random(1, validMoves) <= tileMoves) { m_hintOffset = {x, y}; }
		}
	}
}

void cg::ResourcePuzzle::AnimateHint() {
	m_resourceTiles[m_hintOffset.x][m_hintOffset.y]->AnimateHint();
}

void cg::ResourcePuzzle::StopAnimateHint() {
	m_resourceTiles[m_hintOffset.x][m_hintOffset.y]->StopAnimateHint();
}

void cg::ResourcePuzzle::PlayResourceSound(Constants::ResourceType rType) {
	ceaudio::SoundFX::Play(m_resourceSounds[rType][cecore::Random(0, 1)]);
}
