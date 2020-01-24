#pragma once
#include <memory>
#include <Windows.h>

class RayTraceRenderer;
class GameObject;
class DxilEntryPoints;
class GameController;
namespace sub_objects { class SubObject; };

class GameApp
{
public:
	GameApp();
	~GameApp();
	void Init(HINSTANCE hInst);
	void Run();
	void Terminate();
private:
	bool ProcessMessage();

	std::unique_ptr<RayTraceRenderer> mRenderer;
	std::shared_ptr<sub_objects::SubObject> mSubObject;
	std::shared_ptr<GameController> mController;
};

