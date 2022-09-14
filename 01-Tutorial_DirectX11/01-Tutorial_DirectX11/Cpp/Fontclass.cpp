#include "FontClass.h"

FontClass::FontClass()
{
	m_Font = 0;
	m_Texture = 0;
}


FontClass::FontClass(const FontClass& other)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;


	// 텍스쳐 파일 로드
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	//텍스쳐 로드
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	//텍스쳐 해제
	ReleaseTexture();

	//데이타 해제.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;
	//폰트 담는 버퍼 생성
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}
	//파일을 읽어옴
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	//95개의 글자 배치
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// 파일 닫기.(중요)
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// 폰트 글자 담은 버퍼 해제
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	//텍스쳐 생성
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//텍스쳐 초기화
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	//텍스쳐 해제.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	//받은 vertices를 형변환
	vertexPtr = (VertexType*)vertices;

	//글자수
	numLetters = (int)strlen(sentence);

	//배열 초기화
	index = 0;
	//사각형 안에 각 문자를 그림( 택스쳐를 입힘.)
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		//문자가 공백일 경우 3픽셀 이동
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			//1번 삼각형
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			//2번 삼각형
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			//X위치 업데이트
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}

	return;
}