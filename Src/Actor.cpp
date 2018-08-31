#include "Actor.h"

void Actor::Destroy()
{
	for (auto& itr : m_Components) {
		delete itr.second;
		itr.second = nullptr;
	}
}

void Actor::AddPos(DirectX::XMFLOAT3 pos)
{
	m_Pos = { m_Pos.x + pos.x,m_Pos.y + pos.y,m_Pos.z + pos.z };
}

void Actor::AddRot(DirectX::XMFLOAT3 rot)
{
	m_Rot = { m_Rot.x + rot.x,m_Rot.y + rot.y,m_Rot.z + rot.z };
}

void Actor::AddScale(DirectX::XMFLOAT3 scale)
{
	m_Scale = { m_Scale.x + scale.x,m_Scale.y + scale.y,m_Scale.z + scale.z };
}
