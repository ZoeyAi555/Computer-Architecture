        lw      0       9       start 
        lw      9       2       Kemp
        jalr    3       done
        add     4       0       1
Jump    beq     1       1       Loop 
Loop    noop
        add     1       1       0 
        beq     0       0       Jump
        add     1       4       Kemp
Jump    add     1       4       1 //duplicate defined global
        lw      1       2       8
        noop
done    halt
start   .fill   -65
Kemp    .fill   Global
        .fill   -67 
        .fill   King
King    .fill 520

