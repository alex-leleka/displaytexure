////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
// code based on rastertek tutirial 7
// http://www.rastertek.com/dx11tut07.html
// 
///////////////////////////////////////////////////////////////////////////////////
//	    Displaytexure demo https://github.com/alex-leleka/displaytexure          //
//																				 //
//      Alex Leleka (c) 2016                                                     //
///////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
#include "model.h"

#include <cassert>
#include <fstream> 
#include <string> 

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_TextureDiffuse = 0;
	m_TextureSpecular = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTextureDiffuse()
{
	return m_TextureDiffuse->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTextureSpecular()
{
	return m_TextureSpecular->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	std::unique_ptr<VertexType[]> vertices(new VertexType[m_vertexCount]);
	if(!vertices.get())
	{
		return false;
	}

	for(i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
	}

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
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
	indexData.pSysMem = m_indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	using std::wstring;

	// Create the diffuse texture object.
	m_TextureDiffuse = new TextureClass;
	if(!m_TextureDiffuse)
	{
		return false;
	}
	wstring diffuseTexurePath = filename + wstring(L"_diffuse.dds");
	// Initialize the texture object.
	bool result = m_TextureDiffuse->Initialize(device, diffuseTexurePath.c_str());
	if(!result)
	{
		return false;
	}

	// specular texture
	m_TextureSpecular = new TextureClass;
	if (!m_TextureSpecular)
	{
		return false;
	}
	wstring specularTexurePath = filename + wstring(L"_spec.dds");

	result = m_TextureSpecular->Initialize(device, specularTexurePath.c_str());
	if (!result)
	{
		return false;
	}
	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_TextureDiffuse)
	{
		m_TextureDiffuse->Shutdown();
		delete m_TextureDiffuse;
		m_TextureDiffuse = nullptr;
	}

	if (m_TextureSpecular)
	{
		m_TextureSpecular->Shutdown();
		delete m_TextureSpecular;
		m_TextureSpecular = nullptr;
	}
}


bool ModelClass::LoadModel(char* filename)
{
	m_modelObj.reset(new Model(filename));

	m_vertexCount = m_modelObj->nverts();
	m_indexCount = m_modelObj->nfaces() * 3;
	m_indices.reserve(m_indexCount);
	// Create the model using the vertex count that was read in.
	m_model.reset(new ModelType[m_vertexCount]);
	if (!m_model)
	{
		return false;
	}

	for (int i = 0; i < m_modelObj->nfaces(); i++) {
		std::vector<int> face = m_modelObj->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v = m_modelObj->vert(face[j]);
			// vertex coord
			m_model[face[j]].x = v.x;
			m_model[face[j]].y = v.y;
			m_model[face[j]].z = v.z;
			// norm coord
			auto n = m_modelObj->norm(i, j);
			m_model[face[j]].nx = n.x;
			m_model[face[j]].ny = n.y;
			m_model[face[j]].nz = n.z;
			// uv coord
			auto uv = m_modelObj->uv(i, j);
			m_model[face[j]].tu = uv.x;
			m_model[face[j]].tv = -uv.y;

			m_indices.push_back(face[j]);
		}
	}
	assert(m_indexCount == m_indices.size());

	return true;
}


void ModelClass::ReleaseModel()
{
	m_model.reset();
	m_modelObj.reset();

}