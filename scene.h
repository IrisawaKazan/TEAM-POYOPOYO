//****************************************************************
//
// �V�[���̏���[scene.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _SCENE_H_
#define _SCENE_H_

// �C���N���[�h
#include "object.h"

// �V�[���N���X���`
class CScene
{
public:
	// �V�[���̗񋓌^
	typedef enum
	{
		MODE_TITLE = 0,		// �^�C�g���V�[��
		MODE_TUTORIAL,		// �`���[�g���A���V�[��
		MODE_GAME,			// �Q�[���V�[��
		MODE_RESULT,		// ���U���g�V�[��
		MODE_MAX,			// �V�[���̍ő吔
	}MODE;

	// �R���X�g���N�^�E�f�X�g���N�^
	CScene(MODE Mode);
	virtual ~CScene();

	// �����o�֐�(�������z)
	virtual HRESULT Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Draw(void) = 0;

	// �Q�b�^�[
	MODE GetMode(void) { return m_Mode; };
private:
	// �����o�ϐ�
	MODE m_Mode;	// ���̃��[�h
};
#endif // !_SCENE_H_