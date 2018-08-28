#pragma once

#include <cstdint>
#include "Job.h"
#include "ComponentType.h"

// �R���|�[�l���g�ւ̃x�[�X�N���X
// �eComponent�֐���static uint16_t ID()��
//	�������Ȃ���΂����Ȃ�
class Actor;
class Component {
public:
	virtual ~Component() = default;

	virtual bool Create() = 0;
	virtual void Destroy() = 0;

protected:
	Component(Actor* owner) : m_Owner(owner) {};

	// �R���|�[�l���g����������A�N�^�[
	Actor* m_Owner;
	Job m_Job;
};