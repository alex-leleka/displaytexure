////////////////////////////////////////////////////////////////////////////////
// Filename: DisplayTexture.cpp
////////////////////////////////////////////////////////////////////////////////
/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/
#include "DisplayTexture.h"
#include "textureshaderclass.h"
#include "rendertextureclass.h"

DisplayTexture::DisplayTexture() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr)
{

}

bool DisplayTexture::Render(ID3D11DeviceContext* pDeviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX orthoMatrix, TextureShaderClass* pTextureShader, RenderTextureClass* pRenderTexture, bool bBlurDirH)
{
	unsigned int stride;
	unsigned int offset;
	 		 
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;
	 
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	 
	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	 
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	bool result = pTextureShader->Render(pDeviceContext, m_indexCount, worldMatrix, viewMatrix, orthoMatrix, pRenderTexture->GetShaderResourceView(), bBlurDirH);
	return result;
}

void DisplayTexture::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}

bool DisplayTexture::Initialize(ID3D11Device* pDevice, HWND hwnd)
{
	 	VertexType* vertices;
	 	unsigned long* indices;
	 	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	 	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	 	HRESULT result;

	 	// Set the number of vertices in the vertex array.
		// Set the number of indices in the index array.
		m_indexCount = m_vertexCount = 6;
	 
		 	// Create the vertex array.
		 	vertices = new VertexType[m_vertexCount];
	 	if (!vertices)
		 	{
			 		return false;
			 	}
	 
		 	// Create the index array.
		 	indices = new unsigned long[m_indexCount];
	 	if (!indices)
		 	{
			 		return false;
			 	}
	 
		 	// Initialize vertex array to zeros at first.
		 	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));
	 
		 	// Calculate the screen coordinates of the left side of the bitmap.
		 	float left = -400;// (float)((800 / 2) * -1) + (float)0.0;
	 
		 	// Calculate the screen coordinates of the right side of the bitmap.
		 	float right = 400;// left + (float)800;
	 
		 	// Calculate the screen coordinates of the top of the bitmap.
		 	float top = -300;// (float)(600 / 2) - (float)0;
	 
		 	// Calculate the screen coordinates of the bottom of the bitmap.
		 	float bottom = 300;// top - (float)600;
	 
		 	// Create the vertex array.
		 	vertices = new VertexType[m_vertexCount];
	 	if (!vertices)
		 	{
			 		return false;
			 	}
	 
		 	// Load the vertex array with data.
		 	// First triangle.
		 	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	 	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);
	 
		 	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	 	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);
	 
		 	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	 	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);
	 
		 	// Second triangle.
		 	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	 	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);
	 
		 	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	 	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);
	 
		 	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	 	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);
	 
		 	// Load the index array with data.
		 	for (size_t i = 0; i<m_indexCount; i++)
		 	{
			 		indices[i] = i;
			 	}
	 
		 	// Set up the description of the static vertex buffer.
		 	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	 	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	 	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	 	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	 	vertexBufferDesc.MiscFlags = 0;
	 	vertexBufferDesc.StructureByteStride = 0;
	 
		 	// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
	 	vertexData.SysMemPitch = 0;
	 	vertexData.SysMemSlicePitch = 0;
	 
		 	// Now create the vertex buffer.
		result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	 	if (FAILED(result))
		 	{
			 		return false;
			 	}
	 
		 	// Set up the description of the static index buffer.
		 	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	 	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	 	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	 	indexBufferDesc.CPUAccessFlags = 0;
	 	indexBufferDesc.MiscFlags = 0;
	 	indexBufferDesc.StructureByteStride = 0;
	 
		 	// Give the subresource structure a pointer to the index data.
		 	indexData.pSysMem = indices;
	 	indexData.SysMemPitch = 0;
	 	indexData.SysMemSlicePitch = 0;
	 
		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete[] vertices;
		vertices = 0;


		 	// Create the index buffer.
		result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		delete[] indices;
		indices = 0;
	 	if (FAILED(result))
			return false;
		return true;
}

void DisplayTexture::ShutdownBuffers()
{
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}