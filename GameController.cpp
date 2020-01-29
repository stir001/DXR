#include "GameController.h"
#include "DxInput.h"
#include "Camera.h"
#include "GameObject.h"

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
	if (mInput->IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_RBUTTON))
	{
		mInput->SetMousePos(mSize.x * 0.5f, mSize.y * 0.5f);
	}
	else if (mInput->IsKeyDown(VIRTUAL_KEY_INDEX::KEY_RBUTTON))
	{
		auto mousePos = mInput->GetMousePos();
		auto mouseVec = mousePos - Vector2(mSize.x * 0.5f, mSize.y * 0.5f);

		auto bigger = (mSize.x > mSize.y ? mSize.x : mSize.y) / 2.0f;

		auto unit = Vector2(bigger, bigger);
		float pi = 3.14159265f;
		auto yAxisRota = -mouseVec.x / unit.x * 2.0f * pi;
		auto sideAxisRota = mouseVec.y / unit.y * 2.0f * pi;

		mCamera->AddRotaYAxis(yAxisRota);
		mCamera->AddRotaSideAxis(sideAxisRota);

		mInput->SetMousePos(mSize.x * 0.5f, mSize.y * 0.5f);
	}
	float vel = 0.3f;
	if (mInput->IsKeyDown(VIRTUAL_KEY_INDEX::KEY_W))
	{
		mCamera->MoveFront(vel);
	}
	if (mInput->IsKeyDown(VIRTUAL_KEY_INDEX::KEY_A))
	{ 
		mCamera->MoveSide(vel);
	}
	if (mInput->IsKeyDown(VIRTUAL_KEY_INDEX::KEY_S))
	{
		mCamera->MoveFront(-vel);
	}
	if (mInput->IsKeyDown(VIRTUAL_KEY_INDEX::KEY_D))
	{
		mCamera->MoveSide(-vel);
	}
	if (mInput->IsKeyDown(VIRTUAL_KEY_INDEX::KEY_SHIFT))
	{
		mCamera->AddPos(Vector3(0.0f, -vel, 0.0f));
	}
	if (mInput->IsKeyDown(VIRTUAL_KEY_INDEX::KEY_SPACE))
	{
		mCamera->AddPos(Vector3(0.0f, vel, 0.0f));
	}
}
