//****************************************************************
//
// �J�����̏���[camera.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "camera.h"
#include "manager.h"
#include "math.h"

// ���O���
using namespace std;

// �K��l��ݒ�
const D3DXVECTOR3 CCamera::Config::OffSetR = { 0.0f, 50.0f, 0.0f };
const D3DXVECTOR3 CCamera::Config::OffSetRot = { D3DX_PI * 0.1f, D3DX_PI, 0.0f }; // sato Add
const D3DXVECTOR3 CCamera::Config::CatchSpeedR = { 0.3f, 0.3f, 0.3f };

// �ÓI�����o�ϐ��錾
CInputKeyboard* CCamera::m_pInputKeyboard = CManager::GetInputKeyboard();
CInputJoypad* CCamera::m_pInputJoypad = CManager::GetInputJoypad();
CInputMouse* CCamera::m_pInputMouse = CManager::GetInputMouse();

//***************************************
// �R���X�g���N�^
//***************************************
CCamera::CCamera()
{
	// �J�����̏������̒l
	m_posV = VEC3_NULL;
	m_posVDest = VEC3_NULL;
	m_posR = VEC3_NULL;
	m_posRDest = VEC3_NULL;
	m_vecU = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_fDistance = NULL;

	// ����p�A���E�̍L���̏����l��ݒ�
	m_fFov = NULL;
	m_fZnear = NULL;
	m_fZfar = NULL;
}

//***************************************
// �f�X�g���N�^
//***************************************
CCamera::~CCamera()
{
}

//***************************************
// ����������
//***************************************
HRESULT CCamera::Init(void)
{
	// �J�����̏������̒l
	m_rot = Config::OffSetRot;
	m_posR = VEC3_NULL;
	m_posRDest = VEC3_NULL;

	// �����_�܂ł̋���
	m_fDistance = Config::Defoult::Distance;

	// ���_�A�����_�̈ʒu���v�Z
	m_posV.x = m_posR.x + cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	m_posVDest.x = m_posRDest.x + cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posRDest.y + sinf(m_rot.x) * m_fDistance;
	m_posVDest.z = m_posRDest.z + cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	// ������̃x�N�g����������
	m_vecU = VEC_UP;

	// ���_�̕⊮���x��������
	m_fSpeedV = CCamera::Config::Defoult::SpeedV;

	// ����p�A���E�̍L���̏����l��ݒ�
	m_fFov = Config::Defoult::Fov;
	m_fZnear = Config::Defoult::Near;
	m_fZfar = Config::Defoult::Far;

	// �f�o�C�X���擾
	m_pInputKeyboard = CManager::GetInputKeyboard();
	m_pInputJoypad = CManager::GetInputJoypad();
	m_pInputMouse = CManager::GetInputMouse();

	// �ǂݍ���
	LoadMotion("data\\TEXT\\CameraWork\\CameraWork.txt");

	// ����I��
	return S_OK;
}

//***************************************
// �X�V����
//***************************************
void CCamera::Update(void)
{
	// �|�[�Y���������瑁�����^�[��
	if (CManager::isPause() == true) return;

	// ���[�r�[����������
	if (m_isMovie == true)
	{
		// �G���^�[�ł����ɔ�΂�
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputMouse()->OnDown(1) ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_BACK) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			// ���[�r�[���̃t���O��false�ɂ���
			m_isMovie = false;
		}

		// ���[�r�[�Đ�
		UpdateMotion();

		// �������I���
		return;
	}

	// ���ړ� sato Add
	UpdateKeyboardMoveSide();

	// sato ���̃J��������͈�U�f�o�b�N��p�ɂ��܂���
#ifdef _DEBUG
	// ���ꂼ��̍X�V�������Ă�
	UpdateMouseMove();
	//UpdateJoyPadMove();

	// �z�C�[���ŃJ�����̋�����ς���
	SetMouseWheel(m_pInputMouse->GetMouseState().lZ);
#endif // _DEBUG
}

//***************************************
// �I������
//***************************************
void CCamera::Uninit(void)
{
}

//***************************************
// �J�����̐ݒ菈��
//***************************************
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(m_fFov),						//����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, //�A�X�y�N�g��
		m_fZnear,
		m_fZfar);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//***************************************
// �J�����̃}�E�X�ړ�
//***************************************
void CCamera::UpdateMouseMove(void)
{
	// �}�E�X�̍��W���擾
	D3DXVECTOR2 Pos = CManager::GetInputMouse()->GetPos();
	D3DXVECTOR2 OldPos = CManager::GetInputMouse()->GetPosOld();

	// 1�t���[���̈ړ��ʂ��v�Z
	D3DXVECTOR2 DiffMouse = D3DXVECTOR2((FLOAT)Pos.x - (FLOAT)OldPos.x,
		(FLOAT)Pos.y - (FLOAT)OldPos.y);

	// ���_�𓮂����邩�ǂ���
	if (isMovePosV() == true)
	{
		// �ړ��ʂɌW���������Ă��傤�ǂ悭����
		DiffMouse *= Config::MouseSpeedV;

		// ���]���Ȃ��悤�ɂ���
		if (m_rot.x + DiffMouse.y < D3DX_PI * Config::RotLimit &&
			m_rot.x + DiffMouse.y > -D3DX_PI * Config::RotLimit)
		{
			// �����𓮂���
			m_rot.x += DiffMouse.y;
		}
		// �����𓮂���
		m_rot.y += DiffMouse.x;

		UpdateCameraPositionV();
	}
	// �����_�𓮂����邩�ǂ���
	else if (isMovePosR() == true)
	{
		// �ړ��ʂɌW���������Ă��傤�ǂ悭����
		DiffMouse *= Config::MouseSpeedR;

		// ���]���Ȃ��悤�ɂ���
		if (m_rot.x + DiffMouse.y < D3DX_PI * Config::RotLimit &&
			m_rot.x + DiffMouse.y > -D3DX_PI * Config::RotLimit)
		{
			// �����𓮂���
			m_rot.x += DiffMouse.y;
		}
		// �����𓮂���
		m_rot.y += DiffMouse.x;

		UpdateCameraPositionR();
	}

	// ���K��
	NormalizeCameraRot();
}

//***************************************
// �J�����̃Q�[���p�b�h�ړ�
//***************************************
void CCamera::UpdateJoyPadMove(void)
{
	// �}�E�X�J�[�\���̖߂��ʒu
	const POINT SetMousePos = { (LONG)SCREEN_WIDTH / (LONG)2.0f,(LONG)SCREEN_HEIGHT / (LONG)2.0f };

	// �p�b�h�̃X�e�B�b�N�̊p�x���擾
	XINPUT_STATE* pStick = m_pInputJoypad->GetJoyStickAngle();

	// �X�e�B�b�N�������Ă�����
	if (m_pInputJoypad->GetJoyStickR() == true)
	{
		// �X�e�B�b�N�̈ʒu���擾
		float XAngle = pStick->Gamepad.sThumbRX;
		float YAngle = pStick->Gamepad.sThumbRY;

		// �|�����[�����擾
		float Distance = sqrtf((XAngle * XAngle) + (YAngle * YAngle));

		// �Œ���̐[����ݒ�
		float DeadZone = 32767.0f * 0.0005f;

		// �[������肾������
		if (Distance > DeadZone)
		{
			// �p�x���v�Z
			float Angle = atan2f(XAngle, YAngle);

			// ��܂ōs���Ȃ�������
			if (m_rot.x - cosf(-Angle) * Distance * Config::PadSpeed < D3DX_PI * Config::RotLimit &&
				m_rot.x - cosf(-Angle) * Distance * Config::PadSpeed > -D3DX_PI * Config::RotLimit)
			{
				// �����𓮂���
				m_rot.x -= cosf(Angle) * Distance * Config::PadSpeed;
			}
			// �����𓮂���
			m_rot.y += sinf(Angle) * Distance * Config::PadSpeed;
		}
	}

	// ���K��
	NormalizeCameraRot();
}

//***************************************
// �J�����̃L�[�{�[�h�ړ� sato Add
//***************************************
void CCamera::UpdateKeyboardMoveSide(void)
{
	// �L�[�{�[�h�̓��͂��擾
	bool Left = m_pInputKeyboard->GetPress(DIK_A);
	bool Right = m_pInputKeyboard->GetPress(DIK_D);

	// ���E�ɓ�����
	if (Left == true)
	{
		m_posRDest.x -= Config::MoveSpeedSide;
	}
	if (Right == true)
	{
		m_posRDest.x += Config::MoveSpeedSide;
	}

	// ���W���X�V
	UpdateCameraPositionRSide();
	UpdateCameraPositionVSide();
}

//***************************************
// �J�����̌����̐��K��
//***************************************
void CCamera::NormalizeCameraRot(void)
{
	// �p�x�̐��K��(X)
	if (m_rot.x > D3DX_PI)
	{
		m_rot.x -= D3DX_PI * 2.0f;
	}
	else if (m_rot.x < -D3DX_PI)
	{
		m_rot.x += D3DX_PI * 2.0f;
	}

	// �p�x�̐��K��(Y)
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// �p�x�̐��K��(Z)
	if (m_rot.z > D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}
	else if (m_rot.z < -D3DX_PI)
	{
		m_rot.z += D3DX_PI * 2.0f;
	}
}

//***************************************
// �G�̎�_�ɑ΂��ăG�C���A�V�X�g����
//***************************************
D3DXVECTOR3 CCamera::CalcAssistVec(void)
{
	// �G�C���A�V�X�g�x�N�g���̈ꎞ�ϐ�
	D3DXVECTOR3 AimAssist = VEC3_NULL;

	// NULL��Ԃ�
	return VEC3_NULL;
}

//***************************************
// �A�C���A�V�X�g����
//***************************************
void CCamera::Assist(D3DXVECTOR3 AssistVec, float Strength)
{
	// �G�C���A�V�X�g���g���₷���X�P�[���ɒ���
	const float NormalizeScale = 0.005f;

	// �G�C���A�V�X�g���s
	m_rot.x += (AssistVec.y * NormalizeScale) * Strength;
	m_rot.y += (AssistVec.x * NormalizeScale) * Strength;
}

//***************************************
// �J�����̋�����ς���
//***************************************
void CCamera::SetMouseWheel(int zDelta)
{
	if (zDelta == 0) return;

	// �����𔽉f
	m_fDistance += zDelta * Config::WheelSpeed;

	// ���W���X�V
	m_posV.x = m_posR.x + cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//***************************************
// ���[�r�[��ݒ�
//***************************************
void CCamera::SetMovie(MOTIONTYPE Type)
{
	// ���[�V�����̎�ނ�ݒ�
	m_nMotionType = Type;

	// �L�[��������
	m_nKey = 0;

	// ���̃L�[��������
	m_nNextKey = 0;

	// ���[�V�����J�E���^��������
	m_nCounterMotion = 0;

	// �S�̃t���[����������
	m_nAllFrame = 0;

	// �I���t���O��������
	m_bFinishMotion = false;

	// ���[�r�[�t���O��ݒ�
	m_isMovie = true;
}

//***************************************
// �r���[�n�̃}�g���b�N�X��r���[�|�[�g�����Z�b�g
//***************************************
void CCamera::ResetProjectionMtx(void)
{
	// �ݒ肷��p�̃}�g���b�N�X
	D3DXMATRIX ViewMtx, ProjectionMtx;

	// �A�X�y�N�g���ݒ�
	float fAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �n�_�A�����_�A������̕����x�N�g����ݒ�
	D3DXVECTOR3 PosV, PosR, VecU;
	PosV = CManager::GetCamera()->GetPosV();
	PosR = CManager::GetCamera()->GetPosR();
	VecU = { 0.0f,1.0f,0.0f };

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&ViewMtx);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&ViewMtx,
		&PosV,
		&PosR,
		&VecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &ViewMtx);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&ProjectionMtx);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&ProjectionMtx,			// �}�g���b�N�X
		D3DXToRadian(CCamera::Config::Defoult::Fov),	// ����p
		fAspect,										// �A�X�y�N�g��
		CCamera::Config::Defoult::Near,					// �j�A
		CCamera::Config::Defoult::Far);					// �t�@�[

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMtx);
}

//***************************************
// ���[�V�����ǂݍ���
//***************************************
void CCamera::LoadMotion(std::string Path)
{
	// �t�@�C�����J��
	ifstream ifs(Path);

	// �J���Ȃ�������
	if (!ifs)
	{
		MessageBox(NULL, "�t�@�C�����ǂݍ��߂܂���ł���", "�I�����b�Z�[�W", MB_OK);
		return;
	}

	// �s��ǂݍ��ޗp�̕ϐ�
	string line = {};

	// �p�����[�^�[�A�C�R�[���p�̕ϐ�
	string label = {}, equal = {};

	// �L�[�J�E���g��錾
	int KeyCount = 0;

	// ���[�V������������ϐ���錾
	MOTIONINFO LocalMotion;

	// �I���܂ŌJ��Ԃ�
	while (getline(ifs, line))
	{
		// �ǂݍ��ނ��߂̃t�H�[�}�ƋS�ϊ�
		istringstream iss(line);

		// �L�[���[�h���v�Z
		if (line.find("LOOP") != string::npos)
		{
			// ���[�v���邩�ǂ�����ǂݍ���
			iss >> label >> equal >> LocalMotion.bLoop;
		}
		if (line.find("NUMKEY") != string::npos)
		{
			// �L�[�̑�����ǂݍ���
			iss >> label >> equal >> LocalMotion.nNumKey;

			// �x�N�^�[�̃T�C�Y���m��
			LocalMotion.pKeyInfo.resize(LocalMotion.nNumKey);
		}
		if (line.find("POSV") != string::npos)
		{
			// �n�_�̈ʒu��ǂݍ���
			iss >> label >> equal >> LocalMotion.pKeyInfo[KeyCount].fPosVX >> LocalMotion.pKeyInfo[KeyCount].fPosVY >> LocalMotion.pKeyInfo[KeyCount].fPosVZ;
		}
		if (line.find("POSR") != string::npos)
		{
			// �����_�̈ʒu��ǂݍ���
			iss >> label >> equal >> LocalMotion.pKeyInfo[KeyCount].fPosRX >> LocalMotion.pKeyInfo[KeyCount].fPosRY >> LocalMotion.pKeyInfo[KeyCount].fPosRZ;
		}
		if (line.find("FRAME") != string::npos)
		{
			// �t���[������ǂݍ���
			iss >> label >> equal >> LocalMotion.pKeyInfo[KeyCount].nFrame;
		}
		if (line.find("END_KEY") != string::npos)
		{
			// ���[������Ƃ��C���N�������g
			KeyCount++;
		}
		if (line.find("END_CAMERAWORK") != string::npos)
		{
			// �L�[�J�E���g��������
			KeyCount = 0;

			// �A��
			m_vMotionInfo.push_back(LocalMotion);

			// ���[�J���ϐ���������
			LocalMotion = {};
		}
	}
}

//***************************************
// PosV�̍��W�X�V
//***************************************
void CCamera::UpdateCameraPositionV()
{
	// ���_�̍��W�X�V�A����������藣��
	m_posVDest.x = m_posRDest.x + cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posRDest.y + sinf(m_rot.x) * m_fDistance + 10.0f;
	m_posVDest.z = m_posRDest.z + cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	m_posV.x += (m_posVDest.x - m_posV.x) * m_fSpeedV;
	m_posV.y += (m_posVDest.y - m_posV.y) * m_fSpeedV;
	m_posV.z += (m_posVDest.z - m_posV.z) * m_fSpeedV;
}

//***************************************
// PosR�̍��W�X�V
//***************************************
void CCamera::UpdateCameraPositionR()
{
	// ���_�̍��W�X�V�A�������������ݒ肷��
	m_posRDest.x = cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posRDest.y = sinf(m_rot.x) * m_fDistance + 10.0f;
	m_posRDest.z = cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	// �����_�̍X�V
	m_posR.x += (m_posRDest.x - m_posR.x) * Config::CatchSpeedR.x;
	m_posR.y += (m_posRDest.y - m_posR.y) * Config::CatchSpeedR.y;
	m_posR.z += (m_posRDest.z - m_posR.z) * Config::CatchSpeedR.z;
}

//***************************************
// PosV�̍��W�X�VSide sato Add
//***************************************
void CCamera::UpdateCameraPositionVSide()
{
	// ���_�̍��W�X�V�A����������藣��
	m_posVDest.x = m_posRDest.x + cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	//m_posVDest.y = m_posRDest.y + sinf(m_rot.x) * m_fDistance + 10.0f;
	//m_posVDest.z = m_posRDest.z + cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	m_posV.x += (m_posVDest.x - m_posV.x) * Config::CatchSpeedSide;
	//m_posV.y += (m_posVDest.y - m_posV.y) * Config::CatchSpeedSide;
	//m_posV.z += (m_posVDest.z - m_posV.z) * Config::CatchSpeedSide;
}

//***************************************
// PosR�̍��W�X�VSide sato Add
//***************************************
void CCamera::UpdateCameraPositionRSide()
{
	// �����_�̍X�V
	m_posR.x += (m_posRDest.x - m_posR.x) * Config::CatchSpeedSide;
	//m_posR.y += (m_posRDest.y - m_posR.y) * Config::CatchSpeedSide;
	//m_posR.z += (m_posRDest.z - m_posR.z) * Config::CatchSpeedSide;
}

//***************************************
// ���[�V�������Đ�
//***************************************
void CCamera::UpdateMotion(void)
{
	// ���[�V�����J�E���^���C���N�������g
	m_nCounterMotion++;

	// �S�̃t���[�����C���N�������g
	m_nAllFrame++;

	// �L�[�̑������ǂݍ��߂Ă����玟�̃L�[��ݒ�
	if (m_vMotionInfo[m_nMotionType].nNumKey != NULL) m_nNextKey = (m_nKey + 1) % m_vMotionInfo[m_nMotionType].nNumKey;

	// ���̃L�[�ɐi�߂�
	if (m_nCounterMotion >= m_vMotionInfo[m_nMotionType].pKeyInfo[m_nKey].nFrame)
	{
		// �J�E���^��������
		m_nCounterMotion = 0;

		// �L�[�̑������ǂݍ��߂Ă�����L�[��i�߂�
		if (m_vMotionInfo[m_nMotionType].nNumKey != NULL) m_nKey = (m_nKey + 1) % m_vMotionInfo[m_nMotionType].nNumKey;

		// �L�[�̑����ɒB������
		if (m_nKey >= m_vMotionInfo[m_nMotionType].nNumKey - 1)
		{
			// �S�̃t���[����������
			m_nAllFrame = 0;

			// ���[�v���Ȃ��^�C�v��������
			if (m_vMotionInfo[m_nMotionType].bLoop == false)
			{
				// �I����Ԃɂ���
				m_bFinishMotion = true;
			}
		}
	}

	// ���̃L�[�Ǝ��̃L�[�̈ꎞ�ϐ�
	CCamera::KEY nKey = m_vMotionInfo[m_nMotionType].pKeyInfo[m_nKey];
	CCamera::KEY nNexKey = m_vMotionInfo[m_nMotionType].pKeyInfo[m_nNextKey];

	// �����ƖڕW�̈ꎞ�ϐ�
	CCamera::KEY KeyDef;
	CCamera::KEY KeyDest;

	// ����
	KeyDef.fPosRX = nNexKey.fPosRX - nKey.fPosRX;
	KeyDef.fPosRY = nNexKey.fPosRY - nKey.fPosRY;
	KeyDef.fPosRZ = nNexKey.fPosRZ - nKey.fPosRZ;

	KeyDef.fPosVX = nNexKey.fPosVX - nKey.fPosVX;
	KeyDef.fPosVY = nNexKey.fPosVY - nKey.fPosVY;
	KeyDef.fPosVZ = nNexKey.fPosVZ - nKey.fPosVZ;

	// ��]�̒l
	KeyDest.fPosVX = nKey.fPosVX + KeyDef.fPosVX * ((float)m_nCounterMotion / (float)nKey.nFrame);
	KeyDest.fPosVY = nKey.fPosVY + KeyDef.fPosVY * ((float)m_nCounterMotion / (float)nKey.nFrame);
	KeyDest.fPosVZ = nKey.fPosVZ + KeyDef.fPosVZ * ((float)m_nCounterMotion / (float)nKey.nFrame);

	// ��]�̒l
	KeyDest.fPosRX = nKey.fPosRX + KeyDef.fPosRX * ((float)m_nCounterMotion / (float)nKey.nFrame);
	KeyDest.fPosRY = nKey.fPosRY + KeyDef.fPosRY * ((float)m_nCounterMotion / (float)nKey.nFrame);
	KeyDest.fPosRZ = nKey.fPosRZ + KeyDef.fPosRZ * ((float)m_nCounterMotion / (float)nKey.nFrame);

	// ����
	m_posV.x = KeyDest.fPosVX;
	m_posV.y = KeyDest.fPosVY;
	m_posV.z = KeyDest.fPosVZ;

	// ����
	m_posR.x = KeyDest.fPosRX;
	m_posR.y = KeyDest.fPosRY;
	m_posR.z = KeyDest.fPosRZ;

	// �I�����Ă�����
	if (m_bFinishMotion == true)
	{
		// ���[�r�[�t���O��؂�ւ���
		m_isMovie = false;
	}
}