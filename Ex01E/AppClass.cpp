#include "AppClass.h"
using namespace Simplex;
void Application::Display(void)
{
#pragma region DOES NOT NEED CHANGES
	// Clear the screen
	ClearScreen();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer
#pragma endregion
	//Your code goes here ---------------------------------------------------------
	
	m_sProgrammer = "Therese Henriksson - tah5279@rit.edu"; //Replace with your name and email

	static uint route = 0;
	static float f_percentage = 0;
	float f_totalTimeSec = 2.0f;

	vector3 v3Begin = m_v3StopList[route % m_v3StopList.size()];
	vector3 v3End = m_v3StopList[(route + 1) % m_v3StopList.size()];

	// get percentage traveled, so we can maintain same time traveled between all stops
	f_percentage = MapValue(fTimer, 0.0f, f_totalTimeSec, 0.0f, 1.0f);

	// get current position by lerping between two postions
	vector3 v3CurrentPos = glm::lerp(v3Begin, v3End, f_percentage); 

	if (f_percentage >= 1.0f) {

		// restart timer
		fTimer = m_pSystem->GetDeltaTime(uClock);

		route++;
	}

	matrix4 m4Model = glm::translate(IDENTITY_M4, v3CurrentPos) * ToMatrix4(m_qArcBall);


	
	
	//---------------------------------------------------------
#pragma region DOES NOT NEED CHANGES
	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix

	// render the object
	m_pMesh->Render(m4Projection, m4View, m4Model);

	// draw stops to know we are within stops
	for (uint i = 0; i < m_v3StopList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_v3StopList[i]) * glm::scale(vector3(0.10f)), C_RED);
	}
		
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
#pragma endregion
}
#pragma region DOES NOT NEED CHANGES
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 15.0f, ZERO_V3, AXIS_Y);

	//initializing the model
	m_pMesh = new Simplex::MyMesh();

	//Generate a primitive
	m_pMesh->GenerateCone(1.5f, 2.0f, 15, C_GREEN);

	//Check if stops is empty if it is add some points to it
	if (m_v3StopList.size() == 0)
	{
		m_v3StopList.push_back(vector3(-2.5f,-2.5f,-2.5f));
		m_v3StopList.push_back(vector3( 2.5f,-2.5f,-2.5f));
		m_v3StopList.push_back(vector3( 2.5f, 2.5f, 2.5f));
		m_v3StopList.push_back(vector3(-2.5f, 2.5f, 2.5f));
	}
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
void Application::Release(void)
{
	//release model
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}
#pragma endregion