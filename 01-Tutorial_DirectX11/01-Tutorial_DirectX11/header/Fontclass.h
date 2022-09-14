#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;

#include "Textureclass.h"

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	FontType* m_Font;
	TextureClass* m_Texture;

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
};

#endif // !_FONTCLASS_H_
