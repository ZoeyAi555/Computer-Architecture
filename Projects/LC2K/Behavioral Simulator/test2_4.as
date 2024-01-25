        lw      5       1       uninitMem
        lw      1       1       value1
        lw      2       2       Value2
        add     1       2       start
        noop
start   jalr    0       0
        noop
done    halt
value1  .fill   4
Value2  .fill   2
uninitMem .fill -1

