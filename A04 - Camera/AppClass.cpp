#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Therese Henriksson - tah5279@rit.edu";

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//init the camera
	m_pCamera = new MyCamera();
	m_pCamera->SetPositionTargetAndUp(
			vector3(0.0f, 3.0f, 20.0f), //Where my eyes are
			vector3(0.0f, 3.0f, 19.0f), //where what I'm looking at is
			AXIS_Y);					//what is up

	//Get the singleton
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation(m_fCameraRotateSpeed);

	//Add objects to the Manager
	for (int j = -50; j < 50; j += 2)
	{
		for (int i = -50; i < 50; i += 2)
		{
			m_pMyMeshMngr->AddConeToRenderList(glm::translate(vector3(i, 0.0f, j)));
		}
	}
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	// My camera code:
	// Need to create a clock for movement of camera
	static uint uClock = m_pSystem->GenClock();
	float f_time = m_pSystem->GetDeltaTime(uClock);
	
	// if key is pressed to move camera, multiply with speed
	vector3 m_v3MoveCamera = m_v3MoveKeyPress * m_fMoveSpeed;

	// multiply rotation to the new move direction
	m_v3MoveCamera = m_qRotateCamera * m_v3MoveCamera;
	
	// calculate new position
	static vector3 m_v3NewCameraPosition = vector3();
	m_v3NewCameraPosition += m_v3MoveCamera * f_time;

	// use quat rotation to calculate where to look
	vector3 m_v3NewLookDirection = m_qRotateCamera * -AXIS_Z;

	// use new position plus new look direction to get target
	vector3 m_v3NewTargetToLookAt = m_v3NewCameraPosition + m_v3NewLookDirection;

	m_pCamera->ResetCamera();
	m_pCamera->SetPositionTargetAndUp(
		m_v3NewCameraPosition, //Where my eyes are
		m_v3NewTargetToLookAt, //where what I'm looking at is
		AXIS_Y);			   //what is up

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();

	//release the camera
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
