////==============================================================
////
//// [number.h]
//// Author: Fuma sato
////
////==============================================================
#pragma once

#include "main.h"
#include "object.h"

class CObject2D; // 宣言

//------------------------------
// 数字表示クラス (Object継承)
//------------------------------
class CNumber : CObject
{
public:
	// タイプ
	enum class TYPE : unsigned char
	{
		Normal,
		Time,
		Max
	};

	CNumber(size_t digits, int priority) : CObject(priority), m_digits{ digits }, m_pObjects{}, m_type{}, m_texturePath{}, m_pos{}, m_size{}, m_number{}, m_texUVCount{}, m_texUVSize{} {}
	~CNumber() = default;

	static CNumber* Create(size_t digits, TYPE type, const char* texturePath, D3DXVECTOR2 texUVCount, D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber, int priority = 7);

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// セッター
	void SetType(TYPE type) { m_type = type; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }
	void SetTexturePath(const char* texturePath) { m_texturePath = texturePath; }
	void SetNumber(int number, bool isAdd = false) { isAdd ? m_number += number : m_number = number; }
	void SetTexUVCount(D3DXVECTOR2 texUVCount) { m_texUVCount = texUVCount; m_texUVSize = D3DXVECTOR2(1.0f / m_texUVCount.x, 1.0f / m_texUVCount.y); }

	// ゲッター
	int GetNumber() { return m_number; }
	int GetTotalSeconds() { return m_number; }
	int GetHour() { return m_number / 3600; }
	int GetMinute() { return (m_number / 60) % 60; }
	int GetSecond() { return m_number % 60; }

	D3DXVECTOR3 GetLeftPos();
	D3DXVECTOR3 GetRightPos();
	D3DXVECTOR3 GetTopPos();
	D3DXVECTOR3 GetBottomPos();

	//void SetPos(void);

	//// ゲッター
	//static bool GetEasing(void) { return m_bEasing; };

	//void ColAnim(void);

private:
	static constexpr int COLON = 10;                     // コロン

	int GetDigit(int digitIndex);
	int GetDigit(int number, int digitIndex);

	const size_t m_digits;              // 表示桁数
	std::vector<CObject2D*> m_pObjects; // 数字オブジェクト配列
	TYPE m_type;		                // タイプ
	const char* m_texturePath;          // テクスチャ
	D3DXVECTOR3 m_pos;	                // 位置
	D3DXVECTOR2 m_size;                 // 大きさ (数字1つ分)
	int m_number;                       // 数値
	D3DXVECTOR2 m_texUVCount;           // テクスチャの分割数
	D3DXVECTOR2 m_texUVSize;            // テクスチャ1ブロックのサイズ
							            
	//D3DXVECTOR3 m_Apper;                // 初期位置
	//D3DXVECTOR3 m_Dest;	                // 目的値
	//bool m_bChange;		                // カラー判定用
	//int m_nColTime;		                // カラー用の計算
	//int m_nIdx;			                // インデックス番号
	//float m_fX;				            
	//float m_fY;				            
	//int m_MaxFrame;			            // フレームカウンター
	//int m_nAnimCounter;		            // アニメーションカウンター
	//static bool m_bEasing;	            //　イージングが終わっているかどうか
};