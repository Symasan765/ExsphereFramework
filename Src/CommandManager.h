/*=================================================
//								FILE :	CommandManager.h
//		ファイル説明 :
//		描画処理をまとめて管理するクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include "MainCommand.h"
#include "MainSwapChain.h"
#include "MainCommandQueue.h"
#include "CommandParamStruct.h"
#include "FenceObj.h"
#include "RenderBufferStruct.h"

class cCommandManager
{
public:
	cCommandManager() = default;
	~cCommandManager() = default;
	void Create(ID3D12Device * dev, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi, HWND hwnd, const UINT width, const UINT heigit, 
		const UINT bufferNum);
	void SetNowTarget(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtv, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsv);
	void CommandBuild(Microsoft::WRL::ComPtr<ID3D12Resource>nowBuffer,unsigned totalFrame, unsigned frameIndex);
	void CommandQueueExe(unsigned totalFrame);

	Microsoft::WRL::ComPtr<IDXGISwapChain1> GetSwapChain();
private:
	void Init();
	void FrameUpdate(Microsoft::WRL::ComPtr<ID3D12Resource> nowBuffer, unsigned frameIndex);
	void WaitForFence(unsigned totalFrame,unsigned idx);
	void ExePrologue();
	void ExeGameScene();
	void ExeEpilogue();

	std::unique_ptr<cMainCommand> m_Command;
	std::unique_ptr<cMainCommandQueue> m_Queue;
	std::unique_ptr<cMainSwapChain> m_SwapChain;
	std::unique_ptr<cFenceObj> m_Fence;

	CommandParamStruct m_Param;
	RenderBufferStruct m_NowTarget;
};
