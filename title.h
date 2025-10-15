//****************************************************************
//
// �^�C�g���̏���[title.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _TITLE_H_
#define _TITLE_H_

// �C���N���[�h
#include "scene.h"

class CPlayerTitle;
class CMapEditer;
class CTitleManager;

// �^�C�g���N���X���`
class CTitle : public CScene
{
public:
	// �萔��ݒ�
	struct Config {
		//  �J����
		struct Camera {
			static const D3DXVECTOR3 PosV;	// �n�_�̈ʒu
			static const D3DXVECTOR3 PosR;	// �����_�̈ʒu
			static const D3DXVECTOR3 Rot;	// ����
		};
		// �v���C���[
		struct Player {
			static const D3DXVECTOR3 Pos;	// �ʒu
			static const D3DXVECTOR3 Rot;	// ����
		};
		// ��
		struct Grass {
			static constexpr float MaxHeight = 40.0f;	// ���̍���(�ő�)
			static constexpr float MinHeight = 30.0f;	// ���̍���(�ŏ�)
		};
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CTitle();
	~CTitle();

	// �����o�֐�
	HRESULT Init(void)	override;
	void Update(void)	override;
	void Uninit(void)	override;
	void Draw(void)		override;
private:
	// �ÓI�����o�ϐ�
	static CPlayerTitle* m_pPlayer;				// �v���C���[�̃|�C���^
	static CTitleManager* m_pTitleManager;		// �|�[�Y�}�l�[�W���[�ւ̃|�C���^
};
#endif // !_TITLE_H_