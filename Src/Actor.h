#pragma once

#include <iostream>
#include <cassert>
#include <map>
#include <Windows.h>
#include <DirectXMath.h>
#include "ComponentType.h"
#include "JobScheduler.h"
#include "Job.h"

class Component;

class Actor {
public:
	Actor() = default;
	virtual ~Actor() = default;

	bool Create() {
		m_Job.SetFunction(this, &Actor::Update);
		JobScheduler::Instance()->Register(&m_Job, ComponentID::kActor);
		return true;
	};
	void Destroy() {};

	// フレーム毎の更新処理
	void Update(float delta_time) { printf("%f\n", m_Pos.x); };
	void SetID(uint64_t id) { m_ID = id; };

	void SetPos(DirectX::XMFLOAT3 pos) { m_Pos = pos; };
	void SetRot(DirectX::XMFLOAT4 rot) { m_Rot = rot; };

	void SetPos(DirectX::XMVECTOR pos) {
		DirectX::XMStoreFloat3(&m_Pos, pos);
	}
	void SetRot(DirectX::XMVECTOR rot) {
		DirectX::XMStoreFloat4(&m_Rot, rot);
	}

	DirectX::XMFLOAT3 GetPos() { return m_Pos; };

	unsigned GetID()const { return m_ID; };

	template<typename T>
	T *AddComponent();

	template<typename T>
	T *GetComponent();

private:

	unsigned m_ID = 0;
	Job m_Job;
	std::map<unsigned, Component*> m_Components;
	DirectX::XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT4 m_Rot = { 0.0f,0.0f,0.0f,0.0f };		// クォータニオンとして使用
};

template<typename T>
inline T * Actor::AddComponent()
{
	// Debug時専用のチェック処理
	// コンポーネントの二重登録は不許可
	assert(GetComponent<T>() == nullptr);

	T* component = new T(this);
	component->Create();
	m_Components.insert(std::pair<unsigned, Component*>(T::GetID(), component));

	return component;
}

template<typename T>
inline T * Actor::GetComponent()
{
	std::map<unsigned, Component*>::iterator it = m_Components.find(T::GetID());
	if (it != m_Components.end()) {
		return static_cast<T*>(it->second);
	}

	return nullptr;
}
