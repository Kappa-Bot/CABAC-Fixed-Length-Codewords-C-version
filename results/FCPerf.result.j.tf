Performance test for fcTransferFrom() with:
	Repetitions: 2000000
	Operations:  1000
Execution time in milliseconds: 17136
Finish
# started on Mon May 24 16:51:29 2021


 Performance counter stats for 'java -Xcomp -ea perf.FCPerf 3 2000000 1000':

          22431.14 msec task-clock                #    1.004 CPUs utilized          
             10413      context-switches          #    0.464 K/sec                  
       24897208562      cycles                    #    1.110 GHz                    
       19275689475      instructions              #    0.77  insn per cycle         
        8625158749      cache-references          #  384.517 M/sec                  
         144246363      cache-misses              #    1.672 % of all cache refs    
        4333689498      branch                    #  193.200 M/sec                  
         142928655      branch-misses                                               

      22.350120279 seconds time elapsed

      16.192451000 seconds user
       6.422254000 seconds sys


Performance test for fcTransferFrom() with:
	Repetitions: 2000000
	Operations:  100000
Execution time in milliseconds: 17054
Finish
# started on Mon May 24 16:51:51 2021


 Performance counter stats for 'java -Xcomp -ea perf.FCPerf 3 2000000 100000':

          22395.77 msec task-clock                #    1.003 CPUs utilized          
             10342      context-switches          #    0.462 K/sec                  
       24549770189      cycles                    #    1.096 GHz                    
       19776322603      instructions              #    0.81  insn per cycle         
        9102179547      cache-references          #  406.424 M/sec                  
         148476118      cache-misses              #    1.631 % of all cache refs    
        4464336255      branch                    #  199.338 M/sec                  
         145250597      branch-misses                                               

      22.318647030 seconds time elapsed

      16.631741000 seconds user
       5.948255000 seconds sys


Performance test for fcTransferFrom() with:
	Repetitions: 2000000
	Operations:  10000000
Execution time in milliseconds: 16770
Finish
# started on Mon May 24 16:52:13 2021


 Performance counter stats for 'java -Xcomp -ea perf.FCPerf 3 2000000 10000000':

          22001.39 msec task-clock                #    1.004 CPUs utilized          
              9852      context-switches          #    0.448 K/sec                  
       24440654728      cycles                    #    1.111 GHz                    
       19489871233      instructions              #    0.80  insn per cycle         
        8851062684      cache-references          #  402.296 M/sec                  
         155066716      cache-misses              #    1.752 % of all cache refs    
        4342573543      branch                    #  197.377 M/sec                  
         160561152      branch-misses                                               

      21.922313851 seconds time elapsed

      16.312442000 seconds user
       5.865418000 seconds sys


