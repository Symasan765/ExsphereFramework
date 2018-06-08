/*=================================================
//								FILE :  Utility.h
//		ファイル説明 :
//		システム内で汎用的に利用できる処理などを記述する
//
//									松本 雄之介
=================================================*/
#pragma once
#include <Windows.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ

void CheckHR(HRESULT hr);