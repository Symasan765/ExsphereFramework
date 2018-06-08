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

void CheckHR(HRESULT hr);