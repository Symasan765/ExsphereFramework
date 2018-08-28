/*=================================================
//								FILE :	RenderComponent.h
//		ファイル説明 :
//		Actorにレンダリング機能を付与するコンポーネント
//
//									松本 雄之介
=================================================*/
#pragma once
#include "Component.h"

class cRenderComponent : Component
{
public:
	cRenderComponent(Actor* owner);
	virtual ~cRenderComponent() = default;

	static constexpr unsigned GetID() {
		return ComponentID::kRender;
	};

	virtual bool Create();
	virtual void Destroy();

	void DrawRegistr();		// レンダリングフレームワークに描画命令を登録する
private:

};