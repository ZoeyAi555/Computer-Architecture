    lw      0       1       one
    lw      0       2       two
    lw      0       3       three
    lw      0       4       four
    lw      0       5       five
    lw      0       6       six
    lw      0       7       seven
    lw  1   2   22
    add 1    2   3
    nor 3   2   1   
    sw  2   1   0
    lw  3   4   18
    beq 1   2   thi
    beq 4   2   the
thi  lw  4   5   three
    add 5   5   3
the    halt
one        .fill   1
two        .fill   2
three        .fill   3
four        .fill   4
five        .fill   5
six        .fill   6
seven        .fill   7
