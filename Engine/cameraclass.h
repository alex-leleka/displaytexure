////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
// code based on rastertek tutirial 7
// http://www.rastertek.com/dx11tut07.html
// 
/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass(const D3DXVECTOR3 & up = D3DXVECTOR3(0, 1, 0));
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);

	D3DXVECTOR3 GetPosition();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	D3DXMATRIX m_viewMatrix;
	D3DXVECTOR3 m_up;
};

#endif