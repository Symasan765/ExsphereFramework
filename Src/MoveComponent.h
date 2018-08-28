#pragma once

#include "Component.h"
#include <Windows.h>
#include <DirectXMath.h>

// �ړ������p�̃R���|�[�l���g
class MoveComponent : public Component {
public:
	MoveComponent(Actor* owner);
	virtual ~MoveComponent() {};

	static constexpr unsigned GetID() {
		return ComponentID::kMove;
	};

	virtual bool Create()override;
	virtual void Destroy()override;

	void Update(float delta_time);

	void SetVelocity(DirectX::XMFLOAT4 velocity) {
		m_Velocity = velocity;
	};

	void SetSpeed(float value) {
		m_Speed = value;
	};
private:
	DirectX::XMFLOAT4 m_Velocity = { 0,0,0,0 };		// �ړ�����
	float m_Speed = 0;				// ���x
};