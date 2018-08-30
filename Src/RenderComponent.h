/*=================================================
//								FILE :	RenderComponent.h
//		ファイル説明 :
//		Actorにレンダリング機能を付与するコンポーネント
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"
#include "Component.h"
#include <string>

class cRenderComponent : public Component
{
public:
	cRenderComponent(Actor* owner);
	virtual ~cRenderComponent() = default;

	static constexpr unsigned GetID() {
		return ComponentID::kRender;
	};

	virtual bool Create();
	virtual void Destroy();

	void LoadResource(std::string);

	void DrawRegistr();		// レンダリングフレームワークに描画命令を登録する

	DirectX::XMFLOAT4X4 GetMatrix();
private:
	UINT m_ResourceID = 0;
};