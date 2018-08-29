/*=================================================
//								FILE :	RenderComponent.h
//		�t�@�C������ :
//		Actor�Ƀ����_�����O�@�\��t�^����R���|�[�l���g
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include <Windows.h>
#include "Component.h"
#include <string>

class cRenderComponent : public Component
{
public:
	cRenderComponent(Actor* owner);
	virtual ~cRenderComponent() = default;

	static constexpr unsigned GetID() {
		return ComponentID::kRender;
	};

	virtual bool Create();
	virtual void Destroy();

	void LoadResource(std::string);

	void DrawRegistr();		// �����_�����O�t���[�����[�N�ɕ`�施�߂�o�^����
private:
	UINT m_ResourceID = 0;
};