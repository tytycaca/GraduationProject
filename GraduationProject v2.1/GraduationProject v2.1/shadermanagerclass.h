////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "pointlightshaderclass.h"
#include "bumpmapshaderclass.h"

#include "fireshaderclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderManagerClass
////////////////////////////////////////////////////////////////////////////////
class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderTextureShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

	bool RenderFireTextureShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, float, float);

	bool RenderLightShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, 
						   D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float);

	bool RenderPointLightShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4[],
		D3DXVECTOR4[]);

	bool RenderBumpMapShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, 
							 ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4);

private:
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	PointLightShaderClass* m_PonitLightShader;
	BumpMapShaderClass* m_BumpMapShader;
	FireShaderClass* m_FireShader;
};

#endif