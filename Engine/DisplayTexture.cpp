////////////////////////////////////////////////////////////////////////////////
// Filename: DisplayTexture.cpp
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//	    Displaytexure demo https://github.com/alex-leleka/displaytexure          //
//																				 //
//      Alex Leleka (c) 2016                                                     //
///////////////////////////////////////////////////////////////////////////////////
#include "DisplayTexture.h"
#include "textureshaderclass.h"
#include "rendertextureclass.h"
#include <memory>

DisplayTexture::DisplayTexture() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr)
{

}

bool DisplayTexture::Render(ID3D11DeviceContext* pDeviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, TextureShaderClass* pTextureShader, RenderTextureClass* pRenderTexture, int nBlurDirHV)
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
	pTextureShader->SetBlurDirection(nBlurDirHV);
	bool result = pTextureShader->Render(pDeviceContext, m_indexCount, worldMatrix, viewMatrix, orthoMatrix, pRenderTexture->GetShaderResourceView());
	return result;
}

void DisplayTexture::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}

bool DisplayTexture::Initialize(ID3D11Device* pDevice, int textureWidth, int textureHeight)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_indexCount = m_vertexCount = 6;

	std::unique_ptr<VertexType[]> vertices(new VertexType[m_vertexCount]);
	if (!vertices.get())
	{
		return false;
	}

	std::unique_ptr<unsigned long[]> indices(new unsigned long[m_indexCount]);
	if (!indices.get())
	{
		return false;
	}
	memset(vertices.get(), 0, (sizeof(VertexType) * m_vertexCount));
	float left = -textureWidth / 2.0f;
	float right = textureWidth / 2.0f;
	float top = -textureHeight / 2.0f;
	float bottom = textureHeight / 2.0f;

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
	for (size_t i = 0; i < m_indexCount; i++)
		indices[i] = i;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices.get();
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
	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
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