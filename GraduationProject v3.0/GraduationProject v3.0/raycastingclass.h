////////////////////////////////////////////////////////////////////////////////
// Filename: raycastingclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RAYCASTINGCLASS_H_
#define _RAYCASTINGCLASS_H_

#include <vector>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"
//#include "graphicsclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: RaycastingClass
////////////////////////////////////////////////////////////////////////////////
class RaycastingClass
{
public:
	RaycastingClass();
	RaycastingClass(const RaycastingClass&);
	~RaycastingClass();

	D3DXVECTOR3 doRaycast
	(
		HWND,
		std::vector<D3DXVECTOR3>,
		std::vector<DWORD>,
		D3DXMATRIX,
		D3DXMATRIX,
		D3DXMATRIX
	);

private:
	void pickRayVector
	(
		float mouseX,
		float mouseY,
		D3DXVECTOR3& pickRayInWorldSpacePos,
		D3DXVECTOR3& pickRayInWorldSpaceDir,
		D3DXMATRIX,
		D3DXMATRIX
	);

	float pick
	(
		D3DXVECTOR3 pickRayInWorldSpacePos,
		D3DXVECTOR3 pickRayInWorldSpaceDir,
		std::vector<D3DXVECTOR3>& vertPosArray,
		std::vector<DWORD>& indexPosArray,
		D3DXMATRIX& worldSpace,
		D3DXVECTOR3*
	);

	bool PointInTriangle
	(
		D3DXVECTOR3& triV1,
		D3DXVECTOR3& triV2,
		D3DXVECTOR3& triV3,
		D3DXVECTOR3& point
	);

private:
	std::vector<D3DXVECTOR3> groundVertPosArray;
	std::vector<DWORD> groundVertIndexArray;

	int ClientWidth;
	int ClientHeight;

	float pickedDist;

	int mousex;
	int mousey;

	bool hit;
};


#endif