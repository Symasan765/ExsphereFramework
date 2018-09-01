#include "MainScene.h"
#include "RenderComponent.h"
#include "MoveComponent.h"

cMainScene::cMainScene()
{
}

cMainScene::~cMainScene()
{
	for (int i = 0; i < 2; i++)
		m_Actor[i].Destroy();
}

void cMainScene::Create()
{
	for (int i = 0; i < 2; i++)
		m_Actor[i].SetScale({ 0.1f,0.1f,0.1f });


	auto render = m_Actor[0].AddComponent<cRenderComponent>();
	render->LoadResource("Link.x");

	auto render1 = m_Actor[1].AddComponent<cRenderComponent>();
	render1->LoadResource("Link.x");

	m_Actor[1].SetPos(DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f });
}

void cMainScene::Rendering()
{
	static float moveX = 0.01f;

	auto render = m_Actor[0].GetComponent<cRenderComponent>();
	m_Actor[0].AddPos(DirectX::XMFLOAT3{ -moveX,0.0f,0.0f });
	render->DrawRegistr();

	auto render1 = m_Actor[1].GetComponent<cRenderComponent>();
	m_Actor[1].AddPos(DirectX::XMFLOAT3{ moveX,0.0f,0.0f });
	render1->DrawRegistr();
}