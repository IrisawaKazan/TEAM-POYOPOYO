//****************************************************************
//
// �J�����̏���[camera.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _CAMERA_H_
#define _CAMERA_H_

// �C���N���[�h
#include "main.h"
#include "input.h"

// �J�����N���X���`
class CCamera
{
public:
	// ���[�h sato Add
	enum class MODE : unsigned char
	{
		NORMAL,
		BELTSCROLL,
		MAX
	};

	// �J�����̋K��l��ݒ�
	struct Config {
		static constexpr float MouseSpeedV = 0.005f;
		static constexpr float MouseSpeedR = 0.0005f;
		static constexpr float PadSpeed = 0.0000005f;
		static constexpr float RotLimit = 0.25f;
		static constexpr float WheelSpeed = 0.1f;
		static constexpr float DistanceNear = 100.0f;
		static constexpr float DistanceFar = 200.0f;
		static constexpr float MoveRDist = -8.0f;
		static constexpr float MoveSpeedR = 0.03f;
		static constexpr float MoveSpeedSide = 20.0f;       // sato Add
		static constexpr float CatchSpeedSide = 0.1f;       // sato Add
		static constexpr float SideMoveMin = -2000.0f;      // sato Add
		static constexpr float SideMoveMax = 2000.0f;       // sato Add
		static constexpr MODE startMode = MODE::BELTSCROLL; // sato Add
		static const D3DXVECTOR3 OffSetR;
		static const D3DXVECTOR3 OffSetRot;
		static const D3DXVECTOR3 CatchSpeedR;
		// ���������̒萔
		struct Default {
			static constexpr float Distance = 1000.0f;
			static constexpr float RDist = -10.0f;
			static constexpr float Fov = 45.0f;
			static constexpr float Near = 10.0f;
			static constexpr float Far = 10000.0f;
			static constexpr float SpeedV = 0.3f;
		};
	};

	// ���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_STARTMOVIE = 0,
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	// �L�[���
	struct KEY
	{
		// �����_�̖ڕW�̈ʒu
		float fPosRX;
		float fPosRY;
		float fPosRZ;
		// �n�_�̖ڕW�̈ʒu
		float fPosVX;
		float fPosVY;
		float fPosVZ;
		// �t���[��
		int nFrame;
	};

	// ���[�V�������
	struct MOTIONINFO
	{
		bool bLoop;					// ���[�v���邩�ǂ���
		int nNumKey;				// �L�[�̑���
		std::vector<KEY> pKeyInfo;	// �L�[���̉ϒ��z��
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CCamera();
	~CCamera();

	// �����o�֐�
	HRESULT Init(void);
	void Update(void);
	void Uninit(void);
	void SetCamera(void);
	void UpdateMove(void);
	void UpdateMouseMove(void);
	void UpdateJoyPadMove(void);
	void UpdateKeyboardMoveSide(void);      
	void UpdateCameraPosition(void);
	void UpdateCameraPositionV(void);
	void UpdateCameraPositionR(void);
	void UpdateCameraPositionVNormal(void);
	void UpdateCameraPositionRNormal(void);
	void UpdateCameraPositionVSide(void);
	void UpdateCameraPositionRSide(void);
	void UpdateMotion(void);

	void NormalizeCameraRot(void);
	D3DXVECTOR3 CalcAssistVec(void);
	void Assist(D3DXVECTOR3 AssistVec,float Strength);

	// �������̊֐�
	bool isMovePosV(void) { return m_pInputMouse->OnPress(0) && m_pInputKeyboard->GetPress(DIK_LALT); };
	bool isMovePosR(void) { return m_pInputMouse->OnPress(1) && m_pInputKeyboard->GetPress(DIK_LALT); };
	bool IsAnim(void) { return m_isMovie; };

	// �Z�b�^�[
	void SetDistance(float Distance) { m_fDistance = Distance; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };
	void SetMouseWheel(int zDelta);
	void SetSpeedV(const float Speed) { m_fSpeedV = Speed; };
	void SetMovie(MOTIONTYPE Type);
	static void ResetProjectionMtx(void);
	void SetMode(MODE mode) { m_mode = mode; }

	// �Q�b�^�[
	D3DXVECTOR2 MouseMove(void) { return m_pInputMouse->GetVelocity(); };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	D3DXVECTOR3 GetPosV(void) { return m_posV; };
	D3DXVECTOR3 GetPosR(void) { return m_posR; };
	MOTIONTYPE GetMotion(void) { return m_nMotionType; };
	float GetSpeedV(void) { return m_fSpeedV; };
	MODE GetMode(void) { return m_mode; }

	// ���[�_�[
	void LoadMotion(std::string Path);
private:
	void SetNormal(void);
	void SetBelt(void);

	// �ÓI�����o�ϐ�
	// ���̓f�o�C�X�̃����o�ϐ�
	static CInputKeyboard* m_pInputKeyboard;
	static CInputJoypad* m_pInputJoypad;
	static CInputMouse* m_pInputMouse;

	// �����o�ϐ�
	D3DXMATRIX m_mtxProjection;				// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;					// �r���[�}�g���b�N�X


	D3DXVECTOR3 m_posV;						// ���_
	D3DXVECTOR3 m_posR;						// �����_
	D3DXVECTOR3 m_RSpeed;					// �����_�̑���

	D3DXVECTOR3 m_posVDest;					// ���_
	D3DXVECTOR3 m_posRDest;					// �����_

	D3DXVECTOR3 m_vecU;						// ������x�N�g��

	D3DXVECTOR3 m_rot;						// ����

	float m_fDistance;						// ���_���璍���_�̋���
	float m_fFov;							// ����p
	float m_fZnear;							// ���E�̍ŏ��l
	float m_fZfar;							// ���E�̍ő�l
	float m_fSpeedV;						// �ڕW�̈ʒu�܂ł̑��x
	bool m_isMovie;							// ���[�r�[�����ǂ���

	// ���[�r�[�p
	std::vector<MOTIONINFO> m_vMotionInfo;					// ���[�V�����̏��
	MOTIONTYPE m_nMotionType;								// ���̃��[�V�����̎��
	int m_nAllFrame;										// �S�̃t���[��
	int m_nKey, m_nCounterMotion, m_nNextKey;				// �L�[�̏��ƃ��[�V�����J�E���^�[
	bool m_bFinishMotion;									// ���[�V�������I��������ǂ���
	MODE m_mode;                                            // ���[�h sato Add
};
#endif // ! _CAMERA_H_