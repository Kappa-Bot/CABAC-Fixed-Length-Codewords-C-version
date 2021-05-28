Performance test for fcTransferFrom() with:
	Repetitions: 2000000
	Operations:  1000
Finish
# started on Mon May 24 16:37:39 2021


 Performance counter stats for './FCPerf.o 3 2000000 1000':

           3284.39 msec task-clock                #    0.941 CPUs utilized          
                26      context-switches          #    0.008 K/sec                  
        4901459188      cycles                    #    1.492 GHz                    
        5699340601      instructions              #    1.16  insn per cycle         
        2371077436      cache-references          #  721.924 M/sec                  
          23051671      cache-misses              #    0.972 % of all cache refs    
        1138413586      branch                    #  346.614 M/sec                  
           3236991      branch-misses                                               

       3.489324760 seconds time elapsed

       0.379731000 seconds user
       2.905942000 seconds sys


Performance test for fcTransferFrom() with:
	Repetitions: 2000000
	Operations:  100000
Finish
# started on Mon May 24 16:37:43 2021


 Performance counter stats for './FCPerf.o 3 2000000 100000':

           2868.23 msec task-clock                #    0.416 CPUs utilized          
                33      context-switches          #    0.012 K/sec                  
        4226541721      cycles                    #    1.474 GHz                    
        5185960570      instructions              #    1.23  insn per cycle         
        2160063101      cache-references          #  753.100 M/sec                  
           6665345      cache-misses              #    0.309 % of all cache refs    
        1008448348      branch                    #  351.593 M/sec                  
           5003593      branch-misses                                               

       6.894660687 seconds time elapsed

       0.360682000 seconds user
       2.508747000 seconds sys


Performance test for fcTransferFrom() with:
	Repetitions: 2000000
	Operations:  10000000
Finish
# started on Mon May 24 16:37:49 2021


 Performance counter stats for './FCPerf.o 3 2000000 10000000':

           2801.78 msec task-clock                #    0.705 CPUs utilized          
                33      context-switches          #    0.012 K/sec                  
        4161218960      cycles                    #    1.485 GHz                    
        5182113505      instructions              #    1.25  insn per cycle         
        2158333344      cache-references          #  770.343 M/sec                  
            656918      cache-misses              #    0.030 % of all cache refs    
        1007170496      branch                    #  359.475 M/sec                  
           5473139      branch-misses                                               

       3.974296104 seconds time elapsed

       0.388418000 seconds user
       2.414601000 seconds sys


