#include "Terreno.h"

#include "platform/Application.h"

Terreno::Terreno(const wchar_t* alturas, const wchar_t* textura, const wchar_t* textura2,
	float ancho, float prof, int numtext, int numtext2, const mat4 &transform, LightClass *lightSetup) 
	: Renderable(transform, lightSetup) {

	slot1 = numtext;
	slot2 = numtext2;
	anchof = ancho;
	proff = prof;

	m_shader = new LightShaderClass("shaders/light.vs", "shaders/light.ps");
	// Initialize the light shader object.
	bool result = m_shader->Initialize();
	if (!result)
	{
		MessageBox(Application::GetApplication().GetHWnd(), (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return;
	}

	//cargamos la textura de la figura
	Carga(alturas);
	//en caso del puntero de la imagen sea nulo se brica esta opcion
	m_data = new Maya();
	*m_data = Plano(Ancho(), Alto(), ancho, prof, Dir_Imagen(), 100);

	deltax = anchof / Ancho();
	deltaz = proff / Alto();
	verx = Ancho();
	verz = Alto();

	//disponemos la textura del gdi.
	Descarga();

	PushBuffers();

	m_textureID1 = LoadTexture(textura);
	m_textureID2 = LoadTexture(textura2);
}

Terreno::~Terreno() {
	glDeleteTextures(1, &planoTextura);
	glDeleteTextures(1, &m_textureID1);
	glDeleteTextures(1, &m_textureID2);
}

float Terreno::Superficie(float x, float z) {
	//obtenemos el indice pero podria incluir una fraccion
	float indicefx = (x + anchof / 2) / deltax;
	float indicefz = (z + proff / 2) / deltaz;
	//nos quedamos con solo la parte entera del indice
	int indiceix = (int)indicefx;
	int indiceiz = (int)indicefz;
	//nos quedamos con solo la fraccion del indice
	float difx = indicefx - indiceix;
	float difz = indicefz - indiceiz;

	float altura;
	float D;

	//el cuadro del terreno esta formado por dos triangulos, si difx es mayor que dify 
	//entonces estamos en el triangulo de abajo en caso contrario arriba
	if (difx > difz)
	{
		//obtenemos el vector 1 de dos que se necesitan
		VectorRR v1(m_data->maya[indiceix + 1 + (indiceiz + 1) * verx].Posx - m_data->maya[indiceix + indiceiz * verx].Posx,
			m_data->maya[indiceix + 1 + (indiceiz + 1) * verx].Posy - m_data->maya[indiceix + indiceiz * verx].Posy,
			m_data->maya[indiceix + 1 + (indiceiz + 1) * verx].Posz - m_data->maya[indiceix + indiceiz * verx].Posz);
		//obtenemos el vector 2 de dos
		VectorRR v2(m_data->maya[indiceix + 1 + indiceiz * verx].Posx - m_data->maya[indiceix + indiceiz * verx].Posx,
			m_data->maya[indiceix + 1 + indiceiz * verx].Posy - m_data->maya[indiceix + indiceiz * verx].Posy,
			m_data->maya[indiceix + 1 + indiceiz * verx].Posz - m_data->maya[indiceix + indiceiz * verx].Posz);

		//con el producto punto obtenemos la normal y podremos obtener la ecuacion del plano
		//la parte x de la normal nos da A, la parte y nos da B y la parte z nos da C
		VectorRR normalPlano = Cruz(v1, v2);
		//entonces solo falta calcular D
		D = -1 * (normalPlano.X * m_data->maya[indiceix + indiceiz * verx].Posx +
			normalPlano.Y * m_data->maya[indiceix + indiceiz * verx].Posy +
			normalPlano.Z * m_data->maya[indiceix + indiceiz * verx].Posz);
		//sustituyendo obtenemos la altura de contacto en el terreno
		altura = ((-normalPlano.X * x - normalPlano.Z * z - D) / normalPlano.Y);
	}
	else
	{
		VectorRR v1(m_data->maya[indiceix + (indiceiz + 1) * verx].Posx - m_data->maya[indiceix + indiceiz * verx].Posx,
			m_data->maya[indiceix + (indiceiz + 1) * verx].Posy - m_data->maya[indiceix + indiceiz * verx].Posy,
			m_data->maya[indiceix + (indiceiz + 1) * verx].Posz - m_data->maya[indiceix + indiceiz * verx].Posz);

		VectorRR v2(m_data->maya[indiceix + 1 + (indiceiz + 1) * verx].Posx - m_data->maya[indiceix + indiceiz * verx].Posx,
			m_data->maya[indiceix + 1 + (indiceiz + 1) * verx].Posy - m_data->maya[indiceix + indiceiz * verx].Posy,
			m_data->maya[indiceix + 1 + (indiceiz + 1) * verx].Posz - m_data->maya[indiceix + indiceiz * verx].Posz);


		VectorRR normalPlano = Cruz(v1, v2);

		D = -1 * (normalPlano.X * m_data->maya[indiceix + indiceiz * verx].Posx +
			normalPlano.Y * m_data->maya[indiceix + indiceiz * verx].Posy +
			normalPlano.Z * m_data->maya[indiceix + indiceiz * verx].Posz);

		altura = ((-normalPlano.X * x - normalPlano.Z * z - D) / normalPlano.Y);
	}

	return altura;
}

void Terreno::Bind(FPSCamara* camera) {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0 + slot1);
	glBindTexture(GL_TEXTURE_2D, m_textureID1);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0 + slot2);
	glBindTexture(GL_TEXTURE_2D, m_textureID2);

	m_shader->Pon1Entero("shaderTexture", 0);
	m_shader->Pon1Entero("shaderTexture2", 1);
}

void Terreno::Unbind(FPSCamara* camera) {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0 + slot1);
	glBindTexture(GL_TEXTURE_2D, 0);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0 + slot2);
	glBindTexture(GL_TEXTURE_2D, 0);
}