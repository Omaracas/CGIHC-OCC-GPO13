/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
bool rover = true;

float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float movCoche = 0.0f;
float movAvion = 0.0f;
float movCocheOffset;
float movAvionOffset;

Window mainWindow;
std::vector<MeshModel*> meshList;
std::vector<MeshModel*> meshListModel;
std::vector<Shader> shaderList;

std::vector<glm::vec3> puntosCircuito;
std::vector<GLfloat> distanciasCircuito;
GLfloat longitudCircuito = 0.0f;
unsigned int indicePista = 0;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

Texture pistaTexture;

Model Blackhawk_M;
Model Plane_M;
Model Rover_M;
Model POA_M;

	// Satélite
Model Satelite_M;
Model Antena_M;
Model Panel_M;

Model Ghost_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;
Material MaterialPista;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight* pointLightsHolocron = &pointLights[1];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLightsAvion[6];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	MeshModel* obj5 = new MeshModel();
	obj5->CreateMeshModel(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	MeshModel* obj6 = new MeshModel();
	obj6->CreateMeshModel(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	MeshModel* obj7 = new MeshModel();
	obj7->CreateMeshModel(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearPista()
{
	const GLfloat ANCHO_CIRCUITO = 10.0f;
	const GLfloat ALTURA_BAJA = 2.0f;
	const GLfloat ALTURA_ALTA = 14.0f;

	//Puntos de control para dibujar la pista como la referencia (vista superior).
	//Cada fila describe una curva de Bezier; las rectas tambien usan cuatro puntos.
	const glm::vec2 controlPista[][4] = {
		// Recta inferior con subida
		{ {154.0f,306.0f}, {240.0f,306.0f}, {320.0f,306.0f}, {399.0f,306.0f} },
		// Curva inferior derecha
		{ {399.0f,306.0f}, {428.0f,306.0f}, {445.0f,290.0f}, {445.0f,265.0f} },
		{ {445.0f,265.0f}, {445.0f,234.0f}, {435.0f,211.0f}, {399.0f,211.0f} },
		// Recta central hacia la izquierda
		{ {399.0f,211.0f}, {335.0f,211.0f}, {270.0f,211.0f}, {214.0f,211.0f} },
		// Curva interior de retorno
		{ {214.0f,211.0f}, {192.0f,211.0f}, {181.0f,213.0f}, {181.0f,183.0f} },
		{ {181.0f,183.0f}, {181.0f,163.0f}, {184.0f,150.0f}, {203.0f,146.0f} },
		// Recta diagonal interior
		{ {203.0f,146.0f}, {270.0f,125.0f}, {338.0f,103.0f}, {402.0f,84.0f} },
		// Curva superior derecha
		{ {402.0f,84.0f}, {420.0f,79.0f}, {430.0f,73.0f}, {429.0f,56.0f} },
		{ {429.0f,56.0f}, {428.0f,42.0f}, {419.0f,36.0f}, {396.0f,36.0f} },
		// Recta superior
		{ {396.0f,36.0f}, {303.0f,36.0f}, {212.0f,36.0f}, {120.0f,36.0f} },
		// Curva superior izquierda
		{ {120.0f,36.0f}, {72.0f,36.0f}, {49.0f,63.0f}, {49.0f,110.0f} },
		// Lateral izquierdo con bajada
		{ {49.0f,110.0f}, {49.0f,154.0f}, {49.0f,196.0f}, {49.0f,240.0f} },
		// Curva inferior izquierda
		{ {49.0f,240.0f}, {49.0f,285.0f}, {75.0f,306.0f}, {122.0f,306.0f} },
		// Recta hasta la salida
		{ {122.0f,306.0f}, {132.0f,306.0f}, {143.0f,306.0f}, {154.0f,306.0f} }
	};

	//Se calcula la misma linea central para la pista y el movimiento de la nave.
	puntosCircuito.clear();
	const unsigned int MUESTRAS_POR_TRAMO = 64;
	for (unsigned int tramo = 0; tramo < 14; tramo++)
	{
		for (unsigned int i = 0; i < MUESTRAS_POR_TRAMO; i++)
		{
			GLfloat u = (GLfloat)i / MUESTRAS_POR_TRAMO;
			GLfloat v = 1.0f - u;
			glm::vec2 punto = v * v * v * controlPista[tramo][0]
				+ 3.0f * v * v * u * controlPista[tramo][1]
				+ 3.0f * v * u * u * controlPista[tramo][2]
				+ u * u * u * controlPista[tramo][3];

			GLfloat altura = ALTURA_BAJA;
			if (tramo == 0) // Subida en la recta inferior
			{
				GLfloat t = glm::clamp((u - 0.15f) / 0.70f, 0.0f, 1.0f);
				t = t * t * (3.0f - 2.0f * t);
				altura = ALTURA_BAJA + (ALTURA_ALTA - ALTURA_BAJA) * t;
			}
			else if (tramo >= 1 && tramo <= 10) altura = ALTURA_ALTA;
			else if (tramo == 11) // Bajada en el lateral izquierdo
			{
				GLfloat t = glm::clamp((u - 0.15f) / 0.70f, 0.0f, 1.0f);
				t = t * t * (3.0f - 2.0f * t);
				altura = ALTURA_ALTA - (ALTURA_ALTA - ALTURA_BAJA) * t;
			}
			puntosCircuito.push_back(glm::vec3((punto.x - 240.0f) * 0.55f,
				altura, (punto.y - 170.0f) * 0.55f));
		}
	}

	//Distancia acumulada para recorrer todas las rectas y curvas sin saltos.
	distanciasCircuito.clear();
	distanciasCircuito.push_back(0.0f);
	for (unsigned int i = 0; i < puntosCircuito.size(); i++)
	{
		distanciasCircuito.push_back(distanciasCircuito.back() + glm::length(
			puntosCircuito[(i + 1) % puntosCircuito.size()] - puntosCircuito[i]));
	}
	longitudCircuito = distanciasCircuito.back();

	//Pista con los mismos puntos que sigue la nave
	std::vector<GLfloat> verticesCircuito;
	std::vector<unsigned int> indicesCircuito;
	for (unsigned int i = 0; i <= puntosCircuito.size(); i++)
	{
		unsigned int k = i % (unsigned int)puntosCircuito.size();
		glm::vec3 p = puntosCircuito[k];
		glm::vec3 tangente = glm::normalize(puntosCircuito[(k + 1) % puntosCircuito.size()] -
			puntosCircuito[(k + puntosCircuito.size() - 1) % puntosCircuito.size()]);
		glm::vec3 lateral = glm::normalize(glm::vec3(tangente.z, 0.0f, -tangente.x));
		glm::vec3 normal = glm::normalize(glm::cross(tangente, lateral));
		for (unsigned int j = 0; j < 2; j++)
		{
			glm::vec3 borde = p + lateral * (j == 0 ? ANCHO_CIRCUITO / 2.0f : -ANCHO_CIRCUITO / 2.0f);
			verticesCircuito.insert(verticesCircuito.end(), {
				borde.x, borde.y, borde.z,
				(GLfloat)j, (GLfloat)i / 8.0f,
				normal.x, normal.y, normal.z });
		}
		if (i < puntosCircuito.size())
		{
			unsigned int base = i * 2;
			indicesCircuito.insert(indicesCircuito.end(), { base, base + 1, base + 2,
				base + 1, base + 3, base + 2 });
		}
	}
	MeshModel* pista = new MeshModel();
	pista->CreateMeshModel(verticesCircuito.data(), indicesCircuito.data(),
		(unsigned int)verticesCircuito.size(), (unsigned int)indicesCircuito.size());
	indicePista = (unsigned int)meshList.size();
	meshList.push_back(pista);
}

glm::mat4 MoverNaveOmar(GLfloat& faseNave, GLfloat pasoAnimacion)
{
	//Recorrido de la nave sobre la linea central de la pista
	faseNave += 1.0f * pasoAnimacion;
	if (faseNave >= longitudCircuito) faseNave -= longitudCircuito;
	unsigned int indiceNave = 0;
	while (indiceNave + 2 < distanciasCircuito.size() &&
		faseNave >= distanciasCircuito[indiceNave + 1]) indiceNave++;
	GLfloat uNave = (faseNave - distanciasCircuito[indiceNave]) /
		(distanciasCircuito[indiceNave + 1] - distanciasCircuito[indiceNave]);
	glm::vec3 posicionNave = glm::mix(puntosCircuito[indiceNave],
		puntosCircuito[(indiceNave + 1) % puntosCircuito.size()], uNave);
	glm::vec3 avanceNave = glm::normalize(puntosCircuito[(indiceNave + 1) % puntosCircuito.size()] -
		puntosCircuito[(indiceNave + puntosCircuito.size() - 1) % puntosCircuito.size()]);
	glm::vec3 derechaNave = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), avanceNave));
	glm::vec3 arribaNave = glm::normalize(glm::cross(avanceNave, derechaNave));
	glm::mat4 orientacionNave(1.0f);
	orientacionNave[0] = glm::vec4(derechaNave, 0.0f);
	orientacionNave[1] = glm::vec4(arribaNave, 0.0f);
	orientacionNave[2] = glm::vec4(avanceNave, 0.0f);
	glm::mat4 matrizNave = glm::mat4(1.0f);
	matrizNave = glm::translate(matrizNave, posicionNave + glm::vec3(0.0f, 9.0f, 0.0f));
	matrizNave = matrizNave * orientacionNave;

	return matrizNave;
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearPista();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	pistaTexture = Texture("Textures/plain.png");
	pistaTexture.LoadTextureA();

	Plane_M = Model();
	Plane_M.LoadModel("Models/Plane.obj");

	Rover_M = Model();
	Rover_M.LoadModel("Models/rovermodificado.obj");

	POA_M = Model();
	POA_M.LoadModel("Models/POA.obj");

	Ghost_M = Model();
	Ghost_M.LoadModel("Models/Ghost.gltf");

	Satelite_M = Model();
	Satelite_M.LoadModel("Models/Satelite.obj");
	Antena_M = Model();
	Antena_M.LoadModel("Models/Antena.obj");
	Panel_M = Model();
	Panel_M.LoadModel("Models/Panel.gltf");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	MaterialPista = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Puntual Rover
	pointLights[0] = PointLight(
		0.0f, 1.0f, 0.0f,
		0.1f, 4.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.9f, 0.9999999999999);
	pointLightCount++;

	//// Puntual Holocron
	//pointLightsHolocron[0] = PointLight(
	//	0.0f, 1.0f, 1.0f,
	//	1.0f, 0.2f,
	//	0.0f, 0.0f, 0.0f,
	//	1.0, 0.2f, 0.2f);
	//pointLightCount++;

	unsigned int spotLightCount = 0;

	// Linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// luz frente al rover
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		3.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.02f,
		30.0f);
	spotLightCount++;

	//// luz frente al avion
	//spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
	//	1.0f, 8.0f,
	//	0.0f, 0.0f, 0.0f,
	//	-1.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.05f,
	//	10.0f);
	//spotLightCount++;
	// 
	// luz bajo al avion 1
	spotLightsAvion[0] = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.02f, 0.04f,
		30.0f);

	// luz bajo al avion 2
	spotLightsAvion[1] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.02f, 0.04f,
		30.0f);

	// luz bajo al avion 3
	spotLightsAvion[2] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.02f, 0.04f,
		30.0f);

	// luz bajo al avion 4
	spotLightsAvion[3] = SpotLight(0.0f, 1.0f, 1.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.02f, 0.04f,
		30.0f);

	// luz bajo al avion 5
	spotLightsAvion[4] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.02f, 0.04f,
		30.0f);

	// luz bajo al avion 6
	spotLightsAvion[5] = SpotLight(1.0f, 0.0f, 1.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.02f, 0.04f,
		30.0f);



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movCocheOffset = 1.0f;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	GLfloat faseNave = 0.0f;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.008f * deltaTime;

		if (movCoche > 295.0f) {
			rover = false;
		}
		else if (movCoche < -295.0f) {
			rover = true;
		}

		if (rover) {
			movCoche += movCocheOffset * deltaTime;
		}
		else {
			movCoche -= movCocheOffset * deltaTime;
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		/*if (mainWindow.getLuzRover() && mainWindow.getLuzHolocron()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (mainWindow.getLuzRover() && !mainWindow.getLuzHolocron()) {
			shaderList[0].SetPointLights(pointLights, 1);
		}
		else if (!mainWindow.getLuzRover() && mainWindow.getLuzHolocron()) {
			shaderList[0].SetPointLights(pointLightsHolocron, 1);
		}
		else if (!mainWindow.getLuzRover() && !mainWindow.getLuzHolocron()) {
			shaderList[0].SetPointLights(pointLights, 0);
		}

		if (mainWindow.getCambiarColor() == 0) {

			spotLights[3] = spotLightsAvion[0];
			shaderList[0].SetSpotLights(spotLights, spotLightCount + 1);
		}
		else if (mainWindow.getCambiarColor() == 1) {

			spotLights[3] = spotLightsAvion[1];
			shaderList[0].SetSpotLights(spotLights, spotLightCount + 1);
		}
		else if (mainWindow.getCambiarColor() == 2) {

			spotLights[3] = spotLightsAvion[2];
			shaderList[0].SetSpotLights(spotLights, spotLightCount + 1);
		}
		else if (mainWindow.getCambiarColor() == 3) {

			spotLights[3] = spotLightsAvion[3];
			shaderList[0].SetSpotLights(spotLights, spotLightCount + 1);
		}
		else if (mainWindow.getCambiarColor() == 4) {

			spotLights[3] = spotLightsAvion[4];
			shaderList[0].SetSpotLights(spotLights, spotLightCount + 1);
		}
		else if (mainWindow.getCambiarColor() == 5) {

			spotLights[3] = spotLightsAvion[5];
			shaderList[0].SetSpotLights(spotLights, spotLightCount + 1);
		}*/

	/*	if (mainWindow.getLuzRover()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else {
			shaderList[0].SetPointLights(pointLights, 0);

		}*/

		shaderList[0].SetPointLights(pointLights, 0);
		shaderList[0].SetSpotLights(spotLights, 1);

		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMeshModel();

		//Pista
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.9f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pistaTexture.UseTexture();
		MaterialPista.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[indicePista]->RenderMeshModel();
		model = MoverNaveOmar(faseNave, deltaTime);

		/*spotLights[1].SetPos(glm::vec3(0.9f, -1.3f, 0.0));
		pointLights[0].SetPos(glm::vec3(1.2f, 2.5f, 0.0));*/

		model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ghost_M.RenderModel();

		//// Avion
		//model = glm::mat4(1.0);
		//model = glm::rotate(model, angulovaria, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model,  glm::vec3(0.0f ,6.0f,-270.0));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Plane_M.RenderModel();

		/*spotLights[1].			SetPos(glm::vec3(-1.5f,6.0f,-270.0));
		spotLightsAvion[0].		SetPos(glm::vec3(0.0f ,6.0f,-270.0));
		spotLightsAvion[1].		SetPos(glm::vec3(0.0f ,6.0f,-270.0));
		spotLightsAvion[2].		SetPos(glm::vec3(0.0f ,6.0f,-270.0));
		spotLightsAvion[3].		SetPos(glm::vec3(0.0f ,6.0f,-270.0));
		spotLightsAvion[4].		SetPos(glm::vec3(0.0f ,6.0f,-270.0));
		spotLightsAvion[5].		SetPos(glm::vec3(0.0f ,6.0f,-270.0));*/

		//// Rover
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f +movCoche, -2.0f, -17.0));
		//spotLights[1].SetPos(glm::vec3(0.9f +movCoche, -1.3f, -17.0));
		//pointLights[0].SetPos(glm::vec3(1.2f +movCoche, 2.5f, -17.0));
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Rover_M.RenderModel();

		// --------------------------------------------------------------------------- //	Satélite

		// Cuerpo
		color = glm::vec3(0.8f, 0.8f, 0.8f);
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, angulovaria, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 45.0f, -70.0f));
		model = glm::rotate(model, mainWindow.getsatelite1() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, mainWindow.getsatelite2() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, mainWindow.getsatelite3() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Satelite_M.RenderModel();

		// Antena
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.4614f));
		model = glm::rotate(model, mainWindow.getsatelite4() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Antena_M.RenderModel();

		// Panel 1
		model = modelaux;
		color = glm::vec3(0.12f, 0.23f, 0.43f);
		model = glm::translate(model, glm::vec3(1.4545f, 0.092409f, 0.23857f));
		model = glm::rotate(model, mainWindow.getsatelite5() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Panel_M.RenderModel();

		// Panel 2
		model = modelaux;
		color = glm::vec3(0.12f, 0.23f, 0.43f);
		model = glm::translate(model, glm::vec3(-1.52f, 0.092409f, 0.23857f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, mainWindow.getsatelite6() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Panel_M.RenderModel();
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
