; This program displays the frame on the screen

.model tiny
.code
org 100h
                           

;---------------Constants--------------------------------------------------

videoseg	= 0b800h
color		= 4Eh
count		= 2

x_left		= 30 ; 30
x_right 	= 70
y_left		= 12
y_right		= 17

horizontal_char	= 0ecdh		; =
vertical_char   = 0ebah     	; ¦
up_right_corner	= 0ebbh		; ¬
up_left_corner	= 0ec9h		; ã
dn_right_corner = 0ebch         ; -
dn_left_corner	= 0ec8h        	; L
space		= 0e00h


;---------------begin the program--------------------------------------------

Start:		mov ah, 9
		mov dx, offset msg
		int 21h

		mov ax, videoseg
		mov es, ax            

		mov di, (y_left * 80 + x_left) * 2 

	      	mov dx, count

;---------------draw first line---------------------------------------
		mov ax, up_left_corner
		mov bl, 0				; flag of the draw first line	
		call draw_up_and_down_line
		mov bl, 1                                           	


;---------------draw all central lines---------------------------------
		mov cx, dx
my_loop:
		mov dx, cx
		call draw_central_line

		mov cx, dx
		loop my_loop                                          		


;---------------draw last line----------------------------------------
		mov ax, space
		mov cx, 80 - (x_right - x_left) - 2 

print_probel:	stosw
		loop print_probel

		mov ax, dn_left_corner
		call draw_up_and_down_line

;---------------end the program-------------------------------------------------		

		mov ax, 4c00h
		int 21h







;--------------------------------------------------------------------		
;---------------draw up and down line--------------------------------
draw_up_and_down_line proc
		
		;mov ax, up_left_corner
		stosw

		mov ax, horizontal_char
		mov cx, x_right - x_left	

lopa:		stosw			; mov es:[di], ax + inc ax
		loop lopa
		
		cmp ax, up_left_corner
		je put_dn_right_corner
		
		mov ax, up_right_corner

		cmp bl, 1
		jne draw_first_line
		mov ax, dn_right_corner
		mov bl, 1

draw_first_line:
		stosw

		ret

draw_up_and_down_line endp
;---------------------------------------------------------------------


;---------------------------------------------------------------------
;---------------draw central lines------------------------------------

draw_central_line proc
		mov ax, space
		mov cx, 80 - (x_right - x_left) - 2 

print_probels:	stosw
		loop print_probels


		mov ax, vertical_char
		stosw
		  
		mov ax, space 
		mov cx, x_right - x_left

lopa_2:		stosw
		loop lopa_2

		mov ax, vertical_char
		stosw

		ret
draw_central_line endp

;---------------------------------------------------------------------
;---------------------------------------------------------------------
put_dn_right_corner:		
		mov ax, dn_right_corner
		ret
;---------------------------------------------------------------------		

.data
msg		db 'Hello$'      



end	Start	
