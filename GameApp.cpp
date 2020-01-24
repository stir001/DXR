#include "GameApp.h"

#include <string>
#include "Shader/DirectXCompiler.h"
#include "Shader/DxilEntryPoints.h"
#include "RayTraceRenderer.h"
#include "GameObject.h"
#include "D3DDescriptorHeap.h"
#include "Shader/ShaderTable.h"
#include "GameController.h"

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

void GameApp::Init(HINSTANCE hInst)
{
#ifdef _DEBUG
	ID3D12Debug* debug;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
	debug->Release();
#endif // _DEBUG

	mRenderer = std::make_unique<RayTraceRenderer>(hInst);
	auto size = mRenderer->GetWindowSize();
	mController = std::make_shared<GameController>(mRenderer->CreateDxInput(), mRenderer->GetCamera(), mRenderer->GetGameObjects(), Vector2(size.width, size.height));

}

void GameApp::Run()
{
	while (ProcessMessage())
	{
		mRenderer->BeginFrame();
		mController->Update();
		mRenderer->Render();
	}
}

void GameApp::Terminate()
{
}

bool GameApp::ProcessMessage()
{
	MSG msg = {};
	bool rtn = true;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		rtn = false;
	}

	return rtn;
}
