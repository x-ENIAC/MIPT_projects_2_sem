.model tiny
                                                                                   
x		= 15
y		= 24
videoseg	= 0b800h		; The address of the video memory
color1		= 80h           ; Full animation begin from 80h = 1 000 0000 
							; (1 = animation, 000 = black backgroung, 0000 = black symbols)
                                                                                
.code
org 100h
start:		mov ax, videoseg
		mov es, ax
		mov ah, color1
		mov si, offset msg
		mov di, (y * 80 + x) * 2	; Offset relative to the upper-left corner

		mov bl, color1 

again:		lodsb		        ; mov al, [si] - put next byte from [SI] to AL
		cmp al, '$'
		je  done
			
		cmp al, '~'		; change colour
		je  changecolor
		stosw           	; mov es:[di], ax / inc di - copies the world from AX to the memory
							; location that ES:DI points. Also does INC or DEC DI to prepare
							; for the next copy
		jmp again

changecolor:	inc bl
		mov ah, bl
		jmp again 

done:		mov ax, 4c00h
		int 21h

.data
msg 		db 'R~a~s~-~r~a~s~-~r~a~s~p~u~t~i~n~ ~l~o~v~e~r~ ~o~f~ ~t~h~e~ ~R~u~s~s~i~a~n~ ~q~u~e~e~n$'

end start                                                                   