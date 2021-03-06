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
	enum EBlurDir
	{
		EBlurDir_noBlur,
		EBlurDir_H,
		EBlurDir_V,
	};
	DisplayTexture();
	bool Render(ID3D11DeviceContext* , D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, TextureShaderClass* pTextureShader, RenderTextureClass* pRenderTexture, int nBlurDirHV);
	void Shutdown();
	bool Initialize(ID3D11Device* , int textureWidth, int textureHeight);

private:
	void ShutdownBuffers();
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	unsigned long m_vertexCount;
	unsigned long m_indexCount;
};

#endif