.model tiny
                                                                                   
x		= 15
y		= 24
videoseg	= 0b800h
color1		= 8eh           ;Full animation begin from 8eh, part - from 5eh
                                                                                
.code
org 100h
start:		mov ax, videoseg
		mov es, ax
		mov ah, color1
		mov si, offset msg
		mov di, (y * 80 + x) * 2

		mov bl, color1 

again:		lodsb		        ; mov al, [si]
		cmp al, '$'
		je  done
			
		cmp al, '~'		; changt colour
		je  changecolor
		stosw           	; mov es:[di], ax / inc di
		jmp again

changecolor:	inc bl
		mov ah, bl
		jmp again 

done:		mov ax, 4c00h
		int 21h

.data
msg 		db 'R~a~s~-~r~a~s~-~r~a~s~p~u~t~i~n~ ~l~o~v~e~r~ ~o~f~ ~t~h~e~ ~R~u~s~s~i~a~n~ ~q~u~e~e~n$'

end start                                                                   