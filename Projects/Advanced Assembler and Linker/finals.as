First	lw	    0	    1	    five	
	    lw	    1	    2	    start	
        lw	    1	    2	    start
start	add	    1	    2	    1
	    beq	    0	    0	    start
        sw	    1	    2	    Start
        lw      3       2       First
        noop
        beq     1       3       4	
star	.fill	start
five	.fill	5
neg1	.fill	-1
first   .fill   520	
        .fill   Global
Start   .fill   23
        .fill  Start