        lw      0       1       ArrayS
        lw      0       3       Fcall
        jalr    3       7
        jalr    0       0       sub
        beq     1       1       sub
        noop
sub     add     1       2       3
        jalr    0       0       End
        noop
end      halt
Fcall   .fill   Find
ArrayS  .fill   Array
Array   .fill   0
        .fill   1
        .fill   1111
