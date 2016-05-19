////////////////////////////////////////////////////////////////////////////////
// Filename: DisplayTexture.h
////////////////////////////////////////////////////////////////////////////////
/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/

#ifndef _DisplayTexture_H_
#define _DisplayTexture_H_
#include <d3d11.h>
#include <d3dx10math.h>

class TextureShaderClass;
class RenderTextureClass;
class DisplayTexture
{
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	DisplayTexture(const DisplayTexture &){};
public:
	DisplayTexture();
	bool Render(ID3D11DeviceContext* param1, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX orthoMatrix, TextureShaderClass* pTextureShader, RenderTextureClass* pRenderTexture, bool bBlurDirH);
	void Shutdown();
	bool Initialize(ID3D11Device* param1, HWND hwnd);

private:
	void ShutdownBuffers();

	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	unsigned long m_vertexCount;
	unsigned long m_indexCount;
};

#endif