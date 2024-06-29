.extern isr_timer, isr_terminal

.global handler
.section my_handler
.extern terminal_out, line_feed, carriage_return
handler:
    push %r1
    push %r2
    csrrd %cause, %r1
    ld $2, %r2
    beq %r1, %r2, handle_timer
    ld $3, %r2
    beq %r1, %r2, handle_terminal
    ld $0x45000000, %r2
    st %r2, terminal_out
    ld $0x52000000, %r2
    st %r2, terminal_out
    ld $0x52000000, %r2
    st %r2, terminal_out
    ld $0x4F000000, %r2
    st %r2, terminal_out
    ld $0x52000000, %r2
    st %r2, terminal_out
    ld $line_feed, %r1
    st %r1, terminal_out
    ld $carriage_return, %r1
    st %r1, terminal_out
finish:
    pop %r2
    pop %r1
    iret
# obrada prekida od tajmera
handle_timer:
    call isr_timer
    jmp finish
# obrada prekida od terminala
handle_terminal:
    call isr_terminal
    jmp finish
    
.end
