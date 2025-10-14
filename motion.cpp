//*************************************
//
//　モーション処理[motion.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "motion.h"
#include "math_T.h"
#include "texturemanager.h"

// 名前空間
using namespace std;

//***************************************
// コンストラクタ
//***************************************
CMotion::CMotion()
{
	m_nCounterMotion = NULL;
	m_nKey = NULL;
	m_nNextKey = NULL;
	m_nNumModel = NULL;
	m_nNumMotion = NULL;
}

//***************************************
// デストラクタ
//***************************************
CMotion::~CMotion()
{
}

//***************************************
// 終了処理
//***************************************
void CMotion::Uninit(void)
{
	m_MotionInfo.clear();
	delete this;
}

//***************************************
// 更新処理
//***************************************
void CMotion::Update(vector<CModel*>& pModel)
{
	if (this == NULL) return;

	m_nAllFrame++;

	//全モデル(パーツ)の更新
	for (auto itr = pModel.begin(); itr != pModel.end(); itr++)
	{
		m_nMotionType = Clamp(m_nMotionType, 0, m_nNumMotion);

		m_nBlendMotionType = Clamp(m_nBlendMotionType, 0, m_nNumMotion);


		UpdateKey();

		if (isUpdateCurrentMotion() == true)
		{
			UpdateCurrentMotion(*itr);
		}
		else
		{
			UpdateBlendMotion(*itr);
		}
	}

	if (m_bFirstBlend == false)
	{
		if (m_MotionInfo[m_nMotionType].bLoop == false)
		{
			if (m_nKey >= m_MotionInfo[m_nMotionType].nNumKey - 1)
			{
				if (m_bKeep == false)
				{
					m_nBlendMotionType = 0;
					m_nFrameBlend = m_MotionInfo[m_nMotionType].pKeyInfo[m_nKey].nFrame;
				}
				m_bFinishMotion = true;
			}
		}
	}

	if (m_bFirstBlend == false)
	{
		if (m_bFinishMotion == false)
		{
			m_nCounterMotion++;
		}
	}
	//カウンターモーションのリセット処理
	if (m_nCounterMotion >= m_MotionInfo[m_nMotionType].pKeyInfo[m_nKey].nFrame)
	{
		if (m_nKey >= m_MotionInfo[m_nMotionType].nNumKey - 1)
		{
			m_nAllFrame = 0;
		}
		if (m_bFinishMotion == false && m_bKeep == false)
		{
			m_nCounterMotion = 0;
			m_nKey = Wrap(m_nKey + 1, 0, m_MotionInfo[m_nMotionType].nNumKey - 1);
		}
		else
		{
			m_nKey = Clamp(m_nKey + 1, 0, m_MotionInfo[m_nMotionType].nNumKey - 2);
			m_nCounterMotion = Clamp(m_nCounterMotion, 0, m_MotionInfo[m_nMotionType].pKeyInfo[m_nKey].nFrame);
		}
	}

	if (m_bFinishMotion == true || m_bFirstBlend == true)
	{
		m_nCounterBlend++;
		//カウンターモーションのリセット処理
		if (m_nCounterBlend >= m_nFrameBlend)
		{
			m_nKey = 0;
			m_nAllFrame = 0;
			m_nMotionType = m_nBlendMotionType;
			m_nCounterBlend = 0;
			if (m_bFinishMotion == true)
			{
				if (m_bKeep == false)
				{
					m_bFinishMotion = false;
				}
			}
			if (m_bFirstBlend == true)
			{
				m_nCounterMotion = m_nFrameBlend;
				m_bFirstBlend = false;
			}
		}
	}
}

//***************************************
// 現在のキーとブレンド先のキーを更新
//***************************************
void CMotion::UpdateKey(void)
{
	m_nNextKey = Wrap(m_nKey + 1, 0, m_MotionInfo[m_nMotionType].nNumKey - 1);
	m_nNextKeyBlend = Wrap(m_nKeyBlend + 1, 0, m_MotionInfo[m_nBlendMotionType].nNumKey - 1);
}

//***************************************
// 現在のモーションの更新
//***************************************
void CMotion::UpdateCurrentMotion(CModel* pModel)
{
	//位置の算出
	CMotion::KEY Value;
	CMotion::KEY nKey;
	CMotion::KEY nNextKey;

	//位置の算出
	Value;
	nKey = m_MotionInfo[m_nMotionType].pKeyInfo[m_nKey].pKey[pModel->GetIndx()];
	nNextKey = m_MotionInfo[m_nMotionType].pKeyInfo[m_nNextKey].pKey[pModel->GetIndx()];

	float fFrame = (float)(m_nCounterMotion) / (float)(m_MotionInfo[m_nMotionType].pKeyInfo[m_nKey].nFrame);

	//求める値
	Value.fPosX = pModel->GetOffSet().x + nKey.fPosX + Lerp(fFrame, nNextKey.fPosX, nKey.fPosX);
	Value.fPosY = pModel->GetOffSet().y + nKey.fPosY + Lerp(fFrame, nNextKey.fPosY, nKey.fPosY);
	Value.fPosZ = pModel->GetOffSet().z + nKey.fPosZ + Lerp(fFrame, nNextKey.fPosZ, nKey.fPosZ);

	Value.fRotX = nKey.fRotX + Lerp(fFrame, nNextKey.fRotX, nKey.fRotX);
	Value.fRotY = nKey.fRotY + Lerp(fFrame, nNextKey.fRotY, nKey.fRotY);
	Value.fRotZ = nKey.fRotZ + Lerp(fFrame, nNextKey.fRotZ, nKey.fRotZ);
	//パーツの位置・向きを設定
	pModel->SetPos(VEC3(Value.fPosX, Value.fPosY, Value.fPosZ));
	pModel->SetRot(VEC3(Value.fRotX, Value.fRotY, Value.fRotZ));
}

//***************************************
// 現在のブレンドモーションの更新
//***************************************
void CMotion::UpdateBlendMotion(CModel* pModel)
{
	//位置の算出
	CMotion::KEY Value;
	CMotion::KEY Diff;
	CMotion::KEY Current;
	CMotion::KEY nKey;
	CMotion::KEY nNextKey;

	//位置の算出
	CMotion::KEY ValueBlend;
	CMotion::KEY DiffMotionBlend;
	CMotion::KEY CurrentBlend;
	CMotion::KEY DiffBlend;
	CMotion::KEY nKeyBlend;
	CMotion::KEY nNextKeyBlend;

	nKey = m_MotionInfo[m_nMotionType].pKeyInfo[m_nKey].pKey[pModel->GetIndx()];
	nNextKey = m_MotionInfo[m_nMotionType].pKeyInfo[m_nNextKey].pKey[pModel->GetIndx()];

	float fFrame = (float)(m_nCounterMotion) / (float)(m_MotionInfo[m_nMotionType].pKeyInfo[m_nKey].nFrame);

	Diff.fPosX = nNextKey.fPosX - nKey.fPosX;
	Diff.fPosY = nNextKey.fPosY - nKey.fPosY;
	Diff.fPosZ = nNextKey.fPosZ - nKey.fPosZ;

	Diff.fRotX = nNextKey.fRotX - nKey.fRotX;
	Diff.fRotY = nNextKey.fRotY - nKey.fRotY;
	Diff.fRotZ = nNextKey.fRotZ - nKey.fRotZ;

	Current.fPosX = nKey.fPosX + (Diff.fPosX * fFrame);
	Current.fPosY = nKey.fPosY + (Diff.fPosY * fFrame);
	Current.fPosZ = nKey.fPosZ + (Diff.fPosZ * fFrame);

	Current.fRotX = nKey.fRotX + (Diff.fRotX * fFrame);
	Current.fRotY = nKey.fRotY + (Diff.fRotY * fFrame);
	Current.fRotZ = nKey.fRotZ + (Diff.fRotZ * fFrame);

	//求める値
	Value.fPosX = pModel->GetOffSet().x + nKey.fPosX + Lerp(fFrame, nNextKey.fPosX, nKey.fPosX);
	Value.fPosY = pModel->GetOffSet().y + nKey.fPosY + Lerp(fFrame, nNextKey.fPosY, nKey.fPosY);
	Value.fPosZ = pModel->GetOffSet().z + nKey.fPosZ + Lerp(fFrame, nNextKey.fPosZ, nKey.fPosZ);

	Value.fRotX = nKey.fRotX + Lerp(fFrame, nNextKey.fRotX, nKey.fRotX);
	Value.fRotY = nKey.fRotY + Lerp(fFrame, nNextKey.fRotY, nKey.fRotY);
	Value.fRotZ = nKey.fRotZ + Lerp(fFrame, nNextKey.fRotZ, nKey.fRotZ);

	//位置の算出
	ValueBlend;
	DiffMotionBlend;
	CurrentBlend;
	DiffBlend;
	nKeyBlend = m_MotionInfo[m_nBlendMotionType].pKeyInfo[m_nKeyBlend].pKey[pModel->GetIndx()];
	nNextKeyBlend = m_MotionInfo[m_nBlendMotionType].pKeyInfo[m_nNextKeyBlend].pKey[pModel->GetIndx()];

	float fFrameBlendMotion = (float)(m_nCounterBlend) / (float)(m_MotionInfo[m_nBlendMotionType].pKeyInfo[m_nKeyBlend].nFrame);
	float fFrameBlend = (float)(m_nCounterBlend) / (float)(m_nFrameBlend);

	DiffMotionBlend.fPosX = nNextKeyBlend.fPosX - nKeyBlend.fPosX;
	DiffMotionBlend.fPosY = nNextKeyBlend.fPosY - nKeyBlend.fPosY;
	DiffMotionBlend.fPosZ = nNextKeyBlend.fPosZ - nKeyBlend.fPosZ;

	DiffMotionBlend.fRotX = nNextKeyBlend.fRotX - nKeyBlend.fRotX;
	DiffMotionBlend.fRotY = nNextKeyBlend.fRotY - nKeyBlend.fRotY;
	DiffMotionBlend.fRotZ = nNextKeyBlend.fRotZ - nKeyBlend.fRotZ;

	CurrentBlend.fPosX = nKeyBlend.fPosX + (DiffMotionBlend.fPosX * fFrameBlendMotion);
	CurrentBlend.fPosY = nKeyBlend.fPosY + (DiffMotionBlend.fPosY * fFrameBlendMotion);
	CurrentBlend.fPosZ = nKeyBlend.fPosZ + (DiffMotionBlend.fPosZ * fFrameBlendMotion);

	CurrentBlend.fRotX = nKeyBlend.fRotX + (DiffMotionBlend.fRotX * fFrameBlendMotion);
	CurrentBlend.fRotY = nKeyBlend.fRotY + (DiffMotionBlend.fRotY * fFrameBlendMotion);
	CurrentBlend.fRotZ = nKeyBlend.fRotZ + (DiffMotionBlend.fRotZ * fFrameBlendMotion);

	DiffBlend.fPosX = CurrentBlend.fPosX - Current.fPosX;
	DiffBlend.fPosY = CurrentBlend.fPosY - Current.fPosY;
	DiffBlend.fPosZ = CurrentBlend.fPosZ - Current.fPosZ;

	DiffBlend.fRotX = CurrentBlend.fRotX - Current.fRotX;
	DiffBlend.fRotY = CurrentBlend.fRotY - Current.fRotY;
	DiffBlend.fRotZ = CurrentBlend.fRotZ - Current.fRotZ;

	ValueBlend.fPosX = pModel->GetOffSet().x + Current.fPosX + (DiffBlend.fPosX * fFrameBlend);
	ValueBlend.fPosY = pModel->GetOffSet().y + Current.fPosY + (DiffBlend.fPosY * fFrameBlend);
	ValueBlend.fPosZ = pModel->GetOffSet().z + Current.fPosZ + (DiffBlend.fPosZ * fFrameBlend);

	ValueBlend.fRotX = Current.fRotX + (DiffBlend.fRotX * fFrameBlend);
	ValueBlend.fRotY = Current.fRotY + (DiffBlend.fRotY * fFrameBlend);
	ValueBlend.fRotZ = Current.fRotZ + (DiffBlend.fRotZ * fFrameBlend);
	//パーツの位置・向きを設定
	pModel->SetPos(VEC3(ValueBlend.fPosX, ValueBlend.fPosY, ValueBlend.fPosZ));
	pModel->SetRot(VEC3(ValueBlend.fRotX, ValueBlend.fRotY, ValueBlend.fRotZ));
}

//***************************************
// モーションの読み込み処理
//***************************************
CMotion* CMotion::LoadMotionFile(const char* cFileName, vector<CModel*>& pModel)
{
	CMotion* pMotionInf = new CMotion;

	ifstream ifs(cFileName);
	if (!ifs)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return NULL;
	}

	string line = {};
	string label = {}, equal = {};
	int nNumModel = 0;

	int nModelCount = 0;

	int nMotionCount = 0;

	CModel* LocalpModel;

	while (getline(ifs, line))
	{
		char FilePath[MAX_PATH] = {};

		istringstream iss(line);

		if (line.find("NUM_MODEL") != string::npos)
		{
			iss >> label >> equal >> nNumModel;
			pMotionInf->SetNumModel(nNumModel);
		}
		if (line.find("MODEL_FILENAME") != string::npos)
		{
			iss >> label >> equal >> FilePath;
		}
		if (line.find("CHARACTERSET") != string::npos)
		{
			pMotionInf->LoadCharactorInfo(ifs, pModel);
		}
		if (line.find("MOTIONSET") != string::npos)
		{
			pMotionInf->LoadMotionInfo(ifs, &nMotionCount);
		}
		if (FilePath[0] != NULL)
		{
			LocalpModel = CModel::Create(FilePath);
			pModel.push_back(LocalpModel);
			nModelCount++;
		}

		if (line.find("#") != string::npos)
		{
			continue;
		}
	}
	pMotionInf->SetNumMotion(nMotionCount);
	return pMotionInf;
}

//***************************************
// イベントフレームいないかどうか
//***************************************
bool CMotion::isEvent(const int StartFrame, const int EndFrame)
{
	return m_nAllFrame >= StartFrame && m_nAllFrame < EndFrame;
}

//***************************************
// キャラクター情報を読み込む
//***************************************
void CMotion::LoadCharactorInfo(ifstream& ifs, std::vector <CModel*> pModel)
{
	string line = {};
	string label = {}, equal = {};

	int nModelIndx = 0;
	int nModelParent = 0;

	D3DXVECTOR3 Offset = VEC3_NULL;
	D3DXVECTOR3 Rot = VEC3_NULL;

	while (getline(ifs, line))
	{
		istringstream charactorInfo(line);
		if (line.find("END_CHARACTERSET") != string::npos)
		{
			break;
		}
		if (line.find("PARTSSET") != string::npos)
		{
			while (getline(ifs, line))
			{
				istringstream partsInfo(line);
				if (line.find("END_PARTSSET") != string::npos)
				{
					break;
				}
				if (line.find("INDEX") != string::npos)
				{
					partsInfo >> label >> equal >> nModelIndx;
					pModel[nModelIndx]->SetIndx(nModelIndx);
				}
				if (line.find("PARENT") != string::npos)
				{
					partsInfo >> label >> equal >> nModelParent;
					if (nModelParent != -1)
					{
						pModel[nModelIndx]->SetParent(pModel[nModelParent]);
					}
					else
					{
						pModel[nModelIndx]->SetParent(NULL);
					}
				}
				if (line.find("POS") != string::npos)
				{
					partsInfo >> label >> equal >> Offset.x >> Offset.y >> Offset.z;
					pModel[nModelIndx]->SetOffSet(Offset);
					pModel[nModelIndx]->SetPos(Offset);
				}
				if (line.find("ROT") != string::npos)
				{
					partsInfo >> label >> equal >> Rot.x >> Rot.y >> Rot.z;
					pModel[nModelIndx]->SetRot(Rot);
				}
			}
		}
	}
}

//***************************************
// モーションの情報を読み込む
//***************************************
void CMotion::LoadMotionInfo(ifstream& ifs, int* nMotionCount)
{
	string line = {};
	string label = {}, equal = {};

	int nKeyInfoCount = 0;

	MOTIONINFO LocalMotion;

	while (getline(ifs, line))
	{
		istringstream motionInfo(line);
		if (line.find("END_MOTIONSET") != string::npos)
		{
			LocalMotion.nNumKey = nKeyInfoCount;
			nKeyInfoCount = 0;
			m_MotionInfo.push_back(LocalMotion);
			(*nMotionCount)++;
			break;
		}
		if (line.find("LOOP") != string::npos)
		{
			motionInfo >> label >> equal >> LocalMotion.bLoop;
		}
		if (line.find("KEYSET") != string::npos)
		{
			KEYINFO LocalKeyInfo;
			while (getline(ifs, line))
			{
				istringstream keyInfo(line);
				if (line.find("END_KEYSET") != string::npos)
				{
					LocalMotion.pKeyInfo.push_back(LocalKeyInfo);
					nKeyInfoCount++;
					break;
				}
				if (line.find("FRAME") != string::npos)
				{
					keyInfo >> label >> equal >> LocalKeyInfo.nFrame;
				}
				if (line.find("KEY") != string::npos)
				{
					KEY LocalKey;
					while (getline(ifs, line))
					{
						istringstream key(line);
						if (line.find("END_KEY") != string::npos)
						{
							LocalKeyInfo.pKey.push_back(LocalKey);
							break;
						}
						if (line.find("POS") != string::npos)
						{
							VEC3 LoaclPos = VEC3_NULL;
							key >> label >> equal >> LoaclPos.x >> LoaclPos.y >> LoaclPos.z;
							LocalKey.fPosX = LoaclPos.x;
							LocalKey.fPosY = LoaclPos.y;
							LocalKey.fPosZ = LoaclPos.z;
						}
						if (line.find("ROT") != string::npos)
						{
							VEC3 LoaclRot = VEC3_NULL;
							key >> label >> equal >> LoaclRot.x >> LoaclRot.y >> LoaclRot.z;
							LocalKey.fRotX = LoaclRot.x;
							LocalKey.fRotY = LoaclRot.y;
							LocalKey.fRotZ = LoaclRot.z;
						}
						if (line.find("#") != string::npos)
						{
							continue;
						}
					}
				}
				if (line.find("#") != string::npos)
				{
					continue;
				}
			}
		}
		if (line.find("#") != string::npos)
		{
			continue;
		}
	}
}

//***************************************
// モーションを設定する
//***************************************
void CMotion::SetMotion(int nMotion, const bool isKeep)
{
	// 上半身
	m_nKeyBlend = 0;
	m_nNextKeyBlend = 0;
	m_nAllFrame = 0;
	if (m_bFirstBlend == true)
	{
		m_nCounterBlend = 0;
	}
	m_nBlendMotionType = nMotion;
	m_nFrameBlend = 10;
	m_bBlendMotion = true;
	m_bFinishMotion = false;
	m_bFirstBlend = true;

	m_bKeep = isKeep;
}