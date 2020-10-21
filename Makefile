eeprommake: src/eeprom_main.c src/eeprom.c src/ll_func.c
	rm -f eeprommake
	cp backup_test.txt test.txt
	gcc -o eeprommake src/eeprom_main.c src/eeprom.c src/ll_func.c -I.
	
clean:
	rm -f eeprommake test.txt
	cp backup_test.txt test.txt
