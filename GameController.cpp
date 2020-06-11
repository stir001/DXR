#include "GameController.h"
#include "DxInput.h"
#include "Camera.h"
#include "GameObject.h"
#include "ChessGame.h"
#include <functional>

GameController::GameController(const std::shared_ptr<DxInput>& input, const std::shared_ptr<Camera>& camera
	, std::vector<std::shared_ptr<GameObject>>& objects, const Vector2& wndSize)
	: mInput(input), mCamera(camera), mObjects(objects), mSize(wndSize)
{
	Init();
}

GameController::~GameController()
{
}

void GameController::Update()
{
	mInput->UpdateKeyState();
	CameraMove();
	GameUpdate();
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

void GameController::Init()
{
	mGame = std::make_unique<ChessGame>(mObjects);
}

void GameController::GameUpdate()
{
	//Inputから当たり判定をとる
	//左クリックをした瞬間のみとる
	//順次駒との当たり判定をforループでとりhitした駒のIndexを取得する
	//なにも選択されていない場合はそのターンの駒のみと当たり判定をとると効率がいいか？
	if (!mInput->IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_LBUTTON))
	{
		return;
	}
	for (int i = static_cast<int>(CHESS_PIECE::W_ROOK1); i < static_cast<int>(CHESS_PIECE::W_POWN7); ++i)
	{
		if (IsHitAABB(mObjects[i]->GetPos(), GetTableIndex(static_cast<CHESS_PIECE>(i))))
		{
			bool hit = true;
		}
	}

	for (int i = static_cast<int>(CHESS_PIECE::B_ROOK1); i < static_cast<int>(CHESS_PIECE::B_POWN7); ++i)
	{
		if (IsHitAABB(mObjects[i]->GetPos(), GetTableIndex(static_cast<CHESS_PIECE>(i))))
		{
			bool hit = true;
		}
	}

	//何も駒を選択していない場合駒との当たり判定のみをとる
	
	//駒を選択している場合
	//駒と当たり判定をとりターンの駒だった場合
	//そのコマについての移動先を考慮しながら次の当たり判定をとる
	//敵の駒だった場合その場所が移動可能ならそこに移動する
	//盤だった場合もそこが移動可能なら移動する　移動できないなら何もしない
}

Vector3 AABBMAX[6] = {
	Vector3(0.958322525f ,3.96560836f ,0.865873337f),//rook
	Vector3(0.959904671f ,3.61807108f ,0.909130573f),//knight
	Vector3(0.768050671f ,4.25057077f ,0.686666131f),//bishop
	Vector3(0.797436714f ,5.32523108f ,0.830513060f),//king
	Vector3(0.857939482f ,4.01527405f ,0.795598984f),//queen
	Vector3(0.625482500f ,2.81315231f ,0.586638927f),//pwon
};

Vector3 AABBMIN[6] = {
	Vector3(-0.958322525f ,0.000000000f ,-0.865873337f),//rook
	Vector3(-0.959904671f ,0.000000000f ,-0.909130573f),//knight
	Vector3(-0.768050671f ,0.000000000f ,-0.686666131f),//bishop
	Vector3(-0.797437012f ,0.000000000f ,-0.830513954f),//king
	Vector3(-0.857939482f ,0.000000000f ,-0.795598984f),//queen
	Vector3(-0.625482500f ,0.000000000f ,-0.586638927f),//pwon
};

bool GameController::IsHitAABB(const Vector3& offset, unsigned int tableIndex)
{
	auto screenPos = mInput->GetMousePos();
	auto dir = mCamera->Direction(screenPos);
	auto pos = mCamera->GetPos();
	auto subMax = (AABBMAX[tableIndex] + offset) - pos;
	auto subMin = (AABBMIN[tableIndex] + offset) - pos;

	subMax.x /= dir.x;
	subMax.y /= dir.y;
	subMax.z /= dir.z;

	subMin.x /= dir.x;
	subMin.y /= dir.y;
	subMin.z /= dir.z;

	if (subMax.x < subMin.x)
	{
		std::swap(subMax.x, subMin.x);
	}
	if (subMax.y < subMin.y)
	{
		std::swap(subMax.y, subMin.y);
	}
	if (subMax.z < subMin.z)
	{
		std::swap(subMax.z, subMin.z);
	}

	float tMax = FLT_MAX;
	float tMin = -FLT_MAX;

	std::function intersect = [&tMax, &tMin](float farVal, float nearVal)->bool
	{
		tMax = min(tMax, farVal);
		tMin = max(tMin, nearVal);

		return tMin <= tMax;
	};

	return	intersect(subMax.x, subMin.x) && 
			intersect(subMax.y, subMin.y) && 
			intersect(subMax.x, subMin.z);
}

int GameController::GetTableIndex(CHESS_PIECE piece)
{
	switch (piece)
	{
	case CHESS_PIECE::W_ROOK1:
	case CHESS_PIECE::W_ROOK2:
	case CHESS_PIECE::B_ROOK1:
	case CHESS_PIECE::B_ROOK2:
		return 0;
	case CHESS_PIECE::W_KNIGHT1:
	case CHESS_PIECE::W_KNIGHT2:
	case CHESS_PIECE::B_KNIGHT1:
	case CHESS_PIECE::B_KNIGHT2:
		return 1;
	case CHESS_PIECE::W_BISHOP1:
	case CHESS_PIECE::W_BISHOP2:
	case CHESS_PIECE::B_BISHOP1:
	case CHESS_PIECE::B_BISHOP2:
		return 2;
	case CHESS_PIECE::W_KING:
	case CHESS_PIECE::B_KING:
		return 3;
	case CHESS_PIECE::W_QUEEN:
	case CHESS_PIECE::B_QUEEN:
		return 4;
	case CHESS_PIECE::W_POWN0:
	case CHESS_PIECE::W_POWN1:
	case CHESS_PIECE::W_POWN2:
	case CHESS_PIECE::W_POWN3:
	case CHESS_PIECE::W_POWN4:
	case CHESS_PIECE::W_POWN5:
	case CHESS_PIECE::W_POWN6:
	case CHESS_PIECE::W_POWN7:
	case CHESS_PIECE::B_POWN0:
	case CHESS_PIECE::B_POWN1:
	case CHESS_PIECE::B_POWN2:
	case CHESS_PIECE::B_POWN3:
	case CHESS_PIECE::B_POWN4:
	case CHESS_PIECE::B_POWN5:
	case CHESS_PIECE::B_POWN6:
	case CHESS_PIECE::B_POWN7:
		return 5;
	case CHESS_PIECE::NONE:
		return INT_MAX;
	default:
		break;
	}
	return INT_MAX;
}
