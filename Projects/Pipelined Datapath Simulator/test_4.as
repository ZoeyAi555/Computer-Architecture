    lw      0       1       one
    lw      0       2       two
    lw      0       3       three
    lw      0       4       four
    lw      0       5       five
    lw      0       6       six
    lw      0       7       seven
    noop
    beq     1       1       label
    lw      2       1       3
    nor     2       3       3
label add   2       4       5
    lw      1       2       3
    sw      1       2       3
    beq     2       2       label1
    halt
label1  noop
    lw      0       1       five1
    lw      0       2       NegOne
    add     1       2       3
    beq     4       0       done
    noop
    noop
done    halt
five1   .fill   5
NegOne  .fill   -1
one        .fill   1
two        .fill   2
three        .fill   3
four        .fill   4
five        .fill   5
six        .fill   6
seven        .fill   7