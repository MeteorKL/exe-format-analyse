#include <stdio.h>
int main(void)
{
    FILE *src;
    //    FILE **des;
    unsigned char ch1,ch2,ch3,ch4;
    int i,j,file_length,header_length,relocation_total,relocation_table,*relocation_DS,*relocation_SI;
    int delta_CS,IP,delta_SS,SP;
    //    unsigned char playload[]={0x0E, 0x58, 0x2D, 0x01, 0x00, 0x8E, 0xD8, 0x8E, 0xC0, 0xBA, 0x48, 0x00, 0xB4, 0x09, 0xCD, 0x21, 0xBE, 0x00, 0x00, 0xBF, 0x00, 0x00, 0xB9, 0x48, 0x00, 0xAC, 0x34, 0x5A, 0xAA, 0x49, 0x83, 0xF9, 0x00, 0x75, 0xF6, 0x0E, 0x07, 0xBF, 0x1D, 0x00, 0x8C, 0xD8, 0xAB, 0xBF, 0x29, 0x00, 0x8C, 0xC8, 0xAB, 0xB4, 0x01, 0xCD, 0x21, 0x0E, 0x68, 0x1C, 0x00, 0xCF };
    src=fopen("/Users/meteor/downloads/HELLO.EXE", "rb");
    //    des=fopen("/Users/meteor/desktop/hello_temp.exe", "wb");
    i=0;
    ch1=fgetc(src);
    ch2=fgetc(src);
    printf("+%2x  %2x %2x       ", i, ch1, ch2);
    //    putc(ch1,des);
    //    putc(ch2,des);
    i+=2;
    if (ch1==0x4d&&ch2==0x5a) {
        printf("这是一个dos文件\n");
        ch1=fgetc(src);
        ch2=fgetc(src);
        ch3=fgetc(src);
        ch4=fgetc(src);
        file_length=ch1+(ch2<<8)+0x200*((ch3+(ch4<<8)-1));
        printf("+%2x  %2x %2x %2x %2x 载入内存的文件大小:%x字节\n", i, ch1, ch2, ch3, ch4, file_length);
        //        putc(0xAB,des);
        //        putc(ch2,des);
        //        putc(ch3,des);
        //        putc(ch4,des);
        i+=4;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        relocation_total=ch1+(ch2<<8);
        printf("+%2x  %2x %2x       重定位项数:%x\n", i, ch1, ch2, relocation_total);
        //        putc(ch1,des);
        //        putc(ch2,des);
        i+=2;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        header_length=(ch1+(ch2<<8))*0x10;
        printf("+%2x  %2x %2x       文件头长度:%x字节\n", i, ch1, ch2, header_length);
        //        putc(ch1,des);
        //        putc(ch2,des);
        i+=2;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        printf("+%2x  %2x %2x       为该exe分配的最少内存:%x节\n", i, ch1, ch2, ch1+(ch2<<8));
        //        putc(ch1,des);
        //        putc(ch2,des);
        i+=2;
        ch1=fgetc(src);
        ch2=fgetc(src);
        printf("+%2x  %2x %2x       为该exe分配的最多内存:%x节\n", i, ch1, ch2, ch1+(ch2<<8));
        //        putc(ch1,des);
        //        putc(ch2,des);
        i+=2;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        delta_SS=ch1+(ch2<<8);
        printf("+%2x  %2x %2x       载入文件后的初始SS与起始段地址的差值delta_SS:%x\n", i, ch1, ch2, ch1+(ch2<<8));
        //        putc(ch1,des);
        //        putc(ch2,des);
        i+=2;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        SP=ch1+(ch2<<8);
        printf("+%2x  %2x %2x       载入文件后的初始SP:%x\n", i, ch1, ch2, ch1+(ch2<<8));
        //        putc(ch1,des);
        //        putc(ch2,des);
        i+=2;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        printf("+%2x  %2x %2x       CRC文件校验:%x\n", i, ch1, ch2, ch1+(ch2<<8));
        //        putc(ch1,des);
        //        putc(ch2,des);
        i+=2;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        IP=ch1+(ch2<<8);
        printf("+%2x  %2x %2x       载入文件后的初始IP:%x\n", i, ch1, ch2, IP);
        //        putc(0x60,des);
        //        putc(ch2,des);
        i+=2;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        delta_CS=ch1+(ch2<<8);
        printf("+%2x  %2x %2x       载入文件后的初始CS与起始段地址的差值delta_CS:%x\n", i, ch1, ch2, delta_CS);
        //        putc(ch1,des);
        //        putc(ch2,des);
        i+=2;
        
        ch1=fgetc(src);
        ch2=fgetc(src);
        relocation_table=ch1+(ch2<<8);
        printf("+%2x  %2x %2x       重定位项起始位置:%x\n", i, ch1, ch2, relocation_table);
        //        putc(0x00,des);
        //        putc(0x00,des);
        i+=2;
        
        while(i<relocation_table){
            ch1=fgetc(src);
            //            putc(0x00,des);
            i++;
        }
        relocation_DS=malloc(relocation_total*sizeof(int));
        relocation_SI=malloc(relocation_total*sizeof(int));
        for (j=0; j<relocation_total; j++) {
            ch1=fgetc(src);
            ch2=fgetc(src);
            ch3=fgetc(src);
            ch4=fgetc(src);
            relocation_DS[j]=ch3+(ch4<<8);
            relocation_SI[j]=ch1+(ch2<<8);
            printf("+%2x  %2x %2x %2x %2x 第%d个重定向位置 %x:%x\n", i, ch1, ch2, ch3, ch4, j+1, relocation_DS[j], relocation_SI[j]);
            //            putc(ch1,des);
            //            putc(ch2,des);
            //            putc(ch3,des);
            //            putc(ch4,des);
            i+=4;
        }
        
        while(i<header_length){
            ch1=fgetc(src);
            //            putc(ch1,des);
            i++;
        }
        printf("+%2x             汇编代码开始～\n", i);
        while(i<file_length){
            ch1=fgetc(src);
            printf("%2x %c", ch1, ch1);
            //            ch1=ch1^0x5a;
            //            putc(ch1,des);
            //            printf("%2x %c\n", ch1, ch1);
            i+=1;
        }
        //        j=i;
        //        while(j-i<sizeof(playload)/sizeof(unsigned char)){
        //            ch1=playload[j-i];
        //            //printf("%2x \n", ch1);
        //            putc(ch1,des);
        //            j++;
        //        }
        
        
        
    }
    fclose(src);
    //    fclose(des);
}