#include "Level.h"

#include <fstream>
#include "Actor.h"
#include "MoveComponent.h"
#include "JobScheduler.h"
#include "AsynceJobManager.h"

Level::~Level()
{
	Destroy();
}

bool Level::Create()
{
	m_Job.SetFunction(this, &Level::Execute);
	JobScheduler::Instance()->Register(&m_Job, ComponentID::kLevelUpdate);

	// �񓯊��Ńt�@�C����ǂݍ���
	m_FileName = "Level.txt";
	m_AsyncJob.SetFunction(this, &Level::AsycExecute);
	AsynceJobManager::Instance()->Push(&m_AsyncJob);

	return true;
}

void Level::Execute(uint64_t delta_time)
{
	if (!m_Initialized && m_AsyncJob.IsFinished()) {
		if (m_pActor) {
			// �����������͓��������ōs��
			m_pActor->Create();

			// �񓯊��̏������I������炻�̃f�[�^�����x���ɓK������
			float speed = static_cast<float>(atof(m_Data.c_str()));
			MoveComponent* moveComponent = m_pActor->AddComponent<MoveComponent>();
			if (moveComponent) {
				moveComponent->SetSpeed(speed);
			}
		}

		// ���x�����������Ȃ��悤�Ƀt���O�ݒ�
		m_Initialized = true;
	}
}

void Level::Destroy()
{
	if (m_pActor) {
		m_pActor->Destroy();
		delete m_pActor;
		m_pActor = nullptr;
	}
}

void Level::AsycExecute()
{
	std::ifstream file(m_FileName);
	if (file.fail()) {
		return;
	}

	char line[512];
	std::memset(line, '\0', sizeof(line));
	file.getline(line, sizeof(line));
	m_Data = line;

	// ���������͔񓯊��ł����v
	m_pActor = new Actor();
}
