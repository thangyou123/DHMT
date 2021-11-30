#include <stdlib.h>
#include "glut.h"
#include "glu.h"
#include "GL.H"
#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

#define PI			3.14159261


float	ColorArr[7][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},{1.0, 1.0, 0.0},{255.0 / 255.0,128.0 / 255.0,128.0 / 255.0} ,{255.0 / 255.0,0,128.0 / 255.0} ,{0.2,0.2,0.2} ,{0.0,0.0,1.0} };
int		screenWidth = 1000;
int		screenHeight = 500;
int		View_Color = 1;

float t = 1;
float m = 0;
float bankinh_camera = 5;
float chieu_cao_camera = 1;
float camera[3];
float luutrangthai_camera[6];
bool state2D;
GLfloat doxoay_camera =330;
GLfloat	size_vieww = 15;
GLfloat size_vieww_bandau = size_vieww;

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
	void chuanhoa() {
		x = x / sqrt(x * x + y * y + z * z);
		y = y / sqrt(x * x + y * y + z * z);
		z = z / sqrt(x * x + y * y + z * z);
	}
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


class VertexID
{
public:
	int		vdx; 
	int		cdx;
	int		ndx; 
	
};

class Face
{
public:
	int	n_Vert;
	VertexID* vert;
	Vector3 vN;
	Face()
	{
		n_Vert = 0;
		vert = NULL;
	}
	~Face()
	{
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		n_Vert = 0;
	}
};

class Mesh
{
public:
	int	num_V; 
	Point3* pointt; 

	int	num_F; 
	Face* face; 

public:
	Mesh()
	{
		num_V = 0;
		pointt = NULL;
		num_F = 0;
		face = NULL;
	}
	~Mesh()
	{
		if (pointt != NULL)
		{
			delete[] pointt;
		}
		if (face != NULL)
		{
			delete[] face;
		}
		num_V = 0;
		num_F = 0;
	}
	void DrawWireframe();
	void DrawColor();
	void SetColor(int i_color);
	void v_new();
	void addMaterial(float ambient[], float diffuse[], float specular[], float shiness);
	void Createcanhquat(float t);
	void Createcanhquat2(float t);
	void Createcanhquat3(float t);
	void Createcanhquat4(float t);
	void Createcanhquat5(float t);
	void Createcanhquat6(float t);
	void CreateHinhTru(int so_lopchia, float beday, float bankinh);
	void CreateMotPhanThanhLienKet(float chieu_dai, float chieu_rong, float beday);
	void CreateNuaVongTron(float bankinh, float fx, float beday);
	void CreateNuaVongTron2(float bankinh, float fx, float beday);
	void CreateNuaVongTron3(float bankinh, float fx, float beday);
	void CreateNuaVongTronThanGiua(float bankinh, float fx, float beday, float h);
	void CreateNuaVongTronThanGiua2(float bankinh, float fx, float beday, float h);

};
void  Mesh::CreateNuaVongTronThanGiua(float bankinh, float fx, float beday, float h) {
	int so_lopchia = 32;
	num_V = so_lopchia * 4 + 2;
	pointt = new Point3[num_V];
	int		i;
	int		i_f;
	float	do_quay = (PI) / (so_lopchia - 1.0);
	float	x, y, z, x2, z2;
	pointt[0].set(0, beday / 2, 0);
	pointt[2 * so_lopchia].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x2 = bankinh * -sin(do_quay * i) * h;
		z2 = bankinh * -cos(do_quay * i) * h;
		x = (bankinh + fx) * -sin(do_quay * i) * h;
		z = (bankinh + fx) * -cos(do_quay * i) * h;
		y = beday / 2;
		pointt[i + 1].set(x, y, z);
		pointt[i + 1 + so_lopchia + so_lopchia].set(x2, y, z2);
		y = -beday / 2;
		pointt[i + 1 + so_lopchia].set(x, y, z);
		pointt[i + 1 + so_lopchia + so_lopchia + so_lopchia].set(x2, y, z2);

	}
	pointt[num_V - 1].set(0, -beday / 2, 0);
	num_F = so_lopchia * 4;
	face = new Face[num_F];
	i_f = 0;
	for (i = 0; i < so_lopchia - 1; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = i + 2 + so_lopchia * 2;
		face[i_f].vert[1].vdx = i + 1 + so_lopchia * 2;

		face[i_f].vert[2].vdx = i + 1;
		face[i_f].vert[3].vdx = i + 2;
		i_f++;
	}


	for (i = so_lopchia; i < 2 * so_lopchia - 1; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = i + 1 + so_lopchia * 2;
		face[i_f].vert[1].vdx = i + 2 + so_lopchia * 2;

		face[i_f].vert[2].vdx = i + 2;
		face[i_f].vert[3].vdx = i + 1;
		i_f++;
	}

	
	for (i = 0; i < so_lopchia - 1; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];

		face[i_f].vert[0].vdx = i + 2;

		face[i_f].vert[1].vdx = i + 1;

		face[i_f].vert[2].vdx = face[i_f].vert[1].vdx + so_lopchia;
		face[i_f].vert[3].vdx = face[i_f].vert[0].vdx + so_lopchia ;

		i_f++;
	}

	for (i = 2 * so_lopchia; i < 3 * so_lopchia - 1; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = i + 2;
		face[i_f].vert[1].vdx = i + 1;
		face[i_f].vert[2].vdx = face[i_f].vert[1].vdx + so_lopchia;
		face[i_f].vert[3].vdx = face[i_f].vert[0].vdx + so_lopchia;
		i_f++;
	}
}
void  Mesh::CreateNuaVongTronThanGiua2(float bankinh, float fx, float beday, float h) {
	int so_lopchia = 32;
	num_V = so_lopchia * 4 + 2;
	pointt = new Point3[num_V];
	int		i;
	int		i_f;
	float	do_quay = (PI) / (so_lopchia - 1.0);
	float	x, y, z, x2, z2;
	pointt[0].set(0, beday / 2, 0);
	pointt[2 * so_lopchia].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x2 = bankinh * -sin(do_quay * i) * h;
		z2 = bankinh * -cos(do_quay * i) * h;
		x = (bankinh)*sin(do_quay * i) * h + fx;
		z = (bankinh)*cos(do_quay * i) * h;
		y = beday / 2;
		pointt[i + 1].set(x, y, z);
		pointt[i + 1 + so_lopchia + so_lopchia].set(x2, y, z2);
		y = -beday / 2;
		pointt[i + 1 + so_lopchia].set(x, y, z);
		pointt[i + 1 + so_lopchia + so_lopchia + so_lopchia].set(x2, y, z2);

	}
	pointt[num_V - 1].set(0, -beday / 2, 0);
	num_F = so_lopchia * 4;
	face = new Face[num_F];
	i_f = 0;
	for (i = 0; i < so_lopchia - 1; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = i + 1 + so_lopchia * 2;
		face[i_f].vert[1].vdx = (so_lopchia - i - 2) + 2;
		face[i_f].vert[2].vdx = (so_lopchia - i - 2) + 1;
		face[i_f].vert[3].vdx = i + 2 + so_lopchia * 2;;
		i_f++;
	}
	for (i = so_lopchia; i < 2 * so_lopchia - 1; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = i + 1 + so_lopchia * 2;
		face[i_f].vert[1].vdx = i + 2 + so_lopchia * 2;

		face[i_f].vert[2].vdx = (3 * so_lopchia - i - 2) + 1;
		face[i_f].vert[3].vdx = (3 * so_lopchia - i - 2) + 2;
		i_f++;
	}
}
void  Mesh::CreateNuaVongTron3(float bankinh, float fx, float beday) {
	int so_lopchia = 32;
	num_V = so_lopchia * 2 + 2;
	pointt = new Point3[num_V];
	int		i;
	int		i_f;
	float	do_quay = (PI - 26.0 * PI / 180) / (so_lopchia - 1);
	float	x, y, z;
	pointt[0].set(0, beday / 2, 0);
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
		pointt[i + 1].set(x, y, z);
		y = -beday / 2;
		pointt[i + 1 + so_lopchia].set(x, y, z);
	}
	pointt[num_V - 1].set(0, -beday / 2, 0);
	num_F = so_lopchia * 3;
	face = new Face[num_F];
	i_f = 0;
	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 3;
		face[i_f].vert = new VertexID[3];
		face[i_f].vert[0].vdx = 1;
		if (i < so_lopchia - 1)
			face[i_f].vert[1].vdx = i + 1;
		else
			face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = i + 2;
		i_f++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[4];

		face[i_f].vert[0].vdx = i + 2;
		if (i < so_lopchia - 1)
			face[i_f].vert[1].vdx = i + 1;
		else
			face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = face[i_f].vert[1].vdx + so_lopchia;
		face[i_f].vert[3].vdx = face[i_f].vert[0].vdx + so_lopchia;

		i_f++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 3;
		face[i_f].vert = new VertexID[3];
		face[i_f].vert[0].vdx = so_lopchia + 1;
		if (i < so_lopchia - 1)
			face[i_f].vert[2].vdx = i + 1 + so_lopchia;
		else
			face[i_f].vert[2].vdx = 1 + so_lopchia;
		face[i_f].vert[1].vdx = i + 2 + so_lopchia;
		i_f++;
	}
}
void  Mesh::CreateNuaVongTron2(float bankinh, float fx, float beday) {
	int so_lopchia = 32;
	num_V = so_lopchia * 2 + 2;
	pointt = new Point3[num_V];
	int		i;
	int		i_f;
	float	do_quay = (PI - 26.0 * PI / 180) / (so_lopchia - 1);
	float	x, y, z;
	pointt[0].set(0, beday / 2, 0);
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
		pointt[i + 1].set(x, y, z);
		y = -beday / 2;
		pointt[i + 1 + so_lopchia].set(x, y, z);
	}
	pointt[num_V - 1].set(0, -beday / 2, 0);
	num_F = so_lopchia * 3;
	face = new Face[num_F];
	i_f = 0;
	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 3;
		face[i_f].vert = new VertexID[3];
		face[i_f].vert[0].vdx = 1;
		if (i < so_lopchia - 1)
			face[i_f].vert[1].vdx = i + 1;
		else
			face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = i + 2;
		i_f++;
	}
	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[4];

		face[i_f].vert[0].vdx = i + 2;
		if (i < so_lopchia - 1)
			face[i_f].vert[1].vdx = i + 1;
		else
			face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = face[i_f].vert[1].vdx + so_lopchia;
		face[i_f].vert[3].vdx = face[i_f].vert[0].vdx + so_lopchia;
		i_f++;
	}
	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 3;
		face[i_f].vert = new VertexID[3];
		face[i_f].vert[0].vdx = so_lopchia + 1;
		if (i < so_lopchia - 1)
			face[i_f].vert[2].vdx = i + 1 + so_lopchia;
		else
			face[i_f].vert[2].vdx = 1 + so_lopchia;
		face[i_f].vert[1].vdx = i + 2 + so_lopchia;
		i_f++;
	}
}
void  Mesh::CreateNuaVongTron(float bankinh, float fx, float beday) {
	int so_lopchia = 32;
	num_V = so_lopchia * 2 + 2;
	pointt = new Point3[num_V];
	int		i;
	int		i_f;
	float	do_quay = (PI - 26.0 * PI / 180) / (so_lopchia - 1.0);
	float	x, y, z;

	pointt[0].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x = bankinh * -sin(do_quay * i + 13.0 * PI / 180) - 4.95;
		z = bankinh * -cos(do_quay * i + 13.0 * PI / 180);
		y = beday / 2;
		if (i == 0 || i == so_lopchia - 1) {
			x = -4.95;
			z = 0;
		}
		pointt[i + 1].set(x, y, z);

		y = -beday / 2;
		pointt[i + 1 + so_lopchia].set(x, y, z);
	}
	pointt[num_V - 1].set(0, -beday / 2, 0);

	num_F = so_lopchia * 3;
	face = new Face[num_F];

	i_f = 0;
	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 3;
		face[i_f].vert = new VertexID[3];
		face[i_f].vert[0].vdx = 1;
		if (i < so_lopchia - 1)

			face[i_f].vert[1].vdx = i + 1;
		else
			face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = i + 2;
		i_f++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[4];

		face[i_f].vert[0].vdx = i + 2;
		if (i < so_lopchia - 1)
			face[i_f].vert[1].vdx = i + 1;
		else
			face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = face[i_f].vert[1].vdx + so_lopchia;
		face[i_f].vert[3].vdx = face[i_f].vert[0].vdx + so_lopchia;

		i_f++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = 3;
		face[i_f].vert = new VertexID[3];
		face[i_f].vert[0].vdx = so_lopchia + 1;
		if (i < so_lopchia - 1)
			face[i_f].vert[2].vdx = i + 1 + so_lopchia;
		else
			face[i_f].vert[2].vdx = 1 + so_lopchia;
		face[i_f].vert[1].vdx = i + 2 + so_lopchia;
		i_f++;
	}
}
void  Mesh::CreateHinhTru(int so_lopchia, float beday, float bankinh) {	
	num_V = so_lopchia * 2 + 2;
	pointt = new Point3[num_V];
	num_F = so_lopchia * 3;
	face = new Face[num_F];
	int		i;
	int		i_f,matday=3,matben=4;
	float	do_quay = (2*PI ) / (so_lopchia - 1.0);
	float	x, y, z;
	pointt[0].set(0, beday / 2, 0);
	for (i = 0; i < so_lopchia; i++)
	{
		x = bankinh * -sin(do_quay * i );
		z = bankinh * -cos(do_quay * i );
		y = beday / 2	;
	
		pointt[i + 1].set(x, y, z);

		y = -beday / 2;
		pointt[i + 1 + so_lopchia].set(x, y, z);
	}
	pointt[num_V - 1].set(0, -beday / 2, 0);
	i_f = 0;
	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = matday;
		face[i_f].vert = new VertexID[matday];
		face[i_f].vert[0].vdx = 1;
		if (i < so_lopchia - 1)

			face[i_f].vert[1].vdx = i + 1;
		else
			face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = i + 2;
		i_f++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = matben;
		face[i_f].vert = new VertexID[matben];

		face[i_f].vert[0].vdx = i + 2;
		if (i < so_lopchia - 1)
			face[i_f].vert[1].vdx = i + 1;
		else
			face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = face[i_f].vert[1].vdx + so_lopchia;
		face[i_f].vert[3].vdx = face[i_f].vert[0].vdx + so_lopchia;

		i_f++;
	}

	for (i = 0; i < so_lopchia; i++)
	{
		face[i_f].n_Vert = matday;
		face[i_f].vert = new VertexID[matday];
		face[i_f].vert[0].vdx = so_lopchia + 1;
		if (i < so_lopchia - 1)
			face[i_f].vert[2].vdx = i + 1 + so_lopchia;
		else
			face[i_f].vert[2].vdx = 1 + so_lopchia;
		face[i_f].vert[1].vdx = i + 2 + so_lopchia;
		i_f++;
	}

}
void Mesh::Createcanhquat(float t)
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
	num_V = 8;
	pointt = new Point3[num_V];
	pointt[0].set(-0.5, x2, z2);
	pointt[1].set(0.5, x2, z2);
	pointt[2].set(0.5, 2, 0);
	pointt[3].set(-0.5, 2, 0);
	pointt[4].set(-0.5, x, z);
	pointt[5].set(0.5, x, z);
	pointt[6].set(0.5, 0, 0);
	pointt[7].set(-0.5, 0, 0);
	num_F = 6;
	face = new Face[num_F];

	face[0].n_Vert = 4;
	face[0].vert = new VertexID[4];
	face[0].vert[0].vdx = 1;
	face[0].vert[1].vdx = 5;
	face[0].vert[2].vdx = 6;
	face[0].vert[3].vdx = 2;
	for (i = 0; i < face[0].n_Vert; i++)
		face[0].vert[i].cdx = 0;

	face[1].n_Vert = 4;
	face[1].vert = new VertexID[4];
	face[1].vert[0].vdx = 0;
	face[1].vert[1].vdx = 3;
	face[1].vert[2].vdx = 7;
	face[1].vert[3].vdx = 4;
	for (i = 0; i < face[1].n_Vert; i++)
		face[1].vert[i].cdx = 1;

	face[2].n_Vert = 4;
	face[2].vert = new VertexID[4];
	face[2].vert[0].vdx = 0;
	face[2].vert[1].vdx = 1;
	face[2].vert[2].vdx = 2;
	face[2].vert[3].vdx = 3;
	for (i = 0; i < face[2].n_Vert; i++)
		face[2].vert[i].cdx = 2;

	face[3].n_Vert = 4;
	face[3].vert = new VertexID[4];
	face[3].vert[0].vdx = 7;
	face[3].vert[1].vdx = 6;
	face[3].vert[2].vdx = 5;
	face[3].vert[3].vdx = 4;
	for (i = 0; i < face[3].n_Vert; i++)
		face[3].vert[i].cdx = 3;

	face[4].n_Vert = 4;
	face[4].vert = new VertexID[4];
	face[4].vert[0].vdx = 4;
	face[4].vert[1].vdx = 5;
	face[4].vert[2].vdx = 1;
	face[4].vert[3].vdx = 0;
	for (i = 0; i < face[4].n_Vert; i++)
		face[4].vert[i].cdx = 4;

	face[5].n_Vert = 4;
	face[5].vert = new VertexID[4];
	face[5].vert[0].vdx = 3;
	face[5].vert[1].vdx = 2;
	face[5].vert[2].vdx = 6;
	face[5].vert[3].vdx = 7;
	for (i = 0; i < face[5].n_Vert; i++)
		face[5].vert[i].cdx = 5;

}
void Mesh::Createcanhquat2(float t)
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
	num_V = 8;
	pointt = new Point3[num_V];
	pointt[0].set(-0.5, x2, z2);
	pointt[1].set(0.5, x2, z2);
	pointt[2].set(0.5, 0, 0);
	pointt[3].set(-0.5, 0, 0);
	pointt[4].set(-0.5, x, z);
	pointt[5].set(0.5, x, z);
	pointt[6].set(0.5, -1, sqrt(3));
	pointt[7].set(-0.5, -1, sqrt(3));


	num_F = 6;
	face = new Face[num_F];

	
	face[0].n_Vert = 4;
	face[0].vert = new VertexID[4];
	face[0].vert[0].vdx = 1;
	face[0].vert[1].vdx = 5;
	face[0].vert[2].vdx = 6;
	face[0].vert[3].vdx = 2;
	for (i = 0; i < face[0].n_Vert; i++)
		face[0].vert[i].cdx = 0;

	face[1].n_Vert = 4;
	face[1].vert = new VertexID[4];
	face[1].vert[0].vdx = 0;
	face[1].vert[1].vdx = 3;
	face[1].vert[2].vdx = 7;
	face[1].vert[3].vdx = 4;
	for (i = 0; i < face[1].n_Vert; i++)
		face[1].vert[i].cdx = 1;

	face[2].n_Vert = 4;
	face[2].vert = new VertexID[4];
	face[2].vert[0].vdx = 0;
	face[2].vert[1].vdx = 1;
	face[2].vert[2].vdx = 2;
	face[2].vert[3].vdx = 3;
	for (i = 0; i < face[2].n_Vert; i++)
		face[2].vert[i].cdx = 2;

	face[3].n_Vert = 4;
	face[3].vert = new VertexID[4];
	face[3].vert[0].vdx = 7;
	face[3].vert[1].vdx = 6;
	face[3].vert[2].vdx = 5;
	face[3].vert[3].vdx = 4;
	for (i = 0; i < face[3].n_Vert; i++)
		face[3].vert[i].cdx = 3;

	face[4].n_Vert = 4;
	face[4].vert = new VertexID[4];
	face[4].vert[0].vdx = 4;
	face[4].vert[1].vdx = 5;
	face[4].vert[2].vdx = 1;
	face[4].vert[3].vdx = 0;
	for (i = 0; i < face[4].n_Vert; i++)
		face[4].vert[i].cdx = 4;

	face[5].n_Vert = 4;
	face[5].vert = new VertexID[4];
	face[5].vert[0].vdx = 3;
	face[5].vert[1].vdx = 2;
	face[5].vert[2].vdx = 6;
	face[5].vert[3].vdx = 7;
	for (i = 0; i < face[5].n_Vert; i++)
		face[5].vert[i].cdx = 5;

}
void Mesh::Createcanhquat3(float t)
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
	num_V = 8;
	pointt = new Point3[num_V];
	pointt[0].set(-0.5, x, z);
	pointt[1].set(0.5, x, z);
	pointt[2].set(0.5, 0, 0);
	pointt[3].set(-0.5, 0, 0);
	pointt[4].set(-0.5, x2, z2);
	pointt[5].set(0.5, x2, z2);
	pointt[6].set(0.5, 2, 0);
	pointt[7].set(-0.5, 2, 0);
	num_F = 6;
	face = new Face[num_F];
	face[0].n_Vert = 4;
	face[0].vert = new VertexID[4];
	face[0].vert[0].vdx = 1;
	face[0].vert[1].vdx = 5;
	face[0].vert[2].vdx = 6;
	face[0].vert[3].vdx = 2;
	for (i = 0; i < face[0].n_Vert; i++)
		face[0].vert[i].cdx = 0;
	face[1].n_Vert = 4;
	face[1].vert = new VertexID[4];
	face[1].vert[0].vdx = 4;
	face[1].vert[1].vdx = 0;
	face[1].vert[2].vdx = 3;
	face[1].vert[3].vdx = 7;
	for (i = 0; i < face[1].n_Vert; i++)
		face[1].vert[i].cdx = 1;

	face[2].n_Vert = 4;
	face[2].vert = new VertexID[4];
	face[2].vert[0].vdx = 0;
	face[2].vert[1].vdx = 1;
	face[2].vert[2].vdx = 2;
	face[2].vert[3].vdx = 3;
	for (i = 0; i < face[2].n_Vert; i++)
		face[2].vert[i].cdx = 2;
	face[3].n_Vert = 4;
	face[3].vert = new VertexID[4];
	face[3].vert[0].vdx = 7;
	face[3].vert[1].vdx = 6;
	face[3].vert[2].vdx = 5;
	face[3].vert[3].vdx = 4;
	for (i = 0; i < face[3].n_Vert; i++)
		face[3].vert[i].cdx = 3;

	face[4].n_Vert = 4;
	face[4].vert = new VertexID[4];
	face[4].vert[0].vdx = 0;
	face[4].vert[1].vdx = 4;
	face[4].vert[2].vdx = 5;
	face[4].vert[3].vdx = 1;
	for (i = 0; i < face[4].n_Vert; i++)
		face[4].vert[i].cdx = 4;

	
	face[5].n_Vert = 4;
	face[5].vert = new VertexID[4];
	face[5].vert[0].vdx = 3;
	face[5].vert[1].vdx = 2;
	face[5].vert[2].vdx = 6;
	face[5].vert[3].vdx = 7;
	for (i = 0; i < face[5].n_Vert; i++)
		face[5].vert[i].cdx = 5;

}
void Mesh::Createcanhquat4(float t)
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

	num_V = 8;
	pointt = new Point3[num_V];
	pointt[0].set(-0.5, x2, z2);
	pointt[1].set(0.5, x2, z2);
	pointt[2].set(0.5, 0, 0);
	pointt[3].set(-0.5, 0, 0);
	pointt[4].set(-0.5, x, z);
	pointt[5].set(0.5, x, z);
	pointt[6].set(0.5, -1, -sqrt(3));
	pointt[7].set(-0.5, -1, -sqrt(3));

	num_F = 6;
	face = new Face[num_F];
	face[0].n_Vert = 4;
	face[0].vert = new VertexID[4];
	face[0].vert[0].vdx = 1;
	face[0].vert[1].vdx = 2;
	face[0].vert[2].vdx = 6;
	face[0].vert[3].vdx = 5;
	for (i = 0; i < face[0].n_Vert; i++)
		face[0].vert[i].cdx = 0;

	face[1].n_Vert = 4;
	face[1].vert = new VertexID[4];
	face[1].vert[0].vdx = 0;
	face[1].vert[1].vdx = 4;
	face[1].vert[2].vdx = 7;
	face[1].vert[3].vdx = 3;
	for (i = 0; i < face[1].n_Vert; i++)
		face[1].vert[i].cdx = 1;

	face[2].n_Vert = 4;
	face[2].vert = new VertexID[4];
	face[2].vert[0].vdx = 0;
	face[2].vert[1].vdx = 1;
	face[2].vert[2].vdx = 2;
	face[2].vert[3].vdx = 3;
	for (i = 0; i < face[2].n_Vert; i++)
		face[2].vert[i].cdx = 2;

	face[3].n_Vert = 4;
	face[3].vert = new VertexID[4];
	face[3].vert[0].vdx = 7;
	face[3].vert[1].vdx = 4;
	face[3].vert[2].vdx = 5;
	face[3].vert[3].vdx = 6;
	for (i = 0; i < face[3].n_Vert; i++)
		face[3].vert[i].cdx = 3;

	face[4].n_Vert = 4;
	face[4].vert = new VertexID[4];
	face[4].vert[0].vdx = 0;
	face[4].vert[1].vdx = 4;
	face[4].vert[2].vdx = 5;
	face[4].vert[3].vdx = 1;
	for (i = 0; i < face[4].n_Vert; i++)
		face[4].vert[i].cdx = 4;

	face[5].n_Vert = 4;
	face[5].vert = new VertexID[4];
	face[5].vert[0].vdx = 3;
	face[5].vert[1].vdx = 2;
	face[5].vert[2].vdx = 6;
	face[5].vert[3].vdx = 7;
	for (i = 0; i < face[5].n_Vert; i++)
		face[5].vert[i].cdx = 5;

}
void Mesh::Createcanhquat5(float t)
{
	float bankinh = t;

	float x = bankinh * -sin(15.0 * PI / 180) - 5.0;
	float z = bankinh * -cos(15.0 * PI / 180);


	int i;

	num_V = 8;
	pointt = new Point3[num_V];
	pointt[0].set(-0.5, -5.0, 0);
	pointt[1].set(0.5, -5.0, 0);
	pointt[2].set(0.5, 0, 0);
	pointt[3].set(-0.5, 0, 0);
	pointt[4].set(-0.5, x, z);
	pointt[5].set(0.5, x, z);
	pointt[6].set(0.5, -1, -sqrt(3));
	pointt[7].set(-0.5, -1, -sqrt(3));


	num_F = 6;
	face = new Face[num_F];

	face[0].n_Vert = 4;
	face[0].vert = new VertexID[4];
	face[0].vert[0].vdx = 1;
	face[0].vert[1].vdx = 5;
	face[0].vert[2].vdx = 6;
	face[0].vert[3].vdx = 2;
	for (i = 0; i < face[0].n_Vert; i++)
		face[0].vert[i].cdx = 0;
	face[1].n_Vert = 4;
	face[1].vert = new VertexID[4];
	face[1].vert[0].vdx = 0;
	face[1].vert[1].vdx = 3;
	face[1].vert[2].vdx = 7;
	face[1].vert[3].vdx = 4;
	for (i = 0; i < face[1].n_Vert; i++)
		face[1].vert[i].cdx = 1;
	face[2].n_Vert = 4;
	face[2].vert = new VertexID[4];
	face[2].vert[0].vdx = 0;
	face[2].vert[1].vdx = 1;
	face[2].vert[2].vdx = 2;
	face[2].vert[3].vdx = 3;
	for (i = 0; i < face[2].n_Vert; i++)
		face[2].vert[i].cdx = 2;
	face[3].n_Vert = 4;
	face[3].vert = new VertexID[4];
	face[3].vert[0].vdx = 7;
	face[3].vert[1].vdx = 6;
	face[3].vert[2].vdx = 5;
	face[3].vert[3].vdx = 4;
	for (i = 0; i < face[3].n_Vert; i++)
		face[3].vert[i].cdx = 3;

	face[4].n_Vert = 4;
	face[4].vert = new VertexID[4];
	face[4].vert[0].vdx = 0;
	face[4].vert[1].vdx = 4;
	face[4].vert[2].vdx = 5;
	face[4].vert[3].vdx = 1;
	for (i = 0; i < face[4].n_Vert; i++)
		face[4].vert[i].cdx = 4;
	face[5].n_Vert = 4;
	face[5].vert = new VertexID[4];
	face[5].vert[0].vdx = 3;
	face[5].vert[1].vdx = 2;
	face[5].vert[2].vdx = 6;
	face[5].vert[3].vdx = 7;
	for (i = 0; i < face[5].n_Vert; i++)
		face[5].vert[i].cdx = 5;

}
void Mesh::Createcanhquat6(float t)
{
	float bankinh = t;

	float x = bankinh * -sin(165.0 * PI / 180) - 5.0;
	float z = bankinh * -cos(165.0 * PI / 180);


	int i;

	num_V = 8;
	pointt = new Point3[num_V];
	pointt[0].set(-0.5, -5.0, 0);
	pointt[1].set(0.5, -5.0, 0);
	pointt[2].set(0.5, 0, 0);
	pointt[3].set(-0.5, 0, 0);
	pointt[4].set(-0.5, x, z);
	pointt[5].set(0.5, x, z);
	pointt[6].set(0.5, -1, sqrt(3));
	pointt[7].set(-0.5, -1, sqrt(3));


	num_F = 6;
	face = new Face[num_F];

	face[0].n_Vert = 4;
	face[0].vert = new VertexID[4];
	face[0].vert[0].vdx = 1;
	face[0].vert[1].vdx = 2;
	face[0].vert[2].vdx = 6;
	face[0].vert[3].vdx = 5;
	for (i = 0; i < face[0].n_Vert; i++)
		face[0].vert[i].cdx = 0;

	face[1].n_Vert = 4;
	face[1].vert = new VertexID[4];
	face[1].vert[0].vdx = 0;
	face[1].vert[1].vdx = 4;
	face[1].vert[2].vdx = 7;
	face[1].vert[3].vdx = 3;
	for (i = 0; i < face[1].n_Vert; i++)
		face[1].vert[i].cdx = 1;
	face[2].n_Vert = 4;
	face[2].vert = new VertexID[4];
	face[2].vert[0].vdx = 0;
	face[2].vert[1].vdx = 1;
	face[2].vert[2].vdx = 2;
	face[2].vert[3].vdx = 3;
	for (i = 0; i < face[2].n_Vert; i++)
		face[2].vert[i].cdx = 2;
	face[3].n_Vert = 4;
	face[3].vert = new VertexID[4];
	face[3].vert[0].vdx = 7;
	face[3].vert[1].vdx = 4;
	face[3].vert[2].vdx = 5;
	face[3].vert[3].vdx = 6;
	for (i = 0; i < face[3].n_Vert; i++)
		face[3].vert[i].cdx = 3;
	face[4].n_Vert = 4;
	face[4].vert = new VertexID[4];
	face[4].vert[0].vdx = 0;
	face[4].vert[1].vdx = 4;
	face[4].vert[2].vdx = 5;
	face[4].vert[3].vdx = 1;
	for (i = 0; i < face[4].n_Vert; i++)
		face[4].vert[i].cdx = 4;

	face[5].n_Vert = 4;
	face[5].vert = new VertexID[4];
	face[5].vert[0].vdx = 3;
	face[5].vert[1].vdx = 2;
	face[5].vert[2].vdx = 6;
	face[5].vert[3].vdx = 7;
	for (i = 0; i < face[5].n_Vert; i++)
		face[5].vert[i].cdx = 5;

}
void Mesh::CreateMotPhanThanhLienKet(float chieu_dai, float chieu_rong, float beday)
{
	num_V = 8;
	num_F = 6;
	pointt = new Point3[num_V];
	face = new Face[num_F];
	int i_f = 0;
	int indexColor = 1;
	{
		pointt[0].set(0, 0, 0); 
		pointt[1].set(0, 0, chieu_dai); 
		pointt[2].set(chieu_rong, 0, chieu_dai); 
		pointt[3].set(chieu_rong, 0, 0); 
		pointt[4].set(0, beday, 0); 
		pointt[5].set(0, beday, chieu_dai); 
		pointt[6].set(chieu_rong, beday, chieu_dai); 
		pointt[7].set(chieu_rong, beday, 0);
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = 0;
		face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = 2;
		face[i_f].vert[3].vdx = 3;
		for (int i = 0; i < face[i_f].n_Vert; i++)
			face[i_f].vert[i].cdx = indexColor;
		i_f++;
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = 4;
		face[i_f].vert[1].vdx = 5;
		face[i_f].vert[2].vdx = 6;
		face[i_f].vert[3].vdx = 7;
		for (int i = 0; i < face[i_f].n_Vert; i++)
			face[i_f].vert[i].cdx = indexColor;
		i_f++;
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = 4;
		face[i_f].vert[1].vdx = 0;
		face[i_f].vert[2].vdx = 3;
		face[i_f].vert[3].vdx = 7;
		for (int i = 0; i < face[i_f].n_Vert; i++)
			face[i_f].vert[i].cdx = indexColor;
		i_f++;

		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = 1;
		face[i_f].vert[1].vdx = 2;
		face[i_f].vert[2].vdx = 6;
		face[i_f].vert[3].vdx = 5;
		for (int i = 0; i < face[i_f].n_Vert; i++)
			face[i_f].vert[i].cdx = indexColor;
		i_f++;
		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = 2;
		face[i_f].vert[1].vdx = 3;
		face[i_f].vert[2].vdx = 7;
		face[i_f].vert[3].vdx = 6;
		for (int i = 0; i < face[i_f].n_Vert; i++)
			face[i_f].vert[i].cdx = indexColor;
		i_f++;

		face[i_f].n_Vert = 4;
		face[i_f].vert = new VertexID[face[i_f].n_Vert];
		face[i_f].vert[0].vdx = 0;
		face[i_f].vert[1].vdx = 1;
		face[i_f].vert[2].vdx = 5;
		face[i_f].vert[3].vdx = 4;
		for (int i = 0; i < face[i_f].n_Vert; i++)
			face[i_f].vert[i].cdx = indexColor;
		i_f++;
	}
}
void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < num_F; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].n_Vert; v++)
		{
			int		iv = face[f].vert[v].vdx;
			int ic = face[f].vert[v].cdx;
			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glVertex3f(pointt[iv].x, pointt[iv].y, pointt[iv].z);
		}
		glEnd();
	}
}
void Mesh::DrawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < num_F; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].n_Vert; v++)
		{
			int iv = face[f].vert[v].vdx;
			int ic = face[f].vert[v].cdx;
			
			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glNormal3f(face[f].vN.x, face[f].vN.y, face[f].vN.z);
			glVertex3f(pointt[iv].x, pointt[iv].y, pointt[iv].z);
		
		
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
void Mesh::v_new()
{
	for (int f = 0; f < num_F; f++)
	{
		float vectorN[] = { 0,0,0 };
		for (int v = 0; v < face[f].n_Vert; v++)
		{
			int iv = face[f].vert[v].vdx;
			int next = face[f].vert[(v + 1) % face[f].n_Vert].vdx;
			vectorN[0] += (pointt[iv].y - pointt[next].y) * (pointt[iv].z + pointt[next].z);
			vectorN[1] += (pointt[iv].z - pointt[next].z) * (pointt[iv].x + pointt[next].x);
			vectorN[2] += (pointt[iv].x - pointt[next].x) * (pointt[iv].y + pointt[next].y);
		}
		face[f].vN.set(vectorN[0], vectorN[1], vectorN[2]);
		face[f].vN.chuanhoa();
	}
}
void Mesh::SetColor(int i_color)
{
	for (int f = 0; f < num_F; f++)
	{
		for (int v = 0; v < face[f].n_Vert; v++)
		{
			face[f].vert[v].cdx = i_color;
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
Mesh canhquat1;
Mesh canhquat2;
Mesh canhquat3;
Mesh canhquat4;
Mesh canhquat5;
Mesh canhquat6;

Mesh nuavongtrong1_1;
Mesh nuavongtrong1_2;
Mesh nuavongtrong1_3;
Mesh nuavongtrongthangiua;
Mesh nuavongtrongthangiua2;
Mesh nuavongtrongthangiua3;
Mesh nuavongtrongthangiua4;
Mesh daytruc;
Mesh thantruc;
Mesh goido;
Mesh viengach;


void CreateMotVietGach(float bankinh,float chieucao) {
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

	
	float bankinh2 = bankinh/(cos(15.0*PI/180.0)- sin(15.0 * PI / 180.0)* tan(60.0 * PI / 180.0));
	int so_lopchia =6;
	int so_lopchia2 = 12;
	Point3* pointt_gach = new Point3[so_lopchia+1];
	Point3* pointt_gach2 = new Point3[so_lopchia2 + 1];

	int		i;
	int		i_f;
	float	do_quay = (2*PI) / (so_lopchia );
	float	do_quay2 = (2 * PI) / (so_lopchia2);
	float	x, y, z;
	float a = -15.0 * PI / 180.0;
	
	for (i = 0; i < so_lopchia; i++)
	{
		x = bankinh * sin(do_quay * i );
		z = bankinh * cos(do_quay * i );
		
		y = chieucao;
		
		pointt_gach[i].set(x, y, z);

		
	}
	pointt_gach[so_lopchia ].set(pointt_gach[0].x, pointt_gach[0].y,pointt_gach[0].z);


	for (i = 0; i < so_lopchia2; i++)
	{
		x = bankinh2 * sin(do_quay2 * i+a);
		z = bankinh2 * cos(do_quay2 * i+a);

		y = chieucao;

		pointt_gach2[i].set(x, y, z);


	}
	pointt_gach2[so_lopchia2].set(pointt_gach2[0].x, pointt_gach2[0].y, pointt_gach2[0].z);


	glNormal3f(1.0f, 1.0f, 0.0f);
	glColor3f(1.0, 0.0, 1.0);
	
	
	glBegin(GL_POLYGON);
	
	for (i = 0; i < so_lopchia+1; i++)
	{
		glVertex3f(pointt_gach[i].x,pointt_gach[i].y,pointt_gach[i].z);
	
	}
	glEnd();

	glClearColor(0, 0, 0, 0);
	
	glColor3f(255.0 / 255.0, 128.0 / 255.0, 0.0);
	for (i = 0; i < so_lopchia; i++)
	{
		glBegin(GL_POLYGON);
		glVertex3f(pointt_gach[i].x, pointt_gach[i].y, pointt_gach[i].z);
		glVertex3f(pointt_gach2[i*2].x, pointt_gach2[i*2].y, pointt_gach2[i*2].z);
		glVertex3f(pointt_gach2[i*2+1].x, pointt_gach2[i*2+1].y, pointt_gach2[i*2+1].z);
		glVertex3f(pointt_gach[i].x, pointt_gach[i].y, pointt_gach[i].z);
		glEnd();

	}
	glClearColor(0, 0, 0, 0);
	glColor3f(0, 255.0 / 255.0, 64.0/255.0);

	for (i = 0; i < so_lopchia; i++)
	{	
		
		glBegin(GL_POLYGON);
		glVertex3f(pointt_gach[i].x, pointt_gach[i].y, pointt_gach[i].z);
		glVertex3f(pointt_gach2[i * 2+1].x, pointt_gach2[i *2+1].y, pointt_gach2[i *2 +1].z);
	
		glVertex3f(pointt_gach2[i * 2 + 2].x, pointt_gach2[i * 2 + 1].y, pointt_gach2[i * 2 + 2].z);
		glVertex3f(pointt_gach[i + 1].x, pointt_gach[i + 1].y, pointt_gach[i + 1].z);
		glEnd();

	}


}
void DrawNen() {
	glDisable(GL_LIGHTING);
	float bankinh = 2.0;
	int ttt = 15;
	
	float chieucao = -12.0;
	float bankinh2 = bankinh / (cos(15.0 * PI / 180.0) - sin(15.0 * PI / 180.0) * tan(60.0 * PI / 180.0));
	float dx = bankinh2 * cos(15.0 * PI / 180.0)+bankinh * cos(30.0 * PI / 180.0);
	float dz = bankinh2 * cos(15.0 * PI / 180.0) * 2 + bankinh;
	for (int j = -ttt; j < ttt; j++) {
		for (int n = -ttt; n <ttt; n++) {
			glPushMatrix();
			glTranslatef(dx*j,0,dz*n);
			//glRotatef()
			CreateMotVietGach(bankinh, chieucao);
			glPopMatrix();
		}
	}

	for (int j = -ttt; j < ttt; j++) {
		for (int n = -ttt; n < ttt; n++) {
			glPushMatrix();	
			glTranslatef(dx/2.0, 0, dx*sin(60.0*PI/180.0));
			glTranslatef(dx * j, 0, dz*n);
			//glRotatef()
			CreateMotVietGach( bankinh, chieucao);
			glPopMatrix();
		}
	}
	glEnable(GL_LIGHTING);
}
float* v_new(float iVector[10][3], int n)
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


	if (View_Color == 0)
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


	if (View_Color == 0)
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
		glTranslatef(-1, 0, -t / 2);

		rectangular2.DrawColor();

		glTranslatef(1, 0, t / 2);
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
	if (View_Color == 0)
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
		glTranslatef(-1, 0, -t / 2);

		rectangular2.DrawColor();

		glTranslatef(1, 0, t / 2);
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

	if (View_Color == 0)
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


	if (View_Color == 0)
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


	if (View_Color == 0)
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
void DrawCanhQuat1()
{





	int mau = 0;

	canhquat1.SetColor(mau);
	canhquat2.SetColor(mau);
	canhquat3.SetColor(mau);
	canhquat4.SetColor(mau);
	canhquat5.SetColor(mau);
	canhquat6.SetColor(mau);

	nuavongtrong1_1.SetColor(mau);
	nuavongtrong1_2.SetColor(mau);
	nuavongtrong1_3.SetColor(mau);


	glPushMatrix();

	glTranslatef(0, 0, 0);
	glRotatef(m, 1, 0, 0);
	glRotatef(90, 0, 0, 1);


	if (View_Color == 0)
	{
		nuavongtrong1_3.DrawWireframe();
		nuavongtrong1_2.DrawWireframe();
		nuavongtrong1_1.DrawWireframe();
		glRotatef(-90, 0, 0, 1);
		canhquat1.DrawWireframe();
		canhquat2.DrawWireframe();
		canhquat3.DrawWireframe();
		canhquat4.DrawWireframe();
		canhquat5.DrawWireframe();
		canhquat6.DrawWireframe();

	}
	else
	{
		nuavongtrong1_3.DrawColor();
		nuavongtrong1_2.DrawColor();
		nuavongtrong1_1.DrawColor();
		glRotatef(-90, 0, 0, 1);
		canhquat1.DrawColor();
		canhquat2.DrawColor();
	
		
		canhquat3.DrawColor();
		canhquat4.DrawColor();
		canhquat5.DrawColor();
		canhquat6.DrawColor();
	}


	glPopMatrix();
}
void DrawCanhQuat2() {


	int mau = 1;

	canhquat1.SetColor(mau);
	canhquat2.SetColor(mau);
	canhquat3.SetColor(mau);
	canhquat4.SetColor(mau);
	canhquat5.SetColor(mau);
	canhquat6.SetColor(mau);

	nuavongtrong1_1.SetColor(mau);
	nuavongtrong1_2.SetColor(mau);
	nuavongtrong1_3.SetColor(mau);


	glPushMatrix();

	glTranslatef(-2, -sqrt(12.5), -sqrt(12.5));
	glRotatef(m, 1, 0, 0);


	glRotatef(90, 0, 0, 1);

	if (View_Color == 0)
	{
		nuavongtrong1_3.DrawWireframe();
		nuavongtrong1_2.DrawWireframe();
		nuavongtrong1_1.DrawWireframe();
		glRotatef(-90, 0, 0, 1);
		canhquat1.DrawWireframe();
		canhquat2.DrawWireframe();
		canhquat3.DrawWireframe();
		canhquat4.DrawWireframe();
		canhquat5.DrawWireframe();
		canhquat6.DrawWireframe();

	}
	else
	{
		nuavongtrong1_3.DrawColor();
		nuavongtrong1_2.DrawColor();
		nuavongtrong1_1.DrawColor();
		glRotatef(-90, 0, 0, 1);
		canhquat1.DrawColor();
		canhquat2.DrawColor();
		canhquat3.DrawColor();
		canhquat4.DrawColor();
		canhquat5.DrawColor();
		canhquat6.DrawColor();
	}


	glPopMatrix();




}
void DrawCanhQuat3() {
	int mau = 2;
	
	canhquat1.SetColor(mau);
	canhquat2.SetColor(mau);
	canhquat3.SetColor(mau);
	canhquat4.SetColor(mau);
	canhquat5.SetColor(mau);
	canhquat6.SetColor(mau);

	nuavongtrong1_1.SetColor(mau);
	nuavongtrong1_2.SetColor(mau);
	nuavongtrong1_3.SetColor(mau);




	glPushMatrix();

	glTranslatef(-2 - 2, -sqrt(12.5) + 6.0 * cos(15.0 * PI / 180.0), -sqrt(12.5) + 6.0 * sin(15.0 * PI / 180.0));
	glRotatef(m, 1, 0, 0);


	glRotatef(90, 0, 0, 1);
	
	if (View_Color == 0)
	{
		nuavongtrong1_3.DrawWireframe();
		nuavongtrong1_2.DrawWireframe();
		nuavongtrong1_1.DrawWireframe();
		glRotatef(-90, 0, 0, 1);
		canhquat1.DrawWireframe();
		canhquat2.DrawWireframe();
		canhquat3.DrawWireframe();
		canhquat4.DrawWireframe();
		canhquat5.DrawWireframe();
		canhquat6.DrawWireframe();

	}
	else
	{
		nuavongtrong1_3.DrawColor();
		nuavongtrong1_2.DrawColor();
		nuavongtrong1_1.DrawColor();
		glRotatef(-90, 0, 0, 1);
		canhquat1.DrawColor();
		canhquat2.DrawColor();
		canhquat3.DrawColor();
		canhquat4.DrawColor();
		canhquat5.DrawColor();
		canhquat6.DrawColor();
	}



	glPopMatrix();




}
void DrawTruc1() {
	glPushMatrix();
	
	float pos_goido = 0.5;
	glRotatef(90, 0, 0, 1);
	glRotatef(-m, 0, 1, 0);
	glTranslatef(0, -1.5, 0);
	if (View_Color == 0)
	{	
		daytruc.DrawWireframe();
		glTranslatef(0, -pos_goido, 0);
		glRotatef(m, 0, 1, 0);
		goido.DrawWireframe();
		glRotatef(-m, 0, 1, 0);
		glTranslatef(0, pos_goido, 0);
		glTranslatef(0, -1.5, 0);
		thantruc.DrawWireframe();
	}
	else
	{
		daytruc.DrawColor();
		glTranslatef(0, -pos_goido, 0);
		glRotatef(m, 0, 1, 0);
		goido.DrawColor();
		glRotatef(-m, 0, 1, 0);
		glTranslatef(0, pos_goido, 0);
		glTranslatef(0, -1.5, 0);
		thantruc.DrawColor();
	}
	

	glPopMatrix();

}
void DrawTruc2() {
	glPushMatrix();

	float pos_goido = 0.5;
	glTranslatef(-2 - 2, -sqrt(12.5) + 6.0 * cos(15.0 * PI / 180.0), -sqrt(12.5) + 6.0 * sin(15.0 * PI / 180.0));
	glRotatef(180, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glRotatef(m, 0, 1, 0);
	glTranslatef(0, -1.5, 0);
	if (View_Color == 0)
	{
		daytruc.DrawWireframe();
		glTranslatef(0, -pos_goido, 0);
		glRotatef(-m, 0, 1, 0);
		goido.DrawWireframe();
		glRotatef(m, 0, 1, 0);
		glTranslatef(0, pos_goido, 0);
		glTranslatef(0, -1.5, 0);
		thantruc.DrawWireframe();
	}
	else
	{
		daytruc.DrawColor();
		glTranslatef(0, -pos_goido, 0);
		glRotatef(-m, 0, 1, 0);
		goido.DrawColor();
		glRotatef(m, 0, 1, 0);
		glTranslatef(0, pos_goido, 0);
		glTranslatef(0, -1.5, 0);
		thantruc.DrawColor();
	}


	glPopMatrix();

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
	DrawTruc1();
	DrawTruc2();
	DrawNen();
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
	glFlush();
	glutSwapBuffers();
}
void myInit()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
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
	


	glOrtho(-size_vieww * 2, size_vieww * 2, -size_vieww / 2, size_vieww + size_vieww / 2, -1000, 1000);

}
void updateCam()
{
	camera[0] = bankinh_camera * cos((PI / 180.0) * doxoay_camera);
	camera[1] = camera[1];
	camera[2] = bankinh_camera * sin((PI / 180.0) * doxoay_camera);
}
void OnKeySpecial(int key, int x, int y)
{
	if (state2D==true) {
		return;
	}
	switch (key)
	{

	case GLUT_KEY_RIGHT:
	{
		doxoay_camera--;
		if (doxoay_camera < 0) doxoay_camera = 360.0f;
		updateCam();
		myInit();
		break;
	}
	case GLUT_KEY_LEFT:
	{
		
		doxoay_camera++;
		if (doxoay_camera > 360) doxoay_camera = doxoay_camera - 360.0f;
		updateCam();
		myInit();
		break;
	}
	
	
	case GLUT_KEY_DOWN:
	{
		if (size_vieww >= size_vieww_bandau / 2 && camera[1] >= 0)
		{
			size_vieww -= 0.1;
			camera[1] -= 0.1;
		}
		else
		{
			size_vieww += 0.1;
			camera[1] -= 0.1;
		}
		myInit();
		break;
	}


	case GLUT_KEY_UP:
	{
		if (size_vieww >= size_vieww_bandau / 2 && camera[1] < 0)
		{
			size_vieww -= 0.1;
			camera[1] += 0.1;
		}
		else
		{
			size_vieww += 0.1;
			camera[1] += 0.1;
		}
		myInit();
		break;
	}


	default:
		break;
	}
	
	glutPostRedisplay();
}
void OnKeyBoard(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		
		View_Color = (View_Color + 1) % 2;
	}

	
	else if (key == 'v' || key == 'V')
	{
		
		if (state2D ==true)
		{
			state2D = false;
			camera[0] = luutrangthai_camera[0];
			camera[1] = luutrangthai_camera[1];
			camera[2] = luutrangthai_camera[2];
		
		}
		else
		{

			state2D = true;
			luutrangthai_camera[0] = camera[0];
			luutrangthai_camera[1] = camera[1];
			luutrangthai_camera[2] = camera[2];
			luutrangthai_camera[3] = 0;
			luutrangthai_camera[4] = 90;
			luutrangthai_camera[5] =1;
			camera[0] = luutrangthai_camera[3];
			camera[1] = luutrangthai_camera[4];
			camera[2] = luutrangthai_camera[5];
		
		}
	}
	
	else if (key == '+' && state2D == false)
	{
		if (bankinh_camera < 0)
		{
			bankinh_camera -= 0.1;
			size_vieww += 0.1;
		}
		else
		{
			bankinh_camera -= 0.1;
			size_vieww -= 0.1;
		}
		updateCam();


		myInit();
	}

	else if (key == '-' && state2D ==false)
	{
		if (bankinh_camera >= 0)
		{
			bankinh_camera += 0.1;
			size_vieww += 0.1;
		}
		else
		{
			bankinh_camera += 0.1;
			size_vieww -= 0.1;
		}
		updateCam();
	
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
	myInit();
	glutPostRedisplay();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight); 
	glutInitWindowPosition(0, 0); 
	glutCreateWindow("Doan Quang Chinh 1710685"); 

	cout << "1:		 Xoay canh quat nguoc chieu kim dong ho" << endl;
	cout << "2:		 Xoay canh quat cung chieu kim dong ho" << endl;
	
	cout << "W, w  : Chuyen doi qua lai giua che do khung day va to mau" << endl;
	cout << "V, v  : Chuyen doi che do " << endl;
	
	cout << "+   : Tang khoang cach camera" << endl;
	cout << "-   : Giam khoang cach camera" << endl;
	cout << "up arrow   : Tang chieu cao camera" << endl;
	cout << "down arrow : Giam chieu cao camera" << endl;
	cout << "<-         : Quay camera theo chieu kim dong ho" << endl;
	cout << "->         : Quay camera nguoc chieu kim dong ho" << endl;
	
	camera[0] = bankinh_camera * cos((PI / 180.0) * (doxoay_camera));
	camera[1] = chieu_cao_camera;
	camera[2] = bankinh_camera * sin((PI / 180.0) * (doxoay_camera ));
	state2D = false;
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	float size_canhquat = 1.1;
	canhquat1.Createcanhquat(size_canhquat);
	canhquat1.v_new();
	canhquat1.addMaterial(ambient, diffuse, specular, shininess);
	canhquat2.Createcanhquat2(size_canhquat);
	canhquat2.v_new();
	canhquat2.addMaterial(ambient, diffuse, specular, shininess);
	canhquat3.Createcanhquat3(size_canhquat);
	canhquat3.v_new();
	canhquat3.addMaterial(ambient, diffuse, specular, shininess);
	canhquat4.Createcanhquat4(size_canhquat);
	canhquat4.v_new();
	canhquat4.addMaterial(ambient, diffuse, specular, shininess);
	canhquat5.Createcanhquat5(size_canhquat);
	canhquat5.v_new();
	canhquat5.addMaterial(ambient, diffuse, specular, shininess);
	canhquat6.Createcanhquat6(size_canhquat);
	canhquat6.v_new();
	canhquat6.addMaterial(ambient, diffuse, specular, shininess);
	
	nuavongtrong1_1.CreateNuaVongTron(size_canhquat, 2, 1);
	nuavongtrong1_1.v_new();
	nuavongtrong1_1.addMaterial(ambient, diffuse, specular, shininess);
	
	nuavongtrong1_2.CreateNuaVongTron2(size_canhquat, 2, 1);
	nuavongtrong1_2.v_new();
	nuavongtrong1_2.addMaterial(ambient, diffuse, specular, shininess);
	
	nuavongtrong1_3.CreateNuaVongTron3(size_canhquat, 2, 1);
	nuavongtrong1_3.v_new();
	nuavongtrong1_3.addMaterial(ambient, diffuse, specular, shininess);

	float size_thanhlienket1 = 5.0;
	float size_thanhlienket2 = 6.0;
	int mau_thanhlienket = 3;
	rectangular1.CreateMotPhanThanhLienKet(0.4, 1, size_thanhlienket1);
	rectangular1.SetColor(mau_thanhlienket);
	rectangular1.v_new();
	rectangular1.addMaterial(ambient, diffuse, specular, shininess);
	rectangular2.CreateMotPhanThanhLienKet(0.4, 1, size_thanhlienket1);
	rectangular2.SetColor(mau_thanhlienket);
	rectangular2.v_new();
	rectangular2.addMaterial(ambient, diffuse, specular, shininess);
	rectangular3.CreateMotPhanThanhLienKet(0.4, 1, size_thanhlienket2);
	rectangular3.SetColor(mau_thanhlienket);
	rectangular3.v_new();
	rectangular3.addMaterial(ambient, diffuse, specular, shininess);
	rectangular4.CreateMotPhanThanhLienKet(0.4, 1, size_thanhlienket2);
	rectangular4.SetColor(mau_thanhlienket);
	rectangular4.v_new();
	rectangular4.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua.CreateNuaVongTronThanGiua(0.6, 0.4, 1, 1);
	nuavongtrongthangiua.SetColor(mau_thanhlienket);
	nuavongtrongthangiua.v_new();
	nuavongtrongthangiua.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua2.CreateNuaVongTronThanGiua(0.6, 0.4, 1, -1);
	nuavongtrongthangiua2.SetColor(mau_thanhlienket);
	nuavongtrongthangiua2.v_new();
	nuavongtrongthangiua2.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua3.CreateNuaVongTronThanGiua2(0.6, size_thanhlienket1, 1, -1);
	nuavongtrongthangiua3.SetColor(mau_thanhlienket);
	nuavongtrongthangiua3.v_new();
	nuavongtrongthangiua3.addMaterial(ambient, diffuse, specular, shininess);
	nuavongtrongthangiua4.CreateNuaVongTronThanGiua2(0.6, size_thanhlienket2, 1, -1);
	nuavongtrongthangiua4.SetColor(mau_thanhlienket);
	nuavongtrongthangiua4.v_new();
	nuavongtrongthangiua4.addMaterial(ambient, diffuse, specular, shininess);

	int mau_chot = 4;
	chot1.CreateHinhTru(32, 1, 0.6);
	chot1.SetColor(mau_chot);
	chot1.v_new();
	chot1.addMaterial(ambient, diffuse, specular, shininess);
	chot2.CreateHinhTru(32, 1, 0.6);
	chot2.SetColor(mau_chot);
	chot2.v_new();
	chot2.addMaterial(ambient, diffuse, specular, shininess);
	int mau_truc = 5;
	daytruc.CreateHinhTru(32,2,0.6);
	daytruc.SetColor(mau_truc);
	daytruc.v_new();
	daytruc.addMaterial(ambient, diffuse, specular, shininess);

	thantruc.CreateHinhTru(8, 1.5, 0.5);
	thantruc.SetColor(mau_truc);
	thantruc.v_new();
	thantruc.addMaterial(ambient, diffuse, specular, shininess);
	int mau_goido = 6;

	goido.CreateHinhTru(8, 0.8, 0.8);
	goido.SetColor(mau_goido);
	goido.v_new();
	goido.addMaterial(ambient, diffuse, specular, shininess);
	glutKeyboardFunc(OnKeyBoard);
	glutSpecialFunc(OnKeySpecial);
	myInit();
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	
	return 0;
}