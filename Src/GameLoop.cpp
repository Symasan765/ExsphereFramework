/*=================================================
//								FILE :  GameLoop.cpp
//		ファイル説明 :
//		ゲームループを制御するクラスを定義する。
//		このクラスでは更新と描画の並列処理を実現すること、
//		ループをForward+レンダリングに対応させる
//									松本 雄之介
=================================================*/
#include "GameLoop.h"
#include "DirectX11.h"

/// <summary>
/// 入力更新やアップデート、描画を呼び出す大元
/// </summary>
void cGameLoop::ExeMainLoop()
{
	// TODO ループの最初で入力処理を行う


	// 描画ターゲットのクリア
	const float ClearColor[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
	//const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	cDirectX11::GetContext()->ClearRenderTargetView(
		cDirectX11::GetRenderTargetView(), // クリアする描画ターゲット
		ClearColor);         // クリアする値

	//レンダリング先を画面に切り替え
	const auto tmp = cDirectX11::GetRenderTargetView();
	cDirectX11::GetContext()->OMSetRenderTargets(1, &tmp, true ? NULL : NULL);

	cDirectX11::GetSwapChain()->Present(1,	// 垂直同期で更新する
		0);	// 画面を実際に更新する
	// TODO 更新処理と描画処理を並列で走らせる

	// TODO マネージャー系の解放処理確認を行う
}
