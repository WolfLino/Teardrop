#include <GL/glut.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>

#define PI 3.14159265

using namespace std;

struct Ponto2D {
	int x;
	int y;
};

Ponto2D tdPos;
int sim = 0;
int rot = 0;

float seno(float x) {
	return sin(x * PI / 180);
}

float tang(float x) {
	return tan(x * PI / 180);
}

float coss(float x) {
	return cos(x * PI / 180);
}

// Obtem o raio do arco do teardrop
// Usei a lei dos senos
//				ladoA
// anguloA	---------- anguloC
//		    -       -  
//		    -      -
//		    -     -
//		    -    -
//	ladoC   -   -  ladoB
//		    -  -
//		    - -
//		    --
//		    -
//			anguloA
// ===========================================================================
float cateto(float anguloA, float anguloB, float ladoC) {
	float anguloC = 180 - (anguloA + anguloB);

	float ladoB = (ladoC * seno(anguloB)) / seno(anguloC);

	return ladoB;
}

// ladoB / seno(anguloB) = ladoC / seno(anguloC)
// Logo:
// ladoB = (ladoC * seno(anguloB)) / seno(anguloC);

void teardrop(int x, int y, float comp, float anguloAbertura, float rotacao) {
	float meioAng = anguloAbertura / 2;

	// Linhas
	glBegin(GL_LINES);
	glColor3i(0, 0, 0);
	glVertex2f(x, y);
	glVertex2f(x + (comp * coss(rotacao + meioAng)), y + (comp * seno(rotacao + meioAng)));
	glVertex2f(x, y);
	glVertex2f(x + (comp * coss(rotacao - meioAng)), y + (comp * seno(rotacao - meioAng)));
	glEnd();

	// A variação é distancia do dislocamento gerado ...
	float variacao = (x + (comp * coss(0))) - (x + (comp * coss(meioAng)));

	// Encontra o centro do arco
	float centroArcoX = x + ((comp - variacao) * coss(rotacao));
	float centroArcoY = y + ((comp - variacao) * seno(rotacao));

	// Desenha o arco
	glBegin(GL_LINE_STRIP);
	glColor3i(0, 0, 0);
	// NOTA: dificil de explicar, qq coisa apenas aceite que funciona
	// 
	for (int ang = rotacao + 90; ang >= (rotacao - 90); ang--) {
		//         centro circulo + ladoB do triangulo (raio) * cosseno da abertura do ângulo | Mesma coisa no eixo y
		glVertex2f(centroArcoX + (cateto(90, meioAng, comp - variacao) * coss(ang)), centroArcoY + (cateto(90, meioAng, comp - variacao)) * seno(ang));
	}
	glEnd();
}

void teardropMulti(int x, int y, float comp, float anguloAbertura, int qnt) {
	float rotacao = 360 / qnt;
	
	for (int i = 0; i < qnt; i++) {
		teardrop(x, y, comp, anguloAbertura, rotacao * i);
	}
}

void desenha(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	// Desenha o teardrop
	teardropMulti(tdPos.x, tdPos.y, 100, 45, 6);

	glFlush();
}

void mouse(int botao, int estado, int x, int y) {
	// Obtem a posição do teardrop
	if (botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		tdPos.x = x;
		tdPos.y = y;
	}

	desenha();
}

void inicializa(void) {
	glClearColor(1, 1, 1, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Teardrop");
	gluOrtho2D(0, 600, 600, 0);
	glutDisplayFunc(desenha);
	glutMouseFunc(mouse);
	inicializa();
	glutMainLoop();
	return 0;
}
