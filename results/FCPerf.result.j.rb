Performance test for fcRead1B() (sequential) with:
	Repetitions: 500
	Operations:  1000000
Execution time in milliseconds: 240931
Finish
# started on Mon May 24 16:38:26 2021


 Performance counter stats for 'java -Xcomp -ea perf.FCPerf 1 500 1000000':

         246300.42 msec task-clock                #    1.001 CPUs utilized          
             16649      context-switches          #    0.068 K/sec                  
      360911698614      cycles                    #    1.465 GHz                    
      503839814223      instructions              #    1.40  insn per cycle         
      211883687460      cache-references          #  860.265 M/sec                  
          97278782      cache-misses              #    0.046 % of all cache refs    
      104955271449      branch                    #  426.127 M/sec                  
          95823579      branch-misses                                               

     246.139141471 seconds time elapsed

     245.742637000 seconds user
       0.776475000 seconds sys


Performance test for fcRead1B() (sequential) with:
	Repetitions: 10000
	Operations:  50000
Execution time in milliseconds: 236240
Finish
# started on Mon May 24 16:42:33 2021


 Performance counter stats for 'java -Xcomp -ea perf.FCPerf 1 10000 50000':

         241496.14 msec task-clock                #    1.000 CPUs utilized          
             18117      context-switches          #    0.075 K/sec                  
      353908053276      cycles                    #    1.465 GHz                    
      503844582490      instructions              #    1.42  insn per cycle         
      211904380752      cache-references          #  877.465 M/sec                  
          95808620      cache-misses              #    0.045 % of all cache refs    
      104966028637      branch                    #  434.649 M/sec                  
         101692690      branch-misses                                               

     241.385932239 seconds time elapsed

     240.869286000 seconds user
       0.836365000 seconds sys


Performance test for fcRead1B() (sequential) with:
	Repetitions: 5000000
	Operations:  100
Execution time in milliseconds: 236433
Finish
# started on Mon May 24 16:46:34 2021


 Performance counter stats for 'java -Xcomp -ea perf.FCPerf 1 5000000 100':

         241716.28 msec task-clock                #    1.000 CPUs utilized          
             18034      context-switches          #    0.075 K/sec                  
      353995846647      cycles                    #    1.465 GHz                    
      503405200157      instructions              #    1.42  insn per cycle         
      211908774252      cache-references          #  876.684 M/sec                  
          95701196      cache-misses              #    0.045 % of all cache refs    
      104477402563      branch                    #  432.232 M/sec                  
         100593969      branch-misses                                               

     241.596871699 seconds time elapsed

     241.066192000 seconds user
       0.864136000 seconds sys


