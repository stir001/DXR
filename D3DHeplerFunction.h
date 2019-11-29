#pragma once
#include "Comptr.h"

struct ID3D12GraphicsCommandList4;
struct ID3D12Resource;
enum D3D12_RESOURCE_STATES : int;

namespace d3d_helper_function
{
	void TlansitoinBarrier(const MWCptr<ID3D12GraphicsCommandList4>& cmdList, const MWCptr<ID3D12Resource> resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
};

