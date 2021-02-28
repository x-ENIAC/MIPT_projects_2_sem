.model tiny
.code

org 100h
        
;--------------------------------------------------------------------------
videoseg	= 0b800h
color		= 0Eh
color_regs	= 0Eh
count_rect	= 2

number		= 3802
width_frame	equ 11
height_frame	equ 8

symbols_into_line equ 160
two_bytes	equ 2
max_symbols_into_regs	equ 4

binary_system		equ 2
octal_system		equ 8
decimal_system		equ 10
hexadecimal_system	equ 16

horizontal_char	equ 0ecdh		; =
vertical_char   equ 0ebah     	; ¶
up_right_corner	equ 0ebbh		; ђ
up_left_corner	equ 0ec9h		; a
dn_right_corner equ 0ebch         ; -
dn_left_corner	equ 0ec8h        	; L
space		equ 0e00h
;--------------------------------------------------------------------------



Start:		mov bx, 0
		mov es, bx		; сегмент, равный нулю (будем адресоватьс€ к таблице прерываний)
		mov bx, 9 * 4		; 9 - номер прерывани€, 4 - размер €чейки
		mov ax, es:[bx]
		mov Old09_ofs, ax	; старый адрес
		mov ax, es:[bx+2]	; старша€ часть адреса
		mov Old09_seg, ax	; 

		;call New_handler_09_interrupt
		;ret

		cli					
		mov word ptr es:[bx], offset New_handler_09_interrupt
		mov ax, cs				
		mov es:[bx+2], ax       		
		sti								
							
		mov ax, 3100h				
		mov dx, offset End_program		
		shr dx, 4				
		inc dx					
                int 21h

	close_program:
	                
                mov ax, 4c00h
                int 21h
;------------------------------------------------------------------Ч-------
                


;------------------------------------------------------------------Ч-------
New_handler_09_interrupt proc
		push ax bx cx dx di ds es si		

		mov di, videoseg
		mov es, di
		mov di, (80 + 40) * 2	; (80 + 40) * 2
		mov ah, 4eh

                mov ax, cs
		mov ds, ax		

		in al, 60h   		; read symbol from controller
		;mov es:[di], ax
				
		;cmp al, 30h           	; Binary
		;je binary_numeral_system

		;cmp al, 18h           	; Octal
		;je octal_numeral_system

                ;cmp al, 20h           	; Decimal
		;je decimal_numeral_system		

		;cmp al, 23h       	; Hexadecimal
		;je hexadecimal_numeral_system

	continue_processing_click:

		mov copy_ax_for_print, ax
		mov copy_bx_for_print, bx
		mov copy_cx_for_print, cx
		mov copy_dx_for_print, dx	
			

		cmp al, 02h		; сравниваем символ со скан-кодом 2
		jne EOI		   
                            		
		call draw_frame		
		call return_last_interrupt_handler

		pop si es ds di dx cx bx ax
		iret
		;ret


EOI:		                           
                mov ax, cs
		mov ds, ax		
                                
		;mov copy_ax_for_print, ax
		;mov copy_bx_for_print, bx
		;mov copy_cx_for_print, cx
		;mov copy_dx_for_print, dx		

		;call draw_frame		
		;call return_last_interrupt_handler

		pop si es ds di dx cx bx ax    	; восстановление регистров		

		db 0eah				; самомодифицирущийс€ код (прыжок на старый обработчик) 		
Old09_ofs	dw 0
Old09_seg	dw 0
		
		iret

New_handler_09_interrupt endp
;------------------------------------------------------------------Ч-------

binary_numeral_system:
		mov now_numeral_system, binary_system
		jmp continue_processing_click

octal_numeral_system:
		mov now_numeral_system, octal_system
		jmp continue_processing_click

decimal_numeral_system:
		mov now_numeral_system, decimal_system
		jmp continue_processing_click		

hexadecimal_numeral_system:
		mov now_numeral_system, hexadecimal_system
		jmp continue_processing_click
		



;------------------------------------------------------------------Ч-------
;-------------------calculate offset-----------------------------Ч---------
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
;------------------------------------------------------------------Ч-------



;------------------------------------------------------------------Ч-------
;-------------draw frame with values of registers----------------Ч---------
;------------- !!! trash list: si, es, ds, di !!! -------------------------

draw_frame proc
		push ax bx cx dx

	;-----------draw rectangle---------------		

		push y_left x_left width_frame height_frame				
		call draw_rect
		add sp, 4 * 2

		add x_left, 2

	;-----------print ax---------------------		

		add y_left, 1
		push y_left x_left offset register_ax
		call print_message_from_variable
		add sp, 3 * 2		

	        add x_left, 3
       		;mov ax, 8888
	        mov ax, copy_ax_for_print	        	        
	        push ax now_numeral_system
	        mov di, bx	   
	        call itoa

	        mov bx, di
	        push y_left x_left
	        call print_message_from_stack
	        add sp, 2 * 2
	        sub x_left, 3		

	;-----------print bx---------------------
			
		add y_left, 2
		push y_left x_left offset register_bx
		call print_message_from_variable
		add sp, 3 * 2

	        add x_left, 3	        
	        push copy_bx_for_print now_numeral_system
	        mov di, bx	   
	        call itoa

	        mov bx, di
	        push y_left x_left
	        call print_message_from_stack
	        add sp, 2 * 2
	        sub x_left, 3
		
	;-----------print cx---------------------		

		add y_left, 2
		push y_left x_left offset register_cx
		call print_message_from_variable
		add sp, 3 * 2

	        add x_left, 3	        
	        push copy_cx_for_print now_numeral_system
	        mov di, bx	   
	        call itoa

	        mov bx, di
	        push y_left x_left
	        call print_message_from_stack
	        add sp, 2 * 2
	        sub x_left, 3
	
	;-----------print dx---------------------		
		                   
		add y_left, 2
		push y_left x_left offset register_dx
		call print_message_from_variable
		add sp, 3 * 2

	        add x_left, 3	        
	        push copy_dx_for_print now_numeral_system
	        mov di, bx	   
	        call itoa

	        mov bx, di
	        push y_left x_left
	        call print_message_from_stack
	        add sp, 2 * 2 
	        sub x_left, 3

	        sub y_left, 2 * 4

	        sub x_left, 2
	        add y_left, 1

		pop dx cx bx ax

		ret
draw_frame endp	
;------------------------------------------------------------------Ч-------



;------------------------------------------------------------------Ч-------
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



;-------------------------------------------------------------------Ч------
;------------------------draw line---------------------------------Ч-------
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
		mov es:[bx], ax
		add bx, two_bytes	
		
		mov cx, the_length
		sub cx, 2

		mov ax, central_symbol 

	draw_the_middle:
		mov es:[bx], ax
		add bx, two_bytes		
		loop draw_the_middle

		mov ax, right_symbol
		mov es:[bx], ax
		add bx, two_bytes
		
		pop bp	
	         						
		ret

draw_line endp
;-------------------------------------------------------------------------- 

                                                      

;--------------------------------------------------------------------------
;-------------------print msg from stack-----------------------------------
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

		;mov di, bx
		cld	

	print_loop:
		lodsb		
	        cmp al, '$'
		je end_print_loop		
		mov es:[bx], ax
		add bx, 2	
		jmp print_loop

	end_print_loop:
		pop bp

		ret			
print_message_from_variable	endp
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

		pop length_number_into_itoa

		mov ah, color

	padding_by_zeroes:
		cmp length_number_into_itoa, max_symbols_into_regs
		jae print_loop_stack
		mov al, '0'
		mov es:[bx], ax
		add bx, 2
		add length_number_into_itoa, 1
		jmp padding_by_zeroes		 	

	print_loop_stack:
		pop ax
		mov ah, color			
	        cmp al, '$'
		je end_loop_stack		
		mov es:[bx], ax
		add bx, 2
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
		mov length_number_into_itoa, 0
		mov copy_bp_for_itoa, bp
		mov bp, sp	
		
		mov ax, [bp + 4]		; number
		mov bx, [bp + 2]		; system_to_convert 

		pop cx
		push '$'

		cmp ax, 0000h
		jne itoa_loop

		push '0'
		inc length_number_into_itoa
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
		inc length_number_into_itoa
		jmp itoa_loop

	end_itoa_func:	
		push length_number_into_itoa
		push cx

		mov bp, copy_bp_for_itoa

		ret
itoa endp
;--------------------------------------------------------------------------

;--------------------------------------------------------------------------

return_last_interrupt_handler proc
		;in al, 61h  		; got the symbol
		;mov ah, al

		or al, 80h 		; put the most significant bit
		out 61h, al 		; put symbol
		mov al, ah
		out 61h, al

		mov al, 20h     	; inform the controller that our work is finished
		out 20h, al

		ret
return_last_interrupt_handler endp		


;-------------------------------------------------------------------------
;----------------------------Vars-----------------------------------------		    
.data

register_ax		db 'ax$'
register_bx		db 'bx$'
register_cx		db 'cx$'
msg 		db 'R~a~s~-~r~a~s~-~r~a~s~p~u~t~i~n~ ~l~o~v~e~r~ ~o~f~ ~t~h~e~ ~R~u~s~s~i~a~n~ ~q~u~e~e~n$'
register_dx		db 'dx$'

length_number_into_itoa	dw 0


x_left			dw 68
y_left			dw 3
copy_bp_for_itoa	dw 0
now_numeral_system	dw 10

copy_ax_for_print	dw 999h
copy_bx_for_print	dw 888h
copy_cx_for_print	dw 777h
copy_dx_for_print	dw 666h
;-------------------------------------------------------------------------
                   
            
End_program:	
		
end		Start			
