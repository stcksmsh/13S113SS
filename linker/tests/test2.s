.global test, dat
.section testing
test: ld $14, %r3
    ld $100, %r4
    mul %r3, %r4
    ld dat, %r5
loop:
    jmp $loop
end: halt
dat: .ascii "Hello, World!\n"
