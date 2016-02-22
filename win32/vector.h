#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <math.h>

/**
  *@brief a simple vector structure
  */
 
typedef struct
{
	float x;/**<x component of the vector2d>*/
	float y;/**<y component of the vector2d>*/

}Vector2d;


typedef struct
{
	union {float x; float r;};/**<x component of the vector3d>*/
	union {float y; float g;};/**<y component of the vector3d>*/
	union {float z; float b;};/**<z component of the vector3d>*/

}Vector3d;


typedef struct
{
	union {float x; float r;};/**<x component of the vector4d>*/
	union {float y; float g;};/**<y component of the vector4d>*/
	union {float z; float b;};/**<z component of the vector4d>*/
	union {float w; float a;};/**<w component of the vector4d>*/

}Vector4d;

int VectorCompare(Vector2d v1, Vector2d v2);
Vector2d Normalize(Vector2d v);
void Scale(Vector2d v1, int scale, Vector2d v2);

/**
*
*/

int rect_intersect(SDL_Rect a, SDL_Rect b);

#define ZERO 0

/*Adding Vectors */
#define Vector2d_Add(a, b, c) (c.x=a.x+b.x, c.y=a.y+b.y)/**<macro to add 2Dvectors>*/   
#define Vector3d_Add(a, b, c) (c.x=a.x+b.x, c.y=a.y+b.y, c.z=a.z+b.z)/**<macro to add 3Dvectors>*/
#define Vector4d_Add(a, b, c) (c.x=a.x+b.x, c.y=a.y+b.y, c.z=a.z+b.z, c.w=a.w+b.w)/**<macro to add 4Dvectors>*/

/* Subtracting Vectors */
#define Vector2d_Sub(a, b, c) (c.x=a.x-b.x, c.y=a.y-b.y)/**<macro to subtract 2Dvectors>*/   
#define Vector3d_Sub(a, b, c) (c.x=a.x-b.x, c.y=a.y-b.y, c.z=a.z+b.z)/**<macro to subtract 3Dvectors>*/
#define Vector4d_Sub(a, b, c) (c.x=a.x-b.x, c.y=a.y-b.y, c.z=a.z+b.z, c.w=a.w+b.w)/**<macro to subtract 4Dvectors>*/

/*Copy Vectors */
#define Vector2d_Copy(a, b) (b.x=a.x,b.y=a.y)/**<macro to copy 2Dvectors>*/   
#define Vector3d_Copy(a, b) (b.x=a.x,b.y=a.y,b.z=a.z)/**<macro to copy 3Dvectors>*/
#define Vector4d_Copy(a, b) (b.x=a.x,b.y=a.y,b.z=a.z,b.w=a.w)/**<macro to copy 4Dvectors>*/

/*Clear Vectors */
#define Vector2d_Clear(a) (a.x=a.y=0)/**<macro to clear 2Dvectors>*/   
#define Vector3d_Clear(a) (a.x=a.y=a.z=0)/**<macro to clear 3Dvectors>*/
#define Vector4d_Clear(a) (a.x=a.y=a.z=a.w=0)/**<macro to clear 4Dvectors>*/

/*Negate Vectors */
#define Vector2d_Negate(a, b) (b.x=-a.x,b.y=-a.y)/**<macro to negate 2Dvectors>*/   
#define Vector3d_Negate(a, b) (b.x=-a.x,b.y=-a.y,b.z=-a.z)/**<macro to negate 3Dvectors>*/
#define Vector4d_Negate(a, b) (b.x=-a.x,b.y=-a.y,b.z=-a.z,b.w=-a.w)/**<macro to negate 4Dvectors>*/

/*Set Vectors */
#define Vector2d_Set(a, x, y) (a.x=x,a.y=y)/**<macro to set 2Dvectors>*/   
#define Vector3d_Set(a, x, y, z) (a.x=x,a.y=y,a.z=z)/**<macro to set 3Dvectors>*/
#define Vector4d_Set(a, x, y, z, w) (a.x=x,a.y=y,a.z=z,a.w=w)/**<macro to set 4Dvectors>*/

/*Dot Vectors */
#define Vector2d_Dot(x, y) (x.x*y.x+x.y*y.y)/**<macro to Dot Product 2Dvectors>*/   
#define Vector3d_Dot(x, y) (x.x*y.x+x.y*y.y+x.z*y.z)/**<macro to Dot Product 3Dvectors>*/
#define Vector4d_Dot(x, y) (x.x*y.x+x.y*y.y+x.z*y.z+x.w*y.w)/**<macro to Dot Product 4Dvectors>*/

#endif