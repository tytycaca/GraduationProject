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

	m_movement = 0.0f;
	m_AImovement = 0.0f;
	plusMinus = 1;

	isStart = true;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

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
	m_Camera->SetPosition(0.0f, 30.0f, -100.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);


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
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
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

	///////////
	// Floor //
	///////////

	// Initialize the model0 object. (Floor.obj)
	result = m_Model[0]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Floor.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/grassTexture.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	/////////////////////
	// Trees and Rocks //
	/////////////////////

	// Initialize the model1 object.
	result = m_Model[1]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/SmallTree.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model2 object.
	result = m_Model[2]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Rocks.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model3 object.
	result = m_Model[3]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/SmallRock.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/NatureMap.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	/////////////
	// Pickaxe //
	/////////////

	// Initialize the model4 object.
	result = m_Model[4]->Initialize(m_D3D->GetDevice(), (char*)"../GraduationProject v3.0/Pickaxe.txt",
		(WCHAR*)L"../GraduationProject v3.0/Texture/Pickaxe.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
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

	result = m_Text->SetObject(obj, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetPolygon(poly, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetScreenSize(screenX, screenY, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetCameraPos(m_Camera->GetPosition(), m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetCameraRot(m_Camera->GetRotation(), m_D3D->GetDeviceContext());
	if (!result) return false;

	m_Camera->TickUpdate();
	
	// Render the graphics scene.
	result = Render(rotation, mouseState);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation, DIMOUSESTATE mouseState)
{
	D3DXMATRIX worldMatrix[MODELNUM], viewMatrix, projectionMatrix, translateMatrix, scaleMatrix;
	D3DXMATRIX insMatrix;
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
	m_D3D->GetWorldMatrix(worldMatrix[0]);
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


	///////////////////////
	// MODEL POSITIONING //
	///////////////////////

	///////////
	// Floor //
	///////////

	// Floor
	m_D3D->GetWorldMatrix(worldMatrix[1]);
	D3DXMatrixRotationY(&worldMatrix[1], 0.0f);
	//D3DXMatrixScaling(&scaleMatrix, 50.0f, 1.0f, 100.0f);
	//D3DXMatrixMultiply(&worldMatrix[1], &worldMatrix[1], &scaleMatrix);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[1], &worldMatrix[1], &translateMatrix);
	m_Model[0]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[0]->GetIndexCount(), worldMatrix[1], viewMatrix, projectionMatrix,
		m_Model[0]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}


	/////////////////
	// Environment //
	/////////////////

	// SmallTree
	m_D3D->GetWorldMatrix(worldMatrix[2]);
	D3DXMatrixRotationY(&worldMatrix[2], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, -500.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[2], &worldMatrix[2], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[2], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rocks
	m_D3D->GetWorldMatrix(worldMatrix[3]);
	D3DXMatrixRotationY(&worldMatrix[3], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 400.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[3], &worldMatrix[3], &translateMatrix);
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix[3], viewMatrix, projectionMatrix,
		m_Model[2]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// SmallTree
	m_D3D->GetWorldMatrix(worldMatrix[4]);
	D3DXMatrixRotationY(&worldMatrix[4], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, -200.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[4], &worldMatrix[4], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[4], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rocks
	m_D3D->GetWorldMatrix(worldMatrix[5]);
	D3DXMatrixRotationY(&worldMatrix[5], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 200.0f, 0.0f, -400.0f);
	D3DXMatrixMultiply(&worldMatrix[5], &worldMatrix[5], &translateMatrix);
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix[5], viewMatrix, projectionMatrix,
		m_Model[2]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// SmallTree
	m_D3D->GetWorldMatrix(worldMatrix[6]);
	D3DXMatrixRotationY(&worldMatrix[6], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, -100.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[6], &worldMatrix[6], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[6], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rocks
	m_D3D->GetWorldMatrix(worldMatrix[7]);
	D3DXMatrixRotationY(&worldMatrix[7], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 100.0f, 0.0f, 100.0f);
	D3DXMatrixMultiply(&worldMatrix[7], &worldMatrix[7], &translateMatrix);
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix[7], viewMatrix, projectionMatrix,
		m_Model[2]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// SmallTree
	m_D3D->GetWorldMatrix(worldMatrix[8]);
	D3DXMatrixRotationY(&worldMatrix[8], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix[8], &worldMatrix[8], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[8], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rocks
	m_D3D->GetWorldMatrix(worldMatrix[9]);
	D3DXMatrixRotationY(&worldMatrix[9], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 100.0f, 0.0f, 100.0f);
	D3DXMatrixMultiply(&worldMatrix[9], &worldMatrix[9], &translateMatrix);
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix[9], viewMatrix, projectionMatrix,
		m_Model[2]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// SmallTree
	m_D3D->GetWorldMatrix(worldMatrix[10]);
	D3DXMatrixRotationY(&worldMatrix[10], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, -100.0f, 0.0f, -300.0f);
	D3DXMatrixMultiply(&worldMatrix[10], &worldMatrix[10], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[10], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// SmallTree
	m_D3D->GetWorldMatrix(worldMatrix[11]);
	D3DXMatrixRotationY(&worldMatrix[11], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, -200.0f, 0.0f, -100.0f);
	D3DXMatrixMultiply(&worldMatrix[11], &worldMatrix[11], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[11], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// SmallTree
	m_D3D->GetWorldMatrix(worldMatrix[12]);
	D3DXMatrixRotationY(&worldMatrix[12], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 100.0f, 0.0f, 200.0f);
	D3DXMatrixMultiply(&worldMatrix[12], &worldMatrix[12], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[12], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// SmallTree
	m_D3D->GetWorldMatrix(worldMatrix[13]);
	D3DXMatrixRotationY(&worldMatrix[13], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 200.0f, 0.0f, -200.0f);
	D3DXMatrixMultiply(&worldMatrix[13], &worldMatrix[13], &translateMatrix);
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[13], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rocks
	m_D3D->GetWorldMatrix(worldMatrix[14]);
	D3DXMatrixRotationY(&worldMatrix[14], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 400.0f, 0.0f, -100.0f);
	D3DXMatrixMultiply(&worldMatrix[14], &worldMatrix[14], &translateMatrix);
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix[14], viewMatrix, projectionMatrix,
		m_Model[2]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rocks
	m_D3D->GetWorldMatrix(worldMatrix[15]);
	D3DXMatrixRotationY(&worldMatrix[15], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, 300.0f, 0.0f, -200.0f);
	D3DXMatrixMultiply(&worldMatrix[15], &worldMatrix[15], &translateMatrix);
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix[15], viewMatrix, projectionMatrix,
		m_Model[2]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rocks
	m_D3D->GetWorldMatrix(worldMatrix[16]);
	D3DXMatrixRotationY(&worldMatrix[16], 0.0f);
	D3DXMatrixTranslation(&translateMatrix, -200.0f, 0.0f, -100.0f);
	D3DXMatrixMultiply(&worldMatrix[16], &worldMatrix[16], &translateMatrix);
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix[16], viewMatrix, projectionMatrix,
		m_Model[2]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	


	///////////////
	//// Pickaxe //
	///////////////

	//
	//m_D3D->GetWorldMatrix(worldMatrix[4]);
	//D3DXMatrixRotationY(&worldMatrix[4], 0.0f);
	////D3DXMatrixScaling(&scaleMatrix, 500.0f, 500.0f, 500.0f);
	////D3DXMatrixMultiply(&worldMatrix[4], &worldMatrix[4], &scaleMatrix);

	//D3DXVECTOR3 InsPos;
	//D3DXVECTOR3 trans;
	//D3DXVec3Scale
	//(
	//	&trans,
	//	&D3DXVECTOR3
	//	(
	//		m_Camera->GetLookAtVector().x * 20.0f,
	//		m_Camera->GetLookAtVector().y * 20.0f,
	//		m_Camera->GetLookAtVector().z * 20.0f
	//	),
	//	10.0f
	//);
	//D3DXVec3Add(&InsPos, &m_Camera->GetPosition(), &trans);

	//D3DXMatrixTranslation(&translateMatrix, trans.x, trans.y, trans.z);
	//D3DXMatrixMultiply(&worldMatrix[4], &worldMatrix[4], &translateMatrix);
	//m_Model[3]->Render(m_D3D->GetDeviceContext());
	//result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[3]->GetIndexCount(), worldMatrix[4], viewMatrix, projectionMatrix,
	//	m_Model[3]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
	//	m_Camera->GetPosition(), m_Light[0]->GetSpecularColor(), m_Light[0]->GetSpecularPower());
	//if (!result)
	//{
	//	return false;
	//}


	////////////////////////////
	//// Runtime Instance Obj //
	////////////////////////////
	
	if (mouseState.rgbButtons[0] & 0x80)
	{
		D3DXVECTOR3 tmpInsPos;
		D3DXVECTOR3 transVec;
		D3DXVec3Scale
		(
			&transVec,
			&D3DXVECTOR3
			(
				m_Camera->GetLookAtVector().x * 20.0f,
				m_Camera->GetLookAtVector().y * 20.0f,
				m_Camera->GetLookAtVector().z * 20.0f
			),
			5.0f
		);
		D3DXVec3Add(&tmpInsPos, &m_Camera->GetPosition(), &transVec);
		insPos.push_back(tmpInsPos);
		//insRot.push_back(m_Camera->GetLookAtVector().y);
	}

	if (mouseState.rgbButtons[1] & 0x80)
	{
		if(!insPos.empty())
			insPos.pop_back();
		//if (!insRot.empty())
		//	insRot.pop_back();
	}

	for (int i = 0; i < insPos.size(); i++)
	{
		m_D3D->GetWorldMatrix(insMatrix);
		//D3DXMatrixRotationY(&insMatrix, insRot[i]);
		//D3DXMatrixScaling(&scaleMatrix, 1.0f, 1.0f, 1.0f);
		//D3DXMatrixMultiply(&insMatrix, &insMatrix, &scaleMatrix);
		D3DXMatrixTranslation(&translateMatrix, insPos[i].x, insPos[i].y, insPos[i].z);
		D3DXMatrixMultiply(&insMatrix, &insMatrix, &translateMatrix);
		m_Model[3]->Render(m_D3D->GetDeviceContext());
		result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model[3]->GetIndexCount(), insMatrix, viewMatrix, projectionMatrix,
			m_Model[3]->GetTexture(), m_Light[0]->GetDirection(), m_Light[0]->GetAmbientColor(), m_Light[0]->GetDiffuseColor(),
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

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::MoveCamera(int key)
{
	if (key == DIK_W) m_Camera->FrontWalk(1.0f);
	if (key == DIK_S) m_Camera->FrontWalk(-1.0f);

	if (key == DIK_A) m_Camera->SideWalk(1.0f);
	if (key == DIK_D) m_Camera->SideWalk(-1.0f);
}

void GraphicsClass::RotateCamera(float x, float y, float z)
{

	m_Camera->SetRotation(x, y, z);

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