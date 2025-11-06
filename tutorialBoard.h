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

class CTutorialBoard : public CObject
{
public:
	CTutorialBoard();
	~CTutorialBoard();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTutorialBoard* Create(void);

private:
	CObject2D* m_pBackground;	// 
	CObject2D* m_pBoard;		// 
	bool m_Isprogress;			// 
};

#endif