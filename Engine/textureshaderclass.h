////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
// code based on rastertek tutirial 7
// http://www.rastertek.com/dx11tut07.html
// 
///////////////////////////////////////////////////////////////////////////////////
//	    Displaytexure demo https://github.com/alex-leleka/displaytexure          //
//																				 //
//      Alex Leleka (c) 2016                                                     //
///////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShaderClass
{
private:
	struct ConstantBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct PsConstantBufferType
	{
		D3DXVECTOR4 dir;
		int nBlurPatternIndex;
		D3DXVECTOR3 padding;
	};

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND, int screenWidth, int screenHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void SetBlurPatternIndex(size_t index);
	void SetBlurDirection(int nBlurDirHV);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_constantBuffer;
	ID3D11Buffer* m_constantBufferPs;
	ID3D11SamplerState* m_sampleState;
	size_t m_nBlurPatternIndex;
	int m_nBlurDirHV;
	D3DXVECTOR2 m_textureSize;
};

#endif