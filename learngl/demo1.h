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
	//This piece of code is from Quake3 game engine. It uses some numerical 
	//properties on InvSqrt that makes is faster to compute
	float InvSqrt (float x){
		float xhalf = 0.5f*x;
		int i = *(int*)&x;
		i = 0x5f3759df - (i>>1);
		x = *(float*)&i;
		x = x*(1.5f - xhalf*x*x);
		return x;
	}
	void drawLineAAv2(Vector3f & v1,Vector3f & v2)
	{
		int i=0;
		int x1=(int)v1[0];
		int x2=(int)v2[0];
		int y1=(int)v1[1];
		int y2=(int)v2[1];

		int dx = abs(x2-x1);
		int dy = abs(y2-y1) ;
		int sx,sy;
		if (x1 < x2) sx = 1; else sx = -1;
		if (y1 < y2) sy = 1; else sy = -1;
		int err = dx-dy;

		int twovdu = 0;	/* Numerator of distance; starts at 0 */
		double invD = 1.0 / InvSqrt((float)dx*dx + dy*dy);   /* Precomputed inverse denominator */
		double invD2du = 2.0 * (dx*invD);   /* Precomputed constant */

		while(true)
		{
			glBegin(GL_POINTS);
			glColor3d(twovdu*invD,twovdu*invD,twovdu*invD);
			glVertex2i(x1,y1);
			double color=(invD2du-twovdu*invD);
			glColor3d(color,color,color);
			glVertex2i(x1,y1+1);
			color=(invD2du+twovdu*invD);
			glColor3d(color,color,color);
			glVertex2i(x1,y1-1);
			//glVertex2i(uadd,vadd + vincr);
			glEnd();
			if ((x1 == x2)&&( y1 == y2)) break;
			int e2 = 2*err;
			if (e2 > -dy) 
			{
				twovdu=dx+dy-err;
				err = err - dy;
				x1 = x1 + sx;
				
			}
			if (e2 <  dx) 
			{
				twovdu=dy-err;
				err = err + dx;
				y1 = y1 + sy ;
				
			}
		}

	}
	void drawLineAAv1(Vector3f & v1,Vector3f & v2)
	{
		int x1=(int)v1[0];
		int x2=(int)v2[0];
		int y1=(int)v1[1];
		int y2=(int)v2[1];


		int dx = x2-x1;
		int dy = y2-y1;
		/* By switching to (u,v), we combine all eight octants */
		int du,dv,v,u,uincr,vincr,uadd,vadd;
		uadd=x1;
		vadd=y1;
		if (abs(dx) > abs(dy))
		{
			/* Note: If this were actual C, these integers would be lost
			* at the closing brace.  That's not what I mean to do.  Do what
			* I mean. */
			du = abs(dx);
			dv = abs(dy);
			u = x1;
			v = y1;
			uincr = 1;
			vincr = 1;
			if (dx < 0) uincr = -1;
			if (dy < 0) vincr = -1;
		}
		else
		{
			du = abs(dy);
			dv = abs(dx);
			u = y1;
			v = x1;
			uincr = 1;
			vincr = 1;
			if (dy < 0) uincr = -1;
			if (dx < 0) vincr = -1;
		}
		int uend = u + 2 * du;
		int d = (2 * dv) - du;	    /* Initial value as in Bresenham's */
		int incrS = 2 * dv;	/* Deltad for straight increments */
		int incrD = 2 * (dv - du);	/* Delatad for diagonal increments */
		int twovdu = 0;	/* Numerator of distance; starts at 0 */
		double invD = 1.0 / InvSqrt((float)du*du + dv*dv);   /* Precomputed inverse denominator */
		double invD2du = 2.0 * (du*invD);   /* Precomputed constant */
		glBegin(GL_POINTS);
		do
		{
			/* Note: this pseudocode doesn't ensure that the address is
			* valid, or that it even represents a pixel on the same side of
			* the screen as the adjacent pixel */
			glColor3f(1.0,1.0,1.0);
			//glColor3d(twovdu*invD,twovdu*invD,twovdu*invD);
			glVertex2i(uadd,vadd);
			//DrawPixelD(addr, twovdu*invD);
			//glColor3d(invD2du - twovdu*invD,invD2du - twovdu*invD,invD2du - twovdu*invD);
			//glVertex2i(uadd,vadd + vincr);
			//DrawPixelD(addr + vincr, invD2du - twovdu*invD);
			//glColor3d(invD2du + twovdu*invD,invD2du + twovdu*invD,invD2du + twovdu*invD);
			//glVertex2i(uadd,vadd - vincr);
			//DrawPixelD(addr - vincr, invD2du + twovdu*invD);
			//glColor3f(1.0,1.0,1.0);

			if (d < 0)
			{
				/* choose straight (u direction) */
				twovdu = d + du;
				d = d + incrS;
			}
			else
			{
				/* choose diagonal (u+v direction) */
				twovdu = d - du;
				d = d + incrD;
				v = v+1;
				vadd+=vincr;
			}
			u = u+1;
			uadd+=uincr;

		} while (u < uend);
		glEnd();
	}
	void drawLineMidPoint(Vector3f & v1,Vector3f & v2)
	{
		//implementation based on Wikipedia
		int i=0;
		int x1=(int)v1[0];
		int x2=(int)v2[0];
		int y1=(int)v1[1];
		int y2=(int)v2[1];

		int dx = abs(x2-x1);
		int dy = abs(y2-y1) ;
		int sx,sy;
		if (x1 < x2) sx = 1; else sx = -1;
		if (y1 < y2) sy = 1; else sy = -1;
		int err = dx-dy;

		while(true)
		{
			glBegin(GL_POINTS);
			glVertex2i(x1,y1);
			glEnd();
			if ((x1 == x2)&&( y1 == y2)) break;
			int e2 = 2*err;
			if (e2 > -dy) 
			{
				err = err - dy;
				x1 = x1 + sx;
			}
			if (e2 <  dx) 
			{
				err = err + dx;
				y1 = y1 + sy ;
			}
		}

	}
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
		drawLineMidPoint(v1,v2);
		drawLineMidPoint(v2,v3);
		drawLineAAv2(v3,v1);



	}

};

#endif //_rt_H
