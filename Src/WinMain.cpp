#include <Windows.h>
#include <crtdbg.h>
#include "MainSystem.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment( lib, "winmm.lib" )
#pragma comment(lib,"xinput.lib")
#pragma comment( lib, "legacy_stdio_definitions.lib" )

/// <summary>
/// WinMain�͓����\����1.������ 2.�V�X�e���`�F�b�N 3.�A�b�v�f�[�g 4.����̗���ō\������
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

	return 0;
}