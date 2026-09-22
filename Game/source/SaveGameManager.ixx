/*
 *  SaveGameManager.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/26/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.SaveGameManager;

import CR.Game.ISaveOnTerminate;
import CR.Game.StoryModeSaveInfo;
import CR.Game.ArcadeModeSaveInfo;
import CR.Game.TrophySaveInfo;
import CR.Game.SettingsSaveInfo;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SaveGameManager {
	  public:
		SaveGameManager();
		virtual ~SaveGameManager();

		StoryModeSaveInfo* LoadStoryModeSaveInfo();
		void SaveStoryModeSaveInfo(StoryModeSaveInfo* info);

		ArcadeModeSaveInfo* LoadArcadeModeSaveInfo();
		void SaveArcadeModeSaveInfo(ArcadeModeSaveInfo* info);

		TrophySaveInfo* LoadTrophySaveInfo();
		void SaveTrophySaveInfo(TrophySaveInfo* info);

		SettingsSaveInfo* LoadSettingsSaveInfo();
		void SaveSettingsSaveInfo(SettingsSaveInfo* info);

		void SaveOnTerminate();
		void SetCurrentSaveOnTerminate(ISaveOnTerminate* saveOnTerminate);

		void Reset();

	  private:
		ISaveOnTerminate* m_saveOnTerminate;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::SaveGameManager::SaveGameManager() {
	m_saveOnTerminate = NULL;
}

cg::SaveGameManager::~SaveGameManager() {}

cg::StoryModeSaveInfo* cg::SaveGameManager::LoadStoryModeSaveInfo() {
	StoryModeSaveInfo* info = new StoryModeSaveInfo();
	info->Load();
	return info;
}

void cg::SaveGameManager::SaveStoryModeSaveInfo(StoryModeSaveInfo* info) {
	info->Save();
}

cg::ArcadeModeSaveInfo* cg::SaveGameManager::LoadArcadeModeSaveInfo() {
	ArcadeModeSaveInfo* info = new ArcadeModeSaveInfo();
	info->Load();
	return info;
}

void cg::SaveGameManager::SaveArcadeModeSaveInfo(ArcadeModeSaveInfo* info) {
	info->Save();
}

cg::TrophySaveInfo* cg::SaveGameManager::LoadTrophySaveInfo() {
	TrophySaveInfo* info = new TrophySaveInfo();
	info->Load();
	return info;
}

void cg::SaveGameManager::SaveTrophySaveInfo(TrophySaveInfo* info) {
	info->Save();
}

cg::SettingsSaveInfo* cg::SaveGameManager::LoadSettingsSaveInfo() {
	SettingsSaveInfo* info = new SettingsSaveInfo();
	info->Load();
	return info;
}

void cg::SaveGameManager::SaveSettingsSaveInfo(SettingsSaveInfo* info) {
	info->Save();
}

void cg::SaveGameManager::SaveOnTerminate() {
	if(m_saveOnTerminate != NULL) m_saveOnTerminate->OnSaveOnTerminate();
}

void cg::SaveGameManager::SetCurrentSaveOnTerminate(ISaveOnTerminate* saveOnTerminate) {
	m_saveOnTerminate = saveOnTerminate;
}

void cg::SaveGameManager::Reset() {
	StoryModeSaveInfo* storySave = LoadStoryModeSaveInfo();
	storySave->Reset();
	storySave->Save();

	ArcadeModeSaveInfo* arcadeSave = LoadArcadeModeSaveInfo();
	arcadeSave->Reset();
	arcadeSave->Save();

	SettingsSaveInfo* settingsSave = LoadSettingsSaveInfo();
	settingsSave->Reset();
	settingsSave->Save();

	TrophySaveInfo* trophySave = LoadTrophySaveInfo();
	trophySave->Reset();
	trophySave->Save();
}