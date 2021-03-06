#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cRay.h"
#include "cPlayer.h"
#include "cMonsterManager.h"
#include "cMapManager.h"

cMainGame::cMainGame(void)
	: m_pCamera(NULL)
	, m_pSkinnedMesh(NULL)
	,m_pPlayer(NULL)
	,m_pMonsterManager(NULL)
	,m_pMapManager(NULL)
{
	//예제코드
	//m_vec3Pivot1 = D3DXVECTOR3(15.0f, 0.0f, 2.0f);
	//m_fDistPerSec1 = 5.0f;
	//m_vec3Direction1 = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
	//m_pSphereCollider1 = new cSphereCollider(5.0f, &m_vec3Pivot1, &m_fDistPerSec1, &m_vec3Direction1);
	//
	//m_vec3Pivot2 = D3DXVECTOR3(-15.0f, 0.0f, -2.0f);
	//m_fDistPerSec2 = 5.0f;
	//m_vec3Direction2 = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
	//m_pSphereCollider2 = new cSphereCollider(5.0f, &m_vec3Pivot2, &m_fDistPerSec2, &m_vec3Direction2);
	//예제코드
}

cMainGame::~cMainGame(void)
{
	//예제코드
	//delete m_pSphereCollider2;
	//delete m_pSphereCollider1;
	//예제코드
	SAFE_DELETE(m_pCamera);
	m_pPlayer->Destroy();
	SAFE_RELEASE(m_pPlayer);
	SAFE_DESTROY(m_pMonsterManager);
	SAFE_DESTROY(m_pMapManager);
	g_pSkillManager->Destroy();
	g_pColliderManager->Destroy();
	g_pTextureManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pObjectManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	m_pCamera = new cCamera;
	m_pCamera->Initialize();

	m_pPlayer = new cPlayer;
	m_pPlayer->Init();

	g_pSkillManager->Init();

	m_pMonsterManager = new cMonsterManager;
	m_pMonsterManager->Init();

	m_pMapManager = new cMapManager;
	m_pMapManager->Init();

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	
	g_pColliderManager->Update();

	if (m_pMapManager)
		m_pMapManager->Update();


	if (m_pPlayer)
		m_pPlayer->Update();

	if (m_pMonsterManager)
		m_pMonsterManager->Update(m_pPlayer->GetPos());

	g_pSkillManager->Update();

	if (m_pCamera)
		m_pCamera->Update(&m_pPlayer->GetPos());

}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(230, 121, 112),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	// 그림을 그린다.
	if (GetKeyState(VK_F1) & 0x0001)
	{
		g_pColliderManager->Render();
	}
	m_pMapManager->Render(m_pCamera->GetFrustum());
	
	m_pPlayer->Render();
	
	if(m_pMonsterManager)
		m_pMonsterManager->Render();

	g_pSkillManager->Render();
	g_pColliderManager->Render();
	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pCamera)
		m_pCamera->WindowInputProc(message, wParam, lParam);

	switch (message)
	{
		case WM_RBUTTONDOWN:
				cRay ray = cRay::GetRayVector(LOWORD(lParam), HIWORD(lParam));
				float u, v, dist;
				for (int i = 0; i < 6; i+=3)
				{
					if (D3DXIntersectTri(
						&m_pMapManager->GetTerrain()->GetPickingGround()[i].p,
						&m_pMapManager->GetTerrain()->GetPickingGround()[i + 1].p,
						&m_pMapManager->GetTerrain()->GetPickingGround()[i + 2].p,
						ray.GetOrigin(),
						ray.GetDirection(),
						&u, &v, &dist))
					{
						D3DXVECTOR3 origin = *ray.GetOrigin();
						D3DXVECTOR3 dir = *ray.GetDirection();
						D3DXVECTOR3 pickedPosition = *ray.GetOrigin() + ((*ray.GetDirection()) * dist);
						pickedPosition.y = 0.0f;
						m_pPlayer->SetDestination(pickedPosition);
						break;
					}
				}
			break;
	}

	
}


