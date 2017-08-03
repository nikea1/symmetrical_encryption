//
//  main.c
//  symmetric_encryption
//
//  Created by Adenike Adeyemi on 7/22/17.
//  Copyright © 2017 Adenike Adeyemi. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//helper function
void printBin(int num, int nbits){
    
    //makes sure number of bits to print is valid
    nbits = ((nbits > sizeof(int)*4 || nbits < 1) ? sizeof(int)*4 : nbits);
    
    //initialize buffer
    char buffer[nbits+1];
    buffer[nbits] = '\0';
    
    //convert bits to char representative and fills backwards
    for(int i = nbits-1; i >= 0; i--){
        buffer[i] = (num&1)+(0x30);
        num = num>>1;
    }
    
    //print binary string
    printf("%s\n", buffer);
    
}


int reverseByte(char c){

    char l,r;
    //masks
    int m1 = 0xFF7F; //1111 1111 0111 1111
    int m2 = 0x0001; //0000 0000 0000 0001
    int m3 = 0x0080; //0000 0000 1000 0000
    int m4 = 0xFFFE; //1111 1111 1111 1110
    
    
    for(int j = 0; j < 4; j++){
        
        //get values from left and right of bits
        l = (m3&c)>>(7-j);
        r = (m2&c)>>j;
        
        //check if different
        //if different flip bits
        if(l != r){
            if(l == 1){
                c =((c&m1)|m2);
            }
            else{
                c = ((c|m3)&m4);
            }
        }
        //shift mask
        m1 = (m1>>1);
        m2 = (m2<<1);
        m3 = (m3>>1);
        m4 = (m4<<1)+1;
        
    }

    return (0x00FF&c);
}

//hashing function
int hashKey(int c, int key){
    if(key > 0x0F || key < 0x08){
        printf("Abort: Key is out of bounds");
        exit(1);
    }
    c = c&0xFF;
    
    //get remainder
    int r = c%key;
    //get quotient
    int q = c/key;
    
    return ((r<<5)|q)&0x00FF;
    
}

//unhash function
int orderKey(char c, int key){
    if(key > 0x0F || key < 0x08){
        printf("Abort: Key is out of bounds");
        exit(1);
    }
    
    int q,r;
    
    q = (c&0x1F);
    r = (c&0xE0)>>5;
    
    return ((q*key)+r)&0x00FF;
}
//encoder
char *encode(char message[], int key){
    if(key > 0x0F || key < 0x08){
        printf("Abort: Key is out of bounds");
        exit(1);
    }
    int i = 0;
    while(message[i] != '\0'){
        message[i] = (char)hashKey(reverseByte(message[i]), key);
        i++;
    }
    return message;
}

//decoder
char *decode(char cipher[], int key){
    //checks if key is valid
    if(key > 0x0F || key < 0x08){
        printf("Abort: Key is out of bounds");
        exit(1);
    }
    
    //initialize loop
    int i = 0;
    //decode loop
    while(cipher[i] != '\0'){
        //unhash character
        cipher[i] = (char)reverseByte(orderKey(cipher[i], key));
        i++;
    
    }
    return cipher;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    printf("Imma linkin this a github\n");
    
    char str[] = "Hello, World!";
    
    printf("%s\n", str);
    encode(str, 8);
    printf("%s\n", str);
    decode(str, 8);
    printf("%s\n", str);

    
    
    return 0;
}
