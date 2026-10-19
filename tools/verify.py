#!/usr/bin/env python3
"""Check the grader's output/correctness questions against a Python oracle."""
import re
import subprocess
import sys
from pathlib import Path

binary = str(Path(sys.argv[1] if len(sys.argv) > 1 else './mt-collatz').resolve())


def run(n, t, unlocked=False):
    args = [binary, str(n), str(t)] + (['-nolock'] if unlocked else [])
    result = subprocess.run(args, capture_output=True, text=True, timeout=90, check=True)
    rows = [line.split(',') for line in result.stdout.splitlines()]
    assert len(rows) == 1001
    assert [int(row[0]) for row in rows] == list(range(1001))
    assert re.fullmatch(rf'{n},{t},\d+\.\d{{9}}\n', result.stderr), result.stderr
    return [int(row[1]) for row in rows]


def reference(n):
    histogram = [0] * 1001
    for value in range(1, n + 1):
        steps = 0
        while value != 1:
            value = value // 2 if value % 2 == 0 else 3 * value + 1
            steps += 1
        histogram[steps] += 1
    return histogram


cases = 0
for n in (1, 2, 10, 27, 1000, 100000):
    expected = reference(n)
    for t in (1, 2, 4, 8):
        for repeat in range(3):
            assert run(n, t) == expected, (n, t, repeat)
            cases += 1
    assert run(n, 1, True) == expected
    cases += 1
print(f'PASS: {cases} histograms match independent Python reference; CSV streams valid.')
expected = reference(100000)
totals = []
mismatches = 0
for t in (4, 8):
    for repeat in range(5):
        actual = run(100000, t, True)
        mismatches += actual != expected
        totals.append(sum(actual))
print(f'OBSERVED: {mismatches}/10 unlocked histograms differ; totals {min(totals)}..{max(totals)}.')
# Races are intentional undefined behavior; their manifestation is not a test assertion.
for args in ([], ['1'], ['0', '1'], ['-1', '1'], ['1', '0'], ['1', '-1'],
             ['abc', '2'], ['2x', '1'], ['1', '2x'], ['1', '1', '-bad'],
             ['18446744073709551616', '1'], ['1', '2147483648'], ['2147483648', '1']):
    result = subprocess.run([binary] + args, capture_output=True, text=True, timeout=5)
    assert result.returncode != 0 and result.stderr and not result.stdout, args
print('PASS: 13 invalid invocations fail cleanly without histogram output.')
