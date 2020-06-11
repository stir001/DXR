#pragma once
#include "Mathatic.h"
#include <memory>
#include <vector>

class DxInput;
class Camera;
class GameObject;
class ChessGame;
enum class CHESS_PIECE : int;

class GameController
{
public:
	GameController(const std::shared_ptr<DxInput>& input, const std::shared_ptr<Camera>& camera
		, std::vector<std::shared_ptr<GameObject>>& object, const Vector2& wndSize);
	~GameController();
	void Update();
private:
	void CameraMove();
	void Init();
	void GameUpdate();
	bool IsHitAABB(const Vector3& offset, unsigned int tableIndex);
	int GetTableIndex(CHESS_PIECE piece);

	std::shared_ptr<DxInput> mInput;
	std::shared_ptr<Camera> mCamera;
	std::unique_ptr<ChessGame> mGame;
	std::vector<std::shared_ptr<GameObject>>& mObjects;
	Vector2 mMousePos;
	Vector2 mSize;
};

