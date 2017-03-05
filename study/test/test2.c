#if   1

#include <stdio.h>

void MP_Ellipse(int xc , int yc , int a, int b)

{

　　double sqa = a * a;

　　double sqb = b * b;

　　double d = sqb + sqa * (-b + 0.25);

　　int x = 0;

　　int y = b;

　　EllipsePlot(xc, yc, x, y);


　　while( sqb * (x + 1) < sqa * (y - 0.5))
　　{

	　　if (d < 0)
	　　{

	　	　d += sqb * (2 * x + 3);

	　　}

	　　else{

		　　d += (sqb * (2 * x + 3) + sqa * (-2 * y + 2));

		　　y--;

	　　}

　　		x++;

　　		EllipsePlot(xc, yc, x, y);

　　}


　　d = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a * b) * 2;

　　while(y > 0)

　　{

	　　if (d < 0)
		{

		　　d += sqb * (2 * x + 2) + sqa * (-2 * y + 3);

		　　x++;

	　　}else{

	　　		d += sqa * (-2 * y + 3);

	　　}

	　　y--;

	　　EllipsePlot(xc, yc, x, y);

　　}

}


int main()
{
	printf("hello world\n");

	return 0;
}

#else  



    #include "stdafx.h"  
	#include <stdio.h>
	#include "graphics.h" 
	#include "conio.h"     
      
    #include "GL/glut.h"  
    #include "stdlib.h"  
      
    void init()  
    {  
        glClearColor(1.0,1.0,1.0,0.0);  
          
        glMatrixMode(GL_PROJECTION);  
        gluOrtho2D(0.0,600.0,0.0,400.0);  
    }  
      
    inline int round(int x)  
    {  
        return int(x+0.5);  
    }  
      
    void setPixel(int x, int y)  
    {  
        glBegin(GL_POINTS);  
        glVertex2i(x,y);  
        glEnd();  
    }  
      
    void ellipsePlotPoints(int xc,int yc,int x,int y)  
    {  
        setPixel(xc+x,yc+y);  
        setPixel(xc-x,yc+y);  
        setPixel(xc-x,yc-y);  
        setPixel(xc+x,yc-y);  
    }  
      
    void ellipseMidPoint(int xc,int yc,int rx,int ry)  
    {  
        int x, y, p1,p2;  
        int rx2=rx*rx,ry2=ry*ry;  
        x=0;  
        y=ry;  
      
        /*region 1*/  
        ellipsePlotPoints(xc,yc,x,y);  
        p1=round(rx2-(rx2*ry)+(0.25*rx2));  
        while(ry2*x<=rx2*y)  
        {  
            if(p1<=0)  
            {  
                p1+=ry2*(2*x+3);  
                x++;  
            }  
            else  
            {  
                p1+=ry2*(2*x+3)+rx2*(2-2*y);  
                x++;  
                y--;  
            }  
             ellipsePlotPoints(xc,yc,x,y);  
        }  
      
        /*region 2*/  
        p2=round(ry2*(x+0.5)*(x+0.5)+rx2*(y-1)*(y-1)-rx2*ry2);  
        while(y>=0)  
        {  
            if(p2<=0)  
            {  
                p2+=ry2*(2*x+2)+rx2*(3-2*y);  
                x++;  
                y--;  
            }  
            else  
            {  
                p2+=rx2*(3-2*y);  
                y--;  
            }  
            ellipsePlotPoints(xc,yc,x,y);  
        }  
    }  
      
    void render()  
    {  
        glClear(GL_COLOR_BUFFER_BIT);  
        glColor3f(1.0,0.0,0.0);  
          
        ellipseMidPoint(200,200,180,150);  
        ellipseMidPoint(400,200,80,160);  
          
        glFlush();  
    }  
      
    int main(int argc, char* argv[])  
    {  
        glutInit(&argc,argv);  
        glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);  
        glutInitWindowPosition(50,100);  
        glutInitWindowSize(600,400);  
        glutCreateWindow("ellipse");  
          
        init();  
        glutDisplayFunc(render);  
        glutMainLoop();  
          
        return 0;  
    }  

#endif
