#pragma once
#include <vector>
#include "Comptr.h"

struct ID3D12Device5;
struct ID3D12GraphicsCommandList4;
struct ID3D12Resource;
struct D3D12_RAYTRACING_GEOMETRY_DESC;
struct D3D12_RAYTRACING_INSTANCE_DESC;
enum D3D12_RAYTRACING_GEOMETRY_FLAGS;
enum DXGI_FORMAT : int;

class AccelerationStructure
{
public:
	AccelerationStructure();
	~AccelerationStructure();

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

	static ASBuffer InitTopLevelAS(const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& commandList, const std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs);
	static ASBuffer InitBottomLevelTriangleAS(const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& commandList, const D3D12_RAYTRACING_GEOMETRY_DESC* geometrys, unsigned int geometryCount);
	static ASBuffer UpdateTopLevelAS(const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& commandList, const std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, AccelerationStructure::ASBuffer& buffer);
	static D3D12_RAYTRACING_GEOMETRY_DESC CreateTriangleGeometryDesc(const TriangleBuffers& buffers, const D3D12_RAYTRACING_GEOMETRY_FLAGS& flags);
private:
};

