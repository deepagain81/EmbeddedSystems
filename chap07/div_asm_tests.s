; .. Copyright (c) 2013 Bryan A. Jones, Robert B. Reese, and J. W. Bruce ("AUTHORS")
;    All rights reserved.
;    (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
;    (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
;    (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
;
;    Permission to use, copy, modify, and distribute this software and its documentation for any purpose, without fee, and without written agreement is hereby granted, provided that the above copyright notice, the following two paragraphs and the authors appear in all copies of this software.
;
;    IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS" HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;    THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
;
;    Please maintain this header in its entirety when copying/modifying these files.
;
; ***************
; div_asm_tests.s
; ***************

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code.


         .data            ;unitialized data section

         .bss
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
u16_k:   .space 2        ;Allocating space (in bytes) to variable.
u8_j:    .space 1        ;Allocating space (in bytes) to variable.

.text                          ;Start of Code section
__reset:                       ; first instruction located at __reset label
       mov #__SP_init, w15     ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0
       mov W0, SPLIM           ;Initialize the stack limit register
       rcall main              ;call main()
       reset                   ;start over
 ;__SP_init set by linker to be after allocated data

main:
      mov #0x9EF0,W2
      mov #0x018D,W4
      repeat #17
      div.u W2,W4

      mov #0x9A00,W2  ; 39424 unsigned, -26112 as signed
      mov #0x6600,W3  ; 26112 unsigned, +26112 as unsigned
      mov #105,W4     ; 105 unsigned, +105 signed
      mov #0xFF97,W5  ; 65431 unsigned, -105 signed


      repeat #17
      div.u W2,W4     ;39424/105 =375=W0, r=49=W1
      repeat #17
      div.s W2,W4     ;-26112/+105 = -248=W0, r= -72=W1
      repeat #17
      div.s W3,W5     ;+26112/-105 = -248=W0, r= +72=W1
      repeat #17
      div.s W2,W5     ;-26112/-105 = +248=W0, r= -72=W1

      mov #0x0,W6
      mov #0x0001,W7  ;W7:W6 = 0x00010000= 65536 doubleword
      mov #0x01,W8    ;W8=1
      repeat #17
      div.ud W6,W8    ;65536/1 is overflow as quotient > 16-bits

      mov  #0x8000,W2 ;-32768 signed
      mov  #0xFFFF,W3 ; -1 signed
      repeat #17
      div.s W2,W3     ;-32768/-1 is overflow since +32768 > 16-bits

done:                     ;do not return
       bra done






    .end
