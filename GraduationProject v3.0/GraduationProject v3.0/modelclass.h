////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include <vector>

#include <xnamath.h>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

#include "collisionclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	bool InitializeFire(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();

	vector<D3DXVECTOR3> getVertices();
	vector<DWORD> getIndices();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	bool LoadFireTextures(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture, *m_Texture1, *m_Texture2, *m_Texture3;
	ModelType* m_model;
	
	vector<D3DXVECTOR3> vertexOut;
	vector<DWORD> indexOut;

	// boundingBoxVolume
	vector<XMFLOAT3> modelVerts;
	vector<XMFLOAT3> boundingBoxVerts;
	vector<DWORD> boundingBoxVertIndex;
	float boundingSphere;
	XMVECTOR centerOffset;

	XMVECTOR boundingBoxMin;
	XMVECTOR boundingBoxMax;

public:
	void MakeAABB(XMMATRIX);
	XMVECTOR GetBoundingBoxMin();
	void SetBoundingBoxMin(XMVECTOR);
	XMVECTOR GetBoundingBoxMax();
	void SetBoundingBoxMax(XMVECTOR);
	XMMATRIX GetWorldMatrix();
};

#endif