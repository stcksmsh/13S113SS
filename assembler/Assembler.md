# Instructions

## Instruction syntax

| Instruction | Syntax                        |
| ----------- | ----------------------------- |
| HALT        | `halt`                      |
| INT         | `int`                       |
| IRET        | `iret`                      |
| CALL        | `call operand`              |
| RET         | `ret`                       |
| JMP         | `jmp operand`               |
| BEQ         | `beq %gpr1, %gpr2, operand` |
| BNE         | `bne %gpr1, %gpr2, operand` |
| BGT         | `bgt %gpr1, %gpr2, operand` |
| PUSH        | `push %gpr`                 |
| POP         | `pop %gpr`                  |
| XCHG        | `xchg %gprS, %gprD`         |
| ADD         | `add %gprS, %gprD`          |
| SUB         | `sub %gprS, %gprD`          |
| MUL         | `mul %gprS, %gprD`          |
| DIV         | `div %gprS, %gprD`          |
| NOT         | `not %gpr`                  |
| AND         | `and %gprS, %gprD`          |
| OR          | `or %gprS, %gprD`           |
| XOR         | `xor %gprS, %gprD`          |
| SHL         | `shl %gprS, %gprD`          |
| SHR         | `shr %gprS, %gprD`          |
| LD          | `ld operand, %gpr`          |
| ST          | `st %gpr, operand`          |
| CSRRD       | `csrrd %csr, %gpr`          |
| CSRWR       | `csrwr %gpr, %csr`          |

## Instruction format

| 31 .. 27 | 26 .. 24 | 23 .. 20   | 19 .. 16   | 15 .. 12   | 11 .. 0      |
| -------- | -------- | ---------- | ---------- | ---------- | ------------ |
| Opcode   | Modifier | Register A | Register B | Register C | Displacement |

### Opcodes

| Instruction | Opcode  |
| ----------- | ------- |
| HALT        | 0b00000 |
| INT         | 0b00001 |
| IRET        | 0b00010 |
| CALL        | 0b00011 |
| RET         | 0b00100 |
| JMP         | 0b00101 |
| BEQ         | 0b00110 |
| BNE         | 0b00111 |
| BGT         | 0b01000 |
| PUSH        | 0b01001 |
| POP         | 0b01010 |
| XCHG        | 0b01011 |
| ADD         | 0b01100 |
| SUB         | 0b01101 |
| MUL         | 0b01110 |
| DIV         | 0b01111 |
| NOT         | 0b10000 |
| AND         | 0b10001 |
| OR          | 0b10010 |
| XOR         | 0b10011 |
| SHL         | 0b10100 |
| SHR         | 0b10101 |
| LD          | 0b10110 |
| ST          | 0b10111 |
| CSRRD       | 0b11000 |
| CSRWR       | 0b11001 |

### Modifiers

The modifiers specify how the `operand` is represented:

| Syntax                   | Modifier | Value                                                |
| ------------------------ | -------- | ---------------------------------------------------- |
| `$<literal>`           | 0b000    | The value of `<literal>`                           |
| `$<symbol>`            | 0b001    | The value of `<symbol>`                            |
| `<literal>`            | 0b010    | The value in memory at address `<literal>`         |
| `<symbol>`             | 0b011    | The value in memory at address `<symbol>`          |
| `%<reg>`               | 0b100    | The value in register `<reg>`                      |
| `[%<reg>]`             | 0b101    | The value in memory at address `<reg>`             |
| `[%<reg> + <literal>]` | 0b110    | The value in memory at address `<reg> + <literal>` |
| `[%<reg> + <symbol>]`  | 0b111    | The value in memory at address `<reg> + <symbol>`  |

The register number (`<reg>`) from the `operand` is stored in `RegC`.
