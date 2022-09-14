cbuffer PerFrameBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


PixelInputType FontVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    //행렬단위를 4단위로 바꿈 (Vector3 -> Cector4 이런 느낌?).
    input.position.w = 1.0f;

    //세계, 뷰 및 투영 행렬에 대한 정점의 위치를 ​​계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    //픽셀 셰이더의 텍스처 좌표를 저장
    output.tex = input.tex;
    
    return output;
}