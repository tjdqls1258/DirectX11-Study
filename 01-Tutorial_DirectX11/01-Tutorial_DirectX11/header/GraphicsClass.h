#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "TextureShaderClass.h"
#include "lightshaderclass.h"
#include "Lightclass.h"
#include "Bitmapclass.h"
#include "Textclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true; 
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;

public: 
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	bool Frame();

	void Shutdown(); 

private:
	bool Render(float);
};

#endif // !_GRAPHICSCLASS_H_