#include "RenderComponent.h"
#include "Actor.h"
#include "RenderingFramework.h"

using namespace DirectX;

cRenderComponent::cRenderComponent(Actor * owner) : Component(owner)
{

}

bool cRenderComponent::Create()
{
	// �����_�����O�R���|�[�l���g�͑��̃R���|�[�l���g�ƈႢ�A
	// �ʏ�̍X�V�����ƈقȂ�̂ŃW���u�̓o�^�͍s��Ȃ�
	return true;
}

void cRenderComponent::Destroy()
{
}

void cRenderComponent::LoadResource(std::string fileName)
{
	m_ResourceID = cRenderingFramework::GetID(fileName);
}

void cRenderComponent::DrawRegistr()
{
	cRenderingFramework::RnederingRegister(m_ResourceID, this);
}

DirectX::XMFLOAT4X4 cRenderComponent::GetWorldMatrix()
{
	static float rotY = 0.0f;
	XMFLOAT3 pos = m_Owner->GetPos();
	XMFLOAT3 rot = m_Owner->GetRot();
	XMFLOAT3 scale = m_Owner->GetScale();
	DirectX::XMMATRIX mat = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationRollPitchYaw(rot.x, rotY, rot.z);
	mat *= XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMFLOAT4X4 ret;
	XMStoreFloat4x4(&ret, XMMatrixTranspose(mat));
	return ret;
}
