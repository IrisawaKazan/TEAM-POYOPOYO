//*************************************
//
//　モーション処理[motion.h]
//　Author:Hatori Kensaku
//
//*************************************
// 二重インクルード防止
#ifndef _MOTION_H_
#define _MOTION_H_
// インクルード
#include "model.h"
#include "main.h"
//#include "collision.h"

// モーションクラスを定義
class CMotion
{
public:
	// パーツのキー情報
	struct KEY
	{
		// 目標の位置
		float fPosX;
		float fPosY;
		float fPosZ;
		// 目標の向き
		float fRotX;
		float fRotY;
		float fRotZ;
	};
	// モーションのキー情報
	struct KEYINFO
	{
		int nFrame;				// フレーム
		std::vector<KEY> pKey;	// キー情報
	};
	// モーション情報
	struct MOTIONINFO
	{
		bool bLoop;						// ループするかどうか
		int nNumKey;					// キーの総数
		std::vector<KEYINFO> pKeyInfo;	// キーの情報
	};
	// コンストラクタ・デストラクタ
	CMotion();
	~CMotion();
	// メンバ関数
	void Uninit(void);
	void Update(std::vector<CModel*> &pModel);
	void UpdateKey(void);
	void UpdateCurrentMotion(CModel* pModel);
	void UpdateBlendMotion(CModel* pModel);
	// 条件式の関数化
	bool isUpdateCurrentMotion(void) { return m_bFinishMotion == false && m_bFirstBlend == false; };
	bool isEvent(const int StartFrame, const int EndFrame);
	// ローダー
	void LoadCharactorInfo(std::ifstream& ifs, std::vector <CModel*> pModel);
	void LoadMotionInfo(std::ifstream& ifs, int* nMotionCount);
	// セッター
	void SetNumModel(int nNumModel) { m_nNumModel = nNumModel; };
	void SetNumMotion(int nNumMotion) { m_nNumMotion = nNumMotion; };
	void SetMotion(int nMotion,const bool isKeep);
	void SetisKeep(const bool bKeep) { m_bKeep = bKeep; };
	// ゲッター
	int GetNumModel(void) { return m_nNumModel; };
	int GetNumMotion(void) { return m_nNumMotion; };
	int GetMotion(void) { return m_nMotionType; };
	int GetBlendMotion(void) { return m_nBlendMotionType; };
	int GetFirstBlend(void) { return m_bFirstBlend; };
	int GetKey(void) { return m_nKey; };
	int GetNextKey(void) { return m_nNextKey; };
	int GetBlendKey(void) { return m_nKeyBlend; };
	int GetBlendNextKey(void) { return m_nNextKeyBlend; };
	int GetMotionCount(void) { return m_nCounterMotion; };
	int GetMotionBlendCount(void) { return m_nCounterBlend; };
	int GetMotionFrame(void) { return m_nCounterMotion; };
	bool GetFinishMotion(void) { return m_bFinishMotion; };
	// 静的メンバ関数
	static CMotion* LoadMotionFile(const char* cFileName, std::vector <CModel*> &pModel);
private:
	// メンバ変数
	std::vector<MOTIONINFO> m_MotionInfo;										// モーションの情報
	int m_nAllFrame;															// 全体フレーム
	int m_nMotionType, m_nBlendMotionType;										// 今のモーションの種類、ブレンド先のモーションの種類
	int m_nNumKey, m_nKey, m_nCounterMotion, m_nNextKey;						// キーの情報とモーションカウンター
	int m_nNumModel;															// モデルの数
	int m_nNumMotion;															// モーションの数
	int m_nNumKeyBlend, m_nKeyBlend, m_nCounterMotionBlend, m_nNextKeyBlend;	// ブレンド先のキーの情報とカウンター
	int m_nCounterBlend, m_nFrameBlend;											// ブレンド先のモーションのカウンターとブレンドのカウンター
	bool m_bBlendMotion;														// ブレンドするかどうか
	bool m_bFinishMotion;														// モーションが終わったかどうか
	bool m_bFirstBlend;															// 最初のブレンド
	bool m_bKeep;																// 最後のキーで維持するかどうか(上半身用)

};
#endif // !_MOTION_H_

