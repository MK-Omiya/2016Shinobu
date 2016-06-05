//詳細は一番下に書いてあります。
/*
###注意###
使用する際は[CPP]形式で保存してください。
更新:2016/01/11::[きんいろモザイク]Rhodanthe*はいいぞ！[130K]
*/
#ifndef MK1024_LIB_2015_10_11_03_06
#define MK1024_LIB_2015_10_11_03_06
//MK_AC[2015-10-11[C-GCUS][T-KNMS]]
//MK_AC2::2016/01/11::[きんいろモザイク]Rhodanthe*はいいぞ！[130K]
#include<stdio.h>
#include<stdlib.h>



//ピクセルデータ
//「色」を扱う変数
typedef struct{
    unsigned char R;
	unsigned char G;
	unsigned char B;
}RGB;



int bmpOut(char* FileName,int Wd,int Ht,RGB *rgb,int showinfo = 0){
	unsigned char color[4];
	//FileHeader
	const char bfType[2] 				= {'B','M'};//BitMapだよー
	unsigned long  bfSize;					//ファイルサイズ Wd*Ht*4+54
	const unsigned short bfReserved1	= 0x00;//予約領域 常に0
	const unsigned short bfReserved2	= 0x00;//予約領域 常に0
	const unsigned long  bfOffBits		= 54;//ファイルオフセット
	
	//BitmapInformation
	const unsigned long  biSize			=40;//ビットマップ情報サイズ
	long biWidth;							//横幅
	long  biHeight;							//高さ
	const unsigned short biPlanes		=1; //プレーンサイズ　常に1
	const unsigned short biBitCount		=24;//1画素当たりのデータサイズ(32BitTrueColorに設定)*************************************[変更中][default:32]
	const unsigned long  biCompression	=0;	//圧縮(無圧縮なので0を指定)
	const unsigned long  biSizeImage	=0; //画像dpi多分使わない
	const long biXPixPerMeter			=0;	//1m当たりのdpi(x)使わない
	const long biYPixPerMeter			=0;	//1m当たりのdpi(y)使わない
	const unsigned long  biClrUsed		=0;	//仕様パレット数 使わない
	const unsigned long  biClrImporant	=0;	//重要パレットインデックス 使わない
	
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
	
	//*****ファイルヘッダを書き込む*****
	fwrite(bfType,sizeof(unsigned char),2,f);			//BM(BitMap)
	fwrite(&bfSize,sizeof(unsigned long),1,f);			//ファイルサイズ Wd*Ht*4+54
	fwrite(&bfReserved1,sizeof(unsigned short),1,f);	//予約領域 常に0
	fwrite(&bfReserved2,sizeof(unsigned short),1,f);	//予約領域 常に0
	fwrite(&bfOffBits,sizeof(unsigned long),1,f);		//ファイルオフセット
	
	//*****ビットマップ情報を書き込む*****
	fwrite(&biSize,sizeof(unsigned long),1,f);			//ビットマップ情報サイズ
	fwrite(&biWidth,sizeof(long),1,f);					//横幅
	fwrite(&biHeight,sizeof(long),1,f);					//高さ
	fwrite(&biPlanes,sizeof(unsigned short),1,f);		//プレーンサイズ(常に1)
	fwrite(&biBitCount,sizeof(unsigned short),1,f);		//1画素当たりのデータサイズ(32BitTrueColorに設定)
	fwrite(&biCompression,sizeof(unsigned long),1,f);	//圧縮(無圧縮なので0を指定)
	fwrite(&biSizeImage,sizeof(unsigned long),1,f);		//画像dpi多分使わない
	fwrite(&biXPixPerMeter,sizeof(long),1,f);			//1m当たりのdpi(x)使わない
	fwrite(&biYPixPerMeter,sizeof(long),1,f);			//1m当たりのdpi(y)使わない
	fwrite(&biClrUsed,sizeof(unsigned long),1,f);		//仕様パレット数 使わない
	fwrite(&biClrImporant,sizeof(unsigned long),1,f);	//重要パレットインデックス 使わない
	
	//内容を書き込む
	
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


//画像を読み込む(24Bit bitmap 非圧縮)
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
		printf("ファイル読み込みエラー(%s)\n",FileName);
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
	
	//BmpOut("きんいろデバッグ.bmp",640,480,rgbd);
	
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

//画像ファイルの縦横サイズを取得
int getImgSize(char* path,int* width,int* height){
	static int FileSize;
	static int BitCount;
	
	FILE *fmain;
	fmain = fopen(path,"rb");
	if(fmain == NULL){
		printf("ファイル読み込みエラー(%s)\n",path);
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





//x,y 座標から横サイズをもとにアドレスに変換する 
int imgAddr(int x,int y,int Wd){
	return Wd * y + x;
}

//指定したx,y座標の色を設定する。r,g,bを数値で指定
void setColor(RGB* pix,int width,int x,int y,int r,int g,int b){
	pix[imgAddr(x,y,width)].R = r;
	pix[imgAddr(x,y,width)].G = g;
	pix[imgAddr(x,y,width)].B = b;
}

//指定したx,y座標の色を設定する。RGB型で指定
void setColor2(RGB* pix,int width,int x,int y,RGB rgb){
	pix[imgAddr(x,y,width)].R = rgb.R;
	pix[imgAddr(x,y,width)].G = rgb.G;
	pix[imgAddr(x,y,width)].B = rgb.B;
}

//RGB型の色を反転させる、ネガが作れる。
RGB invColor(RGB rgb){
	RGB rRGB;
	rRGB.R = (255 - rgb.R);
	rRGB.G = (255 - rgb.G);
	rRGB.B = (255 - rgb.B);
	return rRGB;	
}

//数値をRGB型に変換する。
RGB numToRgb(int r,int g,int b){
	RGB rgb;
	rgb.R = r;
	rgb.G = g;
	rgb.B = b;	
	return rgb;	
}

//指定したx,y座標の色を取得する
RGB getColor(RGB* pix,int width,int x,int y){
	RGB rrgb;
	rrgb.R = pix[imgAddr(x,y,width)].R;
	rrgb.G = pix[imgAddr(x,y,width)].G;
	rrgb.B = pix[imgAddr(x,y,width)].B;
	return rrgb;
}

//画像を上に張り付ける
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
[詳細]
+---------------------------------------------------------------
|関数 int BmpOut(char* FileName,int Wd,int Ht,RGB *rgb,int showinfo = 0)
+---------------------------------------------------------------
|FileName 	保存するファイル名
|Wd			保存する画像の横幅
|Ht			保存する画像の高さ
|*rgb		色データ
|showinfo	情報を表示するかどうか
|帰り値：成功:0 失敗:その他
+---------------------------------------------------------------
色データの詳細
typedef struct tagRGBQUAD{
    unsigned char R;
	unsigned char G;
	unsigned char B;
}RGB;

このような構造体になっている。
使用する際は自動的に書き込む使用のため R,G,Bの順番は気にしなくてもよい

使用方法
rgbについては書き込みたい画像ファイルの「縦×横」分の配列を取り
[左下を開始地点、右上を終了地点]となるようにデータを代入

例 100×100の画像
( 0,99)     (99,99)
   ↑---------->|
   ↑---------->|
   ↑---------->|
   ↑---------->|
   ↑---------->|
( 0, 0)     (99, 0) 

(0,0) -> (99,0) -> (0,1) -> (99,1) -> (0,2) -> (99,2) -> ... -> (0,99) -> (99,99)
というように進む

データ数より縦横サイズが小さい場合はそのサイズに従って書き込まれます。
データ数のほうが大きい場合は保証できませんよ。



+---------------------------------------------------------------
|関数 BmpIn(char* FileName,int* WdAdr,int* HtAdr,RGB* rgb,int showinfo = 0)
+---------------------------------------------------------------
|FileName 	読み込むファイル名
|Wd			取得した画像の横幅を格納する変数のポインタ
|Ht			取得した画像の高さを格納する変数のポインタ
|*rgb		色データ
|showinfo	情報を表示するかどうか
|帰り値：成功:0 失敗:その他
+---------------------------------------------------------------



+---------------------------------------------------------------
関数 int imgAddr(int x,int y,int Wd){
+---------------------------------------------------------------
|x	X座標
|y	Y座標
|Wd 横幅
|return：配列上の参照番号(アドレス)
+---------------------------------------------------------------
ぶっちゃけ説明なくね？？
*/



#endif