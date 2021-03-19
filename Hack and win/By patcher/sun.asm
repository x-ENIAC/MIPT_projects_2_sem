.model tiny
                                                                                
.code
org 100h
start:		
		
		mov ax, 0b800h
		mov es, ax

		mov ah, 0h
		mov al, 0h

		mov di, (17 * 80 + 0) * 2
		mov si, (25 * 80 + 25) * 2

	fill_black:

		mov es:[di], ax
		inc di
		cmp di, si
		je end_fill_black
		jmp fill_black

	end_fill_black:

		mov ah, 06h
		mov al, 0fh

		mov bh, 7h
		mov bl, 0fh

	draw_circle:		

		mov es:[(17 * 80 + 33) * 2], ax
		mov es:[(17 * 80 + 34) * 2], ax
		mov es:[(17 * 80 + 35) * 2], ax
		mov es:[(17 * 80 + 36) * 2], ax
		mov es:[(17 * 80 + 37) * 2], ax
		mov es:[(17 * 80 + 38) * 2], ax
		mov es:[(17 * 80 + 39) * 2], ax

		mov es:[(18 * 80 + 32) * 2], ax
		mov es:[(18 * 80 + 33) * 2], ax
		mov es:[(18 * 80 + 34) * 2], ax
		mov es:[(18 * 80 + 35) * 2], bx
		mov es:[(18 * 80 + 36) * 2], ax
		mov es:[(18 * 80 + 37) * 2], bx
		mov es:[(18 * 80 + 38) * 2], ax
		mov es:[(18 * 80 + 39) * 2], ax
		mov es:[(18 * 80 + 40) * 2], ax

		mov es:[(19 * 80 + 32) * 2], ax
		mov es:[(19 * 80 + 33) * 2], bx
		mov es:[(19 * 80 + 34) * 2], ax
		mov es:[(19 * 80 + 35) * 2], ax
		mov es:[(19 * 80 + 36) * 2], bx
		mov es:[(19 * 80 + 37) * 2], ax
		mov es:[(19 * 80 + 38) * 2], ax
		mov es:[(19 * 80 + 39) * 2], bx
		mov es:[(19 * 80 + 40) * 2], ax		

		mov es:[(20 * 80 + 32) * 2], ax
		mov es:[(20 * 80 + 33) * 2], ax
		mov es:[(20 * 80 + 34) * 2], bx
		mov es:[(20 * 80 + 35) * 2], bx
		mov es:[(20 * 80 + 36) * 2], bx
		mov es:[(20 * 80 + 37) * 2], bx		
		mov es:[(20 * 80 + 38) * 2], bx
		mov es:[(20 * 80 + 39) * 2], ax
		mov es:[(20 * 80 + 40) * 2], ax

		mov es:[(21 * 80 + 33) * 2], ax
		mov es:[(21 * 80 + 34) * 2], ax
		mov es:[(21 * 80 + 35) * 2], ax
		mov es:[(21 * 80 + 36) * 2], ax
		mov es:[(21 * 80 + 37) * 2], ax
		mov es:[(21 * 80 + 38) * 2], ax
		mov es:[(21 * 80 + 39) * 2], ax

draw_rays:

		mov es:[(16 * 80 + 36) * 2], ax
		mov es:[(15 * 80 + 36) * 2], ax
		mov es:[(14 * 80 + 36) * 2], ax		

		mov es:[(22 * 80 + 36) * 2], ax
		mov es:[(23 * 80 + 36) * 2], ax
		mov es:[(24 * 80 + 36) * 2], ax		

		mov es:[(17 * 80 + 42) * 2], ax
		mov es:[(16 * 80 + 44) * 2], ax

		mov es:[(21 * 80 + 42) * 2], ax
		mov es:[(22 * 80 + 44) * 2], ax

		mov es:[(19 * 80 + 43) * 2], ax
		mov es:[(19 * 80 + 45) * 2], ax			
		mov es:[(19 * 80 + 47) * 2], ax		

		mov es:[(17 * 80 + 30) * 2], ax
		mov es:[(16 * 80 + 28) * 2], ax

		mov es:[(21 * 80 + 30) * 2], ax
		mov es:[(22 * 80 + 28) * 2], ax

		mov es:[(19 * 80 + 29) * 2], ax
		mov es:[(19 * 80 + 27) * 2], ax
		mov es:[(19 * 80 + 25) * 2], ax		

		mov al, 'M'
		mov es:[(19 * 80 + 5) * 2], ax

		mov al, 'r'
		mov es:[(19 * 80 + 6) * 2], ax
		mov es:[(19 * 80 + 7) * 2], ax
		mov es:[(19 * 80 + 8) * 2], ax

		mov al, '^'
		mov es:[(19 * 80 + 10) * 2], ax
		mov es:[(19 * 80 + 14) * 2], ax

		mov al, '.'
		mov es:[(19 * 80 + 11) * 2], ax
		mov es:[(19 * 80 + 13) * 2], ax	

		mov al, '_'
		mov es:[(19 * 80 + 12) * 2], ax	

		mov al, 'M'
		mov es:[(19 * 80 + 55) * 2], ax

		mov al, 'e'
		mov es:[(19 * 80 + 56) * 2], ax

		mov al, 'o'
		mov es:[(19 * 80 + 57) * 2], ax

		mov al, 'w'
		mov es:[(19 * 80 + 58) * 2], ax

		mov al, '('
		mov es:[(19 * 80 + 60) * 2], ax

		mov al, '='
		mov es:[(19 * 80 + 61) * 2], ax		
		mov es:[(19 * 80 + 67) * 2], ax		

		mov al, '^'
		mov es:[(19 * 80 + 62) * 2], ax		
		mov es:[(19 * 80 + 66) * 2], ax			

		mov al, '.'
		mov es:[(19 * 80 + 63) * 2], ax		
		mov es:[(19 * 80 + 65) * 2], ax	

		mov al, 'w'
		mov es:[(19 * 80 + 64) * 2], ax

		mov al, ')'
		mov es:[(19 * 80 + 68) * 2], ax				

		mov ax, 4c00h
		int 21h	

end start                     