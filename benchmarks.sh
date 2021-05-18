#!/bin/bash
cd c; make; cd ../;
cd java; make; cd ../;
################################################################################
# Init: parameter sets for performance benchmarking for 1G/5M/1M operations
perf_events="task-clock,context-switches,cycles,instructions,cache-references,cache-misses,branch,branch-misses"
out_ac_c="../../results/ACPerf.result"
out_ac_j="../results/ACPerf.result"

################################################################################
# Performance testing for ArithmeticCoderFLW
echo "" > results/ACPerf.result
cd c/perf

perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 0 5000 10000 1024 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 1 500 100000 1024 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 1 10000 5000 1024 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 1 500000 100 1024 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 2 100 100000 1024 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 2 1000 10000 1024 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 2 100000 100 1024 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 3 50000 10000 1024 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./ACPerf.o 4 50000 10000 1024 > $out_ac_c

cd ../../
cd java

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 0 5000 10000 1024 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 1 500 100000 1024 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 1 10000 5000 1024 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 1 500000 100 1024 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 2 100 100000 1024 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 2 1000 10000 1024 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 2 100000 100 1024 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 3 50000 10000 1024 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.ACPerf 4 50000 10000 1024 > $out_ac_j

cd ../
################################################################################
# Performance testing for ByteStream
echo "" > results/BSPerf.result
cd c/perf

perf stat -e $perf_events --append -o $out_ac_c ./BSPerf.o 0 20000 10000 1024 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./BSPerf.o 1 20000 10000 1024 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./BSPerf.o 2 200 1000000 1 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./BSPerf.o 2 10000 20000 1 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./BSPerf.o 2 2000000 100 1 > $out_ac_c

cd ../../
cd java

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 0 20000 10000 1024 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 1 20000 10000 1024 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 2 200 1000000 1 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 2 10000 20000 1 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 2 2000000 100 1 > $out_ac_j

cd ../
################################################################################
# Performance testing for FileChannel
echo "" > results/FCPerf.result
cd c/perf

perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 0 1000 10000000 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 0 100000 100000 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 0 10000000 1000 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 1 500 1000000 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 1 10000 50000 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 1 5000000 100 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 2 500 1000000 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 2 10000 50000 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 2 5000000 100 > $out_ac_c

perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 3 2000000 1000 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 3 2000000 100000 > $out_ac_c
perf stat -e $perf_events --append -o $out_ac_c ./FCPerf.o 3 2000000 10000000 > $out_ac_c

cd ../../
cd java

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 0 1000 10000000 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 0 100000 100000 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 0 10000000 1000 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 1 500 1000000 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 1 10000 50000 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 1 5000000 100 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 2 500 1000000 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 2 10000 50000 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 2 5000000 100 > $out_ac_j

perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 3 2000000 1000 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 3 2000000 100000 > $out_ac_j
perf stat -e $perf_events --append -o $out_ac_j java -Xcomp -ea perf.BSPerf 3 2000000 10000000 > $out_ac_j

cd ../
################################################################################
unset arr0
unset arr1
unset arr2
unset arr3
unset arr4
unset arr5
