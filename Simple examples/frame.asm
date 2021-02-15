; This program displays the frame on the screen

.model tiny
.code
org 100h

;------------------------------------------------------------------------
;---------------Macro----------------------------------------------------
save macro registers
	irp reg, <registers>
		push reg
	endm
endm

restore macro registers
	irp reg, <registers>
		pop reg
	endm
endm			
;-------------------------------------------------------------------------
           

           
;---------------Constants--------------------------------------------------

videoseg	= 0b800h
color		= 0Eh
count_rect	= 1

number		= 3802

horizontal_char	= 0ecdh		; =
vertical_char   = 0ebah     	; ¦
up_right_corner	= 0ebbh		; ¬
up_left_corner	= 0ec9h		; ã
dn_right_corner = 0ebch         ; -
dn_left_corner	= 0ec8h        	; L
space		= 0e00h
;--------------------------------------------------------------------------
		


;--------------------------------------------------------------------------		
;---------------begin the program------------------------------------------
;---------- !!! trash list: ax, es, cx, di, si !!! ------------------------

Start:		mov ax, videoseg
		mov es, ax 

;-----------------------begin draw-----------------------------------------
		
		mov cx, count_rect
rects:
		push cx		
		call offset_calculate       ; write into ax

		mov di, ax		
		call draw_rect

		pop cx

		sub x_left, 2
		sub y_left, 2
		add x_right, 2

		inc count_lines
		loop rects

;---------------begin the program------------------------------------------
		inc x_left
		mov si, offset decimal		
		call print_msg         			; Decimal:                       

		mov ax, number
		mov bx, 10
		push '$'
		call itoa
		call print_msg_from_stack		; Decimal number
		                                                            
		add y_left, 2		                                    
		mov si, offset hexadecimal		                    
		call print_msg          		; Hexadecimal:

		mov ax, number
		mov bx, 16
		push '$'
		call itoa
		call print_msg_from_stack     		; Hexadecimal number

		add y_left, 2		                                    
		mov si, offset octal		                    
		call print_msg          		; Octal:

		mov ax, number
		mov bx, 8
		push '$'
		call itoa
		call print_msg_from_stack     		; Octal number                              
		           		                                    
		add y_left, 2
		mov si, offset binary	                            
		call print_msg			 	; Binary:			                    

		mov ax, number
		mov bx, 2
		push '$'
		call itoa
		call print_msg_from_stack     		; Binary number
		                                                            
;---------------end the program--------------------------------------------
		mov ax, 4c00h
		int 21h
;--------------------------------------------------------------------------		



;------------------------------------------------------------------—-------
;--------------calculate offset----------------------------------—---------
;-------------- !!! trash list: ax, bx !!! --------------------------------
	
offset_calculate proc
		mov ax, y_left
		mov bx, symbols_into_line
		mul bx     
		add ax, x_left
		mov bx, two_bytes
		mul bx
		ret
offset_calculate endp		
;------------------------------------------------------------------—-------		



;------------------------------------------------------------------—-------
;---------------draw rect-----------------------------------------—--------
;---------- !!! trash list: ax, bl, dx, cx !!! ----------------------------
	   
draw_rect proc
;---------------draw first line-------------------------------------—------
		mov ax, up_left_corner
		mov bl, 0 ; flag of the draw first line
		call draw_up_and_down_line
		mov bl, 1

;---------------draw all central lines-------------------------------—-----  
		mov dx, count_lines
		mov cx, dx
my_loop:
		mov dx, cx
		call draw_central_line

		mov cx, dx
		loop my_loop

;---------------draw last line-----------------------------------------—---
                mov cx, symbols_into_line ; 80 - (x_right - x_left) - 2
   		sub cx, x_right
   		add cx, x_left
   		sub cx, 2
		mov ax, space

print_probel:	stosw
		save <dx, cx, ax>
		call delay
		restore <ax, cx, dx>
		loop print_probel

		mov ax, dn_left_corner
		call draw_up_and_down_line
		ret
draw_rect endp
;--------------------------------------------------------------------------		



;-------------------------------------------------------------------—------
;---------------draw up and down line------------------------------—-------
;---------- !!! trash list: cx, ax, bl !!! --------------------------------

draw_up_and_down_line proc

		stosw

		mov cx, x_right
		sub cx, x_left
		mov ax, horizontal_char
			
	lopa:	stosw ; mov es:[di], ax + inc ax
		save <dx, cx, ax>
		call delay
		restore <ax, cx, dx>
		loop lopa
		
		mov ax, up_right_corner
			
		cmp bl, 1
		jne draw_first_line                                      
		mov ax, dn_right_corner
		mov bl, 1
		
	draw_first_line:
		stosw
		save <dx, cx, ax>
		call delay
		restore <ax, cx, dx>
						
		ret

draw_up_and_down_line endp
;--------------------------------------------------------------------------



;-------------------------------------------------------------------—------
;---------------draw central lines----------------------------------—------
;-------------- !!! trash list: cx, ax !!! --------------------------------

draw_central_line proc                                      
                mov cx, symbols_into_line ; 80 - (x_right - x_left) - 2
   		sub cx, x_right
   		add cx, x_left
   		sub cx, 2             
		mov ax, space
		
print_probels:	stosw
		save <dx, cx, ax>
		call delay
		restore <ax, cx, dx>
		loop print_probels

		
		mov ax, vertical_char
		stosw
		
		mov cx, x_right
		sub cx, x_left
		mov ax, space
		
lopa_2: 	stosw
		save <dx, cx, ax>
		call delay
		restore <ax, cx, dx>
		loop lopa_2
		
		mov ax, vertical_char
		stosw

		save <dx, cx, ax>
		call delay
		restore <ax, cx, dx>
				
		ret
draw_central_line endp
;-------------------------------------------------------------------—------



;--------------------------------------------------------------------------
;---------------------------delay------------------------------------------
;---------- !!! trash list: dx, cx, ah !!! --------------------------------

delay 	proc     
		mov dx, 64	;3EBh	;0BB8h
		xor cx, cx
		mov ah, 86h
		int 15h
          
		ret

delay endp
;--------------------------------------------------------------------------



;--------------------------------------------------------------------------
;-------------------------print msg----------------------------------------
;---------- !!! trash list: ax, bx, di !!! --------------------------------

print_msg	proc		
 		
		mov ax, y_left
		add ax, 3
		mov bx, symbols_into_line  
		mul bx
		add ax, x_left 
		add ax, 3
		mov bx, 2
		mul bx

		mov di, ax


		mov ah, color
		xor al, al

	print_cycle:
		lodsb
		cmp al, '$'
		je end_cycle		
		stosw	
		jmp print_cycle

	end_cycle:
		ret			
print_msg	endp
;--------------------------------------------------------------------------



;--------------------------------------------------------------------------
;-------------------print msg from stack-----------------------------------
;---------- !!! trash list: ax, bx, cx, di !!! ----------------------------

print_msg_from_stack	proc
		pop cx
		xor ax, ax

	print_cycle_stack:
		pop ax
		mov ah, color
	        cmp al, '$'
		je end_cycle_stack		
		stosw	
		jmp print_cycle_stack

	end_cycle_stack:
		push cx
		ret			
print_msg_from_stack	endp
;--------------------------------------------------------------------------



;--------------------------------------------------------------------------
;---------------------------Itoa-------------------------------------------
;---------- !!! trash list: cx, dx !!! ------------------------------------

itoa proc
		pop cx
itoa_loop:	cmp ax, 0000h
		je end_func

		mov dx, 0000h
		div bx
		add dx, '0'

		cmp dx, '9'
		jbe is_a_digit

		add dx, 'A' - '9' - 1

	is_a_digit:
		push dx
		jmp itoa_loop

	end_func:
		push cx
		ret
itoa endp
;--------------------------------------------------------------------------

		

;-------------------------------------------------------------------------
;----------------------------Vars-----------------------------------------		    
.data
msg		db 'Hello$'

x_left		dw 23 
x_right 	dw 47
y_left		dw 14             
y_right		dw 22
symbols_into_line dw 80
two_bytes	dw 2
count_lines	dw 7

decimal		db 'Decimal: $'
hexadecimal	db 'Hexadecimal: $'
octal		db 'Octal: $'
binary		db 'Binary: $'
;-------------------------------------------------------------------------


end	Start	
