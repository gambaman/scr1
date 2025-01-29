# Return pseudo-random 16 bit word using lfsr algorithm

# Taps: 15, 14, 12, 3 (https://docs.xilinx.com/v/u/en-US/xapp052)
# Input
#   a0: initial value
# Output
#   a0[15:0]: repeat 16 ( SHL(a0, a[15]^a[14]^a[12]^a[3]) )
#   a0[31:16]: 0

.global prand16
prand16:
    li t0, 16           # shift counter
loop:
    srli t1, a0, 3
    srli t2, a0, 12
    xor t1, t1, t2      # xor bits 3, 12
    srli t2, a0, 14
    xor t1, t1, t2      # xor bit 14
    srli t2, a0, 15
    xor t1, t1, t2      # xor bit 15
    andi t1, t1, 1
    slli a0, a0, 1
    or a0, a0, t1
    addi t0, t0, -1
    bgt t0, x0, loop
    li t0, 0x0000ffff
    and a0, a0, t0
    ret
