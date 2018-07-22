
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/fttrigon.h>
#include <freetype/ftstroke.h>

extern int char2uni(const unsigned char *rawstring, int boundlen,unsigned short *uni);

//char *UNILangPath = "/usr/freetype/lib/font/simfang.ttf";
char *UNILangPath = "simfang.TTF";

void clear_screen(void)
{// ESC[2J
    printf("\033[2J");
    fflush(stdout);
}
void cusor_moveto(int x, int y)
{// ESC[y;xH
    printf("\033[%d;%dH",y,x);
    fflush(stdout);
} 
int Ptext(unsigned short inptr,unsigned int fsize)
{

	unsigned short ucode = inptr;
	unsigned int psize = fsize;
	int row, pixel;
	
	FT_Library library;
	FT_Face face;
	FT_UInt glyph_index;

	printf("unicode = %x, size %d\t", ucode,psize);

	if(FT_Init_FreeType(&library))
		return 11;

	if(FT_New_Face(library, UNILangPath,0, &face))
		return 12;

	if(FT_Set_Pixel_Sizes(face, psize,0))
		return 13;

	glyph_index = FT_Get_Char_Index(face, ucode);
	if(glyph_index == 0)
	{
		return 2;
	}
	
	if(FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT))
	{
		return 3;
	}
	
	if(FT_Render_Glyph(face->glyph,FT_RENDER_MODE_MONO))
	{
		return 4;
	}
	
	printf("bitmap:left=%d\t"
	"top=%d\t"
	"rows=%d\t"
	"width=%d\t"
	"pitch=%d\t"
	"pixel_mode=%d\n",
	face->glyph->bitmap_left,
	face->glyph->bitmap_top,
	face->glyph->bitmap.rows,
	face->glyph->bitmap.width,
	face->glyph->bitmap.pitch,
	face->glyph->bitmap.pixel_mode);
	//printf("\n\n");
	
#if 1
	for(row = 0;row<(face->glyph->bitmap.rows - face->glyph->bitmap_top);++row)
	{
		for(pixel = 0; pixel<face->glyph->bitmap.width; ++pixel)
		{
			printf(" ");
		}
		printf("\n");
	}
	
	for(row = 0; row < face->glyph->bitmap.rows; ++row)
	{
		for(pixel = 0; pixel < face->glyph->bitmap_left; ++pixel)
			printf(" ");
		
		for(pixel = 0; pixel < face->glyph->bitmap.width; ++pixel)
		{//显示正文
			printf("%c", (face->glyph->bitmap.buffer[row * face->glyph->bitmap.pitch +pixel/8] & (0xC0 >> (pixel % 8)))?'0':' ');
		}
		printf("\n");
	}
#else
	for(row = 0;row<(face->glyph->bitmap.rows - face->glyph->bitmap_top);++row)
	{
		for(pixel = 0; pixel<face->glyph->bitmap.width; ++pixel)
		{
			printf("#");//绘制距离顶部的空余像素点
		}
		printf("\n");
	}
	
	for(row = 0; row < face->glyph->bitmap.rows; ++row)
	{
		for(pixel = 0; pixel < face->glyph->bitmap_left; ++pixel)
			printf("*"); //绘制距离左边的空余像素点
		
		for(pixel = 0; pixel < face->glyph->bitmap.width; ++pixel)
		{//显示正文
			printf("%c", (face->glyph->bitmap.buffer[row * face->glyph->bitmap.pitch +pixel/8] & (0xC0 >> (pixel % 8)))?'0':'_');
		}
		printf("\n");
	}
#endif	
	return 0;
}

int main()
{
	int cont = 12;
	int ret;
	unsigned short uni;
	
	char2uni("中",2,&uni);
	while(1)
	{
		clear_screen();
		cusor_moveto(0,0);
		cont+=3;
		
		ret = Ptext(uni,cont);
		if(ret != 0)
			printf("\nError %d\n",ret);
		sleep(1);
		if(cont >= 32)
			cont = 12;
		
	}
}
