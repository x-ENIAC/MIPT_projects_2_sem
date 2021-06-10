
Well, this repo is created for 2 semester study projects at MIPT. Below you can see the names of the projects and their small descriptions.

## [Drawing a frame](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/Draw%20frame)

This program draws a frame with a shadow. Seems like that's all. 

The work was carried out in the operating system emulator called DOSBox.

![image](https://sun9-65.userapi.com/impf/xvJGBJR0lX1LcKXh6apjE3Eh0n_mntwcbf0WBA/B3OWxmRyd0I.jpg?size=1023x672&quality=96&sign=33e0ed1c2f34c2ac0034cb78092b7b09&type=album)

## [Creating your own handler for the 9th interrupt](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/My%20own%20interruption)

The work was carried out in the operating system emulator called DOSBox.

![image](https://sun9-14.userapi.com/impf/R258qUMBbqoFVXIe01chKSFfts4qyVtKoTbN1g/FbuSpPh93T0.jpg?size=1026x674&quality=96&sign=cc3070f40f8f282e53135e786947025f&type=album)

## [My own printf](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/My%20own%20printfn)

This program checks the password that you give it on input. The program reports about any verdict (correct/incorrect). The purpose of writing this program is to be used in the next work (see below). So that my program would not be quickly solved, I added a lot of unnecessary and confusing jumps to useless labels, etc. The rabbit confused the tracks with all his might. But a couple of vulnerabilities had to be left for the fox to win.

The work was carried out in the operating system emulator called DOSBox.

## [Cracking password](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/Hack%20and%20win)

My university partner wrote a program in assembler that checks if the password is correct. She purposefully laid the vulnerabilities I needed to find. But also there were some highlights and a couple of non-trivial jumps. She gave me a .com file; no source code. I read the recovered code from the binary using a disassembler and understood it. In the end, when I rebuilt the algorithm, I hacked her program in two ways, presented below.

The work was carried out in the operating system emulator called DOSBox.

### [Crack with buffer overflow](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/Hack%20and%20win/By%20buffer%20overflow)

The first way to hack his program is to overflow the buffer. The program accepts a file with a sequence of some characters as input. I know where the buffer was located for the input data. After that, there was a code that displays two types of text on the screen: "access granted" and "try again". Having overflowed the buffer (there was no check for the maximum allowable size), part of the code in her .com file was rewritten, which displayed a success message.

![image](https://sun9-23.userapi.com/impf/au4zeYuq72cDK_EXKOm6-tEe6V03GHzewFhdpQ/GZj_ezHMzIk.jpg?size=1027x675&quality=96&sign=02ba91459fe519003d9ca7b27e4b4e97&type=album)

### [Crack with patcher](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/Hack%20and%20win/By%20patcher)

The second way to get access with an incorrect password is to mess up part of its code using a third-party program (patcher). Knowing the structure of the enemy's code and having the binary file in front of you, you can easily tell what and in what place needs to be changed in order for everything to work. I did this: I found the beginning of the code showing a success message, copied it into a custom array, overwritten the rest of the code with my easter egg, and added the saved bytes from the array at the end. As a result, now the program emits very crazy and frightening sounds, draws the sun and writes that access has been obtained. Hooray!

![image](https://sun1-15.userapi.com/impf/8laVRGeXEDdzycVzjasZZTThZdktabCimyA54g/sZj3AvB-rMo.jpg?size=1005x779&quality=96&sign=aedabf35c7c163235c6fcd94f09641cd&type=album)

## [Mandelbrot](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/Mandelbrot)

(using SSE!)

## [Alpha blending](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/Cat%20and%20table)

(using AVX + SFML)

## [Hash table](https://github.com/owl1234/MIPT_projects_2_sem/tree/master/Hash%20table)

Read more here: [investigating the possibility of speeding up a hash table](https://github.com/owl1234/MIPT_projects_2_sem/blob/master/Hash%20table/Report.pdf).

