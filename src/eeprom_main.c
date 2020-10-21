/*
    @file   eeprom_main.c
    
    @brief  This file contains tests for eeprom

    @author     Frank Lee
*/

#include "../include/eeprom_main.h"

int main()
{
    
    eeprom_read_test();
    eeprom_reset();
    eeprom_write_test();

    return 0;
}


/*
    This test tests the eeprom_read function. For all four cases, it does 
    3 different reads. The last test is checking invalid inputs.
*/
void eeprom_read_test() {
    char out[256];
    
    // eeprom_read Case #1
    printf("----eeprom_read Case #1----\n");
    eeprom_read(0, 32, out);
    printf("Offset:0, Size:32 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(0, 64, out);
    printf("Offset:0, Size:64 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(2048, 32, out);
    printf("Offset:2048, Size:32 --->%s\n\n", out);
    memset(out, 0, 255);
    
    // eeprom_read Case #2
    printf("----eeprom_read Case #2----\n");
    eeprom_read(0, 16, out);
    printf("Offset:0, Size:16 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(32, 58, out);
    printf("Offset:32, Size:58 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(128, 200, out);
    printf("Offset:128, Size:200 --->%s\n\n", out);
    memset(out, 0, 255);
    

    // eeprom_read Case #3
    printf("----eeprom_read Case #3----\n");
    eeprom_read(10, 22, out);
    printf("Offset:10, Size:22 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(38, 90, out);
    printf("Offset:38, Size:90 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(2000, 48, out);
    printf("Offset:2000, Size:48 --->%s\n\n", out);
    memset(out, 0, 255);
    
    // eeprom_read Case #4
    printf("----eeprom_read Case #4----\n");
    eeprom_read(10, 24, out);
    printf("Offset:10, Size:24 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(38, 97, out);
    printf("Offset:38, Size:97 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(2002, 76, out);
    printf("Offset:2002, Size:76 --->%s\n\n", out);
    memset(out, 0, 255);
    
    // eeprom_read invalid inputs
    printf("---eeprom_read invalid inputs----\n");
    eeprom_read(9000, 22, out);
    printf("Offset:-5, Size:22 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(38, 0, out);
    printf("Offset:38, Size:0 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(2000, -8, out);
    printf("Offset:2000, Size:-8 --->%s\n\n", out);
    memset(out, 0, 255);
    
    eeprom_read(8190, 10, out);
    printf("Offset:8190, Size:10 --->%s\n\n", out);
    memset(out, 0, 255);
    
}

/*
    This test tests the eeprom_write function. For each case, 
    it does one writes. Then it checks for invalid inputs.
*/
void eeprom_write_test() {

    char out[256];
    
    char str[32] = "I love rock n roll, so put anoth";

    // eeprom_write Case #1
    printf("----eeprom_write Case #1----\n");
    memcpy(out, str, 32);
    eeprom_write(0, 32, out);
    printf("Offset:0, Size:32 --->%s\n\n", out);
    
    // eeprom_write Case #2
    printf("----eeprom_write Case #2----\n");
    memcpy(out, str, 16);
    eeprom_write(128, 16, out);
    printf("Offset:128, Size:16 --->%s\n\n", out);
    memset(out, 0, 255);
    
    // eeprom_write Case #3
    printf("----eeprom_write Case #3----\n");
    memcpy(out, str, 18);
    eeprom_write(248, 18, out);
    printf("Offset:248, Size:18 --->%s\n\n", out);
    memset(out, 0, 255);
    
    // eeprom_write Case #4
    printf("----eeprom_write Case #4----\n");
    memcpy(out, str, 22);
    eeprom_write(500, 22, out);
    printf("Offset:500, Size:22 --->%s\n\n", out);
    memset(out, 0, 255);
    
    // eeprom_write invalid inputs
    printf("---eeprom_write invalid inputs----\n");
    memcpy(out, str, 32);
    eeprom_write(-5, 22, out);
    printf("Offset:-5, Size:22 --->%s\n\n", out);
    
    eeprom_write(38, 0, out);
    printf("Offset:38, Size:0 --->%s\n\n", out);
    
    eeprom_write(2000, -8, out);
    printf("Offset:2000, Size:-8 --->%s\n\n", out);
    
    eeprom_write(8190, 10, out);
    printf("Offset:8190, Size:10 --->%s\n\n", out);
    
    eeprom_write(0, 10, out);
    printf("Offset:8190, Size:10 --->%s\n\n", out);
    memset(out, 0, 255);


}
