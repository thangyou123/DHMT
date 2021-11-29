#include <stdlib.h>
#include "glut.h"
#include "glu.h"
#include "GL.H"
#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

#define PI			3.1415926
#define	COLORNUM		15
#define DEGRADIAN	(PI / 180.0)


float	ColorArr[COLORNUM][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},{1.0, 1.0, 0.0},{255.0 / 255.0,128.0 / 255.0,128.0 / 255.0} ,{255.0 / 255.0,0,128.0 / 255.0} };
int		screenWidth = 1000;
int		screenHeight = 500;
int		nChoice = 1;
int* arr = new int[2000];
float t = 1;
float m = 0;
float camera_dis = 5;
float camera_height = 1;
float camera[3];
float cameraState[7];
bool lightState = true;
GLfloat camera_angle =330;
GLfloat	fHalfSize = 15;
GLfloat fConstFS = fHalfSize;

class Point3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	void set(Point3& p)
	{
		x = p.x; y = p.y; z = p.z;
	}
	string printPoint3()
	{
		string res = to_string(x) + " ; " + to_string(y) + " ; " + to_string(z);
		return res;
	}
	Point3() { x = y = z = 0; }
	Point3(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}

};
class Color3
{
public:
	float r, g, b;
	void set(float red, float green, float blue)
	{
		r = red; g = green; b = blue;
	}
	void set(Color3& c)
	{
		r = c.r; g = c.g; b = c.b;
	}
	Color3() { r = g = b = 0; }
	Color3(float red, float green, float blue)
	{
		r = red; g = green; b = blue;
	}

};
class Vector3
{
public:
	float	x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	void set(Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	void flip()
	{
		x = -x; y = -y; z = -z;
	}
	void normalize();
	Vector3() { x = y = z = 0; }
	Vector3(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	Vector3(Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}

};

void Vector3::normalize()
{
	float temp = sqrt(x * x + y * y + z * z);
	x = x / temp;
	y = y / temp;
	z = z / temp;
}

class VertexID
{
public:
	int		vertIndex; //index of this vertex in the vertex list
	int		normIndex; // index of this vertex's normal
	int		colorIndex; // index of this vertex's color
};

class Face
{
public:
	int	nVerts;
	VertexID* vert;
	// danh sách các đỉnh + vector pháp tuyến của đỉnh
	Vector3 vtNewell;
	Face()
	{
		nVerts = 0;
		vert = NULL;
	}
	~Face()
	{
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	int	numVerts; // số lượng đỉnh
	Point3* pt; // danh sách các đỉnh

	int	numFaces; // số lượng mặt 
	Face* face; // danh sách các mặt

public:
	Mesh()
	{
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}
		if (face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe();
	void Draw(int chooseColor);
	void DrawColor();
	void SetColor(int colorIdx);
	void addMaterial(float ambient[], float diffuse[], float specular[], float shiness);
	void CreateCube(float t);
	void CreateCube2(float t);
	void CreateCube3(float t);
	void CreateCube4(float t);
	void CreateCube5(float t);
	void CreateCube6(float t);
	void CreateHinhTru(int nso_lopchiament, float beday, float bankinh);
	void CreateRectangular(float chieu_dai, float chieu_rong, float beday);
	void CreateNuaVongTron(float t, float fx, float beday);
	void CreateNuaVongTron2(float t, float fx, float beday);
	void CreateNuaVongTron3(float t, float fx, float beday);
	void CreateNuaVongTronThanGiua(float t, float fx, float beday, float h);
	void CreateNuaVongTronThanGiua2(float t, float fx, float beday, float h);
	void CreateNuavongtron1_1(float t, float fx, float beday);
	void CreateNuavongtron1_2(float t, float fx, float beday);
	void CreateNuavongtron1_3(float t, float fx, float beday);


	void vectorNewell();

};

void  Mesh::CreateNuaVongTronThanGiua(float t, float fx, float beday, float h) {
	int so_lopchia = 32;
	numVerts = so_lopchia * 4 + 2;
	pt = new Point3[numVerts];

	float bankinh = t;
	int		i;
	int		idx;
	float	do_quay = (PI) / (so_lopchia - 1.0);
	float	x, y, z, x2, z2;

	pt[0].set(0, beday / 2, 0);
	pt[2 * so_lopchia].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x2 = bankinh * -sin(do_quay * i) * h;
		z2 = bankinh * -cos(do_quay * i) * h;

		x = (bankinh + fx) * -sin(do_quay * i) * h;
		z = (bankinh + fx) * -cos(do_quay * i) * h;
		y = beday / 2;

		pt[i + 1].set(x, y, z);
		pt[i + 1 + so_lopchia + so_lopchia].set(x2, y, z2);

		y = -beday / 2;
		pt[i + 1 + so_lopchia].set(x, y, z);
		pt[i + 1 + so_lopchia + so_lopchia + so_lopchia].set(x2, y, z2);

	}

	pt[numVerts - 1].set(0, -beday / 2, 0);

	numFaces = so_lopchia * 4;
	face = new Face[numFaces];

	idx = 0;

	for (i = 0; i < so_lopchia - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 2 + so_lopchia * 2;
		face[idx].vert[1].vertIndex = i + 1 + so_lopchia * 2;

		face[idx].vert[2].vertIndex = i + 1;
		face[idx].vert[3].vertIndex = i + 2;
		idx++;
	}


	for (i = so_lopchia; i < 2 * so_lopchia - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 + so_lopchia * 2;
		face[idx].vert[1].vertIndex = i + 2 + so_lopchia * 2;

		face[idx].vert[2].vertIndex = i + 2;
		face[idx].vert[3].vertIndex = i + 1;
		idx++;
	}

	
	for (i = 0; i < so_lopchia - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 2;

		face[idx].vert[1].vertIndex = i + 1;

		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + so_lopchia;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + so_lopchia ;

		idx++;
	}

	for (i = 2 * so_lopchia; i < 3 * so_lopchia - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 2;




		face[idx].vert[1].vertIndex = i + 1;

		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + so_lopchia;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + so_lopchia;

		idx++;
	}







}
void  Mesh::CreateNuaVongTronThanGiua2(float t, float fx, float beday, float h) {
	int so_lopchia = 32;
	numVerts = so_lopchia * 4 + 2;
	pt = new Point3[numVerts];

	float bankinh = t;
	int		i;
	int		idx;
	float	do_quay = (PI) / (so_lopchia - 1.0);
	float	x, y, z, x2, z2;

	pt[0].set(0, beday / 2, 0);
	pt[2 * so_lopchia].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x2 = bankinh * -sin(do_quay * i) * h;
		z2 = bankinh * -cos(do_quay * i) * h;

		x = (bankinh)*sin(do_quay * i) * h + fx;
		z = (bankinh)*cos(do_quay * i) * h;
		y = beday / 2;

		pt[i + 1].set(x, y, z);
		pt[i + 1 + so_lopchia + so_lopchia].set(x2, y, z2);

		y = -beday / 2;
		pt[i + 1 + so_lopchia].set(x, y, z);
		pt[i + 1 + so_lopchia + so_lopchia + so_lopchia].set(x2, y, z2);

	}

	pt[numVerts - 1].set(0, -beday / 2, 0);

	numFaces = so_lopchia * 4;
	face = new Face[numFaces];

	idx = 0;

	for (i = 0; i < so_lopchia - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 + so_lopchia * 2;
		face[idx].vert[1].vertIndex = (so_lopchia - i - 2) + 2;

		face[idx].vert[2].vertIndex = (so_lopchia - i - 2) + 1;
		face[idx].vert[3].vertIndex = i + 2 + so_lopchia * 2;;
		idx++;
	}


	for (i = so_lopchia; i < 2 * so_lopchia - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 + so_lopchia * 2;
		face[idx].vert[1].vertIndex = i + 2 + so_lopchia * 2;

		face[idx].vert[2].vertIndex = (3 * so_lopchia - i - 2) + 1;
		face[idx].vert[3].vertIndex = (3 * so_lopchia - i - 2) + 2;
		idx++;
	}







}

void  Mesh::CreateNuaVongTron3(float t, float fx, float beday) {
	int so_lopchia = 32;
	numVerts = so_lopchia * 2 + 2;
	pt = new Point3[numVerts];
	float bankinh = t;
	int		i;
	int		idx;
	float	do_quay = (PI - 26.0 * PI / 180) / (so_lopchia - 1);
	float	x, y, z;

	pt[0].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x = bankinh * sin(do_quay * i + 13.0 * PI / 180);
		z = bankinh * cos(do_quay * i + 13.0 * PI / 180);
		float dx = x * cos(-60 * PI / 180) - z * sin(-60 * PI / 180);
		float dz = x * sin(-60 * PI / 180) + z * cos(-60 * PI / 180);
		x = dx + 4.95 * cos(-60 * PI / 180);
		z = dz + 4.95 * sin(-60 * PI / 180);
		y = beday / 2;
		if (i == 0 || i == so_lopchia - 1) {
			x = 4.95 * cos(-60 * PI / 180);
			z = 4.95 * sin(-60 * PI / 180);
		}
		pt[i + 1].set(x, y, z);

		y = -beday / 2;
		pt[i + 1 + so_lopchia].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -beday / 2, 0);

	numFaces = so_lopchia * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 1;
		if (i < so_lopchia - 1)
			face[idx].vert[1].vertIndex = i + 1;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 2;
		idx++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 2;
		if (i < so_lopchia - 1)
			face[idx].vert[1].vertIndex = i + 1;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + so_lopchia;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + so_lopchia;

		idx++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = so_lopchia + 1;
		if (i < so_lopchia - 1)
			face[idx].vert[2].vertIndex = i + 1 + so_lopchia;
		else
			face[idx].vert[2].vertIndex = 1 + so_lopchia;
		face[idx].vert[1].vertIndex = i + 2 + so_lopchia;
		idx++;
	}





}
void  Mesh::CreateNuaVongTron2(float t, float fx, float beday) {
	int so_lopchia = 32;
	numVerts = so_lopchia * 2 + 2;
	pt = new Point3[numVerts];
	float bankinh = t;
	int		i;
	int		idx;
	float	do_quay = (PI - 26.0 * PI / 180) / (so_lopchia - 1);
	float	x, y, z;

	pt[0].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x = bankinh * sin(do_quay * i + 13.0 * PI / 180);
		z = bankinh * cos(do_quay * i + 13.0 * PI / 180);
		float dx = x * cos(60 * PI / 180) - z * sin(60 * PI / 180);
		float dz = x * sin(60 * PI / 180) + z * cos(60 * PI / 180);
		x = dx + 4.95 * cos(60 * PI / 180);
		z = dz + 4.95 * sin(60 * PI / 180);
		y = beday / 2;
		if (i == 0 || i == so_lopchia - 1) {
			x = 4.95 * cos(60 * PI / 180);
			z = 4.95 * sin(60 * PI / 180);
		}
		pt[i + 1].set(x, y, z);

		y = -beday / 2;
		pt[i + 1 + so_lopchia].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -beday / 2, 0);

	numFaces = so_lopchia * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 1;
		if (i < so_lopchia - 1)
			face[idx].vert[1].vertIndex = i + 1;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 2;
		idx++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 2;
		if (i < so_lopchia - 1)
			face[idx].vert[1].vertIndex = i + 1;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + so_lopchia;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + so_lopchia;

		idx++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = so_lopchia + 1;
		if (i < so_lopchia - 1)
			face[idx].vert[2].vertIndex = i + 1 + so_lopchia;
		else
			face[idx].vert[2].vertIndex = 1 + so_lopchia;
		face[idx].vert[1].vertIndex = i + 2 + so_lopchia;
		idx++;
	}






}
void  Mesh::CreateNuaVongTron(float t, float fx, float beday) {
	int so_lopchia = 32;
	numVerts = so_lopchia * 2 + 2;
	pt = new Point3[numVerts];
	float bankinh = t;
	int		i;
	int		idx;
	float	do_quay = (PI - 26.0 * PI / 180) / (so_lopchia - 1.0);
	float	x, y, z;

	pt[0].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x = bankinh * -sin(do_quay * i + 13.0 * PI / 180) - 4.95;
		z = bankinh * -cos(do_quay * i + 13.0 * PI / 180);
		y = beday / 2;
		if (i == 0 || i == so_lopchia - 1) {
			x = -4.95;
			z = 0;
		}
		pt[i + 1].set(x, y, z);

		y = -beday / 2;
		pt[i + 1 + so_lopchia].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -beday / 2, 0);

	numFaces = so_lopchia * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 1;
		if (i < so_lopchia - 1)

			face[idx].vert[1].vertIndex = i + 1;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 2;
		idx++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 2;
		if (i < so_lopchia - 1)
			face[idx].vert[1].vertIndex = i + 1;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + so_lopchia;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + so_lopchia;

		idx++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = so_lopchia + 1;
		if (i < so_lopchia - 1)
			face[idx].vert[2].vertIndex = i + 1 + so_lopchia;
		else
			face[idx].vert[2].vertIndex = 1 + so_lopchia;
		face[idx].vert[1].vertIndex = i + 2 + so_lopchia;
		idx++;
	}






}

void Mesh::CreateHinhTru(int nso_lopchiament, float beday, float bankinh)
{
	numVerts = nso_lopchiament * 2 + 2;
	pt = new Point3[numVerts];

	int		i;
	int		idx;
	float	do_quay = 2 * PI / (nso_lopchiament - 1);
	float	x, y, z;

	pt[0].set(0, beday / 2, 0);
	for (i = 0; i < nso_lopchiament; i++)
	{
		x = bankinh * cos(do_quay * i);
		z = bankinh * sin(do_quay * i);
		y = beday / 2;
		pt[i + 1].set(x, y, z);

		y = -beday / 2;
		pt[i + 1 + nso_lopchiament].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -beday / 2, 0);

	numFaces = nso_lopchiament * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < nso_lopchiament; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0;
		if (i < nso_lopchiament - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 1;
		idx++;
	}

	for (i = 0; i < nso_lopchiament; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;
		if (i < nso_lopchiament - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nso_lopchiament;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nso_lopchiament;

		idx++;
	}

	for (i = 0; i < nso_lopchiament; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = numVerts - 1;
		if (i < nso_lopchiament - 1)
			face[idx].vert[2].vertIndex = i + 2 + nso_lopchiament;
		else
			face[idx].vert[2].vertIndex = 1 + nso_lopchiament;
		face[idx].vert[1].vertIndex = i + 1 + nso_lopchiament;
		idx++;
	}

}



void Mesh::CreateCube(float t)
{
	float bankinh = t;

	float x = bankinh * sin(0 + 15.0 * PI / 180);

	float z = bankinh * cos(0 + 15.0 * PI / 180);
	float dx = x * cos(60 * PI / 180) - z * sin(60 * PI / 180);
	float dz = x * sin(60 * PI / 180) + z * cos(60 * PI / 180);
	x = 5 * cos(60 * PI / 180);
	z = 5 * sin(60 * PI / 180);

	float x2 = bankinh * sin(PI - 15.0 * PI / 180);

	float z2 = bankinh * cos(PI - 15.0 * PI / 180);

	float dx2 = x2 * cos(60 * PI / 180) - z2 * sin(60 * PI / 180);
	float dz2 = x2 * sin(60 * PI / 180) + z2 * cos(60 * PI / 180);
	x2 = dx2 + 5 * cos(60 * PI / 180);
	z2 = dz2 + 5 * sin(60 * PI / 180);


	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-0.5, x2, z2);
	pt[1].set(0.5, x2, z2);
	pt[2].set(0.5, 2, 0);
	pt[3].set(-0.5, 2, 0);
	pt[4].set(-0.5, x, z);
	pt[5].set(0.5, x, z);
	pt[6].set(0.5, 0, 0);
	pt[7].set(-0.5, 0, 0);


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

}

void Mesh::CreateCube2(float t)
{
	float bankinh = t;

	float x = bankinh * sin(15.0 * PI / 180);

	float z = bankinh * cos(15.0 * PI / 180);
	float dx = x * cos(60 * PI / 180) - z * sin(60 * PI / 180);
	float dz = x * sin(60 * PI / 180) + z * cos(60 * PI / 180);
	x = dx + 5 * cos(60 * PI / 180);
	z = -dx + 5 * sin(60 * PI / 180);


	float	x2 = 5 * cos(60 * PI / 180);
	float z2 = 5 * sin(60 * PI / 180);


	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-0.5, x2, z2);
	pt[1].set(0.5, x2, z2);
	pt[2].set(0.5, 0, 0);
	pt[3].set(-0.5, 0, 0);
	pt[4].set(-0.5, x, z);
	pt[5].set(0.5, x, z);
	pt[6].set(0.5, -1, sqrt(3));
	pt[7].set(-0.5, -1, sqrt(3));


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

}

void Mesh::CreateCube3(float t)
{
	float bankinh = t;


	float x = 5 * cos(-60 * PI / 180);
	float z = 5 * sin(-60 * PI / 180);



	float x2 = bankinh * sin(0 + 15.0 * PI / 180);

	float z2 = bankinh * cos(0 + 15.0 * PI / 180);

	float dx2 = x2 * cos(-60 * PI / 180) - z2 * sin(-60 * PI / 180);
	float dz2 = x2 * sin(-60 * PI / 180) + z2 * cos(-60 * PI / 180);

	x2 = dx2 + 5 * cos(-60 * PI / 180);
	z2 = dz2 + 5 * sin(-60 * PI / 180);

	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-0.5, x, z);
	pt[1].set(0.5, x, z);
	pt[2].set(0.5, 0, 0);
	pt[3].set(-0.5, 0, 0);
	pt[4].set(-0.5, x2, z2);
	pt[5].set(0.5, x2, z2);
	pt[6].set(0.5, 2, 0);
	pt[7].set(-0.5, 2, 0);


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

}

void Mesh::CreateCube4(float t)
{
	float bankinh = t;

	float x = bankinh * sin(165.0 * PI / 180);

	float z = bankinh * cos(165.0 * PI / 180);
	float dx = x * cos(-60 * PI / 180) - z * sin(-60 * PI / 180);
	float dz = x * sin(-60 * PI / 180) + z * cos(-60 * PI / 180);
	x = dx + 5 * cos(-60 * PI / 180);
	z = dx + 5 * sin(-60 * PI / 180);


	float	x2 = 5 * cos(-60 * PI / 180);
	float z2 = 5 * sin(-60 * PI / 180);


	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-0.5, x2, z2);
	pt[1].set(0.5, x2, z2);
	pt[2].set(0.5, 0, 0);
	pt[3].set(-0.5, 0, 0);
	pt[4].set(-0.5, x, z);
	pt[5].set(0.5, x, z);
	pt[6].set(0.5, -1, -sqrt(3));
	pt[7].set(-0.5, -1, -sqrt(3));


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 2;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 5;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 4;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 3;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 4;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 6;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

}
void Mesh::CreateCube5(float t)
{
	float bankinh = t;

	float x = bankinh * -sin(15.0 * PI / 180) - 5.0;
	float z = bankinh * -cos(15.0 * PI / 180);


	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-0.5, -5.0, 0);
	pt[1].set(0.5, -5.0, 0);
	pt[2].set(0.5, 0, 0);
	pt[3].set(-0.5, 0, 0);
	pt[4].set(-0.5, x, z);
	pt[5].set(0.5, x, z);
	pt[6].set(0.5, -1, -sqrt(3));
	pt[7].set(-0.5, -1, -sqrt(3));


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

}
void Mesh::CreateCube6(float t)
{
	float bankinh = t;

	float x = bankinh * -sin(165.0 * PI / 180) - 5.0;
	float z = bankinh * -cos(165.0 * PI / 180);


	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-0.5, -5.0, 0);
	pt[1].set(0.5, -5.0, 0);
	pt[2].set(0.5, 0, 0);
	pt[3].set(-0.5, 0, 0);
	pt[4].set(-0.5, x, z);
	pt[5].set(0.5, x, z);
	pt[6].set(0.5, -1, sqrt(3));
	pt[7].set(-0.5, -1, sqrt(3));


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 2;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 5;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 4;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 3;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 4;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 6;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;


}
void Mesh::CreateRectangular(float chieu_dai, float chieu_rong, float beday)
{
	numVerts = 8;
	numFaces = 6;
	pt = new Point3[numVerts];
	face = new Face[numFaces];
	int idx = 0;
	int indexColor = 1;
	{
		pt[0].set(0, 0, 0); //A
		pt[1].set(0, 0, chieu_dai); //B
		pt[2].set(chieu_rong, 0, chieu_dai); //C
		pt[3].set(chieu_rong, 0, 0); //D
		pt[4].set(0, beday, 0); //A'
		pt[5].set(0, beday, chieu_dai); //B
		pt[6].set(chieu_rong, beday, chieu_dai); //C
		pt[7].set(chieu_rong, beday, 0); //D

		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0;
		face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = 2;
		face[idx].vert[3].vertIndex = 3;
		for (int i = 0; i < face[idx].nVerts; i++)
			face[idx].vert[i].colorIndex = indexColor;
		idx++;

		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 4;
		face[idx].vert[1].vertIndex = 5;
		face[idx].vert[2].vertIndex = 6;
		face[idx].vert[3].vertIndex = 7;
		for (int i = 0; i < face[idx].nVerts; i++)
			face[idx].vert[i].colorIndex = indexColor;
		idx++;

		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0;
		face[idx].vert[1].vertIndex = 3;
		face[idx].vert[2].vertIndex = 7;
		face[idx].vert[3].vertIndex = 4;
		for (int i = 0; i < face[idx].nVerts; i++)
			face[idx].vert[i].colorIndex = indexColor;
		idx++;

		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 1;
		face[idx].vert[1].vertIndex = 2;
		face[idx].vert[2].vertIndex = 6;
		face[idx].vert[3].vertIndex = 5;
		for (int i = 0; i < face[idx].nVerts; i++)
			face[idx].vert[i].colorIndex = indexColor;
		idx++;

		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 2;
		face[idx].vert[1].vertIndex = 3;
		face[idx].vert[2].vertIndex = 7;
		face[idx].vert[3].vertIndex = 6;
		for (int i = 0; i < face[idx].nVerts; i++)
			face[idx].vert[i].colorIndex = indexColor;
		idx++;

		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0;
		face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = 5;
		face[idx].vert[3].vertIndex = 4;
		for (int i = 0; i < face[idx].nVerts; i++)
			face[idx].vert[i].colorIndex = indexColor;
		idx++;
	}
}

void Mesh::CreateNuavongtron1_3(float t, float fx, float beday) {
	CreateNuaVongTron3(t, fx, beday);

}
void Mesh::CreateNuavongtron1_1(float t, float fx, float beday) {
	CreateNuaVongTron(t, fx, beday);

}
void Mesh::CreateNuavongtron1_2(float t, float fx, float beday) {


	CreateNuaVongTron2(t, fx, beday);

}

void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int ic = face[f].vert[v].colorIndex;
			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}
void Mesh::DrawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			int ic = face[f].vert[v].colorIndex;
			
			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glNormal3f(face[f].vtNewell.x, face[f].vtNewell.y, face[f].vtNewell.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		
		
		}
		glEnd();
	}
}
void Mesh::addMaterial(float ambient[], float diffuse[], float specular[], float shiness)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiness);
}
void Mesh::vectorNewell()
{
	for (int f = 0; f < numFaces; f++)
	{
		float vectorN[] = { 0,0,0 };
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			int next = face[f].vert[(v + 1) % face[f].nVerts].vertIndex;
			vectorN[0] += (pt[iv].y - pt[next].y) * (pt[iv].z + pt[next].z);
			vectorN[1] += (pt[iv].z - pt[next].z) * (pt[iv].x + pt[next].x);
			vectorN[2] += (pt[iv].x - pt[next].x) * (pt[iv].y + pt[next].y);
		}
		face[f].vtNewell.set(vectorN[0], vectorN[1], vectorN[2]);
		face[f].vtNewell.normalize();
	}
}
void Mesh::SetColor(int colorIdx)
{
	for (int f = 0; f < numFaces; f++)
	{
		for (int v = 0; v < face[f].nVerts; v++)
		{
			face[f].vert[v].colorIndex = colorIdx;
		}
	}
}

Mesh rectangular;
Mesh rectangular1;
Mesh rectangular2;
Mesh rectangular3;
Mesh rectangular4;


Mesh chot1;
Mesh chot2;
Mesh chot3;
Mesh cube1;
Mesh cube2;
Mesh cube3;
Mesh cube4;
Mesh cube5;
Mesh cube6;

Mesh nuavongtrong1_1;
Mesh nuavongtrong1_2;
Mesh nuavongtrong1_3;
Mesh nuavongtrongthangiua;
Mesh nuavongtrongthangiua2;
Mesh nuavongtrongthangiua3;
Mesh nuavongtrongthangiua4;



float* vectorNewell(float iVector[10][3], int n)
{
	float vectorResult[] = { 0,0,0 };
	for (int i = 0; i < n; i++)
	{
		vectorResult[0] += (iVector[i][1] - iVector[(i + 1) % n][1]) * (iVector[i][2] + iVector[(i + 1) % n][2]);
		vectorResult[1] += (iVector[i][2] - iVector[(i + 1) % n][2]) * (iVector[i][0] + iVector[(i + 1) % n][0]);
		vectorResult[2] += (iVector[i][0] - iVector[(i + 1) % n][0]) * (iVector[i][1] + iVector[(i + 1) % n][1]);
	}
	return vectorResult;
}
void drawviengachvuong(float x, float y, float z, float alpha)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glColor4f(0.3, 1.0, 1.0, alpha);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(x, y, z);
	glVertex3f(x + 2, y, z);
	glVertex3f(x + 2, y, z + 2);
	glVertex3f(x, y, z + 2);
	glEnd();
}

void drawNen1(float alpha)
{
	float x = -31.4;
	float z = -30;
	float y = -10;
	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 1.0f, 1.0f);
	for (int i = 0; i < 30; i++)
	{
		x = x + 2.1;
		for (int j = 0; j < 30; j++) {
			z = z + 2.1;
			drawviengachvuong(x, y, z, alpha);
		}
		z = -30;
	}
	glEnable(GL_LIGHTING);
}
void viengach1(float a, float b, float alpha) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(151 / 255, 255 / 255, 255 / 255, alpha);
	glLineWidth(2);
	float R = 1;
	float y = 0;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 90; i++) {
		float x = a + R * cos(i * PI / 180.0);
		float z = b + R * sin(i * PI / 180.0);
		glVertex3f(x, y, z);
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (int i = 180; i <= 270; i++) {
		float x = a + R * cos(i * PI / 180.0) + 2;
		float z = b + R * sin(i * PI / 180.0) + 2;
		glVertex3f(x, y, z);
	}
	glEnd();
}
void viengach2(float a, float b, float alpha) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(151 / 255, 255 / 255, 255 / 255, alpha);
	glLineWidth(2);
	float R = 1;
	float y = 0;
	glBegin(GL_LINE_STRIP);
	for (int i = 90; i <= 180; i++) {
		float x = a + R * cos(i * PI / 180.0) + 2;
		float z = b + R * sin(i * PI / 180.0);
		glVertex3f(x, y, z);
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (int i = 270; i <= 360; i++) {
		float x = a + R * cos(i * PI / 180.0);
		float z = b + R * sin(i * PI / 180.0) + 2;
		glVertex3f(x, y, z);
	}
	glEnd();
}
void viengach3(float a, float b, float alpha) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(151 / 255, 255 / 255, 255 / 255, alpha);
	glLineWidth(2);
	float y = 0;
	glBegin(GL_LINE_STRIP);
	glVertex3f(a + 1.0, y, b); glVertex3f(a + 1.0, y, b + 2.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(a, y, b + 1.0); glVertex3f(a + 2.0, y, b + 1.0);
	glEnd();
}
void viengach4(float a, float b, float alpha) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(151 / 255, 255 / 255, 255 / 255, alpha);
	float R = 0.5;
	glLineWidth(2);
	float y = 0;
	glBegin(GL_LINE_STRIP);
	glVertex3f(a + 1.0, y, b); glVertex3f(a + 1.0, y, b + 0.5);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(a + 1.0, y, b + 1.5); glVertex3f(a + 1.0, y, b + 2.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(a, y, b + 1.0); glVertex3f(a + 0.5, y, b + 1.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(a + 1.5, y, b + 1.0); glVertex3f(a + 2.0, y, b + 1.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i++) {
		float x = a + R * cos(i * PI / 180.0) + 1;
		float z = b + R * sin(i * PI / 180.0) + 1;
		glVertex3f(x, y, z);
	}
	glEnd();
}
void drawNen(float alpha)
{
	int m = 0;
	float y = 0;
	glDisable(GL_LIGHTING);
	for (float x = -40; x <= 40; x = x + 2)
	{
		for (float z = -40; z <= 40; z = z + 2) {
			switch (arr[m]) {
			case 0:
				viengach1(x, z, alpha);
				break;
			case 1:
				viengach2(x, z, alpha);
				break;
			case 2:
				viengach3(x, z, alpha);
				break;
			case 3:
				viengach4(x, z, alpha);
			default:
				break;
			}
			m++;
		}
	}
	glEnable(GL_LIGHTING);
}
void DrawThanhLienKet1()
{
	float t = 0.8;
	glPushMatrix();
	glTranslatef(0.5 - 2, 0, 0);

	glRotatef(m, 1, 0, 0);




	float h1 = 5 * sin(30 * PI / 180);
	float h2 = 5 * cos(30 * PI / 180);

	glTranslatef(0, h1, h2);

	glRotatef(45, 1, 0, 0);
	glRotatef(180, 1, 0, 0);

	glRotatef(-m, 1, 0, 0);


	if (nChoice == 0)
	{
		glTranslatef(0, 0, t);
		rectangular1.DrawWireframe();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
		rectangular2.DrawWireframe();
		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		chot1.DrawWireframe();
		nuavongtrongthangiua3.DrawWireframe();
		nuavongtrongthangiua.DrawWireframe();
		glTranslatef(5, 0, 0);
		chot2.DrawWireframe();
		nuavongtrongthangiua2.DrawWireframe();

	}
	else
	{
		glTranslatef(0, 0, t);
		rectangular1.DrawColor();
		glTranslatef(0, 0, -t);
		glTranslatef(0, 0, -t);
		
		glRotatef(180, 0, 1, 0);
		glTranslatef(-1, 0, -t/2);

		rectangular2.DrawColor();

		glTranslatef(1, 0, t/2);
		glRotatef(-180, 0, 1, 0);

		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		nuavongtrongthangiua3.DrawColor();
		chot1.DrawColor();
		nuavongtrongthangiua.DrawColor();
		glTranslatef(5, 0, 0);
		chot2.DrawColor();
		nuavongtrongthangiua2.DrawColor();

	}


	glPopMatrix();
}

void DrawThanhLienKet2()
{
	float t = 0.8;
	glPushMatrix();
	glTranslatef(0.5 - 2, 0, 0);

	glRotatef(m, 1, 0, 0);




	float h1 = 5 * sin(30 * PI / 180);
	float h2 = -5 * cos(30 * PI / 180);

	glTranslatef(0, h1, h2);

	glRotatef(45, 1, 0, 0);
	glRotatef(180, 1, 0, 0);

	glRotatef(-m, 1, 0, 0);


	if (nChoice == 0)
	{
		glTranslatef(0, 0, t);
		rectangular1.DrawWireframe();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
		
		

		rectangular2.DrawWireframe();

		



		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		chot1.DrawWireframe();
		nuavongtrongthangiua3.DrawWireframe();
		nuavongtrongthangiua.DrawWireframe();
		glTranslatef(5, 0, 0);
		chot2.DrawWireframe();
		nuavongtrongthangiua2.DrawWireframe();

	}
	else
	{
		glTranslatef(0, 0, t);
		rectangular1.DrawColor();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
		rectangular2.DrawColor();
		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		nuavongtrongthangiua3.DrawColor();
		chot1.DrawColor();
		nuavongtrongthangiua.DrawColor();
		glTranslatef(5, 0, 0);
		chot2.DrawColor();
		nuavongtrongthangiua2.DrawColor();

	}


	glPopMatrix();
}
void DrawThanhLienKet3()
{
	float t = 0.8;
	glPushMatrix();
	glTranslatef(0.5 - 2, 0, 0);

	glRotatef(m, 1, 0, 0);




	float h1 = -5.0;
	float h2 = 0;

	glTranslatef(0, h1, h2);

	glRotatef(45, 1, 0, 0);
	glRotatef(180, 1, 0, 0);

	glRotatef(-m, 1, 0, 0);


	if (nChoice == 0)
	{
		glTranslatef(0, 0, t);
		rectangular1.DrawWireframe();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
		

	

		rectangular2.DrawWireframe();

		

		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		chot1.DrawWireframe();
		nuavongtrongthangiua3.DrawWireframe();
		nuavongtrongthangiua.DrawWireframe();
		glTranslatef(5, 0, 0);
		chot2.DrawWireframe();
		nuavongtrongthangiua2.DrawWireframe();

	}
	else
	{
		glTranslatef(0, 0, t);
		rectangular1.DrawColor();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
		rectangular2.DrawColor();
		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		nuavongtrongthangiua3.DrawColor();
		chot1.DrawColor();
		nuavongtrongthangiua.DrawColor();
		glTranslatef(5, 0, 0);
		chot2.DrawColor();
		nuavongtrongthangiua2.DrawColor();

	}


	glPopMatrix();
}
void DrawThanhLienKet4()
{
	float t = 0.8;
	float u = 6;
	glPushMatrix();
	glTranslatef(0.5 - 2 - 2, 0, 0);

	glTranslatef(0, -sqrt(12.5), -sqrt(12.5));
	glRotatef(m, 1, 0, 0);




	float h1 = -5.0;
	float h2 = 0;

	glTranslatef(0, h1, h2);

	
	glRotatef(15, 1, 0, 0);

	glRotatef(-m, 1, 0, 0);


	if (nChoice == 0)
	{
		glTranslatef(0, 0, t);
		rectangular3.DrawWireframe();
		glTranslatef(0, 0, -t);

		glTranslatef(0, 0, -t);
		rectangular4.DrawWireframe();
		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		chot1.DrawWireframe();
		nuavongtrongthangiua4.DrawWireframe();
		nuavongtrongthangiua.DrawWireframe();
		glTranslatef(u, 0, 0);
		chot2.DrawWireframe();
		nuavongtrongthangiua2.DrawWireframe();

	}
	else
	{
		glTranslatef(0, 0, t);
		rectangular3.DrawColor();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
		glRotatef(180, 0, 1, 0);
		glTranslatef(-1, 0, -t / 2);

		rectangular4.DrawColor();

		glTranslatef(1, 0, t / 2);
		glRotatef(-180, 0, 1, 0);
		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		nuavongtrongthangiua4.DrawColor();
		chot1.DrawColor();
		nuavongtrongthangiua.DrawColor();
		glTranslatef(u, 0, 0);
		chot2.DrawColor();
		nuavongtrongthangiua2.DrawColor();

	}


	glPopMatrix();
}

void DrawThanhLienKet5()
{
	float t = 0.8;
	float u = 6;
	glPushMatrix();
	glTranslatef(0.5 - 2 - 2, 0, 0);
	glTranslatef(0, -sqrt(12.5), -sqrt(12.5));
	glRotatef(m, 1, 0, 0);


	

	float h1 = 5 * sin(30 * PI / 180);
	float h2 = 5 * cos(30 * PI / 180);

	glTranslatef(0, h1, h2);

	glRotatef(15, 1, 0, 0);


	glRotatef(-m, 1, 0, 0);


	if (nChoice == 0)
	{
		glTranslatef(0, 0, t);
		rectangular3.DrawWireframe();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
	

		rectangular4.DrawWireframe();

	
		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		chot1.DrawWireframe();
		nuavongtrongthangiua4.DrawWireframe();
		nuavongtrongthangiua.DrawWireframe();
		glTranslatef(u, 0, 0);
		chot2.DrawWireframe();
		nuavongtrongthangiua2.DrawWireframe();

	}
	else
	{
		glTranslatef(0, 0, t);
		rectangular3.DrawColor();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
		

		glRotatef(180, 0, 1, 0);
		glTranslatef(-1, 0, -t / 2);

		rectangular4.DrawColor();

		glTranslatef(1, 0, t / 2);
		glRotatef(-180, 0, 1, 0);

		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		nuavongtrongthangiua4.DrawColor();
		chot1.DrawColor();
		nuavongtrongthangiua.DrawColor();
		glTranslatef(u, 0, 0);
		chot2.DrawColor();
		nuavongtrongthangiua2.DrawColor();

	}



	glPopMatrix();
}

void DrawThanhLienKet6()
{
	float t = 0.8;
	float u = 6;
	glPushMatrix();
	glTranslatef(0.5 - 2 - 2, 0, 0);
	glTranslatef(0, -sqrt(12.5), -sqrt(12.5));
	glRotatef(m, 1, 0, 0);




	float h1 = 5 * sin(30 * PI / 180);
	float h2 = -5 * cos(30 * PI / 180);

	glTranslatef(0, h1, h2);

	glRotatef(15, 1, 0, 0);


	glRotatef(-m, 1, 0, 0);


	if (nChoice == 0)
	{
		glTranslatef(0, 0, t);
		rectangular3.DrawWireframe();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);
	

		rectangular4.DrawWireframe();

		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		chot1.DrawWireframe();
		nuavongtrongthangiua4.DrawWireframe();
		nuavongtrongthangiua.DrawWireframe();
		glTranslatef(u, 0, 0);
		chot2.DrawWireframe();
		nuavongtrongthangiua2.DrawWireframe();

	}
	else
	{
		glTranslatef(0, 0, t);
		rectangular3.DrawColor();
		glTranslatef(0, 0, -t);


		glTranslatef(0, 0, -t);


		glRotatef(180, 0, 1, 0);
		glTranslatef(-1, 0, -t / 2);

		rectangular4.DrawColor();

		glTranslatef(1, 0, t / 2);
		glRotatef(-180, 0, 1, 0);

		glTranslatef(0, 0, t);
		glTranslatef(0, 0, t / 4);
		glTranslatef(0.5, 0, 0);
		glRotatef(90, 0, 0, 1);
		nuavongtrongthangiua4.DrawColor();
		chot1.DrawColor();
		nuavongtrongthangiua.DrawColor();
		glTranslatef(u, 0, 0);
		chot2.DrawColor();
		nuavongtrongthangiua2.DrawColor();

	}

	// He so cua vat the

	glPopMatrix();
}



void DrawCanhQuat1()
{





	int mau = 0;

	cube1.SetColor(mau);
	cube2.SetColor(mau);
	cube3.SetColor(mau);
	cube4.SetColor(mau);
	cube5.SetColor(mau);
	cube6.SetColor(mau);

	nuavongtrong1_1.SetColor(mau);
	nuavongtrong1_2.SetColor(mau);
	nuavongtrong1_3.SetColor(mau);


	glPushMatrix();

	glTranslatef(0, 0, 0);
	glRotatef(m, 1, 0, 0);
	glRotatef(90, 0, 0, 1);


	if (nChoice == 0)
	{
		nuavongtrong1_3.DrawWireframe();
		nuavongtrong1_2.DrawWireframe();
		nuavongtrong1_1.DrawWireframe();
		glRotatef(-90, 0, 0, 1);
		cube1.DrawWireframe();
		cube2.DrawWireframe();
		cube3.DrawWireframe();
		cube4.DrawWireframe();
		cube5.DrawWireframe();
		cube6.DrawWireframe();

	}
	else
	{
		nuavongtrong1_3.DrawColor();
		nuavongtrong1_2.DrawColor();
		nuavongtrong1_1.DrawColor();
		glRotatef(-90, 0, 0, 1);
		cube1.DrawColor();
		cube2.DrawColor();
	
		
		cube3.DrawColor();
		cube4.DrawColor();
		cube5.DrawColor();
		cube6.DrawColor();
	}


	glPopMatrix();
}


void DrawCanhQuat2() {


	int mau = 1;

	cube1.SetColor(mau);
	cube2.SetColor(mau);
	cube3.SetColor(mau);
	cube4.SetColor(mau);
	cube5.SetColor(mau);
	cube6.SetColor(mau);

	nuavongtrong1_1.SetColor(mau);
	nuavongtrong1_2.SetColor(mau);
	nuavongtrong1_3.SetColor(mau);


	glPushMatrix();

	glTranslatef(-2, -sqrt(12.5), -sqrt(12.5));
	glRotatef(m, 1, 0, 0);


	glRotatef(90, 0, 0, 1);

	if (nChoice == 0)
	{
		nuavongtrong1_3.DrawWireframe();
		nuavongtrong1_2.DrawWireframe();
		nuavongtrong1_1.DrawWireframe();
		glRotatef(-90, 0, 0, 1);
		cube1.DrawWireframe();
		cube2.DrawWireframe();
		cube3.DrawWireframe();
		cube4.DrawWireframe();
		cube5.DrawWireframe();
		cube6.DrawWireframe();

	}
	else
	{
		nuavongtrong1_3.DrawColor();
		nuavongtrong1_2.DrawColor();
		nuavongtrong1_1.DrawColor();
		glRotatef(-90, 0, 0, 1);
		cube1.DrawColor();
		cube2.DrawColor();
		cube3.DrawColor();
		cube4.DrawColor();
		cube5.DrawColor();
		cube6.DrawColor();
	}


	glPopMatrix();




}


void DrawCanhQuat3() {
	int mau = 2;
	
	cube1.SetColor(mau);
	cube2.SetColor(mau);
	cube3.SetColor(mau);
	cube4.SetColor(mau);
	cube5.SetColor(mau);
	cube6.SetColor(mau);

	nuavongtrong1_1.SetColor(mau);
	nuavongtrong1_2.SetColor(mau);
	nuavongtrong1_3.SetColor(mau);




	glPushMatrix();

	glTranslatef(-2 - 2, -sqrt(12.5) + 6.0 * cos(15.0 * PI / 180.0), -sqrt(12.5) + 6.0 * sin(15.0 * PI / 180.0));
	glRotatef(m, 1, 0, 0);


	glRotatef(90, 0, 0, 1);
	//glRotatef(((atan((t * sin(m * PI / 180.0)) / (4.0 + t * cos(m * PI / 180.0)))) * 180) / PI, 1, 0, 0);

	if (nChoice == 0)
	{
		nuavongtrong1_3.DrawWireframe();
		nuavongtrong1_2.DrawWireframe();
		nuavongtrong1_1.DrawWireframe();
		glRotatef(-90, 0, 0, 1);
		cube1.DrawWireframe();
		cube2.DrawWireframe();
		cube3.DrawWireframe();
		cube4.DrawWireframe();
		cube5.DrawWireframe();
		cube6.DrawWireframe();

	}
	else
	{
		nuavongtrong1_3.DrawColor();
		nuavongtrong1_2.DrawColor();
		nuavongtrong1_1.DrawColor();
		glRotatef(-90, 0, 0, 1);
		cube1.DrawColor();
		cube2.DrawColor();
		cube3.DrawColor();
		cube4.DrawColor();
		cube5.DrawColor();
		cube6.DrawColor();
	}
	// He so cua vat the


	glPopMatrix();




}

void DrawObject()
{
	glColor3f(0, 0, 0);

	DrawThanhLienKet1();
	
	DrawThanhLienKet2();
	DrawThanhLienKet3();
	DrawThanhLienKet4();
	DrawThanhLienKet5();
	DrawThanhLienKet6();
	DrawCanhQuat1();

	DrawCanhQuat2();
	DrawCanhQuat3();


}
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	

	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2], 0, 0, 0, 0, 1, 0);
	glViewport(0, 0, screenWidth, screenHeight);





	GLfloat light_position0[] = { -30.0, 30.0, -15.0, 0.0 };


	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	GLfloat light_position1[] = { 30.0, 30.0, -15.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);


	glClearStencil(0);
	glClearDepth(1.0f);

	
	DrawObject();
	
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glDisable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawNen1(0.7f);
	glDisable(GL_BLEND);

	glFlush();
	glutSwapBuffers();
}

void myInit()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	float pos = 0.3;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
	
	
	GLfloat lmodel_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	GLfloat light_ambient0[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
	
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular0);
	


	glOrtho(-fHalfSize * 2, fHalfSize * 2, -fHalfSize / 2, fHalfSize + fHalfSize / 2, -1000, 1000);

}
void rotateCamera(int value)
{
	camera_angle += (GLfloat)value;
	if (camera_angle > 360) camera_angle = camera_angle - 360.0f;
	camera[0] = camera_dis * cos(DEGRADIAN * camera_angle);
	camera[2] = camera_dis * sin(DEGRADIAN * camera_angle);
	glutTimerFunc(10, rotateCamera, 1);
	glutPostRedisplay();
}
void updatePosCamera()
{
	camera[0] = camera_dis * cos(DEGRADIAN * camera_angle);
	camera[1] = camera[1];
	camera[2] = camera_dis * sin(DEGRADIAN * camera_angle);
}
void OnKeySpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
	{
		camera_angle++;
		if (camera_angle > 360) camera_angle = camera_angle - 360.0f;
		updatePosCamera();
		myInit();
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		camera_angle--;
		if (camera_angle < 0) camera_angle = 360.0f;
		updatePosCamera();
		myInit();
		break;
	}
	case GLUT_KEY_UP:
	{
		if (fHalfSize >= fConstFS / 2 && camera[1] < 0)
		{
			fHalfSize -= 0.1;
			camera[1] += 0.1;
		}
		else
		{
			fHalfSize += 0.1;
			camera[1] += 0.1;
		}
		myInit();
		break;
	}

	case GLUT_KEY_DOWN:
	{
		if (fHalfSize >= fConstFS / 2 && camera[1] >= 0)
		{
			fHalfSize -= 0.1;
			camera[1] -= 0.1;
		}
		else
		{
			fHalfSize += 0.1;
			camera[1] -= 0.1;
		}
		myInit();
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}
void OnKeyboard(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		
		nChoice = (nChoice + 1) % 2;
	}

	
	else if (key == 'v' || key == 'V')
	{
		cameraState[6] = ((int)cameraState[6] + 1) % 2;
		if (cameraState[6] == 0)
		{
			camera[0] = cameraState[0];
			camera[1] = cameraState[1];
			camera[2] = cameraState[2];
			
		}
		else
		{
			cameraState[0] = camera[0];
			cameraState[1] = camera[1];
			cameraState[2] = camera[2];
			cameraState[3] = 0;
			cameraState[4] = 90;
			cameraState[5] = -10;

			camera[0] = cameraState[3];
			camera[1] = cameraState[4];
			camera[2] = cameraState[5];

			
			
		}
	}

	else if (key == '+' && cameraState[6] != 1)
	{
		if (camera_dis < 0)
		{
			camera_dis -= 0.1;
			fHalfSize += 0.1;
		}
		else
		{
			camera_dis -= 0.1;
			fHalfSize -= 0.1;
		}
		updatePosCamera();
		glOrtho(-fHalfSize * 2, fHalfSize * 2, -fHalfSize / 2, fHalfSize + fHalfSize / 2, -1000, 1000);
		myInit();
	}

	else if (key == '-' && cameraState[6] != 1)
	{
		if (camera_dis >= 0)
		{
			camera_dis += 0.1;
			fHalfSize += 0.1;
		}
		else
		{
			camera_dis += 0.1;
			fHalfSize -= 0.1;
		}
		updatePosCamera();
		glOrtho(-fHalfSize * 2, fHalfSize * 2, -fHalfSize / 2, fHalfSize + fHalfSize / 2, -1000, 1000);
		myInit();
	}

	
	else if (key == '1')
	{
		m = m + 2;
		if (m >= 720) {
			m = 0;
		}
	}
	else if (key == '2')
	{
		m = m - 2;
		if (m <= -720) {
			m = 0;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(0, 0); // set window position on screen
	glutCreateWindow("Huynh Thi Truong Duy - MSSV: 1710779"); // open the screen window

	cout << "1: Xoay ban quay nguoc chieu kim dong ho" << endl;
	cout << "2: Xoay ban quay cung chieu kim dong ho" << endl;
	cout << "3,4: Dieu chinh vi tri chot 2" << endl;
	cout << "W, w: Chuyen doi qua lai giua che do khung day va to mau" << endl;
	cout << "V, v: Chuyen doi qua lai giua hai che do nhin khac nhau" << endl;
	cout << "D, d: Bat/tat nguon sang thu hai" << endl;
	cout << "+   : Tang khoang cach camera" << endl;
	cout << "-   : Giam khoang cach camera" << endl;
	cout << "up arrow  : Tang chieu cao camera" << endl;
	cout << "down arrow: Giam chieu cao camera" << endl;
	cout << "<-        : Quay camera theo chieu kim dong ho" << endl;
	cout << "->        : Quay camera nguoc chieu kim dong ho" << endl;
	//	chay vong lap luu cac gia tri ramdom cua nen nha
	for (int i = 0; i < 2000; i++) {
		arr[i] = rand() % 4;
	}
	//Bật đèn	
	lightState = true;
	//Thiết lập thông số ban đầu camera
	camera[0] = camera_dis * cos(DEGRADIAN * (camera_angle));
	camera[1] = camera_height;
	camera[2] = camera_dis * sin(DEGRADIAN * (camera_angle ));
	cameraState[6] = 0;

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;



	float size_canhquat = 1.1;
	
	cube1.CreateCube(size_canhquat);
	
	cube1.vectorNewell();
	cube1.addMaterial(ambient, diffuse, specular, shininess);
	cube2.CreateCube2(size_canhquat);
	
	cube2.vectorNewell();
	cube2.addMaterial(ambient, diffuse, specular, shininess);
	cube3.CreateCube3(size_canhquat);
	
	cube3.vectorNewell();
	cube3.addMaterial(ambient, diffuse, specular, shininess);
	cube4.CreateCube4(size_canhquat);
	
	cube4.vectorNewell();
	cube4.addMaterial(ambient, diffuse, specular, shininess);
	cube5.CreateCube5(size_canhquat);
	
	cube5.vectorNewell();
	cube5.addMaterial(ambient, diffuse, specular, shininess);
	cube6.CreateCube6(size_canhquat);
	
	cube6.vectorNewell();
	cube6.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrong1_1.CreateNuavongtron1_1(size_canhquat, 2, 1);

	nuavongtrong1_1.vectorNewell();
	nuavongtrong1_1.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrong1_2.CreateNuavongtron1_2(size_canhquat, 2, 1);
	
	nuavongtrong1_2.vectorNewell();
	nuavongtrong1_2.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrong1_3.CreateNuavongtron1_3(size_canhquat, 2, 1);

	nuavongtrong1_3.vectorNewell();
	nuavongtrong1_3.addMaterial(ambient, diffuse, specular, shininess);
	float size_thanhlienket1 = 5.0;
	float size_thanhlienket2 = 6.0;
	int mau_thanhlienket = 3;
	rectangular1.CreateRectangular(0.4, 1, size_thanhlienket1);
	rectangular1.SetColor(mau_thanhlienket);
	rectangular1.vectorNewell();
	rectangular1.addMaterial(ambient, diffuse, specular, shininess);
	rectangular2.CreateRectangular(0.4, 1, size_thanhlienket1);
	rectangular2.SetColor(mau_thanhlienket);
	rectangular2.vectorNewell();
	rectangular2.addMaterial(ambient, diffuse, specular, shininess);
	rectangular3.CreateRectangular(0.4, 1, size_thanhlienket2);
	rectangular3.SetColor(mau_thanhlienket);
	rectangular3.vectorNewell();
	rectangular3.addMaterial(ambient, diffuse, specular, shininess);
	rectangular4.CreateRectangular(0.4, 1, size_thanhlienket2);
	rectangular4.SetColor(mau_thanhlienket);
	rectangular4.vectorNewell();
	rectangular4.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua.CreateNuaVongTronThanGiua(0.6, 0.4, 1, 1);
	nuavongtrongthangiua.SetColor(mau_thanhlienket);
	nuavongtrongthangiua.vectorNewell();
	nuavongtrongthangiua.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua2.CreateNuaVongTronThanGiua(0.6, 0.4, 1, -1);
	nuavongtrongthangiua2.SetColor(mau_thanhlienket);
	nuavongtrongthangiua2.vectorNewell();
	nuavongtrongthangiua2.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua3.CreateNuaVongTronThanGiua2(0.6, size_thanhlienket1, 1, -1);
	nuavongtrongthangiua3.SetColor(mau_thanhlienket);
	nuavongtrongthangiua3.vectorNewell();
	nuavongtrongthangiua3.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua4.CreateNuaVongTronThanGiua2(0.6, size_thanhlienket2, 1, -1);
	nuavongtrongthangiua4.SetColor(mau_thanhlienket);
	nuavongtrongthangiua4.vectorNewell();
	nuavongtrongthangiua4.addMaterial(ambient, diffuse, specular, shininess);

	int mau_chot = 4;
	chot1.CreateHinhTru(32, 1, 0.6);
	chot1.SetColor(mau_chot);
	chot1.vectorNewell();
	chot1.addMaterial(ambient, diffuse, specular, shininess);
	chot2.CreateHinhTru(32, 1, 0.6);
	chot2.SetColor(mau_chot);
	chot2.vectorNewell();
	chot2.addMaterial(ambient, diffuse, specular, shininess);


	glutKeyboardFunc(OnKeyboard);
	glutSpecialFunc(OnKeySpecial);

	myInit();

	glutDisplayFunc(myDisplay);
	glutMainLoop();
	delete[] arr;
	return 0;
}