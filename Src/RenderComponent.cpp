#include "RenderComponent.h"
#include "Actor.h"

cRenderComponent::cRenderComponent(Actor * owner) : Component(owner)
{

}

bool cRenderComponent::Create()
{
	// レンダリングコンポーネントは他のコンポーネントと違い、
	// 通常の更新処理と異なるのでジョブの登録は行わない
	return true;
}

void cRenderComponent::Destroy()
{
}

void cRenderComponent::DrawRegistr()
{
}
