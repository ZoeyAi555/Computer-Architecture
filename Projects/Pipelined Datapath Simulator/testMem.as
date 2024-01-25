    lw	0	1	data1	$1= mem[data1]
    lw  0   2   data2
    lw  0   3   data3
    lw  0   4   data4
    lw  0   5   data5
    add 1   2   3   check is my data1 ==regA, data2==regB
    halt
data1	.fill	12345
data2	.fill	2645
data3	.fill	345
data4	.fill	124
data5	.fill	234