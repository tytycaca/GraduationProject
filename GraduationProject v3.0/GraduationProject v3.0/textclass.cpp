///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"


TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;

	//m_sentence1 = 0;
	//m_sentence2 = 0;

	for (int i = 0; i < TEXTNUM; i++) m_sentence[i] = 0;
}


TextClass::TextClass(const TextClass& other)
{
}


TextClass::~TextClass()
{
}


bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight,
	D3DXMATRIX baseViewMatrix)
{
	bool result;


	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, (char*)"../GraduationProject v3.0/data/fontdata.txt", (WCHAR*)L"../GraduationProject v3.0/data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	//// Initialize the first sentence.
	//result = InitializeSentence(&m_sentence1, 16, device);
	//if (!result)
	//{
	//	return false;
	//}

	//// Now update the sentence vertex buffer with the new string information.
	//result = UpdateSentence(m_sentence1, (char*)"Hello", 75, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	//if (!result)
	//{
	//	return false;
	//}

	//// Initialize the first sentence.
	//result = InitializeSentence(&m_sentence2, 16, device);
	//if (!result)
	//{
	//	return false;
	//}

	//// Now update the sentence vertex buffer with the new string information.
	//result = UpdateSentence(m_sentence2, (char*)"Goodbye", 75, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	//if (!result)
	//{
	//	return false;
	//}

	for (int i = 0; i < TEXTNUM; i++)
	{
		result = InitializeSentence(&m_sentence[i], 32, device);
		if (!result) return false;

		result = UpdateSentence(m_sentence[i], (char*)"null", 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
		if (!result) return false;
	}

	return true;
}


void TextClass::Shutdown()
{
	//// Release the first sentence.
	//ReleaseSentence(&m_sentence1);

	//// Release the second sentence.
	//ReleaseSentence(&m_sentence2);

	for (int i = 0; i < TEXTNUM; i++) ReleaseSentence(&m_sentence[i]);

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;


	//// Draw the first sentence.
	//result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	//if (!result)
	//{
	//	return false;
	//}

	//// Draw the second sentence.
	//result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
	//if (!result)
	//{
	//	return false;
	//}

	for (int i = 0; i < TEXTNUM; i++)
	{
		result = RenderSentence(deviceContext, m_sentence[i], worldMatrix, orthoMatrix);
		if (!result)
			return false;
	}

	return true;
}

//bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
//{
//	char tempString[2][32];
//	char mouseString[32];
//	bool result;
//	// Convert the mouseX integer to string format.
//	_itoa_s(mouseX, tempString[0], 10);
//	// Convert the mouseY integer to string format.
//	_itoa_s(mouseY, tempString[1], 10);
//
//	// Setup the mouseX string.
//	strcpy_s(mouseString, "Mouse : ");
//	strcat_s(mouseString, tempString[0]);
//
//	// Setup the mouseY string.
//	strcat_s(mouseString, ", ");
//	strcat_s(mouseString, tempString[1]);
//	// Update the sentence vertex buffer with the new string information.
//	result = UpdateSentence(m_sentence[7], mouseString, 20, 120, 1.0f, 1.0f, 1.0f, deviceContext);
//	if (!result)
//	{
//		return false;
//	}
//	return true;
//}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[32];
	char fpsString[32];
	float red, green, blue;
	bool result;
	// Truncate the fps to below 10,000.
	if (fps > 9999)
	{
		fps = 9999;
	}
	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);
	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);
	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[0], fpsString, 20, 40, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[32];
	char cpuString[32];
	bool result;
	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);
	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[1], cpuString, 20, 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetObject(int obj, ID3D11DeviceContext* deviceContext)
{
	char tempString[32];
	char objString[64];
	bool result;

	_itoa_s(obj, tempString, 10);

	strcpy_s(objString, "Objects : ");
	strcat_s(objString, tempString);

	result = UpdateSentence(m_sentence[2], objString, 20, 80, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result) return false;

	return true;

}

bool TextClass::SetPolygon(int poly, ID3D11DeviceContext* deviceContext)
{
	char tempString[32];
	char polyString[32];
	bool result;

	_itoa_s(poly, tempString, 10);

	strcpy_s(polyString, "Polygons : ");
	strcat_s(polyString, tempString);

	result = UpdateSentence(m_sentence[3], polyString, 20, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result) return false;

	return true;

}

bool TextClass::SetScreenSize(int x, int y, ID3D11DeviceContext* deviceContext)
{
	char tempString[2][32];
	char ScreenSizeString[32];
	bool result;

	_itoa_s(x, tempString[0], 10);
	_itoa_s(y, tempString[1], 10);

	strcpy_s(ScreenSizeString, "Screen : ");
	strcat_s(ScreenSizeString, tempString[0]);
	strcat_s(ScreenSizeString, "x");
	strcat_s(ScreenSizeString, tempString[1]);

	result = UpdateSentence(m_sentence[4], ScreenSizeString, 20, 120, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result) return false;

	return true;

}

bool TextClass::SetCameraPos(D3DXVECTOR3 pos, ID3D11DeviceContext* deviceContext)
{
	char tempString[3][32];
	char CameraPosString[32];
	bool result;

	_itoa_s(pos.x, tempString[0], 10);
	_itoa_s(pos.y, tempString[1], 10);
	_itoa_s(pos.z, tempString[2], 10);

	strcpy_s(CameraPosString, "Camera : ");
	strcat_s(CameraPosString, tempString[0]);
	strcat_s(CameraPosString, ",");
	strcat_s(CameraPosString, tempString[1]);
	strcat_s(CameraPosString, ",");
	strcat_s(CameraPosString, tempString[2]);

	result = UpdateSentence(m_sentence[5], CameraPosString, 20, 140, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result) return false;

	return true;
}

bool TextClass::SetCameraRot(D3DXVECTOR3 rot, ID3D11DeviceContext* deviceContext)
{
	char tempString[3][32];
	char CameraRotString[32];
	bool result;

	_itoa_s(rot.x, tempString[0], 10);
	_itoa_s(rot.y, tempString[1], 10);
	_itoa_s(rot.z, tempString[2], 10);

	strcpy_s(CameraRotString, "CamRot : ");
	strcat_s(CameraRotString, tempString[0]);
	strcat_s(CameraRotString, ",");
	strcat_s(CameraRotString, tempString[1]);
	strcat_s(CameraRotString, ",");
	strcat_s(CameraRotString, tempString[2]);

	result = UpdateSentence(m_sentence[6], CameraRotString, 20, 160, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result) return false;

	return true;
}

// ¼öÁ¤
//bool TextClass::SetUpPos(D3DXVECTOR3 up, ID3D11DeviceContext* deviceContext)
//{
//	char tempString[3][32];
//	char CameraUpString[32];
//	bool result;
//
//	_itoa_s(up.x * 100.0f, tempString[0], 10);
//	_itoa_s(up.y * 100.0f, tempString[1], 10);
//	_itoa_s(up.z * 100.0f, tempString[2], 10);
//
//	strcpy_s(CameraUpString, "CamUp : ");
//	strcat_s(CameraUpString, tempString[0]);
//	strcat_s(CameraUpString, ",");
//	strcat_s(CameraUpString, tempString[1]);
//	strcat_s(CameraUpString, ",");
//	strcat_s(CameraUpString, tempString[2]);
//
//	result = UpdateSentence(m_sentence[8], CameraUpString, 20, 180, 1.0f, 1.0f, 1.0f, deviceContext);
//	if (!result) return false;
//
//	return true;
//}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}


bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
	ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}


void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}


bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
	D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(),
		pixelColor);
	if (!result)
	{
		false;
	}

	return true;
}