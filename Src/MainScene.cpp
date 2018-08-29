#include "MainScene.h"
#include "RenderComponent.h"

cMainScene::cMainScene()
{
}

cMainScene::~cMainScene()
{
}

void cMainScene::Create()
{
	auto render = m_Actor.AddComponent<cRenderComponent>();
	render->LoadResource("Private/Link.x");
}

void cMainScene::Rendering()
{
	auto render = m_Actor.GetComponent<cRenderComponent>();
	render->DrawRegistr();
}
