#pragma once
#include <memory>
#include <vector>
#include <d3d12.h>

struct D3D12_ROOT_PARAMETER;
struct D3D12_DESCRIPTOR_RANGE;

struct RootSignatureDescBundle
{
	D3D12_ROOT_SIGNATURE_DESC desc;
	std::vector<D3D12_ROOT_PARAMETER> param;
	std::vector<D3D12_DESCRIPTOR_RANGE> range;
};