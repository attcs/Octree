var BENCHMARK_DATA = `---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                                                                                       Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmarks::Base::GetNodeID/1000                                                                                            13901 ns        13811 ns        49778 items_per_second=72.4044M/s
Benchmarks::Base::GetDepthID/1000                                                                                             894 ns          907 ns       896000 items_per_second=1.10277G/s
Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000                                                                     10516 ns        10463 ns        74667 items_per_second=95.5738M/s
Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000                                                                       13277 ns        13114 ns        56000 items_per_second=76.2553M/s
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, SeqExec>/10                                                      0.569 us        0.572 us      1120000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, SeqExec>/20                                                       1.43 us         1.45 us       560000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, SeqExec>/50                                                       3.87 us         3.92 us       179200
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, SeqExec>/100                                                      8.54 us         8.54 us        89600
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, SeqExec>/1000                                                      117 us          117 us         5600
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, SeqExec>/10000                                                    2337 us         2352 us          299
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, SeqExec>/100000                                                  29140 us        29297 us           24
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, SeqExec>/1000000                                                417305 us       414062 us            2
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, ParExec>/10                                                       1.41 us         1.37 us       560000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, ParExec>/20                                                       3.33 us         3.22 us       213333
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, ParExec>/50                                                       12.5 us         9.49 us       112000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, ParExec>/100                                                      20.0 us         17.4 us        44800
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, ParExec>/1000                                                      143 us          119 us         4978
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, ParExec>/10000                                                    1205 us         1099 us          640
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, ParExec>/100000                                                  16333 us        14404 us           64
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create<3, ParExec>/1000000                                                182638 us       143750 us            5
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                                 0.579 us        0.578 us      1000000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                                  1.47 us         1.46 us       448000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                                  3.89 us         3.92 us       179200
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                                 8.56 us         8.58 us        74667
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                                 118 us          120 us         6400
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                               2195 us         2195 us          299
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                             29414 us        28750 us           25
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                           414013 us       414062 us            2
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                                  1.42 us         1.37 us       560000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                                  3.33 us         3.05 us       235789
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                                  12.9 us         10.3 us        56000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                                 19.8 us         16.9 us        40727
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                                 145 us          125 us         6400
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                               1027 us          899 us          747
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                             18059 us        14863 us           41
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                           180546 us       152344 us            4
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::PickSearch/100                                                               153 ns          153 ns      4480000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::PickSearch/1000                                                              181 ns          180 ns      4072727
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::PickSearch/10000                                                             234 ns          230 ns      2986667
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::PickSearch/100000                                                            352 ns          345 ns      1947826
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RangeSearch/100                                                              104 ns          103 ns      6400000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RangeSearch/1000                                                             128 ns          128 ns      5600000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RangeSearch/10000                                                            192 ns          193 ns      3733333
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RangeSearch/100000                                                           456 ns          455 ns      1544828
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::FrustumCulling/1000                                                        0.487 us        0.487 us      1445161
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::FrustumCulling/10000                                                        1.97 us         1.97 us       373333
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection<SeqExec>/100                                             8.46 us         8.54 us        89600
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                             287 us          289 us         2489
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection<SeqExec>/10000                                           8638 us         8542 us           75
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection<SeqExec>/100000                                        234401 us       234375 us            3
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection<ParExec>/100                                             8.89 us         8.72 us        89600
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection<ParExec>/1000                                             288 us          289 us         2489
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection<ParExec>/10000                                           8683 us         8854 us           90
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection<ParExec>/100000                                        233025 us       229167 us            3
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection__WithOtherTree/100                                       8.30 us         8.37 us        89600
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                                      60.3 us         61.4 us        11200
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                                     1862 us         1843 us          373
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                                   28232 us        28125 us           25
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RayIntersectedFirst/100                                                    0.440 us        0.435 us      1544828
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RayIntersectedFirst/1000                                                   0.511 us        0.516 us      1000000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RayIntersectedFirst/10000                                                  0.959 us        0.952 us       640000
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RayIntersectedFirst/100000                                                  1.91 us         1.88 us       373333
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RayIntersectedAll/100                                                      0.396 us        0.390 us      1723077
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RayIntersectedAll/1000                                                     0.453 us        0.450 us      1493333
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RayIntersectedAll/10000                                                    0.892 us        0.900 us       746667
Benchmarks::StaticBVHBox__MBR_WithoutReverseMap::RayIntersectedAll/100000                                                    3.64 us         3.69 us       194783
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                0.693 us        0.680 us       896000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                 1.68 us         1.67 us       448000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                 2.16 us         2.13 us       373333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                3.78 us         3.61 us       194783
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                               39.2 us         39.2 us        17920
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                               706 us          698 us         1120
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                            11113 us        11042 us           75
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                          159700 us       156250 us            4
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                 1.51 us         1.41 us       497778
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                 3.82 us         3.66 us       213333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                 8.46 us         7.67 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                16.5 us         13.7 us        56000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                               64.2 us         62.5 us        10000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                               511 us          500 us         1000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                             6942 us         6975 us          112
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                          123280 us       125000 us            6
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10           0.700 us        0.698 us       896000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20            1.67 us         1.61 us       407273
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50            2.13 us         2.13 us       344615
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100           4.44 us         4.30 us       160000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000          51.9 us         51.6 us        10000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000          810 us          785 us          896
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000       12011 us        11963 us           64
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000     182056 us       183594 us            4
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10            1.53 us         1.50 us       448000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20            4.02 us         3.76 us       203636
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50            9.10 us         8.02 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100           16.8 us         13.5 us        49778
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000          73.1 us         68.0 us         8960
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000          546 us          500 us         1000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000        7760 us         7708 us           75
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000     138798 us       137500 us            5
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                0.857 us        0.858 us       746667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                 2.71 us         2.73 us       263529
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                 4.93 us         5.00 us       100000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                7.84 us         7.85 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                               87.0 us         87.2 us         8960
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                               981 us         1001 us          640
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                            27704 us        27500 us           25
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                          164706 us       164062 us            4
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/10                                 1.59 us         1.54 us       497778
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/20                                 4.29 us         4.27 us       179200
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/50                                 11.0 us         9.52 us        64000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/100                                18.2 us         13.6 us        44800
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/1000                               98.1 us         92.1 us         7467
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/10000                               643 us          641 us         1000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/100000                            23318 us        23438 us           28
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                          109230 us       106771 us            6
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                          0.787 us        0.781 us      1120000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                           1.08 us         1.07 us       640000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                           1.91 us         1.88 us       373333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                          3.29 us         3.22 us       203636
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                         34.2 us         33.7 us        19478
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                         504 us          500 us         1000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                       5767 us         5720 us          112
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                     74578 us        72917 us            9
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                           1.27 us         1.26 us       497778
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                           2.06 us         1.97 us       373333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                           4.13 us         3.92 us       179200
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                          8.40 us         6.98 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                         36.1 us         31.5 us        20364
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                         368 us          369 us         1948
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                       4059 us         4047 us          166
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                     55171 us        55398 us           11
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10                              1.21 us         1.22 us       640000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/20                              3.50 us         3.45 us       203636
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/50                              6.92 us         6.98 us       112000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/100                             15.5 us         15.3 us        44800
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                             164 us          164 us         4480
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                           2464 us         2455 us          280
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/10                                     2.14 us         2.15 us       320000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/20                                     3.63 us         3.69 us       194783
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/50                                     9.22 us         9.42 us        74667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/100                                    16.9 us         17.1 us        44800
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/1000                                    196 us          195 us         3446
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/10000                                  3314 us         3301 us          213
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertUnique/10                                       2.99 us         3.05 us       235789
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertUnique/20                                       7.60 us         7.67 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertUnique/50                                       19.2 us         19.3 us        37333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertUnique/100                                      44.7 us         44.5 us        15448
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertUnique/1000                                      654 us          656 us         1120
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::InsertUnique/10000                                   10375 us        10417 us           75
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Update/10                                             1.72 us         1.73 us       407273 items_per_second=5.79233M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Update/20                                             3.22 us         3.28 us       224000 items_per_second=6.10043M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Update/50                                             7.69 us         7.67 us        89600 items_per_second=6.51636M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Update/100                                            20.6 us         20.4 us        34462 items_per_second=4.90126M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Update/1000                                            665 us          656 us         1120 items_per_second=1.52511M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::Update/10000                                         10768 us        10795 us           55 items_per_second=926.316k/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                        153 ns          150 ns      4480000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                       243 ns          246 ns      2986667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                      428 ns          433 ns      1659259
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                     909 ns          900 ns       746667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/100                              0.915 us        0.921 us       746667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/1000                              1.44 us         1.44 us       497778
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/10000                             2.65 us         2.68 us       280000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/100000                            3.98 us         3.92 us       179200
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                           4.44 us         4.49 us       160000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                          18.5 us         18.4 us        37333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                   1.53 us         1.53 us       448000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                  10.5 us         10.5 us        64000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                   0.754 us        0.750 us       896000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                    1.67 us         1.65 us       407273
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                    2.17 us         2.15 us       320000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                   3.63 us         3.60 us       186667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                  38.4 us         39.3 us        18667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                  707 us          698 us          896
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                               10052 us        10010 us           64
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                             171816 us       171875 us            4
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                    1.53 us         1.45 us       560000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                    4.05 us         3.99 us       172308
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                    8.65 us         7.32 us        74667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                   14.7 us         13.2 us        49778
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                  66.1 us         64.2 us        11200
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                  528 us          502 us         1120
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                6987 us         7118 us           90
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                             133974 us       132812 us            6
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10              0.757 us        0.767 us       896000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20               1.66 us         1.67 us       448000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50               2.20 us         2.15 us       320000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100              4.29 us         4.33 us       165926
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000             50.1 us         50.0 us        10000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000             807 us          802 us          896
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000          11581 us        11475 us           64
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000        188152 us       183594 us            4
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10               1.54 us         1.50 us       448000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20               3.99 us         3.99 us       172308
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50               8.81 us         8.37 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100              18.3 us         13.0 us        40727
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000             72.2 us         68.0 us         8960
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000             571 us          558 us         1120
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000           7849 us         7500 us           75
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000        142008 us       140625 us            5
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/10                                   0.882 us        0.889 us       896000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/20                                    2.74 us         2.79 us       263529
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/50                                    4.94 us         5.00 us       100000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/100                                   7.76 us         7.85 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/1000                                  87.5 us         87.9 us         7467
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/10000                                 1018 us         1025 us          747
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/100000                               26605 us        26442 us           26
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/1000000                             193404 us       191406 us            4
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/10                                    1.67 us         1.63 us       373333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/20                                    5.23 us         5.31 us       100000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/50                                    11.3 us         10.7 us        64000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/100                                   18.6 us         16.3 us        49778
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/1000                                  99.2 us         99.4 us         8960
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/10000                                  671 us          670 us         1120
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/100000                               23929 us        23958 us           30
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/1000000                             122838 us       119792 us            6
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                             0.841 us        0.837 us       746667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                              1.15 us         1.15 us       640000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                              2.02 us         1.99 us       344615
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                             3.52 us         3.53 us       203636
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                            35.9 us         36.1 us        20364
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                            526 us          531 us         1000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                          6034 us         5999 us          112
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                        79758 us        79861 us            9
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                              1.39 us         1.30 us       746667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                              2.12 us         2.04 us       344615
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                              4.48 us         4.30 us       160000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                             9.65 us         7.74 us        74667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                            37.7 us         30.8 us        21333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                            399 us          392 us         1792
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                          4404 us         4160 us          154
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                        62978 us        60764 us            9
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10                                 1.52 us         1.50 us       448000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/20                                 3.82 us         3.85 us       186667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/50                                 7.33 us         7.32 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/100                                16.2 us         16.4 us        44800
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/1000                                172 us          173 us         4073
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10000                              2629 us         2604 us          264
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/10                                        2.50 us         2.51 us       298667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/20                                        4.11 us         4.17 us       172308
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/50                                        10.4 us         10.3 us        64000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/100                                       17.9 us         18.0 us        40727
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/1000                                       205 us          204 us         3446
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/10000                                     3295 us         3301 us          213
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertUnique/10                                          3.28 us         3.30 us       203636
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertUnique/20                                          8.01 us         8.02 us        89600
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertUnique/50                                          19.6 us         19.7 us        37333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertUnique/100                                         45.3 us         44.5 us        15448
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertUnique/1000                                         660 us          656 us         1120
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::InsertUnique/10000                                      10473 us        10498 us           64
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Update/10                                                1.61 us         1.64 us       448000 items_per_second=6.10043M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Update/20                                                3.18 us         3.21 us       224000 items_per_second=6.23304M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Update/50                                                8.90 us         8.89 us        89600 items_per_second=5.62196M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Update/100                                               22.4 us         22.5 us        29867 items_per_second=4.44532M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Update/1000                                               687 us          684 us         1120 items_per_second=1.46286M/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::Update/10000                                            14254 us        14482 us           41 items_per_second=690.526k/s
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::RangeSearch/100                                           153 ns          154 ns      4977778
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                          245 ns          240 ns      2800000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                         423 ns          420 ns      1600000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                        907 ns          900 ns       746667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/100                                 0.938 us        0.900 us       746667
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/1000                                 1.45 us         1.43 us       448000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/10000                                2.67 us         2.67 us       263529
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/100000                               4.01 us         4.05 us       165926
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                              4.46 us         4.45 us       154483
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                             18.5 us         18.8 us        37333
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                      1.53 us         1.53 us       448000
Benchmarks::DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                     10.4 us         10.3 us        74667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                     0.635 us        0.628 us       896000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                      1.62 us         1.61 us       407273
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                      2.19 us         2.20 us       320000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                     3.90 us         3.85 us       186667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                    41.9 us         41.4 us        16593
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                    802 us          802 us          896
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                 13095 us        13114 us           56
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                               218923 us       218750 us            3
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                      1.42 us         1.35 us       497778
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                      3.61 us         3.21 us       194783
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                      9.08 us         8.37 us        89600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                     16.6 us         15.0 us        40727
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                    66.7 us         65.6 us        11200
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                    585 us          586 us         1120
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                  8238 us         8333 us           90
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                               164497 us       164062 us            4
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.631 us        0.641 us      1000000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.63 us         1.64 us       448000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 2.21 us         2.20 us       298667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                4.46 us         4.35 us       154483
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               54.4 us         54.4 us        11200
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               886 us          889 us          896
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            12323 us        12207 us           64
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          239094 us       239583 us            3
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.38 us         1.26 us       560000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.28 us         3.30 us       213333
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 8.35 us         7.50 us        89600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                17.1 us         13.3 us        44800
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000               73.2 us         64.5 us         8960
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000               602 us          600 us         1120
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000             8052 us         7812 us           90
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          175363 us       171875 us            4
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                     0.786 us        0.785 us       896000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                      2.89 us         2.92 us       235789
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                      5.40 us         5.30 us       112000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                     8.63 us         8.58 us        74667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                                    93.8 us         94.2 us         7467
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                                   1115 us         1074 us          640
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                                 32704 us        32812 us           20
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                               238600 us       239583 us            3
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10                                      1.48 us         1.40 us       448000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/20                                      5.48 us         5.41 us       144516
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/50                                      11.7 us         11.0 us        64000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100                                     19.1 us         17.6 us        40727
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000                                     113 us          103 us         5600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10000                                    805 us          732 us          896
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100000                                 25127 us        25240 us           26
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                               151605 us       148438 us            4
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                               0.807 us        0.823 us      1120000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                1.16 us         1.14 us       560000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                2.03 us         2.05 us       320000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                               3.57 us         3.53 us       194783
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                              37.1 us         37.7 us        19478
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                              569 us          558 us         1120
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                            7058 us         7118 us           90
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                         105975 us       104911 us            7
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                1.72 us         1.69 us       407273
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                2.50 us         2.34 us       280000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                5.49 us         4.71 us       149333
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                               10.9 us         8.16 us        74667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                              47.1 us         43.6 us        17920
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                              441 us          420 us         1600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                            5166 us         5312 us          100
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                          80960 us        79861 us            9
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                   1.05 us         1.05 us       746667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                   3.29 us         3.22 us       203636
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                   6.50 us         6.45 us        89600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/100                                  15.1 us         15.0 us        44800
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                                  177 us          173 us         4073
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                                2554 us         2567 us          280
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/10                                          2.27 us         2.20 us       320000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/20                                          3.93 us         3.92 us       179200
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/50                                          10.2 us         10.3 us        74667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/100                                         19.0 us         19.0 us        34462
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/1000                                         289 us          289 us         2489
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/10000                                       4305 us         4297 us          160
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertUnique/10                                            2.32 us         2.35 us       298667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertUnique/20                                            6.28 us         6.28 us       112000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertUnique/50                                            16.4 us         16.1 us        40727
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertUnique/100                                           37.7 us         37.7 us        18667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertUnique/1000                                           492 us          497 us         1445
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::InsertUnique/10000                                         7652 us         7673 us          112
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Update/10                                                  1.65 us         1.67 us       448000 items_per_second=5.97333M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Update/20                                                  3.13 us         3.18 us       235789 items_per_second=6.28771M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Update/50                                                  7.93 us         8.02 us        89600 items_per_second=6.23304M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Update/100                                                 20.3 us         20.0 us        32000 items_per_second=4.99512M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Update/1000                                                 662 us          656 us         1120 items_per_second=1.52511M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::Update/10000                                              12249 us        12087 us           53 items_per_second=827.317k/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::RangeSearch/100                                             139 ns          141 ns      4977778
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                            202 ns          204 ns      3446154
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                           344 ns          345 ns      2036364
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                          701 ns          680 ns       896000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100                                   0.871 us        0.858 us       746667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/1000                                   1.34 us         1.35 us       497778
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/10000                                  2.22 us         2.25 us       320000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100000                                 3.61 us         3.58 us       165926
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                                3.73 us         3.77 us       186667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                               15.8 us         16.0 us        44800
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                        1.37 us         1.37 us       560000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                       9.77 us         9.77 us        64000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                        0.681 us        0.680 us       896000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                         1.72 us         1.73 us       407273
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                         2.22 us         2.25 us       320000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                        3.78 us         3.75 us       179200
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                       41.1 us         41.7 us        17231
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                       807 us          802 us          896
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                    12098 us        12207 us           64
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                  244641 us       244792 us            3
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                         1.49 us         1.46 us       448000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                         3.47 us         3.38 us       203636
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                         8.67 us         7.53 us       112000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                        16.5 us         14.5 us        56000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                       67.9 us         65.6 us        11200
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                       610 us          586 us         1120
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                     7877 us         7986 us           90
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                  198706 us       199219 us            4
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.703 us        0.698 us       896000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    1.68 us         1.65 us       407273
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    2.29 us         2.29 us       320000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   4.48 us         4.45 us       154483
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  53.2 us         53.0 us        11200
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  894 us          907 us          896
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               12951 us        13114 us           56
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000             253598 us       255208 us            3
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.50 us         1.38 us       497778
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                    3.45 us         3.45 us       235789
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                    9.04 us         8.89 us        89600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                   17.1 us         13.8 us        49778
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  75.0 us         71.1 us        11200
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  625 us          610 us          896
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000               11049 us        11042 us           75
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             201487 us       195312 us            4
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10                                        0.837 us        0.837 us       896000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/20                                         2.94 us         2.85 us       235789
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/50                                         5.42 us         5.62 us       100000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100                                        8.84 us         8.79 us        74667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000                                       98.8 us         98.3 us         7467
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10000                                      1248 us         1256 us          560
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100000                                    41678 us        42279 us           17
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000000                                  315474 us       312500 us            2
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/10                                         1.68 us         1.65 us       560000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/20                                         5.31 us         5.16 us       100000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/50                                         11.9 us         11.5 us        64000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/100                                        19.2 us         16.5 us        40727
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000                                        114 us          112 us         6400
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/10000                                       817 us          785 us          896
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/100000                                    36341 us        36184 us           19
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000000                                  170143 us       167969 us            4
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                  0.881 us        0.879 us       746667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                   1.23 us         1.25 us       640000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                   2.20 us         2.20 us       320000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                  3.80 us         3.66 us       179200
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                 39.4 us         39.2 us        17920
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                 578 us          572 us         1120
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                               7276 us         7115 us          112
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                            104333 us       104167 us            6
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                   1.80 us         1.76 us       373333
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                   2.64 us         2.40 us       280000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                   5.70 us         5.62 us       100000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                                  9.91 us         8.89 us        89600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                 51.4 us         47.1 us        14933
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                 471 us          465 us         1545
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                               5583 us         5625 us          100
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                             88392 us        88542 us            9
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10                                      1.31 us         1.32 us       497778
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__RebalanceMode/20                                      3.72 us         3.77 us       194783
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__RebalanceMode/50                                      7.19 us         7.15 us        89600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__RebalanceMode/100                                     16.4 us         16.1 us        40727
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__RebalanceMode/1000                                     178 us          176 us         4073
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10000                                   2668 us         2723 us          264
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertIntoLeaf/10                                             2.64 us         2.61 us       263529
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertIntoLeaf/20                                             4.47 us         4.55 us       154483
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertIntoLeaf/50                                             10.9 us         10.7 us        64000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertIntoLeaf/100                                            20.2 us         20.5 us        37333
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertIntoLeaf/1000                                            300 us          305 us         2358
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertIntoLeaf/10000                                          4420 us         4395 us          160
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertUnique/10                                               2.60 us         2.61 us       263529
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertUnique/20                                               6.73 us         6.63 us        89600
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertUnique/50                                               17.2 us         17.3 us        40727
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertUnique/100                                              40.4 us         40.1 us        17920
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertUnique/1000                                              507 us          500 us         1000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::InsertUnique/10000                                            7807 us         7812 us           90
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Update/10                                                     1.48 us         1.46 us       448000 items_per_second=6.82667M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Update/20                                                     2.92 us         2.92 us       235789 items_per_second=6.85932M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Update/50                                                     7.95 us         8.02 us        89600 items_per_second=6.23304M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Update/100                                                    20.6 us         20.4 us        34462 items_per_second=4.90126M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Update/1000                                                    659 us          670 us         1120 items_per_second=1.49333M/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::Update/10000                                                 14201 us        14172 us           43 items_per_second=705.641k/s
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::RangeSearch/100                                                138 ns          135 ns      4977778
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::RangeSearch/1000                                               202 ns          199 ns      3446154
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::RangeSearch/10000                                              334 ns          330 ns      2036364
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::RangeSearch/100000                                             687 ns          680 ns       896000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors/100                                      0.869 us        0.858 us       746667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors/1000                                      1.30 us         1.31 us       560000
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors/10000                                     2.25 us         2.25 us       298667
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors/100000                                    3.62 us         3.61 us       194783
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                                   3.71 us         3.69 us       194783
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                                  15.7 us         15.7 us        44800
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::FrustumCulling/1000                                           1.38 us         1.38 us       497778
Benchmarks::DynamicOrthoTreePoint__MBR_Regular_WithReverseMap::FrustumCulling/10000                                          10.1 us         9.77 us        64000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                      0.734 us        0.732 us       746667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                       1.88 us         1.84 us       373333
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                       2.37 us         2.34 us       280000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                      4.27 us         4.24 us       165926
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                     40.9 us         40.8 us        17231
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                     659 us          663 us          896
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                   8288 us         8333 us           90
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                105112 us       106771 us            6
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                       1.57 us         1.50 us       448000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                       3.77 us         3.68 us       186667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                       8.31 us         7.85 us        89600
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                      15.0 us         13.8 us        49778
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                     90.3 us         83.7 us         8960
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                     569 us          547 us         1000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                   5109 us         4957 us          145
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                 53216 us        51562 us           10
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                 0.746 us        0.750 us       896000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                  1.93 us         1.90 us       344615
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                  2.48 us         2.46 us       280000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                 4.60 us         4.59 us       160000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                39.1 us         39.2 us        17920
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                630 us          628 us          896
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              8414 us         8507 us           90
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           107227 us       107143 us            7
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                  1.53 us         1.46 us       448000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                  4.35 us         4.19 us       179200
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                  8.54 us         7.11 us        74667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                 15.9 us         14.1 us        49778
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                89.9 us         79.5 us        11200
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                541 us          531 us         1000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000              5350 us         5441 us          112
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            57096 us        56818 us           11
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                      0.873 us        0.889 us       896000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                       3.09 us         3.07 us       224000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                       5.79 us         5.58 us       112000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                      9.30 us         9.21 us        74667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                                     89.0 us         87.9 us         7467
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                                     956 us          962 us          747
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                                  15903 us        16319 us           45
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                                149791 us       152344 us            4
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10                                       1.61 us         1.53 us       448000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/20                                       5.23 us         4.88 us       112000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/50                                       11.0 us         10.9 us        56000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100                                      26.9 us         21.4 us        29867
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000                                      124 us          112 us         6400
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10000                                     631 us          593 us          896
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100000                                   9650 us         9583 us           75
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                                 58463 us        53125 us           10
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::RangeSearch/100                                              122 ns          123 ns      5600000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                             162 ns          164 ns      4480000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                            259 ns          261 ns      2635294
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                           516 ns          516 ns      1000000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100                                    0.831 us        0.837 us       746667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/1000                                    1.17 us         1.17 us       640000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/10000                                   1.77 us         1.76 us       373333
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100000                                  2.97 us         2.92 us       235789
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                                 2.95 us         2.85 us       235789
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                                11.2 us         11.2 us        64000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                         1.12 us         1.12 us       640000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                        7.07 us         7.15 us        89600
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                         0.742 us        0.732 us       896000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                          1.87 us         1.92 us       407273
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                          2.38 us         2.41 us       298667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                         4.25 us         4.24 us       165926
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                        41.2 us         40.5 us        16593
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                        661 us          670 us         1120
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                      8611 us         8542 us           75
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                   106122 us       104167 us            6
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                          1.55 us         1.40 us       448000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                          4.06 us         4.01 us       179200
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                          8.67 us         8.37 us        89600
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                         15.2 us         14.8 us        56000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                        89.4 us         81.2 us        10000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                        567 us          530 us         1120
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                      5142 us         5156 us          100
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                    52866 us        51136 us           11
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    0.740 us        0.732 us       896000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                     1.91 us         1.93 us       373333
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                     2.50 us         2.51 us       280000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                    4.51 us         4.55 us       154483
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                   39.3 us         39.2 us        17920
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   631 us          628 us         1120
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                 8319 us         8333 us           90
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              108509 us       109375 us            6
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                     1.56 us         1.51 us       497778
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                     4.22 us         4.17 us       194783
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                     8.47 us         8.16 us        74667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                    15.8 us         13.8 us        49778
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                   88.3 us         85.8 us         7467
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                   526 us          500 us         1000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                 5181 us         5000 us          100
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000               53012 us        53125 us           10
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10                                         0.852 us        0.837 us       746667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/20                                          3.08 us         3.07 us       224000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/50                                          5.75 us         5.72 us       112000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100                                         9.31 us         9.00 us        74667
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000                                        88.7 us         87.9 us         7467
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10000                                        952 us          941 us          747
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100000                                     15870 us        15625 us           45
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000000                                   150042 us       152344 us            4
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/10                                          1.63 us         1.54 us       497778
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/20                                          5.37 us         5.31 us       100000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/50                                          10.9 us         11.0 us        64000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/100                                         24.8 us         22.2 us        34462
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000                                         124 us          113 us         7467
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/10000                                        625 us          625 us         1000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/100000                                      9655 us         9521 us           64
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000000                                    58154 us        56818 us           11
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::RangeSearch/100                                                 122 ns          120 ns      5600000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::RangeSearch/1000                                                161 ns          161 ns      4072727
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::RangeSearch/10000                                               258 ns          261 ns      2635294
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::RangeSearch/100000                                              518 ns          516 ns      1000000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors/100                                       0.824 us        0.820 us       896000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors/1000                                       1.17 us         1.17 us       560000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors/10000                                      1.76 us         1.73 us       407273
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors/100000                                     2.95 us         2.92 us       235789
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                                    2.92 us         2.92 us       235789
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                                   11.1 us         10.6 us        56000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::FrustumCulling/1000                                            1.11 us         1.09 us       560000
Benchmarks::StaticOrthoTreePoint__MBR_Regular_WithReverseMap::FrustumCulling/10000                                           7.06 us         6.98 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                  0.919 us        0.900 us       746667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                   1.73 us         1.73 us       407273
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                   3.19 us         3.21 us       224000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                  6.10 us         6.09 us       100000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                 54.3 us         54.4 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                 702 us          698 us         1120
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                               7782 us         7812 us           90
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                             92170 us        91518 us            7
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                   1.62 us         1.57 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                   3.07 us         2.92 us       235789
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                   8.91 us         8.54 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                  16.0 us         12.7 us        64000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                 63.6 us         57.2 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                 346 us          330 us         2036
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                               3189 us         3209 us          224
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                             39795 us        40441 us           17
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10             0.909 us        0.900 us       746667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20              1.72 us         1.73 us       407273
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50              3.89 us         3.93 us       186667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100             8.28 us         8.20 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000            64.1 us         64.2 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000            788 us          767 us          896
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000          8583 us         8542 us           75
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000        99220 us        98214 us            7
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10              1.60 us         1.50 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20              3.17 us         2.85 us       235789
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50              8.89 us         8.16 us        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100             17.5 us         14.8 us        49778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000            67.3 us         65.6 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000            368 us          366 us         2133
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000          3497 us         3448 us          213
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000        40683 us        40441 us           17
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                            0.945 us        0.942 us       746667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                             1.42 us         1.41 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                             2.72 us         2.79 us       263529
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                            4.99 us         5.00 us       100000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                           47.1 us         46.5 us        14452
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                           633 us          628 us         1120
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                         7247 us         7292 us           90
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                       81250 us        79861 us            9
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                             1.69 us         1.61 us       407273
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                             2.87 us         2.68 us       280000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                             5.24 us         4.71 us       149333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                            9.05 us         5.93 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                           37.9 us         31.8 us        18667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                           370 us          369 us         1948
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                         4202 us         4236 us          166
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                       46801 us        46875 us           11
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/10                                        0.348 us        0.345 us      1947826
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/20                                        0.347 us        0.345 us      1947826
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/50                                        0.349 us        0.345 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/100                                       0.348 us        0.353 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/1000                                      0.348 us        0.353 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/10000                                     0.348 us        0.353 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                1.24 us         1.26 us       560000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                3.24 us         3.22 us       213333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                9.82 us         9.84 us        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/100                               15.3 us         15.0 us        44800
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                               143 us          144 us         4978
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                             1495 us         1475 us          498
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Update/10                                               1.99 us         2.01 us       373333 items_per_second=4.97777M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Update/20                                               3.67 us         3.68 us       186667 items_per_second=5.43031M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Update/50                                               8.36 us         8.37 us        74667 items_per_second=5.97336M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Update/100                                              16.9 us         16.9 us        40727 items_per_second=5.92393M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Update/1000                                              258 us          255 us         2635 items_per_second=3.92186M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Update/10000                                            6525 us         6424 us           90 items_per_second=1.55676M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::PickSearch/100                                           197 ns          197 ns      3733333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::PickSearch/1000                                          235 ns          235 ns      2986667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::PickSearch/10000                                         778 ns          767 ns       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::PickSearch/100000                                       7730 ns         7743 ns        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                          156 ns          153 ns      4480000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                         192 ns          193 ns      3733333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                        735 ns          715 ns       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                      6077 ns         6138 ns       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                    0.546 us        0.530 us      1120000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                    3.43 us         3.45 us       203636
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                         3.62 us         3.69 us       194783
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                         129 us          128 us         5600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                       5814 us         5859 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                    215163 us       213542 us            3
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                         3.87 us         3.84 us       179200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                         131 us          131 us         5600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                       2060 us          672 us         1000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                     80047 us        54688 us           10
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                   22.7 us         22.9 us        32000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                   180 us          176 us         4073
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                 6025 us         6094 us          100
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000              515671 us       515625 us            1
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100                                0.599 us        0.600 us      1120000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                1.42 us         1.41 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/10000                               6.91 us         6.80 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100000                              72.2 us         71.5 us         8960
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100                                  0.549 us        0.544 us      1120000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/1000                                  1.38 us         1.35 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/10000                                 6.88 us         6.98 us       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100000                                80.7 us         83.7 us         7467
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                    0.758 us        0.767 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                     1.46 us         1.48 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                     2.46 us         2.46 us       298667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                    5.14 us         5.02 us       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                   47.8 us         48.1 us        14933
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                   696 us          698 us         1120
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                 8908 us         8958 us           75
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                              122924 us       122396 us            6
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10                                     1.44 us         1.42 us       560000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/20                                     2.91 us         2.72 us       224000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/50                                     8.60 us         8.37 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100                                    17.6 us         13.1 us        56000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                   71.2 us         64.5 us         8960
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                   447 us          444 us         1723
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                 4914 us         5022 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                               74939 us        74653 us            9
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10               0.754 us        0.750 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                1.40 us         1.38 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                3.06 us         3.00 us       224000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100               7.25 us         7.32 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000              57.7 us         57.2 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000              790 us          785 us          896
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            9826 us        10000 us           75
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000         131756 us       132812 us            6
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                1.46 us         1.48 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                3.02 us         2.92 us       235789
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                8.93 us         8.37 us        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100               19.6 us         16.3 us        37333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000              75.2 us         73.2 us         8960
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000              476 us          474 us         1120
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000            5427 us         5312 us          100
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          78497 us        74653 us            9
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                              0.817 us        0.820 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                               1.16 us         1.17 us       640000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                               2.09 us         2.10 us       320000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                              3.65 us         3.60 us       186667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                             35.7 us         35.3 us        19478
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                             577 us          578 us         1000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                           6816 us         6836 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                         73897 us        73864 us           11
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                               1.49 us         1.40 us       560000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                               2.17 us         1.77 us       344615
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                               4.26 us         4.14 us       165926
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                              10.2 us         8.09 us       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                             34.7 us         31.5 us        21333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                             437 us          420 us         1600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                           5025 us         5065 us          145
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                         57114 us        57292 us            9
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/10                                          0.351 us        0.345 us      1947826
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/20                                          0.359 us        0.360 us      1866667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/50                                          0.360 us        0.361 us      1947826
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/100                                         0.352 us        0.353 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/1000                                        0.351 us        0.353 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/10000                                       0.344 us        0.345 us      1947826
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/10                                  1.16 us         1.14 us       560000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/20                                  2.85 us         2.83 us       248889
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/50                                  8.34 us         8.37 us        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/100                                 15.3 us         15.4 us        49778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/1000                                 144 us          144 us         4978
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/10000                               1930 us         1927 us          373
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Update/10                                                 1.69 us         1.69 us       407273 items_per_second=5.92397M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Update/20                                                 3.23 us         3.28 us       224000 items_per_second=6.10043M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Update/50                                                 8.07 us         7.95 us        74667 items_per_second=6.28775M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Update/100                                                14.3 us         14.4 us        49778 items_per_second=6.92563M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Update/1000                                                154 us          153 us         4480 items_per_second=6.51636M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Update/10000                                              2786 us         2781 us          236 items_per_second=3.59619M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::PickSearch/100                                             238 ns          241 ns      2986667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::PickSearch/1000                                            292 ns          295 ns      2488889
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::PickSearch/10000                                           545 ns          544 ns      1120000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::PickSearch/100000                                          987 ns          984 ns       746667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RangeSearch/100                                            210 ns          209 ns      3446154
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RangeSearch/1000                                           306 ns          300 ns      2240000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RangeSearch/10000                                         1020 ns         1025 ns       640000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RangeSearch/100000                                       10138 ns        10045 ns        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::FrustumCulling/1000                                       1.12 us         1.12 us       640000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::FrustumCulling/10000                                      3.77 us         3.85 us       194783
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                           49.9 us         50.0 us        10000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                          1407 us         1412 us          498
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                        27250 us        26875 us           25
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                      471909 us       468750 us            2
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                           50.8 us         51.6 us        10000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                          1424 us         1412 us          498
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                         9941 us         3815 us          172
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                      132595 us        93750 us            9
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                     32.9 us         33.0 us        21333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                     247 us          246 us         2800
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                   6596 us         6557 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                148713 us       150000 us            5
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100                                  0.929 us        0.921 us       746667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                  1.33 us         1.34 us       560000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                 3.18 us         3.22 us       213333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                7.92 us         8.02 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100                                    0.663 us        0.645 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/1000                                    1.02 us         1.03 us       640000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/10000                                   2.64 us         2.67 us       263529
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100000                                  10.3 us         10.5 us        74667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                       0.842 us        0.837 us       746667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                        1.70 us         1.69 us       407273
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                        3.21 us         3.28 us       224000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                       6.28 us         6.28 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                      57.2 us         57.2 us        11200
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                      797 us          795 us          747
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                    8513 us         8507 us           90
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                 122477 us       122396 us            6
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                        1.72 us         1.53 us       407273
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                        3.55 us         3.15 us       213333
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                        12.7 us         10.0 us        64000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                       16.3 us         11.2 us        44800
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                      68.5 us         62.5 us        10000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                      413 us          396 us         1659
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                    3687 us         3760 us          187
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                  65335 us        64236 us            9
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                  0.855 us        0.858 us       746667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                   1.78 us         1.76 us       407273
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                   4.00 us         3.92 us       179200
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                  8.59 us         8.72 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                 68.3 us         68.0 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                 845 us          854 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               9138 us         9167 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            129825 us       130208 us            6
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                   1.67 us         1.54 us       497778
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                   3.16 us         3.00 us       224000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                   9.03 us         8.79 us        74667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                  18.3 us         16.9 us        40727
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                 71.4 us         71.5 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                 408 us          410 us         1792
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000               3680 us         3593 us          187
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             63906 us        64236 us            9
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                                 0.989 us        0.977 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                  1.46 us         1.43 us       448000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                  2.82 us         2.83 us       248889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                                 5.25 us         5.16 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                                51.5 us         51.6 us        10000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                                683 us          670 us         1120
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                              8737 us         8750 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                           111800 us       111979 us            6
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                  1.95 us         1.80 us       373333
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                  3.05 us         2.58 us       224000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                  6.74 us         6.00 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                                 10.7 us         9.94 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                                50.9 us         45.9 us        16000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                                470 us          461 us         1659
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                              6139 us         6250 us          100
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                            90629 us        91518 us            7
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::InsertToLeaf/10                                             0.291 us        0.295 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::InsertToLeaf/20                                             0.288 us        0.289 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::InsertToLeaf/50                                             0.302 us        0.310 us      1866667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::InsertToLeaf/100                                            0.360 us        0.360 us      1866667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::InsertToLeaf/1000                                           0.345 us        0.353 us      1947826
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::InsertToLeaf/10000                                          0.449 us        0.410 us      1792000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                     1.44 us         1.39 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                     6.24 us         5.00 us       100000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                     14.1 us         12.6 us        44800
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/100                                    19.5 us         18.8 us        40727
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                                    198 us          193 us         3733
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                                  1774 us         1766 us          407
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Update/10                                                    1.92 us         1.86 us       344615 items_per_second=5.37936M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Update/20                                                    3.65 us         3.66 us       179200 items_per_second=5.46133M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Update/50                                                    8.21 us         8.20 us        89600 items_per_second=6.10043M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Update/100                                                   17.2 us         17.3 us        40727 items_per_second=5.79228M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Update/1000                                                   293 us          285 us         1867 items_per_second=3.51435M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::Update/10000                                                 8266 us         8092 us          112 items_per_second=1.23586M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::PickSearch/100                                                165 ns          165 ns      4072727
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::PickSearch/1000                                               192 ns          193 ns      3733333
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::PickSearch/10000                                              716 ns          715 ns       896000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::PickSearch/100000                                            7936 ns         7673 ns        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RangeSearch/100                                               130 ns          128 ns      5600000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                              155 ns          153 ns      4480000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                             670 ns          670 ns      1120000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                           6140 ns         6138 ns       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                         0.448 us        0.439 us      1600000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                         3.11 us         3.07 us       224000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                              3.43 us         3.44 us       213333
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                              117 us          120 us         6400
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                            5567 us         5556 us           90
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                         222735 us       223958 us            3
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                              3.94 us         4.02 us       186667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                              118 us          114 us         5600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                            1895 us          600 us         1120
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                          79822 us        56250 us           10
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                        12.3 us         12.3 us        56000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                        120 us          120 us         5600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                      4590 us         4590 us          160
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                   502636 us       500000 us            1
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                     0.428 us        0.433 us      1659259
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                     1.14 us         1.14 us       560000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                    6.37 us         6.42 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                   72.5 us         71.1 us         7467
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                       0.459 us        0.469 us      1600000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                       1.13 us         1.15 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                      6.33 us         6.42 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                     81.3 us         80.2 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                         0.704 us        0.711 us      1120000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                          1.41 us         1.41 us       497778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                          2.46 us         2.46 us       298667
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                         5.41 us         5.31 us       100000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                        51.6 us         50.2 us        11200
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                        780 us          785 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                     12340 us        12277 us           56
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                   190940 us       191406 us            4
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10                                          1.41 us         1.37 us       560000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/20                                          2.79 us         2.65 us       235789
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/50                                          8.59 us         7.85 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100                                         18.3 us         13.8 us        49778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                        79.7 us         71.5 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                        521 us          500 us         1000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                      7971 us         7917 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                   131426 us       132812 us            6
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    0.731 us        0.725 us      1120000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                     1.41 us         1.43 us       448000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                     3.22 us         3.15 us       213333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                    7.60 us         7.67 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                   62.1 us         62.8 us        11200
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   863 us          854 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                13925 us        13889 us           45
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              209559 us       208333 us            3
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                     1.44 us         1.42 us       560000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                     2.93 us         2.76 us       248889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                     9.95 us         9.77 us        56000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                    19.2 us         15.9 us        37333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                   82.2 us         75.3 us        11200
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                   536 us          530 us         1120
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                 8462 us         8542 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              144461 us       143750 us            5
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                                   0.871 us        0.858 us       746667
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                    1.29 us         1.23 us       560000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                    2.27 us         2.30 us       298667
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                                   3.96 us         3.90 us       172308
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                                  39.1 us         39.2 us        17920
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                                  628 us          628 us         1120
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                                9437 us         9583 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                             115521 us       114583 us            6
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                    1.77 us         1.65 us       407273
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                    2.68 us         2.61 us       203636
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                    5.83 us         5.44 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                                   10.3 us         8.54 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                                  49.4 us         43.0 us        16000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                                  506 us          500 us         1000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                                6873 us         6836 us          112
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                              94641 us        91518 us            7
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::InsertToLeaf/10                                               0.299 us        0.292 us      2357895
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::InsertToLeaf/20                                               0.291 us        0.289 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::InsertToLeaf/50                                               0.289 us        0.292 us      2357895
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::InsertToLeaf/100                                              0.291 us        0.295 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::InsertToLeaf/1000                                             0.289 us        0.289 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::InsertToLeaf/10000                                            0.291 us        0.292 us      2357895
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/10                                      0.930 us        0.903 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/20                                       2.63 us         2.61 us       263529
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/50                                       8.13 us         8.20 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/100                                      15.1 us         15.4 us        49778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/1000                                      144 us          144 us         4978
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/10000                                    2051 us         2038 us          345
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Update/10                                                      1.57 us         1.60 us       497778 items_per_second=6.24663M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Update/20                                                      3.02 us         3.05 us       235789 items_per_second=6.56109M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Update/50                                                      7.54 us         7.67 us        89600 items_per_second=6.51636M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Update/100                                                     13.2 us         13.2 us        49778 items_per_second=7.58522M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Update/1000                                                     149 us          148 us         4978 items_per_second=6.77855M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::Update/10000                                                   2823 us         2811 us          239 items_per_second=3.55721M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::PickSearch/100                                                  156 ns          157 ns      4480000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::PickSearch/1000                                                 169 ns          169 ns      4072727
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::PickSearch/10000                                                233 ns          230 ns      2986667
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::PickSearch/100000                                               359 ns          361 ns      1947826
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RangeSearch/100                                                 141 ns          140 ns      4480000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RangeSearch/1000                                                139 ns          141 ns      4977778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RangeSearch/10000                                               203 ns          205 ns      3200000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RangeSearch/100000                                              570 ns          558 ns       896000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::FrustumCulling/1000                                           0.321 us        0.322 us      2133333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::FrustumCulling/10000                                           1.26 us         1.28 us       560000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                                33.7 us         33.0 us        20364
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                691 us          680 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                             11968 us        11998 us           56
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                           233773 us       229167 us            3
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                                34.7 us         34.5 us        20364
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                                685 us          680 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                              4610 us         1730 us          560
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                            71898 us        50000 us           10
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                          13.4 us         12.9 us        49778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                          125 us          126 us         5600
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                        2203 us         2197 us          320
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                      69298 us        69602 us           11
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100                                       0.348 us        0.345 us      2036364
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                      0.418 us        0.414 us      1659259
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                     0.830 us        0.837 us       896000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                     1.84 us         1.84 us       373333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100                                         0.375 us        0.377 us      1947826
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedAll/1000                                        0.438 us        0.433 us      1659259
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedAll/10000                                       0.876 us        0.889 us       896000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100000                                       3.53 us         3.53 us       203636
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                     0.953 us        0.963 us       746667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                      1.77 us         1.76 us       373333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                      3.21 us         3.15 us       213333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                     6.09 us         6.14 us       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                    54.3 us         54.7 us        10000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                    718 us          711 us         1120
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                                  8581 us         8542 us           75
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                               109732 us       109375 us            6
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                      1.83 us         1.69 us       407273
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                      3.80 us         3.53 us       194783
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                      9.15 us         8.16 us        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                     16.5 us         13.4 us        56000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                    66.0 us         62.8 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                    382 us          377 us         1948
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                  4182 us         4236 us          166
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                                64366 us        61080 us           11
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.988 us        0.984 us       746667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.77 us         1.73 us       407273
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 3.87 us         3.84 us       179200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                8.27 us         8.37 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               64.3 us         64.2 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               795 us          802 us          896
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000             9295 us         9375 us           75
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          114797 us       114583 us            6
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.74 us         1.67 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                 4.02 us         3.60 us       186667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                 12.5 us         11.2 us        64000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                17.9 us         13.8 us        49778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000               69.3 us         64.1 us        10000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000               396 us          354 us         1723
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000             4077 us         3953 us          166
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000           59048 us        58239 us           11
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                1.01 us         1.00 us       640000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                1.48 us         1.51 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                2.81 us         2.85 us       263529
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                               5.22 us         5.16 us       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                              48.8 us         48.8 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                              664 us          656 us         1120
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                            7697 us         7465 us           90
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                          88208 us        87054 us            7
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                1.67 us         1.50 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                3.13 us         2.58 us       224000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                6.27 us         5.45 us       143360
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                               9.39 us         6.84 us       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                              41.4 us         38.1 us        21333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                              401 us          399 us         1723
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                            4740 us         4667 us          154
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                          59821 us        56818 us           11
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::InsertToLeaf/10                                           0.380 us        0.366 us      1792000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::InsertToLeaf/20                                           0.343 us        0.337 us      2133333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::InsertToLeaf/50                                           0.337 us        0.337 us      2133333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::InsertToLeaf/100                                          0.340 us        0.344 us      2133333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::InsertToLeaf/1000                                         0.339 us        0.338 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::InsertToLeaf/10000                                        0.339 us        0.344 us      2133333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10                                   1.53 us         1.50 us       407273
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/20                                   3.67 us         3.69 us       194783
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/50                                   10.6 us         10.7 us        64000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/100                                  16.5 us         16.5 us        40727
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/1000                                  154 us          154 us         4978
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10000                                1588 us         1569 us          448
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Update/10                                                  1.43 us         1.44 us       497778 items_per_second=6.92561M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Update/20                                                  2.65 us         2.67 us       263529 items_per_second=7.49594M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Update/50                                                  5.78 us         5.86 us       112000 items_per_second=8.53333M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Update/100                                                 12.0 us         12.0 us        64000 items_per_second=8.35918M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Update/1000                                                 210 us          213 us         3446 items_per_second=4.69243M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::Update/10000                                               6144 us         6138 us          112 items_per_second=1.62909M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::PickSearch/100                                              200 ns          199 ns      3446154
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::PickSearch/1000                                             241 ns          235 ns      2986667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::PickSearch/10000                                            788 ns          767 ns       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::PickSearch/100000                                          7890 ns         7952 ns        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RangeSearch/100                                             159 ns          157 ns      4480000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                            196 ns          197 ns      3733333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                           735 ns          732 ns       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                         6241 ns         6278 ns       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                       0.554 us        0.562 us      1000000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                       3.46 us         3.45 us       203636
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                            3.68 us         3.69 us       194783
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                            130 us          129 us         4978
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                          5895 us         5859 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                       224706 us       223958 us            3
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100                            4.04 us         4.01 us       179200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                            142 us          138 us         4978
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                          2047 us          879 us          747
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                        81804 us        58239 us           11
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                      24.7 us         24.9 us        32000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                      183 us          184 us         4073
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                    5995 us         5999 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                 558978 us       562500 us            1
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100                                   0.607 us        0.593 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/1000                                   1.44 us         1.44 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/10000                                  6.91 us         6.80 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100000                                 75.2 us         75.0 us         8960
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100                                     0.554 us        0.558 us      1120000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RayIntersectedAll/1000                                     1.38 us         1.38 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RayIntersectedAll/10000                                    6.96 us         6.98 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100000                                   83.4 us         83.7 us         8960
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10                                       0.804 us        0.802 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/20                                        1.44 us         1.43 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/50                                        2.42 us         2.46 us       298667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100                                       5.13 us         5.16 us       100000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000                                      47.3 us         47.6 us        14452
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10000                                      707 us          698 us          896
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100000                                   10864 us        10742 us           64
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                 156470 us       156250 us            4
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10                                        1.53 us         1.40 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/20                                        2.92 us         2.70 us       248889
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/50                                        8.76 us         7.32 us        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100                                       16.9 us         13.6 us        44800
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000                                      74.3 us         66.3 us         8960
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10000                                      482 us          454 us         1445
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100000                                    7238 us         7118 us           90
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000000                                 106505 us       106771 us            6
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                  0.798 us        0.802 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                   1.44 us         1.40 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                   3.05 us         3.05 us       235789
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                  7.24 us         7.15 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                 57.6 us         57.8 us        10000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                 792 us          774 us          747
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              11883 us        11719 us           56
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            163025 us       164062 us            4
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                   1.58 us         1.57 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                   2.81 us         2.83 us       248889
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                   9.76 us         8.51 us       112000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                  19.6 us         16.6 us        49778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                 84.1 us         80.2 us         8960
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                 518 us          502 us         1120
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000               7743 us         7639 us           90
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            119972 us       118750 us            5
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                 0.884 us        0.889 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                  1.35 us         1.31 us       560000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                  2.22 us         2.25 us       298667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                 3.91 us         3.92 us       179200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                39.5 us         38.5 us        18667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                602 us          600 us         1120
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                              7492 us         7394 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                            84319 us        84821 us            7
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10                                  1.43 us         1.44 us       497778
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/20                                  2.10 us         2.00 us       320000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/50                                  4.22 us         3.45 us       194783
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100                                 11.0 us         8.16 us        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                36.8 us         35.3 us        19478
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                471 us          475 us         1545
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100000                              5268 us         5301 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                            61683 us        59659 us           11
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::InsertToLeaf/10                                             0.341 us        0.338 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::InsertToLeaf/20                                             0.337 us        0.337 us      1947826
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::InsertToLeaf/50                                             0.338 us        0.338 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::InsertToLeaf/100                                            0.338 us        0.338 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::InsertToLeaf/1000                                           0.339 us        0.330 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::InsertToLeaf/10000                                          0.338 us        0.338 us      2036364
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/10                                     1.44 us         1.43 us       448000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/20                                     3.33 us         3.30 us       213333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/50                                     9.07 us         9.00 us        74667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/100                                    16.4 us         16.5 us        40727
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/1000                                    156 us          157 us         4480
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/10000                                  2008 us         2038 us          345
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Update/10                                                    1.29 us         1.29 us       497778 items_per_second=7.77019M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Update/20                                                    2.41 us         2.40 us       280000 items_per_second=8.33488M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Update/50                                                    6.02 us         6.14 us       112000 items_per_second=8.14545M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Update/100                                                   10.5 us         10.5 us        64000 items_per_second=9.52558M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Update/1000                                                   119 us          120 us         6400 items_per_second=8.35918M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::Update/10000                                                 2430 us         2427 us          264 items_per_second=4.12098M/s
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::PickSearch/100                                                240 ns          241 ns      2986667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::PickSearch/1000                                               296 ns          292 ns      2357895
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::PickSearch/10000                                              545 ns          544 ns      1120000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::PickSearch/100000                                            1013 ns         1025 ns       640000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RangeSearch/100                                               206 ns          205 ns      3200000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RangeSearch/1000                                              303 ns          305 ns      2357895
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RangeSearch/10000                                            1010 ns         1025 ns       746667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RangeSearch/100000                                          10704 ns        10742 ns        64000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::FrustumCulling/1000                                          1.13 us         1.12 us       640000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::FrustumCulling/10000                                         3.77 us         3.77 us       186667
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                              50.6 us         48.8 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                             1414 us         1412 us          498
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                           27434 us        26875 us           25
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                         496426 us       500000 us            2
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100                              50.6 us         50.2 us        11200
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                             1424 us         1443 us          498
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                            9648 us         3731 us          201
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                         141297 us        96354 us            6
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                        33.1 us         33.0 us        21333
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                        250 us          249 us         2635
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                      6653 us         6696 us          112
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                   155161 us       152344 us            4
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100                                     0.794 us        0.785 us       896000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/1000                                     1.19 us         1.17 us       560000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/10000                                    2.99 us         2.98 us       235789
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100000                                   7.79 us         7.67 us        89600
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100                                       0.661 us        0.670 us      1120000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RayIntersectedAll/1000                                       1.02 us         1.03 us       640000
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RayIntersectedAll/10000                                      2.63 us         2.67 us       263529
Benchmarks::DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100000                                     10.6 us         10.5 us        64000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                          0.887 us        0.879 us       746667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                           1.77 us         1.76 us       407273
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                           3.27 us         3.30 us       213333
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                          6.38 us         6.28 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                         57.6 us         57.8 us        10000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                         771 us          767 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                       8986 us         8958 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                    130693 us       132812 us            6
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                           1.61 us         1.46 us       448000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                           3.59 us         3.45 us       235789
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                           9.52 us         8.06 us        64000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                          16.6 us         14.1 us        49778
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                         70.0 us         66.3 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                         415 us          414 us         1659
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                       4209 us         4236 us          166
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                     76104 us        74653 us            9
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                     0.896 us        0.907 us       896000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                      1.78 us         1.76 us       407273
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                      4.04 us         4.08 us       172308
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                     8.68 us         8.58 us        74667
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                    68.0 us         68.4 us        11200
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                    854 us          854 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                  9771 us         9792 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000               138236 us       140625 us            5
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                      1.64 us         1.53 us       407273
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                      3.20 us         3.05 us       194783
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                      8.30 us         8.37 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                     18.0 us         15.7 us        40727
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                    73.5 us         64.5 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                    428 us          426 us         1723
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                  4231 us         4297 us          160
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                73747 us        72443 us           11
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                     1.06 us         1.05 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                     1.58 us         1.57 us       448000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                     2.97 us         2.98 us       235789
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                    5.47 us         5.47 us       100000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                   51.8 us         51.6 us        11200
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                   699 us          698 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                                 9231 us         9375 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                              120281 us       119792 us            6
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                     2.06 us         1.96 us       407273
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                     3.54 us         3.28 us       224000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                     7.59 us         6.63 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                                    10.5 us         9.42 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                   53.3 us         50.0 us        10000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                   469 us          429 us         1493
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                                 6359 us         6417 us          112
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                               86437 us        85069 us            9
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::InsertToLeaf/10                                                0.280 us        0.283 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::InsertToLeaf/20                                                0.280 us        0.276 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::InsertToLeaf/50                                                0.282 us        0.283 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::InsertToLeaf/100                                               0.282 us        0.283 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::InsertToLeaf/1000                                              0.282 us        0.283 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::InsertToLeaf/10000                                             0.281 us        0.283 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10                                        1.39 us         1.38 us       497778
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__RebalanceMode/20                                        3.58 us         3.53 us       194783
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__RebalanceMode/50                                        10.5 us         10.5 us        64000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__RebalanceMode/100                                       16.5 us         16.5 us        40727
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__RebalanceMode/1000                                       156 us          157 us         4480
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10000                                     1667 us         1651 us          407
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Update/10                                                       1.34 us         1.35 us       497778 items_per_second=7.40879M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Update/20                                                       2.40 us         2.40 us       280000 items_per_second=8.33488M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Update/50                                                       5.35 us         5.30 us       112000 items_per_second=9.43158M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Update/100                                                      11.1 us         11.2 us        64000 items_per_second=8.90435M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Update/1000                                                      198 us          197 us         3733 items_per_second=5.08323M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::Update/10000                                                    6077 us         6094 us          100 items_per_second=1.64103M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::PickSearch/100                                                   161 ns          160 ns      4480000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::PickSearch/1000                                                  189 ns          184 ns      3733333
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::PickSearch/10000                                                 695 ns          698 ns       896000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::PickSearch/100000                                               7787 ns         7673 ns        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RangeSearch/100                                                  130 ns          129 ns      4977778
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RangeSearch/1000                                                 157 ns          157 ns      4480000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RangeSearch/10000                                                653 ns          656 ns      1120000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RangeSearch/100000                                              6100 ns         5999 ns       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::FrustumCulling/1000                                            0.439 us        0.439 us      1600000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::FrustumCulling/10000                                            3.05 us         3.05 us       235789
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                                 3.20 us         3.21 us       224000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                                 107 us          107 us         6400
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                               5400 us         5441 us          112
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                            213410 us       213542 us            3
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100                                 3.58 us         3.61 us       194783
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                                 107 us          105 us         6400
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                               1947 us          830 us          640
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                             76826 us        54688 us           10
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                           11.9 us         12.0 us        64000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                           113 us          112 us         6400
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                         4329 us         4270 us          172
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                      469614 us       468750 us            2
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RayIntersectedFirst/100                                        0.426 us        0.424 us      1659259
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RayIntersectedFirst/1000                                        1.11 us         1.12 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RayIntersectedFirst/10000                                       6.10 us         6.14 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RayIntersectedFirst/100000                                      71.0 us         69.8 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RayIntersectedAll/100                                          0.448 us        0.445 us      1544828
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RayIntersectedAll/1000                                          1.14 us         1.15 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RayIntersectedAll/10000                                         6.27 us         6.28 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Regular_WithReverseMap::RayIntersectedAll/100000                                        79.5 us         80.2 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10                                            0.744 us        0.753 us      1120000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, SeqExec>/20                                             1.45 us         1.48 us       497778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, SeqExec>/50                                             2.48 us         2.46 us       280000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100                                            5.38 us         5.31 us       100000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000                                           50.9 us         51.6 us        10000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10000                                           788 us          774 us          747
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100000                                        11903 us        11998 us           56
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                      188630 us       187500 us            4
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, ParExec>/10                                             1.38 us         1.22 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, ParExec>/20                                             2.79 us         2.73 us       263529
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, ParExec>/50                                             8.38 us         7.81 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, ParExec>/100                                            17.8 us         14.6 us        40727
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000                                           76.1 us         66.3 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, ParExec>/10000                                           536 us          516 us         1000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, ParExec>/100000                                         7476 us         7533 us          112
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000000                                      131279 us       131250 us            5
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                       0.743 us        0.732 us       896000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                        1.44 us         1.43 us       448000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                        3.21 us         3.22 us       213333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                       7.63 us         7.50 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                      61.7 us         61.4 us        11200
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                      859 us          858 us          747
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                   12333 us        12277 us           56
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                 197216 us       197917 us            3
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                        1.31 us         1.29 us       497778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                        3.02 us         2.83 us       248889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                        8.00 us         7.95 us        74667
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                       20.4 us         18.0 us        40727
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                      82.7 us         82.0 us         8960
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                      546 us          544 us         1120
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                    7989 us         7812 us           90
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                 146054 us       143750 us            5
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                      0.917 us        0.924 us       896000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                       1.30 us         1.31 us       560000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                       2.35 us         2.34 us       280000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                      4.17 us         4.17 us       172308
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                     40.6 us         40.8 us        17231
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                     647 us          642 us         1120
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                                   8878 us         8958 us           75
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                                117137 us       117188 us            6
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10                                       1.81 us         1.80 us       373333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/20                                       2.91 us         2.54 us       320000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/50                                       5.58 us         5.51 us       144516
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100                                      10.3 us         8.79 us       112000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                     49.5 us         50.0 us        10000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                     522 us          516 us         1000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100000                                   7130 us         6944 us           90
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                                 92680 us        93750 us            7
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::InsertToLeaf/10                                                  0.279 us        0.283 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::InsertToLeaf/20                                                  0.278 us        0.276 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::InsertToLeaf/50                                                  0.278 us        0.276 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::InsertToLeaf/100                                                 0.279 us        0.278 us      2357895
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::InsertToLeaf/1000                                                0.278 us        0.279 us      2635294
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::InsertToLeaf/10000                                               0.279 us        0.276 us      2488889
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__RebalanceMode/10                                          1.24 us         1.20 us       560000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__RebalanceMode/20                                          3.15 us         3.07 us       224000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__RebalanceMode/50                                          9.08 us         9.24 us        89600
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__RebalanceMode/100                                         16.7 us         16.7 us        44800
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__RebalanceMode/1000                                         157 us          157 us         4480
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Insert__RebalanceMode/10000                                       2116 us         2129 us          345
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Update/10                                                         1.19 us         1.20 us       560000 items_per_second=8.33488M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Update/20                                                         2.23 us         2.20 us       320000 items_per_second=9.10222M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Update/50                                                         5.43 us         5.44 us       112000 items_per_second=9.18974M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Update/100                                                        9.19 us         9.21 us        74667 items_per_second=10.8607M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Update/1000                                                        106 us          107 us         6400 items_per_second=9.30909M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::Update/10000                                                      2344 us         2344 us          280 items_per_second=4.26667M/s
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::PickSearch/100                                                     157 ns          160 ns      4480000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::PickSearch/1000                                                    170 ns          169 ns      4072727
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::PickSearch/10000                                                   231 ns          229 ns      2800000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::PickSearch/100000                                                  356 ns          353 ns      1947826
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RangeSearch/100                                                    125 ns          126 ns      5600000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RangeSearch/1000                                                   135 ns          135 ns      4977778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RangeSearch/10000                                                  201 ns          199 ns      3446154
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RangeSearch/100000                                                 548 ns          544 ns      1120000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::FrustumCulling/1000                                              0.328 us        0.322 us      2133333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::FrustumCulling/10000                                              1.26 us         1.27 us       640000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                                   34.8 us         35.2 us        21333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                                   683 us          663 us          896
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                                11809 us        11963 us           64
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                              229673 us       229167 us            3
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100                                   33.9 us         33.8 us        20364
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                                   683 us          670 us         1120
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                                 4641 us         1986 us          299
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                               68562 us        47917 us           15
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                             13.3 us         13.2 us        49778
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                             123 us          123 us         5600
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                           2179 us         2197 us          320
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                         52886 us        53125 us           10
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RayIntersectedFirst/100                                          0.349 us        0.353 us      2036364
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RayIntersectedFirst/1000                                         0.429 us        0.424 us      1659259
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RayIntersectedFirst/10000                                        0.821 us        0.816 us       746667
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RayIntersectedFirst/100000                                        1.81 us         1.80 us       373333
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RayIntersectedAll/100                                            0.370 us        0.361 us      1947826
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RayIntersectedAll/1000                                           0.441 us        0.439 us      1600000
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RayIntersectedAll/10000                                          0.873 us        0.879 us       746667
Benchmarks::DynamicOrthoTreeBox__MBR_Loose_WithReverseMap::RayIntersectedAll/100000                                          3.35 us         3.30 us       213333
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                   0.861 us        0.858 us       746667
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                    1.63 us         1.64 us       448000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                    3.03 us         3.00 us       224000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                   5.59 us         5.62 us       100000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                  51.0 us         50.2 us        11200
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                  684 us          684 us         1120
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                7200 us         7292 us           90
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                              79867 us        79545 us           11
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                    1.61 us         1.46 us       448000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                    2.93 us         2.85 us       263529
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                    9.96 us         8.54 us        64000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                   15.6 us         12.9 us        49778
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                  64.1 us         61.4 us        11200
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                  374 us          369 us         1948
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                2825 us         2824 us          249
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                              28415 us        27644 us           26
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10              0.866 us        0.872 us       896000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20               1.68 us         1.65 us       407273
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50               3.12 us         3.11 us       235789
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100              5.77 us         5.78 us       100000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000             51.9 us         51.6 us        10000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000             698 us          698 us          896
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000           7231 us         7292 us           90
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000         78305 us        79861 us            9
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10               1.66 us         1.60 us       497778
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20               3.11 us         3.02 us       263529
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50               9.11 us         7.95 us        74667
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100              16.2 us         12.6 us        44800
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000             65.3 us         58.6 us        11200
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000             388 us          345 us         1948
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000           2942 us         2788 us          213
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000         29065 us        29375 us           25
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::PickSearch/100                                            160 ns          160 ns      4480000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::PickSearch/1000                                           192 ns          193 ns      3733333
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::PickSearch/10000                                          696 ns          698 ns      1120000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::PickSearch/100000                                        9058 ns         8196 ns        89600
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                           104 ns          103 ns      6400000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                          132 ns          132 ns      4977778
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                         683 ns          680 ns       896000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                       6704 ns         6696 ns       112000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                     0.541 us        0.481 us      1493333
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                     3.30 us         3.30 us       213333
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                          3.38 us         3.37 us       213333
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                          136 us          135 us         4978
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                        5733 us         5720 us          112
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                     218544 us       218750 us            3
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                          3.72 us         3.68 us       186667
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                          131 us          131 us         5600
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                        1770 us         1676 us          373
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                      75834 us        30540 us           22
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                    13.8 us         13.8 us        49778
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                    131 us          129 us         4978
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                  5678 us         5720 us          112
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000               540018 us       546875 us            1
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100                                 0.419 us        0.420 us      1600000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                 1.28 us         1.28 us       560000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                6.57 us         6.56 us       112000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100000                               72.4 us         71.5 us         8960
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100                                   0.455 us        0.450 us      1493333
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/1000                                   1.29 us         1.28 us       560000
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/10000                                  6.67 us         6.63 us        89600
Benchmarks::StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100000                                 80.2 us         80.2 us         8960
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                     0.851 us        0.854 us       896000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                      1.47 us         1.46 us       448000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                      2.42 us         2.40 us       280000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                     4.53 us         4.55 us       154483
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                    44.4 us         43.9 us        14933
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                    639 us          642 us         1120
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                  7127 us         7254 us          112
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                80010 us        79861 us            9
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10                                      1.64 us         1.55 us       373333
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/20                                      3.25 us         3.10 us       186667
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/50                                      8.28 us         7.50 us        89600
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100                                     13.5 us         11.7 us        74667
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                    76.7 us         68.4 us        11200
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                    453 us          446 us         1120
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                  3841 us         3928 us          179
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                40295 us        39062 us           18
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.871 us        0.858 us       746667
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.48 us         1.46 us       448000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 2.42 us         2.40 us       280000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                4.68 us         4.71 us       149333
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               45.5 us         45.0 us        14933
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               657 us          656 us         1120
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000             7292 us         7254 us          112
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           80794 us        80966 us           11
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.73 us         1.65 us       407273
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.25 us         3.21 us       224000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 8.60 us         7.67 us        89600
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                17.0 us         12.6 us        49778
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000               79.1 us         69.8 us        11200
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000               456 us          424 us         1659
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000             4010 us         4015 us          179
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000           40648 us        39062 us           18
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::PickSearch/100                                              174 ns          176 ns      4072727
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::PickSearch/1000                                             196 ns          197 ns      3733333
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::PickSearch/10000                                            332 ns          330 ns      2036364
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::PickSearch/100000                                           573 ns          572 ns      1120000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RangeSearch/100                                             128 ns          128 ns      5600000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RangeSearch/1000                                            194 ns          195 ns      3446154
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RangeSearch/10000                                           617 ns          614 ns      1120000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RangeSearch/100000                                         6461 ns         6417 ns       112000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::FrustumCulling/1000                                       0.747 us        0.750 us       896000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::FrustumCulling/10000                                       2.47 us         2.46 us       298667
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                            38.2 us         38.5 us        18667
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                           1182 us         1172 us          560
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                         23698 us        23438 us           30
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                       438084 us       437500 us            2
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                            38.8 us         38.1 us        17231
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                           1188 us         1172 us          560
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                          8179 us         2912 us          279
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                       128850 us       130208 us            6
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                      17.6 us         17.6 us        40727
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                      164 us          165 us         4073
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                    4880 us         4957 us          145
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                 126712 us       127232 us            7
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100                                   0.534 us        0.544 us      1120000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                  0.970 us        0.963 us       746667
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                  2.09 us         2.10 us       320000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                 5.47 us         5.44 us       112000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100                                     0.485 us        0.487 us      1445161
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/1000                                    0.814 us        0.820 us       896000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/10000                                    2.12 us         2.10 us       320000
Benchmarks::StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100000                                   8.14 us         8.20 us        89600
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                         1.09 us         1.07 us       640000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                         2.13 us         2.18 us       344615
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                         3.66 us         3.69 us       194783
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                        6.84 us         6.84 us       112000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                       55.6 us         56.2 us        10000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                       732 us          732 us          896
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                     7786 us         7812 us           90
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                   99152 us        98958 us            6
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                         2.10 us         1.95 us       344615
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                         4.39 us         4.10 us       186667
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                         9.33 us         9.21 us        74667
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                        18.8 us         16.0 us        44800
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                       87.0 us         78.5 us         8960
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                       457 us          410 us         1792
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                     3088 us         3112 us          236
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                   33409 us        32552 us           24
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    1.09 us         1.09 us       560000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    2.12 us         2.10 us       320000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    3.68 us         3.69 us       194783
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   6.84 us         6.80 us        89600
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  55.8 us         57.8 us        10000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  733 us          739 us         1120
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                7724 us         7708 us           75
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              99061 us        98214 us            7
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                    2.58 us         2.30 us       298667
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                    4.16 us         4.20 us       160000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                    10.3 us         9.03 us        64000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                   18.8 us         17.6 us        40727
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  85.0 us         85.1 us        11200
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  454 us          435 us         1545
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                3053 us         2934 us          213
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              32611 us        31250 us           26
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::PickSearch/100                                                 155 ns          157 ns      4480000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::PickSearch/1000                                                183 ns          184 ns      3733333
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::PickSearch/10000                                               646 ns          642 ns      1120000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::PickSearch/100000                                             7659 ns         7673 ns       112000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RangeSearch/100                                                103 ns          103 ns      6400000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                               135 ns          135 ns      4977778
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                              669 ns          663 ns       896000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                            6640 ns         6696 ns       112000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                          0.416 us        0.414 us      1659259
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                          2.97 us         2.98 us       235789
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                               3.39 us         3.38 us       203636
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                               110 us          109 us         5600
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                             5335 us         5312 us          100
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                          208737 us       208333 us            3
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                               3.66 us         3.69 us       194783
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                               109 us          107 us         6400
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                             1637 us         1537 us          498
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                           73315 us        28125 us           20
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                         11.0 us         11.0 us        64000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                         107 us          109 us         7467
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                       4357 us         4330 us          166
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                    472199 us       476562 us            2
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                      0.404 us        0.399 us      1723077
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                      1.08 us         1.07 us       640000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                     5.96 us         6.00 us       112000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                    69.6 us         69.8 us        11200
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                        0.421 us        0.424 us      1659259
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                        1.09 us         1.07 us       640000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                       6.03 us         6.14 us       112000
Benchmarks::StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                      82.1 us         80.2 us         8960
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                          0.817 us        0.802 us       896000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                           1.69 us         1.73 us       497778
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                           2.75 us         2.76 us       248889
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                          6.16 us         6.14 us       112000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                         49.7 us         50.0 us        10000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                         701 us          698 us          896
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                       8076 us         8125 us           75
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                    107314 us       109375 us            6
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10                                           1.45 us         1.37 us       560000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/20                                           3.28 us         3.07 us       224000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/50                                           8.35 us         8.02 us        89600
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100                                          17.3 us         14.2 us        40727
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                         98.8 us         94.2 us         7467
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                         547 us          502 us         1120
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                       4049 us         4015 us          179
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                     43677 us        41992 us           16
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                     0.795 us        0.809 us      1120000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                      1.67 us         1.67 us       448000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                      2.78 us         2.76 us       248889
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                     6.22 us         6.28 us       112000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                    49.4 us         50.0 us        10000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                    692 us          698 us          896
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                  7829 us         7812 us          112
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000               108575 us       109375 us            6
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                      1.45 us         1.45 us       560000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                      3.08 us         3.07 us       224000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                      8.25 us         7.53 us       112000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                     17.0 us         15.0 us        44800
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                    98.0 us         85.8 us         7467
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                    513 us          440 us         1493
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                  4136 us         4179 us          172
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                44391 us        42708 us           15
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::PickSearch/100                                                   157 ns          157 ns      4480000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::PickSearch/1000                                                  161 ns          164 ns      4480000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::PickSearch/10000                                                 212 ns          213 ns      3446154
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::PickSearch/100000                                                299 ns          298 ns      2357895
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RangeSearch/100                                                  109 ns          105 ns      6400000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RangeSearch/1000                                                 113 ns          115 ns      6400000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RangeSearch/10000                                                163 ns          161 ns      4072727
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RangeSearch/100000                                               424 ns          420 ns      1600000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::FrustumCulling/1000                                            0.283 us        0.283 us      2488889
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::FrustumCulling/10000                                            1.03 us         1.03 us       746667
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                                 33.3 us         33.0 us        21333
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                 671 us          663 us          896
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                              11711 us        11719 us           64
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                            215706 us       213542 us            3
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                                 33.8 us         33.7 us        21333
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                                 670 us          680 us          896
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                               3834 us         1514 us          640
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                             64798 us        65972 us            9
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                           11.7 us         11.7 us        56000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                           116 us          117 us         6400
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                         2012 us         2011 us          373
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                       40319 us        40441 us           17
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100                                        0.334 us        0.330 us      2036364
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                       0.412 us        0.414 us      1659259
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                      0.833 us        0.837 us       896000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                      1.66 us         1.67 us       448000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100                                          0.348 us        0.345 us      1947826
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedAll/1000                                         0.407 us        0.405 us      1659259
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedAll/10000                                        0.806 us        0.802 us       896000
Benchmarks::StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100000                                        2.78 us         2.76 us       248889
`;