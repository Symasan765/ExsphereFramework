/*=================================================
//								FILE :	RenderComponent.h
//		�t�@�C������ :
//		Actor�Ƀ����_�����O�@�\��t�^����R���|�[�l���g
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "Component.h"

class cRenderComponent : Component
{
public:
	cRenderComponent(Actor* owner);
	virtual ~cRenderComponent() = default;

	static constexpr unsigned GetID() {
		return ComponentID::kRender;
	};

	virtual bool Create();
	virtual void Destroy();

	void DrawRegistr();		// �����_�����O�t���[�����[�N�ɕ`�施�߂�o�^����
private:

};