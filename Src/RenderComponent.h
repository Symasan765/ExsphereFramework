/*=================================================
//								FILE :	RenderComponent.h
//		�t�@�C������ :
//		Actor�Ƀ����_�����O�@�\��t�^����R���|�[�l���g
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "Component.h"
#include <string>
#include "DXMath.h"
#include "DirectX12.h"

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

	DirectX::XMFLOAT4X4 GetWorldMatrix();
private:
	UINT m_ResourceID = 0;
};