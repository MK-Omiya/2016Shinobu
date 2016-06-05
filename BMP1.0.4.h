//�ڍׂ͈�ԉ��ɏ����Ă���܂��B
/*
###����###
�g�p����ۂ�[CPP]�`���ŕۑ����Ă��������B
�X�V:2016/01/11::[���񂢂냂�U�C�N]Rhodanthe*�͂������I[130K]
*/
#ifndef MK1024_LIB_2015_10_11_03_06
#define MK1024_LIB_2015_10_11_03_06
//MK_AC[2015-10-11[C-GCUS][T-KNMS]]
//MK_AC2::2016/01/11::[���񂢂냂�U�C�N]Rhodanthe*�͂������I[130K]
#include<stdio.h>
#include<stdlib.h>



//�s�N�Z���f�[�^
//�u�F�v�������ϐ�
typedef struct{
    unsigned char R;
	unsigned char G;
	unsigned char B;
}RGB;



int bmpOut(char* FileName,int Wd,int Ht,RGB *rgb,int showinfo = 0){
	unsigned char color[4];
	//FileHeader
	const char bfType[2] 				= {'B','M'};//BitMap����[
	unsigned long  bfSize;					//�t�@�C���T�C�Y Wd*Ht*4+54
	const unsigned short bfReserved1	= 0x00;//�\��̈� ���0
	const unsigned short bfReserved2	= 0x00;//�\��̈� ���0
	const unsigned long  bfOffBits		= 54;//�t�@�C���I�t�Z�b�g
	
	//BitmapInformation
	const unsigned long  biSize			=40;//�r�b�g�}�b�v���T�C�Y
	long biWidth;							//����
	long  biHeight;							//����
	const unsigned short biPlanes		=1; //�v���[���T�C�Y�@���1
	const unsigned short biBitCount		=24;//1��f������̃f�[�^�T�C�Y(32BitTrueColor�ɐݒ�)*************************************[�ύX��][default:32]
	const unsigned long  biCompression	=0;	//���k(�����k�Ȃ̂�0���w��)
	const unsigned long  biSizeImage	=0; //�摜dpi�����g��Ȃ�
	const long biXPixPerMeter			=0;	//1m�������dpi(x)�g��Ȃ�
	const long biYPixPerMeter			=0;	//1m�������dpi(y)�g��Ȃ�
	const unsigned long  biClrUsed		=0;	//�d�l�p���b�g�� �g��Ȃ�
	const unsigned long  biClrImporant	=0;	//�d�v�p���b�g�C���f�b�N�X �g��Ȃ�
	
	if(showinfo != 0){
		printf("OutPutFileName:%s\n",FileName);
		printf("SIZE : %d x %d\n",Wd,Ht);
	}
	FILE *f = fopen(FileName,"wb");
	if(f == NULL){
		return -1;
	}
	bfSize = (unsigned long)(Wd*Ht*4+54);
	biWidth =(long)Wd;
	biHeight=(long)Ht;
	
	//*****�t�@�C���w�b�_����������*****
	fwrite(bfType,sizeof(unsigned char),2,f);			//BM(BitMap)
	fwrite(&bfSize,sizeof(unsigned long),1,f);			//�t�@�C���T�C�Y Wd*Ht*4+54
	fwrite(&bfReserved1,sizeof(unsigned short),1,f);	//�\��̈� ���0
	fwrite(&bfReserved2,sizeof(unsigned short),1,f);	//�\��̈� ���0
	fwrite(&bfOffBits,sizeof(unsigned long),1,f);		//�t�@�C���I�t�Z�b�g
	
	//*****�r�b�g�}�b�v������������*****
	fwrite(&biSize,sizeof(unsigned long),1,f);			//�r�b�g�}�b�v���T�C�Y
	fwrite(&biWidth,sizeof(long),1,f);					//����
	fwrite(&biHeight,sizeof(long),1,f);					//����
	fwrite(&biPlanes,sizeof(unsigned short),1,f);		//�v���[���T�C�Y(���1)
	fwrite(&biBitCount,sizeof(unsigned short),1,f);		//1��f������̃f�[�^�T�C�Y(32BitTrueColor�ɐݒ�)
	fwrite(&biCompression,sizeof(unsigned long),1,f);	//���k(�����k�Ȃ̂�0���w��)
	fwrite(&biSizeImage,sizeof(unsigned long),1,f);		//�摜dpi�����g��Ȃ�
	fwrite(&biXPixPerMeter,sizeof(long),1,f);			//1m�������dpi(x)�g��Ȃ�
	fwrite(&biYPixPerMeter,sizeof(long),1,f);			//1m�������dpi(y)�g��Ȃ�
	fwrite(&biClrUsed,sizeof(unsigned long),1,f);		//�d�l�p���b�g�� �g��Ȃ�
	fwrite(&biClrImporant,sizeof(unsigned long),1,f);	//�d�v�p���b�g�C���f�b�N�X �g��Ȃ�
	
	//���e����������
	
	for(int i = 0;i<=Wd*Ht;i++){
		color[0] = (rgb+i)->B;
		color[1] = (rgb+i)->G;
		color[2] = (rgb+i)->R;
		color[3] = 0;
		
		fwrite(&color[0],sizeof(unsigned char),1,f);
		fwrite(&color[1],sizeof(unsigned char),1,f);
		fwrite(&color[2],sizeof(unsigned char),1,f);
		//fwrite(&color[3],sizeof(unsigned char),1,f);
	}
	//fwrite(rgb,sizeof(RGB),Wd*Ht,f);
	
	
	
	fclose(f);
	
	return 0;
}


//�摜��ǂݍ���(24Bit bitmap �񈳏k)
int bmpIn(char* FileName,int* WdAdr,int* HtAdr,RGB* rgb,int showinfo = 0){
	
	static unsigned char* pixels;
	static RGB* rgbd;

	static int FileSize;
	static int Width;
	static int Height;
	static int BitCount;
	
	FILE *fmain;
	fmain = fopen(FileName,"rb");
	if(fmain == NULL){
		printf("�t�@�C���ǂݍ��݃G���[(%s)\n",FileName);
		return  -1;
	}
	
	fseek(fmain, 2, SEEK_SET);
	fread(&FileSize,1,4,fmain);
	fseek(fmain, 18L, SEEK_SET);
	fread(&Width, 1, 4,fmain);
	fread(&Height,1, 4,fmain);
	fseek(fmain, 28L, SEEK_SET);
	fread(&BitCount,1, 2,fmain);
		
	rewind(fmain);
	fseek(fmain, 54L, SEEK_SET);
	if(showinfo == 1){
		printf("SIZE  :%d\n",FileSize);
		printf("WIDTH :%ld\n",Width);
		printf("HEIGHT:%ld\n",Height);
		printf("COLBIT:%ld\n",BitCount);
	}

	pixels = (unsigned char*)malloc(Width*Height*3);
	rgbd   = (RGB*)malloc(Width*Height*3);
	//rgb = (RGB*)malloc(Width*Height*3);
	
	fread(pixels, sizeof(char),FileSize-54 , fmain);
	
	
	
	for(int i=0;i<Width*Height;i++){
		rgbd[i].B = pixels[i*3];
		rgbd[i].G = pixels[(i*3)+1];
		rgbd[i].R = pixels[(i*3)+2];
	}
	
	//BmpOut("���񂢂�f�o�b�O.bmp",640,480,rgbd);
	
	*WdAdr = Width;
	*HtAdr = Height;
	for(int i=0;i<Width*Height;i++){
		rgb[i].B = rgbd[i].B;
		rgb[i].G = rgbd[i].G;
		rgb[i].R = rgbd[i].R;
	}
	free(rgbd);
	fclose(fmain);
	return 1;
}

//�摜�t�@�C���̏c���T�C�Y���擾
int getImgSize(char* path,int* width,int* height){
	static int FileSize;
	static int BitCount;
	
	FILE *fmain;
	fmain = fopen(path,"rb");
	if(fmain == NULL){
		printf("�t�@�C���ǂݍ��݃G���[(%s)\n",path);
		return  -1;
	}
	
	fseek(fmain, 2, SEEK_SET);
	fread(&FileSize,1,4,fmain);
	fseek(fmain, 18L, SEEK_SET);
	fread(width, 1, 4,fmain);
	fread(height,1, 4,fmain);
	fseek(fmain, 28L, SEEK_SET);
	fread(&BitCount,1, 2,fmain);
	fclose(fmain);
	return 1;	
}





//x,y ���W���牡�T�C�Y�����ƂɃA�h���X�ɕϊ����� 
int imgAddr(int x,int y,int Wd){
	return Wd * y + x;
}

//�w�肵��x,y���W�̐F��ݒ肷��Br,g,b�𐔒l�Ŏw��
void setColor(RGB* pix,int width,int x,int y,int r,int g,int b){
	pix[imgAddr(x,y,width)].R = r;
	pix[imgAddr(x,y,width)].G = g;
	pix[imgAddr(x,y,width)].B = b;
}

//�w�肵��x,y���W�̐F��ݒ肷��BRGB�^�Ŏw��
void setColor2(RGB* pix,int width,int x,int y,RGB rgb){
	pix[imgAddr(x,y,width)].R = rgb.R;
	pix[imgAddr(x,y,width)].G = rgb.G;
	pix[imgAddr(x,y,width)].B = rgb.B;
}

//RGB�^�̐F�𔽓]������A�l�K������B
RGB invColor(RGB rgb){
	RGB rRGB;
	rRGB.R = (255 - rgb.R);
	rRGB.G = (255 - rgb.G);
	rRGB.B = (255 - rgb.B);
	return rRGB;	
}

//���l��RGB�^�ɕϊ�����B
RGB numToRgb(int r,int g,int b){
	RGB rgb;
	rgb.R = r;
	rgb.G = g;
	rgb.B = b;	
	return rgb;	
}

//�w�肵��x,y���W�̐F���擾����
RGB getColor(RGB* pix,int width,int x,int y){
	RGB rrgb;
	rrgb.R = pix[imgAddr(x,y,width)].R;
	rrgb.G = pix[imgAddr(x,y,width)].G;
	rrgb.B = pix[imgAddr(x,y,width)].B;
	return rrgb;
}

//�摜����ɒ���t����
int setPicture(RGB* pdata,char* path,int width,int x,int y){
	RGB* pixRgb;
	int picWidth,picHeight;
	
	getImgSize(path,&picWidth,&picHeight);
	pixRgb = (RGB*)malloc(picWidth*picHeight*3);
	bmpIn(path,&picWidth,&picHeight,pixRgb);
	
	RGB tmp;
	
	for(int xx=x;xx<x+picWidth;xx++){
		for(int yy=y;yy<y+picHeight;yy++){
			tmp = getColor(pixRgb,picWidth,xx-x,yy-y);
			setColor(pdata,width,xx,yy,tmp.R,tmp.G,tmp.B);
		}
	}
	
	free(pixRgb);
	return 0;
}

/*
[�ڍ�]
+---------------------------------------------------------------
|�֐� int BmpOut(char* FileName,int Wd,int Ht,RGB *rgb,int showinfo = 0)
+---------------------------------------------------------------
|FileName 	�ۑ�����t�@�C����
|Wd			�ۑ�����摜�̉���
|Ht			�ۑ�����摜�̍���
|*rgb		�F�f�[�^
|showinfo	����\�����邩�ǂ���
|�A��l�F����:0 ���s:���̑�
+---------------------------------------------------------------
�F�f�[�^�̏ڍ�
typedef struct tagRGBQUAD{
    unsigned char R;
	unsigned char G;
	unsigned char B;
}RGB;

���̂悤�ȍ\���̂ɂȂ��Ă���B
�g�p����ۂ͎����I�ɏ������ގg�p�̂��� R,G,B�̏��Ԃ͋C�ɂ��Ȃ��Ă��悢

�g�p���@
rgb�ɂ��Ă͏������݂����摜�t�@�C���́u�c�~���v���̔z������
[�������J�n�n�_�A�E����I���n�_]�ƂȂ�悤�Ƀf�[�^����

�� 100�~100�̉摜
( 0,99)     (99,99)
   ��---------->|
   ��---------->|
   ��---------->|
   ��---------->|
   ��---------->|
( 0, 0)     (99, 0) 

(0,0) -> (99,0) -> (0,1) -> (99,1) -> (0,2) -> (99,2) -> ... -> (0,99) -> (99,99)
�Ƃ����悤�ɐi��

�f�[�^�����c���T�C�Y���������ꍇ�͂��̃T�C�Y�ɏ]���ď������܂�܂��B
�f�[�^���̂ق����傫���ꍇ�͕ۏ؂ł��܂����B



+---------------------------------------------------------------
|�֐� BmpIn(char* FileName,int* WdAdr,int* HtAdr,RGB* rgb,int showinfo = 0)
+---------------------------------------------------------------
|FileName 	�ǂݍ��ރt�@�C����
|Wd			�擾�����摜�̉������i�[����ϐ��̃|�C���^
|Ht			�擾�����摜�̍������i�[����ϐ��̃|�C���^
|*rgb		�F�f�[�^
|showinfo	����\�����邩�ǂ���
|�A��l�F����:0 ���s:���̑�
+---------------------------------------------------------------



+---------------------------------------------------------------
�֐� int imgAddr(int x,int y,int Wd){
+---------------------------------------------------------------
|x	X���W
|y	Y���W
|Wd ����
|return�F�z���̎Q�Ɣԍ�(�A�h���X)
+---------------------------------------------------------------
�Ԃ����Ⴏ�����Ȃ��ˁH�H
*/



#endif