# file: isr_terminal.s

.extern my_counter

#.global terminal_out, terminal_in

.section isr
# prekidna rutina za terminal
#.equ terminal_out, 0xFFFFFF00
#.equ terminal_in, 0xFFFFFF04
#.equ character_offset, 2
.global isr_terminal
isr_terminal:
    push %r1
    push %r2
    ld 0xFFFFFF04, %r1
    ld $2, %r2
    add %r2, %r1
    st %r1, $0xFFFFFF00
    ld my_counter, %r1
    ld $1, %r2
    add %r2, %r1
    st %r1, $my_counter
    pop %r2
    pop %r1
    ret
