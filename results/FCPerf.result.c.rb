Performance test for fcRead1B() (sequential) with:
	Repetitions: 500
	Operations:  1000000
Finish
# started on Mon May 24 16:33:48 2021


 Performance counter stats for './FCPerf.o 1 500 1000000':

           2044.21 msec task-clock                #    0.999 CPUs utilized          
                 3      context-switches          #    0.001 K/sec                  
        3024476713      cycles                    #    1.480 GHz                    
        6504007091      instructions              #    2.15  insn per cycle         
        1501569212      cache-references          #  734.548 M/sec                  
            544280      cache-misses              #    0.036 % of all cache refs    
        1500841955      branch                    #  734.192 M/sec                  
             24343      branch-misses                                               

       2.045490753 seconds time elapsed

       2.045650000 seconds user
       0.000000000 seconds sys


Performance test for fcRead1B() (sequential) with:
	Repetitions: 10000
	Operations:  50000
Finish
# started on Mon May 24 16:33:50 2021


 Performance counter stats for './FCPerf.o 1 10000 50000':

           2018.72 msec task-clock                #    0.999 CPUs utilized          
                 7      context-switches          #    0.003 K/sec                  
        3006111834      cycles                    #    1.489 GHz                    
        6503797378      instructions              #    2.16  insn per cycle         
        1501482506      cache-references          #  743.781 M/sec                  
            683861      cache-misses              #    0.046 % of all cache refs    
        1500796562      branch                    #  743.441 M/sec                  
             34484      branch-misses                                               

       2.019885053 seconds time elapsed

       2.020056000 seconds user
       0.000000000 seconds sys


Performance test for fcRead1B() (sequential) with:
	Repetitions: 5000000
	Operations:  100
Finish
# started on Mon May 24 16:33:53 2021


 Performance counter stats for './FCPerf.o 1 5000000 100':

           2075.71 msec task-clock                #    0.999 CPUs utilized          
                10      context-switches          #    0.005 K/sec                  
        3096017274      cycles                    #    1.492 GHz                    
        6528916290      instructions              #    2.11  insn per cycle         
        1501548479      cache-references          #  723.391 M/sec                  
             29301      cache-misses              #    0.002 % of all cache refs    
        1505817352      branch                    #  725.448 M/sec                  
           5026367      branch-misses                                               

       2.076894518 seconds time elapsed

       2.072948000 seconds user
       0.004001000 seconds sys


