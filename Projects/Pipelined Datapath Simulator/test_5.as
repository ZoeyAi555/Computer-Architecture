    lw      0       1       one
    lw      0       2       two
    lw      0       3       three
    lw      0       4       four
    lw      0       5       five
    lw      0       6       six
    lw      0       7       seven
    lw      0       1       seven
    sw      0       1       five
    lw      0       3       seven
    add     1       2       4
    sw      0       3       six
    noop 
    add     1       3       2
    lw      0       3       three
    add     1       2       4
    noop
    sw      0       3       four
    lw      0       3       seven
    add     1       2       3
    sw      0       3       one
    add     1       3       1
done    halt
one        .fill   1
two        .fill   2
three        .fill   3
four        .fill   4
five        .fill   5
six        .fill   6
seven        .fill   7