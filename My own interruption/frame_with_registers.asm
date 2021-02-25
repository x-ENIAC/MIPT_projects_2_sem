.model tiny
.code

org 100h

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

;--------------------------------------------------------------------------
videoseg	= 0b800h
color		= 0Eh
color_regs	= 0Eh
count_rect	= 2

number		= 3802
x_right		equ 75
x_left		equ 60
width_frame	equ 9
height_frame	equ 9

y_left		equ 3
symbols_into_line equ 160
two_bytes	equ 2

true		= 1
false		= 0

horizontal_char	equ 0ecdh		; =
vertical_char   equ 0ebah     	; ¶
up_right_corner	equ 0ebbh		; ђ
up_left_corner	equ 0ec9h		; a
dn_right_corner equ 0ebch         ; -
dn_left_corner	equ 0ec8h        	; L
space		equ 0e00h
;--------------------------------------------------------------------------



Start:		mov ax, 0b800h
		mov es, ax
		mov di, 80 * 2
		mov ah, 4eh
		
		;cld			; clear destination flag 
				
Next:		;in al, 60h                 -------------
		;stosw
		;mov es:[di], ax
		;cmp al, 1
		;jne Next 	------------

		;mov bx, 0
		;mov es, bx		; сегмент, равный нулю (будем адресоватьс€ к таблице прерываний)
		;mov bx, 9 * 4		; 9 - номер прерывани€, 4 - размер €чейки
		;mov ax, es:[bx]
		;mov Old09_ofs, ax	; старый адрес
		;mov ax, es:[bx+2]	; старша€ часть адреса
		;mov Old09_seg, ax	; 

		call New09
		ret

		;----------------- работа с таблицей прерываний ---------------------
		cli			; запрещаем прерывани€, чтобы помен€ть сегмент и смещение
		mov es:[bx], offset New09	; кладЄм смещение
		mov ax, cs		; узнаем сегмент
		mov es:[bx+2], ax       	; кладЄм сегмент
		sti			; разрешаем прерывани€

		;mov ax, 4c00h		; 4c - освобождает пам€ть после завершени€ - будет сущий кошмар
					; (адрес-то останетс€, а его содержимое помен€етс€)
		mov ax, 3100h		; Terminate 
		mov dx, offset EndProgram	; Memory size - в параграфах - сколько проге нужно пам€ти (1 пар. = 16 байт)		
		shr dx, 4		; делим на 16
		inc dx			; адрес может не нацело делитьс€ 
                int 21h

                mov ax, 4c00h
                int 21h
;------------------------------------------------------------------Ч-------
                

New09	proc
		;push es di ax
		
		;mov di, videoseg
		;mov es, di
		;mov di, (80) * 2	; (80 + 40) * 2
		;mov ah, 4eh
		;in al, 60h   		; read symbol from controller
		;mov es:[di], ax
		
		;cmp al, 02h		; сравниваем символ со скан-кодом 2
		;jne EOI

		;mov es:[di+4],  ax	; если гор€ча€ клавиша, то р€дом кладем нас же

		push ax bx cx dx di ds es si

		mov ax, cs
		mov ds, ax
                            		
		call draw_frame
		pop si es ds di dx cx bx ax		

		ret

		;call print_regs          
		


		;call return_last_interrupt_handler

		;pop ax di es
		iret
		;ret

EOI:		pop ax di es    	; восстановление регистров

		db 0eah			; самомодифицирущийс€ код (прыжок на старый обработчик) 		
Old09_ofs	dw 0
Old09_seg	dw 0
		
		;iret

New09	endp
;------------------------------------------------------------------Ч-------



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


draw_frame proc
		push ax bx cx dx

		push y_left
		push x_left
		push width_frame
		push height_frame		

		call draw_rect
		add sp, 4 * 2		             

		pop dx cx bx ax

		ret
draw_frame endp	



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
		
		mov cx, the_width

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
;---------------------------delay------------------------------------------
;---------- !!! trash list: dx, cx, ah !!! --------------------------------

delay 	proc     
		;mov dx, 64	;3EBh	;0BB8h
		;xor cx, cx
		;mov ah, 86h
		;int 15h
          
		ret

delay endp
;--------------------------------------------------------------------------

                                                      

;--------------------------------------------------------------------------
;-------------------print msg from stack-----------------------------------
;---------- !!! trash list: ax, bx, cx, di !!! ----------------------------

print_msg_from_stack	proc message_offset
		;pop cx
		xor ax, ax

		push bp
		mov bp, sp

		mov si, message_offset
		mov ah, color		

	print_cycle_stack:
		;pop ax
		lodsb

	        cmp al, '$'
		je end_cycle_stack		
		stosw	
		jmp print_cycle_stack

	end_cycle_stack:
		;push cx

		pop bp
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



;--------------------------------------------------------------------------
;-------------------Print regs' values-------------------------------------
;---------- !!! trash list: !!! ------------------------------------
print_regs proc
		;mov ah, 13h
		;mov cx, 2
		;mov bl, color_regs

		push offset reg_ax
		call print_msg_from_stack

		ret
print_regs endp
;--------------------------------------------------------------------------


return_last_interrupt_handler proc
		;in al, 61h  		; достали символ
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

reg_ax		db 'ax', '$'
reg_bx		db 'bx$'
reg_cx		db 'cx$'
msg 		db 'R~a~s~-~r~a~s~-~r~a~s~p~u~t~i~n~ ~l~o~v~e~r~ ~o~f~ ~t~h~e~ ~R~u~s~s~i~a~n~ ~q~u~e~e~n$'
reg_dx		db 'dx$'

is_first_line_drawn db false
;-------------------------------------------------------------------------
                   
            
EndProgram:	
		
end		Start			
