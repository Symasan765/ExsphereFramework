/*=================================================
//								FILE :	ImGUIManager.cpp
//		ファイル説明 :
//		ImGUIの描画に必要な各種オブジェクトの生成と保持を行う。
//		基本的にソースコードはImGUI公式サンプルを参考にしている
//									松本 雄之介
=================================================*/
#include "ImGUIManager.h"
#include "DrawCommand.h"
#include "MainWindow.h"
#include "Utility.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx12.h"

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cImGUIManager::g_pd3dSrvDescHeap;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cImGUIManager::g_pd3dCommandList;

void cImGUIManager::Create()
{
	CreateDescriptorHeap();

	CreateCommandList();

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplDX12_Init(cMainWindow::GetHWND(), DrawParam::g_MaxFrameLatency, cDirectX12::GetDevice(),
		DXGI_FORMAT_R8G8B8A8_UNORM,
		g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
		g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
}

void cImGUIManager::CreateDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT hr = cDirectX12::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(g_pd3dSrvDescHeap.ReleaseAndGetAddressOf()));
	CheckHR(hr);
}

void cImGUIManager::CreateCommandList()
{
	CheckHR(cDirectX12::GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cDrawCommand::GetStartAllocator(), NULL, IID_PPV_ARGS(&g_pd3dCommandList)));
	CheckHR(g_pd3dCommandList->Close());
}

void cImGUIManager::DrawCommand()
{

}

void cImGUIManager::Destroy()
{

}
