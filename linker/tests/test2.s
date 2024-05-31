.global handler
.section test2
handler: .skip 0x10
MSG3:    .ascii "Hello, World!\n"
.section my_code_main
    ld $handler, %r1
