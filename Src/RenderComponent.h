/*=================================================
//								FILE :	RenderComponent.h
//		�t�@�C������ :
//		Actor�Ƀ����_�����O�@�\��t�^����R���|�[�l���g
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"
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

	DirectX::XMFLOAT4X4 GetMatrix();
private:
	UINT m_ResourceID = 0;
};