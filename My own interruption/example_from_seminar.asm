.model tiny
.code

org 100h
Start:		;mov ax, 0b800h
		;mov es, ax
		;mov di, (80 + 40) * 2
		;mov ah, 4eh
		;cld			; clear destination flag 
				
Next:		;in al, 60h
		;stosw
		;mov es:[di], ax
		;cmp al, 1
		;jne Next

		mov bx, 0
		mov es, bx		; сегмент, равный нулю (будем адресоватьс€ к таблице прерываний)
		mov bx, 9 * 4		; 9 - номер прерывани€, 4 - размер €чейки
		mov ax, es:[bx]
		mov Old09_ofs, ax	; старый адрес
		mov ax, es:[bx+2]	; старша€ часть адреса
		mov Old09_seg, ax	; 

		;----------------- работа с таблицей прерываний ---------------------
		cli			; запрещаем прерывани€, чтобы помен€ть сегмент и смещение
		mov es:[bx], offset New09	; кладЄм смещение
		mov ax, cs		; узнаем сегмент
		mov es:[bx+2], ax       	; кладЄм сегмент
		sti			; разрешаем прерывани€

		mov ax, 4c00h		; 4c - освобождает пам€ть после завершени€ - будет сущий кошмар
					; (адрес-то останетс€, а его содержимое помен€етс€)
		mov ax, 3100h		; Terminate 
		mov dx, offset EndProgram	; Memory size - в параграфах - сколько проге нужно пам€ти (1 пар. = 16 байт)		
		shr dx, 4		; делим на 16
		inc dx			; адрес может не нацело делитьс€ 
                int 21h

New09	proc
		push es di ax
		
		mov di, 0b800h
		mov es, di
		mov di, (80 + 40) * 2
		mov ah, 4eh
		in al, 60h   		; читаем из порта символ
		mov es:[di], ax
		
		cmp al, 02h		; сравниваем символ со скан-кодом 2
		jne EOI

		mov es:[di+4],  ax	; если гор€ча€ клавиша, то р€дом кладем нас же

		in al, 61h  		; достали символ
		mov ah, al
		or al, 80h 		; поставили старший бит
		out 61h, al
		mov al, ah
		out 61h, al

		mov al, 20h
		out 20h, al

		pop ax di es
		iret

EOI:		pop ax di es    	; восстановление регистров

		db 0eah			; самомодифицирущийс€ код (прыжок на старый обработчик) 		
Old09_ofs	dw 0
Old09_seg	dw 0
		
		;iret

New09	endp		
                    

EndProgram:	
		
end		Start			
