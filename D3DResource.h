#pragma once
#include "Comptr.h"

struct ID3D12Resource;

struct D3DResource
{
	MWCptr<ID3D12Resource> resource;
	unsigned int heapIndex;
};

