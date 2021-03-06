#include <Windows.h>
#include <crtdbg.h>
#include "MainSystem.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment( lib, "winmm.lib" )
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"assimp-vc140-mt.lib")
#pragma comment( lib, "legacy_stdio_definitions.lib" )

/// <summary>
/// WinMainは内部構造を1.初期化 2.システムチェック 3.アップデート 4.解放の流れで構成する
/// </summary>
/// <param name="hInst"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cMainSystem mainSystem(hInst);
	mainSystem.SystemLoop();

	mainSystem.Destroy();

	return 0;
}