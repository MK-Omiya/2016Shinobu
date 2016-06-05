#include<stdio.h>
#include"BMP1.0.4.h"
#include<math.h>


int distance(RGB p1,RGB p2){
	return abs(sqrt(pow(p1.R,2)+pow(p1.G,2)+pow(p1.B,2))-sqrt(pow(p2.R,2)+pow(p2.G,2)+pow(p2.B,2)));	
}


const int oPixScale = 96;
char* iPath = "shino240.bmp";

int main(void){
	
	static RGB pos[600];
	FILE *fp = fopen("img\\img.csv","r");
	int i=0;

	while (fscanf(fp, "%d,%d,%d", &pos[i].R, &pos[i].G, &pos[i].B) != EOF){
        i++;
    }
	
	
	RGB *irgb,*orgb;
	int imgInputSizeX,imgInputSizeY;
	int W,H;
	
	getImgSize(iPath,&imgInputSizeX,&imgInputSizeY);
	
	int IW = imgInputSizeX;
	int IH = imgInputSizeY;
	
	int OW = imgInputSizeX*oPixScale;
	int OH = imgInputSizeY*oPixScale;
	
	irgb = (RGB*)malloc(IW*IH*3);
	orgb = (RGB*)malloc(OW*OH*3);
	
	bmpIn(iPath,&W,&H,irgb);
	
	if((W != IW)||(H != IH)){printf("サイズ指定エラー\n");return -1;}
	
	int dataCount = i;

	
	RGB st,minLocation = pos[0];
	int minDistance = distance(pos[0],st);
	int minNo = 0;
	
	char* ufn;
	
	for(int x=0;x<IW;x++){
		for(int y=0;y<IH;y++){
			RGB grgb = getColor(irgb,IW,x,y);
			st = grgb;
			
			for(i=0;i<dataCount;i++){
				if(distance(pos[i],st) < minDistance){
					minDistance = distance(pos[i],st);
					minLocation = pos[i];
					minNo = i;
				}
		
				//printf("[%0.5d]%d %d %d:%d\n",i,pos[i].R,pos[i].G,pos[i].B,distance(pos[i],st));
			}
			printf("一番近いとこ：%0.3d で距離は%0.3d 座標は[%0.3d %0.3d %0.3d]だよっ！::",minNo,minDistance,minLocation.R,minLocation.G,minLocation.B);
			sprintf(ufn,"img\\%0.3d_%0.3d_%0.3d.bmp",minLocation.R,minLocation.G,minLocation.B);
			
			
			
			for(int ct1=0 ; ct1< oPixScale ; ct1++){
				for(int ct2 = 0; ct2 < oPixScale ; ct2++){
					setColor(orgb, OW, oPixScale*x + ct1,oPixScale*y+ct2, grgb.R, grgb.G, grgb.B);
				}
			}
			setPicture(orgb,ufn,OW,oPixScale*x,oPixScale*y);
			printf ("%d %d\n",x,y);
			minLocation = pos[0];
			minDistance = distance(pos[0],st);
			minNo = 0;
			
		}
	}
	
	bmpOut("kiniromosaicart.bmp",OW,OH,orgb);
	
	return 0;
}
