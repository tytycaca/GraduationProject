////////////////////////////////////////////////////////////////////////////////
// Filename: gameclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GAMECLASS_H_
#define _GAMECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include "timerclass.h"
#include "soundclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: GameClss
////////////////////////////////////////////////////////////////////////////////
class GameClass
{
public:
	GameClass();
	GameClass(const GameClass&);
	~GameClass();

	bool Initialize();

	int GetSceneType();
	void SetSceneType(int);

	int GetEmeralCnt();
	int GetRubyCnt();
	int GetSapphCnt();
	int GetAmethCnt();
	int GetDiaCnt();

	void SetEmeralCnt(int);
	void SetRubyCnt(int);
	void SetSapphCnt(int);
	void SetAmethCnt(int);
	void SetDiaCnt(int);

	void IncEmeralCnt();
	void IncRubyCnt();
	void IncSapphCnt();
	void IncAmethCnt();
	void IncDiaCnt();

	void DecEmeralCnt();
	void DecRubyCnt();
	void DecSapphCnt();
	void DecAmethCnt();
	void DecDiaCnt();

	void ConstructCrate();
	void DestructCrate();
	int GetConsModelNum();
	void SetConsModelNum(int);

	bool AbleToConstruct();

private:
	int m_sceneType;

	int m_consModelNum;

	int m_sapphireCnt;
	int m_emeraldCnt;
	int m_diamondCnt;
	int m_amethystCnt;
	int m_rubyCnt;

	bool m_isGameCleared;
};

#endif