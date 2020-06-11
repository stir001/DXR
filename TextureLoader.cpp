
#include "TextureLoader.h"
#include "DirectXTex.h"
#include "CharConvert.h"

#include <d3dx12.h>
#include <tchar.h>
#include <algorithm>

TextureLoader* TextureLoader::mInstance = nullptr;

const float GAMMA_VALUE = 1.0f / 2.2f;

TextureLoader::TextureLoader()
{
	CoInitialize(nullptr);
}

TextureLoader::~TextureLoader()
{
	//mTextures.clear();
}

//void TextureLoader::Release(const std::string & filePath)
//{
//	mTextures.erase(filePath);
//}
//
//std::shared_ptr<TextureObject> TextureLoader::LoadTexture(const std::string& filepath)
//{
//	DirectX::ScratchImage imageData;
//	D3D12_SUBRESOURCE_DATA subresource;
//
//	std::wstring wstrPath;
//	ToWChar(wstrPath, filepath);
//
//	auto tex = mTextures.find(filepath);
//	if (tex != mTextures.end())
//	{
//		return tex->second;
//	}
//	std::shared_ptr<TextureObject> rtn = std::make_shared<TextureObject>();
//
//	HRESULT result = S_OK;
//
//	DirectX::TexMetadata texMetaData = {};
//	result = LoadWIC(wstrPath, texMetaData, imageData);
//	if (FAILED(result))
//	{
//		result = LoadTGA(wstrPath, texMetaData, imageData);
//	}
//
//	if (FAILED(result))
//	{
//		result = LoadDDS(wstrPath, texMetaData, imageData);
//	}
//
//	//if (FAILED(result))
//	//{
//	//	/*#ifdef _DEBUG
//	//	std::wstring outputstr = L"テクスチャパスが違うか対応していない拡張子です\n FilePath: ";
//	//	outputstr += wstrPath;
//	//	MessageBox(nullptr, outputstr.data() , L"CAUTION", MB_OK);
//	//	#endif*/
//	//	CreateNullTexture(rtn);
//	//	rtn->mFilepath = filepath;
//	//	rtn->mTextureName = GetTextureName(wstrPath);
//	//	mTextures[filepath] = rtn;
//	//	return rtn;
//	//}
//	else
//	{
//		subresource.pData = imageData.GetPixels();
//		subresource.RowPitch = imageData.GetImages()->rowPitch;//画像の横幅のバイト単位のサイズ
//		subresource.SlicePitch = imageData.GetImages()->slicePitch;//画像全体のサイズ
//	}
//
//	{
//		//rtn->mTextureName = GetTextureName(wstrPath);
//		/*
//		rtn->mShaderResource = std::make_shared<ShaderResourceObject>(rtn->mTextureName, d12.GetDev(),static_cast<unsigned int>(texMetaData.width),static_cast<unsigned int>(texMetaData.height),texMetaData.format);
//		rtn->mShaderResource->CreateShaderResourceViewDesc();*/
//	}
//
//	//if (IsUseGamma(texMetaData.format))
//	//{
//	//	rtn->mGamma = GAMMA_VALUE;
//	//}
//	//else
//	//{
//	//	rtn->mGamma = 1.0f;
//	//}
//
//	CreateTexUpdateSubResources(rtn);
//
//	/*rtn->mFilepath = filepath;
//	mTextures[filepath] = rtn;*/
//	return rtn;
//}


//void TextureLoader::CreateTexUpdateSubResources(std::shared_ptr<TextureObject>& inTex)
//{
//	Microsoft::WRL::ComPtr<ID3D12Resource> updateBuffer;
//	Microsoft::WRL::ComPtr<ID3D12Resource>& dstResource = inTex->mShaderResource->GetBuffer();
//
//	D3D12_HEAP_PROPERTIES heapProp = {};
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
//	heapProp.CreationNodeMask = 1;
//	heapProp.VisibleNodeMask = 1;
//
//	D3D12_RESOURCE_DESC uploadDesc;
//	uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	uploadDesc.Alignment = 0;
//	uploadDesc.Width = GetRequiredIntermediateSize(dstResource.Get(), 0, 1);
//	uploadDesc.Height = 1;
//	uploadDesc.DepthOrArraySize = 1;
//	uploadDesc.MipLevels = 1;
//	uploadDesc.Format = DXGI_FORMAT_UNKNOWN;
//	uploadDesc.SampleDesc.Count = 1;
//	uploadDesc.SampleDesc.Quality = 0;
//	uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//	uploadDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
//
//	d12.result = d12.GetDev()->CreateCommittedResource(
//		&heapProp, 
//		D3D12_HEAP_FLAG_NONE,
//		&uploadDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&updateBuffer));
//
//	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = d12.GetCmdList()->GetCommandList();
//
//	//https://msdn.microsoft.com/en-us/library/windows/desktop/dn986740(v=vs.85).aspx
//	D3D12_RESOURCE_BARRIER barrier;
//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//	barrier.Transition.pResource = dstResource.Get();
//	barrier.Transition.Subresource = 0;
//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//	cmdList->ResourceBarrier(1, &barrier);
//
//	UINT64 num = UpdateSubresources(cmdList.Get(),
//		dstResource.Get(),
//		updateBuffer.Get(),
//		0,
//		0,
//		1,
//		&inTex->mSubresource);
//
//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
//	cmdList->ResourceBarrier(1, &barrier);
//
//	cmdList->Close();
//
//	d12.GetCmdQueue()->ExecuteCommandList(d12.GetCmdList());
//
//	d12.GetCmdQueue()->Signal();
//	d12.GetCmdQueue()->Wait();
//	
//	d12.GetCmdList()->Reset();
//}
//
//std::string TextureLoader::GetTextureName(const std::wstring& filePath)
//{
//	size_t size = filePath.rfind('/');
//	if (size == std::string::npos)
//	{
//		size = 0;
//	}
//	else
//	{
//		size += 1;
//	}
//	
//	std::wstring wstrbuff(filePath.begin() + size, filePath.end());
//
//	std::string rtn;
//	ToChar(rtn, wstrbuff);
//
//	return rtn;
//}

bool TextureLoader::IsUseGamma(DXGI_FORMAT fomat)
{
	return(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB == fomat ||
		DXGI_FORMAT_BC1_UNORM_SRGB == fomat ||
		DXGI_FORMAT_BC2_UNORM_SRGB == fomat ||
		DXGI_FORMAT_BC3_UNORM_SRGB == fomat ||
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB == fomat ||
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB == fomat ||
		DXGI_FORMAT_BC7_UNORM_SRGB == fomat);
}

HRESULT TextureLoader::LoadWIC(const std::wstring& path, DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage)
{
	return DirectX::LoadFromWICFile(path.data(), 0, &metaData, scratchImage);
}

HRESULT TextureLoader::LoadTGA(const std::wstring& path, DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchmage)
{
	return DirectX::LoadFromTGAFile(path.data(), &metaData, scratchmage);
}

HRESULT TextureLoader::LoadDDS(const std::wstring& path, DirectX::TexMetadata& metaData, DirectX::ScratchImage& scracgImage)
{
	return DirectX::LoadFromDDSFile(path.data(), 0, &metaData, scracgImage);
}
