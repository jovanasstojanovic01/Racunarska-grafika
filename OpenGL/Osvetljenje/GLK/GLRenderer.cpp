#include "stdafx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	alpha = 30;
	beta = 90;

	red = false;
	green = false;
	blue = false;
	normals = false;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis(double width)
{
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0.0);
	glVertex3f(width, 0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0.0);
	glVertex3f(0.0, width, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0.0);
	glVertex3f(0.0, 0.0, width);

	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta, int color)
{
	glDisable(GL_LIGHTING);
	double x, y, z;
	for (int i = 0; i < nSegAlpha; i++) {
		glBegin(GL_QUAD_STRIP);
		double alpha1 = -90 + i * 180.0 / nSegAlpha, alpha2 = -90 + (i + 1) * 180.0 / nSegAlpha;
		for (int j = 0; j <= nSegBeta; j++) {
			double beta = j * 360.0 / nSegBeta;
			double alphaRad[] = { alpha1 * pi / 180, alpha2 * pi / 180 }, betaRad = beta * pi / 180;

			for (int k = 0; k < 2; k++) {
				x = r * cos(alphaRad[k]) * cos(betaRad);
				y = r * sin(alphaRad[k]);
				z = r * cos(alphaRad[k]) * sin(betaRad);
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawRectangle(double a1, double b1, double a2, double b2, double c, int nSeg, int axis, int sgn)
{
	GLfloat mat_amb_floor[] = { 0.8, 0.8, 0.8, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb_floor);
	GLfloat mat_d_floor[] = { 0.85, 0.85, 0.85, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_floor);
	
	GLfloat normal[] = { 0, 0, 0 };
	normal[axis] = sgn;
	glNormal3fv(normal);

	for (int i = 0; i < nSeg; i++) {
		glBegin(GL_QUAD_STRIP);
		double q1 = a1 + i * (a2 - a1) / nSeg;
		double q2 = a1 + (i + 1) * (a2 - a1) / nSeg;
		for (int j = 0; j <= nSeg; j++) {
			double w = b1 + j * (b2 - b1) / nSeg;
			if (axis == 0) {
				glVertex3f(c, q1, w);
				glVertex3f(c, q2, w);
			}
			else if (axis == 1) {
				glVertex3f(q1, c, w);
				glVertex3f(q2, c, w);
			}
			else if (axis == 2) {
				glVertex3f(q1, w, c);
				glVertex3f(q2, w, c);
			}
		}
		glEnd();
	}
}

void CGLRenderer::DrawCuboid(double x, double y, double z, int nSeg, int sgn)
{
	DrawRectangle(-x / 2, -y / 2, x / 2, y / 2, z / 2, nSeg, 2, -sgn);
	DrawRectangle(-x / 2, -y / 2, x / 2, y / 2, -z / 2, nSeg, 2, sgn);

	DrawRectangle(-x / 2, -z / 2, x / 2, z / 2, y / 2, nSeg, 1, -sgn);
	DrawRectangle(-x / 2, -z / 2, x / 2, z / 2, -y / 2, nSeg, 1, sgn);

	DrawRectangle(-y / 2, -z / 2, y / 2, z / 2, x / 2, nSeg, 0, -sgn);
	DrawRectangle(-y / 2, -z / 2, y / 2, z / 2, -x / 2, nSeg, 0, sgn);
}

void CGLRenderer::DrawHemisphere(double r, int nSegAlpha, int nSegBeta) {
	GLfloat mat[] = { 0.84, 0.84, 0.84, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);

	double x, y, z, nx, ny, nz;
	for (int i = 0; i < nSegAlpha; i++) {
		glBegin(GL_QUAD_STRIP);
		double alpha1 = i * 90.0 / nSegAlpha, alpha2 = (i + 1) * 90.0 / nSegAlpha;
		for (int j = 0; j <= nSegBeta; j++) {
			double beta = j * 360.0 / nSegBeta;
			double alphaRad[] = { alpha1 * pi / 180, alpha2 * pi / 180 }, betaRad = beta * pi / 180;

			for (int k = 0; k < 2; k++) {
				nx = cos(alphaRad[k]) * cos(betaRad);
				ny = sin(alphaRad[k]);
				nz = cos(alphaRad[k]) * sin(betaRad);
				glNormal3f(nx, ny, nz);

				x = r * cos(alphaRad[k]) * cos(betaRad);
				y = r * sin(alphaRad[k]);
				z = r * cos(alphaRad[k]) * sin(betaRad);
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
}

void CGLRenderer::DrawCone(double h, double r1, double r2, int nSeg, bool isDark)
{
	GLfloat amb[] = { 0, 0, 0, 1.0 };
	GLfloat dif[] = { 0, 0, 0, 1.0 };
	if (isDark) { //CYAN BOJA - VAZA BOJA 1
		amb[0] = 0.0; amb[1] = 1.0; amb[2] = 1.0;
		dif[0] = 0; dif[1] = 1; dif[2] = 0;
	}
	else { //CRVENA BOJA - VAZA BOJA 2
		amb[0] = 1.0; amb[1] = 0.0; amb[2] = 0.0;
		dif[0] = 1; dif[1] = 0; dif[2] = 0;
	}
	GLfloat shine = 64.0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

	glBegin(GL_QUAD_STRIP);
	double l = 2;//sqrt(h * h + (r1 - r2) * (r1 - r2));
	double nr = h / l, ny = (r1 - r2) / l;
	double* normals = new double[7 * (nSeg + 1)];

	for (int i = 0; i <= nSeg; i++) {
		// normale
		double nx = nr * cos(i * 2 * pi / nSeg);
		double nz = nr * sin(i * 2 * pi / nSeg);
		glNormal3f(nx, ny, nz);
		normals[7 * i] = nx;
		normals[7 * i + 1] = ny;
		normals[7 * i + 2] = nz;
		normals[7 * i + 3] = r1 * cos(i * 2 * pi / nSeg); // x1
		normals[7 * i + 4] = r1 * sin(i * 2 * pi / nSeg); // z1
		normals[7 * i + 5] = r2 * cos(i * 2 * pi / nSeg); // x2
		normals[7 * i + 6] = r2 * sin(i * 2 * pi / nSeg); // z2

		glVertex3f(r1 * cos(i * 2 * pi / nSeg), 0, r1 * sin(i * 2 * pi / nSeg));
		glVertex3f(r2 * cos(i * 2 * pi / nSeg), h, r2 * sin(i * 2 * pi / nSeg));
	}
	glEnd();

	// crtanje normala
	if (this->normals) {
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		for (int i = 0; i <= nSeg; i++) {
			glVertex3f(normals[7 * i + 3], 0, normals[7 * i + 4]);
			glVertex3f(normals[7 * i + 3] + normals[7 * i + 0], normals[7 * i + 1], normals[7 * i + 4] + normals[7 * i + 2]);

			glVertex3f(normals[7 * i + 5], h, normals[7 * i + 6]);
			glVertex3f(normals[7 * i + 5] + normals[7 * i + 0], h + normals[7 * i + 1], normals[7 * i + 6] + normals[7 * i + 2]);
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
}

void CGLRenderer::DrawLights()
{
	if (red) {
		GLfloat red_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		glPushMatrix();
		glTranslatef(0, 120, 0);
		glColor3fv(red_diffuse);
		DrawSphere(3, 10, 10, 0);
		glPopMatrix();
	}
	if (green) {
		GLfloat green_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
		glPushMatrix();
		glTranslatef(0, 75, -125);
		glColor3fv(green_diffuse);
		DrawSphere(3, 10, 10, 2);
		glPopMatrix();
	}
	if (blue) {
		GLfloat blue_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
		glPushMatrix();
		glTranslatef(0, 75, 125);
		glColor3fv(blue_diffuse);
		DrawSphere(3, 10, 10, 1);
		glPopMatrix();
	}
}

void CGLRenderer::DrawCylinder(double h, double r, int nSeg)
{
	GLfloat mat[] = { 0.84, 0.84, 0.84, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(r * cos(i * 2 * pi / nSeg), 0, r * sin(i * 2 * pi / nSeg));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);

	double l = sqrt(h * h + r * r);
	double nr = h / l;
	for (int i = 0; i <= nSeg; i++) {
		// normale
		double nx = nr * cos(i * 2 * pi / nSeg);
		double nz = nr * sin(i * 2 * pi / nSeg);
		glNormal3f(nx, 0, nz);

		glVertex3f(r * cos(i * 2 * pi / nSeg), 0, r * sin(i * 2 * pi / nSeg));
		glVertex3f(r * cos(i * 2 * pi / nSeg), h, r * sin(i * 2 * pi / nSeg));
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, h, 0);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(r * cos(i * 2 * pi / nSeg), h, r * sin(i * 2 * pi / nSeg));
	}
	glEnd();
}

void CGLRenderer::DrawFigure()
{
	float H = 3, r1 = 6, r2 = 9, r3 = 12, r4 = 15, nSeg = 30, cylinderH = 20, cuboidH = 6;
	glPushMatrix();
	DrawHemisphere(r2, nSeg, nSeg);

	glTranslatef(0, r2 / 2, 0);
	DrawCylinder(cylinderH, r2 / 2, nSeg);

	glTranslatef(0, cylinderH, 0);
	DrawCuboid(30, cuboidH, 30, nSeg, -1);

	glTranslatef(0, cuboidH / 2, 0);
	DrawCone(H, r3, r2, nSeg, false);

	glTranslatef(0, H, 0);
	DrawCone(H, r2, r1, nSeg, true);

	glTranslatef(0, H, 0);
	DrawCone(H, r1, r1, nSeg, false);

	glTranslatef(0, H, 0);
	DrawCone(H, r1, r1, nSeg, true);

	//vaza

	glTranslatef(0, H, 0);
	DrawCone(H, r1, r2, nSeg, false);

	glTranslatef(0, H, 0);
	DrawCone(H, r2, r1, nSeg, true);

	glTranslatef(0, H, 0);
	DrawCone(H, r1, r2, nSeg, false);

	glTranslatef(0, H, 0);
	DrawCone(H, r2, r3, nSeg, true);

	glTranslatef(0, H, 0);
	DrawCone(H, r3, r2, nSeg, false);

	glTranslatef(0, H, 0);
	DrawCone(H, r2, r1, nSeg, true);

	glTranslatef(0, H, 0);
	DrawCone(H, r1, r2, nSeg, false);

	glTranslatef(0, H, 0);
	DrawCone(H, r2, r3, nSeg, true);

	glTranslatef(0, H, 0);
	DrawCone(H, r3, r3+3, nSeg, false);

	glTranslatef(0, H, 0);
	DrawCone(H, r3+3, r3 + 6, nSeg, true);

	glPopMatrix();
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glLoadIdentity();
	double alphaRad = alpha * pi / 180;
	double betaRad = beta * pi / 180;
	double eyeX = distance * cos(alphaRad) * cos(betaRad);
	double eyeY = distance * sin(alphaRad);
	double eyeZ = distance * cos(alphaRad) * sin(betaRad);

	glClearColor(0.53, 0.75, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(eyeX, eyeY, eyeZ, 0, 40, 0, 0, 1, 0);

	//ambijentalno svetlo
	glEnable(GL_LIGHTING);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	float light_ambient[] = { 0.3,0.3,0.3, 1.0 };
	float light_diffuse[] = { 0.5,0.5,0.5, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { /*1.5,1,0.5, 0.0*/ -10,-5,1,0};
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT1);


	if (red) { //SVETLO 3 **LEVO** - FUSCIA BOJA (1.00,0.00,1.00)
		GLfloat light_ambient[] = { 1.0, 0.0, 1.0, 1.0 };
		GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; //RGB komponente
		GLfloat light_position[] = { 0, 200, 0, 1.0 }; //pozicija
		GLfloat spot_direction[] = { 0, -1, 0 }; //orijentacija
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);// Boja svetla R
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT2, GL_POSITION, light_position);

		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25.0);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.05);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
		glEnable(GL_LIGHT2);
	}
	else glDisable(GL_LIGHT2);

	if (green) { //SVETLO 2 **DESNO** - CRVENA BOJA (1.00,0.00,0.00)
		GLfloat light_ambient[] = { 1.00,0.00,0.00, 1.0 };
		GLfloat light_diffuse[] = { 1.00,0.00,0.00, 1.0 }; //RGB komponente
		GLfloat light_position[] = { 0, 105, -105, 1.0 }; //pozicija
		GLfloat spot_direction[] = { 0, 0, 1 }; //orijentacija
		glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);// Boja svetla R
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT3, GL_POSITION, light_position);

		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25.0);
		glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0.05);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
		glEnable(GL_LIGHT3);
	}
	else glDisable(GL_LIGHT3);

	if (blue) { //SVETLO 3 **LEVO** - CRVENA BOJA (0.00,1.00,1.00)
		GLfloat light_ambient[] = { 1,0,0, 1.0 };
		GLfloat light_diffuse[] = { 1.00,0.00,0.00, 1.0 }; //RGB komponente
		GLfloat light_position[] = { 0, 105, 105, 1.0 }; //pozicija
		GLfloat spot_direction[] = { 0, 0, -1 }; //orijentacija
		glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient);// Boja svetla R
		glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT4, GL_POSITION, light_position);

		glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 25.0);
		glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 0.05);
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
		glEnable(GL_LIGHT4);
	}
	else glDisable(GL_LIGHT4);

	DrawLights();
	glPushMatrix();
	glTranslatef(0, 125, 0);
	DrawCuboid(250, 250, 250, 250, 1);
	glPopMatrix();
	glDisable(GL_LIGHTING);
	DrawAxis(150);
	glEnable(GL_LIGHTING);
	DrawFigure();
	//glRotatef(90,1,0,0);
	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::incrementAlpha(int angle)
{
	alpha += angle;
	if (alpha >= 90) alpha = 89.9;
	else if (alpha <= -90) alpha = -89.9;
}

void CGLRenderer::incrementBeta(int angle)
{
	beta += angle;
}

void CGLRenderer::toggleRedLight()
{
	red = !red;
}

void CGLRenderer::toggleGreenLight()
{
	green = !green;
}

void CGLRenderer::toggleBlueLight()
{
	blue = !blue;
}

void CGLRenderer::toggleNormals()
{
	normals = !normals;
}
