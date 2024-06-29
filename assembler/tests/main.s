.section code
.equ line_feed, 0x0A000000
.equ carriage_return, 0x0D000000
.equ terminal_out, 0xFFFFFF00
.equ message_len, message_end - message_start
    ld $0, %r2
loop:
    ld $message_start, %r3
    add %r2, %r3
    ld [%r3], %r3
    st %r3, terminal_out
    ld $1, %r1
    add %r1, %r2
    ld $message_len, %r1
    bne %r2, %r1, loop
    ld $line_feed, %r1
    st %r1, terminal_out
    ld $carriage_return, %r1
    st %r1, terminal_out
    halt
message_start:
.ascii "This is a test!\n"
message_end:
.ascii "t"
.end
