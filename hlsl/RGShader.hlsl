/***************************************************************************
# Copyright (c) 2018, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************/
GlobalRootSignature GlobalRootSig =
{
	"DescriptorTable(CBV(b0))"
};

LocalRootSignature RayGenRootSig =
{
	"DescriptorTable(UAV(u0), SRV(t0))"
};

LocalRootSignature EmptyRootSig =
{
	" "
};

LocalRootSignature NormalRootSig =
{
	"DescriptorTable(SRV(t1))"
};

TriangleHitGroup DefaulltHitGroup = {
	"",
	"chs"
};

TriangleHitGroup NormalHitGroup = {
	"",
	"chessChs"
};

SubobjectToExportsAssociation RayGenAssociation =
{
	"RayGenRootSig",
	"rayGen"
};

SubobjectToExportsAssociation MissAssociation = {
	"EmptyRootSig",
	"miss"
};

SubobjectToExportsAssociation ClosestHitAssociation = {
	"NormalRootSig",
	"NormalHitGroup"
};

RaytracingShaderConfig ShaderConfig = {
	12,
	8,
};

RaytracingPipelineConfig PipelineConfig = {
	1
};

struct CameraConstant
{
	matrix screenToWorld;
	float4 pos;
};

RWTexture2D<float4> gOutput : register(u0);
RaytracingAccelerationStructure gRtScene : register(t0);
StructuredBuffer<float3> normal : register(t1);
ConstantBuffer<CameraConstant> camera : register(b0);

float3 linearToSrgb(float3 c)
{
	// Based on http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
	float3 sq1 = sqrt(c);
	float3 sq2 = sqrt(sq1);
	float3 sq3 = sqrt(sq2);
	float3 srgb = 0.662002687 * sq1 + 0.684122060 * sq2 - 0.323583601 * sq3 - 0.0225411470 * c;
	return srgb;
}

struct RayPayload
{
	float3 color;
};

[shader("raygeneration")]
void rayGen()
{
	int3 launchIndex = DispatchRaysIndex();
	int3 launchDim = DispatchRaysDimensions();
	float2 screen = launchIndex.xy - (launchDim.xy / 2);//ç∂è„(0,0)Å®Å´+
	//yÇÕâ∫Ç™+ÇÃç¿ïWånÇ…Ç»Ç¡ÇƒÇ¢ÇÈÇÃÇ≈-1Çä|ÇØÇƒÇ¢ÇÈ
	float4 worldPos = mul(camera.screenToWorld, float4(screen.x / (launchDim.x / 2.0f), -screen.y / (launchDim.y / 2.0f), 0.0f, 1.0f));

	worldPos = worldPos / worldPos.w;

	RayDesc ray;
	ray.Origin = camera.pos.xyz;
	ray.Direction = normalize(float3(worldPos.xyz - camera.pos.xyz));

	ray.TMin = 0;
	ray.TMax = 100000;

	RayPayload payload;
	TraceRay(gRtScene, 0 /*rayFlags*/, 0xFF, 0 /* ray index*/, 0, 0, ray, payload);
	//float3 col = linearToSrgb(payload.color);
	float3 col = payload.color;
	gOutput[launchIndex.xy] = float4(col, 1);
}

[shader("miss")]
void miss(inout RayPayload payload)
{
	payload.color = float3(0.4, 0.6, 0.2);
}

[shader("closesthit")]
void chs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
	float3 barycentrics = float3(1.0 - attribs.barycentrics.x - attribs.barycentrics.y, attribs.barycentrics.x, attribs.barycentrics.y);

	const float3 A = float3(1, 0, 0);
	const float3 B = float3(0, 1, 0);
	const float3 C = float3(0, 0, 1);

	payload.color = A * barycentrics.x + B * barycentrics.y + C * barycentrics.z;
}

[shader("closesthit")]
void chessChs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
	float3 light = normalize(float3(-1.0f, -1.0f, 1.0f));
	float cos = dot(-light, normal[PrimitiveIndex()]);
	payload.color = float3(1.0f,1.0f,1.0f) * cos;
}