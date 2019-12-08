////////////////////////////////////////////////////////////////////////////////
// Filename: gameclss.cpp
////////////////////////////////////////////////////////////////////////////////
#include "gameclass.h"

GameClass::GameClass()
{
	m_sceneType = 0;

	m_consModelNum = 0;

	m_emeraldCnt = 0;
	m_rubyCnt = 0;
	m_sapphireCnt = 0;
	m_amethystCnt = 0;
	m_diamondCnt = 0;
}


GameClass::GameClass(const GameClass& other)
{

}


GameClass::~GameClass()
{

}


////////////////
// Initialize //
////////////////

bool GameClass::Initialize()
{
	return true;
}


//////////////////
// GetSceneType //
//////////////////

int GameClass::GetSceneType()
{
	return m_sceneType;
}


//////////////////
// SetSceneType //
//////////////////

void GameClass::SetSceneType(int sceneType)
{
	m_sceneType = sceneType;
}


//////////////////
// Get GemCount //
//////////////////

int GameClass::GetEmeralCnt()
{
	return m_emeraldCnt;
}


int GameClass::GetRubyCnt()
{
	return m_rubyCnt;
}


int GameClass::GetSapphCnt()
{
	return m_sapphireCnt;
}


int GameClass::GetAmethCnt()
{
	return m_amethystCnt;
}


int GameClass::GetDiaCnt()
{
	return m_diamondCnt;
}


//////////////////
// Set GemCount //
//////////////////

void GameClass::SetEmeralCnt(int cnt)
{
	m_emeraldCnt = cnt;
}


void GameClass::SetRubyCnt(int cnt)
{
	m_rubyCnt = cnt;
}


void GameClass::SetSapphCnt(int cnt)
{
	m_sapphireCnt = cnt;
}


void GameClass::SetAmethCnt(int cnt)
{
	m_amethystCnt = cnt;
}


void GameClass::SetDiaCnt(int cnt)
{
	m_diamondCnt = cnt;
}


////////////////////
// ConstructCrate //
////////////////////

void GameClass::ConstructCrate()
{
	++m_consModelNum;

	--m_emeraldCnt;
	--m_rubyCnt;
	--m_sapphireCnt;
	--m_amethystCnt;
}


///////////////////
// DestructCrate //
///////////////////

void GameClass::DestructCrate()
{
	--m_consModelNum;

	++m_emeraldCnt;
	++m_rubyCnt;
	++m_sapphireCnt;
	++m_amethystCnt;
}


/////////////////////
// GetConsModelNum //
/////////////////////

int GameClass::GetConsModelNum()
{
	return m_consModelNum;
}


/////////////////////
// SetConsModelNum //
/////////////////////

void GameClass::SetConsModelNum(int num)
{
	m_consModelNum = num;
}


/////////////////////
// AbleToConstruct //
/////////////////////

bool GameClass::AbleToConstruct()
{
	if (GetEmeralCnt() >= 1 && GetRubyCnt() >= 1 && GetSapphCnt() >= 1 && GetAmethCnt() >= 1)
		return true;

	return false;
}


//////////////
// Increase //
//////////////

void GameClass::IncEmeralCnt()
{
	++m_emeraldCnt;
}

void GameClass::IncRubyCnt()
{
	++m_rubyCnt;
}

void GameClass::IncSapphCnt()
{
	++m_sapphireCnt;
}

void GameClass::IncAmethCnt()
{
	++m_amethystCnt;
}

void GameClass::IncDiaCnt()
{
	++m_diamondCnt;
}


//////////////
// Decrease //
//////////////

void GameClass::DecEmeralCnt()
{
	--m_emeraldCnt;
}

void GameClass::DecRubyCnt()
{
	--m_rubyCnt;
}

void GameClass::DecSapphCnt()
{
	--m_sapphireCnt;
}

void GameClass::DecAmethCnt()
{
	--m_amethystCnt;
}

void GameClass::DecDiaCnt()
{
	--m_diamondCnt;
}