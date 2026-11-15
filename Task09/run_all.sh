#!/bin/bash

echo "===== TASK 10: PERFORMANCE BENCHMARKING ====="

echo "Compiling programs..."
gcc -Wall -Wextra bench_copy.c -o bench_copy
gcc -Wall -Wextra fork_vs_thread.c -o fork_vs_thread -pthread

echo "Creating 100 MB test file..."
dd if=/dev/zero of=big.dat bs=1M count=100 status=none

echo ""
echo "===== SYSTEM CALL COMPARISON ====="

if command -v strace >/dev/null 2>&1; then
    for b in 64 512 4096 65536 1048576
    do
        echo ""
        echo "Buffer size: $b bytes"
        strace -c -e trace=read,write ./bench_copy rw big.dat out.dat $b
    done
else
    echo "strace is not installed."
fi

echo ""
echo "===== TIME COMPARISON ====="

for b in 64 512 4096 65536 1048576
do
    echo ""
    echo "read/write buffer: $b bytes"
    time ./bench_copy rw big.dat out.dat $b
done

echo ""
echo "===== STDIO COMPARISON ====="
time ./bench_copy stdio big.dat out.dat 4096

echo ""
echo "===== MMAP COMPARISON ====="
time ./bench_copy mmap big.dat out.dat 4096

echo ""
echo "===== PROCESS VS THREAD ====="
./fork_vs_thread

echo ""
echo "===== TASK 10 COMPLETED ====="
