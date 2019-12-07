////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_ShaderManager = 0;
	m_Camera = 0;
	for (int i = 0; i < LIGHTNUM; i++)
		m_Light[i] = 0;
	for (int i = 0; i < MODELNUM; i++)
		m_Model[i] = 0;

	m_PointLight0 = 0;

	m_Text = 0;

	m_Skybox = 0;

	m_Md5Model = 0;

	for (int i = 0; i < BITMAPNUM; i++)
		m_Bitmap[i] = 0;

	m_movement = 0.0f;
	m_AImovement = 0.0f;
	plusMinus = 1;

	isStart = true;

	checkFirst = false;

	isLClicked = false;
	isRClicked = false;

	m_animTimeStack = 0.0f;
	m_constructAnimTrigger = false;

	m_charPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_charRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	m_hwnd = hwnd;

	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	if(!m_ShaderManager)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}


	////////////
	// Camera //
	////////////

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 30.0f, 0.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(m_BaseViewMatrix);
	m_Camera->SetPosition(0.0f, 100.0f, 0.0f);


	////////////
	// Skybox //
	////////////

	// Create the skybox object.
	m_Skybox = new SkyboxClass;
	if (!m_Skybox)
	{
		return false;
	}

	m_Skybox->CreateSphere(m_D3D->GetDevice(), 10, 10);
	m_Skybox->InitializeSkyboxShader(m_D3D->GetDevice());


	////////////////
	// FontEngine //
	////////////////

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, m_BaseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}


	//////////////////
	// BitmapShader //
	//////////////////

	// Create the texture shader object.
	m_BitmapShader = new BitmapShaderClass;
	if (!m_BitmapShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_BitmapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap shader object.", L"Error", MB_OK);
		return false;
	}


	/////////////////
	// Bitmap (UI) //
	/////////////////

	for (int i = 0; i < BITMAPNUM; i++)
	{
		// Create the bitmap object.
		m_Bitmap[i] = new BitmapClass;
		if (!m_Bitmap)
		{
			return false;
		}
	}

	// Initialize the bitmap object.
	result = m_Bitmap[0]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, (WCHAR*)L"../GraduationProject v3.0/UI/UI Construction container SciFi crate.png", 287, 287);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap[1]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, (WCHAR*)L"../GraduationProject v3.0/UI/UI Inventory HUD with Jewels.png", 728, 88);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}




	/////////////
	// Raycast //
	/////////////

	// Create the raycast object.
	m_Raycast = new RaycastingClass;
	if (!m_Raycast)
	{
		return false;
	}


	/////////////////
	// NormalLight //
	/////////////////

	// Create the light object.
	for (int i = 0; i < LIGHTNUM; i++)
		m_Light[i] = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.

	//0 -1 0 Direction Light with high SpecularPower
	m_Light[0]->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light[0]->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light[0]->SetDirection(0.0f, -1.0f, 0.0f);
	m_Light[0]->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light[0]->SetSpecularPower(0.5f);

	//1 -1 1 Direction Light with high SpecularPower
	m_Light[1]->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light[1]->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light[1]->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light[1]->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light[1]->SetSpecularPower(64.0f);


	////////////////
	// PointLight //
	////////////////

	// Create the first pointlight object.
	m_PointLight0 = new PointLightClass;
	if (!m_PointLight0)
	{
		return false;
	}

	// Initialize the first pointlight object.
	m_PointLight0->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight0->SetPosition(-1463.0f, 786.0f, -959.0f);


	// Create the model objects.
	for (int i = 0; i < MODELNUM; i++)
	{
		m_Model[i] = new ModelClass;
		if (!m_Model[i])
		{
			return false;
		}
	}


	///////////////
	// Md5 Model //
	///////////////

	// Create the Md5Model object.
	m_Md5Model = new Md5ModelClass;
	if (!m_Md5Model)
	{
		return false;
	}

	// Initialize the Md5Model object.
	result = m_Md5Model->InitializeMd5Model
	(
		L"NuclearSuit character.md5mesh",
		L"NuclearSuit character Walk.md5anim",
		L"NuclearSuit character Construction.md5anim",
		m_D3D->GetDevice()
	);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Md5Model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the Md5Shader.
	m_Md5Model->InitializeMd5Shader(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), screenWidth, screenHeight);

	m_Md5Model->MakeAABB(XMMatrixIdentity());

	///////////
	// Floor //
	///////////

	// Initialize the model0 object. (Floor.obj)
	result = m_Model[0]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Floor.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/darkGreenGrass.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	/////////////////////
	// Trees and Rocks //
	/////////////////////

	// Initialize the model1 object.
	result = m_Model[1]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Rock1.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model2 object.
	result = m_Model[2]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Rock2.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model3 object.
	result = m_Model[3]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Rock3.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model4 object.
	result = m_Model[4]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Rock4.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model5 object.
	result = m_Model[5]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Rock5.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model6 object.
	result = m_Model[6]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Rock6.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model7 object.
	result = m_Model[7]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Tree1.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model8 object.
	result = m_Model[8]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/PineTree1.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/Nature_PineTree_RoseBush_Weeds.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model9 object.
	result = m_Model[9]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/PineTree2.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/Nature_PineTree_RoseBush_Weeds.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model10 object.
	result = m_Model[10]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/PineTree3.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/Nature_PineTree_RoseBush_Weeds.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model11 object.
	result = m_Model[11]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/RoseBush1.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/Nature_PineTree_RoseBush_Weeds.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model12 object.
	result = m_Model[12]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/RoseBush2.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/Nature_PineTree_RoseBush_Weeds.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model13 object.
	result = m_Model[13]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Weeds1.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/Nature_PineTree_RoseBush_Weeds.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model14 object.
	result = m_Model[14]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Mushrooms1.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/Nature_Mushrooms.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	/////////////////
	// SciFiCrates //
	/////////////////

	// Initialize the model13 object.
	result = m_Model[15]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/SciFiCrates.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/crateTexture.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	for (int i = 1; i < 15; i++)
	{
		m_Model[i]->MakeAABB(XMMatrixIdentity());
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model objects.
	for (int i = 0; i < MODELNUM; i++)
	{
		m_Model[i]->Shutdown();
		delete m_Model[i];
		m_Model[i] = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		for (int i = 0; i < LIGHTNUM; i++)
		{
			delete m_Light[i];
			m_Light[i] = 0;
		}
	}

	// Release the light objects.
	if (m_PointLight0)
	{
		delete m_PointLight0;
		m_PointLight0 = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the bitmap shader object.
	if (m_BitmapShader)
	{
		m_BitmapShader->Shutdown();
		delete m_BitmapShader;
		m_BitmapShader = 0;
	}

	// Release the bitmap objects.
	for (int i = 0; i < BITMAPNUM; i++)
	{
		if (m_Bitmap)
		{
			m_Bitmap[i]->Shutdown();
			delete m_Bitmap[i];
			m_Bitmap[i] = 0;
		}
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Skybox)
	{
		m_Skybox->Shutdown();
		delete m_Skybox;
		m_Skybox = 0;
	}

	if (m_Raycast)
	{
		delete m_Raycast;
		m_Raycast = 0;
	}

	if (m_Md5Model)
	{
		delete m_Md5Model;
		m_Md5Model = 0;
	}

	// Release the shader manager object.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(int fps, int cpu, int obj, int poly, int screenX, int screenY, float frameTime, DIMOUSESTATE mouseState)
{
	bool result;
	static float rotation = 0.0f;

	if (!checkFirst)
	{
		m_obj = obj;
		m_poly = poly;
		
		checkFirst = true;
	}

	//m_Camera->TickUpdate();

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.001f * frameTime;
	if (rotation > 2 * (float)D3DX_PI)
	{
		rotation -= 2 * (float)D3DX_PI;
	}

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetObject(m_obj, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetPolygon(m_poly, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetScreenSize(screenX, screenY, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetCameraPos(m_Camera->GetPosition(), m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetCameraRot(m_Camera->GetRotation(), m_D3D->GetDeviceContext());
	if (!result) return false;

	m_Camera->TickUpdate();

	// Render the graphics scene.
	result = Render(rotation, mouseState, frameTime);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation, DIMOUSESTATE mouseState, float frameTime)
{
	D3DXMATRIX worldMatrix[MODELNUM], viewMatrix, projectionMatrix, translateMatrix, scaleMatrix;
	D3DXMATRIX insWorld[100];
	D3DXMATRIX bitmapWorld[BITMAPNUM];
	D3DXMATRIX orthoMatrix;
	D3DXVECTOR4 diffuseColor[1];
	D3DXVECTOR4 lightPosition[1];
	bool result;

	// Create the diffuse color array from the four light colors. (pointlight)
	diffuseColor[0] = m_PointLight0->GetDiffuseColor();

	// Create the light position array from the four light positions. (pointlight)
	lightPosition[0] = m_PointLight0->GetPosition();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	///////////////////
	// CREATE SKYBOX //
	///////////////////

	D3DXMATRIX sphereWorld;
	D3DXMATRIX Scale;
	D3DXMATRIX Translation;

	D3DXMATRIX viewMatrixTmp;
	D3DXMATRIX projectionMatrixTmp;

	//Reset sphereWorld
	D3DXMatrixIdentity(&sphereWorld);
	//Define sphereWorld's world space matrix
	D3DXMatrixScaling(&Scale, 5.0f, 5.0f, 5.0f);
	//Make sure the sphere is always centered around camera
	D3DXMatrixTranslation(&Translation, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	//Set sphereWorld's world space using the transformations
	sphereWorld = Scale * Translation;

	m_Camera->GetViewMatrix(viewMatrixTmp);
	m_D3D->GetProjectionMatrix(projectionMatrixTmp);

	// return Cullmode from D3D11_CULL_NONE to D3D11_CULL_BACK
	m_Skybox->RenderSkyboxShader(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), sphereWorld, (D3DXMATRIX)viewMatrixTmp, (D3DXMATRIX)projectionMatrixTmp);
	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RasterizerState* RSCullBack;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.CullMode = D3D11_CULL_BACK;
	m_D3D->GetDevice()->CreateRasterizerState(&rasterDesc, &RSCullBack);


	/////////////////////
	// RENDER MD5MODEL //
	/////////////////////

	//////////////////
	// Nuclear Suit //
	//////////////////

	D3DXMATRIX md5World;
	//D3DXMATRIX rotationMatrix;
	//D3DXMATRIX inverseMat;
	D3DXVECTOR3 trans;
	//XMFLOAT4X4 convertMat;
	//D3DXMATRIX inputWorld, inputView, inputProj;
	//float camCharDistance;

	m_D3D->GetWorldMatrix(md5World);
	D3DXVec3Scale
	(
		&trans,
		&D3DXVECTOR3
		(
			m_Camera->GetLookAtVector().x * 10.0f,
			m_Camera->GetLookAtVector().y * 10.0f,
			m_Camera->GetLookAtVector().z * 10.0f
		),
		10.0f
	);

	m_charPos = trans;

	D3DXVec3Add(&trans, &m_Camera->GetPosition(), &trans);
	D3DXMatrixRotationY(&md5World, m_charRot.y + 180.0f * 0.0174532925f);
	D3DXMatrixTranslation(&translateMatrix, trans.x, 20.0f, trans.z);
	D3DXMatrixMultiply(&md5World, &md5World, &translateMatrix);
	
	/*if(m_Camera->GetRotation().y != m_oldCamRot)
		m_Camera->SetPosition
		(
			m_Camera->GetPosition().x,
			m_Camera->GetPosition().y - sin(m_Camera->GetRotation().y) * 10.0f,
			m_Camera->GetPosition().z
		);
	m_oldCamRot = m_Camera->GetRotation().y;*/

	/*convertMat = XMFLOAT4X4(md5World);
		XMFLOAT4X4
	{
		md5World._11, md5World._12, md5World._13, md5World._14,
		md5World._21, md5World._22, md5World._23, md5World._24,
		md5World._31, md5World._32, md5World._33, md5World._34,
		md5World._41, md5World._42, md5World._43, md5World._44
	};
	inputWorld = XMLoadFloat4x4(&convertMat);

	convertMat = XMFLOAT4X4(viewMatrix);
		XMFLOAT4X4
	{
		viewMatrix._11, viewMatrix._12, viewMatrix._13, viewMatrix._14,
		viewMatrix._21, viewMatrix._22, viewMatrix._23, viewMatrix._24,
		viewMatrix._31, viewMatrix._32, viewMatrix._33, viewMatrix._34,
		viewMatrix._41, viewMatrix._42, viewMatrix._43, viewMatrix._44
	};
	inputView = XMLoadFloat4x4(&convertMat);

	convertMat = XMFLOAT4X4(projectionMatrix);
		XMFLOAT4X4
	{
		projectionMatrix._11, projectionMatrix._12, projectionMatrix._13, projectionMatrix._14,
		projectionMatrix._21, projectionMatrix._22, projectionMatrix._23, projectionMatrix._24,
		projectionMatrix._31, projectionMatrix._32, projectionMatrix._33, projectionMatrix._34,
		projectionMatrix._41, projectionMatrix._42, projectionMatrix._43, projectionMatrix._44
	};
	inputProj = XMLoadFloat4x4(&convertMat);*/

	m_Md5Model->DrawMd5Model(m_D3D->GetDeviceContext(), md5World, viewMatrix, projectionMatrix);



	///////////////////////
	// MODEL POSITIONING //
	///////////////////////

	///////////
	// Floor //
	///////////

	// Floor
	m_D3D->GetWorldMatrix(worldMatrix[0]);
	D3DXMatrixRotationY(&worldMatrix[0], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[0], &worldMatrix[0], &translateMatrix);
	m_Model[0]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[0]->GetIndexCount(), worldMatrix[0], viewMatrix, projectionMatrix,
		m_Model[0]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	/////////////////
	// Environment //
	/////////////////

	// Rock1
	m_D3D->GetWorldMatrix(worldMatrix[1]);
	D3DXMatrixRotationY(&worldMatrix[1], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[1], &worldMatrix[1], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[1], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rock2
	m_D3D->GetWorldMatrix(worldMatrix[2]);
	D3DXMatrixRotationY(&worldMatrix[2], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[2], &worldMatrix[2], &translateMatrix);
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix[2], viewMatrix, projectionMatrix,
		m_Model[2]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rock3
	m_D3D->GetWorldMatrix(worldMatrix[3]);
	D3DXMatrixRotationY(&worldMatrix[3], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[3], &worldMatrix[3], &translateMatrix);
	m_Model[3]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[3]->GetIndexCount(), worldMatrix[3], viewMatrix, projectionMatrix,
		m_Model[3]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rock4
	m_D3D->GetWorldMatrix(worldMatrix[4]);
	D3DXMatrixRotationY(&worldMatrix[4], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[4], &worldMatrix[4], &translateMatrix);
	m_Model[4]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[4]->GetIndexCount(), worldMatrix[4], viewMatrix, projectionMatrix,
		m_Model[4]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rock5
	m_D3D->GetWorldMatrix(worldMatrix[5]);
	D3DXMatrixRotationY(&worldMatrix[5], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[5], &worldMatrix[5], &translateMatrix);
	m_Model[5]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[5]->GetIndexCount(), worldMatrix[5], viewMatrix, projectionMatrix,
		m_Model[5]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rock6
	m_D3D->GetWorldMatrix(worldMatrix[6]);
	D3DXMatrixRotationY(&worldMatrix[6], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[6], &worldMatrix[6], &translateMatrix);
	m_Model[6]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[6]->GetIndexCount(), worldMatrix[6], viewMatrix, projectionMatrix,
		m_Model[6]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Tree1
	m_D3D->GetWorldMatrix(worldMatrix[7]);
	D3DXMatrixRotationY(&worldMatrix[7], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[7], &worldMatrix[7], &translateMatrix);
	m_Model[7]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[7]->GetIndexCount(), worldMatrix[7], viewMatrix, projectionMatrix,
		m_Model[7]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// PineTree1
	m_D3D->GetWorldMatrix(worldMatrix[8]);
	D3DXMatrixRotationY(&worldMatrix[8], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[8], &worldMatrix[8], &translateMatrix);
	m_Model[8]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[8]->GetIndexCount(), worldMatrix[8], viewMatrix, projectionMatrix,
		m_Model[8]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// PineTree2
	m_D3D->GetWorldMatrix(worldMatrix[9]);
	D3DXMatrixRotationY(&worldMatrix[9], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[9], &worldMatrix[9], &translateMatrix);
	m_Model[9]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[9]->GetIndexCount(), worldMatrix[9], viewMatrix, projectionMatrix,
		m_Model[9]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// PineTree3
	m_D3D->GetWorldMatrix(worldMatrix[10]);
	D3DXMatrixRotationY(&worldMatrix[10], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[10], &worldMatrix[10], &translateMatrix);
	m_Model[10]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[10]->GetIndexCount(), worldMatrix[10], viewMatrix, projectionMatrix,
		m_Model[10]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// RoseBush1
	m_D3D->GetWorldMatrix(worldMatrix[11]);
	D3DXMatrixRotationY(&worldMatrix[11], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[11], &worldMatrix[11], &translateMatrix);
	m_Model[11]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[11]->GetIndexCount(), worldMatrix[11], viewMatrix, projectionMatrix,
		m_Model[11]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// RoseBush2
	m_D3D->GetWorldMatrix(worldMatrix[12]);
	D3DXMatrixRotationY(&worldMatrix[12], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[12], &worldMatrix[12], &translateMatrix);
	m_Model[12]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[12]->GetIndexCount(), worldMatrix[12], viewMatrix, projectionMatrix,
		m_Model[12]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Weeds
	m_D3D->GetWorldMatrix(worldMatrix[13]);
	D3DXMatrixRotationY(&worldMatrix[13], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[13], &worldMatrix[13], &translateMatrix);
	m_Model[13]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[13]->GetIndexCount(), worldMatrix[13], viewMatrix, projectionMatrix,
		m_Model[13]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Mushrooms
	m_D3D->GetWorldMatrix(worldMatrix[14]);
	D3DXMatrixRotationY(&worldMatrix[14], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[14], &worldMatrix[14], &translateMatrix);
	m_Model[14]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[12]->GetIndexCount(), worldMatrix[14], viewMatrix, projectionMatrix,
		m_Model[14]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}


	////////////////////////////
	//// Runtime Instance Obj //
	////////////////////////////
	
	/////////////////
	//  SciFiCrate //
	/////////////////

	if (mouseState.rgbButtons[0] && !isLClicked)
	{
		isLClicked = true;
		m_constructAnimTrigger = true;

		m_obj += 1;
		m_poly += 184;

		D3DXVECTOR3 tmpInsPos;
		D3DXVECTOR3 transVec;
		D3DXVec3Scale
		(
			&transVec,
			&D3DXVECTOR3
			(
				m_Camera->GetLookAtVector().x * 10.0f,
				m_Camera->GetLookAtVector().y * 10.0f,
				m_Camera->GetLookAtVector().z * 10.0f
			),
			15.0f
		);

		D3DXVec3Add(&tmpInsPos, &m_Camera->GetPosition(), &transVec);

		/*insPos.push_back(m_Raycast->doRaycast(m_hwnd, m_Model[0]->getVertices(), m_Model[0]->getIndices(), worldMatrix[1], projectionMatrix, viewMatrix));*/
		insPos.push_back(tmpInsPos);
		insRot.push_back(m_charRot.y);
		//insRot.push_back(m_Camera->GetLookAtVector().y);
	}
	
	if (mouseState.rgbButtons[1] && !isRClicked && !insPos.empty())
	{
		isRClicked = true;
		m_constructAnimTrigger = true;

		m_obj -= 1;
		m_poly -= 184;

		if(!insPos.empty())
			insPos.pop_back();
		if (!insRot.empty())
			insRot.pop_back();
	}

	if (m_constructAnimTrigger)
	{
		m_animTimeStack += frameTime * 0.005f;

		if (m_animTimeStack < m_Md5Model->getTotalAnimTime(1))
			AnimCharConstruct(frameTime);

		if (m_animTimeStack >= m_Md5Model->getTotalAnimTime(1))
		{
			m_animTimeStack = 0.0f;
			m_constructAnimTrigger = false;
		}
	}

	if (!mouseState.rgbButtons[0])
	{
		isLClicked = false;
	}

	if (!mouseState.rgbButtons[1])
	{
		isRClicked = false;
	}

	for (int i = 0; i < insPos.size(); i++)
	{
		m_D3D->GetWorldMatrix(insWorld[i]);
		D3DXMatrixRotationY(&insWorld[i], insRot[i]);
		//D3DXMatrixScaling(&scaleMatrix, 1.0f, 1.0f, 1.0f);
		//D3DXMatrixMultiply(&insMatrix, &insMatrix, &scaleMatrix);
		D3DXMatrixTranslation(&translateMatrix, insPos[i].x, /*insPos[i].y*/ 1.0f, insPos[i].z);
		D3DXMatrixMultiply(&insWorld[i], &insWorld[i], &translateMatrix);

		//m_Md5Model->MakeAABB((XMMATRIX)insWorld[i]);

		m_Model[15]->Render(m_D3D->GetDeviceContext());
		result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[15]->GetIndexCount(), insWorld[i], viewMatrix, projectionMatrix,
			m_Model[15]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
		if (!result)
		{
			return false;
		}
	}

	////////////////////
	// Text Rendering //
	////////////////////

	m_D3D->GetWorldMatrix(worldMatrix[49]);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 20.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[49], &worldMatrix[49], &translateMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix[49], orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Set the Start Camera Rotation.
	if (isStart)
	{
		m_Camera->SetRotation(20.0f, 0.0f, 0.0f);
		isStart = false;
	}


	//////////////////////
	// Bitmap Rendering //
	//////////////////////

	for(int i = 0; i < BITMAPNUM; i++)
		m_D3D->GetWorldMatrix(bitmapWorld[i]);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap(UI Construction container SciFi crate.png) vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap[0]->Render(m_D3D->GetDeviceContext(), 0, 773);
	if (!result)
	{
		return false;
	}

	// Render the bitmap(UI Construction container SciFi crate.png) with the texture shader.
	result = m_BitmapShader->Render(m_D3D->GetDeviceContext(), m_Bitmap[0]->GetIndexCount(), bitmapWorld[0], m_BaseViewMatrix, orthoMatrix, m_Bitmap[0]->GetTexture());
	if (!result)
	{
		return false;
	}

	// Put the bitmap(UI Inventory HUD with Jewels.png) vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap[1]->Render(m_D3D->GetDeviceContext(), 596, 972);
	if (!result)
	{
		return false;
	}

	// Render the bitmap(UI Inventory HUD with Jewels.png) with the texture shader.
	result = m_BitmapShader->Render(m_D3D->GetDeviceContext(), m_Bitmap[1]->GetIndexCount(), bitmapWorld[1], m_BaseViewMatrix, orthoMatrix, m_Bitmap[1]->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::MoveCameraAndChar(int key)
{
	D3DXVECTOR3 trans;
	D3DXMATRIX md5World;
	D3DXMATRIX translateMatrix;

	if (key == DIK_W)
	{
		m_Camera->FrontWalk(1.0f);

		D3DXMatrixIdentity(&md5World);

		D3DXVec3Scale
		(
			&trans,
			&D3DXVECTOR3
			(
				m_Camera->GetLookAtVector().x * 10.0f,
				m_Camera->GetLookAtVector().y * 10.0f,
				m_Camera->GetLookAtVector().z * 10.0f
			),
			10.0f
		);

		D3DXVec3Add(&trans, &m_Camera->GetPosition(), &trans);
		D3DXMatrixRotationY(&md5World, m_charRot.y + 180.0f * 0.0174532925f);
		D3DXMatrixTranslation(&translateMatrix, trans.x, 20.0f, trans.z);
		D3DXMatrixMultiply(&md5World, &md5World, &translateMatrix);

		m_Md5Model->MakeAABB((XMMATRIX)md5World);

		//charPos = m_Camera->GetPosition();
		for (int i = 0; i < 14; i++)
		{
			if (CollisionClass::BoundingBoxCollision(
				m_Md5Model->GetBoundingBoxMin(),
				m_Md5Model->GetBoundingBoxMax(),
				m_Model[i]->GetBoundingBoxMin(),
				m_Model[i]->GetBoundingBoxMax()))
			{
				m_Camera->FrontWalk(-2.0f);
			}
		}
	}
	if (key == DIK_S)
	{
		m_Camera->FrontWalk(-1.0f);

		D3DXMatrixIdentity(&md5World);

		D3DXVec3Scale
		(
			&trans,
			&D3DXVECTOR3
			(
				m_Camera->GetLookAtVector().x * 10.0f,
				m_Camera->GetLookAtVector().y * 10.0f,
				m_Camera->GetLookAtVector().z * 10.0f
			),
			10.0f
		);

		D3DXVec3Add(&trans, &m_Camera->GetPosition(), &trans);
		D3DXMatrixRotationY(&md5World, m_charRot.y + 180.0f * 0.0174532925f);
		D3DXMatrixTranslation(&translateMatrix, trans.x, 20.0f, trans.z);
		D3DXMatrixMultiply(&md5World, &md5World, &translateMatrix);

		m_Md5Model->MakeAABB((XMMATRIX)md5World);

		//charPos = m_Camera->GetPosition();
		for (int i = 0; i < 14; i++)
		{
			if (CollisionClass::BoundingBoxCollision(
				m_Md5Model->GetBoundingBoxMin(),
				m_Md5Model->GetBoundingBoxMax(),
				m_Model[i]->GetBoundingBoxMin(),
				m_Model[i]->GetBoundingBoxMax()))
			{
				m_Camera->FrontWalk(+2.0f);
			}
		}
	}

	if (key == DIK_A)
	{
		m_Camera->SideWalk(1.0f);

		D3DXMatrixIdentity(&md5World);

		D3DXVec3Scale
		(
			&trans,
			&D3DXVECTOR3
			(
				m_Camera->GetLookAtVector().x * 10.0f,
				m_Camera->GetLookAtVector().y * 10.0f,
				m_Camera->GetLookAtVector().z * 10.0f
			),
			10.0f
		);

		D3DXVec3Add(&trans, &m_Camera->GetPosition(), &trans);
		D3DXMatrixRotationY(&md5World, m_charRot.y + 180.0f * 0.0174532925f);
		D3DXMatrixTranslation(&translateMatrix, trans.x, 20.0f, trans.z);
		D3DXMatrixMultiply(&md5World, &md5World, &translateMatrix);

		m_Md5Model->MakeAABB((XMMATRIX)md5World);

		//charPos = m_Camera->GetPosition();
		for (int i = 0; i < 14; i++)
		{
			if (CollisionClass::BoundingBoxCollision(
				m_Md5Model->GetBoundingBoxMin(),
				m_Md5Model->GetBoundingBoxMax(),
				m_Model[i]->GetBoundingBoxMin(),
				m_Model[i]->GetBoundingBoxMax()))
			{
				m_Camera->SideWalk(-1.0f);
			}
		}
	}
	if (key == DIK_D)
	{
		m_Camera->SideWalk(-1.0f);

		D3DXMatrixIdentity(&md5World);

		D3DXVec3Scale
		(
			&trans,
			&D3DXVECTOR3
			(
				m_Camera->GetLookAtVector().x * 10.0f,
				m_Camera->GetLookAtVector().y * 10.0f,
				m_Camera->GetLookAtVector().z * 10.0f
			),
			10.0f
		);

		D3DXVec3Add(&trans, &m_Camera->GetPosition(), &trans);
		D3DXMatrixRotationY(&md5World, m_charRot.y + 180.0f * 0.0174532925f);
		D3DXMatrixTranslation(&translateMatrix, trans.x, 20.0f, trans.z);
		D3DXMatrixMultiply(&md5World, &md5World, &translateMatrix);

		m_Md5Model->MakeAABB((XMMATRIX)md5World);

		//charPos = m_Camera->GetPosition();
		for (int i = 0; i < 14; i++)
		{
			if (CollisionClass::BoundingBoxCollision(
				m_Md5Model->GetBoundingBoxMin(),
				m_Md5Model->GetBoundingBoxMax(),
				m_Model[i]->GetBoundingBoxMin(),
				m_Model[i]->GetBoundingBoxMax()))
			{
				m_Camera->SideWalk(+1.0f);
			}
		}
	}
}

void GraphicsClass::RotateCameraAndChar(float x, float y, float z)
{
	for (int i = 0; i < 14; i++)
	{
		if (CollisionClass::BoundingBoxCollision(
			m_Md5Model->GetBoundingBoxMin(),
			m_Md5Model->GetBoundingBoxMax(),
			m_Model[i]->GetBoundingBoxMin(),
			m_Model[i]->GetBoundingBoxMax()))
		{
			return;
		}
	}

	m_Camera->SetRotation(0.0f, y, z);
	m_charRot = m_Camera->GetRotation() * 0.0174532925f;

	
}

void GraphicsClass::MovePad(int key, float frametime)
{
	if (key == DIK_LEFT)
		if(m_movement > -150.0f)
			m_movement -= 0.5f * frametime;
	if (key == DIK_RIGHT)
		if(m_movement < 150.0f)
			m_movement += 0.5f * frametime;
}

void GraphicsClass::MovePadAI(float frametime)
{
	if (m_AImovement < -150.0f)
		plusMinus = 1;
	if (m_AImovement > 150.0f)
		plusMinus = -1;

	m_AImovement += 0.5f * plusMinus * frametime;	
}

void GraphicsClass::AnimCharWalk(float frametime)
{
	float timeFactor = 0.005f;	// You can speed up or slow down time by changing this
	m_Md5Model->UpdateMD5Model(frametime*timeFactor, 0, m_D3D->GetDeviceContext());
}

void GraphicsClass::AnimCharConstruct(float frametime)
{
	float timeFactor = 0.005f;	// You can speed up or slow down time by changing this
	m_Md5Model->UpdateMD5Model(frametime*timeFactor, 1, m_D3D->GetDeviceContext());
}