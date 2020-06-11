#define MULTI_GEOMETRY_HIT_GROUP_INDEX (3)
#define RAY_RECURSIVE_DEPTH (7)
#define LIGHT_WEIGHT (0.01f)
#define TMIN_VALUE (0.01f)
#define REFLECT_INDEX (0.1f)

GlobalRootSignature GlobalRootSig =
{
	"DescriptorTable(SRV(t0), CBV(b0))"
};

LocalRootSignature RayGenRootSig =
{
	"DescriptorTable(UAV(u0))"
};

LocalRootSignature EmptyRootSig =
{
	" "
};

LocalRootSignature ChessRootSig =
{
	"DescriptorTable(SRV(t1),CBV(b2))"
};

LocalRootSignature BoardRootSig =
{
	"DescriptorTable(CBV(b1))"
};

TriangleHitGroup DefaulltHitGroup = {
	"",
	"chs"
};

TriangleHitGroup ChessHitGroup = {
	"",
	"chessChs"
};

TriangleHitGroup TransHitGroup = {
	"",
	"transChs"
};

TriangleHitGroup ReflectHitGroup = {
	"",
	"reflectChs"
};

TriangleHitGroup BoardTransHitGroup = {
	"",
	"boardTransChs"
};

TriangleHitGroup BoardRefHitGroup = {
	"",
	"boardRefChs"
};

TriangleHitGroup BoardHitGroup = {
	"",
	"boardChs"
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

SubobjectToExportsAssociation ChessHitAssociation = {
	"ChessRootSig",
	"ChessHitGroup"
};

SubobjectToExportsAssociation ChessReflectAssociation = {
	"ChessRootSig",
	"ReflectHitGroup"
};

SubobjectToExportsAssociation TransHitAssociation = {
	"ChessRootSig",
	"TransHitGroup"
};

SubobjectToExportsAssociation BoardTransAssociation = {
	"BoardRootSig",
	"BoardTransHitGroup"
};

SubobjectToExportsAssociation BoardRefAssociation = {
	"BoardRootSig",
	"BoardRefHitGroup"
};

SubobjectToExportsAssociation BoardHitAssociation = {
	"BoardRootSig",
	"BoardHitGroup"
};

RaytracingShaderConfig ShaderConfig = {
	28,
	8,
};

RaytracingPipelineConfig PipelineConfig = {
	RAY_RECURSIVE_DEPTH
};

struct CameraConstant
{
	matrix screenToWorld;
	float4 pos;
};

struct BoardConstant
{
	float3 color;
};

struct ChessMaterial
{
	float4 color;
};

struct RayPayload
{
	float3 color;
	uint recursive;
	int normalSign;
	float refractive;
	float reflectIndex;
};

//GlobalRootSignature
ConstantBuffer<CameraConstant> camera : register(b0);
RaytracingAccelerationStructure gRtScene : register(t0);

//RayGenerationRootSignature
RWTexture2D<float4> gOutput : register(u0);

//BoadrRootSignature
ConstantBuffer<BoardConstant> board : register(b1);

//ChessRootSigantrue
StructuredBuffer<float3> normals : register(t1);
ConstantBuffer<ChessMaterial> chess : register(b2);

float3 linearToSrgb(float3 c)
{
	// Based on http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
	float3 sq1 = sqrt(c);
	float3 sq2 = sqrt(sq1);
	float3 sq3 = sqrt(sq2);
	float3 srgb = 0.662002687 * sq1 + 0.684122060 * sq2 - 0.323583601 * sq3 - 0.0225411470 * c;
	return srgb;
}

float3 hitPos()
{
	return WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
}

matrix rotaAxis(float3 axis, float rad)
{
	float c = cos(rad);
	float s = sin(rad);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	return matrix(
		c + x * x * (1 - c)		, x * y * (1 - c) - z * s	, z * x * (1 - c) + y * s	, 0,
		x * y * (1 - c) + z * s	, c + y * y * (1 - c)		, y * z * (1 - c) - z * s	, 0,
		z * x * (1 - c) - y * s	, y * z * (1 - c) + x * s	, c + z * z * (1 - c)		, 0,
		0						, 0							, 0							, 1);
}

float3 lighting(float3 baseColor, float3 normal)
{
	float3 light = normalize(float3(-0.5f, -1.0f, 0.5f));
	float3 refVec = reflect(light, normal);
	float s = saturate(dot(normalize(camera.pos.xyz - hitPos()), refVec));
	float3x4 mat = ObjectToWorld3x4();
	mat._14_24_34 = 0.0f;
	float cos = abs(dot(-light, mul(mat, float4(normal, 1.0f)).xyz));
	return saturate(linearToSrgb((baseColor * cos * (cos + 0.01f) + baseColor * 0.1f + baseColor * pow(s,15))));
}

float3 transLighting(float3 baseColor, float alpha, float transColor)
{
	return saturate(baseColor * alpha + transColor * (1.0f - alpha));
}

[shader("raygeneration")]
void rayGen()
{
	float2 launchIndex = DispatchRaysIndex().xy + 0.5f;
	float2 launchDim = DispatchRaysDimensions().xy;
	float2 screen = launchIndex / launchDim.xy * 2.0f - 1.0f;//左上(0,0)→↓+
	screen.y *= -1;
	//yは下が+の座標系になっているので-1を掛けている
	//float4 worldPos = mul(camera.screenToWorld, float4(screen.x / (launchDim.x / 2.0f), -screen.y / (launchDim.y / 2.0f), 1.0f, 1.0f));
	float4 worldPos = mul(camera.screenToWorld, float4(screen, 1.0f, 1.0f));

	worldPos = worldPos / worldPos.w;

	RayDesc ray;
	ray.Origin = camera.pos.xyz;
	ray.Direction = normalize(float3(worldPos.xyz - camera.pos.xyz));
	ray.TMin = 0;
	ray.TMax = 100000;

	RayPayload payload;
	payload.color = float3(1.0f, 1.0f, 1.0f);
	payload.refractive = 1.05f;
	payload.reflectIndex = REFLECT_INDEX;
	payload.recursive = 0;
	payload.normalSign = 1;
	//AS, RayFlags, InstanceInclusionMask, RayContributionToHitGroupIndex, MultiplierForGeometryContributionToShaderIndex, MissShaderIndex, Ray, PayLoad
	TraceRay(gRtScene, 0 /*rayFlags*/, 0xFF, 0 /* ray index*/, MULTI_GEOMETRY_HIT_GROUP_INDEX, 0, ray, payload);
	float3 col = payload.color;
	gOutput[launchIndex.xy] = float4(col, 1);
}

#define MISS_COLOR (float3(1.0f,1.0f,1.0f))

[shader("miss")]
void miss(inout RayPayload payload)
{
	payload.color = MISS_COLOR;
}

[shader("miss")]
void transMiss(inout RayPayload payload)
{
	//payload.color = MISS_COLOR;
}

[shader("miss")]
void reflectMiss(inout RayPayload payload)
{
	//payload.color = MISS_COLOR;
}

[shader("closesthit")]
void chessChs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
	//payload.color = transLighting(lighting(chess.color.xyz, normals[PrimitiveIndex()] * payload.normalSign), chess.color.w, payload.color);
	payload.color = lighting(chess.color.xyz, normals[PrimitiveIndex()] * payload.normalSign);
}

[shader("closesthit")]
void transChs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
	float3x4 m = ObjectToWorld3x4();
	m._14_24_34 = 0.0f;
	float3 n = mul(m, normals[PrimitiveIndex()]) * payload.normalSign;
	++payload.recursive;
	RayDesc ray;
	ray.Origin = hitPos();
	ray.TMin = TMIN_VALUE;
	ray.TMax = 100000.0f;
	float3 worldRay = WorldRayDirection();
	float3 refRay = reflect(worldRay, normals[PrimitiveIndex()] * payload.normalSign);

	RayPayload secondPayload;
	secondPayload.color = lighting(chess.color.xyz, normals[PrimitiveIndex()] * payload.normalSign);
	secondPayload.recursive = payload.recursive;
	secondPayload.normalSign = payload.normalSign;
	secondPayload.reflectIndex = REFLECT_INDEX;

	if (dot(n, -worldRay) > 0.1f)
	{
		ray.Direction = refract(worldRay, n, payload.refractive);
		payload.refractive = 1.0f / payload.refractive;

		payload.normalSign *= -1;

		TraceRay(gRtScene, 0, 0xff, (payload.recursive / (RAY_RECURSIVE_DEPTH - 2)) * 2, MULTI_GEOMETRY_HIT_GROUP_INDEX, 1, ray, payload);//屈折光
		payload.color = transLighting(lighting(chess.color.xyz, normals[PrimitiveIndex()] * payload.normalSign), chess.color.w, payload.color);
	}

	ray.Direction = refRay;
	TraceRay(gRtScene, 0, 0xff, (secondPayload.recursive / (RAY_RECURSIVE_DEPTH - 2)) * 1 + 1, MULTI_GEOMETRY_HIT_GROUP_INDEX, 2, ray, secondPayload);//反射光
	payload.color = payload.color * (1.0f - secondPayload.reflectIndex) + secondPayload.color * (secondPayload.reflectIndex);
}

[shader("closesthit")]
void reflectChs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
	float3 rayDir = WorldRayDirection();
	float3 refRay = reflect(rayDir, normals[PrimitiveIndex()]);

	RayDesc ray;
	ray.Origin = hitPos();
	ray.Direction = refRay;
	ray.TMin = TMIN_VALUE;
	ray.TMax = 100000.0f;

	++payload.recursive;
	TraceRay(gRtScene, 0, 0xFF, (payload.recursive / (RAY_RECURSIVE_DEPTH - 2)) * 1 + 1, MULTI_GEOMETRY_HIT_GROUP_INDEX, 2, ray, payload);
	payload.color = lighting(chess.color.xyz, normals[PrimitiveIndex()] * payload.normalSign) * (1.0f - payload.reflectIndex) + payload.color * payload.reflectIndex;
}

[shader("closesthit")]
void boardTransChs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
	float3 n = float3(0.0f, 1.0f, 0.0f);
	float3 rayDir = WorldRayDirection();
	float3 refRay = reflect(rayDir, n);

	RayDesc ray;
	ray.Origin = hitPos();
	ray.Direction = refRay;
	ray.TMin = TMIN_VALUE;
	ray.TMax = 100000.0f;
	++payload.recursive;
	TraceRay(gRtScene, 0, 0xFF, (payload.recursive / (RAY_RECURSIVE_DEPTH - 2)) * 2, MULTI_GEOMETRY_HIT_GROUP_INDEX, 1, ray, payload);
	payload.color = lighting(board.color.xyz, n) * (1.0f - 0.5f) + payload.color * (0.5f);
}

[shader("closesthit")]
void boardRefChs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
	float3 n = float3(0.0f, 1.0f, 0.0f);
	float3 rayDir = WorldRayDirection();
	float3 refRay = reflect(rayDir, n);

	RayDesc ray;
	ray.Origin = hitPos();
	ray.Direction = refRay;
	ray.TMin = TMIN_VALUE;
	ray.TMax = 100000.0f;

	++payload.recursive;
	TraceRay(gRtScene, 0, 0xFF, ((payload.recursive + (RAY_RECURSIVE_DEPTH - 2) / 2) / (RAY_RECURSIVE_DEPTH - 2)) * 2, MULTI_GEOMETRY_HIT_GROUP_INDEX, 2, ray, payload);
	payload.color = lighting(board.color.xyz, n) * (1.0f - 0.5f) + (payload.color) * (0.5f);
}

[shader("closesthit")]
void boardChs(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes attribs)
{
	float3 n = float3(0.0f, 1.0f, 0.0f);
	payload.color = lighting(board.color.xyz, n);
}