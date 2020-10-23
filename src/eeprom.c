/*
    @file   eeprom.c
    
    @brief  This file contains basic eeprom functions, read/write/reset

    @author     Frank Lee
*/

#include "../include/eeprom.h"

// Global mem_mutex that is being used by both eeprom_read and eeprom_write
pthread_mutex_t mem_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
    This function reads from EEPROM memory and stores the read values
    into a character array given in the parameter. To fulfill all
    corner/edge cases, this function is divided into four big cases.
    The explanation of each case is given in README.md file.
    When the read begins, it locks the mem_mutex and only unlocks when
    the function returns.
    
    
    @param offset: Amount of offset from the beginning of EEPROM
    @param size: Size of desired read access
    @param *buf: Pointer to the buffer to store the read values
    
    @return: 0 for successful read
    @return: -1 for invalid offset
    @return: -2 for invalid size
    @return: -3 for index out of bound
*/
int eeprom_read(uint32_t offset, int size, char *buf) {

    // Checking input validity
    int param_check = eeprom_param_check(offset, size);
    if (param_check != 0) {
        return param_check;
    }
    
    // Incrementer
    int i;
    int j;
    
    // Number of page reads to do
    int num_page;
    // Temp page
    char temp[PAGE_SIZE];
    
    // Case #1: offset and size are both multiples of PAGE_SIZE
    if (offset % PAGE_SIZE == 0 && size % PAGE_SIZE == 0) {
        // Because size is a multiple of PAGE_SIZE, size/PAGE_SIZE is number of page reads
        num_page = size/PAGE_SIZE;
        // Lock memory mutex
        pthread_mutex_lock(&mem_mutex);
        for (i = 0; i < num_page; i++) {
            ll_read(offset + i*PAGE_SIZE, buf + i*PAGE_SIZE); // Copying the read bytes into buf
        }
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
    }
    
    // Case #2: offset is multiple of PAGE_SIZE and size is not multiple of PAGE_SIZE
    else if (offset % PAGE_SIZE == 0 && size % PAGE_SIZE != 0) {
        // The number of page reads is the truncated value of size, which is still
        // size/PAGE_SIZE
        num_page = size/PAGE_SIZE;
        // Lock memory mutex
        pthread_mutex_lock(&mem_mutex);
        for (i = 0; i < num_page; i++) {
            ll_read(offset + i*PAGE_SIZE, buf + i*PAGE_SIZE); // Copying the read bytes into buf
        }
        // Reading the remaining bytes
        ll_read((i+offset/PAGE_SIZE)*PAGE_SIZE, temp);    // Reading entire page
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
        memcpy(buf + i*PAGE_SIZE, temp, size%PAGE_SIZE);  // Storing only desired bytes from the page
        memset(temp, 0, PAGE_SIZE-1);    // Clear temp array
    }
    
    // Case #3: offset is not a multiple of PAGE_SIZE and offset+size is multiple of PAGE_SIZE
    else if (offset % PAGE_SIZE != 0 && (offset+size) % PAGE_SIZE == 0) {
        // Lock memory mutex
        pthread_mutex_lock(&mem_mutex);
        ll_read((offset/PAGE_SIZE)*PAGE_SIZE, temp);  // Reading the floor(offset)'th page
        memcpy(buf, temp+(offset%PAGE_SIZE), PAGE_SIZE-(offset%PAGE_SIZE));  // Storing only desired bytes
        memset(temp, 0, PAGE_SIZE-1);    // Clear temp array
        
        // Number of page reads is the floor(size)
        num_page = size/PAGE_SIZE;
        // This incrementer indicates the ceil(offset), where page reads should begin
        j = (offset/PAGE_SIZE)+1;  
        for (i = 0; i < num_page; i++) {
            ll_read(j*PAGE_SIZE, buf+(PAGE_SIZE-(offset%PAGE_SIZE))+(i*PAGE_SIZE));
            j++;
        }
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
    }
    
    // Case #4: offset is not multiple of PAGE_SIZE and offset+size is not multiple of PAGE_SIZE
    else if (offset % PAGE_SIZE != 0 && size % PAGE_SIZE != 0) {
        // Lock memory mutex
        pthread_mutex_lock(&mem_mutex);
        // Same behavior as above to read first couple bytes
        ll_read((offset/PAGE_SIZE)*PAGE_SIZE, temp);
        memcpy(buf, temp+(offset%PAGE_SIZE), PAGE_SIZE-(offset%PAGE_SIZE));
        memset(temp, 0, PAGE_SIZE-1);
        
        // Number of page reads is the number of aligned pages in size
        num_page = ((offset+size)/PAGE_SIZE) - ((offset/PAGE_SIZE)+1);
        // Same behavior as Case #3
        j = (offset/PAGE_SIZE)+1;
        for (i = 0; i < num_page; i++) {
            ll_read(j*PAGE_SIZE, buf+(PAGE_SIZE-(offset%PAGE_SIZE))+(i*PAGE_SIZE));
            j++;
        }
        
        // Do one more page read after where we left off
        ll_read(j*PAGE_SIZE, temp);
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
        // Storing only desired bytes
        memcpy(buf+(i*PAGE_SIZE)+(PAGE_SIZE-(offset%PAGE_SIZE)), temp, (offset+size)%PAGE_SIZE);
        memset(temp, 0, PAGE_SIZE-1);
    }
    
    // Ending the character array
    buf[size] = '\0';
    return 0;
}


/*
    This function writes to EEPROM memory with offset and size given by
    a character array in the parameter. To fulfill all
    corner/edge cases, this function is divided into four big cases.
    The explanation of each case is given in README.md file.
    When the write begins, it locks the mem_mutex and only unlocks when
    the function returns.
    
    
    @param offset: Amount of offset from the beginning of EEPROM
    @param size: Size of desired read access
    @param *buf: Pointer to the buffer to be written in EEPROM
    
    @return: 0 for successful write
    @return: -1 for invalid offset
    @return: -2 for invalid size
    @return: -3 for index out of bound
    @return: -4 for sizeof(buf) != size
*/
int eeprom_write(uint32_t offset, int size, char *buf) {

    

    // Checking for input validity
    int param_check = eeprom_param_check(offset, size);
    if (param_check != 0) {
        return param_check;
    }
    // Checking the input buf size
    if (strlen(buf) != size) {
        printf("ERROR: Size of buf is different than the amount of size to be written!\n");
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
        return -4;
    }
    
    // Incrementer
    int i;
    int j;
    
    // Number of page writes to do
    int num_page;
    // Temp page
    char temp[PAGE_SIZE];
    
    // Case #1: offset and size are both multiples of PAGE_SIZE
    if (offset % PAGE_SIZE == 0 && size % PAGE_SIZE == 0) {
        // Because size is a multiple of PAGE_SIZE, size/PAGE_SIZE is number of page writes
        num_page = size/PAGE_SIZE;
        // Lock memory mutex
        pthread_mutex_lock(&mem_mutex);
        for (i = 0; i < num_page; i++) {
            ll_write(offset + i*PAGE_SIZE, buf + i*PAGE_SIZE); // Write the bytes
        }
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
    }
    
    // Case #2: offset is multiple of PAGE_SIZE and size is not multiple of PAGE_SIZE
    else if (offset % PAGE_SIZE == 0 && size % PAGE_SIZE != 0) {
        // The number of page writes is the truncated value of size, which is still
        // size/PAGE_SIZE
        num_page = size/PAGE_SIZE;
        // Lock memory mutex
        pthread_mutex_lock(&mem_mutex);
        for (i = 0; i < num_page; i++) {
            ll_write(offset + i*PAGE_SIZE, buf + i*PAGE_SIZE); // Write the bytes
        }
        // Writing the remaining bytes
        ll_read((i+offset/PAGE_SIZE)*PAGE_SIZE, temp);      // Read entire page to temp
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
        memcpy(temp, buf + i*PAGE_SIZE, size%PAGE_SIZE);    // Overwriting portion of temp with remaining bytes
        ll_write((i+offset/PAGE_SIZE)*PAGE_SIZE, temp);     // Copy the updated page back
        memset(temp, 0, PAGE_SIZE-1);    // Clear temp array
    }
    
    // Case #3: offset is not a multiple of PAGE_SIZE and offset+size is multiple of PAGE_SIZE
    else if (offset % PAGE_SIZE != 0 && (offset+size) % PAGE_SIZE == 0) {
        // Lock memory mutex
        pthread_mutex_lock(&mem_mutex);
        ll_read((offset/PAGE_SIZE)*PAGE_SIZE, temp);    // Reading the floor(offset)'th page
        memcpy(temp+(offset%PAGE_SIZE), buf, PAGE_SIZE-(offset%PAGE_SIZE));     // Overwriting portion of temp
        ll_write((offset/PAGE_SIZE)*PAGE_SIZE, temp);   // Copy the updated page back
        memset(temp, 0, PAGE_SIZE-1);    // Clear temp array
        
        // Number of page writes is the floor(size)
        num_page = size/PAGE_SIZE;
        // This incrementer indicates the ceil(offset), where page writes should begin
        j = (offset/PAGE_SIZE)+1;  
        for (i = 0; i < num_page; i++) {
            ll_write(j*PAGE_SIZE, buf+(PAGE_SIZE-(offset%PAGE_SIZE))+(i*PAGE_SIZE));
            j++;
        }
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
    }
    
    // Case #4: offset is not multiple of PAGE_SIZE and offset+size is not multiple of PAGE_SIZE
    else if (offset % PAGE_SIZE != 0 && size % PAGE_SIZE != 0) {
        // Lock memory mutex
        pthread_mutex_lock(&mem_mutex);
        // Same behavior as above to write first couple bytes
        ll_read((offset/PAGE_SIZE)*PAGE_SIZE, temp);
        memcpy(temp+(offset%PAGE_SIZE), buf, PAGE_SIZE-(offset%PAGE_SIZE));
        ll_write((offset/PAGE_SIZE)*PAGE_SIZE, temp);
        memset(temp, 0, PAGE_SIZE-1);
        
        // Number of page writes is the number of aligned pages in size
        num_page = ((offset+size)/PAGE_SIZE) - ((offset/PAGE_SIZE)+1);
        // Same behavior as Case #3
        j = (offset/PAGE_SIZE)+1;
        for (i = 0; i < num_page; i++) {
            ll_write(j*PAGE_SIZE, buf+(PAGE_SIZE-(offset%PAGE_SIZE))+(i*PAGE_SIZE));
            j++;
        }
        
        // Do one more page write after where we left off
        ll_read(j*PAGE_SIZE, temp);
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
        // Storing only desired bytes
        memcpy(temp, buf+(i*PAGE_SIZE)+(PAGE_SIZE-(offset%PAGE_SIZE)), (offset+size)%PAGE_SIZE);
        ll_write(j*PAGE_SIZE, temp);        // Updating the new page
        memset(temp, 0, PAGE_SIZE-1);
    }
    
    return 0;
    
}
/*
    This function resets the eeprom by calling the lowlevl reset
    function. 
    
*/
void eeprom_reset() {
    ll_eeprom_reset();
}


int eeprom_param_check(uint32_t offset, int size) {
    // Checking for input validity
    if (offset > EEPROM_SIZE) {
        printf("ERROR: Invalid offset value!\n");
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
        return -1;
    }
    if (size <= 0) {
        printf("ERROR: Invalid size value!\n");
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
        return -2;
    }
    // Checking for index out of bound
    if (offset + size > EEPROM_SIZE) {
        printf("ERROR: Index out of bound!\n"); 
        // Unlock memory mutex
        pthread_mutex_unlock(&mem_mutex);
        return -3;
    }
    return 0;
}
