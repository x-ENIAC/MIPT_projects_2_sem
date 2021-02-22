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

do_code macro your_code
		local do_nothing

		cmp drawn_first_rect, true
		je do_nothing

		your_code

	do_nothing:	
		nop
endm		

;-------------------------------------------------------------------------
           

           
;---------------Constants--------------------------------------------------

videoseg	= 0b800h
color		= 0Eh
count_rect	= 3

number		= 3802

true		= 1
false		= 0

short_delay	= 64

horizontal_char	= 0ecdh		; =
vertical_char   = 0ebah     	; ¦
up_right_corner	= 0ebbh		; ¬
up_left_corner	= 0ec9h		; ã
dn_right_corner = 0ebch         ; -
dn_left_corner	= 0ec8h        	; L
space		= 0e00h
;--------------------------------------------------------------------------
		


;--------------------------------------------------------------------------		
;---------------------begin the program------------------------------------
;---------- !!! trash list: ax, es, cx, di, si !!! ------------------------

Start:		mov ax, videoseg
		mov es, ax 

;-----------------------begin draw-----------------------------------------
		
		mov cx, count_rect
rects:
		push cx		
		call offset_calculate       ; write into ax

		mov di, ax
		mov is_first_central_line, false	
		call draw_rect
		do_code <call draw_shadow>

		pop cx

		sub x_left, 2
		sub y_left, 2
		add x_right, 2
		sub y_right, 1

		mov drawn_first_rect, true

		loop rects

;---------------begin the program------------------------------------------
		inc x_left
		mov si, offset decimal		
		call print_msg         			; print "Decimal:"                       

		mov ax, number
		mov bx, 10
		push '$'
		call itoa
		call print_msg_from_stack		; Decimal number
		                                                            
		add y_left, 2		                                    
		mov si, offset hexadecimal		                    
		call print_msg          		; print "Hexadecimal:"

		mov ax, number
		mov bx, 16
		push '$'
		call itoa
		call print_msg_from_stack     		; Hexadecimal number

		add y_left, 2		                                    
		mov si, offset octal		                    
		call print_msg          		; print "Octal:"

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
;------------------------draw rect-----------------------------------------
;---------- !!! trash list: ax, bl, dx, cx !!! ----------------------------
	   
draw_rect proc
;---------------draw first line-------------------------------------—------
		mov ax, up_left_corner
		mov bl, 0 ; flag of the draw first line
		call draw_up_and_down_line
		mov bl, true

		mov ax, space
		stosw		

;---------------draw all central lines-------------------------------—-----  
		mov dx, y_right
		sub dx, y_left
		sub dx, 1

		mov cx, dx       
my_loop:
		mov dx, cx
		call draw_central_line		
		mov cx, dx
		mov is_first_central_line, true		

		loop my_loop

;---------------draw last line-----------------------------------------—---
                mov cx, symbols_into_line ; 80 - (x_right - x_left) - 2
   		sub cx, x_right
   		add cx, x_left
   		sub cx, 3
   		add cx, drawn_first_rect   ; made so that the frame doesn't move
   					   ; in the absence of a shadow 		
		mov ax, space

print_probel:	stosw
		save <dx, cx, ax>
		call delay
		restore <ax, cx, dx>
		loop print_probel

		mov ax, dn_left_corner
		call draw_up_and_down_line

		do_code<mov ax, 4eB0h>
		do_code<stosw>              		                                          		

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
			
		cmp bl, true
		jne draw_first_line                                      
		mov ax, dn_right_corner
		mov bl, true

		stosw
		;mov ax, 4eB0h
		;stosw

		ret
		
	draw_first_line:
		inc cx
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
   		sub cx, 3
   		
   		cmp drawn_first_rect, true
   		jne not_remove_offset_for_shadow

		cmp is_first_central_line, false
		je not_remove_offset_for_shadow

		add cx, 1

            not_remove_offset_for_shadow: 
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

		do_code<mov ax, 4eB0h>
		do_code<stosw>
				
		ret
draw_central_line endp
;-------------------------------------------------------------------—------



;--------------------------------------------------------------------------
;----------------------draw the shadow-------------------------------------
;---------- !!! trash list: !!! --------------------------------

draw_shadow proc 
		add di, symbols_into_line
		add di, symbols_into_line
		sub di, x_right
		sub di, x_right
		add di, x_left
		add di, x_left

		push ax 
		mov ax, di
		shr di, 1
		shl di, 1
		cmp di, ax
	 
		je draw_from_this_place

		add di, 1
                		
	draw_from_this_place:
		;pop bx
		;pop di
		pop ax
				
		mov cx, x_right
		sub cx, x_left
		mov ax, 4eB0h

	loop_draw_shadow:		
		mov es:[di], ax
		add di, 2
		loop loop_draw_shadow

		ret
		
draw_shadow endp
;--------------------------------------------------------------------------



;--------------------------------------------------------------------------
;---------------------------delay------------------------------------------
;---------- !!! trash list: dx, cx, ah !!! --------------------------------
;---------- dx = delay (into mircoseconds) --------------------------------
delay 	proc     
		mov dx, short_delay
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

	print_msg_loop:
		lodsb
		cmp al, '$'
		je end_print_msg		
		stosw	
		jmp print_msg_loop

	end_print_msg:
		ret			
print_msg	endp
;--------------------------------------------------------------------------



;--------------------------------------------------------------------------
;-------------------print msg from stack-----------------------------------
;---------- !!! trash list: ax, bx, cx, di !!! ----------------------------

print_msg_from_stack	proc
		pop cx
		xor ax, ax

	push_into_stack:
		pop ax
		mov ah, color
	        cmp al, '$'
		je end_push_into_stack
				
		stosw	
		jmp push_into_stack

	end_push_into_stack:
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
msg			db 'Hello$'

x_left			dw 23 
x_right 		dw 47
y_left			dw 14             
y_right			dw 22
symbols_into_line	dw 80
two_bytes		dw 2
count_lines		dw 7
drawn_first_rect 	dw 0	; 0 - not drawn, 1 - drawn
is_first_central_line 	dw 0

decimal			db 'Decimal: $'
hexadecimal		db 'Hexadecimal: $'
octal			db 'Octal: $'
binary			db 'Binary: $'
;-------------------------------------------------------------------------


end	Start	
