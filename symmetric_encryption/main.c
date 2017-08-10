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
#include <string.h>

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

void printByte(char c){
    printBin(c, 8);
}

int reverseByte(char c){

    char l,r;
    //masks

    int m2 = 0x0001; //0000 0000 0000 0001
    int m3 = 0x0080; //0000 0000 1000 0000
    
    for(int j = 0; j < 4; j++){
        
        //get values from left and right of bits
        l = (m3&c)>>(7-j);
        r = (m2&c)>>j;
        
        //check for difference in left and right pointer bits
        if(l != r){ //swap when difference is found
            if(l == 1){
                c =((c&(~m3))|m2);
            }
            else{
                c = ((c|m3)&(~m2));
            }
        }
        //shift mask

        m2 = (m2<<1);
        m3 = (m3>>1);
        
    }//end of for loop

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
    
    if(argc == 4){                                // Make sure command is formatted correctly
        
        int key = atoi(argv[3]);                  //get 4-bit key
        
        if(key < 8 || key > 15){                  //make sure key is 4-bit else terminate
            printf("Error: Key is out of bounds\n");
            exit(1);
        }
        
        FILE *fpi, *fpo;                          //initalize file pointers for input and output
        
        int flag = 0;                             //set flag for encode and decode success
        
        fpi = fopen(argv[2], "r");                //open text to be encoded or decoded for reading file
        fpo = fopen("cipher.txt", "w+");          //open file to write output
        
        if(fpi == NULL || fpo == NULL){           //make sure files are opened else close files and exit
            printf("Error: file does not exist\n");
            fclose(fpi);
            fclose(fpo);
            exit(1);
        }
        
        int f_length = 2048;                      //buffer length
        fseek(fpi, 0, SEEK_SET);                  //make sure pointer is at the beginning
        
        char buffer[f_length];                    //create buffer
        
        if(strcmp(argv[1], "encode") == 0){       //if encoder mode
            
            while(feof(fpi) == 0){                //loop through file until end of file and encode text
                fgets(buffer, f_length, fpi);
                encode(buffer, 8);
                fputs(buffer, fpo);
            }
            
            flag = 1;                             //flag for completion
            
        }//end of if(encode)
        
        else if(strcmp(argv[1], "decode") == 0){  //if decoder mode
            while(feof(fpi) == 0){                //loop through file until end of filr and decode cipher

                fgets(buffer, f_length, fpi);
                decode(buffer, 8);
                fputs(buffer, fpo);
            }
            
            flag = 1;                             //flag for completion
        }//end of else if(decode)
        else{                                     //if something else other than decode or encode
            printf("Error: Invalid command. Has to be 'decode' or 'encode' without quotes\n");
            fclose(fpi);
            fclose(fpo);
            exit(1);
        }
        
        fclose(fpi);                              //close input and output files
        fclose(fpo);
        
        if(flag == 1){
            remove(argv[2]);                      //destroy input file
            rename("cipher.txt", argv[2]);        //rename output file to old file name
        }
        
    }//end of if(argc == 4)
    else{
        
        printf("Format: %s [mode] [text file] [key]\n", argv[0]); //print out correct format of command
    }//end of else
    

    return 0;
}
