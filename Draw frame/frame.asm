; This program displays the frame on the screen

.model tiny
.code
org 100h
          

           
;---------------Constants--------------------------------------------------

videoseg	= 0b800h
color		= 0Eh
color_regs	= 0Eh
count_rect	= 2

number		= 3802
width_frame	equ 30
height_frame	equ 8

symbols_into_line equ 160
two_bytes	equ 2
max_symbols_into_regs	equ 4

binary_system		equ 2
octal_system		equ 8
decimal_system		equ 10
hexadecimal_system	equ 16

horizontal_delta_offset	equ 14
vertical_delta_offset	equ 2

horizontal_char	equ 0ecdh		; =
vertical_char   equ 0ebah     	; ¦
up_right_corner	equ 0ebbh		; ¬
up_left_corner	equ 0ec9h		; a
dn_right_corner equ 0ebch         ; -
dn_left_corner	equ 0ec8h        	; L
shadow_char	equ 4eB0h
space		equ 0e00h
background	equ 1e00h
;--------------------------------------------------------------------------
		


;--------------------------------------------------------------------------		
;---------------------begin the program------------------------------------
;---------- !!! trash list: ax, es, cx, di, si !!! ------------------------

Start:		mov di, videoseg
		mov es, di

		call draw_frame	

		mov ax, 4c00h
		int 21h
;--------------------------------------------------------------------------		
                                                                          		


;------------------------------------------------------------------—-------
;-------------------calculate offset-----------------------------—---------
;-------------- !!! trash list: ax, bx !!! --------------------------------
	
offset_calculate proc 
		push bx
		
		mov bx, two_bytes
		mul bx

		mov bx, ax
		mov ax, cx

		mov cx, symbols_into_line
		mul cx
		  
		add ax, bx
		pop bx

		ret
offset_calculate endp		
;------------------------------------------------------------------—-------
		



;--------------------------------------------------------------------------
;---------------------------delay------------------------------------------
;---------- !!! trash list: dx, cx, ah !!! -------------------------------- 

delay 	proc     
		push cx dx ax

		mov dx, 64h
		xor cx, cx
		mov ah, 86h
		int 15h

		pop ax dx cx
          
		ret

delay endp
;--------------------------------------------------------------------------



;------------------------------------------------------------------—-------
;-------------draw frame with values of registers----------------—---------
;------------- !!! trash list: si, es, ds, di !!! -------------------------

draw_frame proc
		push ax bx cx dx

	;-----------draw rectangle---------------		

		push y_left x_left width_frame height_frame				
		call draw_rect
		add sp, 4 * 2

		push y_left x_left width_frame height_frame
		call draw_shadow
		add sp, 4 * 2

		add x_left, 2

		call print_numbers		

		pop dx cx bx ax

		ret
draw_frame endp	
;------------------------------------------------------------------—-------



;------------------------------------------------------------------—-------
;------------------------draw rect-----------------------------------------
;-------------- !!! trash list: ax, dx, cx !!! ----------------------------
	   
draw_rect proc the_height, the_width, horizontal_offset, vertical_offset
		push bp
		mov bp, sp

		mov bx, videoseg
		mov es, bx

		push vertical_offset
		push horizontal_offset
		push the_width		
		push up_right_corner 
		push horizontal_char
		push up_left_corner 				
		call draw_line
		add sp, 6 * 2		
		
		mov cx, the_height

	draw_central_lines:
		inc vertical_offset

		push cx

		push vertical_offset
		push horizontal_offset
		push the_width		
		push vertical_char 
		push space
		push vertical_char
		call draw_line
		add sp, 6 * 2		

		pop cx

		loop draw_central_lines

		push vertical_offset
		push horizontal_offset
		push the_width		
		push dn_right_corner
		push horizontal_char
		push dn_left_corner
		call draw_line
		add sp, 6 * 2		

		pop bp

		ret 
draw_rect endp
;--------------------------------------------------------------------------


draw_shadow proc the_height, the_width, horizontal_offset, vertical_offset
		push bp
		mov bp, sp
		
		push ax cx

		mov ax, horizontal_offset
		add ax, the_width		
		mov cx, vertical_offset
		add cx, 1
		call offset_calculate

		mov bx, ax

		pop cx ax

		mov cx, the_height
		mov ax, shadow_char

	draw_vertical_shadow:		
		
		call put_symbol
		call go_to_next_line
		call delay
		loop draw_vertical_shadow

		mov cx, the_width

	draw_horizontal_shadow:

        	call put_symbol    
        	call go_back
        	call delay
        	loop draw_horizontal_shadow

		pop bp

		ret
draw_shadow endp		



;-------------------------------------------------------------------—------
;------------------------draw line---------------------------------—-------
;-------------- !!! trash list: cx, ax !!! --------------------------------

draw_line proc left_symbol, central_symbol, right_symbol, the_length, goriz_offset, vertical_offset

		push bp
		mov bp, sp

		push ax cx

		mov ax, goriz_offset 
		mov cx, vertical_offset
		call offset_calculate

		mov bx, ax

		pop cx ax

		mov ax, left_symbol
		call put_symbol
		call go_forward
		call delay	
		
		mov cx, the_length
		sub cx, 2

		mov ax, central_symbol 

	draw_the_middle:
		call put_symbol
		call go_forward
		call delay		
		loop draw_the_middle

		mov ax, right_symbol
		call put_symbol
		call go_forward
		call delay
		
		pop bp	
	         						
		ret

draw_line endp
;-------------------------------------------------------------------------- 


;--------------------------------------------------------------------------
;---------print numbers in different systems-------------------------------
;---------- !!! trash list: ax, es, cx, di, si !!! ------------------------

print_numbers proc
	;----------- decimal ---------------------		

		add y_left, 1
		push y_left x_left offset decimal
		call print_message_from_variable
		add sp, 3 * 2

	        add x_left, horizontal_delta_offset	        
	        push number decimal_system
	        mov di, bx	   
	        call itoa

	        mov bx, di
	        push y_left x_left
	        call print_message_from_stack
	        add sp, 2 * 2
	        sub x_left, horizontal_delta_offset

	;----------- binary ---------------------

		add y_left, vertical_delta_offset
		push y_left x_left offset binary
		call print_message_from_variable
		add sp, 3 * 2

	        add x_left, horizontal_delta_offset       
	        push number binary_system
	        mov di, bx	   
	        call itoa

	        mov bx, di
	        push y_left x_left
	        call print_message_from_stack
	        add sp, 2 * 2
	        sub x_left, horizontal_delta_offset

	;----------- octal ----------------------		

		add y_left, vertical_delta_offset
		push y_left x_left offset octal
		call print_message_from_variable
		add sp, 3 * 2

	        add x_left, horizontal_delta_offset	        
	        push number octal_system
	        mov di, bx	   
	        call itoa

	        mov bx, di
	        push y_left x_left
	        call print_message_from_stack
	        add sp, 2 * 2
	        sub x_left, horizontal_delta_offset
	        

	;----------- hexadecimal ----------------		

		add y_left, vertical_delta_offset
		push y_left x_left offset hexadecimal
		call print_message_from_variable
		add sp, 3 * 2

	        add x_left, horizontal_delta_offset	        
	        push number decimal_system
	        mov di, bx	   
	        call itoa

	        mov bx, di
	        push y_left x_left
	        call print_message_from_stack
	        add sp, 2 * 2
	        sub x_left, horizontal_delta_offset
	        
		ret
		
print_numbers endp
;--------------------------------------------------------------------------  
		
	                                                     

;--------------------------------------------------------------------------
;-------------------print symbol variable----------------------------------
;---------- !!! trash list: ax, bx, cx, di !!! ----------------------------

print_message_from_variable proc message_offset, gorizontal_offset, vertical_offset 
		push bp
		mov bp, sp

		push ax cx

		mov ax, gorizontal_offset 
		mov cx, vertical_offset
		call offset_calculate
		mov bx, ax

		pop cx ax	

		mov si, message_offset
		mov ah, color

		cld	

	print_loop:
		lodsb		
	        cmp al, '$'
		je end_print_loop
				
		call put_symbol
		call go_forward
		call delay
		jmp print_loop

	end_print_loop:
		pop bp

		ret			
print_message_from_variable	endp
;--------------------------------------------------------------------------



;--------------------------------------------------------------------------
;------------------put symbol into es:[bx]---------------------------------
;-------------- !!! trash list: nothing... !!! ----------------------------

put_symbol proc

		mov es:[bx], ax
		ret

put_symbol endp
;--------------------------------------------------------------------------



;--------------------------------------------------------------------------
;-------------adds to bx 2 (needs to put symbol)---------------------------
;------------------- !!! trash list: bx !!! -------------------------------

go_forward proc
		add bx, 2
		ret
go_forward endp
;--------------------------------------------------------------------------
                           


;--------------------------------------------------------------------------
;-------------subs to bx 2 (needs to put symbol)---------------------------
;------------------- !!! trash list: bx !!! -------------------------------

go_back proc
		sub bx, 2
		ret
go_back endp
;--------------------------------------------------------------------------



;--------------------------------------------------------------------------
;-------------adds to bx 160 (needs to put symbol)-------------------------
;------------------- !!! trash list: bx !!! -------------------------------

go_to_next_line proc
		add bx, symbols_into_line
		ret
go_to_next_line endp
;--------------------------------------------------------------------------


print_message_from_stack proc gorizontal_offset, vertical_offset
		mov cx, bp
		mov bp, sp

		pop dx		
		push cx dx

		mov ax, [bp + 2]	; gorizontal_offset 
		mov cx, [bp + 4]	; vertical_offset
		call offset_calculate
		mov bx, ax
		pop dx cx		

		add sp, 2 * 2

		mov ah, color                                     		 	

	print_loop_stack:
		pop ax
		mov ah, color			
	        cmp al, '$'
		je end_loop_stack
				
		call put_symbol
		call go_forward
		call delay
		jmp print_loop_stack

	end_loop_stack:
		mov bp, cx
		push dx

		ret

print_message_from_stack endp



;--------------------------------------------------------------------------
;---------------------------Itoa-------------------------------------------
;---------- !!! trash list: ax, bx, cx, dx !!! ----------------------------

itoa proc system_to_convert, number
		mov copy_bp_for_itoa, bp
		mov bp, sp	
		
		mov ax, [bp + 4]		; number
		mov bx, [bp + 2]		; system_to_convert 

		pop cx
		push '$'

		cmp ax, 0000h
		jne itoa_loop

		push '0'
		jmp end_itoa_func

	itoa_loop:	
		cmp ax, 0000h
		je end_itoa_func
		
		mov dx, 0000h
		div bx
		add dx, '0'

		cmp dx, '9'
		jbe is_a_digit

		add dx, 'A' - '9' - 1
		
	is_a_digit:
		push dx
		jmp itoa_loop

	end_itoa_func:	
		push cx

		mov bp, copy_bp_for_itoa

		ret
itoa endp
;--------------------------------------------------------------------------
		

;-------------------------------------------------------------------------
;----------------------------Vars-----------------------------------------		    
.data
x_left			dw 25
y_left			dw 10

decimal			db 'Decimal: $'
hexadecimal		db 'Hexadecimal: $'
octal			db 'Octal: $'
binary			db 'Binary: $'

copy_bp_for_itoa	dw 0
;-------------------------------------------------------------------------


end	Start	
