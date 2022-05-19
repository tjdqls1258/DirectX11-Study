#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result; 
	
	// system 객체를 생성한다. 
	System = new SystemClass; //input => 사용자의 키보드/ 마우스 입력,  그래픽 => 그려주는 클래스 -> 랜더링 
	if(!System)
	{ 
		return 0; 
	} 
	
	// system 객체를 초기화하고 run을 호출한다. 
	result = System->Initalize(); 
	if(result) 
	{
		System->Run(); 
	} 
	
	// system객체를 종료하고 메모리를 반환한다. 
	System->Shutdown(); 
	delete System; 
	System = 0;

	return 0; 
}