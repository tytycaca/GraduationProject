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

	void initialize();

	int getSceneType();

	int getSapphCnt();
	int getEmeralCnt();
	int getDiaCnt();
	int getAmethCnt();
	int getRubyCnt();

	void setSceneType(int);

	void setSapphCnt(int);
	void setEmeralCnt(int);
	void setDiaCnt(int);
	void setAmethCnt(int);
	void setRubyCnt(int);

private:
	void sceneManager();



private:
	int m_sceneType;

	int m_sapphireCnt;
	int m_emeraldCnt;
	int m_diamondCnt;
	int m_amethystCnt;
	int m_rubyCnt;
};

#endif