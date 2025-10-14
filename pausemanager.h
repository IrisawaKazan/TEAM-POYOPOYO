//****************************************************************
//
// �|�[�Y�}�l�[�W���[�̏���[pausemanager.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _PAUSEMANAGER_H_
#define _PAUSEMANAGER_H_

// �C���N���[�h
#include "pause.h"

// �|�[�Y�}�l�W���[�N���X���`
class CPauseManager
{
public:
	// �|�[�Y���j���[�̋K��l��ݒ�
	struct Config {
		static constexpr float OffSet = 75.0f;		// ���j���[�̌���
		static constexpr float Width = 100.0f;		// ����
		static constexpr float Height = 25.0f;		// ����
		static constexpr float DestWidth = 125.0f;	// ����
		static constexpr float DestHeight = 50.0f;	// ����
		static constexpr int Frame = 30;			// �t���[��
	};

	// �f�X�g���N�^
	~CPauseManager();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// �Z�b�^�[
	void SetMenu(CPause::Menu Menu) { m_SelectMenu = Menu; };

	// �Q�b�^�[
	CPause::Menu GetSelectMenu(void) { return m_SelectMenu; };

	// �ÓI�����o�֐�
	// �Q�b�^�[
	static CPauseManager* GetSingleton(void) { return m_Singleton; };
	// ����
	static CPauseManager* CreateSingleton(void);
private:
	// �R���X�g���N�^
	CPauseManager() {};

	// �ÓI�����o�ϐ�
	static CPauseManager* m_Singleton;	// �V���O���g��
	std::vector<CPause*> m_apPauseMenu;	// �|�[�Y���j���[
	CPause::Menu m_SelectMenu;			// �I��ł��郁�j���[
};
#endif // !_PAUSEMANAGER_H_