<<<<<<< HEAD:test2.as
; file ps.as
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
macro m1
 inc r6
 mov r3, W
endm
 lea STR, r6
 m1
 sub r1, r4
 bne END
 cmp val1, #-6
 bne END[r15]
 dec K
.entry MAIN
 sub LOOP[r10] ,r14
END: stop
STR: .string “abcd”
LIST: .data 6, -9
 .data -100
.entry K
K: .data 31
.extern val1
=======
; file ps.as 
.entry LIST 
.extern W 
MAIN: add r3, LIST 
LOOP: prn #48 
lea STR, r6 
inc r6 
mov r3, W 
sub r1, r4 
bne END 
cmp val1, #-6 
bne END[r15] 
dec K 
.entry MAIN 
sub LOOP[r10] ,r14 
END: stop 
STR: .string 
LIST: .data 6, -9 
.data -100 
.entry K 
K: .data 31 
.extern val1 
.extern val1 
>>>>>>> f7375110181730651ad2cfa6d0b1b039311907c2:test1.am