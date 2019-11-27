////////////////////////////////////////////////////////////////////////////////
// Filename: raycastingclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "raycastingclass.h"


RaycastingClass::RaycastingClass()
{
	ClientWidth = 0;
	ClientHeight = 0;

	pickedDist = 0.0f;

	int mousex = 0;
	int mousey = 0;

	bool hit = false;
}


RaycastingClass::RaycastingClass(const RaycastingClass& other)
{

}


RaycastingClass::~RaycastingClass()
{

}


D3DXVECTOR3 RaycastingClass::doRaycast
(
	HWND hwnd,
	std::vector<D3DXVECTOR3> floorVertPosArray,
	std::vector<DWORD> floorVertIndexArray,
	D3DXMATRIX floorWorld,
	D3DXMATRIX camProjection,
	D3DXMATRIX camView
)
{
	POINT mousePos;

	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);

	mousex = mousePos.x;
	mousey = mousePos.y;

	float tempDist;
	float closestDist = FLT_MAX;
	int hitIndex;

	D3DXVECTOR3 prwsPos, prwsDir;
	pickRayVector(mousex, mousey, prwsPos, prwsDir, camProjection, camView);

	D3DXVECTOR3 castedPos;

	if (hit == false) //No need to check bottles already hit
	{
		tempDist = pick(prwsPos, prwsDir, floorVertPosArray, floorVertIndexArray, floorWorld, &castedPos);
		if (tempDist < closestDist)
		{
			closestDist = tempDist;
		}
	}

	if (closestDist < FLT_MAX)
	{
		hit = true;
		pickedDist = closestDist;
	}

	if (hit == true)
		return castedPos;

	return D3DXVECTOR3(0, 0, 0);
}


void RaycastingClass::pickRayVector
(
	float mouseX,
	float mouseY,
	D3DXVECTOR3& pickRayInWorldSpacePos,
	D3DXVECTOR3& pickRayInWorldSpaceDir,
	D3DXMATRIX camProjection,
	D3DXMATRIX camView
)
{
	D3DXVECTOR3 pickRayInViewSpaceDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pickRayInViewSpacePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;

	//Transform 2D pick position on screen space to 3D ray in View space
	PRVecX = /*(((2.0f * mouseX) / ClientWidth) - 1) / camProjection(0, 0)*/ ClientWidth / 2;
	PRVecY = /*-(((2.0f * mouseY) / ClientHeight) - 1) / camProjection(1, 1)*/ ClientHeight / 2;
	PRVecZ = 1.0f;	//View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	pickRayInViewSpaceDir = D3DXVECTOR3(PRVecX, PRVecY, PRVecZ);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	D3DXMATRIX pickRayToWorldSpaceMatrix;
	D3DXMATRIX matInvDeter;	//We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = *D3DXMatrixInverse(&matInvDeter, NULL, &camView);	//Inverse of View Space matrix is World space matrix

	D3DXVECTOR3 tempVec;

	pickRayInWorldSpacePos = *D3DXVec3TransformCoord(&tempVec, &pickRayInViewSpacePos, &pickRayToWorldSpaceMatrix);
	pickRayInWorldSpaceDir = *D3DXVec3TransformNormal(&tempVec, &pickRayInViewSpaceDir, &pickRayToWorldSpaceMatrix);
}


float RaycastingClass::pick
(
	D3DXVECTOR3 pickRayInWorldSpacePos,
	D3DXVECTOR3 pickRayInWorldSpaceDir,
	std::vector<D3DXVECTOR3>& vertPosArray,
	std::vector<DWORD>& indexPosArray,
	D3DXMATRIX& worldSpace,
	D3DXVECTOR3* castedPos
)
{
	D3DXVECTOR3 tempVec;

	//Loop through each triangle in the object
	for (int i = 0; i < indexPosArray.size() / 3; i++)
	{
		//Triangle's vertices V1, V2, V3
		D3DXVECTOR3 tri1V1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 tri1V2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 tri1V3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//Temporary 3d floats for each vertex
		D3DXVECTOR3 tV1, tV2, tV3;

		//Get triangle 
		tV1 = vertPosArray[indexPosArray[(i * 3) + 0]];
		tV2 = vertPosArray[indexPosArray[(i * 3) + 1]];
		tV3 = vertPosArray[indexPosArray[(i * 3) + 2]];

		tri1V1 = D3DXVECTOR3(tV1.x, tV1.y, tV1.z);
		tri1V2 = D3DXVECTOR3(tV2.x, tV2.y, tV2.z);
		tri1V3 = D3DXVECTOR3(tV3.x, tV3.y, tV3.z);

		//Transform the vertices to world space
		tri1V1 = *D3DXVec3TransformCoord(&tempVec, &tri1V1, &worldSpace);
		tri1V2 = *D3DXVec3TransformCoord(&tempVec, &tri1V2, &worldSpace);
		tri1V3 = *D3DXVec3TransformCoord(&tempVec, &tri1V3, &worldSpace);

		//Find the normal using U, V coordinates (two edges)
		D3DXVECTOR3 U = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 V = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 faceNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		U = tri1V2 - tri1V1;
		V = tri1V3 - tri1V1;

		//Compute face normal by crossing U, V
		faceNormal = *D3DXVec3Cross(&tempVec, &U, &V);

		faceNormal = *D3DXVec3Normalize(&tempVec, &faceNormal);

		//Calculate a point on the triangle for the plane equation
		D3DXVECTOR3 triPoint = tri1V1;

		//Get plane equation ("Ax + By + Cz + D = 0") Variables
		float tri1A = faceNormal.x;
		float tri1B = faceNormal.y;
		float tri1C = faceNormal.z; 
		float tri1D = (-tri1A * triPoint.x - tri1B * triPoint.y - tri1C * triPoint.z);

		//Now we find where (on the ray) the ray intersects with the triangles plane
		float ep1, ep2, t = 0.0f;
		float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
		D3DXVECTOR3 pointInPlane = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ep1 = (pickRayInWorldSpacePos.x * tri1A) + (pickRayInWorldSpacePos.y * tri1B) + (pickRayInWorldSpacePos.z * tri1C);
		ep2 = (pickRayInWorldSpaceDir.x * tri1A) + (pickRayInWorldSpaceDir.y * tri1B) + (pickRayInWorldSpaceDir.z * tri1C);

		//Make sure there are no divide-by-zeros
		if (ep2 != 0.0f)
			t = -(ep1 + tri1D) / (ep2);

		if (t > 0.0f)    //Make sure you don't pick objects behind the camera
		{
			//Get the point on the plane
			planeIntersectX = pickRayInWorldSpacePos.x + pickRayInWorldSpaceDir.x * t;
			planeIntersectY = pickRayInWorldSpacePos.y + pickRayInWorldSpaceDir.y * t;
			planeIntersectZ = pickRayInWorldSpacePos.z + pickRayInWorldSpaceDir.z * t;

			castedPos = &D3DXVECTOR3(planeIntersectX, planeIntersectY, planeIntersectZ);

			pointInPlane = D3DXVECTOR3(planeIntersectX, planeIntersectY, planeIntersectZ);

			//Call function to check if point is in the triangle
			if (PointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
			{
				//Return the distance to the hit, so you can check all the other pickable objects in your scene
				//and choose whichever object is closest to the camera
				return t / 2.0f;
			}
		}
	}
	//return the max float value (near infinity) if an object was not picked
	return FLT_MAX;
}


bool RaycastingClass::PointInTriangle(D3DXVECTOR3& triV1, D3DXVECTOR3& triV2, D3DXVECTOR3& triV3, D3DXVECTOR3& point)
{
	D3DXVECTOR3 tempVec;

	//To find out if the point is inside the triangle, we will check to see if the point
	//is on the correct side of each of the triangles edges.

	D3DXVECTOR3 cp1 = *D3DXVec3Cross(&tempVec, &(triV3 - triV2), &(point - triV2));
	D3DXVECTOR3 cp2 = *D3DXVec3Cross(&tempVec, &(triV3 - triV2), &(triV1 - triV2));
	if (D3DXVec3Dot(&cp1, &cp2) >= 0)
	{
		cp1 = *D3DXVec3Cross(&tempVec, &(triV3 - triV1), &(point - triV1));
		cp2 = *D3DXVec3Cross(&tempVec, &(triV3 - triV1), &(triV2 - triV1));
		if (D3DXVec3Dot(&cp1, &cp2) >= 0)
		{
			cp1 = *D3DXVec3Cross(&tempVec, &(triV2 - triV1), &(point - triV1));
			cp2 = *D3DXVec3Cross(&tempVec, &(triV2 - triV1), &(triV3 - triV1));
			if (D3DXVec3Dot(&cp1, &cp2) >= 0)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}