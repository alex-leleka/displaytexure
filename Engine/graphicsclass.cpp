////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "Rendertextureclass.h"
#include "textureshaderclass.h"
#include "DisplayTexture.h"


GraphicsClass::GraphicsClass() : m_TextureShader(0), m_DisplayTexture(0), m_texViewCamera(0)
{
	m_D3D = 0;
	m_worldCamera = 0;
	m_RenderTexture = 0;
	m_RenderTexture2 = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_worldCamera = new CameraClass;
	if(!m_worldCamera)
	{
		return false;
	}

	m_texViewCamera = new CameraClass(D3DXVECTOR3(0, -1, 0)); // when you looking at screen "up" is opposite to Y axis
	if(!m_texViewCamera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_worldCamera->SetPosition(0.0f, 0.0f, 3.0f);
	m_texViewCamera->SetPosition(0.0f, 0.0f, 3.0f);

	
	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	//result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/seafloor.dds");
	result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/obj/african_head.obj", L"../Engine/data/obj/african_head");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}


	m_RenderTexture2 = new RenderTextureClass;
	if (!m_RenderTexture2)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture2->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, -1.0f);

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_DisplayTexture = new DisplayTexture; // TODO: covert to auto ptr

	m_DisplayTexture->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_DisplayTexture)
	{
		m_DisplayTexture->Shutdown();
		delete m_DisplayTexture;
		m_DisplayTexture = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	if (m_RenderTexture2)
	{
		m_RenderTexture2->Shutdown();
		delete m_RenderTexture2;
		m_RenderTexture2 = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_worldCamera)
	{
		delete m_worldCamera;
		m_worldCamera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.001f;
	if (rotation > 2 * (float)D3DX_PI)
	{
		rotation -= 2 * (float)D3DX_PI;
	}
	
	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::RenderScene(float rotation)
{
	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
	bool result;
	m_worldCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.

	m_worldCamera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.

	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Model->GetTextureDiffuse(), m_Model->GetTextureSpecular(), m_Light->GetDirection(), m_Light->GetDiffuseColor(),
		m_worldCamera->GetPosition());

	return result;
}

bool GraphicsClass::Render(float rotation)
{
	// Render the entire scene to the texture first.
	bool bresult = RenderToTexture(m_RenderTexture, [this](){return RenderScene(0);});
	if (!bresult)
	{
		return false;
	}
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_texViewCamera->Render();
	m_texViewCamera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// apply blur in two steps
	// Horizontal blur
	bresult = RenderToTexture(m_RenderTexture2, [&,this](){return m_DisplayTexture->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix,
		projectionMatrix, orthoMatrix, m_TextureShader, m_RenderTexture, true);});
	if (!bresult)
	{
		return false;
	}
	// Vertical blur
	bresult = RenderToTexture(m_RenderTexture, [&,this](){return m_DisplayTexture->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix,
		projectionMatrix, orthoMatrix, m_TextureShader, m_RenderTexture2, false);});
	if (!bresult)
	{
		return false;
	}
	//bresult = RenderToTexture(m_RenderTexture2, [&,this](){return m_DisplayTexture->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix,
	//	projectionMatrix, orthoMatrix, m_TextureShader, m_RenderTexture, false);});

	SetBackBufferRT();
	m_D3D->BeginScene(0.0f, 1.0f, 0.0f, 1.0f);
	m_DisplayTexture->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix,
		projectionMatrix, orthoMatrix, m_TextureShader, m_RenderTexture, true);
	// Generate the view matrix based on the camera's position.
	//if (!RenderScene(rotation)) 
	//	return false;
	//RenderScene(0);
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::SetBackBufferRT()
{
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();
}

bool GraphicsClass::RenderToTexture(RenderTextureClass * pRenderTexture, std::function<bool()> render)
{
	bool result;


	// Set the render target to be the render to texture.
	pRenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the render to texture.
	pRenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	result = render();//RenderScene(0);
	if (!result)
	{
		return false;
	}

	return true;
}