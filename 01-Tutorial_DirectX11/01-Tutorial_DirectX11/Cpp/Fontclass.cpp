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


	// �ؽ��� ���� �ε�
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	//�ؽ��� �ε�
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	//�ؽ��� ����
	ReleaseTexture();

	//����Ÿ ����.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;
	//��Ʈ ��� ���� ����
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}
	//������ �о��
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	//95���� ���� ��ġ
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

	// ���� �ݱ�.(�߿�)
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// ��Ʈ ���� ���� ���� ����
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


	//�ؽ��� ����
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//�ؽ��� �ʱ�ȭ
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	//�ؽ��� ����.
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


	//���� vertices�� ����ȯ
	vertexPtr = (VertexType*)vertices;

	//���ڼ�
	numLetters = (int)strlen(sentence);

	//�迭 �ʱ�ȭ
	index = 0;
	//�簢�� �ȿ� �� ���ڸ� �׸�( �ý��ĸ� ����.)
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		//���ڰ� ������ ��� 3�ȼ� �̵�
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			//1�� �ﰢ��
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			//2�� �ﰢ��
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			//X��ġ ������Ʈ
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}

	return;
}