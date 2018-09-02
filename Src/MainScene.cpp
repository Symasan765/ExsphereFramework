#include "MainScene.h"
#include "RenderComponent.h"
#include "MoveComponent.h"

cMainScene::cMainScene()
{
}

cMainScene::~cMainScene()
{
	for (int i = 0; i < 3; i++)
		m_Pac[i].Destroy();
}

void cMainScene::Create()
{
	for (int i = 0; i < 3; i++)
		m_Pac[i].SetScale({ 0.1f,0.1f,0.1f });
	m_Stage.SetScale({ 0.1f,0.1f,0.1f });



	auto render = m_Stage.AddComponent<cRenderComponent>();
	m_Stage.SetScale({ 0.12f,0.12f,0.12f });
	m_Stage.SetPos({ 0.0f,-0.25f,0.0f });
	render->LoadResource("Stage.fbx");

	for (int i = 0; i < 3; i++) {
		std::string name = "Pac" + std::to_string(i + 1) + ".fbx";

		auto render1 = m_Pac[i].AddComponent<cRenderComponent>();
		render1->LoadResource(name);
	}



}

void cMainScene::Rendering()
{
	static int PacNo = 0;

	auto StageRender = m_Stage.GetComponent<cRenderComponent>();
	StageRender->DrawRegistr();

	int anmNo[] = { 1,0,2,0 };
	auto render1 = m_Pac[anmNo[(PacNo / 7) % 4]].GetComponent<cRenderComponent>();
	render1->DrawRegistr();
	PacNo++;
}