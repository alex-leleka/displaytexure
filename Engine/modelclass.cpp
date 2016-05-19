////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
// code based on rastertek tutirial 7
// http://www.rastertek.com/dx11tut07.html
// 
/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/
#include "modelclass.h"
#include "model.h"

#include <cassert>
#include <fstream> 
#include <string> 

ModelClass::ModelClass() : m_indices(nullptr)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_TextureDiffuse = 0;
	m_TextureSpecular = 0;
	m_model = 0;
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
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	//indices = new unsigned long[m_indexCount];
	//if(!indices)
	//{
	//	return false;
	//}

	// Load the vertex array and index array with data.
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
    vertexData.pSysMem = vertices;
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
	indexData.pSysMem = m_indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	//delete [] indices;
	//indices = 0;

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

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	using std::wstring;
	bool result;


	// Create the diffuse texture object.
	m_TextureDiffuse = new TextureClass;
	if(!m_TextureDiffuse)
	{
		return false;
	}
	wstring diffuseTexurePath = filename + wstring(L"_diffuse.dds");
	// Initialize the texture object.
	result = m_TextureDiffuse->Initialize(device, diffuseTexurePath.c_str());
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
		m_TextureDiffuse = 0;
	}

	if (m_TextureSpecular)
	{
		m_TextureSpecular->Shutdown();
		delete m_TextureSpecular;
		m_TextureSpecular = 0;
	}

	return;
}


bool ModelClass::LoadModel(char* filename)
{
	m_modelObj = new Model(filename);

	m_vertexCount = m_modelObj->nverts();
	m_indexCount = m_modelObj->nfaces() * 3;
	std::vector<unsigned long> indexes;
	indexes.reserve(m_indexCount);
	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	for (int i = 0; i<m_modelObj->nfaces(); i++) {
		std::vector<int> face = m_modelObj->face(i);
		Vec3i screen_coords[3];
		Vec3f world_coords[3];
		float intensity[3];
		for (int j = 0; j<3; j++) {
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
			indexes.push_back(face[j]);
		}
	}
	m_indices = new unsigned long[m_indexCount];
	assert(m_indexCount == indexes.size());
	for (unsigned i = 0; i < indexes.size(); ++i)
		m_indices[i] = indexes[i];

	return true;
}


void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}
	if (m_modelObj)
	{
		delete m_modelObj;
		m_modelObj = nullptr;
	}

	return;
}