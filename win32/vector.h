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

}Vector2d


typedef struct
{
	union {float x; float r;}/**<x component of the vector3d>*/
	union {float y; float g;}/**<y component of the vector3d>*/
	union {float z; float b;}/**<z component of the vector3d>*/

}Vector3d


typedef struct
{
	union {float x; float r;}/**<x component of the vector4d>*/
	union {float y; float g;}/**<y component of the vector4d>*/
	union {float z; float b;}/**<z component of the vector4d>*/
	union {float w; float a;}/**<w component of the vector4d>*/

}Vector4d

#define ZERO 0

#define Vector2d_Add(a, b, c) (c.x=a.x+b.x, c.y=a.y+b.y)/**<macro to add 2Dvectors>*/   
#define Vector3d_Add(a, b, c) (c.x=a.x+b.x, c.y=a.y+b.y, c.z=a.z+b.z)/**<macro to add 3Dvectors>*/
#define Vector4d_Add(a, b, c) (c.x=a.x+b.x, c.y=a.y+b.y, c.z=a.z+b.z, c.w=a.w+b.w)/**<macro to add 4Dvectors>*/


#endif