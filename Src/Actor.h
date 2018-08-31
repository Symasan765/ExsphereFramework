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
	void Destroy();

	// フレーム毎の更新処理
	void Update(float delta_time) { printf("%f\n", m_Pos.x); };
	void SetID(uint64_t id) { m_ID = id; };

	void SetPos(DirectX::XMFLOAT3 pos) { m_Pos = pos; };
	void SetRot(DirectX::XMFLOAT3 rot) { m_Rot = rot; };
	void SetScale(DirectX::XMFLOAT3 scale) { m_Scale = scale; };

	void AddPos(DirectX::XMFLOAT3 pos);
	void AddRot(DirectX::XMFLOAT3 rot);
	void AddScale(DirectX::XMFLOAT3 scale);

	DirectX::XMFLOAT3 GetPos() { return m_Pos; };
	DirectX::XMFLOAT3 GetRot() { return m_Rot; };
	DirectX::XMFLOAT3 GetScale() { return m_Scale; };

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
	DirectX::XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	DirectX::XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };
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
