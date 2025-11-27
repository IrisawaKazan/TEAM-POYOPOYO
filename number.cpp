//****************************************************************
//
// [number.h]
// Author: Irisawa Kazan
//
//****************************************************************
#include "number.h"
#include "manager.h"
#include "texturemanager.h"
#include "object2D.h"

////****************************************************************
//// 静的メンバ変数
////****************************************************************
//bool CNumber::m_bEasing = NULL;

//****************************************************************
// 生成
//****************************************************************
CNumber* CNumber::Create(size_t digits, TYPE type, const char* texturePath, D3DXVECTOR2 texUVCount, D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber, int priority)
{
	// インスタンスの生成
	CNumber* pNumber = new CNumber(digits, priority); // 表示桁数
	if (pNumber == nullptr)
	{
		return nullptr;
	}

	pNumber->SetType(type);               // タイプ
	pNumber->SetTexturePath(texturePath); // テクスチャ
	pNumber->SetPos(pos);                 // 位置
	pNumber->SetSize(size);               // 大きさ(1つ分)
	pNumber->SetNumber(nNumber);          // 数値
	pNumber->SetTexUVCount(texUVCount);   // テクスチャ分割

	// 初期化
	if (FAILED(pNumber->Init()))
	{
		delete pNumber;
		return nullptr;
	}
	return pNumber;
}

//****************************************************************
// 初期化
//****************************************************************
HRESULT CNumber::Init()
{
	switch (m_type)
	{
		// 通常(スコアなど)
	case CNumber::TYPE::Normal:
	{
		float allWidth = m_size.x * float(m_digits);
		D3DXVECTOR3 leftPos = D3DXVECTOR3(m_pos.x - allWidth * 0.5f + m_size.x * 0.5f, m_pos.y, 0.0f);
		for (size_t cnt = 0; cnt < m_digits; ++cnt, leftPos.x += m_size.x)
		{
			m_pObjects.push_back(CObject2D::Create(leftPos, VEC3_NULL, m_size));
		}
		size_t idx{ m_digits - 1 };
		for (auto& pObject : m_pObjects)
		{
			pObject->SetAlphaTest(true);
			pObject->SetTexIndx(CTextureManager::Instance()->Register(m_texturePath));
			pObject->SetUv(D3DXVECTOR2((float)GetDigit(idx), 0), m_texUVSize.x, m_texUVSize.y);
			--idx;
		}
		break;
	}
	// 時間表示
	case CNumber::TYPE::Time:
	{
		float digits = float(m_digits * 3 - 1);
		float allWidth = m_size.x * digits;
		D3DXVECTOR3 leftPos = D3DXVECTOR3(m_pos.x - allWidth * 0.5f + m_size.x * 0.5f, m_pos.y, 0.0f);
		for (size_t cnt = 0; cnt < digits; ++cnt, leftPos.x += m_size.x)
		{
			m_pObjects.push_back(CObject2D::Create(leftPos, VEC3_NULL, m_size * 0.5f));
		}

		size_t idx{};
		bool isOK{};  // それ以降は実行
		if (m_digits >= 3)
		{
			for (size_t cnt = 0; cnt < 2; ++cnt, ++idx)
			{// 時間
				m_pObjects[idx]->SetAlphaTest(true);
				m_pObjects[idx]->SetTexIndx(CTextureManager::Instance()->Register(m_texturePath));
				m_pObjects[idx]->SetUv(D3DXVECTOR2((float)GetDigit(GetHour(), 1 - cnt), 0), m_texUVSize.x, m_texUVSize.y);
			}

			// コロン
			m_pObjects[idx]->SetAlphaTest(true);
			m_pObjects[idx]->SetTexIndx(CTextureManager::Instance()->Register(m_texturePath));
			m_pObjects[idx]->SetUv(D3DXVECTOR2(COLON, 0), m_texUVSize.x, m_texUVSize.y);
			++idx;
			isOK = true;
		}
		if (isOK || m_digits == 2)
		{
			for (size_t cnt = 0; cnt < 2; ++cnt, ++idx)
			{// 分
				m_pObjects[idx]->SetAlphaTest(true);
				m_pObjects[idx]->SetTexIndx(CTextureManager::Instance()->Register(m_texturePath));
				m_pObjects[idx]->SetUv(D3DXVECTOR2((float)GetDigit(GetMinute(), 1 - cnt), 0), m_texUVSize.x, m_texUVSize.y);
			}

			// コロン
			m_pObjects[idx]->SetAlphaTest(true);
			m_pObjects[idx]->SetTexIndx(CTextureManager::Instance()->Register(m_texturePath));
			m_pObjects[idx]->SetUv(D3DXVECTOR2(COLON, 0), m_texUVSize.x, m_texUVSize.y);
			++idx;
			isOK = true;
		}
		if (isOK || m_digits <= 1)
		{
			for (size_t cnt = 0; cnt < 2; ++cnt, ++idx)
			{// 秒
				m_pObjects[idx]->SetAlphaTest(true);
				m_pObjects[idx]->SetTexIndx(CTextureManager::Instance()->Register(m_texturePath));
				m_pObjects[idx]->SetUv(D3DXVECTOR2((float)GetDigit(GetSecond(), 1 - cnt), 0), m_texUVSize.x, m_texUVSize.y);
			}
		}
		break;
	}
	}

	//m_nIdx = CTextureManager::Instance()->Register(FileName);
	//m_nColTime = 0;
	//m_MaxFrame = 30;
	//m_nAnimCounter = NULL;
	//m_Type = type;
	//m_bEasing = false;

	//m_fX[0] = fX1 + nCnt * fNum1;
	//m_fX[1] = fX2 + nCnt * fNum1 + fNum2;

	//m_fY[0] = fY1 + nCnt2 * fNum3;
	//m_fY[1] = fY2 + nCnt2 * fNum3;

	//D3DXVECTOR2 screenSize{};
	//CManager::GetRenderer()->GetBackBufferSize(&screenSize);
	//m_Dest = { m_fX[0] * 1.0f,screenSize.y * 0.5f,0.0f };	    // 目標位置
	//m_Apper[0] = { m_fX[0] * 8.425f,screenSize.y * 0.5f,0.0f };	// 初期位置
	//m_Apper[1] = { m_fX[1] * 8.425f,screenSize.y * 0.5f,0.0f };	// 初期位置

	//if (m_Type != TYPE_NONE)
	//{
	//	fX1 = 1280.0f;
	//	fX2 = 1280.0f;
	//	fY1 = 360.0f;
	//	fY2 = 360.0f;
	//}

	return S_OK;
}

//****************************************************************
// 終了
//****************************************************************
void CNumber::Uninit(void)
{
	// 数字オブジェクトの破棄
	for (auto& pObject : m_pObjects)
	{
		if (pObject != nullptr)
		{
			pObject->Uninit();
			pObject = nullptr;
		}
	}
	m_pObjects.clear();
	m_pObjects.shrink_to_fit();
	Release();
}

//****************************************************************
// 更新
//****************************************************************
void CNumber::Update(void)
{
	switch (m_type)
	{
	// 通常(スコアなど)
	case CNumber::TYPE::Normal:
	{
		size_t idx{ m_digits - 1 };
		for (auto& pObject : m_pObjects)
		{
			pObject->SetUv(D3DXVECTOR2((float)GetDigit(idx), 0), m_texUVSize.x, m_texUVSize.y);
			--idx;
		}
		break;
	}
	// 時間用
	case CNumber::TYPE::Time:
	{
		size_t idx{};
		bool isOK{};  // それ以降は実行
		if (m_digits >= 3)
		{
			for (size_t cnt = 0; cnt < 2; ++cnt, ++idx)
			{// 時間
				m_pObjects[idx]->SetUv(D3DXVECTOR2((float)GetDigit(GetHour(), 1 - cnt), 0), m_texUVSize.x, m_texUVSize.y);
			}
			++idx;
			isOK = true;
		}
		if (isOK || m_digits == 2)
		{
			for (size_t cnt = 0; cnt < 2; ++cnt, ++idx)
			{// 分
				m_pObjects[idx]->SetUv(D3DXVECTOR2((float)GetDigit(GetMinute(), 1 - cnt), 0), m_texUVSize.x, m_texUVSize.y);
			}
			++idx;
			isOK = true;
		}
		if (isOK || m_digits <= 1)
		{
			for (size_t cnt = 0; cnt < 2; ++cnt, ++idx)
			{// 秒
				m_pObjects[idx]->SetUv(D3DXVECTOR2((float)GetDigit(GetSecond(), 1 - cnt), 0), m_texUVSize.x, m_texUVSize.y);
			}
		}
		break;
	}
	}
}

//****************************************************************
// 描画
//****************************************************************
void CNumber::Draw(void)
{

}

//****************************************************************
// 右座標
//****************************************************************
D3DXVECTOR3 CNumber::GetLeftPos()
{
	switch (m_type)
	{
		// 通常(スコアなど)
	case CNumber::TYPE::Normal:
	{
		float allWidth = m_size.x * float(m_digits);
		D3DXVECTOR3 leftPos = D3DXVECTOR3(m_pos.x - allWidth * 0.5f, m_pos.y, 0.0f);
		return leftPos;
	}
	// 時間表示
	case CNumber::TYPE::Time:
	{
		float digits = float(m_digits * 3 - 1);
		float allWidth = m_size.x * digits;
		D3DXVECTOR3 leftPos = D3DXVECTOR3(m_pos.x - allWidth * 0.5f, m_pos.y, 0.0f);
		return leftPos;
	}
	}
	return m_pos;
}

//****************************************************************
// 左座標
//****************************************************************
D3DXVECTOR3 CNumber::GetRightPos()
{
	switch (m_type)
	{
		// 通常(スコアなど)
	case CNumber::TYPE::Normal:
	{
		float allWidth = m_size.x * float(m_digits);
		D3DXVECTOR3 rightPos = D3DXVECTOR3(m_pos.x + allWidth * 0.5f, m_pos.y, 0.0f);
		return rightPos;
	}
	// 時間表示
	case CNumber::TYPE::Time:
	{
		float digits = float(m_digits * 3 - 1);
		float allWidth = m_size.x * digits;
		D3DXVECTOR3 rightPos = D3DXVECTOR3(m_pos.x + allWidth * 0.5f, m_pos.y, 0.0f);
		return rightPos;
	}
	}
	return m_pos;
}

//****************************************************************
// 上座標
//****************************************************************
D3DXVECTOR3 CNumber::GetTopPos()
{
	float allHeight = m_size.y;
	D3DXVECTOR3 topPos = D3DXVECTOR3(m_pos.x, m_pos.y - allHeight * 0.5f, 0.0f);
	return topPos;
}

//****************************************************************
// 下座標
//****************************************************************
D3DXVECTOR3 CNumber::GetBottomPos()
{
	float allHeight = m_size.y;
	D3DXVECTOR3 bottomPos = D3DXVECTOR3(m_pos.x, m_pos.y + allHeight * 0.5f, 0.0f);
	return bottomPos;
}

//****************************************************************
// 桁を返す
//****************************************************************
int CNumber::GetDigit(int digitIndex)
{
	int tempNumber = m_number; // コピー

	// 指定された桁まで位を下げる
	for (int cnt = 0; cnt < digitIndex; ++cnt)
	{
		tempNumber /= 10;
	}

	// 1の位を取り出す
	return tempNumber % 10;
}

//****************************************************************
// 桁を返す
//****************************************************************
int CNumber::GetDigit(int number, int digitIndex)
{
	// 指定された桁まで位を下げる
	for (int cnt = 0; cnt < digitIndex; ++cnt)
	{
		number /= 10;
	}

	// 1の位を取り出す
	return number % 10;
}