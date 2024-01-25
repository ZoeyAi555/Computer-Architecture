        lw      0       9       start1
        lw      9       2       Kemp
        jalr    3       done
        add     4       0       1
Jump    beq     1       1       Loop
Loop    noop
        add     start       -1       0
        beq     0       0       Jump
        add     1       4       Kemp
Jump    add     1       4       1
        lw      1       2       865667899
        bor     0       4       1
        nor     7       -1
        noop
done    halt
start   .fill   -65
Kemp    .fill   Global
start   .fill   -67
