        sw      0   3   size
        lw      0   6   1
        sw      0   4   Real
        sw      0   4   Real
        nor     3   6   3
        lw      2   1   Real//duplicate use of undefined global labels
Clear    jalr    6   1
        halt
size    .fill   55
Neg1    .fill   -1

