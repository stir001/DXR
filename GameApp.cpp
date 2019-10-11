#include "GameApp.h"

#include <Windows.h>
#include <dxcapi.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <string>
#include <vector>
#include "Helper/Comptr.h"
#include "Shader/DirectXCompiler.h"
#include "Shader/DxilEntryPoints.h"
#include "D3DCreateHelper.h"

const std::wstring SHADER_PATH = L"RGShader.hlsl";
const std::wstring TARGET = L"lib_6_3";

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

void GameApp::Init()
{
	HRESULT hr = 0;
	DirectXCompiler compiler;
	auto blob = compiler.Compile(SHADER_PATH, TARGET);

	const unsigned int ENTRY_POINT_COUNT = 3U;

	////エントリーポイントの登録
	std::vector<std::wstring> entryPoints(ENTRY_POINT_COUNT);
	entryPoints[0] = L"rayGen";
	entryPoints[1] = L"miss";
	entryPoints[2] = L"chs";

	DxilEntryPoints dxil(entryPoints, blob);

	auto dxgiFactory = d3d_create_helper::CreateDxgiFactory4();

	auto device = d3d_create_helper::CreateDevice5(dxgiFactory);

#ifdef _DEBUG
	MWCptr<ID3D12Debug>debugLayer;
	hr = device->QueryInterface<ID3D12Debug>(&debugLayer);
	if (debugLayer != nullptr)
	{
		debugLayer->EnableDebugLayer();
	}
	else
	{
		hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
		if (debugLayer != nullptr)
		{
			debugLayer->EnableDebugLayer();
			hr = device->GetDeviceRemovedReason();
			d3d_create_helper::D3DError(hr);
		}
	}

#endif

	MWCptr<ID3D12CommandAllocator> cmdAllocator = d3d_create_helper::CreateCommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	MWCptr<ID3D12GraphicsCommandList4> rayTraceCommandList = d3d_create_helper::CreateGraphicsCommandList4(device, cmdAllocator, D3D12_COMMAND_LIST_TYPE_DIRECT);

}

void GameApp::Run()
{
}

void GameApp::Terminate()
{
}
