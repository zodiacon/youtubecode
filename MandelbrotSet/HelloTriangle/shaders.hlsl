//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

float2 from : register(b0);
float2 to : register(b1);

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct VSInput {
    float4 position : POSITION;
    float4 color : COLOR;
};

PSInput VSMain(VSInput input)
{
    PSInput result;

    result.position = input.position;
    result.color = input.color;

    return result;
}

int MandelbrotColor(float2 c) {
    float2 z = 0;
    int count = 255;

    while (count > 0 && z.x * z.x + z.y * z.y <= 4) {
        float2 t = z;
		// z = z * z + c
        z.x = t.x * t.x - t.y * t.y + c.x;
        z.y = 2 * t.x * t.y + c.y;
        count--;
    }
    return count;
}

int Mandelbrot(float2 pos, float2 size) {
    float2 c = from + (to - from) * pos / size;
    return MandelbrotColor(c);
}

float4 PSMain(PSInput input) : SV_TARGET
{
    int count = Mandelbrot(input.position.xy, float2(1600, 900));
    return float4(count / 255.0f, count / 255.0f, count / 255.0f, 1.0);
}

float4 PSRectMain(PSInput input) : SV_TARGET
{
    return input.color;
}
