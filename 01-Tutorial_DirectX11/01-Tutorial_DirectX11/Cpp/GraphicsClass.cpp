#include "GraphicsClass.h"

GraphicsClass::GraphicsClass() 
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_Bitmap = 0;
	m_Text = 0;
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
	D3DXMATRIX baseViewMatrix;

	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	result = m_D3D->Initalize(screenHeight, screenWidth, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D)", L"Error", MB_OK);
		return false;
	}

	// 카메라 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 위치설정
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	/*
	//모델 생성
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	//모델 설정
	result = m_Model->Initialize(m_D3D->GetDevice(),"../01-Tutorial_DirectX11/Data/Cube.txt",L"../01-Tutorial_DirectX11/Data/05Week.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	*/
	//텍스쳐 쉐이더 오브젝트 생성
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// 텍스쳐 쉐이더 오브젝트 초기화.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	//조명
	/*
	//조명 셰이더 객체 생성 
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	//조명 쉐이더 객체 초기화
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	//조명 객체 생성
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	//조명 설정(생상 및 방향)
	m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(0.0f);
	*/
	//비트맵 생성
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	//비트맵 초기화
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../01-Tutorial_DirectX11/Data/05Week.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"비트맵 오브젝트 생성 실패.", L"Error", MB_OK);
		return false;
	}

	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	//텍스트 오브젝트 생성
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"텍스트 오브젝트 생성 실패.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the color shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
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

	// 프레임마다 회전
	rotation += (float)D3DX_PI * 0.001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true; 
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	//D3DXMatrixRotationYawPitchRoll(&worldMatrix, rotation, 0.0f, 0.0f);

	//// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model->Render(m_D3D->GetDeviceContext());

	//// Render the model using the color shader.
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor() ,m_Light->GetDiffuseColor(),m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	//if (!result)
	//{
	//	return false;
	//}

	m_D3D->GetOrthoMatrix(orthoMatrix);

	//2D 랜더링 구간 (Z버퍼 Off)
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 256, 128);
	if (!result)
	{
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	m_D3D->TurnZBufferOn();//Z버퍼 온

	m_D3D->EndScene();

	return true; 
}