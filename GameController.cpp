#include "GameController.h"
#include "DxInput.h"
#include "Camera.h"

GameController::GameController(const std::shared_ptr<DxInput>& input, const std::shared_ptr<Camera>& camera
	, std::vector<std::shared_ptr<GameObject>>& objects, const Vector2& wndSize)
	: mInput(input), mCamera(camera), mObjects(objects), mSize(wndSize)
{
}

void GameController::Update()
{
	mInput->UpdateKeyState();
	CameraMove();
}

void GameController::CameraMove()
{
	auto pos = mInput->GetMousePos();
	auto sub = pos - mMousePos;
	auto xRatio = sub.x / mSize.x;
	mCamera->AddRotaYAxis(acos(xRatio));
}
