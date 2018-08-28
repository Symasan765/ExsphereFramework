#pragma once

#include <cstdint>
#include "Job.h"
#include "ComponentType.h"

// コンポーネントへのベースクラス
// 各Component関数はstatic uint16_t ID()を
//	実装しなければいけない
class Actor;
class Component {
public:
	virtual ~Component() = default;

	virtual bool Create() = 0;
	virtual void Destroy() = 0;

protected:
	Component(Actor* owner) : m_Owner(owner) {};

	// コンポーネントを所持するアクター
	Actor* m_Owner;
	Job m_Job;
};