#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class InputClass
{
private:
	bool m_keys[256];

public: 
	InputClass(); 
	InputClass(const InputClass&); 
	~InputClass(); 

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);
};

#endif // !_INPUTCLASS_H_