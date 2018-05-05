/** DEFINE */
#define DEBUG
#define FLAG 1
//#define NOCOLOR 100
//#define RED		0
//#define GREEN	1
//#define BLUE	2
#ifdef DEBUG
#define DbugPrintf(format,arg) printf(format,##arg);
#else 
#define DbugPrintf 
#endif 

typedef struct myCircle {
	int p[3];
	long area;
} myCircle;

typedef struct myRect {
	CvPoint p[4];
	long area;
} myRect;

typedef struct myTrangle {
	CvPoint p[3];
	long area;
} myTrangle;
typedef enum { CIRCLE = 0, RECTANGLE, TRANGLE ,NOSHAPE} SHAPE;
typedef enum { RED = 0, GREEN, BLUE ,NOCOLOR} COLORMODE;

extern IplImage* CutImage(IplImage* src );
extern SHAPE getShape(IplImage *img, CvRect *rect);
extern int colorParseHsv( IplImage * src);
extern COLORMODE colorParseRgb(IplImage * frame);

