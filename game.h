//****************************************************************
//
// �Q�[���̏���[game.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��\�C���N���[�h�h�~
#ifndef _GAME_H_
#define _GAME_H_

// �C���N���[�h
#include "scene.h"

// �O���錾
class CPlayer;
class CScore;
class CTimer;
class CMapEditer;
class CPauseManager;
class CPlayerManager;

// �Q�[���V�[���N���X���`
class CGame : public CScene
{
public:
	// �K��l��ݒ�
	struct Config {
		// �p�X
		struct FilePath {
			static constexpr const char* MeshField =		"data\\TEXT\\DefoultStage\\MeshField.txt";		// ���b�V���̏��
			static constexpr const char* MapObjectInfo =	"data\\TEXT\\DefoultStage\\MapInfo.json";		// �}�b�v�̏��
			static constexpr const char* GimmickInfo =		"data\\TEXT\\DefoultStage\\GimmickInfo.json";	// �M�~�b�N�̏��
			static constexpr const char* MapWallInfo =		"data\\TEXT\\DefoultStage\\MapWallInfo.json";	// �����Ȃ��ǂ̏��
		};
		// ���b�V���V�����_�[
		struct MeshCylinder {
			static const D3DXVECTOR3 Pos;				// �ʒu
			static const D3DXVECTOR3 Rot;				// ����
			static constexpr float Radius = 5000.0f;	// ���a
			static constexpr float Height = 2500.0f;	// ����
			static constexpr int DivX = 8;				// ������
			static constexpr int DivY = 3;				// ������
		};
		// ��
		struct River {
			static const D3DXVECTOR3 Pos;				// �ʒu
			static const D3DXVECTOR2 Size;				// �傫��
		};
		// �v���C���[
		struct Player {
			static const D3DXVECTOR3 Pos;				// �ʒu
			static const D3DXVECTOR3 Rot;				// ����
		};
		// �J����
		struct Camera {
			static const D3DXVECTOR3 Rot;				// ����
			static const D3DXVECTOR3 PosR;				// �����_
			static const D3DXVECTOR3 PosV;				// �n�_
		};
		// �������ԗp�̉�
		struct WildFire {
			static constexpr float FireDistance = 200.0f;	// �Ԋu
			static constexpr int FireInterval = 30;			// ���Ԃ̊Ԋu
			static constexpr int FireNum = 6;				// ��
		};
		// ��
		struct Sky {
			static const D3DXVECTOR3 Pos;				// �ʒu
			static constexpr float Radius = 7000.0f;	// ���a
			static constexpr int Horizon = 8;			// ������
			static constexpr int Vertical = 5;			// ������
		};
	};
	// �R���X�g���N�^�E�f�X�g���N�^
	CGame();
	~CGame();

	// �����o�֐�
	HRESULT Init(void)	override;
	void Update(void)	override;
	void Uninit(void)	override;
	void Draw(void)		override;

	// �ÓI�����o�֐�
	// �Z�b�^�[
	static void ResetPlayer(void);

	// �Q�b�^�[
	static bool GetPause(void) { return m_isPause; };
private:
	// �ÓI�����o�ϐ�
	static CPauseManager* m_pPauseManager;				// �|�[�Y�}�l�[�W���[�ւ̃|�C���^
	static CPlayerManager* m_pPlayerManager;			// �v���C���[�}�l�[�W���[
	static D3DXVECTOR3 m_WildFirePos;					// �R�Ύ��̈ʒu�̔z��
	static bool m_isPause;								// �|�[�Y�����ǂ���
	std::unique_ptr<btCollisionShape> m_GroundShape;	// �����蔻��
	std::unique_ptr<btRigidBody> m_RigitBody;			// ���W�b�g�{�f�B�[
};
#endif // !_GAME_H_