#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 1024
#define HEIGHT 768
#define PI 3.1415926

void *fonts[]=
{
    GLUT_BITMAP_9_BY_15,
    GLUT_BITMAP_TIMES_ROMAN_10,
    GLUT_BITMAP_TIMES_ROMAN_24,
    GLUT_BITMAP_HELVETICA_18,
    GLUT_BITMAP_HELVETICA_12
};


int page=-1; //start at intro page
float theta=1;


//for printing text
void output(int x, int y, char *string, void *font)
{
    int len, i;
    glRasterPos2f(x, y);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, string[i]);
    }
}

void drawHelixStrand(float cx, float cy, float r, float angle=0)
{
	float x, y, z, theta;
	int i, n = 565;
	angle = angle * PI / 180.0;
	glBegin(GL_LINE_STRIP);
	    for(i=0; i<n; i++)
	    {
	    	theta = 2.0 * PI * i / n;
	    	x = cx - (r * sinf(1 * theta + angle));
	    	y = cy + i;
	    	z = r * cosf(1 * theta + angle);

	    	glVertex3f(x, y, z);
	    }
    glEnd();
}

void drawSphere(float x, float y, float z, float r=7)
{
    glTranslatef(x, y, z);
    glutSolidSphere(r, 20, 20);
    glTranslatef(-x, -y, -z);
}

void drawHelixLine(float cx, float cy, float r, float angle=180)
{
	float x1, x2, y1, y2, z1, z2, theta;
	int i, n = 565, k=0;
	GLubyte colors[2][3];
	angle = angle * PI / 180.0;
    for(i=0; i<n; i+=n/10)
    {
	    	theta = 2.0 * PI * i / n;
	    	x1 = cx - (r * sinf(1 * theta));
	    	y1 = cy + i;
	    	z1 = r * cosf(1 * theta);

	    	x2 = cx - (r * sinf(1 * theta + angle));
	    	y2 = cy + i;
	    	z2 = r * cosf(1 * theta + angle);

	    switch(k)
	    {
	    	case 0: colors[0][0] = 46; colors[0][1] = 204; colors[0][2] = 113;		//turquoise
	    			colors[1][0] = 249; colors[1][1] = 148; colors[1][2] = 6;		//orange
	    			break;
	    	case 1: colors[0][0] = 249; colors[0][1] = 191; colors[0][2] = 59;		//yellow
	    			colors[1][0] = 155; colors[1][1] = 89; colors[1][2] = 182;		//amethyst
	    			break;
	    	case 2:	colors[0][0] = 249; colors[0][1] = 148; colors[0][2] = 6;
	    			colors[1][0] = 46; colors[1][1] = 204; colors[1][2] = 113;
	    			break;
	    	case 3: colors[0][0] = 155; colors[0][1] = 89; colors[0][2] = 182;
	    			colors[1][0] = 249; colors[1][1] = 191; colors[1][2] = 59;
	    }
		glBegin(GL_LINE_STRIP);

        	glColor3ubv(colors[0]);
	    	glVertex3f(x1, y1, z1);
	    	glVertex3f((x1+x2)/2, (y1+y2)/2, (z1+z2)/2);
        	glColor3ubv(colors[1]);
	    	glVertex3f((x1+x2)/2, (y1+y2)/2, (z1+z2)/2);
	    	glVertex3f(x2, y2, z2);

		glEnd();

        glColor3ub(52, 73, 94);
        drawSphere(x1, y1, z1);
        drawSphere(x2, y2, z2);

        k++;
        if (k>3) k=0;
    }
}

void drawAtom(float cx, float cy, float cz, GLubyte prop[])
{
    float r=prop[0];
    glColor3ub(prop[1],prop[2],prop[3]);
    drawSphere(cx, cy, cz, r);
}

void drawBondLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
    glColor3ub(52, 73, 94);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glEnd();
}

void getHexagonPoints(float points[6][2], float x, float y, float side)
{
	points[0][0] = x;
	points[0][1] = y;
	points[1][0] = 0.866*side + x;
	points[1][1] = 0.5*side + y;
	points[2][0] = 0.866*side + x;
	points[2][1] = 1.5*side + y;
	points[3][0] = x;
	points[3][1] = 2.0*side + y;
	points[4][0] = -0.866*side + x;
	points[4][1] = 1.5*side + y;
	points[5][0] = -0.866*side + x;
	points[5][1] = 0.5*side + y;
}

void drawLegend()
{
	GLubyte red[] = {15, 231, 76, 60};          //oxygen
    GLubyte green[] = {20, 39, 174, 96};        //carbon
    GLubyte blue[] = {20, 52, 152, 219};         //nitrogen
    GLubyte yellow[] = {10, 241, 196, 15};    //hydrogen

    drawAtom(50, 170, 0, yellow);
    glColor3ub(52, 73, 94);
    output(100, 168, "Hydrogen", fonts[0]);

    drawAtom(50, 135, 0, red);
    glColor3ub(52, 73, 94);
    output(100, 133, "Oxygen", fonts[0]);

    drawAtom(50, 90, 0, blue);
    glColor3ub(52, 73, 94);
    output(100, 88, "Nitrogen", fonts[0]);

    drawAtom(50, 40, 0, green);
    glColor3ub(52, 73, 94);
    output(100, 38, "Carbon", fonts[0]);


    glColor3ub(52, 73, 94);
    glLineWidth(1.25);
    glBegin(GL_LINE_STRIP);
    	glVertex3f(0, 200, 0);
    	glVertex3f(200, 200, 0);
    	glVertex3f(200, 0, 0);
    glEnd();
}
/*---------------------------------------------------------------------------------------*/

void intro()
{
    glColor3ub(52, 73, 94);
    output(350, 580, " Simulation of DNA - Deoxyribonucleic Acid", fonts[2]);

    glColor3ub(231, 76, 60);
    output(420, 450, "Tishya Tripathi", fonts[3]);
    output(420, 350, "Vighnesh V Bhat Buchchan", fonts[3]);

    glColor3ub(39, 174, 96);
    output(420, 430, "1RN19CS169", fonts[0]);
    output(420, 330, "1RN19CS178", fonts[0]);

    glColor3ub(52, 73, 94);
    output(370, 125, "Press any key to continue...", fonts[0]);
}

/*--------------------------------------------------------------------------------------*/

void menu()
{
    glColor3ub(52, 73, 94);
    output(350, 580, "DNA - Deoxyribonucleic Acid", fonts[2]);

    glColor3ub(39, 174, 96);
    output(420, 460, "Menu", fonts[2]);

    glColor3ub(231, 76, 60);
    output(420, 420, "1. DNA", fonts[3]);
    output(420, 370, "2. Adenine-Thymine", fonts[3]);
    output(420, 320, "3. Cytosine-Guanine", fonts[3]);
    output(420, 270, "ESC. Exit", fonts[3]);

    glColor3ub(52, 73, 94);
    output(370, 125, "Press 'm' to come back to this menu...", fonts[0]);

}

/*--------------------------------------------------------------------------------------*/

void dna()
{
    float cx = 250, cy = 215, r=100;

    glLineWidth(5.0);

    glPushMatrix();

        glRotatef(45, 0, 0, -1);

        glPushMatrix();

            glTranslatef(cx, cy, 0);
            glRotatef(theta, 0.0, 1.0, 0.0);

            glColor3ub(52, 152, 219);
            drawHelixStrand(0, cy, r, 0);
            glColor3ub(231, 76, 60);
            drawHelixStrand(0, cy, r, 138);
            drawHelixLine(0, cy, r, 138);

            glTranslatef(-cx, -cy, 0);

        glPopMatrix();

    glPopMatrix();

    theta+=0.1;

    glColor3ub(52, 73, 94);
    output(25, 540, "DNA Double Helix", fonts[2]);

    //  Labels

    glBegin(GL_LINES);
        glColor3ub(231, 76, 60);
    	glVertex2f(25, 450);
    	glVertex2f(45, 430);
    	glColor3ub(52, 152, 219);
    	glVertex2f(45, 450);
    	glVertex2f(25, 430);
    glEnd();
    glColor3ub(52, 73, 94);
    output(70, 435, "Helix Strands", fonts[2]);

    drawSphere(35, 400, 0);
    output(70, 395, "Phosphate Base", fonts[2]);

    glBegin(GL_LINES);
        glColor3ub(46, 204, 113);
    	glVertex2f(25, 360);
    	glVertex2f(50, 360);
    glEnd();
    glColor3ub(52, 73, 94);
    output(70, 355, "Adenine", fonts[2]);

    glBegin(GL_LINES);
        glColor3ub(249, 148, 6);
    	glVertex2f(25, 320);
    	glVertex2f(50, 320);
    glEnd();
    glColor3ub(52, 73, 94);
    output(70, 315, "Thymine", fonts[2]);

    glBegin(GL_LINES);
        glColor3ub(249, 191, 59);
    	glVertex2f(25, 280);
    	glVertex2f(50, 280);
    glEnd();
    glColor3ub(52, 73, 94);
    output(70, 275, "Cytosine", fonts[2]);

    glBegin(GL_LINES);
        glColor3ub(155, 89, 182);
    	glVertex2f(25, 240);
    	glVertex2f(50, 240);
    glEnd();
    glColor3ub(52, 73, 94);
    output(70, 235, "Guanine", fonts[2]);
}

/*--------------------------------------------------------------------------------------*/

void adenineThymine()
{
	float hex1[6][2], hex2[6][2];
	float rx=512;

    GLubyte red[] = {15, 231, 76, 60};          //oxygen
    GLubyte green[] = {20, 39, 174, 96};        //carbon
    GLubyte blue[] = {20, 52, 152, 219};         //nitrogen
    GLubyte yellow[] = {10, 241, 196, 15};    //hydrogen
    getHexagonPoints(hex1, -128, 350, 75);
    getHexagonPoints(hex2, 128, 350, 75);

    glPushMatrix();

    	glTranslatef(rx, 0, 0);
    	glRotatef(theta, 0.0, 1.0, 0.0);

	    //hexagon 1
	    drawAtom(hex1[0][0], hex1[0][1], 0, green);
	    drawAtom(hex1[1][0], hex1[1][1], 0, blue);
	    drawAtom(hex1[2][0], hex1[2][1], 0, green);
	    drawAtom(hex1[3][0], hex1[3][1], 0, green);
	    drawAtom(hex1[4][0], hex1[4][1], 0, green);
	    drawAtom(hex1[5][0], hex1[5][1], 0, blue);
	    drawBondLine(hex1[0][0],hex1[0][1],0, hex1[1][0],hex1[1][1],0);
	    drawBondLine(hex1[1][0],hex1[1][1],0, hex1[2][0],hex1[2][1],0);
	    drawBondLine(hex1[2][0],hex1[2][1],0, hex1[3][0],hex1[3][1],0);
	    drawBondLine(hex1[3][0],hex1[3][1],0, hex1[4][0],hex1[4][1],0);
	    drawBondLine(hex1[4][0],hex1[4][1],0, hex1[5][0],hex1[5][1],0);
	    drawBondLine(hex1[5][0],hex1[5][1],0, hex1[0][0],hex1[0][1],0);

	    drawAtom(hex1[0][0], hex1[0][1]-50, 0, red);
	    drawBondLine(hex1[0][0],hex1[0][1],0, hex1[0][0],hex1[0][1]-50,0);

	    drawAtom(hex1[1][0]+15, hex1[1][1]-15, -50, yellow);
	    drawBondLine(hex1[1][0],hex1[1][1],0, hex1[1][0]+15,hex1[1][1]-15,-50);

	    drawAtom(hex1[2][0]+40, hex1[2][1]+40, 0, red);
	    drawBondLine(hex1[2][0],hex1[2][1],0, hex1[2][0]+40,hex1[2][1]+40,0);

	    drawAtom(hex1[3][0], hex1[3][1]+50, 0, green);
	    drawBondLine(hex1[3][0],hex1[3][1],0, hex1[3][0],hex1[3][1]+50,0);
		    drawAtom(hex1[3][0], hex1[3][1]+75, 50, yellow);
		    drawBondLine(hex1[3][0],hex1[3][1]+50,0, hex1[3][0],hex1[3][1]+75,50);
		    drawAtom(hex1[3][0]+35, hex1[3][1]+75, -35, yellow);
		    drawBondLine(hex1[3][0],hex1[3][1]+50,0, hex1[3][0]+35,hex1[3][1]+75,-35);
		    drawAtom(hex1[3][0]-35, hex1[3][1]+75, -35, yellow);
		    drawBondLine(hex1[3][0],hex1[3][1]+50,0, hex1[3][0]-35,hex1[3][1]+75,-35);

		drawAtom(hex1[4][0]-15,hex1[4][1]+15, 50, yellow);
	    drawBondLine(hex1[4][0],hex1[4][1],0, hex1[4][0]-15,hex1[4][1]+15,50);


	    //hexagon 2
	    drawAtom(hex2[0][0], hex2[0][1], 0, blue);
	    drawAtom(hex2[1][0], hex2[1][1], 0, green);
	    drawAtom(hex2[2][0], hex2[2][1], 0, green);
	    drawAtom(hex2[3][0], hex2[3][1], 0, green);
	    drawAtom(hex2[4][0], hex2[4][1], 0, blue);
	    drawAtom(hex2[5][0], hex2[5][1], 0, green);
	    drawBondLine(hex2[0][0],hex2[0][1],0, hex2[1][0],hex2[1][1],0);
	    drawBondLine(hex2[1][0],hex2[1][1],0, hex2[2][0],hex2[2][1],0);
	    drawBondLine(hex2[2][0],hex2[2][1],0, hex2[3][0],hex2[3][1],0);
	    drawBondLine(hex2[3][0],hex2[3][1],0, hex2[4][0],hex2[4][1],0);
	    drawBondLine(hex2[4][0],hex2[4][1],0, hex2[5][0],hex2[5][1],0);
	    drawBondLine(hex2[5][0],hex2[5][1],0, hex2[0][0],hex2[0][1],0);

	    drawAtom(hex2[1][0]+50, hex2[0][1]+25, 50, blue);
	    drawBondLine(hex2[1][0],hex2[1][1],0, hex2[1][0]+50,hex2[0][1]+25,50);
	    drawAtom(hex2[2][0]+50, hex2[3][1]-25, 50, blue);
	    drawBondLine(hex2[2][0],hex2[2][1],0, hex2[2][0]+50,hex2[3][1]-25,50);
		    drawAtom(hex2[2][0]+100,hex2[2][1]-37.5,75, green);
		    drawBondLine(hex2[1][0]+50, hex2[0][1]+25, 50, hex2[2][0]+100,hex2[2][1]-37.5,75);
		    drawBondLine(hex2[2][0]+50, hex2[3][1]-25, 50, hex2[2][0]+100,hex2[2][1]-37.5,75);
			    drawAtom(hex2[2][0]+142.5,hex2[2][1]-37.5,75, yellow);
			    drawBondLine(hex2[2][0]+100,hex2[2][1]-37.5,75, hex2[2][0]+142.5,hex2[2][1]-37.5,75);

	    drawAtom(hex2[3][0], hex2[3][1]+50, 0, blue);
	    drawBondLine(hex2[3][0],hex2[3][1],0, hex2[3][0],hex2[3][1]+50,0);
		    drawAtom(hex2[3][0], hex2[3][1]+75, 50, yellow);
		    drawBondLine(hex2[3][0], hex2[3][1]+50,0, hex2[3][0],hex2[3][1]+75,50);
		    drawAtom(hex2[3][0], hex2[3][1]+75, -50, yellow);
		    drawBondLine(hex2[3][0],hex2[3][1]+50,0, hex2[3][0],hex2[3][1]+75,-50);

		drawAtom(hex2[5][0]-30,hex2[5][1]-30, 0, yellow);
	    drawBondLine(hex2[5][0],hex2[5][1],0, hex2[5][0]-30,hex2[5][1]-30,0);


	    //H-bonds
	    glLineWidth(1.5);
	    glBegin(GL_LINES);
	    	glVertex3f(hex1[2][0]+40,hex1[2][1]+40,0);
	    	glVertex3f(hex2[3][0], hex2[3][1]+75, 50);

	    	glVertex3f(hex1[1][0]+15, hex1[1][1]-15, -50);
	    	glVertex3f(hex2[4][0],hex2[4][1],0);
	    glEnd();

	    glTranslatef(-rx, 0, 0);

	glPopMatrix();
	theta+=0.1;

    drawLegend();
    glColor3ub(52, 73, 94);
    output(300, 190, "Adenine-Thymine Bond", fonts[2]);


}

/*--------------------------------------------------------------------------------------*/

void cytosineGuanine()
{
    float hex1[6][2], hex2[6][2];
    float rx=512;
    //radius and color declarations
    GLubyte red[] = {15, 231, 76, 60};          //oxygen
    GLubyte green[] = {20, 39, 174, 96};        //carbon
    GLubyte blue[] = {20, 52, 152, 219};         //nitrogen
    GLubyte yellow[] = {10, 241, 196, 15};    //hydrogen
    getHexagonPoints(hex1, -192, 350, 75);
    getHexagonPoints(hex2, 192, 350, 75);

    glPushMatrix();

    	glTranslatef(rx, 0, 0);
    	glRotatef(theta, 0.0, 1.0, 0.0);

   		 //hexagon 1
	    drawAtom(hex1[0][0], hex1[0][1], 0, green);
	    drawAtom(hex1[1][0], hex1[1][1], 0, blue);
	    drawAtom(hex1[2][0], hex1[2][1], 0, green);
	    drawAtom(hex1[3][0], hex1[3][1], 0, green);
	    drawAtom(hex1[4][0], hex1[4][1], 0, green);
	    drawAtom(hex1[5][0], hex1[5][1], 0, blue);
	    drawBondLine(hex1[0][0],hex1[0][1],0, hex1[1][0],hex1[1][1],0);
	    drawBondLine(hex1[1][0],hex1[1][1],0, hex1[2][0],hex1[2][1],0);
	    drawBondLine(hex1[2][0],hex1[2][1],0, hex1[3][0],hex1[3][1],0);
	    drawBondLine(hex1[3][0],hex1[3][1],0, hex1[4][0],hex1[4][1],0);
	    drawBondLine(hex1[4][0],hex1[4][1],0, hex1[5][0],hex1[5][1],0);
	    drawBondLine(hex1[5][0],hex1[5][1],0, hex1[0][0],hex1[0][1],0);

	    drawAtom(hex1[0][0], hex1[0][1]-50, 0, red);
	    drawBondLine(hex1[0][0],hex1[0][1],0, hex1[0][0],hex1[0][1]-50,0);

	    drawAtom(hex1[2][0]+50, hex1[2][1]+25, -50, blue);
	    drawBondLine(hex1[2][0],hex1[2][1],0, hex1[2][0]+50,hex1[2][1]+25,-50);
		    drawAtom(hex1[2][0]+50,hex1[2][1]+67.5, -50, yellow);
		    drawBondLine(hex1[2][0]+50,hex1[2][1]+25,-50, hex1[2][0]+50,hex1[2][1]+67.5,-50);
		    drawAtom(hex1[2][0]+85,hex1[2][1],-50, yellow);
		    drawBondLine(hex1[2][0]+50,hex1[2][1]+25,-50, hex1[2][0]+85,hex1[2][1],-50);

	    drawAtom(hex1[3][0],hex1[3][1]+25, -50, yellow);
	    drawBondLine(hex1[3][0],hex1[3][1],0, hex1[3][0],hex1[3][1]+25,-50);

		drawAtom(hex1[4][0]-15,hex1[4][1]+15, 50, yellow);
	    drawBondLine(hex1[4][0],hex1[4][1],0, hex1[4][0]-15,hex1[4][1]+15,50);


	    //hexagon 2
	    drawAtom(hex2[0][0], hex2[0][1], 0, blue);
	    drawAtom(hex2[1][0], hex2[1][1], 0, green);
	    drawAtom(hex2[2][0], hex2[2][1], 0, green);
	    drawAtom(hex2[3][0], hex2[3][1], 0, green);
	    drawAtom(hex2[4][0], hex2[4][1], 0, blue);
	    drawAtom(hex2[5][0], hex2[5][1], 0, green);
	    drawBondLine(hex2[0][0],hex2[0][1],0, hex2[1][0],hex2[1][1],0);
	    drawBondLine(hex2[1][0],hex2[1][1],0, hex2[2][0],hex2[2][1],0);
	    drawBondLine(hex2[2][0],hex2[2][1],0, hex2[3][0],hex2[3][1],0);
	    drawBondLine(hex2[3][0],hex2[3][1],0, hex2[4][0],hex2[4][1],0);
	    drawBondLine(hex2[4][0],hex2[4][1],0, hex2[5][0],hex2[5][1],0);
	    drawBondLine(hex2[5][0],hex2[5][1],0, hex2[0][0],hex2[0][1],0);

	    drawAtom(hex2[1][0]+50, hex2[0][1]+25, 50, blue);
	    drawBondLine(hex2[1][0],hex2[1][1],0, hex2[1][0]+50,hex2[0][1]+25,50);
	    drawAtom(hex2[2][0]+50, hex2[3][1]-25, 50, blue);
	    drawBondLine(hex2[2][0],hex2[2][1],0, hex2[2][0]+50,hex2[3][1]-25,50);
		    drawAtom(hex2[2][0]+100,hex2[2][1]-37.5,75, green);
		    drawBondLine(hex2[1][0]+50, hex2[0][1]+25, 50, hex2[2][0]+100,hex2[2][1]-37.5,75);
		    drawBondLine(hex2[2][0]+50, hex2[3][1]-25, 50, hex2[2][0]+100,hex2[2][1]-37.5,75);
			    drawAtom(hex2[2][0]+142.5,hex2[2][1]-37.5,75, yellow);
			    drawBondLine(hex2[2][0]+100,hex2[2][1]-37.5,75, hex2[2][0]+142.5,hex2[2][1]-37.5,75);

	    drawAtom(hex2[3][0], hex2[3][1]+50, 0, red);
	    drawBondLine(hex2[3][0],hex2[3][1],0, hex2[3][0],hex2[3][1]+50,0);

	    drawAtom(hex2[4][0]-25, hex2[4][1]+25, 0, yellow);
	    drawBondLine(hex2[4][0],hex2[4][1],0, hex2[4][0]-25,hex2[4][1]+25,0);

		drawAtom(hex2[5][0]-50, hex2[0][1]+25, 50, blue);
	    drawBondLine(hex2[5][0],hex2[5][1],0, hex2[5][0]-50,hex2[0][1]+25,50);
		    drawAtom(hex2[5][0]-50, hex2[0][1]-15, 50, yellow);
		    drawBondLine(hex2[5][0]-50,hex2[0][1]+25,50, hex2[5][0]-50,hex2[0][1]-15,50);
		    drawAtom(hex2[5][0]-85, hex2[0][1]+60, 50, yellow);
		    drawBondLine(hex2[5][0]-50,hex2[0][1]+25,50, hex2[5][0]-85,hex2[0][1]+60,50);


	    //H-bonds
	    glLineWidth(1.5);
	    glBegin(GL_LINES);
		    glVertex3f(hex1[0][0],hex1[0][1]-50,0);
		    glVertex3f(hex2[5][0]-85,hex2[0][1]+60,50);

		    glVertex3f(hex1[1][0],hex1[1][1],0);
		    glVertex3f(hex2[4][0]-25,hex2[4][1]+25,0);

		    glVertex3f(hex1[2][0]+85,hex1[2][1],-50);
		    glVertex3f(hex2[3][0],hex2[3][1]+50,0);
		glEnd();

	    glTranslatef(-rx, 0, 0);

	glPopMatrix();
	theta+=0.1;

    drawLegend();
    glColor3ub(52, 73, 94);
    output(300, 190, "Cytosine-Guanine Bond", fonts[2]);
}


/*                              DISPLAY FUNCTION                                         */

void display(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1000, 0.0, 650,-2000,1500);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.9, 0.9, 0.9, 0.9);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    switch(page)
    {
        case -1: intro();           break;
        case  0: menu();            break;
        case  1: dna();             break;
        case  2: adenineThymine();  break;
        case  3: cytosineGuanine(); break;
    }


    glFlush();
    glutSwapBuffers();
}



/*							KEY/MOUSE-MENU FUNCTIONS									 */

void NormalKey(GLubyte key, GLint x, GLint y)
{
    //ESC to exit application
    if(key==27)
        exit(0);

    //at intro
    if(page==-1)
    {
        //if any key is pressed then goto menu
        page=0;
        glutPostRedisplay();
    }

    //at menu
    else if(page==0)
    {
        switch(key)
        {
            case '1': page=1;	break; //goto dna
            case '2': page=2;	break; //goto adenineThymine
            case '3': page=3;	break; //goto cytosineGuanine
            case 'i':
            case 'I': page=-1;	break; //go back to intro
        }
        glutPostRedisplay();
    }

    //at any page
    else
    {
        if(key=='m' || key=='M')
        {
            //go back to menu
            page=0;
            glutPostRedisplay();
        }
    }

}

/*--------------------------------------------------------------------------------------*/

void mouseMenuTrigger(int id)
{
    switch(id)
    {
        case 1: page=1;     break; //goto dna
        case 2: page=2;     break; //goto adenineThymine
        case 3: page=3;     break; //goto cytosineGuanine
        case 4: exit(0);    //exit application
    }
    glutPostRedisplay();
}

void createMouseMenu()
{
    glutCreateMenu(mouseMenuTrigger);
    glutAddMenuEntry("DNA", 1);
    glutAddMenuEntry("Adenine-Thymine", 2);
    glutAddMenuEntry("Cytosine-Guanine", 3);
    glutAddMenuEntry("Exit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/*							MAIN FUNCTION                                                */

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize(1024,768);
    glutInitWindowPosition(0,0);
    glutCreateWindow("DNA");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glEnable(GL_DEPTH_TEST);

    glutKeyboardFunc(NormalKey);
    createMouseMenu();

    glutMainLoop();
    return 0;
}
