.global test, dat
.extern cnt
.section testing
test: ld $14, %r3
    ld $100, %r4
    mul %r3, %r4
    ld dat, %r5
    ld $20, %r7
loop:
    ld cnt, %r1
    bne %r1, %r7, $loop
end: halt
dat: .ascii "Hello, World!\n"
