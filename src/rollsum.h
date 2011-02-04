#ifndef _ROLLSUM_H_
#define _ROLLSUM_H_

#define ROLLSUM_CHECK_MASK (1<<13) -1
#define ROLLSUM_ARBITRARY_VALUE 0x1ba8
#define ROLLSUM_CHAR_OFFSET 31

/* Adler32 type checksum */
typedef struct rollsum {
    unsigned long count;               /* count of bytes included in sum */
    unsigned long s1;                  /* s1 part of sum */
    unsigned long s2;                  /* s2 part of sum */
} trollsum;

void rollsumUpdate(trollsum *sum,const unsigned char *buf,unsigned int len);

/* macro implementations of simple routines */
#define rollsumInit(sum) { \
    (sum)->count=(sum)->s1=(sum)->s2=0; \
}

#define rollsumMinima(sum) (((sum)->s1 & ROLLSUM_CHECK_MASK) == ROLLSUM_ARBITRARY_VALUE)

#define rollsumRotate(sum,out,in) { \
    (sum)->s1 += (unsigned char)(in) - (unsigned char)(out); \
    (sum)->s2 += (sum)->s1 - (sum)->count*((unsigned char)(out)+ROLLSUM_CHAR_OFFSET); \
}

#define rollsumRollin(sum,c) { \
    (sum)->s1 += ((unsigned char)(c)+ROLLSUM_CHAR_OFFSET); \
    (sum)->s2 += (sum)->s1; \
    (sum)->count++; \
}

#define rollsumRollout(sum,c) { \
    (sum)->s1 -= ((unsigned char)(c)+ROLLSUM_CHAR_OFFSET); \
    (sum)->s2 -= (sum)->count*((unsigned char)(c)+ROLLSUM_CHAR_OFFSET); \
    (sum)->count--; \
}

#define rollsumDigest(sum) (((sum)->s2 << 16) | ((sum)->s1 & 0xffff))

#endif /* _ROLLSUM_H_ */
