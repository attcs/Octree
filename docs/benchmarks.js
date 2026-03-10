var BENCHMARK_DATA = `----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                                                                                  Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmarks::Base::GetNodeID/1000                                                                                       13993 ns        14125 ns        49778 items_per_second=70.7954M/s
Benchmarks::Base::GetDepthID/1000                                                                                        876 ns          858 ns       746667 items_per_second=1.16553G/s
Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000                                                                11457 ns        11091 ns        74667 items_per_second=90.1639M/s
Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000                                                                  12160 ns        12277 ns        56000 items_per_second=81.4545M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                0.697 us        0.680 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                 1.61 us         1.60 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                 2.33 us         2.30 us       298667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                3.62 us         3.61 us       194783
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                               38.1 us         37.7 us        18667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                              1059 us         1074 us          640
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                            10699 us        10324 us           56
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                          155867 us       160156 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                 1.50 us         1.54 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                 4.04 us         3.92 us       179200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                 8.60 us         7.32 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                15.9 us         13.9 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                               63.3 us         60.0 us        11200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                               671 us          645 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                             6863 us         7083 us           75
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                          116441 us       117188 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10           0.708 us        0.711 us      1120000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20            1.63 us         1.61 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50            2.35 us         2.35 us       298667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100           4.24 us         4.35 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000          50.1 us         51.6 us        10000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000          812 us          820 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000       11282 us        11475 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000     174536 us       171875 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10            1.49 us         1.40 us       560000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20            3.96 us         3.84 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50            9.12 us         8.16 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100           16.6 us         13.8 us        49778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000          71.8 us         68.0 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000          564 us          578 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000        8288 us         8160 us           90
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000     127090 us       128125 us            5
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                0.828 us        0.837 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                 2.65 us         2.68 us       280000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                 5.11 us         5.31 us       100000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                7.74 us         7.67 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                               89.0 us         88.9 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                              1081 us         1099 us          640
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                            26345 us        26442 us           26
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                          174257 us       171875 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/10                                 1.50 us         1.48 us       560000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/20                                 4.84 us         5.00 us       100000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/50                                 11.0 us         10.3 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/100                                17.7 us         15.4 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/1000                               96.0 us         87.9 us         7467
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/10000                               662 us          663 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/100000                            20444 us        20680 us           34
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                           98306 us        98214 us            7
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                          0.769 us        0.767 us      1120000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                           1.08 us         1.11 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                           1.84 us         1.80 us       373333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                          3.16 us         3.14 us       224000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                         33.0 us         33.0 us        20364
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                         498 us          500 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                       5677 us         5720 us          112
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                     73517 us        72443 us           11
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                           1.29 us         1.22 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                           2.03 us         1.95 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                           4.18 us         3.08 us       213333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                          8.14 us         7.53 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                         37.4 us         35.3 us        20364
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                         373 us          377 us         1867
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                       4101 us         4103 us          179
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                     55734 us        55398 us           11
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10                              1.24 us         1.23 us       560000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/20                              3.48 us         3.45 us       194783
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/50                              6.94 us         6.98 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/100                             15.5 us         15.3 us        44800
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                             165 us          167 us         4480
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                           2469 us         2456 us          299
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/10                                     2.15 us         2.18 us       344615
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/20                                     3.68 us         3.69 us       194783
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/50                                     9.25 us         9.24 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/100                                    17.2 us         16.9 us        40727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/1000                                    197 us          195 us         3446
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/10000                                  3189 us         3154 us          213
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/10                                       3.13 us         3.07 us       224000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/20                                       7.75 us         7.85 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/50                                       19.2 us         19.3 us        37333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/100                                      44.5 us         44.5 us        15448
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/1000                                      645 us          642 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/10000                                   10288 us        10498 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/10                                             1.72 us         1.74 us       448000 items_per_second=5.7344M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/20                                             3.24 us         3.22 us       213333 items_per_second=6.20605M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/50                                             8.48 us         8.37 us        74667 items_per_second=5.97336M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/100                                            19.9 us         20.1 us        37333 items_per_second=4.97773M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/1000                                            665 us          670 us         1120 items_per_second=1.49333M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/10000                                         11153 us        11315 us           58 items_per_second=883.81k/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                        159 ns          157 ns      4480000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                       251 ns          251 ns      2800000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                      427 ns          430 ns      1600000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                     918 ns          907 ns       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/100                              0.939 us        0.942 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/1000                              1.66 us         1.65 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/10000                             2.85 us         2.85 us       235789
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/100000                            4.19 us         4.24 us       165926
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                           4.62 us         4.55 us       154483
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                          18.8 us         19.3 us        37333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                   1.52 us         1.50 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                  10.4 us         10.3 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                     0.638 us        0.642 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                      1.62 us         1.57 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                      2.37 us         2.41 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                     3.70 us         3.68 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                    41.0 us         40.8 us        17231
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                    797 us          785 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                 10769 us        10986 us           64
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                               211670 us       208333 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                      1.40 us         1.37 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                      3.92 us         3.84 us       203636
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                      8.75 us         8.20 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                     16.1 us         12.6 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                    65.2 us         65.6 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                    586 us          586 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                  7374 us         7292 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                               159546 us       160156 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.640 us        0.642 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.61 us         1.60 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 2.37 us         2.35 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                4.36 us         4.39 us       160000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               52.0 us         51.6 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               866 us          858 us          747
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            12099 us        11998 us           56
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          232411 us       234375 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.45 us         1.40 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.97 us         3.93 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 8.64 us         8.09 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                16.0 us         14.1 us        49778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000               74.1 us         71.1 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000               604 us          600 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000             7884 us         7812 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          169575 us       171875 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                     0.773 us        0.785 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                      2.86 us         2.83 us       248889
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                      5.56 us         5.58 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                     8.54 us         8.54 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                                    96.5 us         96.3 us         7467
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                                   1187 us         1172 us          560
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                                 32101 us        31994 us           21
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                               245744 us       244792 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10                                      1.42 us         1.32 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/20                                      4.35 us         4.33 us       165926
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/50                                      12.4 us         11.7 us        56000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100                                     18.4 us         14.0 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000                                     106 us          103 us         7467
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10000                                    772 us          753 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100000                                 23101 us        22917 us           30
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                               148041 us       148438 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                               0.791 us        0.785 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                1.13 us         1.12 us       640000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                1.96 us         1.95 us       344615
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                               3.40 us         3.38 us       203636
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                              35.7 us         35.2 us        18667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                              534 us          530 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                            6646 us         6696 us          112
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                          96293 us        96354 us            6
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                1.63 us         1.43 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                2.61 us         2.49 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                6.06 us         5.02 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                               10.9 us         9.35 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                              49.0 us         50.0 us        10000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                              442 us          417 us         1723
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                            5122 us         5156 us          100
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                          79840 us        79861 us            9
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                   1.07 us         1.05 us       640000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                   3.32 us         3.37 us       213333
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                   6.58 us         6.56 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/100                                  15.2 us         15.0 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                                  169 us          171 us         4480
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                                2549 us         2567 us          280
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/10                                          2.31 us         2.34 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/20                                          4.03 us         3.99 us       172308
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/50                                          10.4 us         10.3 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/100                                         19.3 us         19.3 us        37333
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/1000                                         291 us          292 us         2358
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/10000                                       4251 us         4236 us          166
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/10                                            2.46 us         2.46 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/20                                            6.50 us         6.42 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/50                                            16.6 us         16.5 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/100                                           37.9 us         37.7 us        18667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/1000                                           493 us          497 us         1445
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/10000                                         7647 us         7639 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/10                                                  1.63 us         1.61 us       407273 items_per_second=6.20606M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/20                                                  3.14 us         3.14 us       224000 items_per_second=6.37156M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/50                                                  7.85 us         7.85 us        89600 items_per_second=6.37156M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/100                                                 20.2 us         20.4 us        34462 items_per_second=4.90126M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/1000                                                 664 us          670 us         1120 items_per_second=1.49333M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/10000                                              12443 us        12355 us           43 items_per_second=809.412k/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                             139 ns          138 ns      4977778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                            198 ns          201 ns      3733333
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                           331 ns          330 ns      2133333
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                          684 ns          663 ns       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100                                   0.876 us        0.879 us       746667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/1000                                   1.52 us         1.54 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/10000                                  2.44 us         2.46 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100000                                 3.83 us         3.84 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                                3.99 us         4.01 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                               15.9 us         15.7 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                        1.33 us         1.32 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                       9.61 us         9.63 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                      0.726 us        0.725 us      1120000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                       1.84 us         1.84 us       373333
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                       2.38 us         2.30 us       298667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                      4.02 us         3.99 us       172308
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                     40.6 us         40.8 us        17231
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                     666 us          670 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                   8327 us         8125 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                105848 us       104167 us            6
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                       1.44 us         1.39 us       640000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                       3.94 us         3.85 us       186667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                       8.46 us         8.02 us        89600
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                      15.0 us         13.9 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                     85.7 us         83.7 us        11200
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                     555 us          531 us         1000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                   5142 us         5156 us          100
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                 53555 us        52557 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                 0.732 us        0.732 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                  1.89 us         1.93 us       373333
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                  2.45 us         2.46 us       280000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                 4.21 us         4.14 us       165926
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                39.3 us         39.2 us        17920
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                635 us          628 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              8352 us         8333 us           90
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           106209 us       106771 us            6
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                  1.47 us         1.38 us       497778
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                  4.07 us         3.90 us       172308
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                  8.24 us         7.81 us       112000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                 15.2 us         14.1 us        49778
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                87.9 us         77.4 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                531 us          531 us         1000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000              5196 us         5162 us          112
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            54072 us        55398 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                      0.849 us        0.837 us       746667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                       2.81 us         2.76 us       248889
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                       5.38 us         5.44 us       112000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                      8.74 us         8.58 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                                     87.9 us         88.9 us         8960
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                                     944 us          920 us          747
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                                  15718 us        15972 us           45
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                                148409 us       148438 us            4
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10                                       1.58 us         1.53 us       407273
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/20                                       4.87 us         4.65 us       154483
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/50                                       10.3 us         8.79 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100                                      22.5 us         17.7 us        34462
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000                                      123 us          115 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10000                                     620 us          578 us         1000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100000                                   9336 us         9167 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                                 56908 us        56818 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                              120 ns          120 ns      5600000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                             162 ns          164 ns      4480000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                            260 ns          261 ns      2635294
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                           518 ns          516 ns      1000000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100                                    0.848 us        0.854 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/1000                                    1.40 us         1.38 us       497778
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/10000                                   2.07 us         2.09 us       344615
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100000                                  3.19 us         3.22 us       213333
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                                 3.13 us         3.18 us       235789
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                                11.4 us         11.2 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                         1.12 us         1.12 us       560000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                        7.07 us         7.15 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                   0.753 us        0.753 us      1120000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                    1.65 us         1.65 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                    2.40 us         2.41 us       298667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                   3.66 us         3.68 us       186667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                  38.5 us         38.5 us        18667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                  712 us          711 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                               10073 us        10000 us           75
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                             169577 us       171875 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                    1.52 us         1.44 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                    4.08 us         3.99 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                    10.3 us         9.77 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                   13.9 us         12.8 us        56000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                  65.4 us         60.9 us        10000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                  530 us          516 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                6897 us         6771 us           90
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                             132626 us       134375 us            5
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10              0.748 us        0.753 us      1120000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20               1.66 us         1.65 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50               2.41 us         2.41 us       298667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100              4.31 us         4.24 us       165926
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000             51.4 us         51.6 us        10000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000             819 us          816 us          747
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000          11490 us        11440 us           56
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000        188243 us       187500 us            3
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10               1.53 us         1.54 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20               3.46 us         3.21 us       224000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50               9.05 us         8.65 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100              17.1 us         13.4 us        56000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000             72.5 us         66.3 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000             571 us          547 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000           7801 us         7639 us           90
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000        142270 us       143750 us            5
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/10                                   0.862 us        0.858 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/20                                    2.68 us         2.73 us       263529
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/50                                    5.14 us         5.16 us       100000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/100                                   7.70 us         7.81 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/1000                                  87.4 us         87.2 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/10000                                 1012 us         1025 us          747
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/100000                               26219 us        26442 us           26
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/1000000                             180127 us       179688 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/10                                    1.67 us         1.53 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/20                                    4.95 us         4.74 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/50                                    11.2 us         11.0 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/100                                   18.0 us         14.5 us        56000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/1000                                  97.7 us         90.0 us         7467
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/10000                                  663 us          663 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/100000                               21065 us        21140 us           34
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/1000000                             110248 us       109375 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                             0.815 us        0.816 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                              1.14 us         1.14 us       560000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                              1.96 us         1.95 us       344615
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                             3.38 us         3.38 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                            34.7 us         34.5 us        20364
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                            512 us          516 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                          5863 us         5859 us          112
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                        77166 us        78125 us            9
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                              1.40 us         1.35 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                              2.13 us         2.09 us       344615
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                              4.28 us         2.79 us       263529
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                             10.4 us         8.54 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                            39.0 us         35.3 us        19478
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                            400 us          385 us         1867
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                          4339 us         4395 us          160
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                        60427 us        59659 us           11
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10                                 1.52 us         1.53 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/20                                 3.98 us         3.99 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/50                                 7.74 us         7.67 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/100                                16.9 us         16.9 us        40727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/1000                                172 us          169 us         4073
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10000                              2541 us         2486 us          264
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/10                                        2.48 us         2.46 us       280000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/20                                        4.12 us         4.14 us       165926
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/50                                        10.1 us         10.0 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/100                                       18.2 us         18.4 us        37333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/1000                                       204 us          204 us         3446
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/10000                                     3256 us         3301 us          213
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/10                                          3.47 us         3.45 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/20                                          8.32 us         8.37 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/50                                          20.3 us         19.9 us        34462
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/100                                         46.6 us         47.5 us        15448
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/1000                                         658 us          656 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/10000                                      10430 us        10498 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/10                                                1.67 us         1.67 us       448000 items_per_second=5.97333M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/20                                                3.32 us         3.38 us       203636 items_per_second=5.92396M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/50                                                9.50 us         9.28 us        64000 items_per_second=5.38947M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/100                                               22.1 us         22.0 us        29867 items_per_second=4.55116M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/1000                                               687 us          680 us          896 items_per_second=1.47036M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/10000                                            16904 us        16544 us           34 items_per_second=604.444k/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/100                                           162 ns          165 ns      4072727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                          255 ns          246 ns      2800000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                         431 ns          439 ns      1600000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                        916 ns          942 ns       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/100                                 0.934 us        0.921 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/1000                                 1.67 us         1.67 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/10000                                2.85 us         2.83 us       248889
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/100000                               4.21 us         4.14 us       165926
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                              4.67 us         4.60 us       149333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                             18.6 us         18.8 us        37333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                      1.52 us         1.54 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                     10.2 us         10.3 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                        0.673 us        0.670 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                         1.66 us         1.67 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                         2.43 us         2.46 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                        3.78 us         3.77 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                       41.4 us         41.4 us        16593
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                       801 us          802 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                    10992 us        10986 us           64
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                  226184 us       223958 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                         1.43 us         1.32 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                         3.65 us         3.44 us       213333
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                         8.81 us         8.37 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                        13.7 us         12.3 us        56000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                       68.8 us         64.2 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                       598 us          578 us         1000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                     7528 us         7465 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                  174352 us       175781 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.682 us        0.698 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    1.66 us         1.67 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    2.43 us         2.40 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   4.46 us         4.49 us       160000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  52.6 us         51.6 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  874 us          858 us          747
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               12128 us        11963 us           64
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000             251953 us       250000 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.45 us         1.38 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                    4.03 us         3.96 us       213333
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                    8.95 us         8.79 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                   17.1 us         15.0 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  75.3 us         73.2 us         8960
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  615 us          614 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                8219 us         8160 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             185715 us       187500 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10                                        0.818 us        0.820 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/20                                         2.89 us         2.89 us       248889
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/50                                         5.58 us         5.62 us       100000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100                                        8.40 us         8.54 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000                                       95.5 us         96.3 us         7467
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10000                                      1143 us         1123 us          640
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100000                                    32385 us        32738 us           21
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000000                                  249915 us       255208 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10                                         1.52 us         1.43 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/20                                         5.11 us         5.16 us       100000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/50                                         11.6 us         11.0 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100                                        18.5 us         14.3 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000                                        105 us          100 us         6400
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10000                                       775 us          767 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100000                                    24579 us        24554 us           28
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000000                                  164499 us       164062 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                  0.844 us        0.854 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                   1.20 us         1.17 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                   2.09 us         2.10 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                  3.62 us         3.52 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                 37.8 us         38.5 us        18667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                 557 us          558 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                               6841 us         6771 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                            102054 us       104167 us            6
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                   1.72 us         1.61 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                   2.69 us         2.40 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                   5.62 us         4.69 us       160000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                                  10.6 us         7.95 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                 49.5 us         44.3 us        16593
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                 461 us          435 us         1545
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                               5324 us         5301 us          112
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                             84205 us        84821 us            7
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10                                      1.29 us         1.28 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/20                                      3.71 us         3.68 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/50                                      7.25 us         7.25 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/100                                     16.4 us         16.1 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/1000                                     176 us          176 us         4073
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10000                                   2608 us         2604 us          264
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/10                                             2.64 us         2.68 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/20                                             4.51 us         4.45 us       154483
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/50                                             11.1 us         11.2 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/100                                            20.4 us         20.0 us        32000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/1000                                            297 us          292 us         2358
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/10000                                          4312 us         4297 us          160
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/10                                               2.78 us         2.76 us       248889
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/20                                               6.98 us         6.98 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/50                                               17.6 us         17.6 us        37333
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/100                                              40.2 us         41.0 us        17920
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/1000                                              501 us          500 us         1000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/10000                                            7816 us         7812 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/10                                                     1.48 us         1.48 us       497778 items_per_second=6.77825M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/20                                                     2.95 us         2.92 us       235789 items_per_second=6.85932M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/50                                                     9.38 us         9.42 us        74667 items_per_second=5.30965M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/100                                                    21.1 us         21.2 us        28000 items_per_second=4.71579M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/1000                                                    676 us          680 us          896 items_per_second=1.47036M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/10000                                                 17352 us        17463 us           34 items_per_second=572.632k/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                142 ns          141 ns      4977778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                               200 ns          205 ns      3733333
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                              330 ns          337 ns      2133333
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                             683 ns          663 ns       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100                                      0.880 us        0.879 us       746667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/1000                                      1.51 us         1.50 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/10000                                     2.44 us         2.46 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100000                                    3.81 us         3.84 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                                   3.96 us         3.92 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                                  16.0 us         16.1 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                           1.33 us         1.34 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                          9.16 us         9.21 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                         0.734 us        0.732 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                          1.85 us         1.84 us       373333
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                          2.38 us         2.35 us       298667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                         4.02 us         4.08 us       172308
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                        40.6 us         41.0 us        17920
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                        659 us          670 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                      8256 us         8160 us           90
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                   104741 us       102679 us            7
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                          1.44 us         1.42 us       560000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                          4.01 us         4.02 us       186667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                          8.65 us         8.20 us        89600
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                         15.0 us         13.7 us        56000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                        86.0 us         82.0 us         8960
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                        570 us          558 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                      5112 us         5022 us          112
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                    52718 us        53125 us           10
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    0.738 us        0.732 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                     1.90 us         1.93 us       373333
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                     2.46 us         2.40 us       280000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                    4.24 us         4.24 us       165926
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                   39.2 us         39.3 us        18667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   634 us          628 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                 8358 us         8125 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              105112 us       104911 us            7
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                     1.47 us         1.36 us       448000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                     3.96 us         3.93 us       186667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                     8.80 us         8.37 us        89600
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                    15.3 us         14.5 us        56000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                   85.6 us         77.4 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                   527 us          530 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                 5121 us         5022 us          112
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000               53228 us        53125 us           10
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10                                         0.852 us        0.854 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/20                                          2.81 us         2.83 us       248889
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/50                                          5.35 us         5.30 us       112000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100                                         8.69 us         8.58 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000                                        87.4 us         87.2 us         8960
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10000                                        943 us          941 us          747
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100000                                     15659 us        15278 us           45
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000000                                   149020 us       150000 us            5
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10                                          1.61 us         1.53 us       448000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/20                                          4.77 us         4.81 us       149333
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/50                                          10.2 us         9.52 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100                                         22.3 us         19.1 us        49778
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000                                         120 us          109 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10000                                        609 us          600 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100000                                      9607 us         9583 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000000                                    57093 us        56818 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                 121 ns          120 ns      5600000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                                162 ns          164 ns      4480000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                               260 ns          261 ns      2635294
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                              518 ns          516 ns      1000000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100                                       0.848 us        0.854 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/1000                                       1.40 us         1.38 us       497778
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/10000                                      2.02 us         2.04 us       344615
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100000                                     3.17 us         3.21 us       224000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                                    3.10 us         3.14 us       224000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                                   11.3 us         11.2 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                            1.10 us         1.10 us       640000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                           7.04 us         6.98 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                  0.899 us        0.879 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                   1.72 us         1.69 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                   3.40 us         3.44 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                  6.04 us         6.00 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                 53.9 us         54.7 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                 702 us          698 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                               7747 us         7812 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                             92980 us        92014 us            9
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                   1.68 us         1.64 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                   3.63 us         3.18 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                   12.9 us         11.0 us        49778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                  15.7 us         12.8 us        56000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                 63.3 us         60.0 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                 346 us          345 us         2036
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                               3143 us         3139 us          224
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                             39790 us        39522 us           17
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10             0.902 us        0.924 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20              1.71 us         1.69 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50              4.04 us         4.08 us       172308
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100             8.16 us         8.02 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000            63.4 us         64.2 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000            776 us          767 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000          8403 us         8333 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000        98915 us        98214 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10              1.70 us         1.40 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20              3.52 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50              8.64 us         8.16 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100             17.6 us         14.0 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000            68.1 us         61.0 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000            368 us          351 us         1867
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000          3281 us         3348 us          224
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000        40445 us        40799 us           18
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                            0.917 us        0.921 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                             1.38 us         1.37 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                             2.64 us         2.64 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                            4.87 us         4.87 us       144516
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                           46.2 us         46.5 us        15448
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                           620 us          614 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                         7081 us         6944 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                       77488 us        78125 us            9
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                             1.55 us         1.51 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                             3.12 us         2.91 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                             5.22 us         4.46 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                            9.10 us         6.70 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                           36.3 us         33.8 us        20364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                           356 us          329 us         1948
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                         4132 us         3997 us          172
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                       45880 us        45833 us           15
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/10                                        0.333 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/20                                        0.333 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/50                                        0.334 us        0.330 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/100                                       0.333 us        0.330 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/1000                                      0.332 us        0.330 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/10000                                     0.334 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                1.24 us         1.23 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                3.23 us         3.22 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                9.76 us         9.84 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/100                               15.4 us         15.3 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                               144 us          144 us         4978
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                             1493 us         1475 us          498
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/10                                               1.99 us         2.01 us       373333 items_per_second=4.97777M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/20                                               3.72 us         3.69 us       194783 items_per_second=5.42005M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/50                                               8.41 us         8.54 us        89600 items_per_second=5.85143M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/100                                              17.1 us         16.9 us        40727 items_per_second=5.92393M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/1000                                              281 us          282 us         2489 items_per_second=3.53991M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/10000                                            7955 us         7917 us           75 items_per_second=1.26316M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100                                           199 ns          199 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/1000                                          239 ns          241 ns      2986667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/10000                                         799 ns          795 ns      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100000                                       7881 ns         7847 ns        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                          154 ns          157 ns      4480000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                         195 ns          197 ns      3733333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                        791 ns          802 ns       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                      6765 ns         6836 ns       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                    0.547 us        0.544 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                    3.42 us         3.38 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                         3.36 us         3.37 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                         128 us          126 us         4978
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                       5772 us         5859 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                    212851 us       208333 us            3
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                         3.79 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                         131 us          131 us         5600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                       2072 us          889 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                     71206 us        44922 us           16
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                   22.9 us         22.9 us        28000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                   181 us          180 us         4073
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                 5918 us         5999 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000              489517 us       484375 us            2
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100                                0.594 us        0.600 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                1.41 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/10000                               6.89 us         6.98 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100000                              72.3 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100                                  0.546 us        0.531 us      1000000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/1000                                  1.36 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/10000                                 6.92 us         6.80 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100000                                80.9 us         82.0 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                    0.770 us        0.750 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                     1.43 us         1.40 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                     2.57 us         2.62 us       280000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                    5.05 us         5.00 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                   47.0 us         47.1 us        14933
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                   688 us          684 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                 8538 us         8750 us           75
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                              118944 us       114583 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10                                     1.48 us         1.45 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/20                                     3.20 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/50                                     9.45 us         9.42 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100                                    17.2 us         15.4 us        49778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                   72.7 us         67.2 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                   454 us          452 us         1659
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                 4864 us         5000 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                               75261 us        76389 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10               0.764 us        0.767 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                1.41 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                3.23 us         3.30 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100               7.16 us         6.98 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000              56.8 us         57.8 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000              773 us          802 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            9375 us         9167 us           75
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000         127736 us       127604 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                1.37 us         1.32 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                3.01 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                9.89 us         8.79 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100               18.5 us         16.0 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000              77.8 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000              484 us          430 us         1600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000            5101 us         5022 us          112
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          76944 us        78125 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                              0.803 us        0.785 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                               1.14 us         1.14 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                               2.06 us         2.05 us       320000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                              3.65 us         3.61 us       194783
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                             35.8 us         36.1 us        20364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                             577 us          562 us         1000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                           6722 us         6771 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                         72731 us        72917 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                               1.37 us         1.31 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                               2.21 us         2.01 us       373333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                               5.22 us         4.45 us       179200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                              10.7 us         8.28 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                             33.5 us         31.5 us        21333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                             431 us          430 us         1600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                           4788 us         4719 us          149
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                         52163 us        51136 us           11
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/10                                          0.345 us        0.345 us      1947826
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/20                                          0.346 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/50                                          0.344 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/100                                         0.344 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/1000                                        0.345 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/10000                                       0.344 us        0.353 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/10                                  1.13 us         1.12 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/20                                  2.87 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/50                                  8.49 us         8.37 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/100                                 15.3 us         15.3 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/1000                                 142 us          144 us         4978
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/10000                               1905 us         1927 us          373
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/10                                                 1.71 us         1.73 us       407273 items_per_second=5.79233M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/20                                                 3.26 us         3.22 us       213333 items_per_second=6.20605M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/50                                                 8.19 us         8.16 us        74667 items_per_second=6.12652M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/100                                                14.4 us         14.3 us        44800 items_per_second=6.99317M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/1000                                                153 us          154 us         4978 items_per_second=6.50188M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/10000                                              2981 us         2930 us          224 items_per_second=3.41333M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100                                             239 ns          234 ns      2800000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/1000                                            296 ns          292 ns      2357895
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/10000                                           547 ns          544 ns      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100000                                          980 ns          963 ns       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100                                            201 ns          204 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/1000                                           305 ns          300 ns      2240000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/10000                                         1013 ns         1025 ns       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100000                                        9932 ns         9835 ns        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/1000                                       1.12 us         1.12 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/10000                                      3.75 us         3.68 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                           50.3 us         50.2 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                          1439 us         1443 us          498
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                        27887 us        27500 us           25
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                      475132 us       468750 us            2
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                           54.8 us         54.7 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                          1479 us         1465 us          448
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                         9708 us         3342 us          187
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                      132376 us        89286 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                     32.2 us         32.8 us        22400
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                     244 us          246 us         2800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                   6603 us         6557 us          112
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                145668 us       148438 us            4
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100                                  0.796 us        0.785 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                  1.20 us         1.20 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                 3.17 us         3.14 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                7.74 us         7.67 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100                                    0.662 us        0.670 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/1000                                    1.02 us         1.03 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/10000                                   2.63 us         2.67 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100000                                  10.2 us         10.3 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                       0.834 us        0.816 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                        1.69 us         1.67 us       373333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                        3.39 us         3.44 us       213333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                       6.16 us         6.14 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                      56.2 us         56.2 us        10000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                      753 us          750 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                    8239 us         8160 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                 117438 us       117188 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                        1.62 us         1.50 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                        3.48 us         3.14 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                        8.92 us         8.13 us        69208
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                       16.2 us         12.8 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                      66.8 us         65.6 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                      403 us          399 us         1723
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                    3656 us         3593 us          187
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                  63788 us        63920 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                  0.843 us        0.837 us       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                   1.70 us         1.73 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                   4.15 us         4.08 us       172308
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                  8.46 us         8.54 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                 65.9 us         65.6 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                 820 us          816 us          747
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               8926 us         8750 us           75
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            124049 us       122396 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                   1.50 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                   3.29 us         3.08 us       213333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                   8.38 us         8.20 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                  17.8 us         14.1 us        49778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                 71.6 us         65.6 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                 411 us          408 us         1723
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000               3681 us         3676 us          187
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             63737 us        63920 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                                 0.970 us        0.984 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                  1.45 us         1.43 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                  2.78 us         2.79 us       263529
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                                 5.13 us         5.16 us       100000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                                49.2 us         50.0 us        10000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                                667 us          663 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                              8495 us         8542 us           75
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                           109568 us       109375 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                  2.00 us         1.91 us       497778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                  3.16 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                  7.72 us         7.50 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                                 10.5 us         8.16 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                                49.8 us         42.7 us        17920
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                                438 us          430 us         1600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                              5684 us         5859 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                            77827 us        78125 us            7
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/10                                             0.274 us        0.279 us      2635294
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/20                                             0.275 us        0.273 us      2635294
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/50                                             0.274 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/100                                            0.274 us        0.264 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/1000                                           0.274 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/10000                                          0.275 us        0.273 us      2635294
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                     1.06 us         1.05 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                     3.08 us         3.11 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                     9.69 us         9.63 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/100                                    15.4 us         15.3 us        44800
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                                    147 us          151 us         4978
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                                  1564 us         1569 us          448
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/10                                                    1.90 us         1.93 us       373333 items_per_second=5.1942M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/20                                                    3.48 us         3.53 us       203636 items_per_second=5.66639M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/50                                                    7.85 us         7.85 us        89600 items_per_second=6.37156M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/100                                                   16.0 us         16.0 us        44800 items_per_second=6.23304M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/1000                                                   274 us          270 us         2489 items_per_second=3.70456M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/10000                                                 7927 us         8125 us           75 items_per_second=1.23077M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100                                                161 ns          157 ns      4480000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/1000                                               188 ns          188 ns      3733333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/10000                                              668 ns          670 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100000                                            7425 ns         7499 ns        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                               127 ns          126 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                              155 ns          157 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                             652 ns          656 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                           5831 ns         5859 ns       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                         0.429 us        0.433 us      1659259
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                         3.03 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                              3.26 us         3.35 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                              105 us          107 us         7467
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                            5370 us         5301 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                         207737 us       208333 us            3
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                              3.51 us         3.45 us       203636
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                              107 us          107 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                            1910 us          525 us         1339
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                          72408 us        46875 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                        12.3 us         12.3 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                        114 us          112 us         5600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                      4306 us         4330 us          166
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                   436335 us       437500 us            2
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                     0.425 us        0.424 us      1659259
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                     1.11 us         1.10 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                    6.12 us         6.28 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                   70.6 us         71.1 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                       0.442 us        0.443 us      1659259
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                       1.13 us         1.15 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                      6.26 us         6.28 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                     78.3 us         78.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                         0.697 us        0.698 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                          1.38 us         1.37 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                          2.40 us         2.40 us       280000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                         5.26 us         5.31 us       100000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                        50.7 us         50.0 us        10000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                        763 us          750 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                      9395 us         9277 us           64
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                   164881 us       167969 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10                                          1.37 us         1.26 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/20                                          2.87 us         2.79 us       263529
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/50                                          7.71 us         7.11 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100                                         17.9 us         15.0 us        40727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                        76.5 us         76.7 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                        518 us          488 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                      5576 us         5580 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                   113976 us       114583 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    0.695 us        0.698 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                     1.38 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                     3.12 us         3.14 us       224000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                    7.47 us         7.53 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                   59.1 us         60.0 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   826 us          816 us          747
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                11267 us        10986 us           64
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              182315 us       182292 us            3
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                     1.38 us         1.28 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                     3.02 us         2.78 us       235789
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                     8.51 us         7.67 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                    19.7 us         16.8 us        34462
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                   79.4 us         68.4 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                   525 us          502 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                 5633 us         5720 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              113570 us       111979 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                                   0.844 us        0.854 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                    1.20 us         1.20 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                    2.19 us         2.20 us       320000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                                   3.92 us         3.92 us       179200
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                                  39.1 us         39.2 us        17920
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                                  622 us          628 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                                8259 us         8125 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                             105768 us       104167 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                    1.71 us         1.60 us       448000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                    2.66 us         2.36 us       344615
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                    5.94 us         4.88 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                                   10.4 us         7.74 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                                  46.5 us         33.0 us        16593
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                                  493 us          496 us         1545
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                                6346 us         6278 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                              85141 us        84821 us            7
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/10                                               0.277 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/20                                               0.277 us        0.285 us      2635294
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/50                                               0.279 us        0.279 us      2635294
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/100                                              0.279 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/1000                                             0.278 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/10000                                            0.278 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/10                                      0.928 us        0.942 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/20                                       2.64 us         2.61 us       263529
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/50                                       8.10 us         8.02 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/100                                      15.2 us         15.3 us        44800
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/1000                                      145 us          144 us         4978
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/10000                                    2014 us         1993 us          345
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/10                                                      1.59 us         1.60 us       448000 items_per_second=6.23304M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/20                                                      3.06 us         3.11 us       235789 items_per_second=6.42149M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/50                                                      7.64 us         7.67 us        89600 items_per_second=6.51636M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/100                                                     13.3 us         13.4 us        56000 items_per_second=7.46667M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/1000                                                     146 us          148 us         4978 items_per_second=6.77855M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/10000                                                   2951 us         2947 us          228 items_per_second=3.39349M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100                                                  158 ns          160 ns      4480000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/1000                                                 169 ns          169 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/10000                                                233 ns          230 ns      2986667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100000                                               348 ns          345 ns      1947826
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100                                                 125 ns          126 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/1000                                                134 ns          132 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/10000                                               199 ns          199 ns      3446154
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100000                                              535 ns          547 ns      1000000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/1000                                           0.315 us        0.318 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/10000                                           1.26 us         1.26 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                                33.5 us         33.0 us        20364
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                682 us          680 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                             11847 us        11440 us           56
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                           213601 us       218750 us            3
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                                34.0 us         34.5 us        20364
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                                685 us          698 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                              4296 us         1592 us          373
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                            65416 us        41992 us           16
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                          12.5 us         12.6 us        56000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                          120 us          120 us         5600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                        2146 us         2148 us          320
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                      50045 us        49716 us           11
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100                                       0.346 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                      0.415 us        0.414 us      1659259
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                     0.815 us        0.816 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                     1.77 us         1.76 us       373333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100                                         0.368 us        0.368 us      1866667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/1000                                        0.439 us        0.439 us      1600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/10000                                       0.869 us        0.858 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100000                                       3.21 us         3.28 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                     0.942 us        0.942 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                      1.76 us         1.76 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                      3.43 us         3.45 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                     6.10 us         6.14 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                    54.4 us         54.4 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                    714 us          711 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                                  7893 us         7812 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                95709 us        95982 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                      1.66 us         1.60 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                      3.89 us         3.38 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                      12.9 us         11.9 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                     15.8 us         12.6 us        49778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                    66.3 us         54.4 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                    374 us          366 us         2133
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                  3393 us         3301 us          213
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                                47372 us        47917 us           15
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.946 us        0.942 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.75 us         1.76 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 4.11 us         4.17 us       172308
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                8.22 us         8.02 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               65.3 us         65.6 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               805 us          795 us          747
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000             8735 us         8542 us           75
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          105059 us       106771 us            6
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.65 us         1.57 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.86 us         3.88 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                 9.62 us         8.79 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                17.9 us         13.0 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000               69.6 us         65.6 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000               400 us          385 us         1867
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000             3537 us         3526 us          195
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000           47234 us        47917 us           15
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                               0.995 us        0.984 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                1.45 us         1.43 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                2.75 us         2.70 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                               5.07 us         5.16 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                              47.7 us         48.1 us        14933
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                              640 us          642 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                            7335 us         7465 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                          81233 us        81597 us            9
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                1.72 us         1.66 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                3.34 us         2.89 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                5.69 us         4.74 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                               9.46 us         6.63 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                              38.9 us         37.4 us        21333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                              385 us          375 us         1792
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                            4422 us         4395 us          160
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                          50550 us        50000 us           10
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/10                                           0.339 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/20                                           0.338 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/50                                           0.339 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/100                                          0.340 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/1000                                         0.339 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/10000                                        0.339 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10                                   1.51 us         1.50 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/20                                   3.63 us         3.61 us       194783
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/50                                   10.5 us         10.5 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/100                                  16.5 us         16.4 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/1000                                  153 us          153 us         4480
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10000                                1571 us         1569 us          448
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/10                                                  1.46 us         1.43 us       448000 items_per_second=6.99317M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/20                                                  2.70 us         2.70 us       248889 items_per_second=7.40879M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/50                                                  5.90 us         5.86 us       112000 items_per_second=8.53333M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/100                                                 12.3 us         12.3 us        56000 items_per_second=8.14545M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/1000                                                 230 us          230 us         2987 items_per_second=4.34473M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/10000                                               7456 us         7465 us           90 items_per_second=1.33953M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100                                              200 ns          199 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/1000                                             239 ns          241 ns      2986667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/10000                                            801 ns          802 ns       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100000                                          7786 ns         7743 ns        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100                                             156 ns          157 ns      4977778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                            195 ns          193 ns      3733333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                           790 ns          802 ns       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                         6687 ns         6627 ns        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                       0.541 us        0.547 us      1000000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                       3.41 us         3.38 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                            3.36 us         3.38 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                            128 us          128 us         5600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                          5770 us         5781 us          100
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                       212200 us       213542 us            3
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100                            3.77 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                            130 us          131 us         5600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                          2085 us          785 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                        78212 us        47991 us           14
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                      23.2 us         23.0 us        29867
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                      181 us          180 us         3733
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                    5851 us         5859 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                 489935 us       484375 us            2
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100                                   0.592 us        0.609 us      1000000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/1000                                   1.41 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/10000                                  6.87 us         6.84 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100000                                 72.3 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100                                     0.553 us        0.562 us      1000000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/1000                                     1.36 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/10000                                    6.90 us         6.84 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100000                                   79.9 us         80.2 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10                                       0.997 us         1.00 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/20                                        1.45 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/50                                        2.42 us         2.40 us       280000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100                                       5.09 us         5.16 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000                                      47.5 us         47.6 us        14452
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10000                                      706 us          715 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100000                                    8796 us         8750 us           75
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                 131814 us       130208 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10                                        2.75 us         2.64 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/20                                        3.07 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/50                                        8.33 us         7.15 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100                                       17.9 us         14.4 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000                                      73.1 us         71.1 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10000                                      477 us          455 us         1545
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100000                                    5235 us         5000 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000000                                  87674 us        89286 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                  0.998 us         1.00 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                   1.45 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                   3.05 us         3.00 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                  7.18 us         7.25 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                 58.5 us         58.6 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                 809 us          802 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               9801 us         9766 us           64
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            140307 us       140625 us            5
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                   2.02 us         2.01 us       373333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                   3.15 us         2.98 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                   9.49 us         8.16 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                  19.3 us         17.6 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                 77.9 us         71.1 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                 508 us          500 us         1000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000               5441 us         5469 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             89886 us        89286 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                 0.871 us        0.872 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                  1.22 us         1.23 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                  2.19 us         2.20 us       320000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                 3.83 us         3.84 us       179200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                37.3 us         37.7 us        18667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                595 us          600 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                              6982 us         6944 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                            77146 us        78125 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10                                  1.42 us         1.34 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/20                                  2.21 us         2.09 us       344615
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/50                                  4.23 us         3.81 us       172308
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100                                 11.3 us         7.32 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                35.4 us         34.5 us        20364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                457 us          460 us         1493
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100000                              5071 us         5000 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                            57250 us        56818 us           11
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/10                                             0.343 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/20                                             0.343 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/50                                             0.345 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/100                                            0.344 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/1000                                           0.343 us        0.337 us      1947826
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/10000                                          0.343 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/10                                     1.42 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/20                                     3.27 us         3.22 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/50                                     9.07 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/100                                    16.4 us         16.1 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/1000                                    154 us          157 us         4978
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/10000                                  1982 us         1993 us          345
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/10                                                    1.30 us         1.31 us       560000 items_per_second=7.62553M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/20                                                    2.45 us         2.51 us       280000 items_per_second=7.96444M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/50                                                    6.13 us         6.14 us       112000 items_per_second=8.14545M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/100                                                   10.6 us         10.5 us        64000 items_per_second=9.52558M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/1000                                                   118 us          115 us         6400 items_per_second=8.71489M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/10000                                                 2581 us         2615 us          251 items_per_second=3.82476M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100                                                248 ns          246 ns      2800000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/1000                                               299 ns          298 ns      2357895
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/10000                                              555 ns          558 ns      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100000                                             987 ns          984 ns       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100                                               206 ns          205 ns      3200000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/1000                                              307 ns          300 ns      2240000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/10000                                            1027 ns         1046 ns       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100000                                          10077 ns        10045 ns        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/1000                                          1.13 us         1.12 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/10000                                         3.74 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                              50.2 us         50.0 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                             1435 us         1412 us          498
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                           27361 us        27500 us           25
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                         472465 us       476562 us            2
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100                              50.7 us         51.6 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                             1404 us         1412 us          498
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                            9759 us         3753 us          179
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                         132079 us        93750 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                        32.7 us         33.0 us        21333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                        242 us          246 us         2987
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                      6615 us         6597 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                   146188 us       144531 us            4
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100                                     0.917 us        0.921 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/1000                                     1.32 us         1.34 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/10000                                    3.32 us         3.30 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100000                                   8.02 us         8.02 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100                                       0.660 us        0.670 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/1000                                       1.02 us         1.03 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/10000                                      2.65 us         2.67 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100000                                     10.3 us         10.3 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                           1.05 us         1.07 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                           1.92 us         1.90 us       344615
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                           3.61 us         3.69 us       194783
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                          6.51 us         6.56 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                         57.0 us         57.2 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                         770 us          767 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                       8576 us         8542 us           75
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                    123178 us       125000 us            5
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                           2.54 us         2.53 us       265482
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                           4.91 us         4.83 us       161858
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                           13.8 us         12.0 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                          17.9 us         16.0 us        44800
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                         75.8 us         65.6 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                         455 us          450 us         1493
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                       4433 us         4464 us          154
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                     79218 us        79861 us            9
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                      1.10 us         1.11 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                      2.00 us         1.99 us       344615
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                      4.52 us         4.46 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                     8.90 us         8.89 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                    68.2 us         69.8 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                    846 us          837 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                  9939 us        10000 us           75
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000               135999 us       135417 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                      2.11 us         2.09 us       344615
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                      4.13 us         4.02 us       186667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                      11.8 us         10.8 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                     19.0 us         15.1 us        37333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                    76.0 us         69.8 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                    443 us          439 us         1600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                  3991 us         3997 us          172
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                75677 us        75284 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                     1.22 us         1.23 us       560000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                     1.73 us         1.73 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                     3.11 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                    5.60 us         5.62 us       100000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                   51.1 us         50.2 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                   692 us          698 us         1120
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                                 8949 us         9028 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                              115671 us       117188 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                     2.42 us         2.41 us       298667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                     3.73 us         3.38 us       203636
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                     8.49 us         8.23 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                                    11.2 us         9.24 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                   53.4 us         48.8 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                   464 us          465 us         1545
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                                 6035 us         6138 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                               83649 us        83333 us            9
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/10                                                0.494 us        0.496 us      1544828
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/20                                                0.479 us        0.481 us      1493333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/50                                                0.477 us        0.481 us      1493333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/100                                               0.462 us        0.465 us      1544828
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/1000                                              0.466 us        0.465 us      1544828
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/10000                                             0.482 us        0.465 us      1544828
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10                                        1.54 us         1.53 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/20                                        3.70 us         3.68 us       186667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/50                                        11.5 us         11.4 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/100                                       17.7 us         17.6 us        37333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/1000                                       166 us          165 us         4073
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10000                                     1675 us         1689 us          407
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/10                                                       1.35 us         1.37 us       560000 items_per_second=7.31429M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/20                                                       2.47 us         2.46 us       280000 items_per_second=8.14545M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/50                                                       5.29 us         5.31 us       100000 items_per_second=9.41176M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/100                                                      11.2 us         11.2 us        64000 items_per_second=8.90435M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/1000                                                      223 us          225 us         3200 items_per_second=4.45217M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/10000                                                    7479 us         7465 us           90 items_per_second=1.33953M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/100                                                   162 ns          164 ns      4480000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/1000                                                  190 ns          193 ns      3733333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/10000                                                 675 ns          684 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/100000                                               7499 ns         7499 ns        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                  127 ns          128 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                                 154 ns          153 ns      4480000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                                657 ns          670 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                              5876 ns         5859 ns       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                            0.434 us        0.430 us      1600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                            3.09 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                                 3.80 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                                 110 us          110 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                               5557 us         5469 us          100
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                            217054 us       218750 us            3
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100                                 3.98 us         3.90 us       172308
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                                 115 us          114 us         5600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                               1947 us          628 us         1120
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                             73966 us        48295 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                           12.3 us         12.3 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                           113 us          112 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                         4444 us         4492 us          160
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                      461117 us       460938 us            2
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100                                        0.428 us        0.424 us      1659259
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/1000                                        1.10 us         1.10 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/10000                                       6.15 us         6.14 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100000                                      71.6 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100                                          0.444 us        0.435 us      1544828
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/1000                                          1.14 us         1.15 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/10000                                         6.24 us         6.28 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100000                                        78.1 us         78.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10                                            0.730 us        0.739 us      1120000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/20                                             1.43 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/50                                             2.48 us         2.46 us       280000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100                                            5.40 us         5.47 us       100000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000                                           51.5 us         51.6 us        10000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10000                                           784 us          785 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100000                                         9746 us         9792 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                      178023 us       175781 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10                                             1.41 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/20                                             2.92 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/50                                             8.28 us         7.67 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100                                            17.4 us         15.9 us        56000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000                                           75.9 us         66.3 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10000                                           530 us          502 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100000                                         6064 us         6094 us          100
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000000                                      126357 us       127604 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                       0.726 us        0.732 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                        1.42 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                        3.17 us         3.15 us       213333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                       7.54 us         7.50 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                      59.9 us         60.0 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                      858 us          858 us          747
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                   10280 us        10208 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                 184909 us       183594 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                        1.38 us         1.29 us       640000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                        2.92 us         2.64 us       248889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                        10.2 us         9.07 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                       19.5 us         17.3 us        56000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                      81.6 us         75.0 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                      547 us          547 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                    5879 us         5999 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                 125244 us       125000 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                      0.914 us        0.921 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                       1.29 us         1.29 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                       2.35 us         2.35 us       298667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                      4.17 us         4.14 us       165926
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                     40.9 us         40.8 us        17231
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                     645 us          656 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                                   8562 us         8542 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                                111083 us       111979 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10                                       1.75 us         1.69 us       407273
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/20                                       2.76 us         2.49 us       263529
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/50                                       5.53 us         4.81 us       149333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100                                      10.1 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                     48.9 us         43.9 us        14933
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                     519 us          516 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100000                                   6666 us         6696 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                                 90056 us        91518 us            7
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/10                                                  0.278 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/20                                                  0.278 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/50                                                  0.278 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/100                                                 0.278 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/1000                                                0.278 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/10000                                               0.278 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/10                                          1.23 us         1.23 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/20                                          3.12 us         3.11 us       235789
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/50                                          9.08 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/100                                         16.6 us         16.1 us        40727
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/1000                                         160 us          160 us         4480
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/10000                                       2107 us         2100 us          320
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/10                                                         1.20 us         1.20 us       560000 items_per_second=8.33488M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/20                                                         2.27 us         2.25 us       298667 items_per_second=8.89055M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/50                                                         5.63 us         5.78 us       100000 items_per_second=8.64865M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/100                                                        9.32 us         9.21 us        74667 items_per_second=10.8607M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/1000                                                        106 us          107 us         6400 items_per_second=9.30909M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/10000                                                      2483 us         2486 us          264 items_per_second=4.02286M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/100                                                     156 ns          160 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/1000                                                    167 ns          165 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/10000                                                   230 ns          230 ns      2986667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/100000                                                  346 ns          345 ns      1947826
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100                                                    124 ns          123 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/1000                                                   133 ns          132 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/10000                                                  200 ns          197 ns      3733333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100000                                                 537 ns          530 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/1000                                              0.320 us        0.321 us      2240000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/10000                                              1.31 us         1.28 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                                   33.7 us         33.7 us        21333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                                   683 us          684 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                                11882 us        11719 us           56
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                              214990 us       213542 us            3
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100                                   33.7 us         34.4 us        21333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                                   687 us          680 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                                 4418 us         1709 us          640
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                               63963 us        42969 us           16
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                             13.1 us         13.1 us        56000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                             125 us          126 us         5600
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                           2166 us         2197 us          320
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                         48632 us        47991 us           14
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100                                          0.347 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/1000                                         0.417 us        0.417 us      1723077
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/10000                                        0.823 us        0.816 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100000                                        1.80 us         1.80 us       407273
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100                                            0.371 us        0.368 us      1866667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/1000                                           0.437 us        0.439 us      1600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/10000                                          0.869 us        0.858 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100000                                          3.24 us         3.22 us       213333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                   0.857 us        0.854 us       896000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                    1.64 us         1.64 us       448000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                    3.04 us         3.07 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                   5.89 us         5.86 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                  50.5 us         50.2 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                  684 us          680 us          896
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                7157 us         7254 us          112
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                              76845 us        76389 us            9
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                    1.64 us         1.50 us       448000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                    2.90 us         2.68 us       344615
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                    8.32 us         8.02 us        89600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                   16.4 us         13.8 us        49778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                  66.3 us         65.6 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                  379 us          368 us         1867
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                2849 us         2761 us          249
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                              27282 us        27043 us           26
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10              0.856 us        0.837 us       746667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20               1.68 us         1.67 us       448000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50               3.10 us         3.14 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100              5.98 us         6.00 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000             51.6 us         51.6 us        10000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000             697 us          698 us         1120
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000           7233 us         7254 us          112
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000         77700 us        76389 us            9
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10               1.55 us         1.50 us       448000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20               3.04 us         2.83 us       248889
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50               8.32 us         7.82 us        71937
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100              16.2 us         13.2 us        49778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000             65.2 us         62.8 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000             390 us          388 us         2133
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000           2950 us         2959 us          264
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000         27924 us        27500 us           25
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100                                            158 ns          157 ns      4480000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/1000                                           186 ns          188 ns      4072727
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/10000                                          695 ns          684 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100000                                        7703 ns         7847 ns        89600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                          97.9 ns         97.7 ns      8960000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                          125 ns          126 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                         615 ns          614 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                       5838 ns         5859 ns       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                     0.455 us        0.455 us      1544828
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                     3.25 us         3.22 us       213333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                          3.17 us         3.14 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                          125 us          126 us         5600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                        5656 us         5625 us          100
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                     210610 us       218750 us            3
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                          3.53 us         3.53 us       194783
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                          126 us          126 us         5600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                        1684 us         1506 us          498
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                      70932 us        27644 us           26
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                    12.8 us         12.8 us        56000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                    130 us          131 us         5600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                  5634 us         5625 us          100
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000               509295 us       500000 us            1
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100                                 0.418 us        0.414 us      1659259
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                 1.23 us         1.26 us       560000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                6.42 us         6.42 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100000                               71.3 us         71.1 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100                                   0.446 us        0.445 us      1544828
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/1000                                   1.27 us         1.26 us       560000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/10000                                  6.65 us         6.63 us        89600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100000                                 78.4 us         78.5 us         8960
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                     0.711 us        0.715 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                      1.33 us         1.34 us       560000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                      2.25 us         2.29 us       320000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                     4.76 us         4.71 us       149333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                    44.3 us         44.9 us        16000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                    641 us          642 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                  7158 us         7118 us           90
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                79170 us        78125 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10                                      1.40 us         1.35 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/20                                      3.15 us         3.02 us       263529
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/50                                      6.93 us         6.80 us        89600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100                                     16.7 us         14.0 us        44800
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                    75.6 us         68.4 us        11200
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                    457 us          449 us         1600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                  3883 us         3841 us          179
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                38243 us        38194 us           18
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.705 us        0.698 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.34 us         1.37 us       560000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 2.26 us         2.25 us       298667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                5.03 us         5.16 us       100000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               45.8 us         45.5 us        15448
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               657 us          656 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000             7327 us         7394 us          112
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           79848 us        79861 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.41 us         1.37 us       560000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 2.96 us         2.65 us       224000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 7.39 us         7.11 us       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                19.0 us         14.3 us        44800
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000               78.4 us         76.7 us        11200
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000               449 us          430 us         1600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000             4040 us         3906 us          172
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000           39989 us        39931 us           18
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100                                              174 ns          176 ns      4072727
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/1000                                             196 ns          195 ns      3446154
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/10000                                            335 ns          337 ns      2133333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100000                                           576 ns          572 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100                                             127 ns          126 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/1000                                            192 ns          193 ns      3733333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/10000                                           616 ns          625 ns      1000000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100000                                         6319 ns         6417 ns       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/1000                                       0.769 us        0.785 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/10000                                       2.51 us         2.57 us       280000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                            37.1 us         36.8 us        18667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                           1196 us         1200 us          560
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                         24127 us        24479 us           30
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                       442617 us       445312 us            2
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                            36.7 us         36.8 us        18667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                           1200 us         1196 us          640
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                          7404 us         2757 us          204
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                       117346 us       111979 us            6
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                      18.1 us         18.0 us        37333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                      159 us          160 us         4480
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                    4953 us         5000 us          100
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                 100319 us       102679 us            7
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100                                   0.475 us        0.481 us      1493333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                  0.835 us        0.837 us       746667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                  1.89 us         1.88 us       373333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                 5.13 us         5.16 us       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100                                     0.488 us        0.487 us      1445161
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/1000                                    0.811 us        0.820 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/10000                                    2.10 us         2.10 us       320000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100000                                   8.04 us         8.20 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                        0.941 us        0.942 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                         1.99 us         1.97 us       373333
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                         3.53 us         3.53 us       203636
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                        6.89 us         6.84 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                       55.5 us         56.2 us        10000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                       729 us          739 us         1120
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                     7673 us         7639 us           90
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                   97078 us        97222 us            9
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                         1.70 us         1.61 us       407273
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                         4.04 us         3.81 us       172308
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                         9.46 us         9.42 us        74667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                        18.4 us         16.7 us        44800
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                       85.1 us         69.8 us         8960
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                       459 us          435 us         1545
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                     3051 us         2930 us          224
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                   31936 us        31901 us           24
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.948 us        0.963 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    1.98 us         1.95 us       344615
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    3.51 us         3.61 us       203636
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   6.91 us         6.98 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  55.2 us         54.4 us        11200
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  730 us          715 us          896
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                7664 us         7639 us           90
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              97398 us        98214 us            7
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.70 us         1.61 us       407273
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                    3.76 us         3.75 us       179200
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                    8.98 us         9.00 us        74667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                   17.0 us         15.0 us        40727
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  82.9 us         69.8 us        11200
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  456 us          445 us         1545
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                3085 us         3112 us          236
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              32073 us        31960 us           22
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100                                                 159 ns          160 ns      4480000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/1000                                                188 ns          188 ns      3733333
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/10000                                               650 ns          642 ns      1120000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100000                                             7471 ns         7499 ns        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                                105 ns          105 ns      6400000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                               132 ns          131 ns      5600000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                              600 ns          609 ns      1000000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                            5710 ns         5625 ns       100000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                          0.425 us        0.420 us      1600000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                          2.99 us         3.08 us       248889
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                               3.01 us         3.05 us       235789
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                               102 us          103 us         6400
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                             5284 us         5312 us          100
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                          205390 us       208333 us            3
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                               3.34 us         3.30 us       213333
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                               105 us          105 us         6400
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                             1596 us         1395 us          448
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                           68121 us        28750 us           25
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                         11.3 us         11.5 us        64000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                         110 us          109 us         5600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                       4368 us         4363 us          154
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                    458540 us       453125 us            2
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                      0.402 us        0.408 us      1723077
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                      1.07 us         1.07 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                     5.90 us         5.86 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                    69.4 us         69.8 us         8960
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                        0.421 us        0.414 us      1659259
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                        1.09 us         1.10 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                       6.02 us         6.00 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                      76.3 us         75.0 us         8960
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                          0.801 us        0.802 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                           1.67 us         1.65 us       407273
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                           2.75 us         2.76 us       248889
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                          6.05 us         6.00 us       112000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                         49.4 us         50.0 us        10000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                         688 us          680 us          896
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                       7816 us         7986 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                    109576 us       109375 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10                                           1.48 us         1.48 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/20                                           3.12 us         3.08 us       263529
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/50                                           8.77 us         7.85 us        89600
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100                                          17.5 us         15.3 us        40727
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                         99.2 us         88.9 us         8960
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                         547 us          516 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                       4092 us         4088 us          172
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                     42648 us        42969 us           16
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                     0.799 us        0.802 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                      1.66 us         1.65 us       407273
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                      2.70 us         2.67 us       263529
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                     6.14 us         6.25 us       100000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                    48.8 us         50.0 us        10000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                    691 us          684 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                  7777 us         7812 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000               105745 us       104167 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                      1.37 us         1.37 us       560000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                      3.12 us         2.72 us       235789
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                      7.96 us         6.70 us        74667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                     17.4 us         16.0 us        44800
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                    97.9 us         83.7 us         7467
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                    516 us          488 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                  4116 us         3906 us          172
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                42704 us        42279 us           17
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100                                                   153 ns          150 ns      4072727
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/1000                                                  161 ns          160 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/10000                                                 214 ns          213 ns      3446154
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100000                                                306 ns          307 ns      2240000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100                                                  100 ns          100 ns      7466667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/1000                                                 107 ns          107 ns      6400000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/10000                                                157 ns          153 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100000                                               418 ns          414 ns      1659259
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/1000                                            0.284 us        0.278 us      2357895
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/10000                                            1.00 us         1.00 us       746667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                                 33.6 us         33.8 us        20364
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                 680 us          680 us          896
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                              11873 us        11963 us           64
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                            212945 us       213542 us            3
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                                 34.1 us         33.8 us        20364
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                                 676 us          684 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                               3729 us         1535 us          560
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                             61074 us        59659 us           11
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                           12.1 us         12.0 us        56000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                           114 us          114 us         5600
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                         2043 us         2038 us          345
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                       36541 us        37007 us           19
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100                                        0.332 us        0.330 us      2036364
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                       0.401 us        0.399 us      1723077
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                      0.727 us        0.715 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                      1.44 us         1.43 us       448000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100                                          0.351 us        0.353 us      2036364
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/1000                                         0.411 us        0.408 us      1723077
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/10000                                        0.810 us        0.795 us       746667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100000                                        2.74 us         2.70 us       248889
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                      0.873 us        0.858 us       746667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                       1.65 us         1.65 us       407273
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                       3.06 us         3.00 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                      5.85 us         5.86 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                     50.4 us         50.0 us        10000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                     684 us          670 us         1120
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                                   7156 us         7254 us          112
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                 76761 us        76389 us            9
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                       1.65 us         1.53 us       560000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                       2.89 us         2.70 us       248889
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                       8.25 us         7.53 us        74667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                      16.0 us         13.2 us        49778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                     66.7 us         67.0 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                     378 us          345 us         1948
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                   2893 us         2824 us          249
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                                 29192 us        29375 us           25
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                 0.852 us        0.837 us       746667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                  1.68 us         1.71 us       448000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                  3.09 us         3.07 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                 6.01 us         6.00 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                51.8 us         51.6 us        10000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                738 us          739 us         1120
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              7477 us         7465 us           90
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            78957 us        78125 us            9
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                  1.60 us         1.49 us       746667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                  3.02 us         2.72 us       235789
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                  8.51 us         7.95 us        74667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                 16.4 us         13.3 us        58857
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                66.5 us         61.4 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                396 us          375 us         1792
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000              2953 us         2913 us          236
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            28069 us        28245 us           26
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100                                               159 ns          157 ns      4480000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/1000                                              191 ns          188 ns      3733333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/10000                                             701 ns          698 ns       896000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100000                                           7741 ns         7673 ns        89600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100                                             97.0 ns         96.3 ns      7466667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                             125 ns          126 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                            629 ns          642 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                          5812 ns         5859 ns       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                        0.458 us        0.465 us      1544828
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                        3.27 us         3.21 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                             3.23 us         3.21 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                             128 us          126 us         4978
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                           5742 us         5859 us          112
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                        213560 us       213542 us            3
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100                             3.82 us         3.77 us       194783
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                             130 us          131 us         5600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                           1802 us         1500 us          448
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                         70950 us        28125 us           25
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                       13.5 us         13.7 us        56000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                       136 us          135 us         4978
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                     5897 us         5859 us          112
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                  524112 us       531250 us            1
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100                                    0.441 us        0.443 us      1659259
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/1000                                    1.25 us         1.26 us       560000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/10000                                   6.81 us         6.84 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100000                                  86.8 us         87.2 us         8960
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100                                      0.447 us        0.450 us      1493333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/1000                                      1.38 us         1.38 us       497778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/10000                                     6.71 us         6.70 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100000                                    80.4 us         82.0 us         8960
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10                                        0.712 us        0.698 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/20                                         1.42 us         1.44 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/50                                         2.38 us         2.44 us       320000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100                                        4.74 us         4.75 us       154483
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000                                       44.7 us         44.9 us        16000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10000                                       655 us          656 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100000                                     7166 us         7115 us          112
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                   80684 us        79861 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10                                         1.38 us         1.38 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/20                                         3.07 us         2.79 us       263529
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/50                                         7.28 us         6.98 us       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100                                        17.1 us         14.1 us        49778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000                                       75.9 us         71.1 us        11200
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10000                                       457 us          445 us         1545
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100000                                     3924 us         3815 us          172
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000000                                   39573 us        39931 us           18
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.845 us        0.785 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    1.34 us         1.35 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    2.30 us         2.30 us       298667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   4.94 us         5.00 us       100000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  46.2 us         46.5 us        15448
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  657 us          656 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                7414 us         7394 us          112
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              81864 us        81597 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.39 us         1.35 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                    3.12 us         3.01 us       186667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                    8.71 us         7.53 us       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                   18.2 us         15.3 us        40727
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  76.2 us         73.2 us         8960
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  448 us          405 us         1659
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                4058 us         4015 us          179
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              40492 us        39931 us           18
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100                                                 176 ns          176 ns      3733333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/1000                                                197 ns          195 ns      3446154
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/10000                                               334 ns          330 ns      2036364
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100000                                              574 ns          572 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100                                                126 ns          128 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/1000                                               192 ns          193 ns      3733333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/10000                                              619 ns          614 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100000                                            6347 ns         6417 ns       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/1000                                          0.762 us        0.767 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/10000                                          2.48 us         2.51 us       280000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                               36.1 us         36.1 us        19478
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                              1185 us         1172 us          560
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                            23941 us        23958 us           30
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                          438693 us       437500 us            2
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100                               36.7 us         37.7 us        18667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                              1203 us         1221 us          640
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                             7630 us         3326 us          249
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                          124098 us       122396 us            6
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                         18.5 us         18.4 us        37333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                         160 us          160 us         4480
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                       4976 us         5156 us          100
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                    101169 us       102679 us            7
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100                                      0.474 us        0.465 us      1445161
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/1000                                     0.838 us        0.854 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/10000                                     1.90 us         1.88 us       373333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100000                                    5.12 us         5.16 us       100000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100                                        0.489 us        0.500 us      1000000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/1000                                       0.814 us        0.802 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/10000                                       2.11 us         2.10 us       320000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100000                                      8.10 us         8.20 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                           0.952 us        0.963 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                            2.00 us         1.99 us       344615
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                            3.54 us         3.53 us       194783
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                           6.92 us         6.80 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                          55.3 us         56.2 us        10000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                          729 us          732 us          896
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                        7651 us         7639 us           90
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                      97342 us        98214 us            7
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                            1.67 us         1.43 us       448000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                            3.72 us         3.69 us       194783
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                            8.93 us         8.37 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                           16.6 us         14.8 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                          82.5 us         69.8 us         8960
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                          451 us          410 us         1600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                        3070 us         3112 us          236
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                      32320 us        32552 us           24
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                      0.951 us        0.942 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                       1.98 us         1.95 us       344615
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                       3.52 us         3.53 us       194783
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                      6.94 us         6.80 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                     55.2 us         54.4 us        11200
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                     731 us          732 us          896
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                   7660 us         7465 us           90
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                 97489 us        98214 us            7
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                       1.70 us         1.73 us       497778
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                       4.08 us         3.77 us       186667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                       9.37 us         8.79 us        74667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                      18.4 us         16.3 us        37333
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                     84.0 us         73.2 us         7467
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                     445 us          424 us         1659
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                   3077 us         2979 us          236
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                 31973 us        30506 us           21
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::PickSearch/100                                                    158 ns          157 ns      4072727
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::PickSearch/1000                                                   188 ns          188 ns      3733333
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::PickSearch/10000                                                  649 ns          645 ns       896000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::PickSearch/100000                                                7525 ns         7499 ns        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                   104 ns          105 ns      7466667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                                  130 ns          126 ns      4977778
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                                 600 ns          614 ns      1120000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                               5754 ns         5781 ns       100000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                             0.424 us        0.433 us      1659259
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                             2.97 us         2.98 us       235789
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                                  3.01 us         3.05 us       235789
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                                  104 us          103 us         6400
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                                5279 us         5301 us          112
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                             202506 us       203125 us            3
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100                                  3.34 us         3.37 us       213333
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                                  105 us          107 us         7467
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                                1566 us         1569 us          498
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                              67316 us        27995 us           24
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                            11.4 us         11.5 us        64000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                            109 us          109 us         7467
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                          4361 us         4395 us          160
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                       463525 us       453125 us            2
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100                                         0.403 us        0.408 us      1723077
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/1000                                         1.08 us         1.05 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/10000                                        5.91 us         6.00 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100000                                       69.7 us         69.8 us         8960
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100                                           0.424 us        0.424 us      1659259
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/1000                                           1.09 us         1.10 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/10000                                          6.01 us         6.00 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100000                                         76.5 us         76.7 us        11200
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10                                             0.802 us        0.809 us      1120000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/20                                              1.67 us         1.71 us       448000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/50                                              2.74 us         2.70 us       248889
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100                                             6.04 us         6.00 us       112000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000                                            51.2 us         50.2 us        11200
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10000                                            694 us          698 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100000                                          7795 us         7812 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                       106223 us       106771 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10                                              1.49 us         1.48 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/20                                              3.20 us         3.00 us       224000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/50                                              8.42 us         7.95 us        74667
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100                                             16.7 us         14.9 us        64000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000                                            97.0 us         85.4 us         6400
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10000                                            531 us          531 us         1000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100000                                          4077 us         4015 us          179
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000000                                        42650 us        41992 us           16
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                        0.809 us        0.816 us       746667
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                         1.67 us         1.67 us       448000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                         2.70 us         2.73 us       263529
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                        6.17 us         6.14 us       112000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                       49.0 us         48.7 us        14452
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                       687 us          698 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                     7775 us         7812 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                  104667 us       104911 us            7
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                         1.30 us         1.26 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                         3.05 us         2.98 us       235789
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                         7.85 us         7.50 us        89600
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                        18.2 us         17.0 us        56000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                       98.9 us         79.5 us         7467
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                       523 us          487 us         1445
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                     4126 us         4142 us          166
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                   42692 us        42969 us           16
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::PickSearch/100                                                      153 ns          157 ns      4977778
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::PickSearch/1000                                                     163 ns          164 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::PickSearch/10000                                                    215 ns          220 ns      3200000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::PickSearch/100000                                                   307 ns          300 ns      2240000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100                                                     102 ns          103 ns      6400000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RangeSearch/1000                                                    108 ns          107 ns      6400000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RangeSearch/10000                                                   157 ns          157 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100000                                                  419 ns          426 ns      1723077
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/1000                                               0.285 us        0.283 us      2488889
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/10000                                               1.00 us         1.00 us       746667
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                                    33.3 us         33.0 us        20364
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                                    670 us          680 us          896
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                                 11771 us        11719 us           64
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                               212691 us       213542 us            3
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100                                    33.4 us         33.7 us        21333
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                                    666 us          663 us          896
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                                  3692 us         1676 us          373
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                                60729 us        61080 us           11
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                              12.1 us         12.0 us        56000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                              115 us          114 us         5600
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                            2035 us         2038 us          345
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                          36976 us        36184 us           19
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100                                           0.333 us        0.337 us      2133333
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/1000                                          0.392 us        0.392 us      1792000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/10000                                         0.732 us        0.732 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100000                                         1.44 us         1.43 us       448000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100                                             0.350 us        0.353 us      1947826
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/1000                                            0.411 us        0.408 us      1723077
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/10000                                           0.807 us        0.802 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100000                                           2.75 us         2.79 us       263529
`;