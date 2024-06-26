# file: main.s

.global handler, my_start, my_counter

.section my_code
my_start:
    ld $0xFFFFFEFE, %sp
    ld $handler, %r1
    csrwr %r1, %handler
    
    ld $0x1, %r1
    st %r1, 0xFFFFFF10 # tim_cfg
wait:
    ld my_counter, %r1
    ld my_counter, %r3
    ld $0x400000, %r4
    add %r4, %r3
    st %r3, $0xFFFFFF00
    ld $20, %r2
    bne %r1, %r2, $wait
    halt

.section my_data
my_counter:
.word 0

.end
