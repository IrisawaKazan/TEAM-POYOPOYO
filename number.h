////==============================================================
////
//// [number.h]
//// Author: Irisawa Kazan
////
////==============================================================
//#ifndef _NUMBER_H_ // このマクロ定義がされてなかったら
//#define _NUMBER_H_ // 2重インクルード防止のマクロ定義
//
//#include"main.h"
//
//// マクロ定義
//#define MAX_NUM (64)
//
//// ナンバークラス
//class CNumber
//{
//public:
//    CNumber();
//    virtual ~CNumber();
//
//    HRESULT Init(D3DXVECTOR3 pos, float sizeX, float sizeY);
//    void Uninit(void);
//    void Draw(void);
//    
//    void SetPosition(D3DXVECTOR3 pos);
//    void SetUV(float aUV, float zUV);
//    void SetSize(float sizeX, float sizeY);
//    void SetDigit(int digit);
//
//    void Timer(int timer);
//
//    void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
//
//private:
//    LPDIRECT3DTEXTURE9 m_pTexture;
//    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
//    D3DXVECTOR3 m_pos;
//    float m_faUV;
//    float m_fzUV;
//    float m_fSizeX;
//    float m_fSizeY;
//};
//
//#endif