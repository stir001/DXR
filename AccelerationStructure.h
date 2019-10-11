#pragma once
#include <dxgiformat.h>
#include "Comptr.h"

struct ID3D12Device5;
struct ID3D12GraphicsCommandList4;
struct ID3D12Resource;
struct D3D12_RAYTRACING_GEOMETRY_DESC;
enum D3D12_RAYTRACING_GEOMETRY_FLAGS;

class AccelearationStructure
{
public:
	AccelearationStructure();
	~AccelearationStructure();

	struct TriangleBuffers
	{
		MWCptr<ID3D12Resource> vertexBuffer;
		unsigned int vertexStride;
		DXGI_FORMAT vertexFormat;
		unsigned int vertexCount;
		MWCptr<ID3D12Resource> indexBuffer;
		DXGI_FORMAT indexFormat;
		unsigned int indexCount;
	};

	struct ASBuffer
	{
		MWCptr<ID3D12Resource> scratch;
		MWCptr<ID3D12Resource> result;
		MWCptr<ID3D12Resource> instanceDesc;
	};

	static ASBuffer InitTopLevelAS(MWCptr<ID3D12Device5>& device, MWCptr< ID3D12GraphicsCommandList4>& commandList, MWCptr<ID3D12Resource>& bottomLevelAS);
	static ASBuffer InitBottomLevelTriangleAS(MWCptr<ID3D12Device5>& device, MWCptr<ID3D12GraphicsCommandList4>& commandList, const D3D12_RAYTRACING_GEOMETRY_DESC* geometrys, unsigned int geometryCount);
	static D3D12_RAYTRACING_GEOMETRY_DESC CreateTriangleGeometryDesc(const TriangleBuffers& buffers, D3D12_RAYTRACING_GEOMETRY_FLAGS& flags);
private:
};

