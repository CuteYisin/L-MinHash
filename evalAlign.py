#!/usr/bin/env -S python3 -W ignore

import sys
import numpy as np
from numba import jit


qdbfn = sys.argv[1]
tdbfn = sys.argv[2]
lstfn = sys.argv[3]



@jit
def align(qseq, tseq):
    qlen, tlen = len(qseq), len(tseq)
    a01 = np.zeros([qlen+1, tlen+1], dtype=np.int32)
    a10 = np.zeros([qlen+1, tlen+1], dtype=np.int32)
    a11 = np.zeros([qlen+1, tlen+1], dtype=np.int32)
    for i in range(1, qlen+1):
        for j in range(1, tlen+1):
            if qseq[i-1] == tseq[j-1]:
                a11[i, j] = max(a11[i-1, j-1] + 1, a01[i-1, j-1] + 1, a10[i-1, j-1] + 1, 0)
            else:
                a11[i, j] = max(a11[i-1, j-1] - 1, a01[i-1, j-1] - 1, a10[i-1, j-1] - 1, 0)
            a01[i, j] = max(a01[i, j-1] - 1, a11[i, j-1] - 4, 0)
            a10[i, j] = max(a10[i-1, j] - 1, a11[i-1, j] - 4, 0)
    return np.max(a11) / qlen

qdb = {}
with open(qdbfn) as f:
    header, sequence = None, None
    for l in f.readlines():
        l = l.strip()
        if l[0] == '>':
            if header is not None:
                qdb[header] = sequence
            header, sequence = l[1:], ''
        else:
            sequence += l

tdb = {}
with open(tdbfn) as f:
    header, sequence = None, None
    for l in f.readlines():
        l = l.strip()
        if l[0] == '>':
            if header is not None:
                tdb[header] = sequence
            header, sequence = l[1:], ''
        else:
            sequence += l

score = 0
with open(lstfn) as f:
    for l in f.readlines():
        qid, tid = l.strip().split()
        s = 1 if align(qdb[qid], tdb[tid]) > 0.5 else -1
        print(qid, tid, s)
        score += s
print('#score:', score)

