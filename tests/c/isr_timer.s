# file: isr_timer.s

#.extern terminal_out

.section isr
# prekidna rutina za tajmer
#.equ line_feed, 0xA
#.equ carriage_return, 0xD
#.equ message_len, $message_end - $message_start
.equ message_len, 0x10
.global isr_timer
isr_timer:
    push %r1
    push %r2
    push %r3
    ld $0, %r2
loop:
    ld $message_start, %r3
    add %r2, %r3
    ld [%r3], %r3
    st %r3, $0xFFFFFF00
    ld $1, %r1
    add %r1, %r2
    ld $message_len, %r1
    bne %r1, %r2, $loop
    ld $0xA, %r1
    st %r1, $0xFFFFFF00
    ld $0xD, %r1
    st %r1, $0xFFFFFF00
    pop %r3
    pop %r2
    pop %r1
    ret
message_start:
.ascii "timer interrupt\n"

.end
