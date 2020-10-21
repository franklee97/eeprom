/*
    @file   ll_func.h
    
    @brief  This file contains header functions for ll_func.c

    @author     Frank Lee
*/

#include <stdio.h>
#include <stdint.h>

int ll_read(uint32_t offset, char *buf);
int ll_write(uint32_t offset, char *buf);
void ll_eeprom_reset();
