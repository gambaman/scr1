# Interface to rdcycle, rdtime and rdinstret instructions.
# (Least-significant 32 bits only)

.global read_cycle
read_cycle:
    rdcycle a0
    ret

.global read_time
read_time:
    rdtime a0
    ret

.global read_instret
read_instret:
    rdinstret a0
    ret
    