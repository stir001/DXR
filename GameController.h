#pragma once
#include "Mathatic.h"
#include <memory>
#include <vector>

class DxInput;
class Camera;
class GameObject;

class GameController
{
public:
	GameController(const std::shared_ptr<DxInput>& input, const std::shared_ptr<Camera>& camera
		, std::vector<std::shared_ptr<GameObject>>& object, const Vector2& wndSize);
	void Update();
private:
	void CameraMove();
	std::shared_ptr<DxInput> mInput;
	std::shared_ptr<Camera> mCamera;
	std::vector<std::shared_ptr<GameObject>>& mObjects;
	Vector2 mMousePos;
	Vector2 mSize;
};

