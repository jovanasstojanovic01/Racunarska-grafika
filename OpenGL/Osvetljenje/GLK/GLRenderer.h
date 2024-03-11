#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void incrementAlpha(int angle);
	void incrementBeta(int angle);
	void toggleRedLight();
	void toggleGreenLight();
	void toggleBlueLight();
	void toggleNormals();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

private:
	const double pi = 3.14159265359;
	const double distance = 140;
	double alpha;
	double beta;

	bool red;
	bool green;
	bool blue;
	bool normals;

	void DrawAxis(double width);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta, int color);
	void DrawRectangle(double a1, double b1, double a2, double b2, double c, int nSeg, int axis, int sgn);
	void DrawCuboid(double a, double b, double h, int nSeg, int sgn);
	void DrawHemisphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r, int nSeg);
	void DrawCone(double h, double r1, double r2, int nSeg, bool isDark);
	void DrawLights();
	void DrawFigure();

};
