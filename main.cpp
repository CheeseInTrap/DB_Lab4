#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <time.h>
#include "extmem.h"

#include <map>
using namespace std;

typedef struct Tuple{
    unsigned int attr1;
    unsigned int attr2;
}tuple;

int bufSize = 520;
int blkSize = 64;
int tupleSize = 8;

int RTupleNum = 7;
int RBlockNum = 16;

int STupleNum = 7;
int SBlockNum = 32;

int RAddr = 0;
int SAddr = 10000;

int sortRAddr = 2500;
int sortSAddr = 4000;

int projectAddr = 6000;

int joinAddr = 6100;

int rfinal = 50000;
int sfinal = 55000;



int random(int low , int high){
    return rand() % (high - low) + low + 1;
}

int getValueByNum(unsigned int *blk , Buffer buf,unsigned int * first,unsigned int * last,int num,int addr){
    int blk_num;
    int tuple_num;
    blk_num = num/7;
    tuple_num = num%7;
    blk = (unsigned int *)readBlockFromDisk(addr+blk_num,&buf);
    *(first) = *(blk+2*tuple_num);
    *(last) = *(blk+2*tuple_num+1);
    return 1;
}


int searchLinear(unsigned int *blk,Buffer buf){



    unsigned int a,b,c,d;
    int save_addr = 1000;
    unsigned int * save_blk;
    int i=0,j=0;
    save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    int blk_num = 0;
    int tuple_num = 0;
    printf("\n========================\n线性查找的结果\n");
    printf("R (A==40)\n");
    for(i = 0 ; i < RBlockNum ; i++){
        blk = (unsigned int *)readBlockFromDisk(sortRAddr+i,&buf);
        for(j = 0;j < RTupleNum ;j++){
            a = *(blk + 2*j);
            b = *(blk +2*j +1);

            if(a == 40){
                printf("(%4d,%4d)\t",a,b);
                *(save_blk + 2*tuple_num) = a;
                *(save_blk + 2*tuple_num+1) = b;
                tuple_num++;

            }
            if(tuple_num==7){
                *(save_blk + 2*tuple_num+1) = save_addr+blk_num;
                writeBlockToDisk((unsigned char *)save_blk,save_addr+blk_num,&buf);
                blk_num++;
                tuple_num=0;
            }
        }
        freeBlockInBuffer((unsigned char *)blk,&buf);
    }
    writeBlockToDisk((unsigned char *)save_blk,save_addr+blk_num,&buf);
    printf("\n");


    int jT=7;
    for(i=0;i<=blk_num;i++){
        blk = (unsigned int *)readBlockFromDisk(save_addr+i,&buf);
        if(i==blk_num) jT = tuple_num;
        for(j=0;j<jT;j++){
            a = *(blk + 2*j);
            b = *(blk +2*j +1);
            //printf("(%4d,%4d)\t",a,b);
        }
        printf("\n");
        freeBlockInBuffer((unsigned char *)blk,&buf);
    }

    save_addr = 1200;
    save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    blk_num = 0;
    tuple_num = 0;
    printf("S (C==60)\n");
    for(i = 0 ; i < SBlockNum ; i++){
        blk = (unsigned int *)readBlockFromDisk(sortSAddr+i,&buf);
        for(j = 0;j < STupleNum ;j++){
            c = *(blk + 2*j);
            d = *(blk +2*j +1);

            if(c == 60){
                printf("(%4d,%4d)\t",c,d);
                *(save_blk + 2*tuple_num) = c;
                *(save_blk + 2*tuple_num+1) = d;
                tuple_num++;

            }
            if(tuple_num==7){
                *(save_blk + 2*tuple_num+1) = save_addr+blk_num;
                writeBlockToDisk((unsigned char *)save_blk,save_addr+blk_num,&buf);
                blk_num++;
                tuple_num=0;
            }
        }
        freeBlockInBuffer((unsigned char *)blk,&buf);
    }
    writeBlockToDisk((unsigned char *)save_blk,save_addr+blk_num,&buf);
    printf("\n");


    jT = 7;
    for(i=0;i<=blk_num;i++){
        blk = (unsigned int *)readBlockFromDisk(save_addr+i,&buf);
        if(i == blk_num) jT = tuple_num;
        for(j=0;j<jT;j++){
            c = *(blk + 2*j);
            d = *(blk +2*j +1);
            //printf("(%4d,%4d)\t",c,d);
        }
        //printf("\n");
        freeBlockInBuffer((unsigned char *)blk,&buf);
    }

    return 1;
}

int searchBinary(unsigned int *blk,Buffer buf){
    int low = 0;
    int high = 112;
    int blk_num;
    int tuple_num;
    unsigned int a,b,c,d;


    printf("\n========================\n二元搜索的结果\n");
    printf("R\n");
    while(low <= high){
        int mid = (low + high)/2;
        blk_num = mid/7;
        tuple_num = mid%7;
        blk = (unsigned int *)readBlockFromDisk(sortRAddr+blk_num,&buf);
        a = *(blk+2*tuple_num);
        b = *(blk+2*tuple_num+1);
        freeBlockInBuffer((unsigned char *)blk,&buf);
        if(a == 40){
            printf("(%4d,%4d)\t",a,b);
            int dl = mid-1,ul = mid+1;

            while(dl>-1){

                blk_num = dl/7;
                tuple_num = dl%7;
                blk = (unsigned int *)readBlockFromDisk(sortRAddr+blk_num,&buf);
                a = *(blk+2*tuple_num);
                b = *(blk+2*tuple_num+1);
                freeBlockInBuffer((unsigned char *)blk,&buf);
                if(a == 40){
                    printf("(%4d,%4d)\t",a,b);
                }else{
                    break;
                }
                dl--;
            }

            while(ul<112){

                blk_num = ul/7;
                tuple_num = ul%7;
                blk = (unsigned int *)readBlockFromDisk(sortRAddr+blk_num,&buf);
                a = *(blk+2*tuple_num);
                b = *(blk+2*tuple_num+1);
                freeBlockInBuffer((unsigned char *)blk,&buf);
                if(a == 40){
                    printf("(%4d,%4d)\t",a,b);
                }else{
                    break;
                }
                ul++;
            }
            break;
        }
        else if (a<40)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
    }

    low = 0;
    high = 224;
    printf("\n二元搜索的结果\n");
    printf("S\n");
    while(low <= high){
        int mid = (low + high)/2;
        blk_num = mid/7;
        tuple_num = mid%7;
        blk = (unsigned int *)readBlockFromDisk(sortSAddr+blk_num,&buf);
        c = *(blk+2*tuple_num);
        d = *(blk+2*tuple_num+1);
        freeBlockInBuffer((unsigned char *)blk,&buf);
        if(c == 60){
            printf("(%4d,%4d)\t",c,d);
            int dl = mid-1,ul = mid+1;

            while(dl>-1){

                blk_num = dl/7;
                tuple_num = dl%7;
                blk = (unsigned int *)readBlockFromDisk(sortSAddr+blk_num,&buf);
                c = *(blk+2*tuple_num);
                d = *(blk+2*tuple_num+1);
                freeBlockInBuffer((unsigned char *)blk,&buf);
                if(c == 60){
                    printf("(%4d,%4d)\t",c,d);
                }else{
                    break;
                }
                dl--;
            }

            while(ul<224){

                blk_num = ul/7;
                tuple_num = ul%7;
                blk = (unsigned int *)readBlockFromDisk(sortSAddr+blk_num,&buf);
                c = *(blk+2*tuple_num);
                d = *(blk+2*tuple_num+1);
                freeBlockInBuffer((unsigned char *)blk,&buf);
                if(c == 60){
                    printf("(%4d,%4d)\t",c,d);
                }else{
                    break;
                }
                ul++;
            }
            break;
        }
        else if (c<60)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
    }

    return 1;
}

int searchInsertion(unsigned int *blk,Buffer buf){
    int low = 0;
    int high = 111;
    int blk_num;
    int tuple_num;
    int key = 40;
    unsigned int a=0,b=0,c=0,d=0;


    printf("\n插值搜索的结果\nR\n");
    tuple low_tuple;
    low_tuple.attr1 = 0 ;
    low_tuple.attr2 = 0 ;
    getValueByNum(blk,buf,&low_tuple.attr1,&low_tuple.attr2 ,low,sortRAddr);

    tuple high_tuple;
    high_tuple.attr1 = 0;
    high_tuple.attr2 = 0;
    getValueByNum(blk,buf,&high_tuple.attr1,&high_tuple.attr2 ,high,sortRAddr);

    //while(a[low]!=a[high]&&key>=a[low]&&key<=a[high])
    while(low_tuple.attr1 != high_tuple.attr1 && key >= low_tuple.attr1 && key<= high_tuple.attr1){
        int mid = low + (high - low)*(key - low_tuple.attr1)/(high_tuple.attr1 - low_tuple.attr1);
        getValueByNum(blk,buf,&a,&b,mid,sortRAddr);
        if(a ==40){
            printf("(%4d,%4d)\t",a,b);
            int dl = mid-1,ul = mid+1;

            while(dl>-1){

                blk_num = dl/7;
                tuple_num = dl%7;
                blk = (unsigned int *)readBlockFromDisk(sortRAddr+blk_num,&buf);
                a = *(blk+2*tuple_num);
                b = *(blk+2*tuple_num+1);
                freeBlockInBuffer((unsigned char *)blk,&buf);
                if(a == 40){
                    printf("(%4d,%4d)\t",a,b);
                }else{
                    break;
                }
                dl--;
            }

            while(ul<112){

                blk_num = ul/7;
                tuple_num = ul%7;
                blk = (unsigned int *)readBlockFromDisk(sortRAddr+blk_num,&buf);
                a = *(blk+2*tuple_num);
                b = *(blk+2*tuple_num+1);
                freeBlockInBuffer((unsigned char *)blk,&buf);
                if(a == 40){
                    printf("(%4d,%4d)\t",a,b);
                }else{
                    break;
                }
                ul++;
            }
            break;
        }
        else if(a<40){
            low = mid + 1;
            getValueByNum(blk,buf,&low_tuple.attr1,&low_tuple.attr2 ,low,sortRAddr);
        }
        else{
            high = mid - 1;
            getValueByNum(blk,buf,&high_tuple.attr1,&high_tuple.attr2 ,high,sortRAddr);
        }
    }


    printf("\n插值搜索的结果\nS\n");
    key = 60;
    low = 0;
    high = 223;
    getValueByNum(blk,buf,&low_tuple.attr1,&low_tuple.attr2 ,low,sortSAddr);
    getValueByNum(blk,buf,&high_tuple.attr1,&high_tuple.attr2 ,high,sortSAddr);

    //while(a[low]!=a[high]&&key>=a[low]&&key<=a[high])
    while(low_tuple.attr1 != high_tuple.attr1 && key >= low_tuple.attr1 && key<= high_tuple.attr1){
        int mid = low + (high - low)*(key - low_tuple.attr1)/(high_tuple.attr1 - low_tuple.attr1);
        getValueByNum(blk,buf,&c,&d,mid,sortSAddr);
        if(c == 60){
            printf("(%4d,%4d)\t",c,d);
            int dl = mid-1,ul = mid+1;

            while(dl>-1){

                blk_num = dl/7;
                tuple_num = dl%7;
                blk = (unsigned int *)readBlockFromDisk(sortSAddr+blk_num,&buf);
                c = *(blk+2*tuple_num);
                d = *(blk+2*tuple_num+1);
                freeBlockInBuffer((unsigned char *)blk,&buf);
                if(c == 60){
                    printf("(%4d,%4d)\t",c,d);
                }else{
                    break;
                }
                dl--;
            }

            while(ul<224){

                blk_num = ul/7;
                tuple_num = ul%7;
                blk = (unsigned int *)readBlockFromDisk(sortSAddr+blk_num,&buf);
                c = *(blk+2*tuple_num);
                d = *(blk+2*tuple_num+1);
                freeBlockInBuffer((unsigned char *)blk,&buf);
                if(c == 60){
                    printf("(%4d,%4d)\t",c,d);
                }else{
                    break;
                }
                ul++;
            }
            break;
        }
        else if(c<60){
            low = mid + 1;
            getValueByNum(blk,buf,&low_tuple.attr1,&low_tuple.attr2 ,low,sortSAddr);
        }
        else{
            high = mid - 1;
            getValueByNum(blk,buf,&high_tuple.attr1,&high_tuple.attr2 ,high,sortSAddr);
        }
    }
    return 1;

}

int project(unsigned int *blk , Buffer buf){

    unsigned int * save_blk;
    unsigned int a,b;
    int last_a = -1;
    int tuple_num=0;
    int i,j;
    printf("\n====================================\n投影结果\n");

    save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    for(i = 0 ; i < RBlockNum ; i++){
        blk = (unsigned int *)readBlockFromDisk(sortRAddr+i,&buf);
        for(j = 0;j < RTupleNum ;j++){
            a = *(blk + 2*j);
            b = *(blk + 2*j + 1);
            freeBlockInBuffer((unsigned char *)blk,&buf);
            if(a!=last_a){

                *(save_blk+tuple_num) = a;
                last_a = a;
                tuple_num++;
                printf("%4d\t",a);
                if(tuple_num == 15){
                    *(save_blk+tuple_num) = projectAddr+1;
                    writeBlockToDisk((unsigned char *)save_blk,projectAddr++,&buf);
                    save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                    tuple_num = 0;
                }

            }
            last_a = a;

        }
        if(tuple_num!=0) writeBlockToDisk((unsigned char *)save_blk,projectAddr++,&buf);

    }

    return 1;
}

int nestLoopJoin(Buffer buf){
    int i,j,m,n;
    unsigned int *R_blk;
    unsigned int *S_blk;
    unsigned int a,b,c,d;
    unsigned int *save_blk;
    int count = 0;
    int tuple_num=0;
    printf("\n=========================\n");
    printf("nest loop join 结果\n");
    save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    for(i = 0 ; i < RBlockNum ; i++){

        for(j = 0;j < RTupleNum ;j++){
            R_blk = (unsigned int *)readBlockFromDisk(sortRAddr+i,&buf);
            a = *(R_blk + 2*j);
            b = *(R_blk +2*j +1);
            freeBlockInBuffer((unsigned char *)R_blk,&buf);
            for(m = 0;m<SBlockNum;m++){
                S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+m,&buf);
                for(n = 0;n<STupleNum;n++){
                    c = *(S_blk + 2*n);
                    d = *(S_blk + 2*n + 1);
                    if( a == c){
                        printf("(%4d , %4d , %4d)\t",a,b,d);
                        *(save_blk+tuple_num) = a;
                        *(save_blk+tuple_num+1)= b;
                        *(save_blk+tuple_num+2)= c;
                        count++;
                        tuple_num = tuple_num +3;
                        if(tuple_num == 15){
                            *(save_blk + tuple_num) = joinAddr +1;
                            writeBlockToDisk((unsigned char *)save_blk , joinAddr++,&buf);
                            tuple_num = 0;
                            R_blk = (unsigned int *)readBlockFromDisk(sortRAddr+i,&buf);
                        }
                    }
                }
                freeBlockInBuffer((unsigned char *)S_blk,&buf);
            }


        }
        //printf("\n");
    }
    if(tuple_num!=0){
        writeBlockToDisk((unsigned char *)save_blk , joinAddr++,&buf);
    }
    printf("\n共%d个连接结果\n",count);
    printf("IOcost = %ld\n",buf.numIO);
    return 1;
}

int sortMergeJoin(Buffer buf){

    int i=0,j=0,count=0,flag=0,addr=1000,tuple_num = 0;
    unsigned int a,b,c,d;
    unsigned int last_a = -1,last_c;
    unsigned int *save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    unsigned int *R_blk;
    unsigned int *S_blk;

    printf("\n===============================\nsort merge join 结果\n");
    while( i<RTupleNum*RBlockNum && j<STupleNum*SBlockNum){
        R_blk = (unsigned int *)readBlockFromDisk(sortRAddr+i/7 , &buf);
        a = *(R_blk + 2*(i%7));
        b = *(R_blk + 2*(i%7) + 1);
        freeBlockInBuffer((unsigned char *)R_blk,&buf);
        S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+j/7 , &buf);
        c = *(S_blk + 2*(j%7));
        d = *(S_blk + 2*(j%7) + 1);
        freeBlockInBuffer((unsigned char *)S_blk,&buf);
        if(a == c){
            printf("(%4d,%4d,%4d)\t",a,b,d);
            *(save_blk + tuple_num) = a;
            *(save_blk + tuple_num + 1) = b;
            *(save_blk + tuple_num + 2) = d;
            tuple_num += 3;
            if(last_a < a) flag = i;
            count++;
            if(count%5 == 0 && count!=0) printf("\n");
            if(i==111){
                i = flag;
                j++;
                last_c = c;
            }
            else{
                i++;
                last_a = a;
            }
        } else if(a < c){
            i++;
            last_a = a;
        }else{
            j++;
            last_c = c;
            S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+j/7 , &buf);
            c = *(S_blk + 2*(j%7));
            d = *(S_blk + 2*(j%7));
            freeBlockInBuffer((unsigned char *)S_blk,&buf);
            if(c == last_c) i = flag;
        }
        if(tuple_num == 15){
            *(save_blk+tuple_num) = addr +1;
            writeBlockToDisk((unsigned char *)save_blk,addr++,&buf);
            tuple_num = 0;
        }
        if(tuple_num != 0 ){
            *(save_blk+tuple_num) = addr +1;
            writeBlockToDisk((unsigned char *)save_blk,addr++,&buf);
        }

    }
    printf("\n共%d个连接结果\n",count);
    printf("IOcost = %ld\n",buf.numIO);
    return 1;
}

int hashJoin(Buffer buf){
    map<int,vector<int> > HR;
    map<int,vector<int> > HS;
    int i=0,j=0,count=0,flag=0,addr = 1500,tuple_num = 0;
    unsigned int * save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    unsigned int *R_blk;
    unsigned int *S_blk;
    unsigned int a,b,c,d;
    printf("\n===============================\nhash join 结果\n");
    for(i=0 ; i<RTupleNum*RBlockNum ; i++){
        R_blk = (unsigned int *)readBlockFromDisk(sortRAddr+i/7,&buf);
        a = *(R_blk + 2*(i%7));
        b = *(R_blk + 2*(i%7)+1);
        freeBlockInBuffer((unsigned char *)R_blk,&buf);
        HR[a].push_back(b);
    }
    for(i=0 ; i<STupleNum*SBlockNum ; i++){
        S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+i/7,&buf);
        c = *(S_blk + 2*(i%7));
        d = *(S_blk + 2*(i%7)+1);
        freeBlockInBuffer((unsigned char *)S_blk,&buf);
        HS[c].push_back(d);
    }
    for(i = 20; i<= 40;i++){
        HR[i];
        HS[i];

    for(int m=0;m<HS[i].size();m++){
        for(int n=0;n<HR[i].size();n++){

            printf("(%4d,%4d,%4d)\t",i,HR[i][n],HS[i][m]);
            count++;
            *(save_blk + tuple_num) = i;
            *(save_blk + tuple_num + 1) = HR[i][n];
            *(save_blk + tuple_num + 2) = HS[i][m];
            tuple_num+=3;
            if(tuple_num == 15){
                *(save_blk+tuple_num)=addr+1;
                writeBlockToDisk((unsigned char *)save_blk,addr++,&buf);
                tuple_num = 0;
            }
        }
    }
    }
    if(tuple_num!=0){
        *(save_blk + tuple_num) = addr+1;
        writeBlockToDisk((unsigned char *)save_blk,addr++,&buf);
    }
    printf("\n共%d个连接结果\n",count);
    printf("IOcost = %ld\n",buf.numIO);
    return 1;
}

int unionSet(Buffer buf){

    unsigned int *save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    unsigned int *R_blk;
    unsigned int *S_blk;
    int i=0,j=0;
    int tuple_num = 0,addr = 1600;
    unsigned int a,b,c,d;
    int last = 0;
    int lastc = -1;
    int count=0;

    printf("\n===============================\n并集结果\n");
    for(i=0 ; i<RBlockNum*RTupleNum ; i++){
        for(j=0 ; j<SBlockNum*STupleNum ;j++){
            R_blk = (unsigned int *)readBlockFromDisk(sortRAddr+i/7,&buf);
            a = *(R_blk + 2*(i%7));
            b = *(R_blk + 2*(i%7)+1);
            freeBlockInBuffer((unsigned char *)R_blk,&buf);
            S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+j/7 , &buf);
            c = *(S_blk + 2*(j%7));
            d = *(S_blk + 2*(j%7) + 1);
            freeBlockInBuffer((unsigned char *)S_blk,&buf);
            if(a ==c && b==d){

                if(c!=lastc){
                    last = j-1;
                    lastc = c;
                }
                break;
            }
            if(j == SBlockNum*STupleNum-1){
                if(tuple_num == 0){
                    save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                }
                printf("(%4d,%4d)\t",a,b);
                tuple_num++;
                count++;
                if(count%6 ==0 && count!=0) printf("\n");
                if(tuple_num == 15){
                    *(save_blk+tuple_num) = addr + 1;
                    writeBlockToDisk((unsigned char *)save_blk,addr++,&buf);
                    tuple_num = 0;
                }
            }
        }
    }
    for(j=0;j<SBlockNum*STupleNum;j++){
        S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+j/7 , &buf);
        c = *(S_blk + 2*(j%7));
        d = *(S_blk + 2*(j%7) + 1);
        freeBlockInBuffer((unsigned char *)S_blk,&buf);
        if(tuple_num == 0){
            save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
        }
        printf("(%3d,%3d)\t",c,d);
        tuple_num++;
        count++;
        if(count%6 ==0 && count!=0) printf("\n");
        if(tuple_num == 15){
            *(save_blk+tuple_num) = addr+1;
            writeBlockToDisk((unsigned char *)save_blk,addr++,&buf);
            tuple_num = 0;
        }
    }
    if(tuple_num!=0){
        writeBlockToDisk((unsigned char *)save_blk,addr++,&buf);
    }
    printf("\n共%d个结果\n",count);
}

int intersection(Buffer buf){
    unsigned int * save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    unsigned int *R_blk;
    unsigned int *S_blk;
    unsigned int a,b,c,d;
    int i,j,addr = 1700;
    int tuple_num = 0;
    int count =0;
    int last = 0;
    int lasts=-1;
    printf("\n================================\n交集结果\n");
    for(i = 0;i<RBlockNum*RTupleNum ; i++){
        R_blk = (unsigned int *)readBlockFromDisk(sortRAddr+i/7,&buf);
        a = *(R_blk + 2*(i%7));
        b = *(R_blk + 2*(i%7)+1);
        freeBlockInBuffer((unsigned char *)R_blk,&buf);
        for(j=last ; j<SBlockNum*STupleNum ; j++){
            S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+j/7,&buf);
            c = *(S_blk + 2*(j%7));
            d = *(S_blk + 2*(j%7)+1);
            freeBlockInBuffer((unsigned char *)S_blk,&buf);

            if(a == c && b == d){
                if(c!=lasts)
                {
                    lasts = c;
                    last = j-1;
                }
                *(save_blk + tuple_num) = a;
                *(save_blk + tuple_num + 1) = b;
                printf("(%4d,%4d)\t",a,b);
                count++;
                tuple_num += 2;
                if(tuple_num == 14){
                    *(save_blk + tuple_num) = addr +1;
                    writeBlockToDisk((unsigned char *)save_blk , addr++,&buf);
                    tuple_num = 0;
                }
                break;
            }
        }
    }
    if(tuple_num !=0){
        writeBlockToDisk((unsigned char *)save_blk , addr++,&buf);
    }
    printf("\n共%d个结果\n",count);
    return 1;
}

int RdiffS(Buffer buf){
    unsigned int *R_blk;
    unsigned int *S_blk;
    int addr = 1800,tuple_num =0,count = 0,i=0,j=0;
    unsigned int * save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    unsigned int a,b,c,d;
    int last=0;
    int last_num = -1;

    printf("\n==============================R-S\n");
    for(i=0; i<RBlockNum*RTupleNum;i++){
        for(j=0 ;j<SBlockNum*STupleNum;j++){
            R_blk = (unsigned int *)readBlockFromDisk(sortRAddr+i/7,&buf);
            a = *(R_blk + 2*(i%7));
            b = *(R_blk + 2*(i%7)+1);
            freeBlockInBuffer((unsigned char *)R_blk,&buf);
            S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+j/7 , &buf);
            c = *(S_blk + 2*(j%7));
            d = *(S_blk + 2*(j%7) + 1);
            freeBlockInBuffer((unsigned char *)S_blk,&buf);
            if(a == c && b == d ){
                if(c!=last_num){
                    last = j-1;
                    last_num = c;
                }
                break;
            }
            if(j == SBlockNum*STupleNum-1){
                if(tuple_num == 0){
                    save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                }
                printf("(%4d,%4d)\t",a,b);
                tuple_num++;
                count++;
                if(count%6==0 && count!=0) printf("\n");
                if(tuple_num == 15){
                    *(save_blk + tuple_num) = addr +1;
                    writeBlockToDisk((unsigned char *)save_blk , addr++,&buf);
                    tuple_num = 0;
                }
            }
        }
    }
    if(tuple_num!=15){
        writeBlockToDisk((unsigned char *)save_blk , addr++,&buf);
    }
    printf("\n共%d个结果\n",count);
    return 1;

}

int SdiffR(Buffer buf){
    unsigned int *R_blk;
    unsigned int *S_blk;
    int addr = 1900,tuple_num =0,count = 0,i=0,j=0;
    unsigned int * save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    unsigned int a,b,c,d;
    int last=0;
    int last_num=-1;

    printf("\n==============================S-R\n");
    for(i=0; i<SBlockNum*STupleNum;i++){
        for(j=0 ;j<RBlockNum*RTupleNum;j++){
            S_blk = (unsigned int *)readBlockFromDisk(sortSAddr+i/7,&buf);
            c = *(S_blk + 2*(i%7));
            d = *(S_blk + 2*(i%7)+1);
            freeBlockInBuffer((unsigned char *)S_blk,&buf);
            R_blk = (unsigned int *)readBlockFromDisk(sortRAddr+j/7 , &buf);
            a = *(R_blk + 2*(j%7));
            b = *(R_blk + 2*(j%7) + 1);
            freeBlockInBuffer((unsigned char *)R_blk,&buf);
            if(a == c && b == d ){
                if(a!=last_num){
                    last_num=a;
                    last = j-1;
                }
                break;
            }
            if(j == RBlockNum*RTupleNum-1){
                if(tuple_num == 0){
                    save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                }
                printf("(%4d,%4d)\t",c,d);
                tuple_num++;
                count++;
                if(count%6==0 && count!=0) printf("\n");
                if(tuple_num == 15){
                    *(save_blk + tuple_num) = addr +1;
                    writeBlockToDisk((unsigned char *)save_blk , addr++,&buf);
                    tuple_num = 0;
                }
            }
        }
    }
    if(tuple_num!=15){
        writeBlockToDisk((unsigned char *)save_blk , addr++,&buf);
    }
    printf("\n共%d个结果\n",count);
    return 1;

}

int cmp(const void *a,const void *B){
    return (*(tuple *)a).attr1-(*(tuple *)B).attr1;
}

int mergeSort(Buffer buf){
    return 0;
}
int sortTuple(unsigned int *blk,Buffer buf,tuple R[112],tuple S[224]){
    qsort(R,112,sizeof(tuple),cmp);
    qsort(S,224,sizeof(tuple),cmp);

    int i = 0 , j = 0;
    for(i = 0 ; i < RBlockNum ; i++){
        blk = (unsigned int *)getNewBlockInBuffer(&buf);
        for(j = 0 ; j < RTupleNum ; j++){
            *(blk + j*2) = R[i*7+j].attr1;
            *(blk + j*2 + 1) = R[i*7+j].attr2;
            printf("(%4d,%4d)\t",*(blk + 2*j),*(blk +2*j +1));
        }
        printf("\n");
        *(blk + 2*j + 1) = i+sortRAddr+1;
        if(i == 15) *(blk + 2*j +1) = 0;
        if(writeBlockToDisk((unsigned char *)blk , i+sortRAddr , &buf) !=0 ){
            perror("Writing Block Failed!\n");
            return 0;
        }else{
            freeBlockInBuffer((unsigned char *)blk,&buf);
        }
    }
    printf("sortR complete\n");

    for(i = 0 ; i < SBlockNum ; i++){
        blk = (unsigned int *)getNewBlockInBuffer(&buf);
        for(j = 0 ; j < STupleNum ; j++){
            *(blk + 2*j) = S[i*7+j].attr1;
            *(blk + 2*j + 1) = S[i*7+j].attr2;
            printf("(%4d,%4d)\t",*(blk + 2*j),*(blk + 2*j + 1));
        }
        printf("\n");
        *(blk + 2*j + 1) = i + sortSAddr + 1;
        if(i == 31) *(blk + 2*j + 1) = 0;
        if(writeBlockToDisk((unsigned char *)blk , i + sortSAddr , &buf) != 0){
            perror("Writing Block Failed!\n");
            return 0;
        }else{
            freeBlockInBuffer((unsigned char *)blk,&buf);
        }
    }

    printf("sortS complete\n");
}


int mergeTwo(int addr1,int addr2,int output_addr,Buffer buf,int num){
    int r1=0,r2=0,tuple_num=0;
    unsigned int * R1;
    unsigned int * R2;
    unsigned int * save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    unsigned int a,b,c,d;
    while(true)
    {
        if(tuple_num==0)
        R1 = (unsigned int *)readBlockFromDisk(addr1+r1/7,&buf);

        a = *(R1+2*(r1%7));
        b = *(R1+2*(r1%7)+1);
        freeBlockInBuffer((unsigned char *)R1,&buf);
        R2 = (unsigned int *)readBlockFromDisk(addr2+r2/7,&buf);
        c = *(R2+2*(r2%7));
        d = *(R2+2*(r2%7)+1);
        freeBlockInBuffer((unsigned char *)R2,&buf);
        if(a<=c )
        {
            *(save_blk+tuple_num) = a;
            *(save_blk+tuple_num+1) = b;
            r1++;
        }
        else
        {
            *(save_blk+tuple_num) = c;
            *(save_blk+tuple_num+1) = d;
            r2++;
        }

        tuple_num+=2;
        if(tuple_num == 14)
        {
            *(save_blk + tuple_num+2) = output_addr +1;
            writeBlockToDisk((unsigned char *)save_blk, output_addr++,&buf);
            tuple_num = 0;
            save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
        }

        if(r1 == num || r2 == num)
        {
            if(r1==num)
            {
                while(r2<num)
                {
                    R2 = (unsigned int *)readBlockFromDisk(addr2+r2/7,&buf);
                    c = *(R2+2*(r2%7));
                    d = *(R2+2*(r2%7)+1);
                    freeBlockInBuffer((unsigned char *)R2,&buf);
                    *(save_blk+tuple_num) = c;
                    *(save_blk+tuple_num+1) = d;
                    r2++;
                    tuple_num +=2;
                    if(tuple_num == 14)
                    {
                        *(save_blk + tuple_num+2) = output_addr +1;
                        writeBlockToDisk((unsigned char *)save_blk, output_addr++,&buf);
                        tuple_num = 0;
                        //save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                    }
                }
            }
            else if(r2==num)
            {
                while(r1<num)
                {
                    R1 = (unsigned int *)readBlockFromDisk(addr1+r1/7,&buf);

                    a = *(R1+2*(r1%7));
                    b = *(R1+2*(r1%7)+1);
                    freeBlockInBuffer((unsigned char *)R1,&buf);
                    *(save_blk+tuple_num) = a;
                    *(save_blk+tuple_num+1) = b;
                    r1++;
                    tuple_num +=2;
                    if(tuple_num == 14)
                    {
                        *(save_blk + tuple_num+2) = output_addr +1;
                        writeBlockToDisk((unsigned char *)save_blk, output_addr++,&buf);
                        tuple_num = 0;
                        //save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                    }
                }
            }
            r1=0;
            r2=0;
            return 0;
        }

    }


    return 0;
}

int mergeSortR(Buffer buf){
    int sort_R_addr = 10100;
    int sort_S_addr = 20300;

    unsigned int * blk;
    unsigned int * save_blk;
    int pos = 0,i=0,j=0,k=0;
    unsigned int a,b,c,d;
    int loop = 4;
    tuple sort_object[28];
    for(k=0;k<loop;k++){

        pos = 0;
        for(i = 4*k ; i < 4*k+4 ; i++){
            blk = (unsigned int *)readBlockFromDisk(RAddr+i,&buf);
            for(j = 0;j < RTupleNum ;j++){
                a = *(blk + 2*j);
                b = *(blk +2*j +1);

                sort_object[pos].attr1 = a;
                sort_object[pos].attr2 = b;
                pos++;
            }
            //printf("\n");
            freeBlockInBuffer((unsigned char *)blk,&buf);

        }
        qsort(sort_object,28,sizeof(tuple),cmp);
        for(i = 0; i < 4 ; i++){
            save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
            for(j=0;j<7;j++){
                *(blk + j*2) = sort_object[i*7+j].attr1;
                *(blk + j*2 + 1) = sort_object[i*7+j].attr2;
                //printf("(%4d,%4d)\t",sort_object[i*7+j].attr1,sort_object[i*7+j].attr2);
            }
            //printf("\n");
            if(writeBlockToDisk((unsigned char *)save_blk , sort_R_addr++ , &buf) != 0){
                perror("Writing Block Failed!\n");
                return 0;
            }
        }
    }

    loop = 8;

    for(k=0;k<loop;k++){

        pos = 0;
        for(i = 4*k ; i < 4*k+4 ; i++){
            blk = (unsigned int *)readBlockFromDisk(SAddr+i,&buf);
            for(j = 0;j < 7 ;j++){
                c = *(blk + 2*j);
                d = *(blk +2*j +1);

                sort_object[pos].attr1 = c;
                sort_object[pos].attr2 = d;
                pos++;
            }
            //printf("\n");
            freeBlockInBuffer((unsigned char *)blk,&buf);

        }
        qsort(sort_object,28,sizeof(tuple),cmp);
        for(i = 0; i < 4 ; i++){
            save_blk = (unsigned int *)getNewBlockInBuffer(&buf);
            for(j=0;j<7;j++){
                *(save_blk + j*2) = sort_object[i*7+j].attr1;
                *(save_blk + j*2 + 1) = sort_object[i*7+j].attr2;
                //printf("(%4d,%4d)\t",sort_object[i*7+j].attr1,sort_object[i*7+j].attr2);
            }
            //printf("\n");
            if(writeBlockToDisk((unsigned char *)save_blk , sort_S_addr++ , &buf) != 0){
                perror("Writing Block Failed!\n");
                return 0;
            }
        }
    }

    int addr1 = 10400;
    int addr2 = 10500;
    int R1_1 = 10100;
    int R2_1 = 10104;
    int R1_2 = 10108;
    int R2_2 = 10112;
    printf("=================");
    mergeTwo(R1_1,R2_1,addr1,buf,28);
    mergeTwo(R1_2,R2_2,addr2,buf,28);
    mergeTwo(addr1,addr2,sortRAddr,buf,56);

    printf("===============");
    int s11 = 20300;
    int s21 = 20304;
    int s1 = 21000;
    int s12 = 20308;
    int s22 = 20312;
    int s2 = 22000;
    int s13 = 20316;
    int s23 = 20320;
    int s3 = 23000;
    int s14 = 20324;
    int s24 = 20328;
    int s4 = 24000;

    mergeTwo(s11,s21,s1,buf,28);
    mergeTwo(s12,s22,s2,buf,28);

    mergeTwo(s13,s23,s3,buf,28);
    mergeTwo(s14,s24,s4,buf,28);

    mergeTwo(s1,s2,25000,buf,56);
    mergeTwo(s3,s4,26000,buf,56);
    mergeTwo(25000,26000,sortSAddr,buf,112);
    return 1;
}

//生成数据
int generateData(unsigned int *blk , Buffer buf){

    unsigned int a, b, c, d;
    int i = 0 , j = 0;

    tuple R[112];
    tuple S[224];

    for(i = 0 ; i < RBlockNum ; i++){
        blk = (unsigned int *)getNewBlockInBuffer(&buf);
        for(j = 0 ; j < RTupleNum ; j++){
            a = random(1,40);
            b = random(1,1000);
            *(blk + j*2) = a;
            *(blk + j*2 + 1) = b;
            printf("(%4d,%4d)\t",*(blk + 2*j),*(blk +2*j +1));
            R[i*7+j].attr1 = a;
            R[i*7+j].attr2 = b;
        }
        printf("\n");
        *(blk + 2*j + 1) = i+RAddr+1;
        if(i == 15) *(blk + 2*j +1) = 0;
        if(writeBlockToDisk((unsigned char *)blk , i+RAddr , &buf) !=0 ){
            perror("Writing Block Failed!\n");
            return 0;
        }
    }
    printf("R complete\n");

    for(i = 0 ; i < SBlockNum ; i++){
        blk = (unsigned int *)getNewBlockInBuffer(&buf);
        for(j = 0 ; j < STupleNum ; j++){
            c = random(20,60);
            d = random(1,1000);
            *(blk + 2*j) = c;
            *(blk + 2*j + 1) = d;
            printf("(%4d,%4d)\t",c,d);
            S[i*7+j].attr1 = c;
            S[i*7+j].attr2 = d;
        }
        printf("\n");
        *(blk + 2*j + 1) = i + SAddr + 1;
        if(i == 31) *(blk + 2*j + 1) = 0;
        if(writeBlockToDisk((unsigned char *)blk , i + SAddr , &buf) != 0){
            perror("Writing Block Failed!\n");
            return 0;
        }
    }

    printf("S complete\n");

    sortTuple(blk,buf,R,S);
    return 1;
}

int readData(unsigned int *blk,Buffer buf,int RAddr,int SAddr){

    unsigned int a,b,c,d;

    int i=0,j=0;
    printf("\nR:\n");
    for(i = 0 ; i < RBlockNum ; i++){
        blk = (unsigned int *)readBlockFromDisk(RAddr+i,&buf);
        for(j = 0;j < RTupleNum ;j++){
            a = *(blk + 2*j);
            b = *(blk +2*j +1);
            printf("(%4d,%4d)\t",a,b);

        }
        printf("\n");
        freeBlockInBuffer((unsigned char *)blk,&buf);
    }
    printf("==========================\n");
    printf("\nS:\n");
    for(i = 0 ; i < SBlockNum ; i++){
        blk = (unsigned int *)readBlockFromDisk(SAddr+i,&buf);
        for(j = 0;j < STupleNum ;j++){
            c = *(blk + 2*j);
            d = *(blk +2*j +1);
            printf("(%4d,%4d)\t",c,d);

        }
        freeBlockInBuffer((unsigned char *)blk,&buf);
        printf("\n");
    }
    printf("==========================\n\n");
    return 1;
}


int hashSearch(unsigned int *blk,Buffer buf,int number){
    int base_addr = 50000;
    int addr = 50200;
    int i,j;
    unsigned int a,b;
    int last_a = 2;
    int blk_num = 0;
    int tuple_num = 0;
    unsigned int *hash_blk;
    hash_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    for(i=0;i<RBlockNum;i++){
        blk = (unsigned int *)readBlockFromDisk(sortRAddr+i,&buf);
        for(j=0;j<7;j++){
            a = *(blk + 2*j);
            b = *(blk + 2*j +1);
            if(a!=last_a){
                *(hash_blk+14) = tuple_num;
                *(hash_blk+15) = addr+1;
                for(int k=tuple_num;k<7;k++){
                    *(hash_blk+2*k) = 0;
                    *(hash_blk+2*k+1) = 0;
                }
                writeBlockToDisk((unsigned char *)hash_blk,addr++,&buf);
                addr = base_addr + a*100;
                hash_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                blk_num = 0;
                tuple_num=0;
                last_a = a;
            }
            *(hash_blk+2*tuple_num) = a;
            *(hash_blk+2*tuple_num+1) = b;
            tuple_num++;
            if(tuple_num ==7 ){
                *(hash_blk+14) = 7;
                *(hash_blk+15) = addr+1;
                writeBlockToDisk((unsigned char *)hash_blk,addr++,&buf);
                hash_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                tuple_num =0;
                blk_num++;
            }

        }
        freeBlockInBuffer((unsigned char *)blk,&buf);


    }
    if(tuple_num!=0){
            *(hash_blk+14) = tuple_num;
            *(hash_blk+15) = addr+1;
            writeBlockToDisk((unsigned char *)hash_blk,addr++,&buf);
            addr++;
        }
    blk = (unsigned int *)readBlockFromDisk(base_addr+number*100,&buf);
    int num = *(blk+14);
    printf("num = %d",num);
    for(int k=0;k<num;k++){
        a = *(blk+2*k);
        b = *(blk+2*k+1);
        printf("\n%d,%d",a,b);
    }


    return 1;
}

int hashSearch2(unsigned int *blk,Buffer buf,int number){
    int base_addr = 500000;
    int addr = 502000;
    int i,j;
    unsigned int a,b;
    int last_a = 20;
    int blk_num = 0;
    int tuple_num = 0;
    unsigned int *hash_blk;
    hash_blk = (unsigned int *)getNewBlockInBuffer(&buf);
    for(i=0;i<SBlockNum;i++){
        blk = (unsigned int *)readBlockFromDisk(sortSAddr+i,&buf);
        for(j=0;j<7;j++){
            a = *(blk + 2*j);
            b = *(blk + 2*j +1);
            if(a!=last_a){
                *(hash_blk+14) = tuple_num;
                *(hash_blk+15) = addr+1;
                for(int k=tuple_num;k<7;k++){
                    *(hash_blk+2*k) = 0;
                    *(hash_blk+2*k+1) = 0;
                }
                writeBlockToDisk((unsigned char *)hash_blk,addr++,&buf);
                addr = base_addr + a*100;
                hash_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                blk_num = 0;
                tuple_num=0;
                last_a = a;
            }
            *(hash_blk+2*tuple_num) = a;
            *(hash_blk+2*tuple_num+1) = b;
            tuple_num++;
            if(tuple_num ==7 ){
                *(hash_blk+14) = 7;
                *(hash_blk+15) = addr+1;
                writeBlockToDisk((unsigned char *)hash_blk,addr++,&buf);
                hash_blk = (unsigned int *)getNewBlockInBuffer(&buf);
                tuple_num =0;
                blk_num++;
            }

        }
        freeBlockInBuffer((unsigned char *)blk,&buf);


    }
    if(tuple_num!=0){
            *(hash_blk+14) = tuple_num;
            *(hash_blk+15) = addr+1;
            writeBlockToDisk((unsigned char *)hash_blk,addr++,&buf);
            addr++;
        }
    blk = (unsigned int *)readBlockFromDisk(base_addr+number*100,&buf);
    int num = *(blk+14);
    printf("num = %d",num);
    for(int k=0;k<num;k++){
        a = *(blk+2*k);
        b = *(blk+2*k+1);
        printf("\n%d,%d",a,b);
    }


    return 1;
}


int main(int argc, char **argv)
{
    srand((unsigned)time(NULL));
    Buffer buf; /* A buffer */
    unsigned int *blk; /* A pointer to a block */


    // Initialize the buffer
    if (!initBuffer(bufSize, blkSize, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    printf("Buffer Initialization Success!\n");

    //int write_result = generateData(blk,buf);
//    mergeSort(buf);
//    readData(blk,buf,RAddr,SAddr);
//    printf("\n=============\n归并排序\n");
    readData(blk,buf,sortRAddr,sortSAddr);

//    system("pause");
//    searchLinear(blk,buf);
//    system("pause");
//    searchBinary(blk,buf);
//    system("pause");
//    searchInsertion(blk,buf);
//    system("pause");

    printf("\n============================\n");
//    project(blk,buf);
//    system("pause");
//    nestLoopJoin(buf);
//    system("pause");
//    sortMergeJoin(buf);
//    system("pause");
//    hashJoin(buf);
//    system("pause");

//    printf("\n============================\n");
//    unionSet(buf);
//    system("pause");
//    intersection(buf);
//    system("pause");
//    RdiffS(buf);
//    system("pause");
//    SdiffR(buf);


    hashSearch(blk,buf,9);
    printf("\n============================\n");
    hashSearch2(blk,buf,20);
    return 0;
}



