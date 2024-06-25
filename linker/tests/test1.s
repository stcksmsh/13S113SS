.equ TERM_OUT,  0xFFFFFF00
.equ TERM_IN ,  0xFFFFFF04
.equ TIMER_CFG, 0xFFFFFF10

.extern test
.section my_code_main
    ld $0x5, %r1
    st %r1, $TIMER_CFG
    ld $0xFFFFFEFE, %sp
    ld $handler, %r1
    csrwr %r1, %handler
    int
    ld $100, %r1
    ld $99, %r2
    bgt %r1, %r2, $one
    jmp $two
one: 
    add %r1, %r2
    jmp $test
two: sub %r1, %r2
    jmp $test
.section my_handler
handler:
    push %r1
    push %r2
    ld $3, %r1
    csrrd %cause, %r2
    beq %r1, %r2, $term_handler
    ld $2, %r1
    beq %r1, %r2, $time_handler
    jmp $end_handler
time_handler:
    ld $0x54000000, %r1
    st %r1, $TERM_OUT
    jmp $end_handler    
term_handler:    
    ld TERM_IN, %r1
    st %r1, $TERM_OUT
    jmp $end_handler
end_handler:
    pop %r2
    pop %r1
    iret
