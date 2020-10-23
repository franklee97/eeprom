/*
    @file   eeprom_main.h
    
    @brief  This file contains header functions for eeprom_main.c

    @author     Frank Lee
*/

#include <stdint.h>
#include <stdio.h>
#include <pthread.h>

#include "../include/eeprom.h"
#include "../include/ll_func.h"

int main();

void eeprom_read_test();
void eeprom_write_test();
void *thread_func_0(void *vargp);
void *thread_func_1(void *vargp);
void *thread_func_2(void *vargp);
void *thread_func_3(void *vargp);
