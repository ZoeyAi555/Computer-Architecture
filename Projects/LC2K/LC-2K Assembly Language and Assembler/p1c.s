lw    0    1    n
lw    0    2    r
lw    0    4    Caddr    load combination function address
jalr    4    7        call function
halt
start    beq    2    0    if1    r == 0
beq    1    2    if1    r == n
lw    0    6    pos1    R6=1
sw    5    7    Stack    save return address on stack
add    5    6    5    increment stack pointer
sw    5    1    Stack    save input n on stack
add    5    6    5    increment stack pointer
sw    5    2    Stack    save input r on stack
add    5    6    5    increment stack pointer
lw    0    6    neg1    R6=-1
add    1    6    1    n = n-1
jalr    4    7        call n-1
lw    0    6    pos1
sw    5    3    Stack    save return value on stack
add    5    6    5    increment stack pointer
lw    0    6    neg1
add    1    6    1    n = n-1
add    2    6    2    r = r-1
jalr    4    7        call n-1, r-1
lw    0    6    pos1
sw    5    3    Stack    save return value on stack
add    5    6    5    increment stack pointer
lw    5    4    Stack    R4 = R3(n-1,r)
lw    0    6    neg1    R6 = -1
add    5    6    5    decrement address on stack
add    3    4    3    R3 = R3(n-1,r-1) + R4
lw    0    6    neg1
add    5    6    5    decrement address on stack
lw    5    2    Stack    recover input r from stack
add    5    6    5    decrement address on stack
lw    5    1    Stack    recover input n from stack
add    5    6    5    decrement address on stack
lw    5    7    Stack    recover return address from stack
jalr    7    4        return
if1    lw    0    3    pos1    R3 = 1
lw    0    6    neg1
add    5    6    5    decrement address on stack
lw    5    2    Stack    recover input r from stack
add    5    6    5    decrement address on stack
lw    5    1    Stack    recover input n from stack
add    5    6    5    decrement address on stack
lw    5    7    Stack    recover return address from stack
jalr    7    4        return
n    .fill    7
r    .fill    3
Caddr    .fill    start
pos1    .fill    1
neg1    .fill    -1
Stack    .fill    0

