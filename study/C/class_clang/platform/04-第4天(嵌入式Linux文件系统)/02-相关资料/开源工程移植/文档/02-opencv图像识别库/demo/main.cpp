#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <math.h>
#include "Recognise.h"
extern "C" {
#include <lcd.h>
}

int main( int argc, char** argv )
{
	const char *srcname = "src.jpg";
	if(argc > 1)
		srcname = argv[1];
	IplImage * src0= cvLoadImage(srcname);
	IplImage * tmpImg,*src,*dst;
	CvSize dst_cvsize;
	float scale = 3;
	int color=NOCOLOR;
	CvRect rect;
	SHAPE shap; 
	TFT_Init();
	/** getShape 0--circle 1--triangle */
	/**	2--rectangle 3--noneshape */
	shap=getShape(src0, &rect);
	/** none shape found! */
	if( !rect.x && !rect.y && !rect.width && !rect.height)
		goto End;
	/** catch usefull img from source img. */
	cvSetImageROI( src0, cvRect( rect.x,rect.y ,rect.width,\
					rect.height )); 
	/** catch usefull img from resize img*/
	tmpImg=CutImage(src0);
	/**	parse color with img */
#ifdef HSV_PARSE
	color=colorParseHsv(tmpImg);
#else 
	color=colorParseRgb(tmpImg);
#endif
	static const char *shapMap[] = {
		"Circle", "Rectangle", "Triangle", NULL
	};
	static const char *colorMap[] = {
		"Red", "Green", "Blue", "unable to distinguish", NULL
	};
End:
	switch(shap)
	{
	case CIRCLE:
	case RECTANGLE:
	case TRANGLE:
		printf("The Shap is %s, ", shapMap[shap]);
		printf("The Color is %s\n", colorMap[color]);
		break;
	default:
		printf("No Shap Found!\n");
		break;
	}
	return 0 ;
}

