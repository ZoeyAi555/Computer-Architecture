        lw      0       1       five1
        lw      0       2       NegOne //use of defined global label
        add     1       2       3
        beq     4       0       done
        noop
start   jalr    0       0
        noop
        lw      0       1       maxInt
Skip    lw      0       2       One
        add     1       2       3
done    halt
five1   .fill   start
maxInt  .fill   2023
one     .fill   1
NegOne  .fill   -1
Six     .fill 4