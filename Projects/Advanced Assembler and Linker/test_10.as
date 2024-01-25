        sw      0   3   size
        sw      0   6   -2
        sw      0   4   Real
        nor     3   6   3
        jalr    6   1
        halt
size    .fill   can//use of undefined local labels
neg1    .fill   -1
real    .fill  real