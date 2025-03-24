cbuffer VSConstBuffer0 : register(b0)
{
    matrix mx;
};

cbuffer VSConstBuffer1 : register(b1)
{
    float4 color;
};

struct VSOut
{
    float4 color : Color;
    float4 pos : SV_Position;
};

VSOut main(float2 pos : Position)
{
    VSOut vso;
    vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), mx);
    vso.color = color;
    return vso;
}