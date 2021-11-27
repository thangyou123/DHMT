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


float	ColorArr[COLORNUM][3] = { {1.0, 0.0, 0.0}, {151.0 / 255.0,255.0 / 255 , 255.0 / 255}, { 199.0 / 255.0,21.0 / 255 , 133.0 / 255},
								{0 / 255.0,191.0 / 255 , 255.0 / 255}, { 1.0, 0.0, 1.0},{ 0 / 255.0,250.0 / 255 , 154.0 / 255},
								 {0.3, 0.3, 0.3}, {0 / 255.0,250.0 / 255 , 154.0 / 255}, { 0.9,  0.9, 0.9},
								{1.0, 0.5,  0.5}, { 0.6117664,0.15294 , 0.690196},{ 0.5, 0.5, 1.0},
									{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0},{0 / 255.0,128.0 / 255 , 128.0 / 255} };
int		screenWidth = 1000;
int		screenHeight = 500;
int		nChoice = 1;
int* arr = new int[2000];
float t = 1;
float m = 0;
float camera_dis = 4.5;
float camera_height = 2;
float camera[3];
float cameraState[7];
bool lightState = true;
GLfloat camera_angle = 15.0;
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
	Vector3 cross(Vector3 b);
	float dot(Vector3 b);
};

Vector3 Vector3::cross(Vector3 b)
{
	Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	return c;
}
float Vector3::dot(Vector3 b)
{
	return x * b.x + y * b.y + z * b.z;
}
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
	void CreateCube(float	fSize, float t);
	void CreateCube2(float	fSize, float t);
	void CreateCube3(float	fSize, float t);
	void CreateCube4(float	fSize, float t);
	void CreateCube5(float	fSize, float t);
	void CreateCube6(float	fSize, float t);
	void CreateCylinder(int nSegment, float fHeight, float fRadius);
	void CreateRectangular(float fLength, float fWidth, float fHeight);
	void CreateShape1(float fLength, float fWidth, float fHeight);
	void CreateShape2(float fLength, float fWidth, float fHeight);
	void CreateHalfCylinder1(int nSegment, float fHeight, float fRadius);
	void CreateHalfCylinder2(int nSegment, float fHeight, float fRadius, float fLength);
	void CreateShape3_1(float fLength, float fWidth, float fHeight);
	void CreateShape3_2(float fLength, float fWidth, float fHeight);
	void CreateShape3_3(float fLength, float fWidth, float fHeight);
	void CreateShape4(float day, float rong, float cao);
	void CreateNuaVongTron(float t, float fx, float fHeight);
	void CreateNuaVongTron2(float t, float fx, float fHeight);
	void CreateNuaVongTron3(float t, float fx, float fHeight);
	void CreateNuaVongTronThanGiua(float t, float fx, float fHeight, float h);
	void CreateNuaVongTronThanGiua2(float t, float fx, float fHeight, float h);
	void CreateNuavongtron1_1(float t, float fx, float fHeight);
	void CreateNuavongtron1_2(float t, float fx, float fHeight);
	void CreateNuavongtron1_3(float t, float fx, float fHeight);


	void vectorNewell();

};

void  Mesh::CreateNuaVongTronThanGiua(float t, float fx, float fHeight, float h) {
	int Seg = 32;
	numVerts = Seg * 4 + 2;
	pt = new Point3[numVerts];

	float fRadius = t;
	int		i;
	int		idx;
	float	fAngle = (PI) / (Seg - 1.0);
	float	x, y, z, x2, z2;

	pt[0].set(0, fHeight / 2, 0);
	pt[2 * Seg].set(0, fHeight / 2, 0);
	for (i = 0; i < Seg; i++)
	{
		x2 = fRadius * -sin(fAngle * i) * h;
		z2 = fRadius * -cos(fAngle * i) * h;

		x = (fRadius + fx) * -sin(fAngle * i) * h;
		z = (fRadius + fx) * -cos(fAngle * i) * h;
		y = fHeight / 2;

		pt[i + 1].set(x, y, z);
		pt[i + 1 + Seg + Seg].set(x2, y, z2);

		y = -fHeight / 2;
		pt[i + 1 + Seg].set(x, y, z);
		pt[i + 1 + Seg + Seg + Seg].set(x2, y, z2);

	}

	pt[numVerts - 1].set(0, -fHeight / 2, 0);

	numFaces = Seg * 4;
	face = new Face[numFaces];

	idx = 0;

	for (i = 0; i < Seg - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 + Seg * 2;
		face[idx].vert[1].vertIndex = i + 2 + Seg * 2;

		face[idx].vert[2].vertIndex = i + 2;
		face[idx].vert[3].vertIndex = i + 1;
		idx++;
	}


	for (i = Seg; i < 2 * Seg - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 + Seg * 2;
		face[idx].vert[1].vertIndex = i + 2 + Seg * 2;

		face[idx].vert[2].vertIndex = i + 2;
		face[idx].vert[3].vertIndex = i + 1;
		idx++;
	}


	for (i = 0; i < Seg - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;

		face[idx].vert[1].vertIndex = i + 2;

		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + Seg;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + Seg;

		idx++;
	}

	for (i = 2 * Seg; i < 3 * Seg - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;




		face[idx].vert[1].vertIndex = i + 2;

		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + Seg;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + Seg;

		idx++;
	}







}
void  Mesh::CreateNuaVongTronThanGiua2(float t, float fx, float fHeight, float h) {
	int Seg = 32;
	numVerts = Seg * 4 + 2;
	pt = new Point3[numVerts];

	float fRadius = t;
	int		i;
	int		idx;
	float	fAngle = (PI) / (Seg - 1.0);
	float	x, y, z, x2, z2;

	pt[0].set(0, fHeight / 2, 0);
	pt[2 * Seg].set(0, fHeight / 2, 0);
	for (i = 0; i < Seg; i++)
	{
		x2 = fRadius * -sin(fAngle * i) * h;
		z2 = fRadius * -cos(fAngle * i) * h;

		x = (fRadius)*sin(fAngle * i) * h + 5.0;
		z = (fRadius)*cos(fAngle * i) * h;
		y = fHeight / 2;

		pt[i + 1].set(x, y, z);
		pt[i + 1 + Seg + Seg].set(x2, y, z2);

		y = -fHeight / 2;
		pt[i + 1 + Seg].set(x, y, z);
		pt[i + 1 + Seg + Seg + Seg].set(x2, y, z2);

	}

	pt[numVerts - 1].set(0, -fHeight / 2, 0);

	numFaces = Seg * 4;
	face = new Face[numFaces];

	idx = 0;

	for (i = 0; i < Seg - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 + Seg * 2;
		face[idx].vert[1].vertIndex = (Seg - i - 2) + 2;

		face[idx].vert[2].vertIndex = (Seg - i - 2) + 1;
		face[idx].vert[3].vertIndex = i + 2 + Seg * 2;;
		idx++;
	}


	for (i = Seg; i < 2 * Seg - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = i + 1 + Seg * 2;
		face[idx].vert[1].vertIndex = (3 * Seg - i - 2) + 2;

		face[idx].vert[2].vertIndex = (3 * Seg - i - 2) + 1;
		face[idx].vert[3].vertIndex = i + 2 + Seg * 2;
		idx++;
	}


	for (i = 0; i < Seg - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;

		face[idx].vert[1].vertIndex = i + 2;

		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + Seg;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + Seg;

		idx++;
	}

	for (i = 2 * Seg; i < 3 * Seg - 1; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;




		face[idx].vert[1].vertIndex = i + 2;

		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + Seg;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + Seg;

		idx++;
	}







}
void  Mesh::CreateNuaVongTron3(float t, float fx, float fHeight) {
	int Seg = 32;
	numVerts = Seg * 2 + 2;
	pt = new Point3[numVerts];
	float fRadius = t;
	int		i;
	int		idx;
	float	fAngle = (PI - 30.0 * PI / 180) / (Seg - 1);
	float	x, y, z;

	pt[0].set(0, fHeight / 2, 0);
	for (i = 0; i < Seg; i++)
	{
		x = fRadius * sin(fAngle * i + 15.0 * PI / 180);
		z = fRadius * cos(fAngle * i + 15.0 * PI / 180);
		float dx = x * cos(-60 * PI / 180) - z * sin(-60 * PI / 180);
		float dz = x * sin(-60 * PI / 180) + z * cos(-60 * PI / 180);
		x = dx + 5 * cos(-60 * PI / 180);
		z = dz + 5 * sin(-60 * PI / 180);
		y = fHeight / 2;
		if (i == 0 || i == Seg - 1) {
			x = 5 * cos(-60 * PI / 180);
			z = 5 * sin(-60 * PI / 180);
		}
		pt[i + 1].set(x, y, z);

		y = -fHeight / 2;
		pt[i + 1 + Seg].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -fHeight / 2, 0);

	numFaces = Seg * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 1;
		if (i < Seg - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 1;
		idx++;
	}

	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;
		if (i < Seg - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + Seg;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + Seg;

		idx++;
	}

	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = Seg + 1;
		if (i < Seg - 1)
			face[idx].vert[2].vertIndex = i + 2 + Seg;
		else
			face[idx].vert[2].vertIndex = 1 + Seg;
		face[idx].vert[1].vertIndex = i + 1 + Seg;
		idx++;
	}





}
void  Mesh::CreateNuaVongTron2(float t, float fx, float fHeight) {
	int Seg = 32;
	numVerts = Seg * 2 + 2;
	pt = new Point3[numVerts];
	float fRadius = t;
	int		i;
	int		idx;
	float	fAngle = (PI - 30.0 * PI / 180) / (Seg - 1);
	float	x, y, z;

	pt[0].set(0, fHeight / 2, 0);
	for (i = 0; i < Seg; i++)
	{
		x = fRadius * sin(fAngle * i + 15.0 * PI / 180);
		z = fRadius * cos(fAngle * i + 15.0 * PI / 180);
		float dx = x * cos(60 * PI / 180) - z * sin(60 * PI / 180);
		float dz = x * sin(60 * PI / 180) + z * cos(60 * PI / 180);
		x = dx + 5 * cos(60 * PI / 180);
		z = dz + 5 * sin(60 * PI / 180);
		y = fHeight / 2;
		if (i == 0 || i == Seg - 1) {
			x = 5 * cos(60 * PI / 180);
			z = 5 * sin(60 * PI / 180);
		}
		pt[i + 1].set(x, y, z);

		y = -fHeight / 2;
		pt[i + 1 + Seg].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -fHeight / 2, 0);

	numFaces = Seg * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 1;
		if (i < Seg - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 1;
		idx++;
	}

	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;
		if (i < Seg - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + Seg;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + Seg;

		idx++;
	}

	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = Seg + 1;
		if (i < Seg - 1)
			face[idx].vert[2].vertIndex = i + 2 + Seg;
		else
			face[idx].vert[2].vertIndex = 1 + Seg;
		face[idx].vert[1].vertIndex = i + 1 + Seg;
		idx++;
	}






}
void  Mesh::CreateNuaVongTron(float t, float fx, float fHeight) {
	int Seg = 32;
	numVerts = Seg * 2 + 2;
	pt = new Point3[numVerts];
	float fRadius = t;
	int		i;
	int		idx;
	float	fAngle = (PI - 30.0 * PI / 180) / (Seg - 1.0);
	float	x, y, z;

	pt[0].set(0, fHeight / 2, 0);
	for (i = 0; i < Seg; i++)
	{
		x = fRadius * -sin(fAngle * i + 15.0 * PI / 180) - 5.0;
		z = fRadius * -cos(fAngle * i + 15.0 * PI / 180);
		y = fHeight / 2;
		if (i == 0 || i == Seg - 1) {
			x = -5.0;
			z = 0;
		}
		pt[i + 1].set(x, y, z);

		y = -fHeight / 2;
		pt[i + 1 + Seg].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -fHeight / 2, 0);

	numFaces = Seg * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 1;
		if (i < Seg - 1)

			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 1;
		idx++;
	}

	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;
		if (i < Seg - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + Seg;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + Seg;

		idx++;
	}

	for (i = 0; i < Seg; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = Seg + 1;
		if (i < Seg - 1)
			face[idx].vert[2].vertIndex = i + 2 + Seg;
		else
			face[idx].vert[2].vertIndex = 1 + Seg;
		face[idx].vert[1].vertIndex = i + 1 + Seg;
		idx++;
	}






}

void Mesh::CreateCylinder(int nSegment, float fHeight, float fRadius)
{
	numVerts = nSegment * 2 + 2; // đỉnh ở phía trên + dưới (bao gồm cả tâm)
	pt = new Point3[numVerts];

	int		i;
	int		idx;
	float	fAngle = 2 * PI / (nSegment - 1);
	float	x, y, z;

	pt[0].set(0, fHeight / 2, 0);
	for (i = 0; i < nSegment; i++)
	{
		x = fRadius * cos(fAngle * i);
		z = fRadius * sin(fAngle * i);
		y = fHeight / 2;
		pt[i + 1].set(x, y, z);

		y = -fHeight / 2;
		pt[i + 1 + nSegment].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -fHeight / 2, 0);

	numFaces = nSegment * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 1;
		idx++;
	}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nSegment;

		idx++;
	}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = numVerts - 1;
		if (i < nSegment - 1)
			face[idx].vert[2].vertIndex = i + 2 + nSegment;
		else
			face[idx].vert[2].vertIndex = 1 + nSegment;
		face[idx].vert[1].vertIndex = i + 1 + nSegment;
		idx++;
	}

}
void Mesh::CreateCube(float	fSize, float t)
{
	float fRadius = t;

	float x = fRadius * sin(0 + 15.0 * PI / 180);

	float z = fRadius * cos(0 + 15.0 * PI / 180);
	float dx = x * cos(60 * PI / 180) - z * sin(60 * PI / 180);
	float dz = x * sin(60 * PI / 180) + z * cos(60 * PI / 180);
	x = 5 * cos(60 * PI / 180);
	z = 5 * sin(60 * PI / 180);

	float x2 = fRadius * sin(PI - 15.0 * PI / 180);

	float z2 = fRadius * cos(PI - 15.0 * PI / 180);

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

void Mesh::CreateCube2(float	fSize, float t)
{
	float fRadius = t;

	float x = fRadius * sin(15.0 * PI / 180);

	float z = fRadius * cos(15.0 * PI / 180);
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

void Mesh::CreateCube3(float	fSize, float t)
{
	float fRadius = t;


	float x = 5 * cos(-60 * PI / 180);
	float z = 5 * sin(-60 * PI / 180);



	float x2 = fRadius * sin(0 + 15.0 * PI / 180);

	float z2 = fRadius * cos(0 + 15.0 * PI / 180);

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

void Mesh::CreateCube4(float	fSize, float t)
{
	float fRadius = t;

	float x = fRadius * sin(165.0 * PI / 180);

	float z = fRadius * cos(165.0 * PI / 180);
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
void Mesh::CreateCube5(float	fSize, float t)
{
	float fRadius = t;

	float x = fRadius * -sin(15.0 * PI / 180) - 5.0;
	float z = fRadius * -cos(15.0 * PI / 180);


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
void Mesh::CreateCube6(float	fSize, float t)
{
	float fRadius = t;

	float x = fRadius * -sin(165.0 * PI / 180) - 5.0;
	float z = fRadius * -cos(165.0 * PI / 180);


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
void Mesh::CreateRectangular(float fLength, float fWidth, float fHeight)
{
	numVerts = 8;
	numFaces = 6;
	pt = new Point3[numVerts];
	face = new Face[numFaces];
	int idx = 0;
	int indexColor = 1;
	{
		pt[0].set(0, 0, 0); //A
		pt[1].set(0, 0, fLength); //B
		pt[2].set(fWidth, 0, fLength); //C
		pt[3].set(fWidth, 0, 0); //D
		pt[4].set(0, fHeight, 0); //A'
		pt[5].set(0, fHeight, fLength); //B
		pt[6].set(fWidth, fHeight, fLength); //C
		pt[7].set(fWidth, fHeight, 0); //D

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
void Mesh::CreateShape1(float fLength, float fWidth, float fHeight)
{
	int nSegment = 36;
	float fRadius = fLength / 4.0;
	float a = fHeight / 4.0;
	float b = fHeight * 0.65;
	int i;
	int idx = 0;
	numVerts = 16 + nSegment * 2 + 2;
	pt = new Point3[numVerts];
	pt[0].set(-fLength / 2.0, a, fWidth / 2.0);//A
	pt[1].set(fLength / 2.0, a, fWidth / 2.0);//B
	pt[2].set(fLength / 2.0, 0, fWidth / 2.0);//C
	pt[3].set(-fLength / 2.0, 0, fWidth / 2.0);//D
	pt[4].set(-fLength / 2.0, a, -fWidth / 2.0);//A'
	pt[5].set(fLength / 2.0, a, -fWidth / 2.0);//B'
	pt[6].set(fLength / 2.0, 0, -fWidth / 2.0);//C'
	pt[7].set(-fLength / 2.0, 0, -fWidth / 2.0);//D'

	pt[8].set(-fLength / 4.0, b, fWidth / 2.0);//E
	pt[9].set(fLength / 4.0, b, fWidth / 2.0);//F
	pt[10].set(fLength / 4.0, a, fWidth / 2.0);//G
	pt[11].set(-fLength / 4.0, a, fWidth / 2.0);//H
	pt[12].set(-fLength / 4.0, b, -fWidth / 2.0);//E'
	pt[13].set(fLength / 4.0, b, -fWidth / 2.0);//F'
	pt[14].set(fLength / 4.0, a, -fWidth / 2.0);//G'
	pt[15].set(-fLength / 4.0, a, -fWidth / 2.0);//H'

	numFaces = 10 + nSegment * 3;
	face = new Face[numFaces];
	//khối dưới
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 1;
	face[idx].vert[2].vertIndex = 2;
	face[idx].vert[3].vertIndex = 3;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 1;
	face[idx].vert[2].vertIndex = 5;
	face[idx].vert[3].vertIndex = 4;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 4;
	face[idx].vert[1].vertIndex = 5;
	face[idx].vert[2].vertIndex = 6;
	face[idx].vert[3].vertIndex = 7;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 2;
	face[idx].vert[1].vertIndex = 3;
	face[idx].vert[2].vertIndex = 7;
	face[idx].vert[3].vertIndex = 6;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 3;
	face[idx].vert[2].vertIndex = 7;
	face[idx].vert[3].vertIndex = 4;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 1;
	face[idx].vert[1].vertIndex = 2;
	face[idx].vert[2].vertIndex = 6;
	face[idx].vert[3].vertIndex = 5;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//khối giữa
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 8;
	face[idx].vert[1].vertIndex = 9;
	face[idx].vert[2].vertIndex = 10;
	face[idx].vert[3].vertIndex = 11;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 12;
	face[idx].vert[1].vertIndex = 13;
	face[idx].vert[2].vertIndex = 14;
	face[idx].vert[3].vertIndex = 15;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 8;
	face[idx].vert[1].vertIndex = 11;
	face[idx].vert[2].vertIndex = 15;
	face[idx].vert[3].vertIndex = 12;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 9;
	face[idx].vert[1].vertIndex = 10;
	face[idx].vert[2].vertIndex = 14;
	face[idx].vert[3].vertIndex = 13;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//khối cầu trên
	float	fAngle = PI / (nSegment - 1);
	float	x, y, z;
	int istart = 16;
	pt[16].set(0, b, fWidth / 2);
	for (i = 0; i < nSegment; i++) {
		x = fRadius * cos(fAngle * i);
		y = fRadius * sin(fAngle * i) + b;
		z = fWidth / 2;
		pt[istart + i + 1].set(x, y, z);

		z = -fWidth / 2;
		pt[istart + i + 1 + nSegment].set(x, y, z);
	}
	pt[numVerts - 1].set(0, b, -fWidth / 2);

	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = istart;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = istart + i + 2;
		else
			face[idx].vert[1].vertIndex = istart + 1;
		face[idx].vert[2].vertIndex = istart + i + 1;
		idx++;
	}

	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = istart + i + 1;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = istart + i + 2;
		else
			face[idx].vert[1].vertIndex = istart + 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nSegment;

		idx++;
	}

	for (i = 0; i < nSegment; i++) {
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = numVerts - 1;
		if (i < nSegment - 1)
			face[idx].vert[2].vertIndex = istart + i + 2 + nSegment;
		else
			face[idx].vert[2].vertIndex = istart + 1 + nSegment;
		face[idx].vert[1].vertIndex = istart + i + 1 + nSegment;
		idx++;
	}
}
void Mesh::CreateShape2(float fLength, float fWidth, float fHeight)
{
	int nSegment = 36;
	int	idx = 0, idp = 0;
	float fAngle = PI / nSegment;
	float x, y, z;
	float fRadius = fWidth / 2.0;
	int iStart, iCount;
	numVerts = 8 + nSegment * 4;
	numFaces = 4 + 2 * nSegment;
	face = new Face[numFaces];
	pt = new Point3[numVerts];
	//mat duoi
	{
		iStart = idp;
		float iStop;
		for (int i = 0; i <= nSegment; i++)
		{
			x = fRadius * -sin(fAngle * i);
			z = fRadius * -cos(fAngle * i);
			y = 0;
			pt[iStart + i].set(x, y, z); // 17
			idp++;
		}
		iStart = idp;
		for (int i = 0; i <= nSegment; i++)
		{
			x = fRadius * sin(fAngle * i) + fLength;
			z = fRadius * cos(fAngle * i);
			y = 0;
			pt[iStart + i].set(x, y, z); // 17
			idp++;
		}
		pt[idp].set(pt[0].x, pt[0].y, pt[0].z);
		idp++;
		iCount = idp;
		iStart = 0;
		face[idx].nVerts = iCount;
		face[idx].vert = new VertexID[face[idx].nVerts];
		for (int i = 0; i < iCount; i++)
		{
			face[idx].vert[i].vertIndex = iStart + i;
		}
		idx++;
	}
	//mat tren
	{
		iStart = idp;
		iCount = 0;
		float iStop;
		for (int i = 0; i <= nSegment; i++)
		{
			x = fRadius * -sin(fAngle * i);
			z = fRadius * -cos(fAngle * i);
			y = fHeight;
			pt[iStart + i].set(x, y, z); // 17
			idp++;
			iCount++;
		}
		int iStart2 = idp;
		for (int i = 0; i <= nSegment; i++)
		{
			x = fRadius * sin(fAngle * i) + fLength;
			z = fRadius * cos(fAngle * i);
			y = fHeight;
			pt[iStart2 + i].set(x, y, z); // 17
			idp++;
			iCount++;
		}
		pt[idp].set(pt[iStart].x, pt[iStart].y, pt[iStart].z);
		idp++;
		iCount++;
		face[idx].nVerts = iCount;
		face[idx].vert = new VertexID[face[idx].nVerts];
		for (int i = 0; i < iCount; i++)
		{
			face[idx].vert[i].vertIndex = iStart + i;
		}
		idx++;
	}
	//xung quang
	{
		iStart = 0;
		int iStart2 = 2 * nSegment + 3;
		iCount = 2 * nSegment + 2;
		for (int i = 0; i < iCount; i++)
		{
			face[idx].nVerts = 4;
			face[idx].vert = new VertexID[face[idx].nVerts];
			face[idx].vert[0].vertIndex = iStart + i;
			face[idx].vert[1].vertIndex = iStart2 + i;
			face[idx].vert[2].vertIndex = iStart2 + i + 1;
			face[idx].vert[3].vertIndex = iStart + i + 1;
			idx++;
		}
	}
}
void Mesh::CreateHalfCylinder1(int nSegment, float fHeight, float fRadius)
{
	numVerts = nSegment * 2 + 2;
	pt = new Point3[numVerts];

	int		i;
	int		idx;
	float	fAngle = PI / (nSegment - 1);
	float	x, y, z;

	pt[0].set(0, fHeight / 2, 0);
	for (i = 0; i < nSegment; i++)
	{
		x = fRadius * -sin(fAngle * i);
		z = fRadius * -cos(fAngle * i);
		y = fHeight / 2;
		pt[i + 1].set(x, y, z);

		y = -fHeight / 2;
		pt[i + 1 + nSegment].set(x, y, z);
	}
	pt[numVerts - 1].set(0, -fHeight / 2, 0);

	numFaces = nSegment * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 1;
		idx++;
	}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 4;
		face[idx].vert = new VertexID[face[idx].nVerts];

		face[idx].vert[0].vertIndex = i + 1;
		if (i < nSegment - 1)
			face[idx].vert[1].vertIndex = i + 2;
		else
			face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nSegment;
		face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nSegment;

		idx++;
	}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = numVerts - 1;
		if (i < nSegment - 1)
			face[idx].vert[2].vertIndex = i + 2 + nSegment;
		else
			face[idx].vert[2].vertIndex = 1 + nSegment;
		face[idx].vert[1].vertIndex = i + 1 + nSegment;
		idx++;
	}

}
void Mesh::CreateHalfCylinder2(int nSegment, float fHeight, float fRadius, float length)
{
	numVerts = nSegment * 2 + 2;
	pt = new Point3[numVerts];

	int		i;
	int		idx;
	float	fAngle = PI / (nSegment - 1);
	float	x, y, z;

	pt[0].set(length, fHeight / 2, 0);
	for (i = 0; i < nSegment; i++)
	{
		x = fRadius * sin(fAngle * i) + length;
		z = fRadius * cos(fAngle * i);
		y = fHeight / 2;
		pt[i + 1].set(x, y, z);

		y = -fHeight / 2;
		pt[i + 1 + nSegment].set(x, y, z);
	}
	pt[numVerts - 1].set(length, -fHeight / 2, 0);

	numFaces = nSegment * 3;
	face = new Face[numFaces];

	idx = 0;
	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = 0;
		face[idx].vert[1].vertIndex = 1;
		face[idx].vert[2].vertIndex = i + 1;
		idx++;
	}
	if (i < nSegment - 1)
		face[idx].vert[1].vertIndex = i + 2;
	else

		for (i = 0; i < nSegment; i++)
		{
			face[idx].nVerts = 4;
			face[idx].vert = new VertexID[face[idx].nVerts];

			face[idx].vert[0].vertIndex = i + 1;
			if (i < nSegment - 1)
				face[idx].vert[1].vertIndex = i + 2;
			else
				face[idx].vert[1].vertIndex = 1;
			face[idx].vert[2].vertIndex = face[idx].vert[1].vertIndex + nSegment;
			face[idx].vert[3].vertIndex = face[idx].vert[0].vertIndex + nSegment;

			idx++;
		}

	for (i = 0; i < nSegment; i++)
	{
		face[idx].nVerts = 3;
		face[idx].vert = new VertexID[face[idx].nVerts];
		face[idx].vert[0].vertIndex = numVerts - 1;
		if (i < nSegment - 1)
			face[idx].vert[2].vertIndex = i + 2 + nSegment;
		else
			face[idx].vert[2].vertIndex = 1 + nSegment;
		face[idx].vert[1].vertIndex = i + 1 + nSegment;
		idx++;
	}

}
void Mesh::CreateShape3_1(float fLength, float fWidth, float fHeight)
{
	float linber = fWidth / 6.0;
	float fRadius = fWidth / 2.0;
	float a = fLength / 14.0;
	float b = (fLength * 10) / 14.0;
	float phanc = (fLength) / 15.0;
	int i;
	int idx = 0;
	numVerts = 24;
	pt = new Point3[numVerts];
	pt[0].set(0, fHeight / 2.0, -fRadius);//A
	pt[1].set(a, fHeight / 2.0, -fRadius);//B
	pt[2].set(a, fHeight / 2.0, fRadius);//C
	pt[3].set(0, fHeight / 2.0, fRadius);//D
	pt[4].set(0, -fHeight / 2.0, -fRadius);//A'
	pt[5].set(a, -fHeight / 2.0, -fRadius);//B'
	pt[6].set(a, -fHeight / 2.0, fRadius);//C'
	pt[7].set(0, -fHeight / 2.0, fRadius);//D'
	pt[8].set(b, fHeight / 2.0, -fRadius);//E
	pt[9].set(fLength, fHeight / 2.0, -fRadius);//F
	pt[10].set(fLength, fHeight / 2.0, fRadius);//G
	pt[11].set(b, fHeight / 2.0, fRadius);//H
	pt[12].set(b, -fHeight / 2.0, -fRadius);//E'd
	pt[13].set(fLength, -fHeight / 2.0, -fRadius);//F'
	pt[14].set(fLength, -fHeight / 2.0, fRadius);//G'
	pt[15].set(b, -fHeight / 2.0, fRadius);//H'
	pt[16].set(a, fHeight / 2.0, -linber);//K
	pt[17].set(b, fHeight / 2.0, -linber);//L
	pt[18].set(b, fHeight / 2.0, linber);//M
	pt[19].set(a, fHeight / 2.0, linber);//N
	pt[20].set(a, -fHeight / 2.0, -linber);//K'
	pt[21].set(b, -fHeight / 2.0, -linber);//L'
	pt[22].set(b, -fHeight / 2.0, linber);//M'
	pt[23].set(a, -fHeight / 2.0, linber);//N'
	numFaces = 14;
	face = new Face[numFaces];
	//mặt trên
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 1;
	face[idx].vert[2].vertIndex = 2;
	face[idx].vert[3].vertIndex = 3;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 1;
	face[idx].vert[1].vertIndex = 8;
	face[idx].vert[2].vertIndex = 17;
	face[idx].vert[3].vertIndex = 16;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 19;
	face[idx].vert[1].vertIndex = 18;
	face[idx].vert[2].vertIndex = 11;
	face[idx].vert[3].vertIndex = 2;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 8;
	face[idx].vert[1].vertIndex = 9;
	face[idx].vert[2].vertIndex = 10;
	face[idx].vert[3].vertIndex = 11;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//mặt dưới
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 4;
	face[idx].vert[1].vertIndex = 5;
	face[idx].vert[2].vertIndex = 6;
	face[idx].vert[3].vertIndex = 7;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 5;
	face[idx].vert[1].vertIndex = 12;
	face[idx].vert[2].vertIndex = 21;
	face[idx].vert[3].vertIndex = 20;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 23;
	face[idx].vert[1].vertIndex = 22;
	face[idx].vert[2].vertIndex = 15;
	face[idx].vert[3].vertIndex = 6;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 12;
	face[idx].vert[1].vertIndex = 13;
	face[idx].vert[2].vertIndex = 14;
	face[idx].vert[3].vertIndex = 15;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//mặt xung quang
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 9;
	face[idx].vert[2].vertIndex = 13;
	face[idx].vert[3].vertIndex = 4;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 3;
	face[idx].vert[1].vertIndex = 10;
	face[idx].vert[2].vertIndex = 14;
	face[idx].vert[3].vertIndex = 7;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//mặt trong
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 16;
	face[idx].vert[1].vertIndex = 17;
	face[idx].vert[2].vertIndex = 21;
	face[idx].vert[3].vertIndex = 20;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 18;
	face[idx].vert[1].vertIndex = 19;
	face[idx].vert[2].vertIndex = 23;
	face[idx].vert[3].vertIndex = 22;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 16;
	face[idx].vert[1].vertIndex = 19;
	face[idx].vert[2].vertIndex = 23;
	face[idx].vert[3].vertIndex = 20;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 17;
	face[idx].vert[1].vertIndex = 18;
	face[idx].vert[2].vertIndex = 22;
	face[idx].vert[3].vertIndex = 21;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
}
void Mesh::CreateNuavongtron1_3(float t, float fx, float fHeight) {
	CreateNuaVongTron3(t, fx, fHeight);

}
void Mesh::CreateNuavongtron1_1(float t, float fx, float fHeight) {
	CreateNuaVongTron(t, fx, fHeight);

}

void Mesh::CreateNuavongtron1_2(float t, float fx, float fHeight) {


	CreateNuaVongTron2(t, fx, fHeight);

}

void Mesh::CreateShape3_2(float fLength, float fWidth, float fHeight)
{
	int nSegment = 32;
	int	idx = 0, idp = 0;
	float fAngle = PI / nSegment;
	float x, y, z;
	float fRadius = fWidth / 2.0;
	float phana = (fLength * 4.0) / 15.0;
	float phanb = (fLength * 9.0) / 15.0;
	float phanc = (fLength) / 15.0;

	CreateHalfCylinder1(nSegment, fHeight, fRadius);
}
void Mesh::CreateShape3_3(float fLength, float fWidth, float fHeight)
{
	int nSegment = 32;
	int	idx = 0, idp = 0;
	float fAngle = 2 * PI / nSegment;
	float x, y, z;
	float fRadius = fWidth / 2.0;
	float phana = (fLength * 4.0) / 15.0;
	float phanb = (fLength * 9.0) / 15.0;
	float phanc = (fLength) / 15.0;

	CreateHalfCylinder2(nSegment, fHeight, fRadius, fLength);
}
void Mesh::CreateShape4(float day, float rong, float cao) {
	int i;
	int idx = 0;
	numVerts = 16;
	pt = new Point3[numVerts];
	pt[0].set(0, cao, 0);
	pt[1].set(rong, cao, 0);
	pt[2].set(rong, cao, day);
	pt[3].set(0, cao, day);
	pt[4].set(day, cao - day, day);
	pt[5].set(day, cao - day, 0);
	pt[6].set(rong - day, cao - day, 0);
	pt[7].set(rong - day, cao - day, day);
	pt[8].set(0, 0, day);
	pt[9].set(0, 0, 0);
	pt[10].set(day, 0, 0);
	pt[11].set(day, 0, day);
	pt[12].set(rong - day, 0, day);
	pt[13].set(rong - day, 0, 0);
	pt[14].set(rong, 0, 0);
	pt[15].set(rong, 0, day);
	numFaces = 14;
	face = new Face[numFaces];
	//mặt trên
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 1;
	face[idx].vert[2].vertIndex = 2;
	face[idx].vert[3].vertIndex = 3;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//mặt dưới
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 8;
	face[idx].vert[1].vertIndex = 9;
	face[idx].vert[2].vertIndex = 10;
	face[idx].vert[3].vertIndex = 11;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 12;
	face[idx].vert[1].vertIndex = 13;
	face[idx].vert[2].vertIndex = 14;
	face[idx].vert[3].vertIndex = 15;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//mặt chữ u trong
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 4;
	face[idx].vert[1].vertIndex = 5;
	face[idx].vert[2].vertIndex = 10;
	face[idx].vert[3].vertIndex = 11;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 4;
	face[idx].vert[1].vertIndex = 5;
	face[idx].vert[2].vertIndex = 6;
	face[idx].vert[3].vertIndex = 7;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 6;
	face[idx].vert[1].vertIndex = 7;
	face[idx].vert[2].vertIndex = 12;
	face[idx].vert[3].vertIndex = 13;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//mat bentrong
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 1;
	face[idx].vert[2].vertIndex = 6;
	face[idx].vert[3].vertIndex = 5;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 5;
	face[idx].vert[2].vertIndex = 10;
	face[idx].vert[3].vertIndex = 9;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 1;
	face[idx].vert[1].vertIndex = 6;
	face[idx].vert[2].vertIndex = 13;
	face[idx].vert[3].vertIndex = 14;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//mat ben ngoai
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 2;
	face[idx].vert[1].vertIndex = 3;
	face[idx].vert[2].vertIndex = 4;
	face[idx].vert[3].vertIndex = 7;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 3;
	face[idx].vert[1].vertIndex = 4;
	face[idx].vert[2].vertIndex = 11;
	face[idx].vert[3].vertIndex = 8;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 2;
	face[idx].vert[1].vertIndex = 15;
	face[idx].vert[2].vertIndex = 12;
	face[idx].vert[3].vertIndex = 7;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 0;
	idx++;
	//mặt bên
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 3;
	face[idx].vert[2].vertIndex = 8;
	face[idx].vert[3].vertIndex = 9;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 7;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[face[idx].nVerts];
	face[idx].vert[0].vertIndex = 1;
	face[idx].vert[1].vertIndex = 2;
	face[idx].vert[2].vertIndex = 15;
	face[idx].vert[3].vertIndex = 14;
	for (i = 0; i < face[idx].nVerts; i++)
		face[idx].vert[i].colorIndex = 7;
	idx++;
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
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
			glNormal3f(face[f].vtNewell.x, face[f].vtNewell.y, face[f].vtNewell.z);
		}
		glEnd();
	}
}
void Mesh::Draw(int chooseColor)
{
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
			glNormal3f(face[f].vtNewell.x, face[f].vtNewell.y, face[f].vtNewell.z);
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
Mesh shape1;
Mesh shape2;
Mesh shape3_1;
Mesh shape3_2;
Mesh shape3_3;
Mesh shape4;
Mesh cylinder;
Mesh banquay;
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
void DrawDe()
{
	glPushMatrix();
	glTranslatef(-2.5, 0, -7);
	if (nChoice == 0)
	{
		rectangular.DrawWireframe();
	}
	else
	{
		rectangular.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	rectangular.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void Drawde1()
{
	float t = 0.8;
	glPushMatrix();
	glTranslatef(0.5 - 2, 0, 0);
	//(15, 1, 0, 0);
	glRotatef(m, 1, 0, 0);
	/*float z1 = sqrt(12.5);
	float z2 = sqrt(12.5) ;

	glTranslatef(0, -z1, 0);

	glTranslatef(0, z1,0);*/



	float h1 = 5 * sin(30 * PI / 180);
	float h2 = 5 * cos(30 * PI / 180);

	glTranslatef(0, h1, h2);

	glRotatef(45, 1, 0, 0);
	glRotatef(180, 1, 0, 0);
	//glTranslatef(0,z1, z2);
	//glRotatef(m, 1, 0, 0);
	//glTranslatef(0, -z1, -z2);
	//glRotatef(-m, 1, 0, 0);

	glRotatef(-m, 1, 0, 0);
	//glTranslatef(0, sqrt(12.5), sqrt(12.5));

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

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	rectangular1.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void Drawde2()
{
	glPushMatrix();
	glTranslatef(-0.5, 16, -7);
	if (nChoice == 0)
	{
		rectangular2.DrawWireframe();
	}
	else
	{
		rectangular2.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	rectangular2.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawTayQuay()
{
	glPushMatrix();
	glTranslatef(1.8, 7, 0);
	glRotatef(m, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	if (nChoice == 0)
	{
		shape2.DrawWireframe();
	}
	else
	{
		shape2.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	shape2.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawChot1()
{
	glPushMatrix();
	glTranslatef(2.45, 3 + 11.0 * cos(((atan((t * sin(m * PI / 180.0)) / (4.0 + t * cos(m * PI / 180.0)))) * 180) / PI * PI / 180), 11.0 * sin(((atan((t * sin(m * PI / 180.0)) / (4.0 + t *

		(m * PI / 180.0)))) * 180) / PI * PI / 180));
	glRotatef(90, 0, 0, 1);
	if (nChoice == 0)
	{
		chot1.DrawWireframe();
	}
	else
	{
		chot1.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	chot1.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawChot2()
{
	glPushMatrix();
	glTranslatef(0, t * cos(m * PI / 180.0), t * sin(m * PI / 180.0));
	glTranslatef(1.5, 7, 0);
	glRotatef(90, 0, 0, 1);

	if (nChoice == 0)
	{
		chot2.DrawWireframe();
	}
	else
	{
		chot2.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	chot2.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawChot3()
{
	glPushMatrix();
	glTranslatef(1.4, 3, 0);
	glRotatef(90, 0, 0, 1);
	if (nChoice == 0)
	{
		chot3.DrawWireframe();
	}
	else
	{
		chot3.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	chot3.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawCube1() {
	glPushMatrix();
	if (nChoice == 0)
	{
		cube1.DrawWireframe();
	}
	else
	{
		cube1.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	cube1.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawCube2() {
	glPushMatrix();

	if (nChoice == 0)
	{
		cube2.DrawWireframe();
	}
	else
	{
		cube2.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	cube2.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawThanhchuU() {
	glPushMatrix();
	glTranslatef(2.37, 11.6, 1.1 + 11.0 * sin(((atan((t * sin(m * PI / 180.0)) / (4.0 + t * cos(m * PI / 180.0)))) * 180) / PI * PI / 180));
	glRotatef(90, 0, 1, 0);
	if (nChoice == 0)
	{
		shape4.DrawWireframe();
	}
	else
	{
		shape4.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	shape4.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}






void DrawBanQuay()
{
	glPushMatrix();
	glTranslatef(0.9, 7, 0);
	glRotatef(90, 0, 0, 1);
	if (nChoice == 0)
	{
		banquay.DrawWireframe();
	}
	else
	{
		banquay.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	banquay.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawThanhTruot()
{
	glPushMatrix();
	glTranslatef(2.75, 18, 11.0 * sin(((atan((t * sin(m * PI / 180.0)) / (4.0 + t * cos(m * PI / 180.0)))) * 180) / PI * PI / 180));
	glRotatef(90, 1, 0, 0);
	if (nChoice == 0)
	{
		cylinder.DrawWireframe();
	}
	else
	{
		cylinder.DrawColor();
	}

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	cylinder.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}


void DrawThanhLienKet2() {


	glPushMatrix();

	glTranslatef(-2, -sqrt(12.5), -sqrt(12.5));
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
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	nuavongtrong1_1.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrong1_1.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrong1_1.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();




}


void DrawThanhLienKet()
{
	glPushMatrix();

	glTranslatef(0, 0, 0);
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
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	nuavongtrong1_1.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrong1_1.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrong1_1.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawGoido1() {
	glPushMatrix();
	glTranslatef(0.5, 18, -6);
	glRotatef(-90, 0, 0, 1);
	if (nChoice == 0)
	{
		shape1.DrawWireframe();
	}
	else
	{
		shape1.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	shape1.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawGoido2() {
	glPushMatrix();
	glTranslatef(0.5, 18, 6);
	glRotatef(-90, 0, 0, 1);
	if (nChoice == 0)
	{
		shape1.DrawWireframe();
	}
	else
	{
		shape1.DrawColor();
	}
	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	shape1.addMaterial(ambient, diffuse, specular, shininess);
	glPopMatrix();
}
void DrawObject()
{
	glColor3f(0, 0, 0);
	/*Drawde1();
	DrawDe();
	Drawde2();
	DrawTayQuay();
	DrawChot1();
	DrawChot2();
	DrawChot3();
	DrawCube1();
	DrawCube2();
	DrawThanhchuU();
	DrawThanhTruot();
	DrawBanQuay();*/
	//DrawThanhchuU();
	Drawde1();
	DrawThanhLienKet();
	//DrawTayQuay();
	//DrawThanhLienKet2();
	//DrawCube1();

}
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GLfloat light_position0[] = { 10.0, 10.0, 10.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	if (lightState == true)
	{
		glEnable(GL_LIGHT1);
		GLfloat diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat specular1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat position1[] = { -10.0, 10.0, -10.0, 0.0 };

		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
		glLightfv(GL_LIGHT1, GL_POSITION, position1);
	}
	else
		glDisable(GL_LIGHT1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2], 0, 0, 0, 0, 1, 0);
	glViewport(0, 0, screenWidth, screenHeight);

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

	glPushMatrix();
	glScalef(1, -1, 1);
	DrawObject();
	glPopMatrix();
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
	glEnable(GL_LIGHT0);

	GLfloat lmodel_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	GLfloat light_ambient0[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

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
	if (cameraState[6] == 1)
	{
		return;
	}
	switch (key)
	{
	case GLUT_KEY_LEFT:
	{
		camera_angle++;
		if (camera_angle > 360) camera_angle = camera_angle - 360.0f;
		updatePosCamera();
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		camera_angle--;
		if (camera_angle < 0) camera_angle = 360.0f;
		updatePosCamera();
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

	else if (key == 'd' || key == 'D')
	{
		lightState = !lightState;
	}

	else if (key == 'v' || key == 'V')
	{
		cameraState[6] = ((int)cameraState[6] + 1) % 2;
		if (cameraState[6] == 0)
		{
			camera[0] = cameraState[0];
			camera[1] = cameraState[1];
			camera[2] = cameraState[2];
			lightState = true;
		}
		else
		{
			cameraState[0] = camera[0];
			cameraState[1] = camera[1];
			cameraState[2] = camera[2];
			cameraState[3] = 10;
			cameraState[4] = 90;
			cameraState[5] = 0;

			camera[0] = cameraState[3];
			camera[1] = cameraState[4];
			camera[2] = cameraState[5];

			fHalfSize = 7;
			lightState = false;
			myInit();
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

	else if (key == '3')
	{
		if (t >= 0.0 and t < 1.5) {
			t = t + 0.05;
		}
		else {
			t = 1.5;
		}
	}
	else if (key == '4')
	{
		if (t > 0 and t <= 1.5) {
			t = t - 0.05;
		}
		else {
			t = 0;
		}
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
	camera[0] = camera_dis * cos(DEGRADIAN * camera_angle);
	camera[1] = camera_height;
	camera[2] = camera_dis * sin(DEGRADIAN * camera_angle);
	cameraState[6] = 0;

	float sizez = 1.0;
	cube1.CreateCube(3, sizez);
	cube1.SetColor(4);
	cube1.vectorNewell();

	cube2.CreateCube2(3, sizez);
	cube2.SetColor(4);
	cube2.vectorNewell();
	cube3.CreateCube3(3, sizez);
	cube3.SetColor(4);
	cube3.vectorNewell();
	cube4.CreateCube4(3, sizez);
	cube4.SetColor(4);
	cube4.vectorNewell();
	cube5.CreateCube5(3, sizez);
	cube5.SetColor(4);
	cube5.vectorNewell();
	cube6.CreateCube6(3, sizez);
	cube6.SetColor(4);
	cube6.vectorNewell();

	nuavongtrong1_1.CreateNuavongtron1_1(sizez, 2, 1);
	nuavongtrong1_1.SetColor(5);
	nuavongtrong1_1.vectorNewell();

	nuavongtrong1_2.CreateNuavongtron1_2(sizez, 2, 1);
	nuavongtrong1_2.SetColor(3);
	nuavongtrong1_2.vectorNewell();
	nuavongtrong1_3.CreateNuavongtron1_3(sizez, 2, 1);
	nuavongtrong1_3.SetColor(1);
	nuavongtrong1_3.vectorNewell();
	shape4.CreateShape4(0.65, 2.2, 6);
	shape4.SetColor(10);
	shape4.vectorNewell();
	//kết thúc quá trình tạo//
	rectangular1.CreateRectangular(0.4, 1, 5);
	rectangular1.SetColor(2);
	rectangular1.vectorNewell();
	rectangular2.CreateRectangular(0.4, 1, 5);
	rectangular2.SetColor(3);
	rectangular2.vectorNewell();

	nuavongtrongthangiua.CreateNuaVongTronThanGiua(0.6, 0.4, 1, 1);
	nuavongtrongthangiua.SetColor(5);
	nuavongtrongthangiua.vectorNewell();
	nuavongtrongthangiua2.CreateNuaVongTronThanGiua(0.6, 0.4, 1, -1);
	nuavongtrongthangiua2.SetColor(5);
	nuavongtrongthangiua2.vectorNewell();
	nuavongtrongthangiua3.CreateNuaVongTronThanGiua2(0.6, 0.4, 1, -1);
	nuavongtrongthangiua3.SetColor(5);
	nuavongtrongthangiua3.vectorNewell();
	chot1.CreateCylinder(32, 1, 0.6);
	chot1.SetColor(3);
	chot1.vectorNewell();
	chot2.CreateCylinder(32, 1, 0.6);
	chot2.SetColor(2);
	chot2.vectorNewell();
	glutKeyboardFunc(OnKeyboard);
	glutSpecialFunc(OnKeySpecial);

	myInit();

	glutDisplayFunc(myDisplay);
	glutMainLoop();
	delete[] arr;
	return 0;
}