# Return pseudo-random 32 bit word using lfsr algorithm

# Taps: 31, 21, 1, 0 (https://docs.xilinx.com/v/u/en-US/xapp052)
# Input
#   a0: initial value
# Output
#   a0: repeat 32 ( SHL(a0, a[31]^a[21]^a[1]^a[0]) )

.global prand32
prand32:
    li t0, 32           # shift counter
loop:
    srli t2, a0, 1
    xor t1, t2, a0      # xor bits 0, 1
    srli t2, a0, 21
    xor t1, t1, t2      # xor bit 21
    srli t2, a0, 31
    xor t1, t1, t2      # xor bit 31
    andi t1, t1, 1
    slli a0, a0, 1
    or a0, a0, t1
    addi t0, t0, -1
    bgt t0, x0, loop
    ret
