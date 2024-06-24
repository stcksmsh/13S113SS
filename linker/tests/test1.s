.extern test
.section my_code_main
    ld $100, %r1
    ld $99, %r2
    bgt %r1, %r2, $one
    jmp $two
one: 
    add %r1, %r2
    jmp $test
two: sub %r1, %r2
    jmp $test
