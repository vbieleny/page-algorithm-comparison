; Function that will talk with the drive and load the disk sectors into memory
ata_lba_read:
	mov ebx, eax 						; backup the logical block address (starting sector) 

	; send the highest 8 bits of the lba to disk controller
	shr eax, 24						; shift the eax register 24 bits to the right so that eax contains highest 8 bits of the lba (shr does not wrap)
	or eax, 0xE0						; selects the master drive
	mov dx, 0x01F6						; 0x01F6 is the port that we're expected to write these 8 bits to.  
	out dx, al						; al contains the 8 high bits from earlier.  
								; out instruction copies the value from al to the I/O port specified by dx (copies the address and value to the I/O bus on the motherboard)

	; send the total number of sectors that we are reading to the hard disk controller
	mov eax, ecx
	mov dx, 0x01F2						; 0x01F2 --> set sectorcount
	out dx, al
	
	; send low bits of the lba
	mov eax, ebx						; restore the backup lba
	mov dx, 0x01F3						; 0x01F3 --> set LBAlo
	out dx, al

	; Send mid bits of the lba
	mov dx, 0x01F4						; 0x01F4 --> LBAmid
	mov eax, ebx 		
	shr eax, 8
	out dx, al

	; Send upper 16 bits of the lba
	mov dx, 0x1F5						; 0x01F5 --> LBAhi
	mov eax, ebx		
	shr eax, 16 
	out dx, al

	mov dx, 0x01F7						; 0x01F7 --> Command IO Port
	mov al, 0x20						; 0x0020 --> Read sector(s) command
	out dx, al

.next_sector:
	push ecx						; push ecx onto the stack to save for later (remember it has the total # of sectors we want to read)

; try_again label helps us wait until the disk drive is ready for next command
.try_again:
	mov dx, 0x01F7						
	in al, dx						; we read the ATA status register into al by reading from the command io port
	test al, 8						; check to see if the busy bit is set.  If set, the disk drive still has control of the command block registers (still reading)
	jz .try_again						; jumps back to try_again if the busy bit is set

	; we need to read 256 words (512 bytes, or one sector) at a time
	mov ecx, 256
	mov dx, 0x01F0						; 0x01F0 --> data port
	rep insw						; rep insw reads a word from port 0x01F0 and stores it into 0x0100000 (1 MB, specified by edi register)
								
	pop ecx							; restores the ecx that we saved via push earlier (contains count of sectors left to read)
	loop .next_sector					; decrements ecx register (contains # registers to read) until it hits 0 and we've read all sectors
	ret
