#include "stdafx.h"
#include "cMonster.h"
#include "cMonsterOwnedStates.h"

cMonster::cMonster()
	:m_pSkinnedMesh(nullptr)
	,m_pStateMachine(nullptr)
	, m_fSpeed(0.08f)
	,m_vDir(0.0f,0.0f,-1.0f)
	,m_fAtkRadius(150.0f)
{
}

cMonster::~cMonster()
{
}

void cMonster::Init(char* path, char* fileName)
{
	LoadModel(path, fileName);
	m_pStateMachine = new cStateMachine<cMonster>(this);
	m_pStateMachine->ChangeState(cState_Track::Get());
	m_pStateMachine->SetGlobalState(cGlobalState_Monster::Get());
}

void cMonster::Update( D3DXVECTOR3& vPos)
{
	SetPlayerPos(vPos);
	AIUpdate();
}

void cMonster::Destroy()
{
	SAFE_DELETE(m_pStateMachine);
}

void cMonster::Render()
{
	m_pSkinnedMesh->UpdateAndRender();
}

void cMonster::SetPos(const D3DXVECTOR3& vPos)
{
	m_vPos = vPos;
	m_pSkinnedMesh->SetPosition(vPos);
}

void cMonster::SetAnim(eMonsterAnim anim)
{
	m_pSkinnedMesh->SetAnimationIndex(anim);
}

void cMonster::SetPlayerPos(D3DXVECTOR3 & playerPos)
{
	m_vPlayerPos = playerPos;
}

void cMonster::KeepDistance()
{
	D3DXVECTOR3 vDir = -(m_vPlayerPos - m_vPos);
	D3DXVec3Normalize(&vDir, &vDir);
	
	D3DXVECTOR3 vMove = m_vPos + (vDir * m_fSpeed);

	m_pSkinnedMesh->AddRotY(RotY(vDir));
	SetPos(vMove);
}

void cMonster::Tracking()
{
	D3DXVECTOR3 vDir = m_vPlayerPos - m_vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	D3DXVECTOR3 vMove = m_vPos + (vDir * m_fSpeed);
	float fAngle = RotY(vDir);
	m_pSkinnedMesh->AddRotY(fAngle);
	SetPos(vMove);
}

float cMonster::RotY(D3DXVECTOR3 & vDir)
{
	float fDot = D3DXVec3Dot(&m_vDir, &vDir);
	float fAngle =0.0f;

	if (fDot > 1.0f)
		fAngle = 0.0f;
	else if (fDot < -1.0f)
	{
		fDot = -1.0f;
		fAngle = acosf(fDot);
	}
	else
		fAngle = acosf(fDot);


	D3DXVECTOR3 vCross;
	D3DXVec3Cross(&vCross, &m_vDir, &vDir);

	if (vCross.y < 0.0f)
		fAngle *= -1.0f;

	m_vDir = vDir;
	

	return fAngle;
}

void cMonster::LoadModel(char * path, char * fileName)
{
	m_pSkinnedMesh = new cSkinnedMesh(path, fileName);
}

void cMonster::ChangeToAttack()
{
	D3DXVECTOR3 vDir = m_vPlayerPos - m_vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pSkinnedMesh->AddRotY(RotY(vDir));

	m_pStateMachine->ChangeState(cState_Attack::Get());
}

void cMonster::ChangeToTrack()
{
	m_pStateMachine->ChangeState(cState_Track::Get());
}

void cMonster::ChangeToAvoid()
{
	m_pStateMachine->ChangeState(cState_Avoid::Get());
}

bool cMonster::SenseAtkPlayer()
{
	D3DXVECTOR3 vDist = m_vPlayerPos - m_vPos;
	float fDist =D3DXVec3LengthSq(&vDist);

	if (fDist < m_fAtkRadius)
		return true;
	else
		return false;
}

void cMonster::AIUpdate()
{
	m_pStateMachine->Update();
}
