//************************************************************
//
//　階層構造があるキャラクターの処理[modelcharactor.h]
//　Author:Hatori Kensaku
//
//************************************************************
// 二重インクルード防止
#ifndef _CHARCTER_H_
#define _CHARCTER_H_
// インクルード
#include "object.h"
#include "motion.h"
#include "model.h"
// オブジェクトの派生クラスを定義[階層構造があるキャラクター]
class CModelCharacter : public CObject
{
public:
	// コンストラクタ・デストラクタ
	CModelCharacter(int Priority = 3);
	~CModelCharacter();
	// メンバ関数
	HRESULT Init(const char* MotionFilePath,float Speed);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ゲッター
	CMotion* GetMotionInfo(void) { return m_MotionInfo; };
	CModel* GetModel(const int Indx) { return m_apModel[Indx]; };
	std::vector<CModel*>& GetvModel(void) { return m_apModel; };
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR3* GetpPos(void) { return &m_Pos; };
	D3DXVECTOR3 GetPosOld(void) { return m_OldPos; };
	D3DXVECTOR3 GetRot(void) { return m_Rot; };
	D3DXVECTOR3 GetRotDest(void) { return m_RotDest; };
	D3DXVECTOR3 GetMove(void) { return m_Move; };
	int GetState(void) { return m_State; };
	int GetMotion(void) { return m_Motion; };
	float GetSpeed(void) { return m_fSpeed; };
	bool GetEnable(void) { return m_Enable; };
	// セッター
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; };
	void SetRotDest(D3DXVECTOR3 RotDest) { m_RotDest = RotDest; };
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; };
	void SetmtxRotChest(D3DXMATRIX mtxRot) { m_mtxRotChest = mtxRot; };
	void SetState(int State) { m_State = State; };
	void SetSpeed(float Speed) { m_fSpeed = Speed; };
	void OnLanding(void) { m_isLanding = true; };
	void OffLanding(void) { m_isLanding = false; };
	void SetEnable(bool Enable) { m_Enable = Enable; };
	void ToggleEnable(void) { m_Enable = !m_Enable; };
private:
	// メンバ変数
	std::vector<CModel*> m_apModel;							// モデルの情報を格納する変数
	CMotion* m_MotionInfo;									// モーションの情報
	D3DXMATRIX m_mtxWorld;									// マトリックス
	D3DXMATRIX m_mtxRotChest;								// 胴体の回転行列
	D3DXVECTOR3 m_Pos;										// 位置
	D3DXVECTOR3 m_OldPos;									// 位置フレーム前の位置
	D3DXVECTOR3 m_Rot;										// 向き
	D3DXVECTOR3 m_RotDest;									// 目標の向き
	D3DXVECTOR3 m_Move;										// プレイヤーの移動量
	int m_State;											// プレイヤーのステータス
	int m_Motion;											// 現在のモーション
	float m_fSpeed;											// プレイヤーの移動速度
	bool m_isLanding;										// 着地しているかどうか
	bool m_Enable;											// 有効かどうか
};
#endif // !_MODELCHARACTOR_H_














