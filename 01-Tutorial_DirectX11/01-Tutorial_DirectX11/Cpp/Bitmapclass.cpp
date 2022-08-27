#include "Bitmapclass.h"

BitmapClass::BitmapClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}

BitmapClass::BitmapClass(const BitmapClass& other)
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	bool result;

	//스크린 사이즈
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//비트맵의 픽셀사이즈
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	//랜더링 위치 초기화
	m_previousPosX = -1;
	m_previousPosY = -1;

	//버퍼 생성
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	//해당 모델의 텍스쳐 불러오기
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void BitmapClass::Shutdown()
{
	//텍스쳐 해제
	ReleaseTexture();

	//버퍼 해제
	ShutdownBuffers();

	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result;

	//버퍼 다시 빌드 
	result = UpdateBuffers(deviceContext, positionX, positionY);
	if (!result)
	{
		return false;
	}
	//그리기 준비
	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	//버택스 수
	m_vertexCount = 6;

	//인덱스 수
	m_indexCount = m_vertexCount;

	//버택스 배열
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//인덱스 배열
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//첫번째 버택스 배열 초기화
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	//인덱스 배열 로드
	for (i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}
	//버퍼 설정
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//버텍스 데이터 설정
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//버텍스 버퍼 생성
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	//인덱스 버퍼 설정
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//인덱스 데이터 설정
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//인덱스 버퍼 생성
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//배열 해제
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	// 인덱스 버퍼 해제
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 버텍스 버퍼 해제
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	//비트맵 밖에 있는 객체 업데이트 X
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}
	//위치 업데이트
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	//비트맵 화면 좌표 계산
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;

	top = (float)(m_screenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	//버택스 배열 생성
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//정점(버택스) 배열 설정
	//첫번째 삼각형
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	//두번째 삼각형
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);
	
	//상수 버퍼 사용
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//정점 버퍼 포인터
	verticesPtr = (VertexType*)mappedResource.pData;

	//정점 버퍼 카피
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	//상수 버퍼 사용해제
	deviceContext->Unmap(m_vertexBuffer, 0);

	//배열 해제
	delete[] vertices;
	vertices = 0;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//버택스 버퍼 설정
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	//텍스쳐 생성
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//초기화
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void BitmapClass::ReleaseTexture()
{
	//해당 오브젝트 제거
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}