////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYBOXCLASS_H_
#define _SKYBOXCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>
#include <dinput.h>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyboxClass
////////////////////////////////////////////////////////////////////////////////
class SkyboxClass
{
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		D3DXVECTOR3 pos;
		D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;
	};

	struct cbPerObject
	{
		D3DXMATRIX  WVP;
		D3DXMATRIX World;
	};


	/*ID3D11Device* d3d11Device;*/

	D3DXMATRIX Rotationx;
	D3DXMATRIX Rotationy;
	D3DXMATRIX Rotationz;

	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;
	ID3D11Buffer* cbPerObjectBuffer;

	ID3D11SamplerState* CubesTexSamplerState;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int NumSphereVertices;
	int NumSphereFaces;

	HRESULT hr;

public:
	void CreateSphere(ID3D11Device* , int, int);
	void InitializeSkyboxShader(ID3D11Device*);
	void RenderSkyboxShader(ID3D11Device*, ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void Shutdown();
};


#endif