#include "ranking.h"

using namespace std;

CRanking::CRanking()
{
	m_Time = 0;
	Init();
}

CRanking::~CRanking()
{

}

HRESULT CRanking::Init(void)
{
	ifstream pFile("data\\Ranking.txt");
	string line = {};

	getline(pFile, line);

	istringstream iss(line);

	iss >> m_Time;

	pFile.close();

	return S_OK;
}

void CRanking::Uninit(void)
{

}

void CRanking::Update(void)
{

}

void CRanking::Draw(void)
{

}
