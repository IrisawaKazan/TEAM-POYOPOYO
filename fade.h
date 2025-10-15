//****************************************************************
//
// �t�F�[�h�̏���[fade.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _FADE_H_
#define _FADE_H_

// �C���N���[�h
#include "scene.h"

// �t�F�[�h�N���X���`
class CFade
{
public:
	// �t�F�[�h�̋K��l��ݒ�
	struct Config {
		static const D3DXCOLOR DefoultCol;		// �ʏ�F
		static constexpr float Speed = 0.04f;	// ����
	};

	//�t�F�[�h�̏��
	typedef enum
	{
		FADE_NONE = 0,	// �ʏ�
		FADE_IN,		// �t�F�[�h�C��
		FADE_OUT,		// �t�F�[�h�A�E�g
		FADE_MAX,		// �ő�l
	}FADE;

	// �f�X�g���N�^
	~CFade();

	//�v���g�^�C�v�錾
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	// �Z�b�^�[
	static void SetFade(CScene* pNextScene);

	// �Q�b�^�[
	static FADE GetFade(void) { return m_pSingleto->m_Fade; };
	static CFade* GetFadeSingle(void) { return m_pSingleto; };

	// ����
	static CFade* CreateSingle(void);
private:
	// �R���X�g���N�^
	CFade() {};

	// �ÓI�����o�ϐ�
	static CFade* m_pSingleto;			// �V���O���g��
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// ���_�o�b�t�@
	FADE m_Fade;						// �t�F�[�h�̏��
	CScene* m_pNexScene;				// ���̃V�[���̃C���X�^���X
	D3DXCOLOR m_colorFade;				// �t�F�[�h�̐F
};
#endif // !_FADE_H_