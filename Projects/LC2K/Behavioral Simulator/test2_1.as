        lw      0       1       five1
        lw      0       2       NegOne
        add     1       2       3
        beq     4       0       done
        noop
start   jalr    0       0
        noop
done    halt
five1   .fill   5
NegOne  .fill   -1
