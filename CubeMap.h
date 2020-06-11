#pragma once
#include "GameObject.h"
class CubeMap :
	public GameObject
{
public:
	CubeMap(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDesc, const MWCptr<ID3D12Device5>& device,
		const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable);
	~CubeMap();
private:
	void Init(const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable);
};

