    lw      0       1       one
    lw      0       2       two
    lw      0       3       three
    lw      0       4       four
    lw      0       5       five
    lw      0       6       six
    lw      0       7       seven
    add  1   2   3
    lw 1    2   three
    nor 3   2   1   
    sw  2   3   0
    nor  3   4   1
    beq 4   2   the
    beq 1   2   thx
the    nor  3   4   5
    add 5   5   3
thx    halt
one        .fill   1
two        .fill   2
three        .fill   3
four        .fill   4
five        .fill   5
six        .fill   6
seven        .fill   7
