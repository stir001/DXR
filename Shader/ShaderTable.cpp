#include "ShaderTable.h"
#include "D3DCreateHelper.h"
#include "D3DDescriptorHeap.h"
#include "PipelineState/RtPipelineState.h"
#include <functional>
#include <d3d12.h>

ShaderTable::ShaderTable() : mEntrySize(0)
{
}

ShaderTable::~ShaderTable()
{
}

void ShaderTable::AddHitGroupShader(const ShaderInfo& info)
{
	mHitGroupShaders.push_back(info);
}

void ShaderTable::AddRayGenShader(const ShaderInfo& info)
{
	mRayGenShaders.push_back(info);
}

void ShaderTable::AddMissShader(const ShaderInfo& info)
{
	mMissShaders.push_back(info);
}

void ShaderTable::Create(const ShaderTable::InitStructure& initData)
{
	//一番サイズが大きいものに合わせるために
	//heapに登録している数が最も多いものを探す
	unsigned int maxCount = GetMaxHeapCount();

	CreateShaderTable(maxCount, initData.device);

	WriteShaderTable(initData.pipelineState, initData.heap);
}

unsigned long long ShaderTable::GetShaderTableEntrySize() const
{
	return mEntrySize;
}

D3D12_GPU_VIRTUAL_ADDRESS ShaderTable::GetRayGenTableAddress() const
{
	return mShaderTable->GetGPUVirtualAddress();
}

unsigned int ShaderTable::GetRayGenNum() const
{
	return mRayGenShaders.size();
}

D3D12_GPU_VIRTUAL_ADDRESS ShaderTable::GetHitGroupTableAddress() const
{
	return mShaderTable->GetGPUVirtualAddress() + (mRayGenShaders.size() + mMissShaders.size()) * mEntrySize;
}

unsigned int ShaderTable::GetHitGroupNum() const
{
	return mHitGroupShaders.size();
}

D3D12_GPU_VIRTUAL_ADDRESS ShaderTable::GetMissTableAddress() const
{
	return mShaderTable->GetGPUVirtualAddress() + (mRayGenShaders.size()) * mEntrySize;
}

unsigned int ShaderTable::GetMissNum() const
{
	return mMissShaders.size();
}

unsigned int ShaderTable::GetMaxHeapCount() const
{
	std::function getMaxCount = [](const std::vector<ShaderInfo>& info, unsigned int currentMaxCount)->unsigned int
	{
		for (auto& s : info)
		{
			currentMaxCount = currentMaxCount < s.heapIndices.size() ? s.heapIndices.size() : currentMaxCount;
		}
		return currentMaxCount;
	};
	unsigned int heapCount = 0;
	heapCount = getMaxCount(mRayGenShaders, heapCount);
	heapCount = getMaxCount(mHitGroupShaders, heapCount);
	heapCount = getMaxCount(mMissShaders, heapCount);

	return heapCount;
}

void ShaderTable::CreateShaderTable(unsigned int maxHeapCount, const MWCptr<ID3D12Device5>& device)
{
	//D3D12_SHADER_IDENTIFIER_SIZE_BYTES + 8byte(descriptor-tableのためのもの？) * maxHeapCount
	//descriptor-tableを使わなければ8byteはいらない？
	const unsigned int heapPtrSize = 8U;
	const int identifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	mEntrySize = identifierSize + static_cast<unsigned long long>(heapPtrSize) * maxHeapCount;//一番大きいものに合わせてサイズを設定
	//アライメントは必須
	mEntrySize = d3d_create_helper::Alignment(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, mEntrySize);
	auto bufferSize = mEntrySize * (mHitGroupShaders.size() + mRayGenShaders.size() + mMissShaders.size());
	mShaderTable = d3d_create_helper::CreateBuffer(device, bufferSize, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, d3d_create_helper::GetUploadHeapProps());
}

void ShaderTable::WriteShaderTable(const std::shared_ptr<RtPipelineState>& pipelineState, const std::shared_ptr<D3DDescriptorHeap>& heap)
{
	HRESULT hr = S_OK;
	unsigned char* data = nullptr;
	hr = mShaderTable->Map(0, nullptr, reinterpret_cast<void**>(&data));
	d3d_create_helper::D3DError(hr);

	MWCptr<ID3D12StateObjectProperties> rtPsoProps;
	hr = pipelineState->GetPipelineState()->QueryInterface(IID_PPV_ARGS(&rtPsoProps));
	d3d_create_helper::D3DError(hr);

	std::function writeShaderInfo = [&rtPsoProps](const std::vector<ShaderInfo>& shaders, const std::shared_ptr<D3DDescriptorHeap>& heap
		, unsigned char* writePtr, const unsigned long long entrySize)->unsigned char* 
	{
		for (unsigned int i = 0; i <  shaders.size(); ++i)
		{
			auto ptr = writePtr + i * entrySize;
			auto identifier = rtPsoProps->GetShaderIdentifier(shaders[i].name.data());
			memcpy(ptr, identifier, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
			ptr += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
			auto& indices = shaders[i].heapIndices;
			for (unsigned int j = 0; j < indices.size(); ++j)
			{
				auto gpuHandle = heap->GetGpuHandle(indices[j]).ptr;
				memcpy(ptr, &gpuHandle, sizeof(gpuHandle));
				ptr += sizeof(gpuHandle);
			}
		}

		return writePtr + (shaders.size() * entrySize);
	};

	data = writeShaderInfo(mRayGenShaders, heap, data, mEntrySize);
	data = writeShaderInfo(mMissShaders, heap, data, mEntrySize);
	data = writeShaderInfo(mHitGroupShaders, heap, data, mEntrySize);

	mShaderTable->Unmap(0, nullptr);
}
