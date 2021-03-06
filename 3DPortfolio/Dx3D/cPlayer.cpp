#include "stdafx.h"
#include "cPlayer.h"

#include "cMonsterManager.h"

cPlayer::cPlayer()
	:m_pCharacterController(nullptr)
{
}


cPlayer::~cPlayer()
{

}

void cPlayer::Init()
{
	m_pCharacterController = new CharacterController;
	m_pCharacterController->Init();
}

void cPlayer::Update()
{
	m_pCharacterController->Update();
}

void cPlayer::Destroy()
{
	m_pCharacterController->Destroy();
}

void cPlayer::Render()
{
	m_pCharacterController->Render();
}

void cPlayer::SetDestination(D3DXVECTOR3 & Destination)
{
	m_pCharacterController->StartRunning(Destination);
}

