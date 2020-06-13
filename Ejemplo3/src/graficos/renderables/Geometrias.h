#ifndef _Geom
#define _Geom
#define _USE_MATH_DEFINES

#include <math.h>
#include "matematicas/vec3.h"
#include "matematicas/vec2.h"
#include "matematicas/VectorRR.h"
#include "graficos/Imagenes.h"

//estructura para manejar primitivos con posicion, normal y uv's
/* TODO: Condense into vec3 and vec2 instead of float */
struct Vertices {
	float Posx, Posy, Posz;           
	float Normx, Normy, Normz;
	float u, v;
	float Tx, Ty, Tz;
	float Bx, By, Bz;
};

//En honor a nuestros ancestros llamaremos "Maya" a la malla
//estructura que contiene datos de los vertices y sus indices
struct Maya {
	Vertices *maya;
	unsigned int *indices;
};

class Primitivos: public VectorRR, public Imagenes
{
public:
	unsigned int cantVert;
	unsigned int cantIndices;
public:
	Primitivos();

	//generamos los vertices a traves de coordenadas esfericas
	//conocimiento adquirido en la materia de matematicas para videjuegos I
	Maya BillBoardBasico(float escala);

	//generamos los vertices a traves de coordenadas esfericas
	//conocimiento adquirido en la materia de matematicas para videjuegos I
	Maya Esfera(int stacks, int slices, float radio, float inicio, float final);
	Maya Plano(int vertx, int vertz, float anchof, float profz);
	Maya Plano(int vertx, int vertz, float anchof, float profz, unsigned char* altura, float tile = 1);
};
#endif