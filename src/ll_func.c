/*
    @file   ll_func.c
    
    @brief  This file contains low level functions to be called by eeprom.c

    @author     Frank Lee
*/


#include "../include/ll_func.h"
#include <errno.h>

/*
    This function is supposed to mimic the behavior of a low level
    read function using File IO. The function reads 32 bytes from the
    file test.txt with the beginning index equaling to offset.
    The parameter offset must be a multiple of 32.
    
    @param offset: Amount of offset from the beginning of the file
    @param *buf: The buffer to store the read bytes
    
    @return: 0 for success. -1 for failure to open the file
*/
int ll_read(uint32_t offset, char *buf) {
    FILE *fp;
    errno = 0;
    fp = fopen("/home/vm349/Desktop/eeprom/test.txt", "r");
    if (fp == NULL) {
        printf("Null fp %d\n", errno);
        return -1;
    }
    
    // This function moves the current pointer to the offset
    fseek(fp, offset, SEEK_SET);
    
    // Using fgetc to store value and not fgets as fgets appends a NULL
    // character at the end, which would always limit the size of buf 
    // to 32
    int i;
    for (i = 0; i < 32; i++) {
        buf[i] = fgetc(fp);
    }

    fclose(fp);
    return 0;
}

/*
    This function is supposed to mimic the behavior of a low level
    write function using File IO. The function writes 32 bytes into
    file test.txt with the beginning index equaling to offset. 
    The parameter offset must be a multiple of 32.
    
    @param offset: Amount of offset from the beginning of the file
    @param *buf: The buffer to write bytes into the file
    
    @return: 0 for success. -1 for failure to open the file
*/
int ll_write(uint32_t offset, char *buf) {
    FILE *fp;
    errno = 0;
    fp = fopen("/home/vm349/Desktop/eeprom/test.txt", "r+");
    if (fp == NULL) {
        printf("Null fp %d\n", errno);
        return -1;
    }
    
    // This function moves the current pointer to the offset
    fseek(fp, offset, SEEK_SET);
    
    int i;
    for (i = 0; i < 32; i++) {
        fputc(buf[i], fp);
    }

    fclose(fp);
    return 0;
}

/*
    This function is supposed to mimic the behavior of a low level
    eeprom reset function. EEPROM resets by providing the memory
    with a slightly higher voltage than VCC, which wipes the entire
    memory. I did not write the code for this as it would be outside
    the scope of this prompt.
*/
void ll_eeprom_reset() {
    // This is where the code to raise the voltage woule be
    // raise_voltage_all();
    return;
}

