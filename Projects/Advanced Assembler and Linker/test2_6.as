comb	lw		0		6		pos1	r6 = 1
		sw		5		2		Stack	save input r on stack
base	beq		0		2		ret1	if (r == 0)	return 1
		beq		1		2		ret1	if (n == r) return 1
		lw		0		6		neg1	r6 = -1
recur	add		1		6		1		n = n - 1
		lw		0		4		Caddr
		jalr	4		7				call the first recursion on (n-1, r)
sum		lw		0		6		neg1	r6 = -1
		lw		5		4		Stack	//use of undefined global label
		lw		5		3		comb	r3 = combination(n-1, r)
		add		3		4		3		r3 = r3 + r4
		beq		0		0		end		go to the end and return
ret1	lw		0		3		pos1	r3 = 1
		beq		0		0		end
end		lw		0		6		neg1	r6 = -1
		jalr	7		4				return
pos1	.fill	1
neg1	.fill	-1
Caddr	.fill	comb
