
.global _start

_start:
  div %rbx    # divide by the number-base
  idiv %rbx    # divide by the number-base
  divl (%rbx)    # divide by the number-base
  idivl (%rbx)    # divide by the number-base
  



  mov $-2345, %rax
  lea _output, %rdi
  call rax2uint

  mov $1, %rax
  mov $1, %rdi
  lea _output, %rsi
  mov $20, %rdx
  syscall
  mov $60, %rax
  mov $37, %rdi
  syscall
.byte 0xde
.byte 0xad
.byte 0xbe
.byte 0xef
.byte 'H'
.byte 'i'

  mov _output, %rdi
  lea _output, %rdi
  mov $0, %rax
  #lea %rax, %rax
  lea (%rcx), %rax
  lea (%rcx), %rbx
  lea (%rax), %rax
  lea (%rax), %rbx
  lea (%rbx), %rdx
  
  mov $0x1111111111111111, %rax
  mov $0x1111111111111111, %rcx
  lea    8(,%rax,4), %rax  
  
  mov _output, %rax
  lea _output, %rax
  mov _output, %rcx
  lea _output, %rcx
  mov _output, %rdx
  lea _output, %rdx
  mov _output, %rbx
  lea _output, %rbx
#.char 'R'


.data
_output:
  .space 30

.text
rax2uint:
  #
  push  %rax    # save the caller's registers
  push  %rbx
  push  %rcx
  push  %rdx
  push  %rdi

  mov $10, %rbx # base of the decimal system
  mov $0, %rcx  # number of digits generated

#  cmp

nxdiv:
  mov $0, %rdx  # RAX extended to (RDX,RAX)
  div %rbx    # divide by the number-base
  push  %rdx    # save remainder on the stack
  inc %rcx    # and count this remainder
  cmp $0, %rax  # was the quotient zero?
  jne nxdiv   # no, do another division
nxdgt:
  pop %rdx    # else pop recent remainder
  add $'0', %dl # and convert to a numeral
  mov %dl, (%rdi) # store to memory-buffer
  inc %rdi    # and advance buffer-pointer
  loop  nxdgt   # again for other remainders

  pop %rdi    # recover saved registers
  pop %rdx
  pop %rcx
  pop %rbx
  pop %rax
  ret     # and return to the caller

  .global rax2uint  # make entry-point visible
  .end      # nothing else to assemble


