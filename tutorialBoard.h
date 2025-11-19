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

	void SetUp(std::string boardpath, const bool bNextBoard);

	bool GetProgress(void) { return m_Isprogress; }

private:
	CObject2D* m_pBackground;	// 
	CObject2D* m_pBoard;		// 
	bool m_Isprogress;			// 
	float m_fCountFrame;		// 現在のフレーム数 Misaki
	float m_fMaxFrame;			// 最大フレーム数
	D3DXVECTOR3 m_Dest;			// 目標位置 Misaki
	D3DXVECTOR3 m_pos;			// 初期位置 Misaki
	bool m_bNextBoard;			// 次のボードがあるかどうか Misaki
	int m_nCount;				// 
};

#endif