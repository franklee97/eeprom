/*
    @file   eeprom_main.h
    
    @brief  This file contains header functions for eeprom_main.c

    @author     Frank Lee
*/

#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "../include/eeprom.h"
#include "../include/ll_func.h"

int main();

void eeprom_read_test();
void eeprom_write_test();
void* thread_func(void *ptr);
