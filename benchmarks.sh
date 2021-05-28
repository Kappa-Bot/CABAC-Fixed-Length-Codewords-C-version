#!/bin/bash
cd c; make; cd ../;
cd java; make; cd ../;
cd results; rm *; cd ../;
################################################################################
# Init: parameter sets for performance benchmarking
perf_events="task-clock,context-switches,cycles,instructions,cache-references,cache-misses,branch,branch-misses"

out_ac_c="../../results/ACPerf.result.c"
out_ac_j="../results/ACPerf.result.j"

out_bs_c="../../results/BSPerf.result.c"
out_bs_j="../results/BSPerf.result.j"

out_fc_c="../../results/FCPerf.result.c"
out_fc_j="../results/FCPerf.result.j"

################################################################################
# Performance testing for ArithmeticCoderFLW
cd c/perf

echo -n "ACPerf C 0 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_ac_c.encode ./ACPerf.o 0 5000 10000 1024 >> $out_ac_c.encode
echo "."

echo -n "ACPerf C 1 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_ac_c.decodeN./ACPerf.o 1 500 100000 1024 >> $out_ac_c.decodeN
echo -n "."
perf stat -e $perf_events --append -o $out_ac_c.decodeN ./ACPerf.o 1 10000 5000 1024 >> $out_ac_c.decodeN
echo -n "."
perf stat -e $perf_events --append -o $out_ac_c.decodeN ./ACPerf.o 1 500000 100 1024 >> $out_ac_c.decodeN
echo "."

echo -n "ACPerf C 2 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_ac_c.decodeR ./ACPerf.o 2 100 100000 1 >> $out_ac_c.decodeR
echo -n "."
perf stat -e $perf_events --append -o $out_ac_c.decodeR ./ACPerf.o 2 1000 10000 1 >> $out_ac_c.decodeR
echo -n "."
perf stat -e $perf_events --append -o $out_ac_c.decodeR ./ACPerf.o 2 100000 100 1 >> $out_ac_c.decodeR
echo "."

echo -n "ACPerf C 3 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_ac_c.FPF ./ACPerf.o 3 50000 10000 1024 >> $out_ac_c.FPF
echo "."

echo -n "ACPerf C 4 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_ac_c.FPR ./ACPerf.o 4 50000 10000 1024 >> $out_ac_c.FPR
echo "."

cd ../../
cd java

echo -n "ACPerf J 0 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_ac_j.encode java -Xcomp -ea perf.ACPerf 0 5000 10000 1024 >> $out_ac_j.encode
echo "."

echo -n "ACPerf J 1 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_ac_j.decodeN java -Xcomp -ea perf.ACPerf 1 500 100000 1024 >> $out_ac_j.decodeN
echo -n "."
perf stat -e $perf_events --append -o $out_ac_j.decodeN java -Xcomp -ea perf.ACPerf 1 10000 5000 1024 >> $out_ac_j.decodeN
echo -n "."
perf stat -e $perf_events --append -o $out_ac_j.decodeN java -Xcomp -ea perf.ACPerf 1 500000 100 1024 >> $out_ac_j.decodeN
echo "."

echo -n "ACPerf J 2 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_ac_j.decodeR java -Xcomp -ea perf.ACPerf 2 100 100000 1 >> $out_ac_j.decodeR
echo -n "."
perf stat -e $perf_events --append -o $out_ac_j.decodeR java -Xcomp -ea perf.ACPerf 2 1000 10000 1 >> $out_ac_j.decodeR
echo -n "."
perf stat -e $perf_events --append -o $out_ac_j.decodeR java -Xcomp -ea perf.ACPerf 2 100000 100 1 >> $out_ac_j.decodeR
echo "."

echo -n "ACPerf J 3 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_ac_j.FPF java -Xcomp -ea perf.ACPerf 3 50000 10000 1024 >> $out_ac_j.FPF
echo "."

echo -n "ACPerf J 4 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_ac_j.FPR java -Xcomp -ea perf.ACPerf 4 50000 10000 1024 >> $out_ac_j.FPR
echo "."

cd ../
################################################################################
# Performance testing for ByteStream
cd c/perf

echo -n "BSPerf C 0 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_bs_c.put ./BSPerf.o 0 20000 10000 1024 >> $out_bs_c.put
echo "."

echo -n "BSPerf C 1 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_bs_c.getN ./BSPerf.o 1 20000 10000 1024 >> $out_bs_c.getN
echo "."

echo -n "BSPerf C 2 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_bs_c.getR0 ./BSPerf.o 2 200 1000000 1 >> $out_bs_c.getR0
echo -n "."
perf stat -e $perf_events --append -o $out_bs_c.getR0 ./BSPerf.o 2 10000 20000 1 >> $out_bs_c.getR0
echo -n "."
perf stat -e $perf_events --append -o $out_bs_c.getR0 ./BSPerf.o 2 2000000 100 1 >> $out_bs_c.getR0
echo "."

echo -n "BSPerf C 3 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_bs_c.getR1 ./BSPerf.o 3 200 500000 1 >> $out_bs_c.getR1
echo -n "."
perf stat -e $perf_events --append -o $out_bs_c.getR1 ./BSPerf.o 3 50000 2000 1 >> $out_bs_c.getR1
echo -n "."
perf stat -e $perf_events --append -o $out_bs_c.getR1 ./BSPerf.o 3 200000 500 1 >> $out_bs_c.getR1
echo "."

cd ../../
cd java

echo -n "BSPerf J 0 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_bs_j.put java -Xcomp -ea perf.BSPerf 0 20000 10000 1024 >> $out_bs_j.put
echo "."

echo -n "BSPerf J 1 -> Total: 1 ->"
perf stat -e $perf_events --append -o $out_bs_j.getN java -Xcomp -ea perf.BSPerf 1 20000 10000 1024 >> $out_bs_j.getN
echo "."

echo -n "BSPerf J 2 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_bs_j.getR0 java -Xcomp -ea perf.BSPerf 2 200 10000 1 >> $out_bs_j.getR0
echo -n "."
perf stat -e $perf_events --append -o $out_bs_j.getR0 java -Xcomp -ea perf.BSPerf 2 1000 2000 1 >> $out_bs_j.getR0
echo -n "."
perf stat -e $perf_events --append -o $out_bs_j.getR0 java -Xcomp -ea perf.BSPerf 2 20000 100 1 >> $out_bs_j.getR0
echo "."

cd ../
################################################################################
# Performance testing for FileChannel
cd c/perf

echo -n "FCPerf C 0 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_fc_c.read ./FCPerf.o 0 1000 10000000 >> $out_fc_c.read
echo -n "."
perf stat -e $perf_events --append -o $out_fc_c.read ./FCPerf.o 0 100000 100000 >> $out_fc_c.read
echo -n "."
perf stat -e $perf_events --append -o $out_fc_c.read ./FCPerf.o 0 10000000 1000 >> $out_fc_c.read
echo "."

echo -n "FCPerf C 1 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_fc_c.rb ./FCPerf.o 1 500 1000000 >> $out_fc_c.rb
echo -n "."
perf stat -e $perf_events --append -o $out_fc_c.rb ./FCPerf.o 1 10000 50000 >> $out_fc_c.rb
echo -n "."
perf stat -e $perf_events --append -o $out_fc_c.rb ./FCPerf.o 1 5000000 100 >> $out_fc_c.rb
echo "."

echo -n "FCPerf C 2 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_fc_c.write ./FCPerf.o 2 500 1000000 >> $out_fc_c.write
echo -n "."
perf stat -e $perf_events --append -o $out_fc_c.write ./FCPerf.o 2 10000 50000 >> $out_fc_c.write
echo -n "."
perf stat -e $perf_events --append -o $out_fc_c.write ./FCPerf.o 2 5000000 100 >> $out_fc_c.write
echo "."

echo -n "FCPerf C 3 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_fc_c.tf ./FCPerf.o 3 2000000 1000 >> $out_fc_c.tf
echo -n "."
perf stat -e $perf_events --append -o $out_fc_c.tf ./FCPerf.o 3 2000000 100000 >> $out_fc_c.tf
echo -n "."
perf stat -e $perf_events --append -o $out_fc_c.tf ./FCPerf.o 3 2000000 10000000 >> $out_fc_c.tf
echo "."

cd ../../
cd java

echo -n "FCPerf J 0 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_fc_j.read java -Xcomp -ea perf.FCPerf 0 1000 1000000 >> $out_fc_j.read
echo -n "."
perf stat -e $perf_events --append -o $out_fc_j.read java -Xcomp -ea perf.FCPerf 0 100000 10000 >> $out_fc_j.read
echo -n "."
perf stat -e $perf_events --append -o $out_fc_j.read java -Xcomp -ea perf.FCPerf 0 1000000 1000 >> $out_fc_j.read
echo "."

echo -n "FCPerf J 1 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_fc_j.rb java -Xcomp -ea perf.FCPerf 1 500 1000000 >> $out_fc_j.rb
echo -n "."
perf stat -e $perf_events --append -o $out_fc_j.rb java -Xcomp -ea perf.FCPerf 1 10000 50000 >> $out_fc_j.rb
echo -n "."
perf stat -e $perf_events --append -o $out_fc_j.rb java -Xcomp -ea perf.FCPerf 1 5000000 100 >> $out_fc_j.rb
echo "."

echo -n "FCPerf J 2 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_fc_j.write java -Xcomp -ea perf.FCPerf 2 500 100000 >> $out_fc_j.write
echo -n "."
perf stat -e $perf_events --append -o $out_fc_j.write java -Xcomp -ea perf.FCPerf 2 10000 5000 >> $out_fc_j.write
echo -n "."
perf stat -e $perf_events --append -o $out_fc_j.write java -Xcomp -ea perf.FCPerf 2 500000 100 >> $out_fc_j.write
echo "."

echo -n "FCPerf J 3 -> Total: 3 ->"
perf stat -e $perf_events --append -o $out_fc_j.tf java -Xcomp -ea perf.FCPerf 3 2000000 1000 >> $out_fc_j.tf
echo -n "."
perf stat -e $perf_events --append -o $out_fc_j.tf java -Xcomp -ea perf.FCPerf 3 2000000 100000 >> $out_fc_j.tf
echo -n "."
perf stat -e $perf_events --append -o $out_fc_j.tf java -Xcomp -ea perf.FCPerf 3 2000000 10000000 >> $out_fc_j.tf
echo "."

cd ../
################################################################################
unset arr0
unset arr1
unset arr2
unset arr3
unset arr4
unset arr5
