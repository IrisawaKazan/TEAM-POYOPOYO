//==============================================================
//
// [tutorialBoard.h]
// Author: Irisawa Kazan
//
//==============================================================
#ifndef _TUTORIALBOARD_H_
#define _TUTORIALBOARD_H_

// インクルード


// 前方宣言
class CObject2D;

class CTutorialBoard
{
public:
	CTutorialBoard();
	~CTutorialBoard();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CTutorialBoard* Create(void);

	void SetUp(std::string boardpath);

	bool GetProgress(void) { return m_Isprogress; }

private:
	CObject2D* m_pBackground;	// 
	CObject2D* m_pBoard;		// 
	bool m_Isprogress;			// 
};

#endif