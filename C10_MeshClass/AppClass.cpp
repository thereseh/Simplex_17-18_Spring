#include "AppClass.h"
void Application::InitVariables(void)
{	// 46
	//Make MyMesh object

	for (int i = 0; i < 46; i++) {
		MyMesh* m_pMesh = new MyMesh();
		m_pMesh->GenerateCube(2.0f, C_BLACK);

		m_mesh.push_back(m_pMesh);
	}
	
	/*m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(2.0f, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);
	*/
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	static float fPos = 0.0f;

	m_m4Cube = glm::translate(vector3(fPos, 0.0f, 0.0f));

	for (int i = 0; i < 46; i++) {

		if (i == 0) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, 8.0f, -20.0f)) * m_m4Cube);
		if (i == 1) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f, 8.0f, -20.0f)) * m_m4Cube);
		if (i == 2) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 6.0f, -20.0f)) * m_m4Cube);
		if (i == 3) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 6.0f, -20.0f)) * m_m4Cube);
		if (i == 4) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, 4.0f, -20.0f)) * m_m4Cube);
		if (i == 5) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 4.0f, -20.0f)) * m_m4Cube);
		if (i == 6) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 4.0f, -20.0f)) * m_m4Cube);
		if (i == 7) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 4.0f, -20.0f)) * m_m4Cube);
		if (i == 8) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 4.0f, -20.0f)) * m_m4Cube);
		if (i == 9) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 4.0f, -20.0f)) * m_m4Cube);
		if (i == 10) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f, 4.0f, -20.0f)) * m_m4Cube);
		if (i == 11) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, 2.0f, -20.0f)) * m_m4Cube);
		if (i == 12) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-10.0f, 2.0f, -20.0f)) * m_m4Cube);
		if (i == 13) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 2.0f, -20.0f)) * m_m4Cube);
		if (i == 14) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 2.0f, -20.0f)) * m_m4Cube);
		if (i == 15) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 2.0f, -20.0f)) * m_m4Cube);
		if (i == 16) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f, 2.0f, -20.0f)) * m_m4Cube);
		if (i == 17) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(6.0f, 2.0f, -20.0f)) * m_m4Cube);
		if (i == 18) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-12.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 19) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-10.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 20) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 21) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 22) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 23) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 24) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 25) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 26) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 27) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(6.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 28) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(8.0f, 0.0f, -20.0f)) * m_m4Cube);
		if (i == 29) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-12.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 30) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 31) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 32) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 33) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 34) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 35) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 36) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 37) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(8.0f, -2.0f, -20.0f)) * m_m4Cube);
		if (i == 38) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(8.0f, -4.0f, -20.0f)) * m_m4Cube);
		if (i == 39) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-12.0f, -4.0f, -20.0f)) * m_m4Cube);
		if (i == 40) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-8.0f, -4.0f, -20.0f)) * m_m4Cube);
		if (i == 41) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f, -4.0f, -20.0f)) * m_m4Cube);
		if (i == 42) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-6.0f, -6.0f, -20.0f)) * m_m4Cube);
		if (i == 43) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, -6.0f, -20.0f)) * m_m4Cube);
		if (i == 44) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, -6.0f, -20.0f)) * m_m4Cube);
		if (i == 45) m_mesh[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, -6.0f, -20.0f)) * m_m4Cube);

	}

	fPos += 0.1f;

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}