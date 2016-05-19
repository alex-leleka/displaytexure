////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
// code based on rastertek tutirial 7
// http://www.rastertek.com/dx11tut07.html
// 
/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include <functional>


/////////////
// GLOBALS //
/////////////
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
class RenderTextureClass;
class TextureShaderClass;
class DisplayTexture;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	void SetShadingModelIndex(size_t);
	void SetBlurPatternIndex(size_t);
private:
	bool Render(float);
	bool RenderToTexture(RenderTextureClass *, std::function<bool()> render );
	bool RenderScene(float rotation);
	void SetBackBufferRT();

private:
	D3DClass* m_D3D;
	CameraClass* m_worldCamera;
	CameraClass* m_texViewCamera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	RenderTextureClass* m_RenderTexture;
	RenderTextureClass* m_RenderTexture2;
	TextureShaderClass* m_TextureShader;
	DisplayTexture* m_DisplayTexture;
};

#endif