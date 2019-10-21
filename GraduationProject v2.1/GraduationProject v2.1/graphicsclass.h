////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#define MODELNUM 4
#define LIGHTNUM 2


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "shadermanagerclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "pointlightclass.h"
#include "modelclass.h"
#include "bumpmodelclass.h"
#include "inputclass.h"

#include "textclass.h"
#include "skyboxclass.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, int, int, int, int, float);

	void MoveCamera(int);
	void RotateCamera(float, float, float);

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	ShaderManagerClass* m_ShaderManager;
	CameraClass* m_Camera;
	LightClass* m_Light[LIGHTNUM];
	ModelClass* m_Model[MODELNUM];
	PointLightClass *m_PointLight0;

	TextClass* m_Text;

	SkyboxClass* m_Skybox;
};

#endif