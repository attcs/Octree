var BENCHMARK_DATA = `----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                                                                                  Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmarks::Base::GetNodeID/1000                                                                                       14089 ns        14125 ns        49778 items_per_second=70.7954M/s
Benchmarks::Base::GetDepthID/1000                                                                                        884 ns          879 ns       746667 items_per_second=1.13778G/s
Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000                                                                10514 ns        10498 ns        64000 items_per_second=95.2558M/s
Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000                                                                  12115 ns        11998 ns        56000 items_per_second=83.3488M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                0.744 us        0.732 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                 1.76 us         1.76 us       373333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                 2.18 us         2.20 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                3.67 us         3.68 us       186667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                               39.3 us         39.0 us        17231
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                              1023 us         1025 us          640
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                            12558 us        12277 us           56
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                          154689 us       153125 us            5
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                 1.43 us         1.40 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                 3.98 us         3.61 us       194783
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                 8.71 us         8.02 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                13.8 us         12.6 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                               64.0 us         61.4 us        11200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                               681 us          670 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                             8940 us         8958 us           75
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                          119806 us       119792 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10           0.710 us        0.711 us      1120000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20            1.67 us         1.69 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50            2.17 us         2.20 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100           4.37 us         4.25 us       154483
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000          51.8 us         53.1 us        10000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000          955 us          983 us          747
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000       14289 us        14375 us           50
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000     174904 us       175781 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10            1.42 us         1.41 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20            3.43 us         3.14 us       248889
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50            8.37 us         8.02 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100           16.6 us         13.4 us        56000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000          74.3 us         68.0 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000          714 us          698 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000       10509 us        10208 us           75
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000     126802 us       125000 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                0.845 us        0.837 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                 2.67 us         2.67 us       263529
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                 4.88 us         4.87 us       144516
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                7.85 us         7.85 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                               89.4 us         90.0 us         7467
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                              1271 us         1283 us          560
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                            34286 us        34375 us           20
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                          195992 us       195312 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/10                                 1.60 us         1.41 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/20                                 4.95 us         4.85 us       154483
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/50                                 10.7 us         9.77 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/100                                17.7 us         15.7 us        49778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/1000                               96.0 us         92.1 us         7467
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/10000                               668 us          684 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/100000                            27119 us        26786 us           28
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                           94333 us        93750 us            7
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                          0.775 us        0.767 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                           1.09 us         1.07 us       640000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                           1.91 us         1.93 us       373333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                          3.26 us         3.30 us       213333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                         33.8 us         33.7 us        21333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                         505 us          502 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                       5795 us         5859 us          112
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                     74590 us        73661 us            7
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                           1.36 us         1.31 us       560000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                           2.04 us         1.88 us       373333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                           3.98 us         3.68 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                          7.38 us         6.42 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                         35.2 us         31.1 us        23579
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                         364 us          345 us         2036
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                       4155 us         4142 us          166
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                     54106 us        53977 us           11
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10                              1.24 us         1.23 us       560000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/20                              3.47 us         3.45 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/50                              6.93 us         6.98 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/100                             15.4 us         15.3 us        44800
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                             159 us          160 us         4480
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                           2583 us         2567 us          280
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/10                                     2.17 us         2.14 us       298667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/20                                     3.68 us         3.61 us       194783
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/50                                     9.26 us         9.00 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/100                                    16.9 us         17.1 us        44800
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/1000                                    195 us          195 us         3446
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/10000                                  3703 us         3686 us          195
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/10                                       2.98 us         2.98 us       235789
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/20                                       7.53 us         7.50 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/50                                       19.0 us         18.8 us        37333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/100                                      44.2 us         44.9 us        16000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/1000                                      644 us          642 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/10000                                   10238 us        10254 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/10                                             1.72 us         1.73 us       407273 items_per_second=5.79233M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/20                                             3.31 us         3.30 us       213333 items_per_second=6.06814M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/50                                             8.32 us         8.16 us        74667 items_per_second=6.12652M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/100                                            21.8 us         21.8 us        34462 items_per_second=4.59493M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/1000                                            685 us          688 us         1000 items_per_second=1.45455M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/10000                                         10212 us        10156 us           60 items_per_second=984.615k/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                        159 ns          157 ns      4480000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                       249 ns          251 ns      2800000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                      413 ns          408 ns      1723077
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                     866 ns          872 ns       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/100                              0.910 us        0.900 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/1000                              1.44 us         1.44 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/10000                             2.60 us         2.55 us       263529
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/100000                            3.97 us         3.99 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                           4.35 us         4.39 us       160000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                          18.5 us         18.4 us        40727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                   1.52 us         1.50 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                  10.5 us         10.5 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                     0.637 us        0.628 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                      1.70 us         1.69 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                      2.23 us         2.25 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                     3.84 us         3.85 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                    42.1 us         42.0 us        16725
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                    805 us          802 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                 13618 us        13438 us           50
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                               210132 us       208333 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                      1.42 us         1.35 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                      3.94 us         3.90 us       172308
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                      8.52 us         7.66 us       100000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                     13.5 us         12.5 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                    66.3 us         62.8 us         8960
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                    582 us          572 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                  9276 us         9375 us           75
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                               160031 us       156250 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.632 us        0.642 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.69 us         1.69 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 2.21 us         2.25 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                4.55 us         4.55 us       154483
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               54.0 us         54.4 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               882 us          879 us          747
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            15232 us        14931 us           45
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          230435 us       229167 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.46 us         1.45 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.95 us         3.66 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 8.60 us         7.32 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                17.2 us         14.6 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000               72.5 us         69.8 us         8960
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000               598 us          572 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000            10618 us        10625 us           75
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          169432 us       171875 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                     0.777 us        0.785 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                      2.85 us         2.83 us       248889
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                      5.36 us         5.30 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                     8.58 us         8.58 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                                    97.1 us         96.3 us         7467
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                                   1195 us         1221 us          640
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                                 38581 us        37829 us           19
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                               243515 us       244792 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10                                      1.41 us         1.38 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/20                                      5.03 us         4.92 us       149333
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/50                                      10.9 us         11.0 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100                                     18.2 us         15.0 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000                                     104 us          105 us         7467
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10000                                    764 us          750 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100000                                 31265 us        31901 us           24
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                               151153 us       150000 us            5
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                               0.800 us        0.809 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                1.14 us         1.15 us       640000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                2.02 us         2.04 us       344615
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                               3.50 us         3.53 us       203636
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                              36.6 us         36.9 us        19478
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                              541 us          530 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                            6613 us         6557 us          112
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                          97794 us        98214 us            7
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                1.67 us         1.60 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                2.48 us         2.35 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                4.94 us         4.45 us       154483
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                               9.95 us         9.24 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                              47.2 us         40.8 us        17231
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                              430 us          420 us         1600
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                            5211 us         5312 us          100
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                          77894 us        78125 us            9
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                   1.06 us         1.07 us       640000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                   3.32 us         3.37 us       213333
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                   6.57 us         6.56 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/100                                  15.3 us         15.3 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                                  170 us          173 us         4073
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                                2604 us         2604 us          264
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/10                                          2.32 us         2.29 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/20                                          4.08 us         4.08 us       172308
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/50                                          10.4 us         10.5 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/100                                         19.5 us         19.5 us        34462
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/1000                                         291 us          285 us         2358
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/10000                                       4853 us         4870 us          154
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/10                                            2.29 us         2.25 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/20                                            6.22 us         6.14 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/50                                            16.5 us         16.4 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/100                                           38.2 us         38.5 us        18667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/1000                                           494 us          497 us         1445
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/10000                                         7777 us         7812 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/10                                                  1.63 us         1.61 us       407273 items_per_second=6.20606M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/20                                                  3.30 us         3.28 us       224000 items_per_second=6.10043M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/50                                                  8.16 us         8.16 us        74667 items_per_second=6.12652M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/100                                                 20.0 us         19.7 us        37333 items_per_second=5.08364M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/1000                                                 643 us          642 us         1120 items_per_second=1.55826M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/10000                                              11204 us        11498 us           53 items_per_second=869.744k/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                             139 ns          138 ns      4977778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                            197 ns          197 ns      3733333
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                           308 ns          307 ns      2240000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                          598 ns          586 ns      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100                                   0.860 us        0.872 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/1000                                   1.29 us         1.31 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/10000                                  2.18 us         2.15 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100000                                 3.57 us         3.53 us       194783
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                                3.70 us         3.77 us       194783
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                               15.5 us         15.3 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                        1.32 us         1.34 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                       9.54 us         9.42 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                      0.724 us        0.715 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                       1.87 us         1.88 us       373333
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                       2.40 us         2.41 us       298667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                      4.08 us         4.08 us       172308
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                     40.8 us         40.8 us        17231
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                     671 us          663 us          896
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                   8485 us         8333 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                103904 us       104911 us            7
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                       1.41 us         1.40 us       448000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                       3.84 us         3.66 us       213333
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                       8.79 us         8.58 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                      15.3 us         13.3 us        44800
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                     83.5 us         79.5 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                     556 us          502 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                   5123 us         5156 us          100
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                 53852 us        53977 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                 0.729 us        0.732 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                  1.93 us         1.97 us       373333
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                  2.48 us         2.51 us       280000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                 4.28 us         4.24 us       165926
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                39.4 us         40.1 us        17920
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                638 us          642 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              8382 us         8333 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           104091 us       104167 us            6
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                  1.49 us         1.48 us       497778
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                  3.97 us         3.77 us       186667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                  9.24 us         8.37 us        89600
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                 16.2 us         15.0 us        40727
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                84.6 us         82.0 us         8960
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                526 us          523 us         1493
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000              5168 us         5022 us          112
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            51453 us        51562 us           10
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                      0.853 us        0.854 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                       2.81 us         2.83 us       248889
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                       5.40 us         5.31 us       100000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                      8.76 us         8.79 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                                     87.9 us         87.9 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                                     950 us          962 us          747
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                                  16220 us        15972 us           45
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                                151517 us       153125 us            5
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10                                       1.53 us         1.50 us       448000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/20                                       4.65 us         4.55 us       154483
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/50                                       10.3 us         10.0 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100                                      19.7 us         17.6 us        40727
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000                                      118 us          107 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10000                                     608 us          600 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100000                                   9362 us         9167 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                                 55784 us        55556 us            9
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                              124 ns          126 ns      5600000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                             163 ns          164 ns      4480000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                            246 ns          246 ns      2800000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                           464 ns          471 ns      1493333
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100                                    0.831 us        0.837 us       746667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/1000                                    1.18 us         1.17 us       640000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/10000                                   1.80 us         1.84 us       407273
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100000                                  2.94 us         2.95 us       248889
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                                 2.88 us         2.95 us       248889
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                                11.1 us         11.3 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                         1.11 us         1.13 us       746667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                        7.08 us         6.98 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                   0.751 us        0.732 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                    1.66 us         1.61 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                    2.17 us         2.15 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                   3.64 us         3.61 us       194783
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                  38.4 us         38.5 us        18667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                  707 us          711 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                               11590 us        11475 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                             169643 us       171875 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                    1.51 us         1.36 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                    3.75 us         3.58 us       235789
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                    9.04 us         7.53 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                   14.1 us         11.7 us        56000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                  65.7 us         64.2 us        11200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                  536 us          516 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                8593 us         8542 us           75
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                             132200 us       130208 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10              0.773 us        0.781 us      1120000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20               1.69 us         1.69 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50               2.19 us         2.20 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100              4.31 us         4.30 us       160000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000             51.5 us         51.6 us        11200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000             842 us          854 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000          13745 us        13750 us           50
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000        191085 us       191406 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10               1.52 us         1.50 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20               4.03 us         4.08 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50               8.47 us         7.95 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100              16.4 us         13.8 us        49778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000             72.4 us         66.3 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000             580 us          558 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000          10273 us        10010 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000        142756 us       140625 us            5
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/10                                   0.873 us        0.858 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/20                                    2.70 us         2.64 us       248889
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/50                                    4.88 us         4.87 us       144516
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/100                                   7.74 us         7.67 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/1000                                  87.6 us         88.9 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/10000                                 1009 us         1004 us          747
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/100000                               31225 us        31250 us           24
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/1000000                             180057 us       179688 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/10                                    1.61 us         1.53 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/20                                    4.93 us         4.76 us       144516
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/50                                    11.0 us         10.5 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/100                                   17.7 us         12.2 us        49778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/1000                                  96.2 us         92.1 us         7467
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/10000                                  663 us          656 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/100000                               25721 us        25240 us           26
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/1000000                             113883 us       114583 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                             0.834 us        0.837 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                              1.16 us         1.17 us       560000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                              2.01 us         2.00 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                             3.46 us         3.45 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                            35.5 us         35.2 us        18667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                            518 us          516 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                          5980 us         5999 us          112
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                        79889 us        79861 us            9
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                              1.45 us         1.41 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                              2.15 us         2.09 us       344615
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                              4.26 us         3.08 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                             6.74 us         5.93 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                            37.8 us         34.5 us        19478
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                            391 us          392 us         1792
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                          4293 us         4236 us          166
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                        60899 us        61080 us           11
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10                                 1.50 us         1.46 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/20                                 3.89 us         3.92 us       179200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/50                                 7.56 us         7.50 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/100                                16.3 us         16.4 us        44800
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/1000                                170 us          169 us         4073
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10000                              2558 us         2567 us          280
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/10                                        2.42 us         2.46 us       298667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/20                                        4.05 us         4.10 us       179200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/50                                        9.82 us         9.84 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/100                                       17.8 us         18.0 us        40727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/1000                                       197 us          195 us         3446
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/10000                                     3591 us         3593 us          187
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/10                                          3.25 us         3.22 us       213333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/20                                          8.42 us         8.02 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/50                                          19.8 us         19.9 us        34462
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/100                                         45.1 us         44.5 us        15448
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/1000                                         650 us          656 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/10000                                      10329 us        10254 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/10                                                1.65 us         1.64 us       448000 items_per_second=6.10043M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/20                                                3.20 us         3.21 us       224000 items_per_second=6.23304M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/50                                                8.70 us         8.72 us        89600 items_per_second=5.7344M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/100                                               22.0 us         21.8 us        28000 items_per_second=4.59487M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/1000                                               606 us          609 us         1000 items_per_second=1.64103M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/10000                                            13554 us        13542 us           45 items_per_second=738.462k/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/100                                           161 ns          164 ns      4480000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                          253 ns          251 ns      2800000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                         421 ns          424 ns      1659259
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                        871 ns          858 ns       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/100                                 0.910 us        0.921 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/1000                                 1.44 us         1.44 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/10000                                2.62 us         2.61 us       263529
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/100000                               3.98 us         3.99 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                              4.37 us         4.43 us       165926
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                             18.3 us         18.4 us        40727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                      1.52 us         1.50 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                     10.9 us         10.9 us        56000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                        0.675 us        0.684 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                         1.64 us         1.67 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                         2.19 us         2.20 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                        3.76 us         3.75 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                       41.5 us         41.7 us        17231
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                       803 us          802 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                    13596 us        13438 us           50
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                  223117 us       223958 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                         1.47 us         1.32 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                         4.04 us         4.01 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                         8.39 us         7.74 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                        13.3 us         12.2 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                       67.5 us         61.0 us         8960
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                       605 us          562 us         1000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                     9554 us         9722 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                  170849 us       171875 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.680 us        0.684 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    1.65 us         1.61 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    2.23 us         2.25 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   4.46 us         4.35 us       154483
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  52.4 us         53.1 us        10000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  873 us          879 us          747
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               15059 us        14931 us           45
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000             250799 us       250000 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.42 us         1.32 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                    3.81 us         3.70 us       224000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                    8.43 us         8.54 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                   16.5 us         14.3 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  74.4 us         64.5 us         8960
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  619 us          614 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000               10874 us        10986 us           64
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             183587 us       183594 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10                                        0.820 us        0.837 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/20                                         2.89 us         2.85 us       235789
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/50                                         5.35 us         5.31 us       100000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100                                        8.50 us         8.54 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000                                       96.8 us         96.3 us         7467
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10000                                      1156 us         1144 us          560
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100000                                    38465 us        38194 us           18
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000000                                  251205 us       250000 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10                                         1.54 us         1.43 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/20                                         5.12 us         5.31 us       100000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/50                                         11.3 us         10.7 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100                                        18.5 us         15.0 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000                                        106 us          103 us         7467
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10000                                       787 us          785 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100000                                    31082 us        31250 us           22
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000000                                  166095 us       164062 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                  0.862 us        0.854 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                   1.20 us         1.20 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                   2.13 us         2.13 us       344615
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                  3.71 us         3.68 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                 38.5 us         39.3 us        18667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                 564 us          558 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                               6787 us         6836 us          112
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                            101523 us       102679 us            7
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                   1.79 us         1.76 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                   2.63 us         2.51 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                   5.50 us         5.16 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                                  9.57 us         8.79 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                 49.2 us         40.0 us        16000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                 456 us          435 us         1545
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                               5469 us         5580 us          112
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                             83899 us        85069 us            9
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10                                      1.32 us         1.31 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/20                                      3.71 us         3.68 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/50                                      7.18 us         7.15 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/100                                     16.4 us         16.1 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/1000                                     179 us          176 us         4073
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10000                                   2629 us         2573 us          249
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/10                                             2.65 us         2.64 us       248889
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/20                                             4.58 us         4.55 us       154483
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/50                                             11.2 us         11.2 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/100                                            20.7 us         20.9 us        34462
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/1000                                            297 us          298 us         2358
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/10000                                          4698 us         4741 us          145
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/10                                               2.61 us         2.62 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/20                                               6.69 us         6.70 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/50                                               17.4 us         17.6 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/100                                              39.6 us         39.2 us        17920
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/1000                                              504 us          500 us         1000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/10000                                            7841 us         7812 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/10                                                     1.49 us         1.51 us       497778 items_per_second=6.63704M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/20                                                     2.94 us         2.98 us       235789 items_per_second=6.70689M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/50                                                     8.37 us         8.37 us        89600 items_per_second=5.97333M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/100                                                    22.2 us         22.0 us        32000 items_per_second=4.55111M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/1000                                                    582 us          594 us         1000 items_per_second=1.68421M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/10000                                                 12354 us        12370 us           48 items_per_second=808.421k/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                140 ns          138 ns      4977778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                               198 ns          201 ns      3733333
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                              306 ns          307 ns      2240000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                             597 ns          600 ns      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100                                      0.862 us        0.872 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/1000                                      1.29 us         1.28 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/10000                                     2.17 us         2.20 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100000                                    3.59 us         3.61 us       194783
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                                   3.70 us         3.68 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                                  15.6 us         15.7 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                           1.33 us         1.35 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                          9.42 us         9.21 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                         0.728 us        0.739 us      1120000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                          1.88 us         1.88 us       373333
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                          2.39 us         2.40 us       280000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                         4.07 us         3.99 us       172308
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                        40.8 us         41.7 us        17231
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                        663 us          645 us          896
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                      8281 us         8333 us           90
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                   104543 us       106771 us            6
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                          1.42 us         1.35 us       497778
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                          4.01 us         3.84 us       179200
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                          9.28 us         9.21 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                         15.0 us         13.3 us        44800
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                        84.6 us         76.7 us         8960
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                        555 us          516 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                      5084 us         5156 us          100
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                    50911 us        51562 us           10
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    0.728 us        0.725 us      1120000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                     1.93 us         1.93 us       373333
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                     2.49 us         2.46 us       280000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                    4.28 us         4.24 us       165926
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                   39.4 us         39.2 us        17920
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   634 us          642 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                 8417 us         8333 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              104321 us       104911 us            7
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                     1.42 us         1.42 us       560000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                     4.16 us         4.19 us       186667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                     8.92 us         8.58 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                    14.5 us         13.0 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                   87.0 us         77.4 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                   530 us          516 us         1000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                 5064 us         5022 us          112
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000               52873 us        52557 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10                                         0.847 us        0.854 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/20                                          2.81 us         2.76 us       248889
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/50                                          5.39 us         5.44 us       112000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100                                         8.79 us         8.89 us        89600
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000                                        87.5 us         87.9 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10000                                        950 us          941 us          747
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100000                                     15848 us        15625 us           41
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000000                                   147081 us       148438 us            4
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10                                          1.58 us         1.53 us       407273
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/20                                          4.82 us         4.35 us       154483
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/50                                          10.3 us         9.84 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100                                         20.1 us         16.7 us        37333
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000                                         122 us          112 us         6400
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10000                                        623 us          578 us         1000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100000                                      9664 us         9583 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000000                                    55898 us        55398 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                 124 ns          123 ns      5600000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                                164 ns          164 ns      4480000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                               246 ns          246 ns      2800000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                              465 ns          465 ns      1445161
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100                                       0.830 us        0.820 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/1000                                       1.20 us         1.20 us       560000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/10000                                      1.80 us         1.80 us       407273
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100000                                     2.95 us         2.92 us       235789
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                                    2.89 us         2.92 us       235789
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                                   11.1 us         11.2 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                            1.12 us         1.15 us       640000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                           7.10 us         7.11 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                  0.906 us        0.900 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                   1.74 us         1.76 us       373333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                   3.19 us         3.22 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                  6.96 us         6.80 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                 61.4 us         60.0 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                 871 us          879 us          747
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                              11404 us        11230 us           64
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                            149956 us       153125 us            5
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                   1.63 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                   3.77 us         3.53 us       194783
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                   13.5 us         12.7 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                  16.9 us         12.9 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                 70.7 us         61.4 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                 485 us          465 us         1445
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                               6389 us         6417 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                             87559 us        87054 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10             0.889 us        0.879 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20              1.69 us         1.71 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50              3.72 us         3.77 us       194783
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100             8.84 us         8.79 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000            70.6 us         71.1 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000            976 us          962 us          747
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000         12491 us        12556 us           56
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000       161836 us       160156 us            4
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10              1.72 us         1.57 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20              3.13 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50              9.50 us         8.58 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100             18.1 us         14.2 us        56000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000            77.6 us         69.8 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000            515 us          531 us         1000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000          6538 us         6417 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000        94949 us        93750 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                            0.935 us        0.921 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                             1.39 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                             2.65 us         2.67 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                            4.84 us         4.88 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                           47.3 us         47.1 us        14933
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                           692 us          698 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                         7975 us         7917 us           75
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                       86746 us        87054 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                             1.63 us         1.54 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                             2.77 us         2.67 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                             5.67 us         4.63 us       161858
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                            8.74 us         6.28 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                           37.3 us         33.8 us        20364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                           433 us          436 us         1792
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                         5069 us         5156 us          100
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                       54017 us        54688 us           10
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/10                                        0.342 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/20                                        0.341 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/50                                        0.342 us        0.337 us      1947826
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/100                                       0.341 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/1000                                      0.342 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/10000                                     0.342 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                1.29 us         1.28 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                3.28 us         3.30 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                9.61 us         9.63 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/100                               17.3 us         17.3 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                               176 us          176 us         3733
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                             2337 us         2344 us          320
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/10                                               1.99 us         2.01 us       373333 items_per_second=4.97777M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/20                                               3.63 us         3.61 us       194783 items_per_second=5.54049M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/50                                               9.33 us         9.21 us        74667 items_per_second=5.43033M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/100                                              17.3 us         17.3 us        40727 items_per_second=5.79228M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/1000                                              254 us          251 us         2800 items_per_second=3.98222M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/10000                                            4631 us         4743 us          112 items_per_second=2.10824M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100                                           197 ns          197 ns      3733333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/1000                                          224 ns          225 ns      3200000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/10000                                         314 ns          314 ns      2240000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100000                                        640 ns          642 ns      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                          151 ns          150 ns      4480000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                         170 ns          169 ns      4072727
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                        279 ns          276 ns      2488889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                       906 ns          907 ns       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                    0.431 us        0.430 us      1600000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                    1.20 us         1.20 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                         2.86 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                        29.8 us         29.8 us        23579
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                        782 us          785 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                     19669 us        19761 us           34
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                         3.31 us         3.37 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                        31.5 us         31.4 us        22400
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                        308 us         83.7 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                      7400 us         2979 us          236
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                   22.3 us         22.5 us        32000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                   197 us          197 us         3733
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                 3919 us         3928 us          179
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000              108363 us       109375 us            6
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100                                0.517 us        0.516 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/1000                               0.690 us        0.698 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/10000                               1.45 us         1.43 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100000                              6.26 us         6.14 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100                                  0.471 us        0.471 us      1493333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/1000                                 0.636 us        0.642 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/10000                                 1.36 us         1.34 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100000                                6.37 us         6.42 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                    0.770 us        0.767 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                     1.44 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                     2.41 us         2.41 us       298667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                    5.34 us         5.31 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                   47.7 us         47.6 us        14452
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                   725 us          711 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                10044 us        10010 us           64
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                              136187 us       134375 us            5
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10                                     1.43 us         1.37 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/20                                     2.91 us         2.79 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/50                                     7.40 us         6.28 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100                                    15.8 us         15.7 us        49778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                   71.5 us         64.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                   478 us          460 us         1493
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                 6199 us         6138 us          112
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                               97434 us        97222 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10               0.758 us        0.750 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                1.43 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                3.08 us         3.05 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100               7.57 us         7.67 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000              58.1 us         58.6 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000              810 us          802 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000           10808 us        10986 us           64
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000         148181 us       148438 us            4
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                1.46 us         1.40 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                3.30 us         2.96 us       280000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                8.52 us         8.30 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100               20.3 us         16.9 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000              75.0 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000              507 us          498 us         1600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000            6502 us         6557 us          112
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          99153 us        98214 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                              0.802 us        0.802 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                               1.13 us         1.15 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                               2.09 us         2.13 us       373333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                              3.63 us         3.61 us       194783
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                             35.7 us         36.1 us        19478
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                             580 us          594 us         1000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                           6863 us         6944 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                         76072 us        76389 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                               1.38 us         1.40 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                               2.15 us         2.04 us       344615
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                               4.18 us         4.01 us       194783
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                              10.6 us         8.37 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                             33.7 us         32.8 us        22400
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                             436 us          433 us         1659
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                           4940 us         5000 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                         55567 us        55398 us           11
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/10                                          0.352 us        0.353 us      1947826
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/20                                          0.350 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/50                                          0.351 us        0.353 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/100                                         0.350 us        0.345 us      1947826
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/1000                                        0.350 us        0.345 us      1947826
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/10000                                       0.349 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/10                                  1.21 us         1.20 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/20                                  3.05 us         3.00 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/50                                  8.82 us         8.79 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/100                                 16.2 us         16.4 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/1000                                 158 us          160 us         4978
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/10000                               2301 us         2295 us          320
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/10                                                 1.79 us         1.80 us       407273 items_per_second=5.54585M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/20                                                 3.29 us         3.28 us       224000 items_per_second=6.10043M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/50                                                 8.58 us         8.58 us        74667 items_per_second=5.82767M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/100                                                16.0 us         15.7 us        40727 items_per_second=6.35739M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/1000                                                202 us          204 us         3446 items_per_second=4.90098M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/10000                                              3600 us         3612 us          186 items_per_second=2.76837M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100                                             238 ns          241 ns      2986667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/1000                                            295 ns          298 ns      2357895
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/10000                                           530 ns          531 ns      1000000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100000                                          946 ns          963 ns       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100                                            195 ns          195 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/1000                                           298 ns          298 ns      2357895
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/10000                                         1032 ns         1046 ns       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100000                                       10422 ns        10254 ns        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/1000                                       1.03 us         1.03 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/10000                                      3.28 us         3.30 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                           61.8 us         61.4 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                          1182 us         1172 us          560
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                        21033 us        20996 us           32
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                      275041 us       273438 us            2
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                           61.7 us         62.8 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                          1190 us         1196 us          640
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                         7350 us         3046 us          236
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                       75041 us        56818 us           11
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                     35.0 us         34.5 us        19478
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                     243 us          240 us         2800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                   5279 us         5301 us          112
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                106270 us       104167 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100                                  0.791 us        0.774 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                  1.18 us         1.17 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                 3.09 us         3.14 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                9.84 us         9.77 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100                                    0.664 us        0.684 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/1000                                    1.01 us         1.00 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/10000                                   2.51 us         2.51 us       280000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100000                                  8.91 us         8.89 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                       0.841 us        0.837 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                        1.70 us         1.73 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                        3.18 us         3.15 us       213333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                       6.98 us         6.98 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                      63.2 us         62.8 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                      931 us          941 us          747
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                   12538 us        12556 us           56
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                 202219 us       203125 us            4
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                        2.10 us         1.95 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                        3.47 us         2.98 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                        12.8 us         11.2 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                       16.8 us         13.7 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                      72.6 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                      552 us          544 us         1120
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                    6949 us         6696 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                 134242 us       132812 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                  0.812 us        0.820 us       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                   1.63 us         1.64 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                   3.77 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                  8.82 us         8.79 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                 70.7 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                1019 us         1025 us          747
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              13418 us        13393 us           56
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            211765 us       218750 us            3
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                   1.42 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                   3.02 us         3.01 us       248889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                   9.12 us         9.21 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                  18.4 us         15.4 us        49778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                 78.1 us         75.0 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                 557 us          562 us         1000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000               7091 us         6944 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            136713 us       137500 us            5
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                                 0.984 us         1.00 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                  1.46 us         1.48 us       497778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                  2.80 us         2.85 us       263529
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                                 5.11 us         5.16 us       100000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                                50.8 us         50.2 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                                739 us          732 us          747
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                              9503 us         9583 us           75
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                           120157 us       119792 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                  1.98 us         1.84 us       373333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                  3.19 us         2.70 us       248889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                  7.27 us         6.98 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                                 10.5 us         8.79 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                                50.3 us         46.0 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                                504 us          500 us         1000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                              6566 us         6417 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                            88345 us        88542 us            9
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/10                                             0.287 us        0.289 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/20                                             0.285 us        0.289 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/50                                             0.288 us        0.289 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/100                                            0.285 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/1000                                           0.285 us        0.289 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/10000                                          0.287 us        0.285 us      2357895
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                     1.12 us         1.12 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                     3.15 us         3.14 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                     9.63 us         9.84 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/100                                    17.6 us         17.6 us        40727
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                                    185 us          188 us         3733
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                                  2454 us         2456 us          299
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/10                                                    1.90 us         1.90 us       344615 items_per_second=5.25128M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/20                                                    3.49 us         3.37 us       194783 items_per_second=5.93624M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/50                                                    8.97 us         9.00 us        74667 items_per_second=5.55661M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/100                                                   16.6 us         16.5 us        40727 items_per_second=6.06169M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/1000                                                   245 us          246 us         2800 items_per_second=4.07273M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/10000                                                 4583 us         4464 us          112 items_per_second=2.24M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100                                                155 ns          153 ns      4480000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/1000                                               171 ns          176 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/10000                                              242 ns          240 ns      2800000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100000                                             557 ns          544 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                               124 ns          122 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                              135 ns          138 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                             215 ns          215 ns      3200000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                            770 ns          785 ns       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                         0.318 us        0.321 us      2240000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                        0.952 us        0.942 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                              3.10 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                             30.0 us         30.5 us        23579
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                             729 us          739 us         1120
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                          19152 us        18842 us           34
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                              3.17 us         3.21 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                             30.3 us         30.5 us        23579
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                             274 us         64.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                           7341 us         3753 us          204
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                        13.7 us         13.5 us        49778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                        126 us          127 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                      2636 us         2663 us          264
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                    87520 us        88542 us            9
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                     0.346 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                    0.451 us        0.455 us      1544828
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                    1.02 us         1.03 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                   5.41 us         5.47 us       100000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                       0.367 us        0.360 us      1866667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                      0.475 us        0.481 us      1493333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                      1.05 us         1.03 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                     5.67 us         5.62 us       100000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                         0.882 us        0.858 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                          1.38 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                          2.40 us         2.40 us       280000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                         5.42 us         5.47 us       100000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                        49.1 us         48.8 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                        794 us          802 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                     11268 us        11230 us           64
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                   198841 us       199219 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10                                          1.73 us         1.67 us       448000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/20                                          2.99 us         2.83 us       248889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/50                                          8.19 us         7.15 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100                                         16.8 us         15.0 us        44800
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                        75.0 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                        534 us          502 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                      6875 us         6696 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                   138772 us       140625 us            5
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    0.682 us        0.670 us      1120000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                     1.38 us         1.36 us       448000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                     3.09 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                    7.57 us         7.50 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                   58.2 us         57.8 us        10000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   848 us          837 us          747
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                11920 us        11998 us           56
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              202623 us       203125 us            3
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                     1.36 us         1.29 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                     3.13 us         2.98 us       235789
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                     8.34 us         7.71 us       111503
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                    19.4 us         16.1 us        40727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                   76.0 us         69.8 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                   538 us          516 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                 6958 us         6944 us           90
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              137916 us       137500 us            5
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                                   0.851 us        0.854 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                    1.21 us         1.22 us       640000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                    2.19 us         2.20 us       320000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                                   3.89 us         3.81 us       172308
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                                  38.7 us         38.7 us        18584
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                                  622 us          628 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                                8377 us         8333 us           90
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                             112237 us       111607 us            7
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                    1.67 us         1.67 us       448000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                    2.60 us         2.51 us       298667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                    5.70 us         4.92 us       149333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                                   10.6 us         8.54 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                                  46.9 us         42.4 us        16593
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                                  497 us          500 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                                6451 us         6278 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                              87266 us        87054 us            7
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/10                                               0.282 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/20                                               0.281 us        0.289 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/50                                               0.281 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/100                                              0.283 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/1000                                             0.282 us        0.285 us      2635294
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/10000                                            0.281 us        0.276 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/10                                       1.01 us         1.00 us       640000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/20                                       2.82 us         2.83 us       248889
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/50                                       8.63 us         8.72 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/100                                      16.3 us         16.4 us        44800
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/1000                                      169 us          165 us         4073
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/10000                                    2395 us         2400 us          280
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/10                                                      1.68 us         1.69 us       407273 items_per_second=5.92397M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/20                                                      3.13 us         3.14 us       224000 items_per_second=6.37156M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/50                                                      8.15 us         8.16 us        74667 items_per_second=6.12652M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/100                                                     15.4 us         15.3 us        44800 items_per_second=6.51636M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/1000                                                     193 us          197 us         3733 items_per_second=5.08323M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/10000                                                   3510 us         3509 us          187 items_per_second=2.84952M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100                                                  156 ns          153 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/1000                                                 168 ns          169 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/10000                                                225 ns          225 ns      2986667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100000                                               312 ns          311 ns      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100                                                 124 ns          123 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/1000                                                134 ns          135 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/10000                                               187 ns          188 ns      3733333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100000                                              462 ns          460 ns      1493333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/1000                                           0.300 us        0.305 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/10000                                          0.927 us        0.942 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                                39.1 us         38.1 us        17231
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                419 us          424 us         1659
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                              5332 us         5301 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                            61659 us        61080 us           11
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                                39.8 us         40.1 us        17920
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                                424 us          424 us         1659
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                              2334 us          750 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                            19177 us        13951 us           56
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                          13.1 us         13.4 us        56000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                          121 us          122 us         6400
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                        1676 us         1651 us          407
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                      30979 us        31250 us           22
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100                                       0.343 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                      0.407 us        0.414 us      1659259
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                     0.735 us        0.732 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                     1.82 us         1.80 us       407273
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100                                         0.367 us        0.368 us      1866667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/1000                                        0.428 us        0.433 us      1659259
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/10000                                       0.748 us        0.732 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100000                                       1.99 us         2.01 us       373333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                     0.957 us        0.963 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                      1.76 us         1.72 us       373333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                      3.20 us         3.21 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                     6.81 us         6.84 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                    60.7 us         61.4 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                    875 us          872 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                                 11544 us        11719 us           56
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                               159466 us       160156 us            4
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                      1.69 us         1.65 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                      3.96 us         3.92 us       179200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                      13.6 us         11.0 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                     16.6 us         14.5 us        56000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                    70.5 us         68.0 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                    506 us          516 us         1000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                  6534 us         6557 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                               100897 us       101562 us            6
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.916 us        0.921 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.69 us         1.65 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 3.65 us         3.69 us       194783
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                8.59 us         8.54 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               70.7 us         71.1 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               996 us         1004 us          747
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            12811 us        12835 us           56
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          173755 us       171875 us            4
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.67 us         1.65 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.31 us         2.71 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                 9.42 us         8.79 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                18.4 us         14.0 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000               78.5 us         75.3 us         7467
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000               541 us          531 us         1000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000             6788 us         6597 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          101586 us       101562 us            6
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                1.00 us         1.00 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                1.47 us         1.48 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                2.78 us         2.83 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                               5.07 us         5.00 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                              49.3 us         50.0 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                              714 us          715 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                            8241 us         8333 us           75
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                          91005 us        90278 us            9
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                1.64 us         1.53 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                3.20 us         3.08 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                5.34 us         4.45 us       154388
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                               9.41 us         6.70 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                              39.6 us         37.6 us        20364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                              460 us          455 us         1545
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                            5283 us         5441 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                          57805 us        56818 us           11
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/10                                           0.337 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/20                                           0.338 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/50                                           0.339 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/100                                          0.339 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/1000                                         0.339 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/10000                                        0.339 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10                                   1.58 us         1.60 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/20                                   3.73 us         3.68 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/50                                   10.3 us         10.3 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/100                                  18.5 us         18.0 us        37333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/1000                                  187 us          184 us         3733
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10000                                2404 us         2400 us          280
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/10                                                  1.42 us         1.41 us       497778 items_per_second=7.07951M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/20                                                  2.47 us         2.46 us       280000 items_per_second=8.14545M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/50                                                  6.49 us         6.45 us        89600 items_per_second=7.74919M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/100                                                 12.0 us         12.0 us        56000 items_per_second=8.33488M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/1000                                                 165 us          165 us         4073 items_per_second=6.06214M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/10000                                               3866 us         3928 us          179 items_per_second=2.54578M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100                                              202 ns          204 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/1000                                             226 ns          225 ns      3200000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/10000                                            315 ns          321 ns      2240000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100000                                           638 ns          628 ns      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100                                             156 ns          153 ns      4480000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                            173 ns          169 ns      4072727
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                           279 ns          276 ns      2488889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                          906 ns          879 ns       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                       0.434 us        0.439 us      1600000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                       1.20 us         1.17 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                            2.84 us         2.78 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                           29.9 us         29.3 us        22400
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                           782 us          785 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                        19626 us        20270 us           37
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100                            3.29 us         3.22 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                           32.0 us         32.1 us        22400
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                           309 us         85.4 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                         7432 us         2715 us          236
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                      22.4 us         22.5 us        32000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                      197 us          195 us         3446
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                    3962 us         3928 us          179
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                 104728 us       106771 us            6
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100                                   0.520 us        0.516 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/1000                                  0.693 us        0.680 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/10000                                  1.45 us         1.48 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100000                                 6.29 us         6.28 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100                                     0.471 us        0.465 us      1445161
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/1000                                    0.637 us        0.628 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/10000                                    1.35 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100000                                   6.36 us         6.42 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10                                       0.801 us        0.795 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/20                                        1.45 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/50                                        2.42 us         2.46 us       298667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100                                       5.22 us         5.31 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000                                      46.8 us         47.1 us        14933
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10000                                      733 us          732 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100000                                   10214 us        10254 us           64
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                 153309 us       153125 us            5
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10                                        1.40 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/20                                        3.07 us         2.70 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/50                                        8.52 us         8.20 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100                                       18.0 us         14.9 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000                                      73.4 us         60.0 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10000                                      491 us          460 us         1493
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100000                                    6539 us         6557 us          112
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000000                                 108960 us       109375 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                  0.804 us        0.802 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                   1.44 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                   3.03 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                  7.34 us         7.15 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                 58.1 us         59.4 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                 828 us          820 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              11203 us        11230 us           64
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            165198 us       164062 us            4
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                   1.63 us         1.57 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                   2.83 us         2.55 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                   8.48 us         7.15 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                  20.1 us         18.4 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                 81.0 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                 547 us          530 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000               6992 us         6975 us          112
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            115108 us       114583 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                 0.875 us        0.889 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                  1.23 us         1.23 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                  2.19 us         2.25 us       320000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                 3.88 us         3.84 us       179200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                37.9 us         37.5 us        17920
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                603 us          600 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                              7112 us         7292 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                            80248 us        80357 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10                                  1.44 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/20                                  2.17 us         1.97 us       373333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/50                                  4.38 us         4.39 us       160000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100                                 11.2 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                35.8 us         34.5 us        20364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                468 us          450 us         1493
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100000                              5213 us         5156 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                            59961 us        59659 us           11
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/10                                             0.339 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/20                                             0.341 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/50                                             0.340 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/100                                            0.340 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/1000                                           0.341 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/10000                                          0.341 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/10                                     1.49 us         1.50 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/20                                     3.46 us         3.45 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/50                                     9.51 us         9.63 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/100                                    17.3 us         17.3 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/1000                                    174 us          176 us         4073
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/10000                                  2360 us         2352 us          299
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/10                                                    1.32 us         1.32 us       497778 items_per_second=7.58519M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/20                                                    2.32 us         2.30 us       298667 items_per_second=8.68849M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/50                                                    6.06 us         5.94 us       100000 items_per_second=8.42105M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/100                                                   11.5 us         11.4 us        56000 items_per_second=8.74146M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/1000                                                   134 us          138 us         4978 items_per_second=7.24073M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/10000                                                 2941 us         2979 us          236 items_per_second=3.35644M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100                                                246 ns          246 ns      2800000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/1000                                               303 ns          311 ns      2357895
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/10000                                              533 ns          531 ns      1000000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100000                                             952 ns          942 ns       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100                                               196 ns          190 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/1000                                              303 ns          293 ns      2240000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/10000                                            1033 ns         1050 ns       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100000                                          10448 ns        10254 ns        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/1000                                          1.05 us         1.06 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/10000                                         3.27 us         3.22 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                              61.9 us         61.4 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                             1180 us         1196 us          640
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                           21005 us        20996 us           32
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                         273388 us       270833 us            3
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100                              62.0 us         61.4 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                             1190 us         1196 us          640
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                            7361 us         3078 us          264
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                          78226 us        60938 us           10
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                        33.1 us         33.5 us        22400
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                        245 us          246 us         2987
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                      5302 us         5469 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                   105221 us       106771 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100                                     0.795 us        0.802 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/1000                                     1.19 us         1.20 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/10000                                    3.06 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100000                                   9.72 us         9.77 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100                                       0.657 us        0.645 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/1000                                       1.01 us         1.00 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/10000                                      2.51 us         2.51 us       280000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100000                                     8.95 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                          0.882 us        0.900 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                           1.73 us         1.69 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                           3.23 us         3.28 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                          7.06 us         6.98 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                         63.8 us         62.8 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                         958 us          962 us          747
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                      12755 us        12812 us           50
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                    215484 us       218750 us            3
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                           1.71 us         1.59 us       560000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                           3.71 us         3.22 us       213333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                           8.61 us         7.95 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                          16.8 us         14.0 us        44800
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                         75.7 us         73.2 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                         571 us          558 us         1120
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                       7205 us         7292 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                    140774 us       136719 us            4
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                     0.853 us        0.854 us       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                      1.66 us         1.69 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                      3.77 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                     8.91 us         9.07 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                    71.3 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   1043 us         1050 us          640
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                 13514 us        13750 us           50
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000               217406 us       213542 us            3
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                      1.58 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                      3.01 us         2.65 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                      9.01 us         8.79 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                     18.6 us         15.1 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                    82.4 us         71.1 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                    577 us          578 us         1000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                  7231 us         7188 us          100
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000               142138 us       144531 us            4
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                     1.04 us         1.05 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                     1.51 us         1.50 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                     2.91 us         2.95 us       248889
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                    5.32 us         5.30 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                   52.4 us         51.6 us        10000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                   761 us          750 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                                 9823 us         9792 us           75
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                              122753 us       122396 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                     2.04 us         1.92 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                     3.42 us         3.14 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                     7.36 us         6.10 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                                    10.7 us         9.21 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                   52.2 us         45.4 us        14452
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                   535 us          531 us         1000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                                 6930 us         6836 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                               89591 us        89286 us            7
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/10                                                0.285 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/20                                                0.284 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/50                                                0.285 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/100                                               0.285 us        0.285 us      2357895
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/1000                                              0.284 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/10000                                             0.285 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10                                        1.41 us         1.40 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/20                                        3.61 us         3.68 us       203636
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/50                                        10.4 us         10.3 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/100                                       18.9 us         18.8 us        37333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/1000                                       194 us          193 us         3733
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10000                                     2519 us         2511 us          280
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/10                                                       1.34 us         1.35 us       497778 items_per_second=7.40879M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/20                                                       2.33 us         2.34 us       320000 items_per_second=8.53333M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/50                                                       6.06 us         6.14 us       112000 items_per_second=8.14545M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/100                                                      11.1 us         11.0 us        64000 items_per_second=9.10222M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/1000                                                      160 us          160 us         4480 items_per_second=6.23304M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/10000                                                    3776 us         3753 us          179 items_per_second=2.66419M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/100                                                   157 ns          160 ns      4480000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/1000                                                  174 ns          173 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/10000                                                 247 ns          246 ns      2800000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/100000                                                556 ns          544 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                  126 ns          126 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                                 137 ns          138 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                                217 ns          215 ns      3200000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                               788 ns          785 ns       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                            0.315 us        0.308 us      2133333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                           0.952 us        0.963 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                                 3.04 us         3.00 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                                29.7 us         29.8 us        23579
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                                729 us          732 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                             19432 us        19301 us           34
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100                                 3.20 us         3.15 us       213333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                                30.1 us         29.8 us        23579
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                                289 us         32.8 us        10000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                              7251 us         3666 us          179
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                           13.2 us         13.4 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                           123 us          123 us         5600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                         2639 us         2623 us          280
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                       87472 us        88542 us            9
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100                                        0.346 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/1000                                       0.453 us        0.455 us      1544828
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/10000                                       1.02 us         1.03 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100000                                      5.40 us         5.31 us       100000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100                                          0.367 us        0.360 us      1866667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/1000                                         0.477 us        0.481 us      1493333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/10000                                         1.05 us         1.05 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100000                                        5.66 us         5.62 us       100000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10                                            0.728 us        0.715 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/20                                             1.43 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/50                                             2.44 us         2.46 us       298667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100                                            5.50 us         5.44 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000                                           50.1 us         50.0 us        10000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10000                                           810 us          820 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100000                                        11441 us        11475 us           64
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                      210501 us       213542 us            3
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10                                             1.39 us         1.26 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/20                                             2.92 us         2.83 us       248889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/50                                             8.86 us         7.11 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100                                            18.1 us         14.2 us        37333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000                                           77.9 us         69.8 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10000                                           552 us          544 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100000                                         7219 us         7254 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000000                                      154004 us       153125 us            5
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                       0.730 us        0.715 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                        1.42 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                        3.13 us         3.14 us       224000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                       7.65 us         7.67 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                      59.1 us         60.0 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                      871 us          872 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                   12137 us        12277 us           56
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                 217842 us       218750 us            3
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                        1.48 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                        3.13 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                        8.34 us         7.67 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                       19.9 us         17.2 us        37333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                      78.9 us         76.7 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                      561 us          562 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                    7233 us         7118 us           90
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                 156212 us       156250 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                      0.918 us        0.942 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                       1.28 us         1.26 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                       2.36 us         2.35 us       298667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                      4.20 us         4.14 us       165926
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                     41.4 us         41.4 us        16593
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                     652 us          642 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                                   8817 us         8958 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                                115638 us       114583 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10                                       1.77 us         1.63 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/20                                       2.80 us         2.57 us       248889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/50                                       5.68 us         5.16 us       100000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100                                      10.6 us         7.74 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                     48.9 us         46.0 us        18667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                     526 us          516 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100000                                   6777 us         6696 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                                 94915 us        95982 us            7
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/10                                                  0.286 us        0.291 us      2635294
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/20                                                  0.287 us        0.289 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/50                                                  0.284 us        0.289 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/100                                                 0.285 us        0.283 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/1000                                                0.285 us        0.285 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/10000                                               0.285 us        0.289 us      2488889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/10                                          1.32 us         1.31 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/20                                          3.26 us         3.28 us       224000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/50                                          9.54 us         9.42 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/100                                         17.3 us         17.3 us        40727
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/1000                                         173 us          173 us         4073
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/10000                                       2448 us         2455 us          280
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/10                                                         1.20 us         1.20 us       560000 items_per_second=8.33488M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/20                                                         2.03 us         2.04 us       344615 items_per_second=9.80238M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/50                                                         5.35 us         5.30 us       112000 items_per_second=9.43158M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/100                                                        10.1 us         10.3 us        74667 items_per_second=9.75242M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/1000                                                        120 us          120 us         5600 items_per_second=8.33488M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/10000                                                      2835 us         2847 us          236 items_per_second=3.51256M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/100                                                     156 ns          157 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/1000                                                    169 ns          165 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/10000                                                   219 ns          225 ns      3200000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/100000                                                  311 ns          314 ns      2240000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100                                                    125 ns          126 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/1000                                                   135 ns          135 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/10000                                                  186 ns          188 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100000                                                 469 ns          455 ns      1544828
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/1000                                              0.299 us        0.305 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/10000                                             0.935 us        0.921 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                                   39.1 us         39.0 us        17231
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                                   422 us          414 us         1659
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                                 5356 us         5469 us          100
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                               61800 us        61080 us           11
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100                                   39.6 us         40.1 us        17920
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                                   419 us          414 us         1659
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                                 2270 us          670 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                               19095 us        14931 us           45
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                             13.3 us         13.2 us        49778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                             124 us          123 us         5600
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                           1702 us         1689 us          407
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                         31454 us        31960 us           22
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100                                          0.348 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/1000                                         0.404 us        0.408 us      1723077
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/10000                                        0.728 us        0.715 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100000                                        1.79 us         1.80 us       407273
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100                                            0.366 us        0.360 us      1866667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/1000                                           0.425 us        0.430 us      1600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/10000                                          0.749 us        0.732 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100000                                          1.99 us         1.95 us       344615
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                   0.851 us        0.837 us       746667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                    1.63 us         1.65 us       407273
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                    3.03 us         3.00 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                   6.07 us         6.00 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                  57.4 us         57.8 us        10000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                  809 us          820 us          896
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                8877 us         8750 us           75
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                              97862 us        98214 us            7
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                    1.56 us         1.51 us       497778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                    2.95 us         2.85 us       280000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                    12.2 us         10.3 us        56000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                   16.3 us         12.6 us        49778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                  73.2 us         68.0 us         8960
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                  476 us          465 us         1545
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                4078 us         4088 us          172
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                              42668 us        43199 us           17
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10              0.826 us        0.820 us       896000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20               1.61 us         1.64 us       448000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50               2.91 us         2.92 us       235789
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100              6.13 us         6.00 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000             58.0 us         58.6 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000             860 us          858 us          747
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000           9264 us         9201 us           90
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000        101473 us       101562 us            6
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10               1.40 us         1.32 us       497778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20               2.96 us         2.92 us       235789
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50               8.39 us         7.94 us        92490
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100              16.6 us         13.2 us        74667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000             78.7 us         69.8 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000             501 us          498 us         1600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000           4261 us         4297 us          160
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000         43726 us        44922 us           16
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100                                            160 ns          160 ns      4480000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/1000                                           173 ns          173 ns      4072727
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/10000                                          250 ns          246 ns      2800000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100000                                         536 ns          531 ns      1000000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                          95.1 ns         94.2 ns      7466667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                          106 ns          103 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                         185 ns          188 ns      3733333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                        681 ns          670 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                     0.317 us        0.314 us      2240000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                    0.899 us        0.889 us       896000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                          2.76 us         2.76 us       248889
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                         28.7 us         27.8 us        23579
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                         744 us          750 us          896
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                      19485 us        19301 us           34
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                          3.09 us         3.14 us       248889
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                         29.4 us         28.9 us        24889
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                         735 us          750 us          896
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                       6136 us         5156 us          100
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                    13.0 us         12.6 us        49778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                    134 us          133 us         4480
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                  3493 us         3446 us          195
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                89370 us        91518 us            7
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100                                 0.342 us        0.338 us      2036364
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                0.521 us        0.531 us      1000000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                1.10 us         1.10 us       640000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100000                               5.23 us         5.16 us       100000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100                                   0.374 us        0.377 us      1866667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/1000                                  0.551 us        0.544 us      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/10000                                  1.18 us         1.20 us       560000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100000                                 5.83 us         5.78 us       100000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                     0.718 us        0.725 us      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                      1.33 us         1.34 us       560000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                      2.27 us         2.25 us       298667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                     4.73 us         4.76 us       144516
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                    43.9 us         44.3 us        16593
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                    680 us          684 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                  7347 us         7254 us          112
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                83493 us        84821 us            7
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10                                      1.39 us         1.38 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/20                                      3.15 us         3.08 us       263529
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/50                                      8.64 us         8.37 us        74667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100                                     16.9 us         15.3 us        89600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                    75.1 us         67.2 us        10000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                    464 us          445 us         1545
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                  3982 us         3997 us          172
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                41873 us        42279 us           17
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.708 us        0.711 us      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.34 us         1.37 us       560000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 2.26 us         2.25 us       298667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                4.93 us         5.08 us       144516
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               45.4 us         45.9 us        16000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               684 us          698 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000             7566 us         7673 us          112
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           85427 us        84821 us            7
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.40 us         1.35 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.11 us         2.96 us       263529
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 7.92 us         6.98 us        89600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                19.4 us         16.0 us        44800
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000               76.7 us         73.2 us         8960
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000               464 us          429 us         1493
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000             4130 us         4142 us          166
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000           44137 us        43945 us           16
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100                                              178 ns          176 ns      3733333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/1000                                             207 ns          209 ns      3446154
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/10000                                            339 ns          338 ns      2036364
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100000                                           542 ns          530 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100                                             120 ns          120 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/1000                                            195 ns          195 ns      3446154
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/10000                                           612 ns          614 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100000                                         6310 ns         6278 ns        89600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/1000                                       0.705 us        0.698 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/10000                                       2.02 us         2.04 us       344615
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                            43.0 us         43.0 us        16000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                            865 us          858 us          747
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                         16831 us        16768 us           41
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                       238289 us       239583 us            3
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                            43.1 us         42.4 us        16593
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                            870 us          872 us          896
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                          6204 us         5781 us          100
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                        65357 us        51562 us           10
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                      16.2 us         16.0 us        44800
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                      162 us          160 us         4480
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                    3880 us         3841 us          179
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                  69899 us        71181 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100                                   0.484 us        0.487 us      1445161
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                  0.821 us        0.816 us       746667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                  1.87 us         1.84 us       373333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                 6.20 us         6.28 us       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100                                     0.500 us        0.500 us      1000000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/1000                                    0.809 us        0.802 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/10000                                    2.00 us         2.04 us       344615
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100000                                   6.71 us         6.63 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                        0.931 us        0.921 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                         1.95 us         1.97 us       373333
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                         3.51 us         3.45 us       203636
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                        7.37 us         7.32 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                       63.1 us         64.2 us        11200
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                       868 us          854 us          896
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                     9487 us         9583 us           75
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                  127104 us       127604 us            6
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                         1.71 us         1.44 us       497778
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                         3.75 us         3.61 us       203636
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                         9.10 us         8.58 us        74667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                        19.3 us         17.3 us        49778
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                       93.6 us         88.9 us         8960
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                       587 us          547 us         1000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                     4371 us         4297 us          160
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                   46846 us        46875 us           15
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.907 us        0.921 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    1.89 us         1.88 us       373333
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    3.32 us         3.30 us       203636
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   7.00 us         6.80 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  60.4 us         60.0 us        11200
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  882 us          879 us          747
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                9662 us         9583 us           75
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000             127504 us       127604 us            6
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.63 us         1.50 us       448000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                    3.15 us         2.93 us       224000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                    8.41 us         7.50 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                   19.7 us         18.0 us        40727
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  91.9 us         73.2 us         7467
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  574 us          531 us         1000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                4357 us         4360 us          172
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              45987 us        46875 us           15
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100                                                 152 ns          150 ns      4480000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/1000                                                164 ns          165 ns      4072727
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/10000                                               237 ns          241 ns      2986667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100000                                              531 ns          531 ns      1000000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                                104 ns          105 ns      7466667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                               110 ns          110 ns      6400000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                              177 ns          184 ns      4072727
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                             680 ns          684 ns      1120000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                          0.280 us        0.278 us      2357895
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                         0.824 us        0.820 us       896000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                               2.65 us         2.61 us       263529
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                              26.8 us         26.7 us        26353
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                              670 us          663 us          896
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                           18401 us        18581 us           37
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                               2.92 us         2.85 us       235789
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                              27.6 us         28.3 us        24889
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                              671 us          663 us          896
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                            5800 us         5000 us          100
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                         12.2 us         12.3 us        56000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                         118 us          120 us         6400
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                       2442 us         2455 us          280
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                     77810 us        76389 us            9
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                      0.335 us        0.338 us      2036364
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                     0.435 us        0.430 us      1600000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                    0.929 us        0.942 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                    4.88 us         4.87 us       144516
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                        0.351 us        0.353 us      2036364
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                       0.450 us        0.455 us      1544828
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                      0.985 us         1.00 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                      5.34 us         5.47 us       100000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                          0.788 us        0.802 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                           1.68 us         1.65 us       407273
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                           2.74 us         2.76 us       248889
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                          6.21 us         6.14 us       112000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                         49.9 us         50.0 us        10000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                         728 us          732 us          896
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                       7973 us         7986 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                    112317 us       111979 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10                                           1.44 us         1.35 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/20                                           3.11 us         3.01 us       248889
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/50                                           8.04 us         7.11 us        74667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100                                          18.5 us         17.6 us        37333
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                         95.9 us         85.1 us        11200
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                         578 us          530 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                       4258 us         4261 us          187
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                     46499 us        45898 us           16
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                     0.786 us        0.785 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                      1.65 us         1.67 us       448000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                      2.67 us         2.67 us       263529
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                     6.04 us         6.09 us       100000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                    48.9 us         47.4 us        11200
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                    713 us          715 us          896
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                  8006 us         7986 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000               111525 us       111979 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                      1.46 us         1.46 us       640000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                      3.12 us         2.98 us       235789
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                      7.70 us         7.32 us        89600
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                     18.4 us         16.9 us        40727
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                    94.4 us         85.4 us         8960
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                    533 us          502 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                  4194 us         3953 us          166
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                46477 us        44922 us           16
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100                                                   154 ns          153 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/1000                                                  162 ns          161 ns      4072727
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/10000                                                 207 ns          209 ns      3446154
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100000                                                271 ns          267 ns      2635294
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100                                                  104 ns          105 ns      7466667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/1000                                                 113 ns          115 ns      6400000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/10000                                                151 ns          151 ns      4977778
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100000                                               377 ns          377 ns      1866667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/1000                                            0.267 us        0.267 us      2635294
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/10000                                           0.754 us        0.750 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                                 39.8 us         39.9 us        17231
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                 416 us          417 us         1723
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                               5228 us         5156 us          100
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                             60579 us        61080 us           11
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                                 40.9 us         40.8 us        17231
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                                 416 us          414 us         1659
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                               1649 us         1604 us          448
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                             16025 us        12764 us           71
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                           11.9 us         12.0 us        64000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                           116 us          114 us         5600
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                         1568 us         1604 us          448
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                       22326 us        22396 us           30
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100                                        0.337 us        0.337 us      2133333
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                       0.383 us        0.384 us      1792000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                      0.652 us        0.656 us      1120000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                      1.41 us         1.41 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100                                          0.352 us        0.353 us      2036364
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/1000                                         0.402 us        0.408 us      1723077
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/10000                                        0.698 us        0.684 us      1120000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100000                                        1.69 us         1.71 us       448000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                       1.03 us         1.03 us       640000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                       1.82 us         1.84 us       407273
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                       3.05 us         3.05 us       235789
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                      6.31 us         6.41 us       100000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                     58.2 us         59.4 us        10000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                     818 us          795 us          747
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                                   8869 us         8958 us           75
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                 97754 us        95982 us            7
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                       1.83 us         1.84 us       373333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                       4.07 us         4.10 us       179200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                       13.6 us         11.7 us        56000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                      16.1 us         14.0 us        56000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                     75.0 us         66.3 us         8960
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                     483 us          485 us         1545
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                   4096 us         4103 us          179
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                                 42001 us        41992 us           16
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                 0.827 us        0.820 us       896000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                  1.60 us         1.57 us       407273
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                  2.90 us         2.95 us       248889
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                 6.10 us         6.14 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                57.9 us         59.4 us        10000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                858 us          858 us          747
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              9263 us         9375 us           75
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           101662 us       101562 us            6
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                  1.40 us         1.32 us       497778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                  2.81 us         2.67 us       263529
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                  10.4 us         8.79 us        64000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                 16.7 us         13.8 us        49778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                79.3 us         71.5 us         8960
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                505 us          488 us         1120
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000              4255 us         4236 us          166
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            44150 us        43750 us           15
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100                                               160 ns          164 ns      4480000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/1000                                              173 ns          173 ns      4072727
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/10000                                             249 ns          251 ns      2800000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100000                                            531 ns          531 ns      1000000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100                                             94.5 ns         94.2 ns      7466667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                             107 ns          107 ns      6400000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                            185 ns          184 ns      3733333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                           677 ns          684 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                        0.315 us        0.318 us      2357895
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                       0.891 us        0.900 us       746667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                             2.75 us         2.70 us       248889
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                            28.6 us         28.5 us        23579
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                            734 us          732 us          896
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                         19381 us        19301 us           34
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100                             3.08 us         3.07 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                            29.4 us         29.2 us        23579
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                            737 us          750 us          896
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                          6138 us         5388 us          145
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                       12.8 us         12.8 us        56000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                       134 us          131 us         5600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                     3552 us         3523 us          204
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                   89354 us        89286 us            7
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100                                    0.340 us        0.338 us      2036364
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/1000                                   0.520 us        0.516 us      1000000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/10000                                   1.10 us         1.12 us       640000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100000                                  5.24 us         5.16 us       100000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100                                      0.372 us        0.377 us      1947826
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/1000                                     0.551 us        0.558 us      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/10000                                     1.18 us         1.14 us       560000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100000                                    5.84 us         6.00 us       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10                                        0.887 us        0.879 us       746667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/20                                         1.51 us         1.50 us       448000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/50                                         2.26 us         2.25 us       320000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100                                        4.74 us         4.81 us       149333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000                                       43.9 us         43.9 us        16000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10000                                       679 us          680 us          896
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100000                                     7344 us         7465 us           90
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                   83366 us        83333 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10                                         2.08 us         1.92 us       407273
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/20                                         4.85 us         4.79 us       160000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/50                                         7.49 us         6.70 us        74667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100                                        17.4 us         14.4 us        64000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000                                       72.4 us         68.0 us         8960
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10000                                       461 us          465 us         1545
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100000                                     3964 us         3928 us          179
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000000                                   42371 us        41360 us           17
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.714 us        0.711 us      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    1.34 us         1.35 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    2.26 us         2.29 us       320000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   4.93 us         4.97 us       144516
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  45.5 us         45.0 us        14933
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  685 us          684 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                7544 us         7639 us           90
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              85458 us        85069 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.40 us         1.32 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                    2.84 us         2.72 us       235789
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                    7.28 us         6.91 us        74667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                   16.5 us         15.0 us        40727
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  77.5 us         71.5 us         8960
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  462 us          455 us         1545
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                4130 us         4142 us          166
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              43881 us        42969 us           16
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100                                                 179 ns          180 ns      3733333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/1000                                                212 ns          213 ns      3446154
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/10000                                               340 ns          345 ns      2036364
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100000                                              542 ns          544 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100                                                122 ns          123 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/1000                                               198 ns          199 ns      3446154
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/10000                                              617 ns          614 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100000                                            6303 ns         6417 ns       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/1000                                          0.708 us        0.698 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/10000                                          2.02 us         1.99 us       344615
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                               43.3 us         43.0 us        16000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                               868 us          872 us          896
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                            16735 us        16768 us           41
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                          237585 us       239583 us            3
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100                               43.5 us         44.3 us        16593
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                               869 us          858 us          747
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                             6128 us         5720 us          112
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                           64522 us        55556 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                         16.9 us         16.9 us        40727
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                         162 us          160 us         4480
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                       3862 us         3841 us          179
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                     68253 us        68182 us           11
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100                                      0.660 us        0.656 us      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/1000                                      1.10 us         1.12 us       640000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/10000                                     2.17 us         2.18 us       344615
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100000                                    6.56 us         6.56 us       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100                                        0.499 us        0.500 us      1000000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/1000                                       0.806 us        0.820 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/10000                                       2.01 us         2.04 us       344615
Benchmarks::StaticBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100000                                      6.74 us         6.84 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                            1.09 us         1.11 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                            2.12 us         2.10 us       320000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                            3.89 us         3.92 us       179200
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                           7.66 us         7.67 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                          64.7 us         65.6 us        11200
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                          872 us          858 us          747
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                        9522 us         9583 us           75
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                     126279 us       125000 us            6
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                            1.93 us         1.63 us       344615
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                            4.29 us         4.20 us       160000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                            9.67 us         9.28 us        64000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                           22.9 us         21.8 us        37333
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                           103 us          100 us         7467
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                          596 us          558 us         1120
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                        4362 us         4330 us          166
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                      47308 us        46875 us           15
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                       1.07 us         1.07 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                       2.05 us         2.09 us       344615
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                       3.68 us         3.60 us       186667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                      7.27 us         7.25 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                     61.9 us         60.0 us        11200
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                     887 us          899 us          747
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                   9656 us         9722 us           90
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                127322 us       127604 us            6
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                       1.97 us         1.92 us       407273
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                       4.36 us         4.10 us       160000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                       12.0 us         11.2 us        64000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                      22.4 us         19.9 us        34462
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                      106 us          100 us         6400
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                     590 us          530 us         1120
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                   4328 us         4395 us          160
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                 46753 us        48295 us           11
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::PickSearch/100                                                    152 ns          153 ns      4480000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::PickSearch/1000                                                   164 ns          167 ns      4480000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::PickSearch/10000                                                  235 ns          235 ns      2986667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::PickSearch/100000                                                 533 ns          547 ns      1000000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                   103 ns          103 ns      6400000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                                  109 ns          107 ns      6400000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                                 175 ns          173 ns      4072727
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                                689 ns          684 ns      1120000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                             0.280 us        0.283 us      2488889
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                            0.820 us        0.820 us       896000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                                  4.02 us         3.99 us       172308
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                                 40.9 us         40.5 us        16593
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                                 720 us          725 us         1120
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                              19149 us        19426 us           37
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100                                  4.35 us         4.30 us       160000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                                 42.1 us         42.4 us        16593
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                                 700 us          698 us         1120
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                               5817 us         4883 us          112
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                            11.8 us         11.7 us        64000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                            118 us          117 us         5600
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                          2445 us         2404 us          299
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                        76223 us        76389 us            9
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100                                         0.336 us        0.338 us      2036364
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/1000                                        0.434 us        0.430 us      1600000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/10000                                       0.930 us        0.928 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100000                                       4.91 us         5.00 us       100000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100                                           0.351 us        0.353 us      1947826
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/1000                                          0.451 us        0.459 us      1600000
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/10000                                         0.983 us         1.00 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100000                                         5.32 us         5.31 us       100000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10                                             0.798 us        0.785 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/20                                              1.68 us         1.65 us       407273
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/50                                              2.76 us         2.76 us       248889
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100                                             6.22 us         6.28 us       112000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000                                            49.7 us         48.8 us        11200
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10000                                            726 us          739 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100000                                          7977 us         7986 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                       111752 us       114583 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10                                              1.45 us         1.41 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/20                                              3.08 us         3.00 us       224000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/50                                              8.17 us         6.45 us        89600
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100                                             18.4 us         14.6 us        40727
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000                                            94.9 us         90.7 us         8960
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10000                                            575 us          558 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100000                                          4264 us         4047 us          166
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000000                                        46615 us        46875 us           15
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                        0.795 us        0.809 us      1120000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                         1.66 us         1.65 us       407273
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                         2.70 us         2.73 us       280000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                        6.05 us         6.09 us       100000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                       49.0 us         47.4 us        11200
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                       712 us          715 us          896
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                     7976 us         7986 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                  111374 us       111979 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                         1.47 us         1.38 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                         3.05 us         2.79 us       224000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                         7.78 us         7.15 us        89600
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                        18.4 us         16.4 us        44800
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                       94.0 us         83.7 us         7467
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                       548 us          508 us         1445
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                     4149 us         4088 us          172
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                   46260 us        45956 us           17
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::PickSearch/100                                                      155 ns          157 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::PickSearch/1000                                                     162 ns          164 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::PickSearch/10000                                                    206 ns          200 ns      3200000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::PickSearch/100000                                                   272 ns          270 ns      2488889
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100                                                     104 ns          107 ns      6400000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RangeSearch/1000                                                    113 ns          115 ns      6400000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RangeSearch/10000                                                   152 ns          150 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100000                                                  377 ns          385 ns      1947826
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/1000                                               0.267 us        0.267 us      2635294
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/10000                                              0.756 us        0.753 us      1120000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                                    39.1 us         39.0 us        17231
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                                    412 us          417 us         1723
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                                  5195 us         5312 us          100
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                                60604 us        59659 us           11
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100                                    39.3 us         40.1 us        17920
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                                    409 us          419 us         1792
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                                  1958 us         1535 us          448
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                                17275 us        13125 us           50
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                              11.0 us         11.0 us        64000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                              113 us          112 us         5600
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                            1564 us         1574 us          407
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                          22588 us        22461 us           32
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100                                           0.333 us        0.330 us      2133333
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/1000                                          0.381 us        0.385 us      1866667
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/10000                                         0.647 us        0.642 us      1120000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100000                                         1.41 us         1.41 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100                                             0.352 us        0.353 us      1947826
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/1000                                            0.401 us        0.410 us      1792000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/10000                                           0.698 us        0.698 us      1120000
Benchmarks::StaticBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100000                                           1.69 us         1.69 us       407273
`;