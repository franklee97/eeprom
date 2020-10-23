/*
    @file   eeprom.h
    
    @brief  This file contains header function for eeprom.c
            It also contains #define for PAGE_SIZE and EEPROM_SIZE

    @author     Frank Lee
*/

#include <stdint.h>
#include <stdio.h>
#include "../include/ll_func.h"
#include <string.h>
#include <pthread.h>

#define PAGE_SIZE 32
#define EEPROM_SIZE 8192

extern pthread_mutex_t mem_mutex;

int eeprom_read(uint32_t offset, int size, char *buf);
int eeprom_write(uint32_t offset, int size, char *buf);
void eeprom_reset();
int eeprom_param_check(uint32_t offset, int size);

