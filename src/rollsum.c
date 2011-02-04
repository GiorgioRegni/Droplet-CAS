#include "rollsum.h"

#define DO1(buf,i)  {s1 += buf[i]; s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);
#define OF16(off)  {s1 += 16*off; s2 += 136*off;}

void rollsumUpdate(trollsum *sum,const unsigned char *buf,unsigned int len) {
    unsigned long s1 = sum->s1;
    unsigned long s2 = sum->s2;

    sum->count+=len;                   /* increment sum count */
    while (len >= 16) {
        DO16(buf);
        OF16(ROLLSUM_CHAR_OFFSET);
        buf += 16;
        len -= 16;
    }
    while (len != 0) {
        s1 += (*buf++ + ROLLSUM_CHAR_OFFSET);
        s2 += s1;
        len--;
    }
    sum->s1=s1;
    sum->s2=s2;
}
