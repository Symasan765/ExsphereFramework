#include "RenderComponent.h"
#include "Actor.h"

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

void cRenderComponent::DrawRegistr()
{
}
