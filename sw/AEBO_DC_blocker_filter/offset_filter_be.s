# offset_filter_be
#
# Subtract the mean to a list or 2^a1 16-bit integers in memory
# (Big-endian version)
#
# Parameters:
#   a0: list address
#   a1: number of elements power (elements = 2^a1)
# Return:
#   a0: offset

.global offset_filter_be
offset_filter_be:
    mv t0, a0           # data pointer
    li t1, 1            # calculate end data address
    sll t1, t1, a1      # t1 = t0 + 2 * 2^a1
    slli t1, t1, 1
    add t1, t1, t0
    mv t2, x0           # initialize sum t2

    # Add all numbers in the list and store them to native endian format
add_loop_be:
    lhu t3, 0(t0)

    slli t4, t3, 24      # swap bytes 0 and 1 in t3
    srai t4, t4, 16      # and extend sign (sra)
    srli t3, t3, 8
    or t3, t3, t4
    
    add t2, t2, t3      # accumulate and store in native endianness
    sh t3, 0(t0)
    addi t0, t0, 2
    bne t0, t1, add_loop_be

    sra t2, t2, a1      # calculate offset t2 = t2 / 2^a1
    mv t0, a0           # restart memory pointer

    # Subtract offset to all numbers in the list and convert them back to BE
filter_loop_be:
    lh t3, 0(t0)
    sub t3, t3, t2

    # swap bytes 0 and 1 in t3
    slli t4, t3, 8
    slli t3, t3, 16
    srli t3, t3, 24
    or t3, t3, t4

    sh t3, 0(t0)
    addi t0, t0, 2
    bne t0, t1, filter_loop_be

    # Finish
    mv a0, t2          
    ret
