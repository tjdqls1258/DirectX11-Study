#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initalize()
{
	int screenWidth, screenHeight;	//화면 넓이 높이
	bool result;					//결과

	screenHeight = 0, screenWidth = 0;

	InitalizeWindows(screenWidth, screenHeight);

	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	m_Input->Initialize();

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run() {
	MSG msg; 
	bool done, result; 
	
	// 메세지 구조체를 초기화합니다. 
	ZeroMemory(&msg, sizeof(MSG)); 
	
	// 유저로부터 종료 메세지를 받을 때까지 루프를 돕니다. 
	done = false;
	while(!done) 
	{ 
		// 윈도우 메세지를 처리합니다. 
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		} 
		// 윈도우에서 어플리케이션의 종료를 요청하는 경우 빠져나갑니다. 
		if(msg.message == WM_QUIT) 
		{
			done = true; 
		} 
		else 
		{ 
			// 그 외에는 Frame 함수를 처리합니다. 
			result = Frame();
			if(!result)
			{
				done = true; 
			}
		}
	}
	return; 
}

bool SystemClass::Frame()
{
	bool result;

	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// 키보드가 키가 눌렸는지 확인합니다. 
		case WM_KEYDOWN:
		{ 
			// 키가 눌렸다면 input객체에 이 사실을 전달하여 기록하도록 합니다. 
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		} 
		case WM_KEYUP:
		{ 
			// 키가 떼어졌다면 input객체에 이 사실을 전달하여 이 키를 해제토록합니다. 
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		} 
		// 다른 메세지들은 사용하지 않으므로 기본 메세지 처리기에 전달합니다. 
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitalizeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL; wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
	screenWidth = GetSystemMetrics(SM_CXSCREEN); screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;
		

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName,
		m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	
	ShowCursor(false);
	return;
}

void SystemClass::ShutdownWindows() 
{
	ShowCursor(true); 

	{
		ChangeDisplaySettings(NULL, 0);
	} 

	DestroyWindow(m_hwnd);
	m_hwnd = NULL; 

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL; 
	return; 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) 
{
	switch (umessage) 
	{ 
		case WM_DESTROY: 
		{
			PostQuitMessage(0); 
			return 0; 
		} 
		case WM_CLOSE: 
		{
			PostQuitMessage(0); 
			return 0; 
		} 
		default: 
		{ 
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam); 
		}
	}
}