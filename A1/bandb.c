#include <stdio.h>
#include "bandb.h"

//Jakub Wozniak, ID: 1090034, A1, CIS 2520

void getbytes(unsigned char dest[], int bytes, void *src, int reverse)
{
    char *temp = src;
    int j = 0;

    if(reverse == 0) //if the caller doesnt want reverse order
    {
        for(int i = 0; i < bytes; i++)
        {
           dest[i] = temp[i]; // read in and store bytes in order they appear
        }
    }
    else //if caller wants reverse order
    {
        for(int i = bytes-1; i >= 0; i--)
        {
           dest[j] = temp[i]; //read in and store bytes starting from the last byte to the first
           j++;
        }
        j = 0;
    }
    
}

void getbits(char dest[], int bytes, void *src, int start, int end)
{
    unsigned char *temp = src;
    int q, r, index; // q: quotient, r: remainder, index: the index of what byte is being looked at
    int j = 0;

    for(int i = start; i > end; i--) //loops from the specified starting value to the ending value
    {
        q = i/8; //at the given bit location value finds the quotient
        r = i%8; //at the given bit location value finds the remainder
        index = bytes - q - 1; //the byte currently needed is as many bytes that go into the given bit location minus one as indexing begins at 0

        if((temp[index] >> r & 1) == 1) //r can be equal from 0 to 7 and represents where in the byte the desired bit is, only 1 & 1 = 1 thus if bit & 1 = 1 
        {
            dest[j] = '1'; //the bit is a 1
        }
        else
        {
            dest[j] = '0'; //else the bit is a zero
        }
        
        j++;
    }
    dest[j] = '\0'; //adds a null terminator to the end of the string
    j=0;

}

unsigned long long bits2ull(char *bits)
{
    unsigned long long num = 0;
    int i = 0;

    while(bits[i] != '\0') //runs until the end of the string
    {
        num = num*2; //multiplies the number by 2 each time in order to keep moving down the 2^x rows in binary
        if(bits[i] == '1') //if the bit is a one
        {
            num += 1; //adds one to the number
        }
        i++;
    }

    return num;
}

long long bits2ll(char *bits)
{
    long long num;

    if(bits[0] == '0') //if the sign bit is positive
    {
        num = 0; //the number is set to zero just like in bits2ull
    }
    else //if the sign bit is negative
    {
        num = -1; //the number gets set to -1 so the final answer will be negative
    }
    
    int i = 1; //i is equal to 1 as we ignore the sign bit

    while(bits[i] != '\0')
    {
        num = num*2; // exact same concept as bits2ull
        if(bits[i] == '1')
        {
            num += 1;
        }
        i++;
    }

    return num;
}

void spff(char *sign, char *exponent, char *significand, float *src)
{
    unsigned char bytes[4]; //used to store the float's bytes
    getbytes(bytes, 4, src, 1); //gets all of the float's bytes (in reverse order)
    getbits(sign, sizeof(float), bytes, 31, 30); //reads in and stores the 1 sign bit
    getbits(exponent, sizeof(float), bytes, 30, 22); //reads in and stores the 8 exponent bits
    getbits(significand, sizeof(float), bytes, 22, -1); //reads in and stores the 23 fraction bits
}

void dpff(char *sign, char *exponent, char *significand, double *src)
{
    //same concept as spff except modified for double
    unsigned char bytes[sizeof(double)]; //used to store the double's bytes
    getbytes(bytes, sizeof(double), src, 1); //gets all of the double's bytes (in reverse order)
    getbits(sign, sizeof(double), bytes, 63, 62); //reads in and stores the 1 sign bit
    getbits(exponent, sizeof(double), bytes, 62, 51); //reads in and stores the 11 exponent bits
    getbits(significand, sizeof(double), bytes, 51, -1); //reads in and stores the 52 fraction bits
}