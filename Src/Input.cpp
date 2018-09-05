/*=========================================================//
//		�T�v	:	���͏������L�q����
//					�쐬�ҁ@���{ �Y�V�� (AT12A606)
//						�쐬��	2016/11/2
//					  �X�V��
//
//=========================================================*/
#include "Input.h"
#include "MainWindow.h"

#define DIRECTINPUT_VERSION 0x0800
//#define DEADZONE XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#define DEADZONE 12000

//���̃t�@�C������B�؂�
namespace {
	const int NUM_KEY_MAX = 256;
	LPDIRECTINPUT8			pDInput = NULL;						// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8	pDIDevKeyboard = NULL;				// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
	LPDIRECTINPUTDEVICE8	pDIDevMouse = NULL;					// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�}�E�X)
	BYTE						keyState[NUM_KEY_MAX];				// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE						keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE						keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE						keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	int							keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^
	DIMOUSESTATE2			mouseState;							// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	DIMOUSESTATE2			mouseStateTrigger;					// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	POINT						MousePoint;							// �}�E�X�̌��ݍ��W

	struct XInputLuaGlueStruct {
		std::string vVarName;		//Lua�ɓo�^����ϐ���
		unsigned int vNumber;		//�o�^��������l
	};
}
using namespace Input;

/// <summary>
/// �L�[�{�[�h����������
/// </summary>
cKeyboard::cKeyboard() {
	HRESULT hr;
	if (!pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(cMainWindow::GetHInstance(), DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&pDInput, NULL);
	}
	InitKeyboard(cMainWindow::GetHInstance(), cMainWindow::GetHWND());
}

/// <summary>
/// �L�[�{�[�h�������������A�V�X�e�����痘�p�ł���悤�ɂ���
/// </summary>
/// <param name="hInst"></param>
/// <param name="hWnd"></param>
/// <returns></returns>
HRESULT cKeyboard::InitKeyboard(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr = 0;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIDevKeyboard, NULL);
	if (FAILED(hr) || pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	pDIDevKeyboard->Acquire();

	return S_OK;
}

/// <summary>
/// �L�[�̍X�V���s���B���[�v�̍ŏ��ɍs���Ƃ���
/// </summary>
/// <returns></returns>
HRESULT cKeyboard::UpdateKeyboard(void) {
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = pDIDevKeyboard->GetDeviceState(sizeof(keyState), keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			// �g���K�[�ƃ����[�X���擾
			keyStateTrigger[cnt] = (keyState[cnt] ^ keyStateOld[cnt]) & keyState[cnt];
			keyStateRelease[cnt] = (keyState[cnt] ^ keyStateOld[cnt]) & keyStateOld[cnt];

			// �L�[��������Ă���Ȃ烊�s�[�g�̔��菈��
			if (keyState[cnt])
			{
				if (keyStateRepeatCnt[cnt] < 20)
				{
					keyStateRepeatCnt[cnt]++;
					// �u������́v�������́u�{�^���������Ă���20�t���[���o�߁v
					// �����ꍇ�A���s�[�g�p�z��̃f�[�^��L���ɂ���
					// ��L�����ȊO�̓��s�[�g�p�z��̃f�[�^�𖳌��ɂ���
					if (keyStateRepeatCnt[cnt] == 1 || keyStateRepeatCnt[cnt] >= 20) {
						keyStateRepeat[cnt] = 0x80;
					}
					else {
						keyStateRepeat[cnt] = 0;
					}
				}
			}
			else
			{
				keyStateRepeatCnt[cnt] = 0;
				keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		hr = pDIDevKeyboard->Acquire();
	}

	return hr;
}

/// <summary>
/// �������
/// </summary>
void cKeyboard::UninitKeyboard(void) {
	if (pDIDevKeyboard)
	{
		pDIDevKeyboard->Unacquire();

		pDIDevKeyboard->Release();
		pDIDevKeyboard = nullptr;
	}
}

/// <summary>
/// �������
/// </summary>
cKeyboard::~cKeyboard() {
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	if (pDInput) {
		pDInput->Release();
		pDInput = nullptr;
	}
}

/// <summary>
/// ����̃L�[��������Ă��邩���肷��
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool cKeyboard::Press(int key) {
	return ((keyState[key] & 0x80) != 0);
}

/// <summary>
/// ����̃L�[�������ꂽ�u�Ԃ����肷��
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool cKeyboard::Trigger(int key) {
	return ((keyStateTrigger[key] & 0x80) != 0);
}

/// <summary>
/// ����̃L�[�������ꂽ�u�Ԃ����肷��
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool Input::cKeyboard::Release(int key)
{
	return (keyStateRelease[key]);
}

/// <summary>
/// ����̃Q�[���p�b�h�{�^���������ꂽ�����肷��
/// </summary>
/// <param name="key"></param>
/// <param name="no"></param>
/// <returns></returns>
bool XInput::Press(const int key, const DWORD no) const
{
	if (conect[no]) {
		if (m_State[no].Gamepad.wButtons & key) {
			return true;
		}
	}
	else {
		if (GetKey->Press(GetGamePad->ConvertKeyboardToGamePad(key)))
			return true;
	}
	return false;
}

/// <summary>
/// ����̃Q�[���p�b�h�{�^���������ꂽ�u�Ԃ����肷��
/// </summary>
/// <param name="key"></param>
/// <param name="no"></param>
/// <returns></returns>
bool Input::XInput::Trigger(const int key, const DWORD no) const
{
	if (conect[no]) {
		//���݂̃L�[�ƑO�̃L�[�Ƃ̍��������Ŕ�����s��
		bool nowKey = m_State[no].Gamepad.wButtons & key;
		bool oldKey = m_OldState[no].Gamepad.wButtons & key;
		//�g���K�[���菈��
		if ((nowKey ^ oldKey) & nowKey)
			return true;
	}
	else {
		if (GetKey->Trigger(GetGamePad->ConvertKeyboardToGamePad(key)))
			return true;
	}
	return false;
}

/// <summary>
/// ����̃Q�[���p�b�h�{�^���������ꂽ�u�Ԃ����肷��
/// </summary>
/// <param name="key"></param>
/// <param name="no"></param>
/// <returns></returns>
bool Input::XInput::Release(const int key, const DWORD no) const
{
	if (conect[no]) {
		//���݂̃L�[�ƑO�̃L�[�Ƃ̍��������Ŕ�����s��
		bool nowKey = m_State[no].Gamepad.wButtons & key;
		bool oldKey = m_OldState[no].Gamepad.wButtons & key;
		//�����[�X���菈��
		if ((nowKey ^ oldKey) & oldKey)
			return true;
	}
	else {
		if (GetKey->Release(GetGamePad->ConvertKeyboardToGamePad(key)))
			return true;
	}
	return false;
}

/// <summary>
/// ���t�g�g���K�[�̓��͏����擾����
/// </summary>
/// <param name="no"></param>
/// <returns>�������݋(0.0 ~ 1.0)</returns>
float Input::XInput::LeftTrigger(const DWORD no) const
{
	if (conect[no]) {
		//���l���`�F�b�N���A0.0~1.0�̊Ԃŕԋp����
		float buf = (float)m_State[no].Gamepad.bLeftTrigger / 255.0f;
		return buf;
	}
	else {
		if (GetKey->Press(DIK_Q)) {
			return 1.0f;
		}
	}
	return 0.0f;
}

/// <summary>
/// ���C�g�g���K�[�̓��͏����擾����
/// </summary>
/// <param name="no"></param>
/// <returns>�������݋(0.0 ~ 1.0)</returns>
float Input::XInput::RightTrigger(const DWORD no) const
{
	if (conect[no]) {
		//���l���`�F�b�N���A0.0~1.0�̊Ԃŕԋp����
		float buf = (float)m_State[no].Gamepad.bRightTrigger / 255.0f;
		return buf;
	}
	else {
		if (GetKey->Press(DIK_E)) {
			return 1.0f;
		}
	}
	return 0.0f;
}

/// <summary>
/// ���X�e�B�b�N�̓��͏����擾����
/// </summary>
/// <param name="no"></param>
/// <returns>�|����(0.0 ~ 1.0)</returns>
DirectX::XMFLOAT2 Input::XInput::LeftStick(const DWORD no) const
{
	DirectX::XMFLOAT2 judge{ 0 , 0 };
	if (conect[no]) {
		judge.x = (float)m_State[no].Gamepad.sThumbLX / 32767;
		judge.y = (float)m_State[no].Gamepad.sThumbLY / 32767;
		return judge;
	}
	else {
		//�R���g���[�����Ȃ����Ă��Ȃ�
		if (GetKey->Press(DIK_A)) {
			judge.x -= 1.0f;
		}
		if (GetKey->Press(DIK_D)) {
			judge.x += 1.0f;
		}
		if (GetKey->Press(DIK_W)) {
			judge.y += 1.0f;
		}
		if (GetKey->Press(DIK_S)) {
			judge.y -= 1.0f;
		}
	}

	return judge;
}

/// <summary>
/// �E�X�e�B�b�N�̓��͏����擾����
/// </summary>
/// <param name="no"></param>
/// <returns>�|����(0.0 ~ 1.0)</returns>
DirectX::XMFLOAT2 Input::XInput::RightStick(const DWORD no) const
{
	DirectX::XMFLOAT2 judge{ 0 , 0 };
	if (conect[no]) {
		judge.x = (float)m_State[no].Gamepad.sThumbRX / 32767;
		judge.y = (float)m_State[no].Gamepad.sThumbRY / 32767;
		return judge;
	}
	else {
		//�R���g���[�����Ȃ����Ă��Ȃ�
		if (GetKey->Press(DIK_LEFT)) {
			judge.x -= 1.0f;
		}
		if (GetKey->Press(DIK_RIGHT)) {
			judge.x += 1.0f;
		}
		if (GetKey->Press(DIK_UP)) {
			judge.y += 1.0f;
		}
		if (GetKey->Press(DIK_DOWN)) {
			judge.y -= 1.0f;
		}
	}
	return judge;
}

/// <summary>
/// �L�[���̍X�V���s���B���[�v�̍ŏ��Ɉ�x�s���Ɨǂ��B
/// </summary>
void XInput::UpdateGamePad(void)
{
	//�X�e�[�g�ƃX�g���[�N���擾����
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
		m_OldState[i] = m_State[i];		//�Â��L�[�����i�[����
		DWORD hr = XInputGetState(i, &m_State[i]);	//�L�[�擾

		//�Q�[���p�b�h���ڑ�����Ă��Ȃ�
		if (hr == ERROR_DEVICE_NOT_CONNECTED) {
			conect[i] = false;		//�ڑ��t���O�I�t
			continue;
		}

		//�ڑ�����Ă���
		conect[i] = true;

		//�f�b�h�]�[���̐ݒ�B�f�b�h�]�[�����Ȃ琔�l��0�ɕύX���Ă���
		if ((m_State[i].Gamepad.sThumbLX < DEADZONE &&
			m_State[i].Gamepad.sThumbLX > -DEADZONE)) {
			m_State[i].Gamepad.sThumbLX = 0;
		}
		if (m_State[i].Gamepad.sThumbLY < DEADZONE &&
			m_State[i].Gamepad.sThumbLY > -DEADZONE) {

			m_State[i].Gamepad.sThumbLY = 0;
		}

		if ((m_State[i].Gamepad.sThumbRX < DEADZONE &&
			m_State[i].Gamepad.sThumbRX > -DEADZONE)) {
			m_State[i].Gamepad.sThumbRX = 0;
		}
		if (m_State[i].Gamepad.sThumbRY < DEADZONE &&
			m_State[i].Gamepad.sThumbRY > -DEADZONE) {

			m_State[i].Gamepad.sThumbRY = 0;
		}
	}
}

/// <summary>
/// �Q�[���p�b�h�����݂��Ȃ��ꍇ�ɃL�[�����L�[�{�[�h�Ɋ��蓖�Ă�
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
unsigned Input::XInput::ConvertKeyboardToGamePad(unsigned key)
{
	unsigned type = 0;
	switch (key)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		type = DIK_W;
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		type = DIK_S;
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		type = DIK_A;
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		type = DIK_D;
		break;
	case XINPUT_GAMEPAD_START:
		type = DIK_C;
		break;
	case XINPUT_GAMEPAD_BACK:
		type = DIK_Z;
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		type = DIK_R;
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		type = DIK_T;
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		type = DIK_Q;
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		type = DIK_E;
		break;
	case XINPUT_GAMEPAD_A:
		type = DIK_DOWN;
		break;
	case XINPUT_GAMEPAD_B:
		type = DIK_RIGHT;
		break;
	case XINPUT_GAMEPAD_X:
		type = DIK_LEFT;
		break;
	case XINPUT_GAMEPAD_Y:
		type = DIK_5;
		break;
	}

	return type;
}