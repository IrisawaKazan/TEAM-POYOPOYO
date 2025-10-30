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
class CMapManager;

// �Q�[���V�[���N���X���`
class CGame : public CScene
{
public:
	// �K��l��ݒ�
	struct Config {
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
	static CPlayerManager* GetPlayerManager(void) { return m_pPlayerManager; }; // sato Add

private:
	// �i�r�Q�[�V�����I�u�W�F�N�gUI�̃e�N�X�`�����X�g
	static constexpr std::array<const char*, 4u> NAVI_UI_TEXTURES =
	{
		"data/TEXTURE/UI/ArrowMark002.png",
		"data/TEXTURE/UI/ArrowMark001.png",
		"data/TEXTURE/UI/ArrowMark003.png",
		"data/TEXTURE/UI/ArrowMark000.png"
	};

	// �ÓI�����o�ϐ�
	static CPauseManager* m_pPauseManager;				// �|�[�Y�}�l�[�W���[�ւ̃|�C���^
	static CPlayerManager* m_pPlayerManager;			// �v���C���[�}�l�[�W���[
	static CMapManager* m_pMapManager;			// �v���C���[�}�l�[�W���[
	static bool m_isPause;								// �|�[�Y�����ǂ���
};
#endif // !_GAME_H_