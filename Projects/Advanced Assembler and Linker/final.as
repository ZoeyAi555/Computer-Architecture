Ker	halt
	lw	0	2	Num
	lw	0	2	Num
	lw	0	2	Num1
	lw	0	2	Num2
	lw	0	3	Fcall
	sw	1	4	skip
	jalr	3	7
	add	5	6	5
Find	lw	0	6	NegOne
Top	add	2	6	2
	beq	3	0	skip
skip	beq	0	2	Done
	beq	0	0	Top
Done	jalr	7	3
		.fill skip
NegOne	.fill	-1
Fcall	.fill	Find
Num	.fill	5
Kid	.fill	1111
