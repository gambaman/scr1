# offset_filter
#
# Subtract the mean to a list or 2^a1 16-bit integers in memory
#
# Parameters:
#   a0: list address
#   a1: number of elements power (elements = 2^a1)
# Return:
#   a0: offset

.global offset_filter
offset_filter:
    mv t0, a0                   # data pointer
    li t1, 1                    # calculate end data address
    sll t1, t1, a1              # t1 = t0 + 2 * 2^a1
    slli t1, t1, 1
    add t1, t1, t0
    mv t2, x0                   # initialize sum t2

    # Add all numbers in the list
add_loop:
    lh t3, 0(t0)
    add t2, t2, t3
    addi t0, t0, 2
    bne t0, t1, add_loop

    sra t2, t2, a1              # calculate offset t2 = t2 / 2^a1
    mv t0, a0                   # restart memory pointer

    # Subtract offset to all numbers in the list
filter_loop:
    lh t3, 0(t0)
    sub t3, t3, t2
    sh t3, 0(t0)
    addi t0, t0, 2
    bne t0, t1, filter_loop

    # Finish
    mv a0, t2          
    ret
