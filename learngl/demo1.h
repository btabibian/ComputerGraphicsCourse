#ifndef _rt_H
#define _rt_H

#include <cmath>
#include <vector>
#include <GL/glut.h>

using namespace std;

//
// Sample code for physics simulation
//


// Implements cloth simulation


class Vector3f;
class Triangle;
class TriangleMesh;


class Vector3f {

	float _item[3];

	public:

	float & operator [] (int i) {
		return _item[i];
    	}

	Vector3f(float x, float y, float z) 
	{  _item[0] = x ; _item[1] = y ; _item[2] = z; };

	Vector3f() {};


	Vector3f & operator = (Vector3f & obj) 
	{
		_item[0] = obj[0];
		_item[1] = obj[1];
		_item[2] = obj[2];

		return *this;
	};

	Vector3f & operator += (Vector3f & obj) 
	{
		_item[0] += obj[0];
		_item[1] += obj[1];
		_item[2] += obj[2];

		return *this;
	};
};

ostream & operator << (ostream & stream, Vector3f & obj) 
{
	return stream << obj[0] << ' ' << obj[1] << ' ' << obj[2] << ' ';
};




class Triangle {
friend class TriangleMesh;

	int _vertex[3];
public:

	Triangle(int v1, int v2, int v3) 
	{
		_vertex[0] = v1;  _vertex[1] = v2;  _vertex[2] = v3;  
		
	};
};

float fmax(float f1,float f2, float f3) {
	float f = f1;

	if (f < f2) f = f2;
	if (f < f3) f = f3;

	return f;
};

float fmin(float f1,float f2, float f3) {
	float f = f1;

	if (f > f2) f = f2;
	if (f > f3) f = f3;

	return f;
};


class TriangleMesh 
{
	vector <Vector3f> _v;
	vector <Triangle> _trig;
	float _xmax, _xmin, _ymax, _ymin, _zmin, _zmax;
private:
	void drawLineDDA(Vector3f & v1,Vector3f & v2)
	{
		int i=0;
		float x1=v1[0];
		float x2=v2[0];
		float y1=v1[1];
		float y2=v2[1];
		float dx=x2-x1;
		float dy=y2-y1;
		float n=max(abs(dx),abs(dy));
		float dt=n;
		float dxdt=dx/dt;
		float dydt=dy/dt;
		glBegin(GL_POINTS);
		float x=x1;
		float y=y1;
		for(;n>0;n--)
		{
		glVertex2f(x,y);
		x+=dxdt;
		y+=dydt;
		}
		glEnd();
	}
public: 
	TriangleMesh(char * filename) { loadFile(filename) ;};
	TriangleMesh() {};
	void loadFile(char * filename);

	int trigNum() { return _trig.size() ;};
	int vNum() { return _v.size();};
	Vector3f v(int i) { return _v[i];};

	void getTriangleVertices(int i, Vector3f & v1, Vector3f & v2, Vector3f & v3)
	{
		v1 = _v[_trig[i]._vertex[0]]; 
		v2 = _v[_trig[i]._vertex[1]]; 
		v3 = _v[_trig[i]._vertex[2]]; 
	}
	
	void drawTriangleVertices(int i)
	{
		Vector3f v1,v2,v3;
		v1 = _v[_trig[i]._vertex[0]]; 
		v2 = _v[_trig[i]._vertex[1]]; 
		v3 = _v[_trig[i]._vertex[2]];
		drawLineDDA(v1,v2);
		drawLineDDA(v2,v3);
		drawLineDDA(v3,v1);



	}
			
};

#endif //_rt_H
