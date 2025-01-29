# Test offset filter functions

.equ EP, 4   # Number of elements = 2^EP
.equ SEED, 0x1234

.lcomm list, 2 * 1<<EP

.text

.global main
main:
    # Calculate end list address
    la s1, list         # data pointer
    li s2, 1
    li t1, EP           # calculate end data address
    sll s2, s2, t1      # s2 = s1 + 2 * 2^t1
    slli s2, s2, 1
    add s2, s2, s1

    # Fill-in the list
    li a0, SEED
loop:
    addi sp, sp, -8
    sw ra, 0(sp)
    jal prand16
    lw ra, 0(sp)
    addi sp, sp, 8
    sh a0, 0(s1)
    addi s1, s1, 2
    blt s1, s2, loop

    # Call offset filter
    la a0, list
    li a1, EP
    addi sp, sp, -8
    sw ra, 0(sp)
    jal offset_filter
    lw ra, 0(sp)
    addi sp, sp, 8
    ret
