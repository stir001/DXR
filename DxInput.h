#pragma once

#include <Windows.h>
#include <memory>
#include <Xinput.h>
#include <DirectXMath.h>
#include <vector>

//window���Œ�`����Ă��Ȃ�������
//���z�L�[�R�[�h�̒�`��
//�Q�[���Ŏg�����Ƃ̑������z�L�[�R�[�h��enum��
enum class VIRTUAL_KEY_INDEX
{
	KEY_NON = 0x00,
	//����`�̕������`
	KEY_0 = 0x30,
	KEY_1 = 0x31,
	KEY_2 = 0x32,
	KEY_3 = 0x33,
	KEY_4 = 0x34,
	KEY_5 = 0x35,
	KEY_6 = 0x36,
	KEY_7 = 0x37,
	KEY_8 = 0x38,
	KEY_9 = 0x39,
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4A,
	KEY_K = 0x4B,
	KEY_L = 0x4C,
	KEY_M = 0x4D,
	KEY_N = 0x4E,
	KEY_O = 0x4F,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5A,

	//�g�����Ƃ̑���������enum��
	KEY_LBUTTON = 0x01,//�}�E�X���{�^��
	KEY_RBUTTON = 0x02,//�}�E�X�E�{�^��
	KEY_MBUTTON = 0x04,//�}�E�X�����{�^��
	KEY_ENTER = 0x0D,//Enter(Return)�L�[
	KEY_SHIFT = 0x10,//Shift�L�[(���E����ʂ��Ȃ�)
	KEY_CONTROL = 0x11,//Ctrl�L�[(���E����ʂ��Ȃ�)
	KEY_ALT = 0x12,//Alt�L�[(���E��ʂ��Ȃ�)
	KEY_LSHIFT = 0xA0,//��Shift�L�[
	KEY_RSHIFT = 0xA1,//�EShift�L�[
	KEY_LCONTROL = 0xA2,//��Ctrl�L�[
	KEY_RCONTROL = 0xA3,//�ECtrl�L�[
	KEY_LALT = 0xA4,//��Alt�L�[
	KEY_RALT = 0xA5,//�EAlt�L�[
	KEY_ESCAPE = 0x1B,//ESC�L�[
	KEY_SPACE = 0x20,//Space�L�[
	KEY_LEFT = 0x25,//�����L�[
	KEY_UP = 0x26,//����L�[
	KEY_RIGHT = 0x27,//�E���L�[
	KEY_DOWN = 0x28,//�����L�[
	KEY_NUMPAD0 = 0x60,//�i���p�b�h0�L�[
	KEY_NUMPAD1 = 0x61,//�i���p�b�h1�L�[
	KEY_NUMPAD2 = 0x62,//�i���p�b�h2�L�[
	KEY_NUMPAD3 = 0x63,//�i���p�b�h3�L�[
	KEY_NUMPAD4 = 0x64,//�i���p�b�h4�L�[
	KEY_NUMPAD5 = 0x65,//�i���p�b�h5�L�[
	KEY_NUMPAD6 = 0x66,//�i���p�b�h6�L�[
	KEY_NUMPAD7 = 0x67,//�i���p�b�h7�L�[
	KEY_NUMPAD8 = 0x68,//�i���p�b�h8�L�[
	KEY_NUMPAD9 = 0x69,//�i���p�b�h9�L�[
	KEY_F1 = 0x70,
	KEY_F2 = 0x71,
	KEY_F3 = 0x72,
	KEY_F4 = 0x73,
	KEY_F5 = 0x74,
	KEY_F6 = 0x75,
	KEY_F7 = 0x76,
	KEY_F8 = 0x77,
	KEY_F9 = 0x78,
	KEY_F10 = 0x79,
	KEY_F11 = 0x7A,
	KEY_F12 = 0x7B,
};

enum class XINPUT_PAD_STATE
{
	DPAD_UP			=  0x00000001,
	DPAD_DOWN		=  0x00000002,
	DPAD_LEFT		=  0x00000004,
	DPAD_RIGHT		=  0x00000008,
	START			=  0x00000010,
	BACK			=  0x00000020,
	LEFT_THUMB		=  0x00000040,
	RIGHT_THUMB		=  0x00000080,
	LEFT_SHOULDER	=  0x0100,
	RIGHT_SHOULDER	=  0x0200,
	A				=  0x1000,
	B				=  0x2000,
	X				=  0x4000,
	Y				=  0x8000,

};

enum eKEY_STATE_CHECK
{
	eKEY_STATE_CHECK_DOWN	= 0x80,//�L�[��������Ă��邩�ǂ����̔���
	eKEY_STATE_CHECK_TOGGLE	= 0x01,//�g�O����on��off���̔���
};

class DxInput
{
public:
	DxInput(HWND hwnd);
	~DxInput();
	//���t���[���Ă�
	bool UpdateKeyState();
	const DirectX::XMFLOAT2 GetMousePos() const;

	//@return 256�̗v�f�����z���Ԃ�
	const unsigned char* GetKeyState();

	bool IsKeyDown(VIRTUAL_KEY_INDEX index) const;
	bool IsKeyTrigger(VIRTUAL_KEY_INDEX index) const;
	bool IsKeyToggle(VIRTUAL_KEY_INDEX index) const;

	bool IsKeyUp(VIRTUAL_KEY_INDEX index) const;
private:
	const static int KEY_COUNT = 256;
	const static int PAD_NUM = 4;
	HWND mHWND;

	//�ŏ�ʃr�b�g(0x80)�������Ă���Ƃ��͍�������Ă���
	//�ĉ��ʃr�b�g(0x01)�̓g�O����on off����
	//�ǂ���������Ă��Ȃ��Ƃ��͉�����Ă��Ȃ�
	//�ȉ��̃T�C�g���Q��
	//ttp://www016.upp.so-net.ne.jp/garger-studio/gameprog/vb0142.html
	unsigned char mKeyState[KEY_COUNT];
	unsigned char mPreKeyState[KEY_COUNT];

	std::vector<XINPUT_STATE> mXInputState;
};
