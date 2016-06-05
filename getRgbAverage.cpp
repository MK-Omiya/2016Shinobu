#include<stdio.h>
#include"BMP1.0.4.h"
#include<math.h>

int main(int argc,char** args){
	RGB *irgb;
		
	int IW,IH;
	
	//args[1] = "img\\000.bmp";
	
	getImgSize(args[1],&IW,&IH);
	irgb = (RGB*)malloc(IW*IH*3);
	bmpIn(args[1],&IW,&IH,irgb);
	
	unsigned long r=0;
	unsigned long g=0;
	unsigned long b=0;
		
	for(int x=0;x<IW;x++){
		for(int y=0;y<IH;y++){
			RGB grgb = getColor(irgb,IW,x,y);
			r += grgb.R;
			g += grgb.G;
			b += grgb.B;
		}
	}
	
	printf(":“ü—Í‰æ‘œ:%s\n",args[1]);
	printf(":•½‹ÏRGB:%d %d %d\n",r/(IW*IH),g/(IW*IH),b/(IW*IH));
	printf("ren %s %0.3d_%0.3d_%0.3d.bmp\n",args[1],r/(IW*IH),g/(IW*IH),b/(IW*IH));
	printf(":------------------------------------------------------------\n");
	//bmpOut("FHD_OUT.bmp",IW,IH,irgb);
	
	return 0;
}

