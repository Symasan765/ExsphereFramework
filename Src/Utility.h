/*=================================================
//								FILE :  Utility.h
//		�t�@�C������ :
//		�V�X�e�����Ŕėp�I�ɗ��p�ł��鏈���Ȃǂ��L�q����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include <Windows.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��

/// <summary>
/// ������HRESULT�l���m�F���A�G���[�ł���Η�O��f���֐�
/// </summary>
void CheckHR(HRESULT hr);