#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;


	Release();
	Init();

	
	float f_curr_vertex = 0;
	float f_next_vertex = 0;
	float f_half_height = a_fHeight * 0.5f;

	for (int i = 0; i <= a_nSubdivisions; i++) {
		f_curr_vertex = (i / (float)a_nSubdivisions) * PI * 2;
		f_next_vertex = (i + 1) / (float)a_nSubdivisions * PI * 2;

		// Sides
		vector3 point1(cos(f_curr_vertex) * a_fRadius, -f_half_height, sin(f_curr_vertex) * a_fRadius);
		vector3 point2(cos(f_next_vertex) * a_fRadius, -f_half_height, sin(f_next_vertex) * a_fRadius);
		vector3 point3(0, f_half_height, 0);

		// Bottom
		vector3 point4(cos(f_curr_vertex) * a_fRadius, -f_half_height, sin(f_curr_vertex) * a_fRadius);
		vector3 point5(cos(f_next_vertex) * a_fRadius, -f_half_height, sin(f_next_vertex) * a_fRadius);
		vector3 point6(0, -f_half_height, 0);

		// Front
		AddTri(point1, point2, point3);

		// Back
		AddTri(point2, point1, point3);

		// Bottom
		AddTri(point4, point5, point6);
	}



	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();


	float f_curr_vertex = 0;
	float f_next_vertex = 0;
	float f_half_height = a_fHeight * 0.5f;

	for (int i = 0; i <= a_nSubdivisions; i++) {
		f_curr_vertex = (i / (float)a_nSubdivisions) * PI * 2;
		f_next_vertex = (i + 1) / (float)a_nSubdivisions * PI * 2;

		// Sides
		vector3 point0(cos(f_curr_vertex) * a_fRadius, -f_half_height, sin(f_curr_vertex) * a_fRadius);
		vector3 point1(cos(f_next_vertex) * a_fRadius, -f_half_height, sin(f_next_vertex) * a_fRadius);
		vector3 point2(cos(f_curr_vertex) * a_fRadius, f_half_height, sin(f_curr_vertex) * a_fRadius);
		vector3 point3(cos(f_next_vertex) * a_fRadius, f_half_height, sin(f_next_vertex) * a_fRadius);

		// Bottom
		vector3 point4(cos(f_curr_vertex) * a_fRadius, -f_half_height, sin(f_curr_vertex) * a_fRadius);
		vector3 point5(cos(f_next_vertex) * a_fRadius, -f_half_height, sin(f_next_vertex) * a_fRadius);
		vector3 point6(0, -f_half_height, 0);

		// Top
		vector3 point7(cos(f_curr_vertex) * a_fRadius, f_half_height, sin(f_curr_vertex) * a_fRadius);
		vector3 point8(cos(f_next_vertex) * a_fRadius, f_half_height, sin(f_next_vertex) * a_fRadius);
		vector3 point9(0, f_half_height, 0);


		// Front
		AddQuad(point0, point1, point3, point2);

		// Back
		AddQuad(point1, point0, point2, point3);

		// Bottom
		AddTri(point4, point5, point6);

		// Top
		AddTri(point8, point7, point9);

	}



	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float f_curr_vertex = 0;
	float f_next_vertex = 0;
	float f_half_height = a_fHeight * 0.5f;

	for (int i = 0; i < a_nSubdivisions; i++) {
		f_curr_vertex = (i / (float)a_nSubdivisions) * PI * 2;
		f_next_vertex = (i + 1) / (float)a_nSubdivisions * PI * 2;

		// Outer Sides
		vector3 v_outer_point0(cos(f_curr_vertex) * a_fOuterRadius, -f_half_height, sin(f_curr_vertex) * a_fOuterRadius);
		vector3 v_outer_point1(cos(f_next_vertex) * a_fOuterRadius, -f_half_height, sin(f_next_vertex) * a_fOuterRadius);
		vector3 v_outer_point2(cos(f_next_vertex) * a_fOuterRadius, f_half_height, sin(f_next_vertex) * a_fOuterRadius);
		vector3 v_outer_point3(cos(f_curr_vertex) * a_fOuterRadius, f_half_height, sin(f_curr_vertex) * a_fOuterRadius);


		// Inner Sides
		vector3 v_inner_point0(cos(f_curr_vertex) * a_fInnerRadius, -f_half_height, sin(f_curr_vertex) * a_fInnerRadius);
		vector3 v_inner_point1(cos(f_next_vertex) * a_fInnerRadius, -f_half_height, sin(f_next_vertex) * a_fInnerRadius);
		vector3 v_inner_point2(cos(f_next_vertex) * a_fInnerRadius, f_half_height, sin(f_next_vertex) * a_fInnerRadius);
		vector3 v_inner_point3(cos(f_curr_vertex) * a_fInnerRadius, f_half_height, sin(f_curr_vertex) * a_fInnerRadius);

		// Front
		AddQuad(v_outer_point0, v_outer_point1, v_outer_point3, v_outer_point2);
		AddQuad(v_inner_point0, v_inner_point1, v_inner_point3, v_inner_point2);

		// Back
		AddQuad(v_outer_point1, v_outer_point0, v_outer_point2, v_outer_point3);
		AddQuad(v_inner_point1, v_inner_point0, v_inner_point2, v_inner_point3);
		
		// Top
		AddQuad(v_outer_point2, v_outer_point3, v_inner_point2, v_inner_point3);
		AddQuad(v_outer_point3, v_outer_point2, v_inner_point3, v_inner_point2);

		// Bottom
		AddQuad(v_outer_point0, v_outer_point1, v_inner_point0, v_inner_point1);
		AddQuad(v_outer_point1, v_outer_point0, v_inner_point1, v_inner_point2);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();


	float f_curr_vertex = 0;
	float f_next_vertex = 0;
	float f_curr_radians = 0;
	float f_next_radians = 0;

	float f_center_tube = (a_fOuterRadius - a_fInnerRadius) * 0.5f;
	float f_outer_radius = a_fOuterRadius - f_center_tube;

	for (int i = 0; i < a_nSubdivisionsA; i++) {
		f_curr_radians = (i / (float)a_nSubdivisionsA) * PI * 2;
		f_next_radians = ((i + 1) / (float)a_nSubdivisionsA) * PI * 2;


		for (int j = 0; j < a_nSubdivisionsB; j++) {
			f_curr_vertex = (j / (float)a_nSubdivisionsB) * PI * 2;
			f_next_vertex = ((j + 1) / (float)a_nSubdivisionsB) * PI * 2;


			// curr radians + curr vertex (bottom left)
			vector3 point0((f_outer_radius + f_center_tube * cos(f_curr_vertex)) * cos(f_curr_radians),
				f_center_tube * sin(f_curr_vertex),
				(f_outer_radius + f_center_tube * cos(f_curr_vertex)) * sin(f_curr_radians));

			// next radians + curr vertex (bottom right)
			vector3 point1((f_outer_radius + f_center_tube * cos(f_curr_vertex)) * cos(f_next_radians),
				f_center_tube * sin(f_curr_vertex),
				(f_outer_radius + f_center_tube * cos(f_curr_vertex)) * sin(f_next_radians));

			// curr radians + next vertex (top left)
			vector3 point2((f_outer_radius + f_center_tube * cos(f_next_vertex)) * cos(f_curr_radians),
				f_center_tube * sin(f_next_vertex),
				(f_outer_radius + f_center_tube * cos(f_next_vertex)) * sin(f_curr_radians));

			// next radians + next vertex (top right)
			vector3 point3((f_outer_radius + f_center_tube * cos(f_next_vertex)) * cos(f_next_radians),
				f_center_tube * sin(f_next_vertex),
				(f_outer_radius + f_center_tube * cos(f_next_vertex)) * sin(f_next_radians));
			
			AddQuad(point0, point1, point3, point2);
			AddQuad(point1, point0, point2, point3);

		}

	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Multiply by 2 to make it more round, I'm a cheeta
	float f_half_circle = PI / (a_nSubdivisions * 2 );

	for (float i = 0; i <= 2 * PI; i += f_half_circle)
	{
		for (float j = 0; j <= PI; j += f_half_circle)
		{

			vector3 point0(a_fRadius * sin(j + f_half_circle) * sin(i),
				a_fRadius * cos(j + f_half_circle),
				a_fRadius * sin(j + f_half_circle) * cos(i));

			vector3 point1(a_fRadius * sin(j + f_half_circle) * sin(i + f_half_circle),
				a_fRadius * cos(j + f_half_circle),
				a_fRadius * sin(j + f_half_circle) * cos(i + f_half_circle));

			vector3 point2(a_fRadius * sin(j) * sin(i + f_half_circle),
				a_fRadius * cos(j),
				a_fRadius * sin(j) * cos(i + f_half_circle));

			vector3 point3(a_fRadius * sin(j) * sin(i),
				a_fRadius * cos(j),
				a_fRadius * sin(j) * cos(i));

			AddQuad(point0, point1, point3, point2);
			AddQuad(point1, point2, point2, point3);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}