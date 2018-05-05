#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <math.h>
#include "Recognise.h"
extern "C" {
#include <lcd.h>
}
// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2 
static inline double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt0->x;
	double dy2 = pt2->y - pt0->y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

static inline double distance(CvPoint *pt1, CvPoint *pt2)
{
	return sqrt((double)(pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y));
}

static bool sameRectangle(myRect *r1, myRect *r2, double dist)
{
	int i, j;
	int count = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(distance(&(r1->p[i]), &(r2->p[j])) < dist)
			{
				count++;
				break;
			}
		}
	}
	return (count >= 3);
}

CvRect getBoundRect(CvPoint *p, int num)
{
	int i;
	int sx = p[0].x, sy = p[0].y, lx = p[0].x, ly = p[0].y;
	for(i = 1; i < num; i++)
	{
		if(p[i].x < sx)
			sx = p[i].x;
		if(p[i].y < sy)
			sy = p[i].y;
		if(p[i].x > lx)
			lx = p[i].x;
		if(p[i].y > ly)
			ly = p[i].y;
	}
	return cvRect(sx, sy, lx - sx + 1, ly - sy + 1);
}

CvRect getBoundRect(myCircle *c)
{
	return cvRect(c->p[0] - c->p[2], c->p[1] - c->p[2], c->p[2] * 2, c->p[2] * 2);
}

static bool sameTrangle(myTrangle *r1, myTrangle *r2, double dist)
{
	int i, j;
	int count = 0;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(distance(&(r1->p[i]), &(r2->p[j])) < dist)
			{
				count++;
				break;
			}
		}
	}
	return (count >= 2);
}


//CvSeq(float [0],[1],[2])
CvSeq *isHaveCircle(const IplImage *img)
{
	IplImage* gray = cvCreateImage( cvGetSize(img), 8, 1 );
	CvMemStorage* storage = cvCreateMemStorage(0);
	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 ); // smooth it, otherwise a lot of false circles may be detected
	CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->height/4, 200, 100 );
	CvSeq *ret = cvCreateSeq(0, sizeof(CvSeq), sizeof(myCircle), storage);
	for(int i = 0; i < circles->total; i++)
	{
		float *p = (float *)cvGetSeqElem(circles, i);
		myCircle c;
		c.p[0] = p[0];
		c.p[1] = p[1];
		c.p[2] = p[2];
		c.area = 3.1415926 * p[2] * p[2];
		cvSeqPush(ret, &c);
	}
	//cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );

	return ret;
}

static const int thresh = 50;
void isHaveRectOrTran(const IplImage *img, CvSeq **rects, CvSeq **trangles)
{
	CvSeq* contours;
	int i, c, l, N = 11;
	CvSize sz = cvSize( img->width & -2, img->height & -2 );
	IplImage* timg = cvCloneImage( img ); // make a copy of input image
	IplImage* gray = cvCreateImage( sz, 8, 1 ); 
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
	IplImage* tgray;
	CvSeq* result; 
	double s, t;
	CvMemStorage *storage = cvCreateMemStorage(0);
	// create empty sequence that will contain points -
	// 4 points per square (the square's vertices)
	*rects = cvCreateSeq( 0, sizeof(CvSeq), sizeof(myRect), storage );
	*trangles = cvCreateSeq( 0, sizeof(CvSeq), sizeof(myTrangle), storage );

	// select the maximum ROI in the image
	// with the width and height divisible by 2
	// 设置感兴趣区域
	cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

	// down-scale and upscale the image to filter out the noise
	// 下采样
	cvPyrDown( timg, pyr, CV_GAUSSIAN_5x5 );
	// 上采样
	cvPyrUp( pyr, timg, CV_GAUSSIAN_5x5 );
	tgray = cvCreateImage( sz, 8, 1 );
	// find squares in every color plane of the image
	for( c = 0; c < 3; c++ )
	{
		// extract the c-th color plane
		// 设置感兴趣通道
		cvSetImageCOI( timg, c+1 );
		cvCopy( timg, tgray, 0 );

		// try several threshold levels
		for( l = 0; l < N; l++ )
		{
			// hack: use Canny instead of zero threshold level.
			// Canny helps to catch squares with gradient shading   
			// 二值化
			if( l == 0 )
			{
				// apply Canny. Take the upper threshold from slider
				// and set the lower to 0 (which forces edges merging) 
				cvCanny( tgray, gray, 0, thresh, 5 );
				// dilate canny output to remove potential
				// holes between edge segments 
				cvDilate( gray, gray, 0, 1 );
			}
			else
			{
				// apply threshold if l!=0:
				//     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
				cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
			}

			// find contours and store them all as a list
			// 查找轮廓
			cvFindContours( gray, storage, &contours, sizeof(CvContour),
				CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

			// test each contour
			while( contours )
			{
				// approximate contour with accuracy proportional
				// to the contour perimeter
				// 用指定精度逼近多边形曲线
				result = cvApproxPoly( contours, sizeof(CvContour), storage,
					CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
				// square contours should have 4 vertices after approximation
				// relatively large area (to filter out noisy contours)
				// and be convex.
				// Note: absolute value of an area is used because
				// area may be positive or negative - in accordance with the
				// contour orientation
				// 计算整个轮廓或部分轮廓的面积 > 1000 并且 测试轮廓的凸性 为凸
				if( result->total == 4 &&
					fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
					cvCheckContourConvexity(result) )
				{
					s = 0;
					for( i = 0; i < 5; i++ )
					{
						// find minimum angle between joint
						// edges (maximum of cosine)
						if( i >= 2 )
						{
							t = fabs(angle(
								(CvPoint*)cvGetSeqElem( result, i ),
								(CvPoint*)cvGetSeqElem( result, i-2 ),
								(CvPoint*)cvGetSeqElem( result, i-1 )));
							s = s > t ? s : t;
						}
					}
					// if cosines of all angles are small
					// (all angles are ~90 degree) then write quandrange
					// vertices to resultant sequence 
					if( s < 0.3 )
					{
						myRect rect;
						for( i = 0; i < 4; i++ )
						{
							CvPoint *pnt = (CvPoint*)cvGetSeqElem(result, i);
							//                            cvSeqPush( squares,
							//                                pnt);
							rect.p[i] = *pnt;
						}
						myRect picrect;
						picrect.p[0].x = 0;
						picrect.p[0].y = 0;
						picrect.p[1].x = img->width;
						picrect.p[1].y = 0;
						picrect.p[2].x = 0;
						picrect.p[2].y = img->height;
						picrect.p[3].x = img->width;
						picrect.p[3].y = img->height;
						if(!sameRectangle(&rect, &picrect, 20))
						{
							for(i = 0; i < (*rects)->total; i++)
							{
								if(sameRectangle(&rect, (myRect*)cvGetSeqElem(*rects, i), 20))
									break;
							}
							if(i >= (*rects)->total)
							{
								//								PRINTF3("c = %d, l=%d, contours = %p\n", c, l, contours);
								rect.area = fabs(cvContourArea(result,CV_WHOLE_SEQ));
								cvSeqPush(*rects, &rect);
							}
						}
					}
				}
				// 测试三角形
				else if( result->total == 3 &&
					fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
					cvCheckContourConvexity(result) )
				{
					myTrangle tran;
					for( i = 0; i < 3; i++ )
					{
						CvPoint *pnt = (CvPoint*)cvGetSeqElem(result, i);
						//                            cvSeqPush( squares,
						//                                pnt);
						tran.p[i] = *pnt;
					}
					for(i = 0; i < (*trangles)->total; i++)
					{
						if(sameTrangle(&tran, (myTrangle*)cvGetSeqElem((*trangles), i), 30))
							break;
					}
					if(i >= (*trangles)->total)
					{
						//						PRINTF3("c = %d, l=%d, contours = %p\n", c, l, contours);
						tran.area = fabs(cvContourArea(result,CV_WHOLE_SEQ));
						cvSeqPush((*trangles), &tran);
					}
				}

				// take the next contour
				contours = contours->h_next;
			}
		}
	}
	// release all the temporary images
	cvReleaseImage( &gray );
	cvReleaseImage( &pyr );
	cvReleaseImage( &tgray );
	cvReleaseImage( &timg );
	//	return squares;
}

CvSeq *isHaveRectangle(const IplImage *img)
{
	CvSeq* contours;
	int i, c, l, N = 11;
	CvSize sz = cvSize( img->width & -2, img->height & -2 );
	IplImage* timg = cvCloneImage( img ); // make a copy of input image
	IplImage* gray = cvCreateImage( sz, 8, 1 ); 
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
	IplImage* tgray;
	CvSeq* result; 
	double s, t;
	CvMemStorage *storage = cvCreateMemStorage(0);
	// create empty sequence that will contain points -
	// 4 points per square (the square's vertices)
	CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(myRect), storage );
	// select the maximum ROI in the image
	// with the width and height divisible by 2
	// 设置感兴趣区域
	cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));
	// down-scale and upscale the image to filter out the noise
	// 下采样
	cvPyrDown( timg, pyr, CV_GAUSSIAN_5x5 );
	// 上采样
	cvPyrUp( pyr, timg, CV_GAUSSIAN_5x5 );
	tgray = cvCreateImage( sz, 8, 1 );
	// find squares in every color plane of the image
	for( c = 0; c < 3; c++ )
	{
		// extract the c-th color plane
		// 设置感兴趣通道
		cvSetImageCOI( timg, c+1 );
		cvCopy( timg, tgray, 0 );
		// try several threshold levels
		for( l = 0; l < N; l++ )
		{
			// hack: use Canny instead of zero threshold level.
			// Canny helps to catch squares with gradient shading   
			// 二值化
			if( l == 0 )
			{
				// apply Canny. Take the upper threshold from slider
				// and set the lower to 0 (which forces edges merging) 
				cvCanny( tgray, gray, 0, thresh, 5 );
				// dilate canny output to remove potential
				// holes between edge segments 
				cvDilate( gray, gray, 0, 1 );
			}
			else
			{
				// apply threshold if l!=0:
				//     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
				cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
			}

			// find contours and store them all as a list
			// 查找轮廓
			cvFindContours( gray, storage, &contours, sizeof(CvContour),
				CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

			// test each contour
			while( contours )
			{
				// approximate contour with accuracy proportional
				// to the contour perimeter
				// 用指定精度逼近多边形曲线
				result = cvApproxPoly( contours, sizeof(CvContour), storage,
					CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
				// square contours should have 4 vertices after approximation
				// relatively large area (to filter out noisy contours)
				// and be convex.
				// Note: absolute value of an area is used because
				// area may be positive or negative - in accordance with the
				// contour orientation
				// 计算整个轮廓或部分轮廓的面积 > 1000 并且 测试轮廓的凸性 为凸
				if( result->total == 4 &&
					fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
					cvCheckContourConvexity(result) )
				{
					s = 0;

					for( i = 0; i < 5; i++ )
					{
						// find minimum angle between joint
						// edges (maximum of cosine)
						if( i >= 2 )
						{
							t = fabs(angle(
								(CvPoint*)cvGetSeqElem( result, i ),
								(CvPoint*)cvGetSeqElem( result, i-2 ),
								(CvPoint*)cvGetSeqElem( result, i-1 )));
							s = s > t ? s : t;
						}
					}

					// if cosines of all angles are small
					// (all angles are ~90 degree) then write quandrange
					// vertices to resultant sequence 
					if( s < 0.3 )
					{
						myRect rect;
						for( i = 0; i < 4; i++ )
						{
							CvPoint *pnt = (CvPoint*)cvGetSeqElem(result, i);
							//                            cvSeqPush( squares,
							//                                pnt);
							rect.p[i] = *pnt;
						}
						myRect picrect;
						picrect.p[0].x = 0;
						picrect.p[0].y = 0;
						picrect.p[1].x = img->width;
						picrect.p[1].y = 0;
						picrect.p[2].x = 0;
						picrect.p[2].y = img->height;
						picrect.p[3].x = img->width;
						picrect.p[3].y = img->height;
						if(!sameRectangle(&rect, &picrect, 20))
						{
							for(i = 0; i < squares->total; i++)
							{
								if(sameRectangle(&rect, (myRect*)cvGetSeqElem(squares, i), 20))
									break;
							}
							if(i >= squares->total)
							{
								//								PRINTF3("c = %d, l=%d, contours = %p\n", c, l, contours);
								rect.area = fabs(cvContourArea(result,CV_WHOLE_SEQ));
								cvSeqPush(squares, &rect);
							}
						}
					}
				}

				// take the next contour
				contours = contours->h_next;
			}
		}
	}

	// release all the temporary images
	cvReleaseImage( &gray );
	cvReleaseImage( &pyr );
	cvReleaseImage( &tgray );
	cvReleaseImage( &timg );

	return squares;
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
CvSeq* isHaveTrangle( IplImage* img)
{
	CvSeq* contours;
	int i, c, l, N = 11;
	CvSize sz = cvSize( img->width & -2, img->height & -2 );
	IplImage* timg = cvCloneImage( img ); // make a copy of input image
	IplImage* gray = cvCreateImage( sz, 8, 1 ); 
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
	IplImage* tgray;
	CvSeq* result; 
	//	double s, t;
	CvMemStorage *storage = cvCreateMemStorage(0);
	// create empty sequence that will contain points -
	// 4 points per square (the square's vertices)
	CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(myTrangle), storage );

	// select the maximum ROI in the image
	// with the width and height divisible by 2
	// 设置感兴趣区域
	cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

	// down-scale and upscale the image to filter out the noise
	// 下采样
	cvPyrDown( timg, pyr, CV_GAUSSIAN_5x5 );
	// 上采样
	cvPyrUp( pyr, timg, CV_GAUSSIAN_5x5 );
	tgray = cvCreateImage( sz, 8, 1 );

	// find squares in every color plane of the image
	for( c = 0; c < 3; c++ )
	{
		// extract the c-th color plane
		// 设置感兴趣通道
		cvSetImageCOI( timg, c+1 );
		cvCopy( timg, tgray, 0 );

		// try several threshold levels
		for( l = 0; l < N; l++ )
		{
			// hack: use Canny instead of zero threshold level.
			// Canny helps to catch squares with gradient shading   
			// 二值化
			if( l == 0 )
			{
				// apply Canny. Take the upper threshold from slider
				// and set the lower to 0 (which forces edges merging) 
				cvCanny( tgray, gray, 0, thresh, 5 );
				// dilate canny output to remove potential
				// holes between edge segments 
				cvDilate( gray, gray, 0, 1 );
			}
			else
			{
				// apply threshold if l!=0:
				//     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
				cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
			}

			// find contours and store them all as a list
			// 查找轮廓
			cvFindContours( gray, storage, &contours, sizeof(CvContour),
				CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

			// test each contour
			while( contours )
			{
				// approximate contour with accuracy proportional
				// to the contour perimeter
				// 用指定精度逼近多边形曲线
				result = cvApproxPoly( contours, sizeof(CvContour), storage,
					CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
				// square contours should have 4 vertices after approximation
				// relatively large area (to filter out noisy contours)
				// and be convex.
				// Note: absolute value of an area is used because
				// area may be positive or negative - in accordance with the
				// contour orientation
				// 计算整个轮廓或部分轮廓的面积 > 1000 并且 测试轮廓的凸性 为凸
				if( result->total == 3 &&
					fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
					cvCheckContourConvexity(result) )
				{
					myTrangle tran;
					for( i = 0; i < 3; i++ )
					{
						CvPoint *pnt = (CvPoint*)cvGetSeqElem(result, i);
						//                            cvSeqPush( squares,
						//                                pnt);
						tran.p[i] = *pnt;
					}
					for(i = 0; i < squares->total; i++)
					{
						if(sameTrangle(&tran, (myTrangle*)cvGetSeqElem(squares, i), 30))
							break;
					}
					if(i >= squares->total)
					{
						//						PRINTF3("c = %d, l=%d, contours = %p\n", c, l, contours);
						tran.area = fabs(cvContourArea(result,CV_WHOLE_SEQ));
						cvSeqPush(squares, &tran);
					}
				}
				// take the next contour
				contours = contours->h_next;
			}
		}
	}

	// release all the temporary images
	cvReleaseImage( &gray );
	cvReleaseImage( &pyr );
	cvReleaseImage( &tgray );
	cvReleaseImage( &timg );

	return squares;
}
SHAPE getShape(IplImage *img, CvRect *rect)
{
	CvRect rc;
	SHAPE shape;
	CvSeq *circles = isHaveCircle(img);
	CvSeq *rectangles, *trangles;
	isHaveRectOrTran(img, &rectangles, &trangles);
	int index;
	myCircle lc;
	myTrangle lt;
	myRect lr;
	long ca = 0, ra = 0, ta = 0;
	WIN_HANDLE win = TFT_CreateWindowEx(0, 0, 320, 240, COLOR_BLACK);
	for(index = 0; index < circles->total; index++)
	{
		myCircle *c = (myCircle *)cvGetSeqElem(circles, index);
		TFT_SetColor(win, RGB(0, 255, 0));
		TFT_Circle(win, c->p[0], 240 - c->p[1], 3, 0);
		TFT_SetColor(win, RGB(255, 0, 0));
//		TFT_Circle(win, c->p[0], 240 - c->p[1], c->p[2]-1, 1);
		TFT_Circle(win, c->p[0], 240 - c->p[1], c->p[2], 1);
//		TFT_Circle(win, c->p[0], 240 - c->p[1], c->p[2]+1, 1);
		if(ca < c->area)
		{
			ca = c->area;
			lc = *c;
		}
	}
	for(index = 0; index < rectangles->total; index++)
	{
		int count = 4;
		myRect *rect = (myRect *)cvGetSeqElem(rectangles, index);
		CvPoint *p = rect->p;
		TFT_SetColor(win, RGB(255, 0, 0));
		TFT_Line(win, rect->p[0].x, 240 - rect->p[0].y, rect->p[1].x, 240 - rect->p[1].y);
		TFT_Line(win, rect->p[1].x, 240 - rect->p[1].y, rect->p[2].x, 240 - rect->p[2].y);
		TFT_Line(win, rect->p[2].x, 240 - rect->p[2].y, rect->p[3].x, 240 - rect->p[3].y);
		TFT_Line(win, rect->p[3].x, 240 - rect->p[3].y, rect->p[0].x, 240 - rect->p[0].y);
		if(ra < rect->area)
		{
			ra = rect->area;
			lr = *rect;
		}
	}
	for(index = 0; index < trangles->total; index++)
	{
		myTrangle *t = (myTrangle *)cvGetSeqElem(trangles, index);
		TFT_SetColor(win, RGB(255, 0, 0));
		TFT_Line(win, t->p[0].x, 240 - t->p[0].y, t->p[1].x, 240 - t->p[1].y);
		TFT_Line(win, t->p[1].x, 240 - t->p[1].y, t->p[2].x, 240 - t->p[2].y);
		TFT_Line(win, t->p[2].x, 240 - t->p[2].y, t->p[0].x, 240 - t->p[0].y);

		if(ta < t->area)
		{
			ta = t->area;
			lt = *t;
		}
	}
	if((ca == ra) && (ca == ta)){
		rc.x=0;
		rc.y=0;
		rc.width=0;
		rc.height=0;
		shape=NOSHAPE;
	}
	else
	{
		static const char *shapeMap[] = {
			"Circle",
			"Rectangle",
			"Trangle",
		};
		if(ca > ta)
		{
			if(ca > ra)
			{
				shape = CIRCLE;
				rc = getBoundRect(&lc);
			}
			else
			{
				shape = RECTANGLE;
				rc = getBoundRect(lr.p, 4);
			}
		}
		else
		{
			if(ta > ra)
			{
				shape = TRANGLE;
				rc = getBoundRect(lt.p, 3);
			}
			else
			{
				shape = RECTANGLE;
				rc = getBoundRect(lr.p, 4);
			}
		}
	}
	*rect = rc;
	return shape;
}

int colorParseHsv( IplImage * src)
{	
	if( !src )
		return -1;
	IplImage* hsv = 0;
	IplImage* h_plane = 0;
	IplImage* s_plane = 0;
	IplImage* v_plane = 0;
	unsigned long r_count=0;
	unsigned long g_count=0;
	unsigned long b_count=0;
	int color=NOCOLOR;
	
	hsv = cvCreateImage( cvGetSize(src), src->depth, 3 );
	h_plane = cvCreateImage( cvGetSize(src), src->depth, 1 );
	s_plane = cvCreateImage( cvGetSize(src), src->depth, 1 );
	v_plane = cvCreateImage( cvGetSize(src), src->depth, 1 );
	IplImage* planes[] = { h_plane, s_plane };
	/** H 分量划分为16个等级，S分量划分为8个等级 */
	int h_bins = 3, s_bins =8;
	int hist_size[] = {h_bins, s_bins};

	/** H 分量的变化范围 */
	float h_ranges[] = { 0, 180 }; 

	/** S 分量的变化范围*/
	float s_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges };

	/** 输入图像转换到HSV颜色空间 */
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	/** 创建直方图，二维, 每个维度上均分 */
	CvHistogram * hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	/** 根据H,S两个平面数据统计直方图 */
	cvCalcHist( planes, hist, 0, 0 );

	/** 获取直方图统计的最大值，用于动态显示直方图 */
	float max_value;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );

	/** 设置直方图显示图像 */
	int height = 240;
	int width = (h_bins*s_bins*6);
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );

	/** 用来进行HSV到RGB颜色转换的临时单位图像 */
	IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
	int bin_w = width / (h_bins * s_bins);
	for(int h = 0; h < h_bins; h++)
	{
		for(int s = 0; s < s_bins; s++)
		{
			int i = h*s_bins + s;
			/** 获得直方图中的统计次数，计算显示在图像中的高度 */
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			int intensity = cvRound(bin_val*height/max_value);

			if( h==0 && ( s == 5 || s==6 || s==7 ) ){
				r_count +=bin_val;
			}
			if( h==1 && ( s == 5 || s==6 || s==7 || s==4 || s==3 || s==2 || s==1) ){
				g_count +=bin_val;
			}
			if( h==2 && ( s == 5 || s==6 || s==7 || s==4 || s==3)){
				b_count +=bin_val;
			}
		}
	}
	if( r_count >2000 ){
		color=RED;
	}else{		
		if( r_count <100 && g_count>100 && b_count < 10){
			color=GREEN;
		}
		if(	r_count <100 && b_count > 100){
			color=BLUE;
		}
	}
	return color;
}

int Max( int x ,int y){
	return x>y?x:y;
}
int Min( int x,int y){
	return x>y?y:x;
}

COLORMODE colorParseRgb(IplImage * frame)
{
	if( !frame )
		return NOCOLOR;
	unsigned long Rcount=0;
	unsigned long Gcount=0;
	unsigned long Bcount=0;
	uchar *ptr;
	COLORMODE color = NOCOLOR;
	ptr = (uchar*)frame->imageData;
	for ( int i = 0; i < frame->height; i ++ )
	{
		for ( int j = 0; j < frame->width; j ++ )
		{
			/* black : r=0, g=0,b=0 */
			uchar r, g, b;
			b = *(ptr++);		g = *(ptr++);		r = *(ptr++);
			if( Max(Max(r,g),b)-Min( Min(r,g),b)>50){
				if( r>g  && r>b && r>120)
					Rcount++;
				if( g>r && g>b && g>120)
					Gcount++;
				if( b>g && b>r && b>120)
					Bcount++;
			}
		}
	}
	Rcount= Rcount/100;
	Gcount= Gcount/100;
	Bcount= Bcount/100;
#if 0
	if( Rcount > 2000 ){
		color=RED;
	}else{		
		if( Rcount <500 && Gcount>2000 && Bcount < 500){
			color=GREEN;
		}
		if(	Rcount <500 && Bcount > 100){
			color=BLUE;
		}
	}
#else
	if( Rcount>Bcount && Rcount>Gcount)
		color=RED;
	if( Gcount>Bcount && Gcount>Rcount )
		color=GREEN;
	if( Bcount>Gcount && Bcount>Rcount )
		color=BLUE;
#endif
//	printf("Rcount:%ul\n",Rcount);
//	printf("Gcount:%ul\n",Gcount);
//	printf("Bcount:%ul\n",Bcount);
	return color;
}
IplImage* CutImage(IplImage* src )
{
//	IplImage * src0= cvLoadImage("src.jpg");
	IplImage *dst;
	CvSize dst_cvsize;
	float scale = 3;
	/** Resize usefull img to bigger one. */
	dst_cvsize.width = src->width * scale;		
	dst_cvsize.height = src->height * scale;
	dst = cvCreateImage( dst_cvsize, src->depth, src->nChannels);	
	cvResize(src, dst, CV_INTER_LINEAR);	
	/** catch usefull img from resized img*/
	cvSetImageROI( dst, cvRect(dst_cvsize.width/3,dst_cvsize.height/3 ,\
					dst_cvsize.width/3,dst_cvsize.height/3)); 
	return dst;
}


