////////////////////////////////////////////////////////////////////////////////
// Filename: md5modelclass.h
////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <sstream>
#include <fstream>
#include <istream>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "timerclass.h"
#include "xnamath.h"
#include "D3DX11tex.h"


/////////////
// GLOBALS //
/////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: Md5ModelClass
////////////////////////////////////////////////////////////////////////////////

class Md5ModelClass
{
private:
	struct Weight
	{
		int jointID;
		float bias;
		XMFLOAT3 pos;
		///////////////**************new**************////////////////////
		XMFLOAT3 normal;
		///////////////**************new**************////////////////////
	};

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz,
			float tx, float ty, float tz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz),
			tangent(tx, ty, tz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 biTangent;

		// Will not be sent to shader
		int StartWeight;
		int WeightCount;
	};

	struct ModelSubset
	{
		int texArrayIndex;
		int numTriangles;

		std::vector<Vertex> vertices;
		std::vector<XMFLOAT3> jointSpaceNormals;
		std::vector<DWORD> indices;
		std::vector<Weight> weights;

		std::vector<XMFLOAT3> positions;

		ID3D11Buffer* vertBuff;
		ID3D11Buffer* indexBuff;
	};

	struct Joint
	{
		std::wstring name;
		int parentID;

		XMFLOAT3 pos;
		XMFLOAT4 orientation;
	};

	struct AnimJointInfo
	{
		std::wstring name;
		int parentID;

		int flags;
		int startIndex;
	};

	struct BoundingBox
	{
		XMFLOAT3 min;
		XMFLOAT3 max;
	};

	struct FrameData
	{
		int frameID;
		std::vector<float> frameData;
	};

	struct ModelAnimation
	{
		int numFrames;
		int numJoints;
		int frameRate;
		int numAnimatedComponents;

		float frameTime;
		float totalAnimTime;
		float currAnimTime;

		std::vector<AnimJointInfo> jointInfo;
		std::vector<BoundingBox> frameBounds;
		std::vector<Joint>	baseFrameJoints;
		std::vector<FrameData>	frameData;
		std::vector<std::vector<Joint>> frameSkeleton;
	};

	struct Model3D
	{
		int numSubsets;
		int numJoints;

		std::vector<Joint> joints;
		std::vector<ModelSubset> subsets;

		///////////////**************new**************////////////////////
		std::vector<ModelAnimation> animations;
		///////////////**************new**************////////////////////
	};

	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(Light));
		}
		XMFLOAT3 pos;
		float range;
		XMFLOAT3 dir;
		float cone;
		XMFLOAT3 att;
		float pad2;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
	};

	D3D11_INPUT_ELEMENT_DESC layout[4] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//Create effects constant buffer's structure//
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;

		//These will be used for the pixel shader
		XMFLOAT4 difColor;
		BOOL hasTexture;
		//Because of HLSL structure packing, we will use windows BOOL
		//instead of bool because HLSL packs things into 4 bytes, and
		//bool is only one byte, where BOOL is 4 bytes
		BOOL hasNormMap;
	};

	struct cbPerFrame
	{
		Light  light;
	};

	struct SurfaceMaterial
	{
		std::wstring matName;
		XMFLOAT4 difColor;
		int texArrayIndex;
		int normMapTexArrayIndex;
		bool hasNormMap;
		bool hasTexture;
		bool transparent;
	};

public:
	Md5ModelClass();
	Md5ModelClass(const Md5ModelClass&);
	~Md5ModelClass();

	bool InitializeMd5Model(std::wstring, std::wstring, ID3D11Device*);

	void InitializeMd5Shader(ID3D11Device*, ID3D11DeviceContext*, int, int);

	//LoadMD5Model() function prototype
	bool LoadMD5Model
	(
		std::wstring filename,
		Model3D& MD5Model,
		std::vector<ID3D11ShaderResourceView*>& shaderResourceViewArray,
		std::vector<std::wstring> texFileNameArray,
		ID3D11Device*
	);

	bool LoadMD5Anim(std::wstring filename, Model3D& MD5Model);

	void UpdateMD5Model(float deltaTime, int animation, ID3D11DeviceContext*);

	void DrawMd5Model(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);


private:
	Light light;

	ID3D11InputLayout* vertLayout;
	UINT numElements;
	cbPerObject cbPerObj;
	ID3D11Buffer* cbPerObjectBuffer;
	cbPerFrame constbuffPerFrame;
	ID3D11Buffer* cbPerFrameBuffer;
	ID3D11BlendState* d2dTransparency;
	ID3D11BlendState* Transparency;
	ID3D11SamplerState* CubesTexSamplerState;
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;
	ID3D11RasterizerState* RSCullNone;
	ID3D11DepthStencilState* DSLessEqual;

	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;

	HRESULT hr;

	std::vector<SurfaceMaterial> material;

	Model3D MD5Model;
	std::vector<ID3D11ShaderResourceView*> meshSRV;
	std::vector<std::wstring> textureNameArray;

	//Mesh variables. Each loaded mesh will need its own set of these
	ID3D11Buffer* meshVertBuff;
	ID3D11Buffer* meshIndexBuff;
	//XMMATRIX meshWorld;
	int meshSubsets;
	std::vector<int> meshSubsetIndexStart;
	std::vector<int> meshSubsetTexture;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;
	XMMATRIX WVP;
	XMMATRIX camView;
	XMMATRIX camProjection;
};