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
	m_Stage.Destroy();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			m_Bait[i][j].Destroy();
		}
	}
}

void cMainScene::Create()
{
	for (int i = 0; i < 3; i++)
		m_Pac[i].SetScale({ 0.1f,0.1f,0.1f });
	m_Stage.SetScale({ 0.1f,0.1f,0.1f });

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			m_Bait[i][j].SetScale({ 0.08f,0.08f,0.08f });
			auto render = m_Bait[i][j].AddComponent<cRenderComponent>();
			render->LoadResource("Bait.fbx");
		}
	}

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
	m_Pacman.Update();


	static int PacNo = 0;

	auto StageRender = m_Stage.GetComponent<cRenderComponent>();
	StageRender->DrawRegistr();

	int anmNo[] = { 1,0,2,0 };
	int anmSpeed = 5;
	m_Pac[anmNo[(PacNo / anmSpeed) % 4]].SetPos(m_Pacman.GetWorldPos());
	m_Pac[anmNo[(PacNo / anmSpeed) % 4]].SetRot(m_Pacman.GetRotate());
	//m_Pac[anmNo[(PacNo / 7) % 4]].SetPos({0.9f,12.6f,0.0f});
	auto render1 = m_Pac[anmNo[(PacNo / anmSpeed) % 4]].GetComponent<cRenderComponent>();
	render1->DrawRegistr();
	PacNo++;


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {

			const float troutSize = 1.8f;		// マス自体のワールド上のサイズ
			const float startX = -4.5f;
			const float startY = 12.6f;

			m_Bait[i][j].SetPos({ startX + (float)j * troutSize ,startY - (float)i * troutSize ,0.0f});
			auto render = m_Bait[i][j].GetComponent<cRenderComponent>();
			render->DrawRegistr();
		}
	}
}