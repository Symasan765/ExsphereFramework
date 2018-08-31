#include "Component.h"
#include "Actor.h"
#include "MoveComponent.h"

using namespace DirectX;

MoveComponent::MoveComponent(Actor * owner) : Component(owner)
{
}

bool MoveComponent::Create()
{
	m_Job.SetFunction(this, &MoveComponent::Update);
	JobScheduler* jobScheduler = JobScheduler::Instance();

	jobScheduler->Register(&m_Job, ComponentID::kMove);

	return true;
}

void MoveComponent::Destroy()
{
}

void MoveComponent::Update(float delta_time)
{
	XMVECTOR vector = XMLoadFloat4(&m_Velocity) * m_Speed;

	XMVECTOR pos = XMLoadFloat3(&m_Owner->GetPos());

	XMFLOAT3 move;
	XMStoreFloat3(&move, pos + vector);
	m_Owner->SetPos(move);
}
