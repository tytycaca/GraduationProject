////////////////////////////////////////////////////////////////////////////////
// Filename: md5modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "md5modelclass.h"


Md5ModelClass::Md5ModelClass()
{
	int meshSubsets = 0;

	UINT numElements = ARRAYSIZE(layout);
}


Md5ModelClass::Md5ModelClass(const Md5ModelClass& other)
{

}


Md5ModelClass::~Md5ModelClass()
{

}


void Md5ModelClass::InitializeMd5Shader
(
	ID3D11Device* d3d11Device,
	ID3D11DeviceContext* d3d11DevCon,
	int screenWidth,
	int screenHeight
)
{
	//Compile Shaders from shader file
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);

	//Create the Shader Objects
	hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	//Set Vertex and Pixel Shaders
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

	light.pos = D3DXVECTOR3(0.0f, 7.0f, 0.0f);
	light.dir = D3DXVECTOR3(-0.5f, 0.75f, -0.5f);
	light.range = 1000.0f;
	light.cone = 12.0f;
	light.att = D3DXVECTOR3(0.4f, 0.02f, 0.000f);
	light.ambient = D3DXQUATERNION(1.0f, 1.0f, 1.0f, 1.0f);
	light.diffuse = D3DXQUATERNION(1.0f, 1.0f, 1.0f, 1.0f);

	//Create the Input Layout
	hr = d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	d3d11DevCon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	d3d11DevCon->RSSetViewports(1, &viewport);

	//Create the buffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	//Create the buffer to send to the cbuffer per frame in effect file
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	d3d11Device->CreateBlendState(&blendDesc, &d2dTransparency);

	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	d3d11Device->CreateBlendState(&blendDesc, &Transparency);

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = d3d11Device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);

	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;

	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CWcullMode);

	//cmdesc.CullMode = D3D11_CULL_NONE;
	//cmdesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	d3d11Device->CreateDepthStencilState(&dssDesc, &DSLessEqual);
}


bool Md5ModelClass::InitializeMd5Model(std::wstring filename1, std::wstring filename2, std::wstring filename3, ID3D11Device* d3d11Device)
{
	if (!LoadMD5Model(filename1, MD5Model, meshSRV, textureNameArray, d3d11Device))
		return false;
	///////////////**************new**************////////////////////	
	if (!LoadMD5Anim(filename2, MD5Model))
		return false;
	if (!LoadMD5Anim(filename3, MD5Model))
		return false;
	///////////////**************new**************////////////////////
}


bool Md5ModelClass::LoadMD5Model
(
	std::wstring filename,
	Model3D& MD5Model,
	std::vector<ID3D11ShaderResourceView*>& shaderResourceViewArray,
	std::vector<std::wstring> texFileNameArray,
	ID3D11Device* d3d11Device
)
{
	std::wifstream fileIn(filename.c_str());		// Open file

	std::wstring checkString;						// Stores the next string from our file

	if (fileIn)										// Check if the file was opened
	{
		while (fileIn)								// Loop until the end of the file is reached
		{
			fileIn >> checkString;					// Get next string from file

			if (checkString == L"MD5Version")		// Get MD5 version (this function supports version 10)
			{
				/*fileIn >> checkString;
				MessageBox(0, checkString.c_str(),	//display message
				L"MD5Version", MB_OK);*/
			}
			else if (checkString == L"commandline")
			{
				std::getline(fileIn, checkString);	// Ignore the rest of this line
			}
			else if (checkString == L"numJoints")
			{
				fileIn >> MD5Model.numJoints;		// Store number of joints
			}
			else if (checkString == L"numMeshes")
			{
				fileIn >> MD5Model.numSubsets;		// Store number of meshes or subsets which we will call them
			}
			else if (checkString == L"joints")
			{
				Joint tempJoint;

				fileIn >> checkString;				// Skip the "{"

				for (int i = 0; i < MD5Model.numJoints; i++)
				{
					fileIn >> tempJoint.name;		// Store joints name
					// Sometimes the names might contain spaces. If that is the case, we need to continue
					// to read the name until we get to the closing " (quotation marks)
					if (tempJoint.name[tempJoint.name.size() - 1] != '"')
					{
						wchar_t checkChar;
						bool jointNameFound = false;
						while (!jointNameFound)
						{
							checkChar = fileIn.get();

							if (checkChar == '"')
								jointNameFound = true;

							tempJoint.name += checkChar;
						}
					}

					fileIn >> tempJoint.parentID;	// Store Parent joint's ID

					fileIn >> checkString;			// Skip the "("

					// Store position of this joint (swap y and z axis if model was made in RH Coord Sys)
					fileIn >> tempJoint.pos.x >> tempJoint.pos.z >> tempJoint.pos.y;

					fileIn >> checkString >> checkString;	// Skip the ")" and "("

					// Store orientation of this joint
					fileIn >> tempJoint.orientation.x >> tempJoint.orientation.z >> tempJoint.orientation.y;

					// Remove the quotation marks from joints name
					tempJoint.name.erase(0, 1);
					tempJoint.name.erase(tempJoint.name.size() - 1, 1);

					// Compute the w axis of the quaternion (The MD5 model uses a 3D vector to describe the
					// direction the bone is facing. However, we need to turn this into a quaternion, and the way
					// quaternions work, is the xyz values describe the axis of rotation, while the w is a value
					// between 0 and 1 which describes the angle of rotation)
					float t = 1.0f - (tempJoint.orientation.x * tempJoint.orientation.x)
						- (tempJoint.orientation.y * tempJoint.orientation.y)
						- (tempJoint.orientation.z * tempJoint.orientation.z);
					if (t < 0.0f)
					{
						tempJoint.orientation.w = 0.0f;
					}
					else
					{
						tempJoint.orientation.w = -sqrtf(t);
					}

					std::getline(fileIn, checkString);		// Skip rest of this line

					MD5Model.joints.push_back(tempJoint);	// Store the joint into this models joint vector

					//aabbÁ¦ÀÛ
					XMFLOAT3 tempXm;
					tempXm.x = tempJoint.pos.x;
					tempXm.y = tempJoint.pos.y;
					tempXm.z = tempJoint.pos.z;
					modelVerts.push_back(tempXm);
				}

				fileIn >> checkString;					// Skip the "}"
			}
			else if (checkString == L"mesh")
			{
				ModelSubset subset;
				int numVerts, numTris, numWeights;

				fileIn >> checkString;					// Skip the "{"

				fileIn >> checkString;
				while (checkString != L"}")			// Read until '}'
				{
					// In this lesson, for the sake of simplicity, we will assume a textures filename is givin here.
					// Usually though, the name of a material (stored in a material library. Think back to the lesson on
					// loading .obj files, where the material library was contained in the file .mtl) is givin. Let this
					// be an exercise to load the material from a material library such as obj's .mtl file, instead of
					// just the texture like we will do here.
					if (checkString == L"shader")		// Load the texture or material
					{
						std::wstring fileNamePath;
						fileIn >> fileNamePath;			// Get texture's filename

						// Take spaces into account if filename or material name has a space in it
						if (fileNamePath[fileNamePath.size() - 1] != '"')
						{
							wchar_t checkChar;
							bool fileNameFound = false;
							while (!fileNameFound)
							{
								checkChar = fileIn.get();

								if (checkChar == '"')
									fileNameFound = true;

								fileNamePath += checkChar;
							}
						}

						// Remove the quotation marks from texture path
						fileNamePath.erase(0, 1);
						fileNamePath.erase(fileNamePath.size() - 1, 1);

						//check if this texture has already been loaded
						bool alreadyLoaded = false;
						for (int i = 0; i < texFileNameArray.size(); ++i)
						{
							if (fileNamePath == texFileNameArray[i])
							{
								alreadyLoaded = true;
								subset.texArrayIndex = i;
							}
						}

						//if the texture is not already loaded, load it now
						if (!alreadyLoaded)
						{
							ID3D11ShaderResourceView* tempMeshSRV;
							hr = D3DX11CreateShaderResourceViewFromFile(d3d11Device, fileNamePath.c_str(),
								NULL, NULL, &tempMeshSRV, NULL);
							if (SUCCEEDED(hr))
							{
								texFileNameArray.push_back(fileNamePath.c_str());
								subset.texArrayIndex = shaderResourceViewArray.size();
								shaderResourceViewArray.push_back(tempMeshSRV);
							}
							else
							{
								MessageBox(0, fileNamePath.c_str(),		//display message
									L"Could Not Open:", MB_OK);
								return false;
							}
						}

						std::getline(fileIn, checkString);				// Skip rest of this line
					}
					else if (checkString == L"numverts")
					{
						fileIn >> numVerts;								// Store number of vertices

						std::getline(fileIn, checkString);				// Skip rest of this line

						for (int i = 0; i < numVerts; i++)
						{
							Vertex tempVert;

							fileIn >> checkString						// Skip "vert # ("
								>> checkString
								>> checkString;

							fileIn >> tempVert.texCoord.x				// Store tex coords
								>> tempVert.texCoord.y;

							fileIn >> checkString;						// Skip ")"

							fileIn >> tempVert.StartWeight;				// Index of first weight this vert will be weighted to

							fileIn >> tempVert.WeightCount;				// Number of weights for this vertex

							std::getline(fileIn, checkString);			// Skip rest of this line

							subset.vertices.push_back(tempVert);		// Push back this vertex into subsets vertex vector
						}
					}
					else if (checkString == L"numtris")
					{
						fileIn >> numTris;
						subset.numTriangles = numTris;

						std::getline(fileIn, checkString);				// Skip rest of this line

						for (int i = 0; i < numTris; i++)				// Loop through each triangle
						{
							DWORD tempIndex;
							fileIn >> checkString;						// Skip "tri"
							fileIn >> checkString;						// Skip tri counter

							for (int k = 0; k < 3; k++)					// Store the 3 indices
							{
								fileIn >> tempIndex;
								subset.indices.push_back(tempIndex);
							}

							std::getline(fileIn, checkString);			// Skip rest of this line
						}
					}
					else if (checkString == L"numweights")
					{
						fileIn >> numWeights;

						std::getline(fileIn, checkString);				// Skip rest of this line

						for (int i = 0; i < numWeights; i++)
						{
							Weight tempWeight;
							fileIn >> checkString >> checkString;		// Skip "weight #"

							fileIn >> tempWeight.jointID;				// Store weight's joint ID

							fileIn >> tempWeight.bias;					// Store weight's influence over a vertex

							fileIn >> checkString;						// Skip "("

							fileIn >> tempWeight.pos.x					// Store weight's pos in joint's local space
								>> tempWeight.pos.z
								>> tempWeight.pos.y;

							std::getline(fileIn, checkString);			// Skip rest of this line

							subset.weights.push_back(tempWeight);		// Push back tempWeight into subsets Weight array
						}

					}
					else
						std::getline(fileIn, checkString);				// Skip anything else

					fileIn >> checkString;								// Skip "}"
				}

				//*** find each vertex's position using the joints and weights ***//
				for (int i = 0; i < subset.vertices.size(); ++i)
				{
					Vertex tempVert = subset.vertices[i];
					tempVert.pos = D3DXVECTOR3(0, 0, 0);	// Make sure the vertex's pos is cleared first

					// Sum up the joints and weights information to get vertex's position
					for (int j = 0; j < tempVert.WeightCount; ++j)
					{
						Weight tempWeight = subset.weights[tempVert.StartWeight + j];
						Joint tempJoint = MD5Model.joints[tempWeight.jointID];

						// Convert joint orientation and weight pos to vectors for easier computation
						// When converting a 3d vector to a quaternion, you should put 0 for "w", and
						// When converting a quaternion to a 3d vector, you can just ignore the "w"
						D3DXQUATERNION tempJointOrientation = D3DXQUATERNION(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
						D3DXQUATERNION tempWeightPos = D3DXQUATERNION(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

						// We will need to use the conjugate of the joint orientation quaternion
						// To get the conjugate of a quaternion, all you have to do is inverse the x, y, and z
						D3DXQUATERNION tempJointOrientationConjugate = D3DXQUATERNION(-tempJoint.orientation.x, -tempJoint.orientation.y, -tempJoint.orientation.z, tempJoint.orientation.w);

						// Calculate vertex position (in joint space, eg. rotate the point around (0,0,0)) for this weight using the joint orientation quaternion and its conjugate
						// We can rotate a point using a quaternion with the equation "rotatedPoint = quaternion * point * quaternionConjugate"
						D3DXVECTOR3 rotatedPoint;
						D3DXQUATERNION result;
						D3DXQuaternionMultiply
						(
							&result,
							D3DXQuaternionMultiply(&tmpQuater, &tempJointOrientation, &tempWeightPos),
							&tempJointOrientationConjugate
						);
						rotatedPoint.x = result.x;
						rotatedPoint.y = result.y;
						rotatedPoint.z = result.z;

						// Now move the verices position from joint space (0,0,0) to the joints position in world space, taking the weights bias into account
						// The weight bias is used because multiple weights might have an effect on the vertices final position. Each weight is attached to one joint.
						tempVert.pos.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.bias;
						tempVert.pos.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.bias;
						tempVert.pos.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.bias;

						// Basically what has happened above, is we have taken the weights position relative to the joints position
						// we then rotate the weights position (so that the weight is actually being rotated around (0, 0, 0) in world space) using
						// the quaternion describing the joints rotation. We have stored this rotated point in rotatedPoint, which we then add to
						// the joints position (because we rotated the weight's position around (0,0,0) in world space, and now need to translate it
						// so that it appears to have been rotated around the joints position). Finally we multiply the answer with the weights bias,
						// or how much control the weight has over the final vertices position. All weight's bias effecting a single vertex's position
						// must add up to 1.
					}

					subset.positions.push_back(tempVert.pos);			// Store the vertices position in the position vector instead of straight into the vertex vector
					// since we can use the positions vector for certain things like collision detection or picking
					// without having to work with the entire vertex structure.
				}

				// Put the positions into the vertices for this subset
				for (int i = 0; i < subset.vertices.size(); i++)
				{
					subset.vertices[i].pos = subset.positions[i];
				}

				//*** Calculate vertex normals using normal averaging ***///
				std::vector<D3DXVECTOR3> tempNormal;

				//normalized and unnormalized normals
				D3DXVECTOR3 unnormalized = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//Used to get vectors (sides) from the position of the verts
				float vecX, vecY, vecZ;

				//Two edges of our triangle
				D3DXQUATERNION edge1 = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
				D3DXQUATERNION edge2 = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);

				//Compute face normals
				for (int i = 0; i < subset.numTriangles; ++i)
				{
					//Get the vector describing one edge of our triangle (edge 0,2)
					vecX = subset.vertices[subset.indices[(i * 3)]].pos.x - subset.vertices[subset.indices[(i * 3) + 2]].pos.x;
					vecY = subset.vertices[subset.indices[(i * 3)]].pos.y - subset.vertices[subset.indices[(i * 3) + 2]].pos.y;
					vecZ = subset.vertices[subset.indices[(i * 3)]].pos.z - subset.vertices[subset.indices[(i * 3) + 2]].pos.z;
					edge1 = D3DXQUATERNION(vecX, vecY, vecZ, 0.0f);	//Create our first edge

					//Get the vector describing another edge of our triangle (edge 2,1)
					vecX = subset.vertices[subset.indices[(i * 3) + 2]].pos.x - subset.vertices[subset.indices[(i * 3) + 1]].pos.x;
					vecY = subset.vertices[subset.indices[(i * 3) + 2]].pos.y - subset.vertices[subset.indices[(i * 3) + 1]].pos.y;
					vecZ = subset.vertices[subset.indices[(i * 3) + 2]].pos.z - subset.vertices[subset.indices[(i * 3) + 1]].pos.z;
					edge2 = D3DXQUATERNION(vecX, vecY, vecZ, 0.0f);	//Create our second edge

					//Cross multiply the two edge vectors to get the un-normalized face normal
					D3DXVECTOR3 vec3Edge1 = D3DXVECTOR3(edge1.x, edge1.y, edge1.z);
					D3DXVECTOR3 vec3Edge2 = D3DXVECTOR3(edge2.x, edge2.y, edge2.z);
					unnormalized = *D3DXVec3Cross(&tmpVec3, &vec3Edge1, &vec3Edge2);

					tempNormal.push_back(unnormalized);
				}

				//Compute vertex normals (normal Averaging)
				D3DXQUATERNION normalSum = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
				int facesUsing = 0;
				float tX, tY, tZ;	//temp axis variables

				//Go through each vertex
				for (int i = 0; i < subset.vertices.size(); ++i)
				{
					//Check which triangles use this vertex
					for (int j = 0; j < subset.numTriangles; ++j)
					{
						if (subset.indices[j * 3] == i ||
							subset.indices[(j * 3) + 1] == i ||
							subset.indices[(j * 3) + 2] == i)
						{
							tX = normalSum.x + tempNormal[j].x;
							tY = normalSum.y + tempNormal[j].y;
							tZ = normalSum.z + tempNormal[j].z;

							normalSum = D3DXQUATERNION(tX, tY, tZ, 0.0f);	//If a face is using the vertex, add the unormalized face normal to the normalSum

							facesUsing++;
						}
					}

					//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
					normalSum = normalSum / facesUsing;

					D3DXVECTOR3 vec3NormalSum = D3DXVECTOR3(normalSum.x, normalSum.y, normalSum.z);

					//Normalize the normalSum vector
					D3DXVECTOR3 resultNormalSum = *D3DXVec3Normalize(&tmpVec3, &vec3NormalSum);
					normalSum = D3DXQUATERNION(resultNormalSum.x, resultNormalSum.y, resultNormalSum.z, 0.0f);

					//Store the normal and tangent in our current vertex
					subset.vertices[i].normal.x = -normalSum.x;
					subset.vertices[i].normal.y = -normalSum.y;
					subset.vertices[i].normal.z = -normalSum.z;

					///////////////**************new**************////////////////////
					// Create the joint space normal for easy normal calculations in animation
					Vertex tempVert = subset.vertices[i];						// Get the current vertex
					subset.jointSpaceNormals.push_back(D3DXVECTOR3(0, 0, 0));		// Push back a blank normal
					D3DXQUATERNION normal = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);		// Clear normal

					for (int k = 0; k < tempVert.WeightCount; k++)				// Loop through each of the vertices weights
					{
						Joint tempJoint = MD5Model.joints[subset.weights[tempVert.StartWeight + k].jointID];	// Get the joints orientation
						D3DXQUATERNION jointOrientation = D3DXQUATERNION(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);

						// Calculate normal based off joints orientation (turn into joint space)
						normal = *D3DXQuaternionMultiply
						(
							&tmpQuater,
							D3DXQuaternionMultiply(&tmpQuater, D3DXQuaternionInverse(&tmpQuater ,&jointOrientation), &normalSum),
							&jointOrientation
						);

						subset.weights[tempVert.StartWeight + k].normal = *D3DXVec3Normalize(&tmpVec3, &D3DXVECTOR3(normal.x, normal.y, normal.z));			// Store the normalized quaternion into our weights normal
					}
					///////////////**************new**************////////////////////
					//Clear normalSum, facesUsing for next vertex
					normalSum = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
					facesUsing = 0;
				}

				// Create index buffer
				D3D11_BUFFER_DESC indexBufferDesc;
				ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

				indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				indexBufferDesc.ByteWidth = sizeof(DWORD) * subset.numTriangles * 3;
				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufferDesc.CPUAccessFlags = 0;
				indexBufferDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA iinitData;
				iinitData.pSysMem = &subset.indices[0];
				d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &subset.indexBuff);

				//Create Vertex Buffer
				D3D11_BUFFER_DESC vertexBufferDesc;
				ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

				vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;							// We will be updating this buffer, so we must set as dynamic
				vertexBufferDesc.ByteWidth = sizeof(Vertex) * subset.vertices.size();
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				// Give CPU power to write to buffer
				vertexBufferDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA vertexBufferData;
				ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
				vertexBufferData.pSysMem = &subset.vertices[0];
				hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &subset.vertBuff);

				// Push back the temp subset into the models subset vector
				MD5Model.subsets.push_back(subset);
			}
		}
	}
	else
	{
		//SwapChain->SetFullscreenState(false, NULL);	// Make sure we are out of fullscreen

		// create message
		std::wstring message = L"Could not open: ";
		message += filename;

		MessageBox(0, message.c_str(),	// display message
			L"Error", MB_OK);

		return false;
	}

	return true;
}


bool Md5ModelClass::LoadMD5Anim(std::wstring filename, Model3D& MD5Model)
{
	ModelAnimation tempAnim;						// Temp animation to later store in our model's animation array

	std::wifstream fileIn(filename.c_str());		// Open file

	std::wstring checkString;						// Stores the next string from our file

	if (fileIn)										// Check if the file was opened
	{
		while (fileIn)								// Loop until the end of the file is reached
		{
			fileIn >> checkString;					// Get next string from file

			if (checkString == L"MD5Version")		// Get MD5 version (this function supports version 10)
			{
				fileIn >> checkString;
				/*MessageBox(0, checkString.c_str(),	//display message
				L"MD5Version", MB_OK);*/
			}
			else if (checkString == L"commandline")
			{
				std::getline(fileIn, checkString);	// Ignore the rest of this line
			}
			else if (checkString == L"numFrames")
			{
				fileIn >> tempAnim.numFrames;				// Store number of frames in this animation
			}
			else if (checkString == L"numJoints")
			{
				fileIn >> tempAnim.numJoints;				// Store number of joints (must match .md5mesh)
			}
			else if (checkString == L"frameRate")
			{
				fileIn >> tempAnim.frameRate;				// Store animation's frame rate (frames per second)
			}
			else if (checkString == L"numAnimatedComponents")
			{
				fileIn >> tempAnim.numAnimatedComponents;	// Number of components in each frame section
			}
			else if (checkString == L"hierarchy")
			{
				fileIn >> checkString;				// Skip opening bracket "{"

				for (int i = 0; i < tempAnim.numJoints; i++)	// Load in each joint
				{
					AnimJointInfo tempJoint;

					fileIn >> tempJoint.name;		// Get joints name
					// Sometimes the names might contain spaces. If that is the case, we need to continue
					// to read the name until we get to the closing " (quotation marks)
					if (tempJoint.name[tempJoint.name.size() - 1] != '"')
					{
						wchar_t checkChar;
						bool jointNameFound = false;
						while (!jointNameFound)
						{
							checkChar = fileIn.get();

							if (checkChar == '"')
								jointNameFound = true;

							tempJoint.name += checkChar;
						}
					}

					// Remove the quotation marks from joints name
					tempJoint.name.erase(0, 1);
					tempJoint.name.erase(tempJoint.name.size() - 1, 1);

					fileIn >> tempJoint.parentID;			// Get joints parent ID
					fileIn >> tempJoint.flags;				// Get flags
					fileIn >> tempJoint.startIndex;			// Get joints start index

					// Make sure the joint exists in the model, and the parent ID's match up
					// because the bind pose (md5mesh) joint hierarchy and the animations (md5anim)
					// joint hierarchy must match up
					bool jointMatchFound = false;
					for (int k = 0; k < MD5Model.numJoints; k++)
					{
						if (MD5Model.joints[k].name == tempJoint.name)
						{
							if (MD5Model.joints[k].parentID == tempJoint.parentID)
							{
								jointMatchFound = true;
								tempAnim.jointInfo.push_back(tempJoint);
							}
						}
					}
					if (!jointMatchFound)					// If the skeleton system does not match up, return false
						return false;						// You might want to add an error message here

					std::getline(fileIn, checkString);		// Skip rest of this line
				}
			}
			else if (checkString == L"bounds")			// Load in the AABB for each animation
			{
				fileIn >> checkString;						// Skip opening bracket "{"

				for (int i = 0; i < tempAnim.numFrames; i++)
				{
					BoundingBox tempBB;

					fileIn >> checkString;					// Skip "("
					fileIn >> tempBB.min.x >> tempBB.min.z >> tempBB.min.y;
					fileIn >> checkString >> checkString;	// Skip ") ("
					fileIn >> tempBB.max.x >> tempBB.max.z >> tempBB.max.y;
					fileIn >> checkString;					// Skip ")"

					tempAnim.frameBounds.push_back(tempBB);
				}
			}
			else if (checkString == L"baseframe")			// This is the default position for the animation
			{												// All frames will build their skeletons off this
				fileIn >> checkString;						// Skip opening bracket "{"

				for (int i = 0; i < tempAnim.numJoints; i++)
				{
					Joint tempBFJ;

					fileIn >> checkString;						// Skip "("
					fileIn >> tempBFJ.pos.x >> tempBFJ.pos.z >> tempBFJ.pos.y;
					fileIn >> checkString >> checkString;		// Skip ") ("
					fileIn >> tempBFJ.orientation.x >> tempBFJ.orientation.z >> tempBFJ.orientation.y;
					fileIn >> checkString;						// Skip ")"

					tempAnim.baseFrameJoints.push_back(tempBFJ);
				}
			}
			else if (checkString == L"frame")		// Load in each frames skeleton (the parts of each joint that changed from the base frame)
			{
				FrameData tempFrame;

				fileIn >> tempFrame.frameID;		// Get the frame ID

				fileIn >> checkString;				// Skip opening bracket "{"

				for (int i = 0; i < tempAnim.numAnimatedComponents; i++)
				{
					float tempData;
					fileIn >> tempData;				// Get the data

					tempFrame.frameData.push_back(tempData);
				}

				tempAnim.frameData.push_back(tempFrame);

				///*** build the frame skeleton ***///
				std::vector<Joint> tempSkeleton;

				for (int i = 0; i < tempAnim.jointInfo.size(); i++)
				{
					int k = 0;						// Keep track of position in frameData array

					// Start the frames joint with the base frame's joint
					Joint tempFrameJoint = tempAnim.baseFrameJoints[i];

					tempFrameJoint.parentID = tempAnim.jointInfo[i].parentID;

					// Notice how I have been flipping y and z. this is because some modeling programs such as
					// 3ds max (which is what I use) use a right handed coordinate system. Because of this, we
					// need to flip the y and z axes. If your having problems loading some models, it's possible
					// the model was created in a left hand coordinate system. in that case, just reflip all the
					// y and z axes in our md5 mesh and anim loader.
					if (tempAnim.jointInfo[i].flags & 1)		// pos.x	( 000001 )
						tempFrameJoint.pos.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 2)		// pos.y	( 000010 )
						tempFrameJoint.pos.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 4)		// pos.z	( 000100 )
						tempFrameJoint.pos.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 8)		// orientation.x	( 001000 )
						tempFrameJoint.orientation.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 16)	// orientation.y	( 010000 )
						tempFrameJoint.orientation.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 32)	// orientation.z	( 100000 )
						tempFrameJoint.orientation.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];


					// Compute the quaternions w
					float t = 1.0f - (tempFrameJoint.orientation.x * tempFrameJoint.orientation.x)
						- (tempFrameJoint.orientation.y * tempFrameJoint.orientation.y)
						- (tempFrameJoint.orientation.z * tempFrameJoint.orientation.z);
					if (t < 0.0f)
					{
						tempFrameJoint.orientation.w = 0.0f;
					}
					else
					{
						tempFrameJoint.orientation.w = -sqrtf(t);
					}

					// Now, if the upper arm of your skeleton moves, you need to also move the lower part of your arm, and then the hands, and then finally the fingers (possibly weapon or tool too)
					// This is where joint hierarchy comes in. We start at the top of the hierarchy, and move down to each joints child, rotating and translating them based on their parents rotation
					// and translation. We can assume that by the time we get to the child, the parent has already been rotated and transformed based of it's parent. We can assume this because
					// the child should never come before the parent in the files we loaded in.
					if (tempFrameJoint.parentID >= 0)
					{
						Joint parentJoint = tempSkeleton[tempFrameJoint.parentID];

						// Turn the XMFLOAT3 and 4's into vectors for easier computation
						D3DXQUATERNION parentJointOrientation = D3DXQUATERNION(parentJoint.orientation.x, parentJoint.orientation.y, parentJoint.orientation.z, parentJoint.orientation.w);
						D3DXQUATERNION tempJointPos = D3DXQUATERNION(tempFrameJoint.pos.x, tempFrameJoint.pos.y, tempFrameJoint.pos.z, 0.0f);
						D3DXQUATERNION parentOrientationConjugate = D3DXQUATERNION(-parentJoint.orientation.x, -parentJoint.orientation.y, -parentJoint.orientation.z, parentJoint.orientation.w);

						// Calculate current joints position relative to its parents position
						D3DXVECTOR3 rotatedPos;
						D3DXQUATERNION QuatRotatedPos;
						QuatRotatedPos =
							*D3DXQuaternionMultiply
							(
								&tmpQuater,
								D3DXQuaternionMultiply(&tmpQuater, &parentJointOrientation, &tempJointPos),
								&parentOrientationConjugate
							);
						rotatedPos = D3DXVECTOR3(QuatRotatedPos.x, QuatRotatedPos.y, QuatRotatedPos.z);

						// Translate the joint to model space by adding the parent joint's pos to it
						tempFrameJoint.pos.x = rotatedPos.x + parentJoint.pos.x;
						tempFrameJoint.pos.y = rotatedPos.y + parentJoint.pos.y;
						tempFrameJoint.pos.z = rotatedPos.z + parentJoint.pos.z;

						// Currently the joint is oriented in its parent joints space, we now need to orient it in
						// model space by multiplying the two orientations together (parentOrientation * childOrientation) <- In that order
						D3DXQUATERNION tempJointOrient = D3DXQUATERNION(tempFrameJoint.orientation.x, tempFrameJoint.orientation.y, tempFrameJoint.orientation.z, tempFrameJoint.orientation.w);
						tempJointOrient = *D3DXQuaternionMultiply(&tmpQuater, &parentJointOrientation, &tempJointOrient);

						// Normalize the orienation quaternion
						tempJointOrient = *D3DXQuaternionNormalize(&tmpQuater, &tempJointOrient);

						tempFrameJoint.orientation = tempJointOrient;
					}

					// Store the joint into our temporary frame skeleton
					tempSkeleton.push_back(tempFrameJoint);
				}

				// Push back our newly created frame skeleton into the animation's frameSkeleton array
				tempAnim.frameSkeleton.push_back(tempSkeleton);

				fileIn >> checkString;				// Skip closing bracket "}"
			}
		}

		// Calculate and store some usefull animation data
		tempAnim.frameTime = 1.0f / tempAnim.frameRate;						// Set the time per frame
		tempAnim.totalAnimTime = tempAnim.numFrames * tempAnim.frameTime;	// Set the total time the animation takes
		tempAnim.currAnimTime = 0.0f;										// Set the current time to zero

		MD5Model.animations.push_back(tempAnim);							// Push back the animation into our model object
	}
	else	// If the file was not loaded
	{
		//SwapChain->SetFullscreenState(false, NULL);	// Make sure we are out of fullscreen

		// create message
		std::wstring message = L"Could not open: ";
		message += filename;

		MessageBox(0, message.c_str(),				// display message
			L"Error", MB_OK);

		return false;
	}
	return true;
}


void Md5ModelClass::UpdateMD5Model(float deltaTime, int animation, ID3D11DeviceContext* d3d11DevCon)
{
	MD5Model.animations[animation].currAnimTime += deltaTime;			// Update the current animation time

	if (MD5Model.animations[animation].currAnimTime > MD5Model.animations[animation].totalAnimTime)
		MD5Model.animations[animation].currAnimTime = 0.0f;

	// Which frame are we on
	float currentFrame = MD5Model.animations[animation].currAnimTime * MD5Model.animations[animation].frameRate;
	int frame0 = floorf(currentFrame);
	int frame1 = frame0 + 1;

	// Make sure we don't go over the number of frames	
	if (frame0 == MD5Model.animations[animation].numFrames - 1)
		frame1 = 0;

	float interpolation = currentFrame - frame0;	// Get the remainder (in time) between frame0 and frame1 to use as interpolation factor

	std::vector<Joint> interpolatedSkeleton;		// Create a frame skeleton to store the interpolated skeletons in

	// Compute the interpolated skeleton
	for (int i = 0; i < MD5Model.animations[animation].numJoints; i++)
	{
		Joint tempJoint;
		Joint joint0 = MD5Model.animations[animation].frameSkeleton[frame0][i];		// Get the i'th joint of frame0's skeleton
		Joint joint1 = MD5Model.animations[animation].frameSkeleton[frame1][i];		// Get the i'th joint of frame1's skeleton

		tempJoint.parentID = joint0.parentID;											// Set the tempJoints parent id

		// Turn the two quaternions into XMVECTORs for easy computations
		D3DXQUATERNION joint0Orient = D3DXQUATERNION(joint0.orientation.x, joint0.orientation.y, joint0.orientation.z, joint0.orientation.w);
		D3DXQUATERNION joint1Orient = D3DXQUATERNION(joint1.orientation.x, joint1.orientation.y, joint1.orientation.z, joint1.orientation.w);

		// Interpolate positions
		tempJoint.pos.x = joint0.pos.x + (interpolation * (joint1.pos.x - joint0.pos.x));
		tempJoint.pos.y = joint0.pos.y + (interpolation * (joint1.pos.y - joint0.pos.y));
		tempJoint.pos.z = joint0.pos.z + (interpolation * (joint1.pos.z - joint0.pos.z));

		// Interpolate orientations using spherical interpolation (Slerp)
		tempJoint.orientation = *D3DXQuaternionSlerp(&tmpQuater, &joint0Orient, &joint1Orient, interpolation);

		interpolatedSkeleton.push_back(tempJoint);		// Push the joint back into our interpolated skeleton
	}

	for (int k = 0; k < MD5Model.numSubsets; k++)
	{
		for (int i = 0; i < MD5Model.subsets[k].vertices.size(); ++i)
		{
			Vertex tempVert = MD5Model.subsets[k].vertices[i];
			tempVert.pos = D3DXVECTOR3(0, 0, 0);	// Make sure the vertex's pos is cleared first
			tempVert.normal = D3DXVECTOR3(0, 0, 0);	// Clear vertices normal

			// Sum up the joints and weights information to get vertex's position and normal
			for (int j = 0; j < tempVert.WeightCount; ++j)
			{
				Weight tempWeight = MD5Model.subsets[k].weights[tempVert.StartWeight + j];
				Joint tempJoint = interpolatedSkeleton[tempWeight.jointID];

				// Convert joint orientation and weight pos to vectors for easier computation
				D3DXQUATERNION tempJointOrientation = D3DXQUATERNION(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
				D3DXQUATERNION tempWeightPos = D3DXQUATERNION(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

				// We will need to use the conjugate of the joint orientation quaternion
				D3DXQUATERNION tempJointOrientationConjugate = *D3DXQuaternionInverse(&tmpQuater, &tempJointOrientation);

				// Calculate vertex position (in joint space, eg. rotate the point around (0,0,0)) for this weight using the joint orientation quaternion and its conjugate
				// We can rotate a point using a quaternion with the equation "rotatedPoint = quaternion * point * quaternionConjugate"
				D3DXVECTOR3 rotatedPoint;
				D3DXQUATERNION quatRotatedPoint;
				quatRotatedPoint =
					*D3DXQuaternionMultiply
					(
						&tmpQuater,
						D3DXQuaternionMultiply(&tmpQuater, &tempJointOrientation, &tempWeightPos),
						&tempJointOrientationConjugate
					);
					rotatedPoint = D3DXVECTOR3(quatRotatedPoint.x, quatRotatedPoint.y, quatRotatedPoint.z);

				// Now move the verices position from joint space (0,0,0) to the joints position in world space, taking the weights bias into account
				tempVert.pos.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.bias;
				tempVert.pos.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.bias;
				tempVert.pos.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.bias;

				// Compute the normals for this frames skeleton using the weight normals from before
				// We can comput the normals the same way we compute the vertices position, only we don't have to translate them (just rotate)
				D3DXQUATERNION tempWeightNormal = D3DXQUATERNION(tempWeight.normal.x, tempWeight.normal.y, tempWeight.normal.z, 0.0f);

				// Rotate the normal
				quatRotatedPoint =
					*D3DXQuaternionMultiply
					(
						&tmpQuater,
						D3DXQuaternionMultiply(&tmpQuater, &tempJointOrientation, &tempWeightNormal),
						&tempJointOrientationConjugate
					);
				rotatedPoint = D3DXVECTOR3(quatRotatedPoint.x, quatRotatedPoint.y, quatRotatedPoint.z);

				// Add to vertices normal and ake weight bias into account
				tempVert.normal.x -= rotatedPoint.x * tempWeight.bias;
				tempVert.normal.y -= rotatedPoint.y * tempWeight.bias;
				tempVert.normal.z -= rotatedPoint.z * tempWeight.bias;
			}

			MD5Model.subsets[k].positions[i] = tempVert.pos;				// Store the vertices position in the position vector instead of straight into the vertex vector
			MD5Model.subsets[k].vertices[i].normal = tempVert.normal;		// Store the vertices normal
			MD5Model.subsets[k].vertices[i].normal = 
				*D3DXVec3Normalize
				(
					&tmpVec3,
					&D3DXVECTOR3
					(
						MD5Model.subsets[k].vertices[i].normal.x,
						MD5Model.subsets[k].vertices[i].normal.y,
						MD5Model.subsets[k].vertices[i].normal.z
					)
				);
		}

		// Put the positions into the vertices for this subset
		for (int i = 0; i < MD5Model.subsets[k].vertices.size(); i++)
		{
			MD5Model.subsets[k].vertices[i].pos = MD5Model.subsets[k].positions[i];
		}

		// Update the subsets vertex buffer
		// First lock the buffer
		D3D11_MAPPED_SUBRESOURCE mappedVertBuff;
		hr = d3d11DevCon->Map(MD5Model.subsets[k].vertBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertBuff);

		// Copy the data into the vertex buffer.
		memcpy(mappedVertBuff.pData, &MD5Model.subsets[k].vertices[0], (sizeof(Vertex) * MD5Model.subsets[k].vertices.size()));

		d3d11DevCon->Unmap(MD5Model.subsets[k].vertBuff, 0);

		// The line below is another way to update a buffer. You will use this when you want to update a buffer less
		// than once per frame, since the GPU reads will be faster (the buffer was created as a DEFAULT buffer instead
		// of a DYNAMIC buffer), and the CPU writes will be slower. You can try both methods to find out which one is faster
		// for you. if you want to use the line below, you will have to create the buffer with D3D11_USAGE_DEFAULT instead
		// of D3D11_USAGE_DYNAMIC
		//d3d11DevCon->UpdateSubresource( MD5Model.subsets[k].vertBuff, 0, NULL, &MD5Model.subsets[k].vertices[0], 0, 0 );
	}
}


void Md5ModelClass::DrawMd5Model(ID3D11DeviceContext* d3d11DevCon, D3DXMATRIX md5World, D3DXMATRIX camView, D3DXMATRIX camProjection)
{
	//Clear our render target and depth/stencil view
	/*float bgColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);*/

	constbuffPerFrame.light = light;
	d3d11DevCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbuffPerFrame, 0, 0);
	d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);

	////Set our Render Target
	//d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	//Set the default blend state (no blending) for opaque objects
	d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff);

	//Set Vertex and Pixel Shaders
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	///***Draw MD5 Model***///
	for (int i = 0; i < MD5Model.numSubsets; i++)
	{
		//Set the grounds index buffer
		d3d11DevCon->IASetIndexBuffer(MD5Model.subsets[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);
		//Set the grounds vertex buffer
		d3d11DevCon->IASetVertexBuffers(0, 1, &MD5Model.subsets[i].vertBuff, &stride, &offset);

		//Set the WVP matrix and send it to the constant buffer in effect file
		WVP = md5World * camView * camProjection;
		cbPerObj.WVP = XMLoadFloat4x4(&XMFLOAT4X4(*D3DXMatrixTranspose(&tmpMat, &WVP)));
		cbPerObj.World = XMLoadFloat4x4(&XMFLOAT4X4(*D3DXMatrixTranspose(&tmpMat, &md5World)));
		cbPerObj.hasTexture = true;		// We'll assume all md5 subsets have textures
		cbPerObj.hasNormMap = false;	// We'll also assume md5 models have no normal map (easy to change later though)
		d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
		d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
		d3d11DevCon->PSSetConstantBuffers(1, 1, &cbPerObjectBuffer);
		d3d11DevCon->PSSetShaderResources(0, 1, &meshSRV[MD5Model.subsets[i].texArrayIndex]);
		d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

		d3d11DevCon->RSSetState(RSCullNone);
		d3d11DevCon->DrawIndexed(MD5Model.subsets[i].indices.size(), 0, 0);
	}

	///////Draw our model's NON-transparent subsets/////
	//for (int i = 0; i < meshSubsets; ++i)
	//{
	//	//Set the grounds index buffer
	//	d3d11DevCon->IASetIndexBuffer(meshIndexBuff, DXGI_FORMAT_R32_UINT, 0);
	//	//Set the grounds vertex buffer
	//	d3d11DevCon->IASetVertexBuffers(0, 1, &meshVertBuff, &stride, &offset);

	//	//Set the WVP matrix and send it to the constant buffer in effect file
	//	WVP = meshWorld * camView * camProjection;
	//	cbPerObj.WVP = XMMatrixTranspose(WVP);
	//	cbPerObj.World = XMMatrixTranspose(meshWorld);
	//	cbPerObj.difColor = material[meshSubsetTexture[i]].difColor;
	//	cbPerObj.hasTexture = material[meshSubsetTexture[i]].hasTexture;
	//	cbPerObj.hasNormMap = material[meshSubsetTexture[i]].hasNormMap;
	//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	//	d3d11DevCon->PSSetConstantBuffers(1, 1, &cbPerObjectBuffer);
	//	if (material[meshSubsetTexture[i]].hasTexture)
	//		d3d11DevCon->PSSetShaderResources(0, 1, &meshSRV[material[meshSubsetTexture[i]].texArrayIndex]);
	//	if (material[meshSubsetTexture[i]].hasNormMap)
	//		d3d11DevCon->PSSetShaderResources(1, 1, &meshSRV[material[meshSubsetTexture[i]].normMapTexArrayIndex]);
	//	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	//	d3d11DevCon->RSSetState(RSCullNone);
	//	int indexStart = meshSubsetIndexStart[i];
	//	int indexDrawAmount = meshSubsetIndexStart[i + 1] - meshSubsetIndexStart[i];
	//	if (!material[meshSubsetTexture[i]].transparent)
	//		d3d11DevCon->DrawIndexed(indexDrawAmount, indexStart, 0);
	//}
}

float Md5ModelClass::getTotalAnimTime(int animation)
{
	return MD5Model.animations[animation].totalAnimTime;
}

void Md5ModelClass::MakeAABB(XMMATRIX xmWorld)
{
	CollisionClass::CreateBoundingVolumes(modelVerts,
		boundingBoxVerts,
		boundingBoxVertIndex,
		boundingSphere,
		centerOffset);

	CollisionClass::CalculateAABB(boundingBoxVerts,
		xmWorld,
		boundingBoxMin,
		boundingBoxMax);
}

XMVECTOR Md5ModelClass::GetBoundingBoxMin()
{
	return boundingBoxMin;
}

void Md5ModelClass::SetBoundingBoxMin(XMVECTOR min)
{
	boundingBoxMin = min;
}

XMVECTOR Md5ModelClass::GetBoundingBoxMax()
{
	return boundingBoxMax;
}

void Md5ModelClass::SetBoundingBoxMax(XMVECTOR max)
{
	boundingBoxMax = max;
}