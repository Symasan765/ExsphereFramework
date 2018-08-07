/*=================================================
//								FILE :	MainCommand.h
//		�t�@�C������ :
//		�Q�[�����ŗ��p���郁�C���̕`��R�}���h���Ǘ�����N���X
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include<memory>
#include "MainCommandAllocators.h"
#include "MainCommandLists.h"

class cMainCommand {
public:
	cMainCommand();
	~cMainCommand();
	void Init();
private:
	std::unique_ptr<cMainCommandAllocators> m_Allocators;
	std::unique_ptr<cMainCommandLists> m_Lists;
};