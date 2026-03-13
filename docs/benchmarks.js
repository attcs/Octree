var BENCHMARK_DATA = `----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                                                                                  Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmarks::Base::GetNodeID/1000                                                                                       13956 ns        13811 ns        49778 items_per_second=72.4044M/s
Benchmarks::Base::GetDepthID/1000                                                                                        871 ns          872 ns       896000 items_per_second=1.14688G/s
Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000                                                                10366 ns        10463 ns        74667 items_per_second=95.5738M/s
Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000                                                                  12196 ns        12277 ns        56000 items_per_second=81.4545M/s
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                     0.578 us        0.572 us      1120000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                      1.41 us         1.41 us       497778
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                      3.73 us         3.77 us       186667
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                     8.20 us         8.16 us        74667
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                     112 us          112 us         5600
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                   2302 us         2344 us          320
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                 29104 us        28846 us           26
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                               405496 us       406250 us            2
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                      1.32 us         1.28 us       560000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                      3.17 us         3.00 us       224000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                      10.5 us         9.42 us        89600
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                     19.8 us         15.3 us        40727
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                     136 us          111 us         7467
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                   1184 us          928 us          640
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                 17886 us        14230 us           56
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                               177647 us       175781 us            4
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.605 us        0.614 us      1120000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.42 us         1.41 us       497778
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 3.81 us         3.84 us       179200
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                8.29 us         8.16 us        74667
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                113 us          112 us         6400
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000              2162 us         2148 us          320
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            27575 us        27500 us           25
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          406181 us       406250 us            2
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.28 us         1.26 us       497778
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.15 us         3.11 us       235789
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 9.87 us         8.09 us       112000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                19.3 us         15.1 us        49778
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                136 us          100 us         5600
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000              1009 us          837 us          747
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000            18348 us        16319 us           45
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          177045 us       164062 us            4
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100                                              153 ns          153 ns      4480000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/1000                                             176 ns          176 ns      4072727
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/10000                                            240 ns          246 ns      2800000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100000                                           350 ns          345 ns      2036364
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                             101 ns          103 ns      7466667
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                            126 ns          123 ns      5600000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                           184 ns          184 ns      3733333
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                          441 ns          449 ns      1600000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                       0.508 us        0.516 us      1000000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                       1.78 us         1.73 us       407273
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                            1.92 us         1.97 us       373333
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                           29.4 us         29.8 us        23579
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                          1242 us         1200 us          560
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                        20202 us        20680 us           34
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                            2.33 us         2.30 us       298667
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                           31.1 us         31.4 us        22400
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                          1241 us         1228 us          560
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                        20447 us        20680 us           34
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                      8.08 us         7.95 us        74667
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                     58.6 us         58.6 us        11200
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                    1629 us         1639 us          448
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                  29317 us        29297 us           24
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                   0.453 us        0.455 us      1544828
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                  0.547 us        0.544 us      1120000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                  1.05 us         1.05 us       640000
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                 1.96 us         1.97 us       373333
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                     0.404 us        0.399 us      1723077
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                    0.475 us        0.471 us      1493333
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                   0.975 us        0.963 us       746667
Benchmarks::StaticBVHBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                   3.76 us         3.85 us       186667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                0.695 us        0.698 us      1120000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                 1.61 us         1.60 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                 2.12 us         2.10 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                3.61 us         3.61 us       194783
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                               38.9 us         38.1 us        17231
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                               702 us          711 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                            10232 us        10010 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                          157155 us       160156 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                 1.44 us         1.48 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                 3.53 us         3.30 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                 8.49 us         8.02 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                13.0 us         11.9 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                               64.4 us         57.8 us        10000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                               499 us          500 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                             6673 us         6562 us          100
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                          117050 us       117188 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10           0.701 us        0.698 us      1120000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20            1.62 us         1.60 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50            2.13 us         2.18 us       344615
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100           4.27 us         4.24 us       165926
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000          52.0 us         51.6 us        11200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000          817 us          820 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000       11430 us        11230 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000     176126 us       175781 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10            1.48 us         1.44 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20            3.81 us         3.49 us       179200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50            8.37 us         7.11 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100           17.0 us         13.5 us        49778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000          70.4 us         66.3 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000          537 us          531 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000        7507 us         7292 us           90
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000     125784 us       125000 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                0.844 us        0.854 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                 2.71 us         2.73 us       263529
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                 4.92 us         5.00 us       100000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                7.75 us         7.85 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                               87.1 us         85.8 us         7467
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                               977 us          962 us          747
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                            25484 us        25670 us           28
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                          161396 us       164062 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/10                                 1.59 us         1.46 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/20                                 4.94 us         5.00 us       100000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/50                                 10.6 us         10.3 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/100                                17.7 us         13.3 us        44800
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/1000                               96.4 us         96.3 us         7467
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/10000                               636 us          614 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/100000                            20098 us        19761 us           34
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                           96828 us        98214 us            7
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                          0.777 us        0.767 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                           1.09 us         1.10 us       640000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                           1.94 us         1.90 us       344615
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                          3.35 us         3.37 us       213333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                         34.3 us         33.8 us        20364
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                         494 us          502 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                       5603 us         5580 us          112
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                     73070 us        72917 us            9
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                           1.36 us         1.29 us       640000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                           2.11 us         1.97 us       373333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                           4.18 us         4.17 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                          10.3 us         6.72 us       100000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                         35.8 us         32.2 us        20364
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                         357 us          361 us         2036
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                       3940 us         3928 us          179
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                     54473 us        56250 us           10
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10                              1.24 us         1.25 us       640000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/20                              3.50 us         3.59 us       213333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/50                              7.06 us         6.98 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/100                             15.8 us         15.7 us        44800
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                             167 us          167 us         4480
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                           2447 us         2400 us          280
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/10                                     2.15 us         2.15 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/20                                     3.69 us         3.69 us       194783
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/50                                     9.42 us         9.42 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/100                                    17.2 us         16.7 us        37333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/1000                                    196 us          197 us         3733
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertIntoLeaf/10000                                  3173 us         3154 us          213
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/10                                       3.05 us         3.05 us       235789
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/20                                       7.95 us         8.02 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/50                                       19.9 us         20.1 us        37333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/100                                      46.5 us         47.5 us        15448
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/1000                                      666 us          656 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::InsertUnique/10000                                   10401 us        10498 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/10                                             1.75 us         1.76 us       407273 items_per_second=5.66641M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/20                                             3.31 us         3.30 us       203636 items_per_second=6.06172M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/50                                             8.18 us         8.20 us        89600 items_per_second=6.10043M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/100                                            20.4 us         20.5 us        37333 items_per_second=4.87615M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/1000                                            662 us          684 us         1120 items_per_second=1.46286M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::Update/10000                                         10979 us        11336 us           51 items_per_second=882.162k/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                        155 ns          150 ns      4480000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                       249 ns          257 ns      2800000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                      427 ns          420 ns      1600000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                     914 ns          921 ns       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/100                              0.928 us        0.921 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/1000                              1.46 us         1.46 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/10000                             2.62 us         2.61 us       263529
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors/100000                            4.01 us         3.90 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                           4.46 us         4.45 us       154483
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                          18.3 us         18.1 us        34462
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                   1.58 us         1.57 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                  10.4 us         10.5 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                     0.637 us        0.642 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                      1.60 us         1.60 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                      2.13 us         2.13 us       344615
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                     3.68 us         3.69 us       194783
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                    40.7 us         40.8 us        17231
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                    784 us          785 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                 10826 us        10742 us           64
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                               209718 us       208333 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                      1.44 us         1.29 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                      4.00 us         4.01 us       194783
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                      8.55 us         7.53 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                     14.3 us         11.7 us        56000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                    66.3 us         62.8 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                    576 us          558 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                  7303 us         7292 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                               157056 us       160156 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.646 us        0.642 us      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.61 us         1.61 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 2.14 us         2.10 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                4.38 us         4.39 us       160000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               52.4 us         51.6 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               853 us          858 us          747
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            12254 us        12207 us           64
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          234711 us       229167 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.45 us         1.37 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.93 us         3.99 us       203636
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 8.36 us         7.32 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                16.4 us         12.6 us        56000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000               71.1 us         64.1 us        10000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000               590 us          586 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000             8255 us         8160 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          170994 us       171875 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                     0.785 us        0.767 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                      2.88 us         2.79 us       224000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                      5.38 us         5.47 us       100000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                     8.59 us         8.72 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                                    97.7 us         95.2 us         6400
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                                   1136 us         1147 us          640
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                                 30821 us        30540 us           22
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                               230003 us       229167 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10                                      1.50 us         1.29 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/20                                      5.12 us         5.00 us       100000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/50                                      10.6 us         10.7 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100                                     18.6 us         16.4 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000                                     105 us          103 us         6400
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10000                                    759 us          732 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100000                                 23171 us        23438 us           30
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                               148400 us       148438 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                               0.820 us        0.820 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                1.17 us         1.17 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                2.08 us         2.05 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                               3.61 us         3.61 us       194783
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                              37.5 us         37.7 us        18667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                              538 us          547 us         1000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                            6603 us         6597 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                          95629 us        93750 us            6
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                1.71 us         1.61 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                2.59 us         2.40 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                5.49 us         5.00 us       100000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                               9.65 us         8.20 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                              48.1 us         41.9 us        17920
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                              424 us          410 us         1600
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                            4953 us         4957 us          145
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                          78506 us        76389 us            9
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                   1.08 us         1.07 us       640000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                   3.32 us         3.30 us       203636
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                   6.61 us         6.70 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/100                                  15.3 us         15.3 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                                  170 us          169 us         4073
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                                2554 us         2567 us          280
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/10                                          2.30 us         2.29 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/20                                          4.08 us         4.08 us       172308
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/50                                          10.5 us         10.5 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/100                                         19.5 us         19.5 us        34462
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/1000                                         291 us          295 us         2489
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertIntoLeaf/10000                                       4240 us         4236 us          166
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/10                                            2.32 us         2.30 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/20                                            6.41 us         6.42 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/50                                            17.1 us         17.3 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/100                                           39.5 us         39.0 us        17231
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/1000                                           506 us          502 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::InsertUnique/10000                                         7827 us         7812 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/10                                                  1.65 us         1.65 us       407273 items_per_second=6.06174M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/20                                                  3.20 us         3.21 us       224000 items_per_second=6.23304M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/50                                                  7.85 us         7.85 us        89600 items_per_second=6.37156M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/100                                                 20.7 us         20.4 us        34462 items_per_second=4.90126M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/1000                                                 662 us          663 us          896 items_per_second=1.50905M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::Update/10000                                              11911 us        11968 us           47 items_per_second=835.556k/s
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                             144 ns          144 ns      4977778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                            210 ns          209 ns      3446154
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                           346 ns          353 ns      2036364
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                          709 ns          698 ns       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100                                   0.916 us        0.921 us       746667
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/1000                                   1.70 us         1.70 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/10000                                  2.29 us         2.29 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100000                                 3.82 us         3.92 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                                3.96 us         3.92 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                               16.4 us         16.4 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                        1.43 us         1.44 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                       10.5 us         10.5 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                      0.815 us        0.816 us       746667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                       2.12 us         2.13 us       344615
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                       2.51 us         2.51 us       248889
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                      4.40 us         4.49 us       160000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                     43.7 us         43.3 us        16593
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                     688 us          698 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                   8647 us         8750 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                107778 us       106771 us            6
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                       1.61 us         1.54 us       497778
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                       3.98 us         3.93 us       194783
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                       8.92 us         8.20 us        89600
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                      15.6 us         14.1 us        49778
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                     90.4 us         89.1 us        10000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                     558 us          558 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                   5143 us         5156 us          100
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                 53745 us        54688 us           10
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                 0.767 us        0.767 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                  2.02 us         1.99 us       344615
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                  2.58 us         2.62 us       280000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                 4.61 us         4.60 us       149333
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                39.9 us         40.1 us        17920
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                654 us          645 us          896
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000              8512 us         8333 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           106947 us       106771 us            6
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                  1.52 us         1.45 us       560000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                  4.33 us         4.00 us       160000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                  9.11 us         8.59 us       100000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                 17.1 us         15.7 us        44800
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                90.3 us         81.6 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                539 us          531 us         1000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000              5188 us         5156 us          100
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000            55112 us        54688 us           10
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10                                      0.869 us        0.858 us       746667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/20                                       3.09 us         3.11 us       235789
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/50                                       5.70 us         5.72 us       112000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100                                      9.21 us         9.21 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000                                     88.8 us         87.9 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/10000                                     956 us          962 us          747
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/100000                                  15776 us        15625 us           41
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, SeqExec>/1000000                                149194 us       148438 us            4
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10                                       1.56 us         1.54 us       497778
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/20                                       5.18 us         5.16 us       100000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/50                                       10.7 us         10.7 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100                                      23.8 us         22.2 us        34462
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000                                      126 us          120 us         5600
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/10000                                     642 us          641 us         1000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/100000                                   9542 us         9375 us           90
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::Create<6, ParExec>/1000000                                 58552 us        58239 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                              122 ns          123 ns      5600000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                             162 ns          164 ns      4480000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                            258 ns          255 ns      2635294
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                           522 ns          500 ns      1000000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100                                    0.849 us        0.854 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/1000                                    1.19 us         1.22 us       640000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/10000                                   1.80 us         1.80 us       407273
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors/100000                                  2.96 us         2.98 us       235789
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/1000                                 2.92 us         2.92 us       235789
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::GetNearestNeighbors<6>/10000                                11.2 us         11.2 us        56000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                         1.11 us         1.10 us       640000
Benchmarks::StaticPointTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                        7.06 us         7.11 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                   0.740 us        0.732 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                    1.66 us         1.65 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                    2.16 us         2.15 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                   3.62 us         3.60 us       186667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                  38.3 us         38.5 us        18667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                  701 us          698 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                               10009 us        10010 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                             169942 us       167969 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                    1.57 us         1.57 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                    4.12 us         4.08 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                    8.26 us         6.56 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                   15.5 us         12.7 us        64000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                  64.1 us         59.4 us        10000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                  515 us          500 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                6948 us         6696 us          112
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                             131307 us       132812 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10              0.751 us        0.732 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20               1.66 us         1.69 us       407273
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50               2.17 us         2.15 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100              4.32 us         4.30 us       160000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000             51.1 us         51.6 us        10000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000             805 us          802 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000          11453 us        11440 us           56
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000        187770 us       187500 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10               1.55 us         1.38 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20               4.11 us         4.01 us       179200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50               10.3 us         9.84 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100              16.4 us         12.3 us        40727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000             71.0 us         66.3 us         8960
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000             559 us          558 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000           7653 us         7533 us          112
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000        140235 us       137500 us            5
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/10                                   0.879 us        0.858 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/20                                    2.72 us         2.70 us       248889
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/50                                    4.95 us         5.00 us       100000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/100                                   7.80 us         7.67 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/1000                                  89.7 us         87.9 us         7467
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/10000                                 1077 us         1074 us          640
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/100000                               26087 us        25841 us           26
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, SeqExec>/1000000                             196437 us       195312 us            4
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/10                                    1.67 us         1.50 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/20                                    4.98 us         5.02 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/50                                    13.6 us         12.3 us        56000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/100                                   17.8 us         14.2 us        40727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/1000                                   102 us         97.7 us         6400
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/10000                                  664 us          670 us         1120
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/100000                               20956 us        20996 us           32
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Create<6, ParExec>/1000000                             114206 us       114583 us            6
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                             0.850 us        0.837 us       896000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                              1.19 us         1.20 us       640000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                              2.07 us         2.05 us       320000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                             3.56 us         3.61 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                            36.2 us         36.9 us        19478
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                            513 us          516 us         1000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                          5880 us         5938 us          100
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                        77872 us        78125 us            9
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                              1.40 us         1.29 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                              2.11 us         2.05 us       373333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                              4.37 us         4.39 us       160000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                             10.7 us         7.11 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                            37.4 us         34.3 us        18667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                            383 us          384 us         1792
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                          4223 us         4236 us          166
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                        59282 us        59659 us           11
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10                                 1.51 us         1.50 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/20                                 3.92 us         3.92 us       179200
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/50                                 7.59 us         7.53 us       112000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/100                                16.7 us         16.7 us        44800
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/1000                                171 us          173 us         4073
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10000                              2527 us         2511 us          280
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/10                                        2.51 us         2.51 us       280000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/20                                        4.20 us         4.14 us       165926
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/50                                        10.0 us         10.0 us        74667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/100                                       18.1 us         18.0 us        40727
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/1000                                       200 us          204 us         3446
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertIntoLeaf/10000                                     3223 us         3209 us          224
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/10                                          3.43 us         3.45 us       203636
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/20                                          8.34 us         8.20 us        89600
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/50                                          20.6 us         20.0 us        32000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/100                                         47.9 us         47.6 us        14452
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/1000                                         681 us          680 us          896
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::InsertUnique/10000                                      10534 us        10498 us           64
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/10                                                1.66 us         1.71 us       448000 items_per_second=5.85143M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/20                                                3.27 us         3.22 us       213333 items_per_second=6.20605M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/50                                                9.53 us         9.63 us        74667 items_per_second=5.19423M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/100                                               21.6 us         21.5 us        32000 items_per_second=4.65455M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/1000                                               676 us          680 us          896 items_per_second=1.47036M/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::Update/10000                                            15601 us        16026 us           39 items_per_second=624k/s
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/100                                           157 ns          157 ns      4480000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                          249 ns          251 ns      2800000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                         428 ns          430 ns      1600000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                        908 ns          921 ns       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/100                                 0.930 us        0.942 us       746667
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/1000                                 1.47 us         1.48 us       497778
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/10000                                2.64 us         2.62 us       280000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors/100000                               4.00 us         4.08 us       172308
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                              4.43 us         4.35 us       154483
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                             18.4 us         18.4 us        37333
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                      1.53 us         1.50 us       448000
Benchmarks::DynamicPointTree__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                     10.8 us         11.0 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                        0.676 us        0.663 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                         1.64 us         1.65 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                         2.18 us         2.14 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                        3.73 us         3.77 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                       40.9 us         40.8 us        17231
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                       797 us          802 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                    11040 us        11161 us           56
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                  224226 us       223958 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                         1.41 us         1.34 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                         3.91 us         3.68 us       203636
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                         8.49 us         7.95 us        74667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                        14.2 us         11.4 us        56000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                       67.7 us         64.2 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                       594 us          558 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                     7464 us         7639 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                  169411 us       167969 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.683 us        0.680 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    1.64 us         1.61 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    2.19 us         2.15 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   4.45 us         4.45 us       154483
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  52.1 us         51.6 us        10000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  874 us          879 us          747
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               12152 us        11998 us           56
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000             248812 us       250000 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.38 us         1.37 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                    4.11 us         4.09 us       194783
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                    9.61 us         7.67 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                   16.4 us         13.7 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  74.1 us         69.8 us        11200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  612 us          586 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                8096 us         8160 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             183700 us       187500 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10                                        0.824 us        0.816 us       746667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/20                                         2.91 us         2.92 us       235789
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/50                                         5.41 us         5.30 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100                                        8.55 us         8.54 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000                                       96.3 us         96.3 us         7467
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10000                                      1174 us         1172 us          560
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100000                                    31418 us        31250 us           21
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000000                                  256231 us       260417 us            3
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10                                         1.54 us         1.53 us       448000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/20                                         5.17 us         4.76 us       144516
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/50                                         10.7 us         10.5 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100                                        18.5 us         15.0 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000                                        107 us          103 us         8960
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10000                                       785 us          767 us          896
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100000                                    23984 us        23958 us           30
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000000                                  170080 us       167969 us            4
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                  0.891 us        0.879 us       746667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                   1.26 us         1.26 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                   2.22 us         2.20 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                  3.84 us         3.84 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                 39.6 us         40.1 us        17920
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                 561 us          558 us         1120
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                               6924 us         6771 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                            102736 us       102679 us            7
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                   1.75 us         1.73 us       407273
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                   2.64 us         2.56 us       298667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                   5.40 us         4.50 us       149333
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                                  8.60 us         8.37 us        89600
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                 49.5 us         46.1 us        16593
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                 446 us          443 us         1659
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                               5217 us         5162 us          112
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                             84689 us        83333 us            9
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10                                      1.35 us         1.35 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/20                                      3.80 us         3.75 us       179200
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/50                                      7.28 us         7.25 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/100                                     16.4 us         16.5 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/1000                                     178 us          176 us         3733
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10000                                   2641 us         2663 us          264
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/10                                             2.69 us         2.67 us       263529
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/20                                             4.57 us         4.65 us       154483
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/50                                             11.1 us         11.0 us        64000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/100                                            20.4 us         20.0 us        32000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/1000                                            298 us          298 us         2358
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertIntoLeaf/10000                                          4349 us         4424 us          166
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/10                                               2.59 us         2.57 us       280000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/20                                               6.85 us         6.98 us       112000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/50                                               18.0 us         18.0 us        40727
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/100                                              41.0 us         41.7 us        17231
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/1000                                              518 us          516 us         1000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::InsertUnique/10000                                            7986 us         7986 us           90
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/10                                                     1.48 us         1.50 us       448000 items_per_second=6.66791M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/20                                                     2.98 us         3.01 us       248889 items_per_second=6.63704M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/50                                                     8.58 us         8.58 us        74667 items_per_second=5.82767M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/100                                                    21.1 us         20.9 us        34462 items_per_second=4.79471M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/1000                                                    667 us          670 us         1120 items_per_second=1.49333M/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::Update/10000                                                 15732 us        15625 us           39 items_per_second=640k/s
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                146 ns          148 ns      4977778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                               215 ns          215 ns      3200000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                              345 ns          345 ns      1947826
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                             706 ns          711 ns      1120000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100                                      0.896 us        0.907 us       896000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/1000                                      1.32 us         1.34 us       560000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/10000                                     2.22 us         2.20 us       320000
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100000                                    3.63 us         3.69 us       194783
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                                   3.74 us         3.68 us       186667
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                                  15.8 us         15.7 us        44800
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                           1.33 us         1.32 us       497778
Benchmarks::DynamicPointTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                          9.59 us         9.63 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                         0.753 us        0.750 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                          1.95 us         1.95 us       344615
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                          2.45 us         2.46 us       280000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                         4.37 us         4.39 us       160000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                        42.1 us         41.4 us        16593
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                        678 us          684 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                      8553 us         8681 us           90
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                   106178 us       104911 us            7
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                          1.56 us         1.50 us       448000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                          4.04 us         4.10 us       179200
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                          8.96 us         7.85 us        89600
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                         15.8 us         14.8 us        49778
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                        89.2 us         85.8 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                        559 us          516 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                      5248 us         5156 us          100
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                    53171 us        53125 us           10
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    0.755 us        0.767 us      1120000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                     2.01 us         1.99 us       344615
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                     2.57 us         2.55 us       263529
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                    4.66 us         4.60 us       149333
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                   39.6 us         39.9 us        17231
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   650 us          642 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                 8537 us         8542 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              106944 us       109375 us            6
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                     1.51 us         1.54 us       497778
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                     4.33 us         4.36 us       179200
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                     8.80 us         8.37 us        89600
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                    14.4 us         13.8 us        49778
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                   93.5 us         73.2 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                   538 us          508 us         1445
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                 5177 us         5156 us          100
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000               52858 us        51562 us           10
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10                                         0.864 us        0.858 us       746667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/20                                          3.08 us         3.07 us       224000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/50                                          5.68 us         5.58 us       112000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100                                         9.19 us         9.21 us        74667
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000                                        88.3 us         90.0 us         7467
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/10000                                        950 us          941 us          747
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/100000                                     15644 us        15278 us           45
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, SeqExec>/1000000                                   148781 us       148438 us            4
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10                                          1.63 us         1.61 us       407273
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/20                                          5.35 us         5.31 us       100000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/50                                          10.8 us         10.7 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100                                         27.5 us         25.7 us        28000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000                                         125 us          110 us         6400
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/10000                                        640 us          614 us         1120
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/100000                                      9578 us         9583 us           75
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::Create<6, ParExec>/1000000                                    57972 us        56818 us           11
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                 123 ns          122 ns      4977778
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                                161 ns          160 ns      4480000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                               258 ns          257 ns      2800000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                              521 ns          500 ns      1000000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100                                       0.844 us        0.854 us       896000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/1000                                       1.19 us         1.20 us       640000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/10000                                      1.82 us         1.80 us       407273
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors/100000                                     2.96 us         2.95 us       248889
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/1000                                    2.90 us         2.92 us       235789
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::GetNearestNeighbors<6>/10000                                   11.1 us         11.2 us        64000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                            1.12 us         1.12 us       640000
Benchmarks::StaticPointTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                           7.08 us         7.11 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                  0.908 us        0.900 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                   1.72 us         1.69 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                   3.16 us         3.14 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                  6.04 us         6.14 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                 54.4 us         56.2 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                 705 us          698 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                               7787 us         7812 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                             92823 us        91518 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                   1.65 us         1.63 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                   3.29 us         3.28 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                   8.85 us         8.37 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                  15.7 us         13.2 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                 63.1 us         55.8 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                 342 us          300 us         2133
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                               3106 us         3209 us          224
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                             39401 us        39062 us           18
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10             0.912 us        0.942 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20              1.73 us         1.69 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50              3.85 us         3.93 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100             8.27 us         8.20 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000            65.1 us         65.6 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000            804 us          795 us          747
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000          8560 us         8507 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000       101450 us       102679 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10              1.67 us         1.57 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20              3.88 us         3.81 us       172308
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50              10.6 us         8.89 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100             17.4 us         14.6 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000            66.9 us         64.1 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000            361 us          353 us         1948
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000          3211 us         3228 us          213
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000        40693 us        40799 us           18
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                            0.952 us        0.942 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                             1.43 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                             2.72 us         2.70 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                            4.99 us         5.00 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                           46.8 us         46.5 us        14452
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                           619 us          614 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                         7298 us         7465 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                       77715 us        76389 us            9
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                             1.68 us         1.61 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                             3.10 us         2.72 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                             10.1 us         5.93 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                            8.36 us         8.23 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                           36.7 us         35.2 us        24889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                           355 us          338 us         2036
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                         4147 us         4142 us          166
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                       45446 us        45833 us           15
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/10                                        0.338 us        0.330 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/20                                        0.337 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/50                                        0.338 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/100                                       0.337 us        0.330 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/1000                                      0.338 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::InsertToLeaf/10000                                     0.341 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                1.25 us         1.26 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                3.25 us         3.28 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                9.87 us         9.77 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/100                               15.5 us         15.7 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                               146 us          148 us         4978
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                             1496 us         1506 us          498
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/10                                               1.99 us         2.01 us       373333 items_per_second=4.97777M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/20                                               3.72 us         3.77 us       186667 items_per_second=5.30964M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/50                                               8.38 us         8.37 us        74667 items_per_second=5.97336M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/100                                              17.0 us         16.9 us        40727 items_per_second=5.92393M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/1000                                              277 us          270 us         2489 items_per_second=3.70456M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::Update/10000                                            7819 us         7812 us           90 items_per_second=1.28M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100                                           197 ns          197 ns      3733333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/1000                                          235 ns          235 ns      2986667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/10000                                         780 ns          785 ns       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100000                                       7746 ns         7847 ns        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                          157 ns          157 ns      4480000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                         191 ns          193 ns      3733333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                        724 ns          732 ns       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                      5967 ns         5938 ns       100000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                    0.550 us        0.544 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                    3.48 us         3.53 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                         3.34 us         3.30 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                         128 us          131 us         5600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                       5768 us         5720 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                    213914 us       213542 us            3
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                         3.77 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                         131 us          131 us         5600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                       2057 us          546 us         1116
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                     75147 us        53125 us           10
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                   22.5 us         22.0 us        32000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                   179 us          181 us         4480
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                 5901 us         5781 us          100
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000              490349 us       500000 us            1
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100                                0.602 us        0.614 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                1.43 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/10000                               6.86 us         6.98 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100000                              72.0 us         72.5 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100                                  0.554 us        0.562 us      1000000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/1000                                  1.37 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/10000                                 6.95 us         7.11 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100000                                80.5 us         78.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                    0.758 us        0.767 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                     1.41 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                     2.36 us         2.41 us       298667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                    5.05 us         5.16 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                   47.7 us         46.5 us        14452
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                   694 us          698 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                 8682 us         8507 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                              124287 us       125000 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10                                     1.37 us         1.34 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/20                                     2.88 us         2.79 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/50                                     8.99 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100                                    16.7 us         13.4 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                   71.1 us         62.8 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                   442 us          433 us         1659
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                 4768 us         4667 us          154
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                               76481 us        76389 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10               0.765 us        0.750 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                1.40 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                3.00 us         2.98 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100               7.15 us         7.15 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000              58.6 us         58.6 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000              797 us          785 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000            9567 us         9583 us           75
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000         133345 us       132812 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                1.47 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                2.87 us         2.86 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                10.1 us         8.20 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100               17.9 us         14.8 us        56000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000              75.3 us         68.8 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000              469 us          460 us         1493
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000            5022 us         5000 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000          78170 us        78125 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                              0.816 us        0.837 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                               1.17 us         1.17 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                               2.12 us         2.15 us       320000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                              3.74 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                             36.4 us         36.0 us        18667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                             572 us          578 us         1000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                           6732 us         6696 us          112
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                         72607 us        72443 us           11
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                               1.43 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                               2.19 us         1.88 us       298667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                               4.35 us         4.52 us       165926
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                              11.6 us         6.72 us       100000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                             35.9 us         35.2 us        21333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                             426 us          417 us         1723
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                           4793 us         4741 us          145
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                         52281 us        53125 us           10
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/10                                          0.334 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/20                                          0.335 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/50                                          0.334 us        0.330 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/100                                         0.334 us        0.337 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/1000                                        0.335 us        0.330 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::InsertToLeaf/10000                                       0.334 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/10                                  1.15 us         1.14 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/20                                  2.87 us         2.89 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/50                                  8.42 us         8.37 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/100                                 15.4 us         15.3 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/1000                                 148 us          146 us         4480
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Insert__RebalanceMode/10000                               1909 us         1927 us          373
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/10                                                 1.69 us         1.69 us       407273 items_per_second=5.92397M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/20                                                 3.26 us         3.22 us       213333 items_per_second=6.20605M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/50                                                 8.16 us         8.20 us        89600 items_per_second=6.10043M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/100                                                14.3 us         14.4 us        49778 items_per_second=6.92563M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/1000                                                153 us          153 us         4480 items_per_second=6.51636M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::Update/10000                                              2895 us         2860 us          224 items_per_second=3.49659M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100                                             240 ns          246 ns      2986667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/1000                                            296 ns          285 ns      2357895
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/10000                                           550 ns          544 ns      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100000                                          983 ns          984 ns       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100                                            208 ns          209 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/1000                                           304 ns          300 ns      2240000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/10000                                         1005 ns         1001 ns       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100000                                       10088 ns        10254 ns        64000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/1000                                       1.13 us         1.15 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/10000                                      3.77 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                           50.2 us         50.2 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                          1366 us         1349 us          498
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                        26874 us        27043 us           26
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                      464948 us       468750 us            2
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                           50.7 us         51.6 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                          1369 us         1349 us          498
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                         9802 us         3342 us          187
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                      131314 us        84821 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                     33.8 us         34.4 us        21333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                     250 us          246 us         2800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                   6681 us         6597 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                147984 us       148438 us            4
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100                                  0.823 us        0.820 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                  1.20 us         1.20 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                 3.11 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                7.57 us         7.50 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100                                    0.686 us        0.656 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/1000                                    1.03 us         1.03 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/10000                                   2.65 us         2.64 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100000                                  10.2 us         10.3 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                       0.866 us        0.858 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                        1.75 us         1.76 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                        3.25 us         3.15 us       213333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                       6.28 us         6.28 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                      56.4 us         54.4 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                      753 us          750 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                    8247 us         8160 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                 119144 us       119792 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                        1.68 us         1.53 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                        3.91 us         3.92 us       179200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                        13.5 us         10.0 us        49778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                       16.4 us         12.9 us        44800
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                      66.5 us         60.9 us        10000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                      392 us          361 us         2036
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                    3599 us         3526 us          195
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                  63870 us        62500 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                  0.878 us        0.858 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                   1.77 us         1.80 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                   4.09 us         4.08 us       172308
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                  8.63 us         8.72 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                 65.8 us         65.6 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                 826 us          820 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               8838 us         8958 us           75
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            124717 us       122396 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                   1.66 us         1.43 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                   3.88 us         3.75 us       179200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                   10.3 us         9.53 us        83627
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                  17.7 us         15.1 us        56000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                 68.2 us         64.2 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                 404 us          410 us         1792
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000               3628 us         3686 us          195
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             63131 us        62500 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                                  1.01 us         1.00 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                  1.49 us         1.46 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                  2.86 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                                 5.31 us         5.30 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                                50.9 us         50.0 us        10000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                                670 us          663 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                              8656 us         8854 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                           109568 us       109375 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                  2.00 us         1.97 us       373333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                  3.33 us         2.78 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                  8.80 us         6.63 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                                 11.3 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                                48.9 us         42.2 us        14452
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                                431 us          420 us         1600
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                              5638 us         5720 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                            76513 us        76705 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/10                                             0.296 us        0.292 us      2357895
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/20                                             0.293 us        0.292 us      2357895
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/50                                             0.293 us        0.293 us      2240000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/100                                            0.292 us        0.292 us      2357895
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/1000                                           0.293 us        0.295 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::InsertToLeaf/10000                                          0.294 us        0.292 us      2357895
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10                                     1.10 us         1.10 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/20                                     3.10 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/50                                     9.88 us         10.0 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/100                                    15.7 us         15.7 us        44800
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/1000                                    150 us          151 us         4978
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Insert__RebalanceMode/10000                                  1566 us         1569 us          448
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/10                                                    1.92 us         1.93 us       373333 items_per_second=5.1942M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/20                                                    3.48 us         3.45 us       194783 items_per_second=5.79819M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/50                                                    7.88 us         7.85 us        89600 items_per_second=6.37156M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/100                                                   16.0 us         16.0 us        44800 items_per_second=6.23304M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/1000                                                   269 us          273 us         2635 items_per_second=3.66609M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::Update/10000                                                 7769 us         7812 us           90 items_per_second=1.28M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100                                                171 ns          173 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/1000                                               192 ns          193 ns      3733333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/10000                                              677 ns          663 ns       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100000                                            7547 ns         7533 ns       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                               135 ns          132 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                              168 ns          171 ns      4480000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                             727 ns          711 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                           6532 ns         6557 ns       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                         0.445 us        0.449 us      1600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                         3.04 us         3.05 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                              3.16 us         3.08 us       213333
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                              106 us          107 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                            5359 us         5301 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                         203994 us       203125 us            4
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                              3.54 us         3.53 us       194783
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                              107 us          107 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                            1897 us          641 us         1000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                          72492 us        48828 us           16
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                        11.9 us         12.2 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                        110 us          110 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                      4215 us         4199 us          160
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                   430949 us       437500 us            2
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                     0.430 us        0.439 us      1600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                     1.13 us         1.12 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                    6.22 us         6.42 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                   70.5 us         71.1 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                       0.448 us        0.449 us      1600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                       1.14 us         1.12 us       560000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                      6.26 us         6.14 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                     79.3 us         80.2 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                         0.889 us        0.879 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                          1.61 us         1.60 us       448000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                          2.65 us         2.67 us       263529
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                         5.50 us         5.58 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                        49.8 us         50.8 us        14452
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                        770 us          785 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                      9331 us         9375 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                   167491 us       164062 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10                                          2.34 us         2.30 us       373333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/20                                          4.99 us         5.00 us       100000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/50                                          10.5 us         10.7 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100                                         20.2 us         15.3 us        40727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                        75.3 us         69.8 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                        510 us          508 us         1445
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                      5529 us         5625 us          100
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                   115807 us       117188 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                    0.889 us        0.900 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                     1.61 us         1.60 us       448000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                     3.36 us         3.37 us       213333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                    7.74 us         7.67 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                   60.3 us         60.0 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                   821 us          837 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                 9956 us         9792 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000              175042 us       175781 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                     1.72 us         1.65 us       407273
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                     2.91 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                     9.29 us         8.58 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                    20.1 us         17.6 us        37333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                   79.7 us         73.2 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                   513 us          531 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                 5546 us         5312 us          100
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              116191 us       119792 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10                                    1.11 us         1.10 us       640000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/20                                    1.43 us         1.43 us       448000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/50                                    2.49 us         2.51 us       298667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100                                   4.28 us         4.24 us       165926
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000                                  40.3 us         39.9 us        17231
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/10000                                  624 us          609 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/100000                                8395 us         8542 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, SeqExec>/1000000                             105697 us       104167 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10                                    2.07 us         2.09 us       344615
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/20                                    2.91 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/50                                    5.84 us         5.16 us       100000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100                                   9.17 us         8.37 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000                                  48.5 us         46.5 us        15448
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/10000                                  489 us          481 us         1493
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/100000                                6407 us         6417 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__Bulk<3, ParExec>/1000000                              84669 us        84821 us            7
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/10                                               0.465 us        0.475 us      1544828
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/20                                               0.466 us        0.460 us      1493333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/50                                               0.465 us        0.471 us      1493333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/100                                              0.464 us        0.471 us      1493333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/1000                                             0.465 us        0.460 us      1493333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::InsertToLeaf/10000                                            0.465 us        0.460 us      1493333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/10                                       1.17 us         1.14 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/20                                       2.91 us         2.86 us       224000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/50                                       9.23 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/100                                      17.3 us         17.3 us        40727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/1000                                      160 us          160 us         4480
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Insert__RebalanceMode/10000                                    2099 us         2100 us          320
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/10                                                      1.62 us         1.64 us       448000 items_per_second=6.10043M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/20                                                      3.10 us         3.07 us       224000 items_per_second=6.51636M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/50                                                      7.80 us         7.85 us        89600 items_per_second=6.37156M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/100                                                     13.4 us         13.4 us        56000 items_per_second=7.46667M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/1000                                                     149 us          151 us         4978 items_per_second=6.63733M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::Update/10000                                                   2845 us         2847 us          236 items_per_second=3.51256M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100                                                  168 ns          169 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/1000                                                 178 ns          176 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/10000                                                241 ns          241 ns      2986667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100000                                               355 ns          361 ns      1947826
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100                                                 131 ns          131 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/1000                                                143 ns          144 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/10000                                               209 ns          209 ns      3446154
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100000                                              551 ns          544 ns      1120000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/1000                                           0.327 us        0.330 us      2133333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/10000                                           1.27 us         1.28 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                                50.1 us         48.8 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                885 us          899 us          747
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                             13592 us        13393 us           56
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                           222788 us       223958 us            3
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                                50.0 us         50.0 us        10000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                                884 us          879 us          747
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                              4399 us         1256 us          448
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                            64140 us        44922 us           16
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                          13.3 us         13.4 us        56000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                          124 us          123 us         5600
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                        2183 us         2246 us          320
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                      48782 us        49107 us           14
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100                                       0.357 us        0.361 us      1947826
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                      0.425 us        0.414 us      1659259
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                     0.831 us        0.837 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                     1.77 us         1.76 us       373333
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100                                         0.374 us        0.368 us      1866667
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/1000                                        0.443 us        0.449 us      1600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/10000                                       0.875 us        0.889 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100000                                       3.25 us         3.30 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10                                     0.966 us        0.963 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/20                                      1.84 us         1.81 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/50                                      3.28 us         3.22 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100                                     6.21 us         6.14 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000                                    54.6 us         54.7 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/10000                                    711 us          698 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/100000                                  7903 us         7812 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, SeqExec>/1000000                               101691 us       104911 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10                                      1.76 us         1.65 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/20                                      4.00 us         4.02 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/50                                      13.9 us         9.77 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100                                     16.1 us         13.6 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000                                    63.7 us         60.0 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/10000                                    358 us          353 us         2036
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/100000                                  3337 us         3301 us          213
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create<3, ParExec>/1000000                                48620 us        50000 us           10
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.965 us        0.963 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.80 us         1.84 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 3.96 us         3.92 us       179200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                8.34 us         8.37 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               64.0 us         62.8 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               793 us          802 us          896
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000             8664 us         8681 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000          108705 us       106771 us            6
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.67 us         1.67 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                 3.69 us         3.53 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                 8.20 us         8.20 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                18.6 us         15.0 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000               67.7 us         62.8 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000               378 us          360 us         1867
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000             3460 us         3447 us          204
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000           50244 us        51562 us           10
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                1.02 us         1.00 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                1.49 us         1.51 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                2.81 us         2.78 us       235789
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                               5.16 us         5.02 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                              48.3 us         49.7 us        14452
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                              640 us          628 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                            7329 us         7292 us           90
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                          82009 us        80357 us            7
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                1.61 us         1.53 us       407273
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                3.09 us         2.57 us       248889
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                5.33 us         4.74 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                               8.85 us         8.02 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                              38.8 us         36.8 us        18667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                              381 us          385 us         1867
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                            4431 us         4395 us          160
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                          50657 us        50000 us           10
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/10                                           0.340 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/20                                           0.340 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/50                                           0.343 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/100                                          0.344 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/1000                                         0.342 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::InsertToLeaf/10000                                        0.344 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10                                   1.55 us         1.53 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/20                                   3.75 us         3.68 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/50                                   10.8 us         11.0 us        64000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/100                                  17.0 us         16.9 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/1000                                  155 us          153 us         4480
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Insert__RebalanceMode/10000                                1584 us         1569 us          448
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/10                                                  1.47 us         1.46 us       448000 items_per_second=6.82667M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/20                                                  2.68 us         2.73 us       263529 items_per_second=7.33298M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/50                                                  5.92 us         6.00 us       112000 items_per_second=8.33488M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/100                                                 12.2 us         12.2 us        64000 items_per_second=8.192M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/1000                                                 227 us          225 us         2987 items_per_second=4.44577M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::Update/10000                                               7358 us         7292 us           90 items_per_second=1.37143M/s
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100                                              200 ns          199 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/1000                                             238 ns          235 ns      2986667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/10000                                            781 ns          785 ns       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::PickSearch/100000                                          7679 ns         7673 ns        89600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100                                             160 ns          160 ns      4480000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/1000                                            194 ns          193 ns      3733333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/10000                                           729 ns          732 ns       896000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RangeSearch/100000                                         5998 ns         5999 ns       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/1000                                       0.551 us        0.547 us      1000000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::FrustumCulling/10000                                       3.47 us         3.45 us       194783
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                            3.35 us         3.37 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                            130 us          131 us         5600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                          5775 us         5781 us          100
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                       213636 us       213542 us            3
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100                            3.78 us         3.77 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                            130 us          131 us         5600
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                          2052 us          664 us         1365
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                        70595 us        50000 us           10
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                      23.4 us         23.5 us        29867
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                      183 us          184 us         3733
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                    5919 us         5859 us          112
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                 506293 us       500000 us            1
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100                                   0.614 us        0.628 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/1000                                   1.43 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/10000                                  6.86 us         6.84 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedFirst/100000                                 71.5 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100                                     0.558 us        0.558 us      1120000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/1000                                     1.37 us         1.38 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/10000                                    6.90 us         6.70 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Regular_WithReverseMap::RayIntersectedAll/100000                                   79.8 us         78.5 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10                                       0.813 us        0.820 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/20                                        1.49 us         1.51 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/50                                        2.47 us         2.46 us       280000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100                                       5.29 us         5.16 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000                                      47.6 us         48.1 us        14933
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/10000                                      700 us          698 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/100000                                    8837 us         8958 us           75
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                 134181 us       135417 us            6
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10                                        1.56 us         1.50 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/20                                        3.13 us         2.85 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/50                                        8.15 us         7.25 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100                                       17.2 us         13.8 us        37333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000                                      72.1 us         61.0 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/10000                                      463 us          465 us         1545
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/100000                                    5119 us         5000 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create<3, ParExec>/1000000                                  90168 us        89286 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                  0.819 us        0.820 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                   1.48 us         1.53 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                   3.13 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                  7.36 us         7.50 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                 57.3 us         57.8 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                 788 us          785 us          896
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000               9628 us         9792 us           75
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000            144430 us       143750 us            5
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                   1.54 us         1.50 us       448000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                   3.21 us         3.07 us       224000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                   9.77 us         10.0 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                  18.7 us         14.3 us        44800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                 77.5 us         73.2 us         8960
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                 498 us          497 us         1445
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000               5375 us         5312 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000             92653 us        91518 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                 0.915 us        0.921 us       746667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                  1.26 us         1.23 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                  2.26 us         2.29 us       320000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                 4.01 us         4.01 us       179200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                38.5 us         39.3 us        18667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                597 us          600 us         1120
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                              7020 us         7118 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                            78968 us        78125 us            9
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10                                  1.40 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/20                                  2.67 us         2.49 us       344615
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/50                                  4.49 us         4.45 us       154483
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100                                 10.8 us         6.56 us       112000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                37.6 us         36.8 us        18667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                460 us          465 us         1545
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100000                              5063 us         5000 us          100
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                            58368 us        58239 us           11
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/10                                             0.338 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/20                                             0.343 us        0.344 us      2133333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/50                                             0.342 us        0.329 us      1947826
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/100                                            0.340 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/1000                                           0.341 us        0.338 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::InsertToLeaf/10000                                          0.340 us        0.345 us      2036364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/10                                     1.44 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/20                                     3.36 us         3.38 us       203636
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/50                                     9.39 us         9.42 us        74667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/100                                    16.9 us         16.9 us        40727
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/1000                                    156 us          157 us         4480
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Insert__RebalanceMode/10000                                  1997 us         1993 us          345
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/10                                                    1.31 us         1.29 us       497778 items_per_second=7.77019M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/20                                                    2.48 us         2.51 us       280000 items_per_second=7.96444M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/50                                                    6.08 us         6.14 us       112000 items_per_second=8.14545M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/100                                                   10.5 us         10.7 us        74667 items_per_second=9.36998M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/1000                                                   117 us          117 us         6400 items_per_second=8.53333M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::Update/10000                                                 2484 us         2486 us          264 items_per_second=4.02286M/s
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100                                                241 ns          246 ns      2986667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/1000                                               296 ns          292 ns      2357895
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/10000                                              544 ns          544 ns      1120000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::PickSearch/100000                                             979 ns          977 ns       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100                                               208 ns          209 ns      3446154
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/1000                                              310 ns          305 ns      2357895
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/10000                                            1025 ns         1025 ns       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RangeSearch/100000                                          10141 ns         9766 ns        64000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/1000                                          1.12 us         1.12 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::FrustumCulling/10000                                         3.79 us         3.85 us       186667
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                              51.0 us         51.6 us        10000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                             1423 us         1443 us          498
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                           27660 us        27500 us           25
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                         474070 us       476562 us            2
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100                              52.2 us         51.6 us        11200
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                             1432 us         1443 us          498
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                           10138 us         3294 us          204
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                         132691 us        89286 us            7
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                        32.9 us         31.5 us        20364
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                        248 us          246 us         2800
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                      6724 us         6771 us           90
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                   147571 us       148438 us            4
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100                                     0.819 us        0.820 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/1000                                     1.20 us         1.20 us       560000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/10000                                    3.08 us         3.08 us       213333
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedFirst/100000                                   7.55 us         7.50 us        89600
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100                                       0.670 us        0.663 us       896000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/1000                                       1.03 us         1.03 us       640000
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/10000                                      2.63 us         2.67 us       263529
Benchmarks::DynamicBoxTree__MinPoint_Loose_WithReverseMap::RayIntersectedAll/100000                                     10.2 us         10.3 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10                                          0.909 us        0.924 us       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/20                                           1.78 us         1.80 us       407273
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/50                                           3.33 us         3.37 us       213333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100                                          6.32 us         6.42 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000                                         57.3 us         57.2 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/10000                                         766 us          767 us         1120
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/100000                                       8474 us         8333 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, SeqExec>/1000000                                    130238 us       130208 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10                                           1.68 us         1.59 us       560000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/20                                           3.90 us         3.84 us       179200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/50                                           13.6 us         10.9 us        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100                                          15.8 us         12.9 us        44800
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000                                         68.2 us         62.8 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/10000                                         407 us          410 us         1792
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/100000                                       3786 us         3760 us          187
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create<3, ParExec>/1000000                                     73930 us        72917 us            9
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                     0.905 us        0.900 us       746667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                      1.80 us         1.80 us       373333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                      4.11 us         4.05 us       165926
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                     8.76 us         8.54 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                    66.0 us         68.4 us        11200
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                    834 us          837 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                  9069 us         9028 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000               137943 us       137500 us            5
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                      1.67 us         1.57 us       497778
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                      3.93 us         3.93 us       186667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                      14.8 us         11.2 us        44800
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                     17.5 us         13.8 us        40727
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                    70.1 us         66.3 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                    416 us          417 us         1723
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                  3798 us         3666 us          179
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                74454 us        72443 us           11
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                     1.06 us         1.07 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                     1.56 us         1.53 us       448000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                     2.97 us         3.05 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                    5.41 us         5.30 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                   51.3 us         51.6 us        10000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                   686 us          698 us         1120
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                                 8750 us         8854 us           90
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                              115776 us       117188 us            6
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10                                     2.12 us         2.13 us       373333
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/20                                     3.34 us         2.85 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/50                                     9.46 us         8.37 us        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100                                    11.3 us         9.77 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                   51.9 us         45.0 us        14933
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                   456 us          415 us         1545
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/100000                                 5974 us         5999 us          112
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                               84552 us        83333 us            9
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/10                                                0.299 us        0.293 us      2240000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/20                                                0.299 us        0.298 us      2357895
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/50                                                0.299 us        0.298 us      2357895
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/100                                               0.298 us        0.301 us      2488889
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/1000                                              0.298 us        0.293 us      2240000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::InsertToLeaf/10000                                             0.297 us        0.293 us      2240000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10                                        1.41 us         1.41 us       497778
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/20                                        3.57 us         3.53 us       194783
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/50                                        10.7 us         10.7 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/100                                       16.8 us         16.4 us        44800
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/1000                                       158 us          160 us         4480
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Insert__RebalanceMode/10000                                     1650 us         1651 us          407
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/10                                                       1.39 us         1.38 us       497778 items_per_second=7.24041M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/20                                                       2.47 us         2.46 us       280000 items_per_second=8.14545M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/50                                                       5.41 us         5.47 us       100000 items_per_second=9.14286M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/100                                                      11.2 us         11.2 us        64000 items_per_second=8.90435M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/1000                                                      217 us          215 us         3200 items_per_second=4.65455M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::Update/10000                                                    7300 us         7254 us          112 items_per_second=1.37846M/s
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/100                                                   165 ns          165 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/1000                                                  193 ns          190 ns      3446154
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/10000                                                 675 ns          663 ns       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::PickSearch/100000                                               7487 ns         7499 ns        89600
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100                                                  134 ns          134 ns      5600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/1000                                                 167 ns          169 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/10000                                                773 ns          767 ns       896000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RangeSearch/100000                                              6821 ns         6696 ns        74667
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/1000                                            0.442 us        0.449 us      1600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::FrustumCulling/10000                                            3.04 us         3.05 us       235789
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100                                 3.15 us         3.21 us       224000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/1000                                 105 us          105 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/10000                               5348 us         5312 us          100
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<SeqExec>/100000                            204772 us       208333 us            3
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100                                 3.53 us         3.53 us       194783
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/1000                                 107 us          107 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/10000                               1888 us          750 us          896
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection<ParExec>/100000                             72196 us        49805 us           16
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100                           12.0 us         12.0 us        64000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/1000                           112 us          112 us         6400
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/10000                         4227 us         4142 us          166
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::CollisionDetection__WithOtherTree/100000                      429017 us       429688 us            2
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100                                        0.433 us        0.430 us      1600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/1000                                        1.12 us         1.12 us       640000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/10000                                       6.08 us         6.09 us       100000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedFirst/100000                                      70.7 us         69.8 us         8960
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100                                          0.448 us        0.459 us      1600000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/1000                                          1.14 us         1.12 us       560000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/10000                                         6.24 us         6.28 us       112000
Benchmarks::DynamicBoxTree__MBR_Regular_WithReverseMap::RayIntersectedAll/100000                                        78.7 us         78.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10                                            0.751 us        0.750 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/20                                             1.46 us         1.48 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/50                                             2.51 us         2.51 us       280000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100                                            5.45 us         5.44 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000                                           50.8 us         50.0 us        10000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/10000                                           775 us          767 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/100000                                         9634 us         9583 us           75
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, SeqExec>/1000000                                      179873 us       175781 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10                                             1.42 us         1.44 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/20                                             2.87 us         2.79 us       280000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/50                                             8.70 us         8.02 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100                                            18.5 us         14.4 us        64000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000                                           75.8 us         71.5 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/10000                                           526 us          516 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/100000                                         5896 us         5938 us          100
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create<3, ParExec>/1000000                                      127530 us       127604 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10                       0.750 us        0.750 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/20                        1.46 us         1.46 us       448000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/50                        3.27 us         3.22 us       213333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100                       7.68 us         7.50 us        89600
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                      59.6 us         58.6 us        11200
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                      847 us          854 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                   10350 us        10254 us           64
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000                 188594 us       187500 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10                        1.44 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/20                        2.89 us         2.64 us       248889
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/50                        8.68 us         7.95 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100                       20.8 us         16.0 us        44800
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000                      81.1 us         80.2 us         8960
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/10000                      536 us          516 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/100000                    5915 us         5999 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                 128897 us       128125 us            5
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10                                      0.947 us        0.928 us       640000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/20                                       1.33 us         1.37 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/50                                       2.42 us         2.40 us       280000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100                                      4.31 us         4.30 us       160000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000                                     42.0 us         42.4 us        16593
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/10000                                     649 us          656 us         1120
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/100000                                   8623 us         8681 us           90
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, SeqExec>/1000000                                112667 us       111979 us            6
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10                                       1.86 us         1.84 us       407273
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/20                                       2.79 us         2.46 us       280000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/50                                       5.76 us         4.88 us       112000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100                                      9.50 us         8.30 us        64000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000                                     49.5 us         37.7 us        14933
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/10000                                     514 us          516 us         1000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/100000                                   6662 us         6557 us          112
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__Bulk<3, ParExec>/1000000                                 92417 us        91518 us            7
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/10                                                  0.300 us        0.298 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/20                                                  0.298 us        0.298 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/50                                                  0.300 us        0.298 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/100                                                 0.300 us        0.298 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/1000                                                0.298 us        0.298 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::InsertToLeaf/10000                                               0.299 us        0.298 us      2357895
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/10                                          1.26 us         1.26 us       560000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/20                                          3.11 us         3.08 us       213333
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/50                                          9.07 us         9.00 us        74667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/100                                         16.5 us         16.7 us        44800
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/1000                                         157 us          157 us         4480
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Insert__RebalanceMode/10000                                       2075 us         2083 us          345
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/10                                                         1.18 us         1.17 us       560000 items_per_second=8.53333M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/20                                                         2.30 us         2.30 us       298667 items_per_second=8.68849M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/50                                                         5.69 us         5.72 us       112000 items_per_second=8.74146M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/100                                                        9.73 us         9.63 us        74667 items_per_second=10.3885M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/1000                                                        108 us          110 us         6400 items_per_second=9.10222M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::Update/10000                                                      2404 us         2344 us          280 items_per_second=4.26667M/s
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/100                                                     171 ns          174 ns      4480000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/1000                                                    193 ns          196 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/10000                                                   257 ns          256 ns      2986667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::PickSearch/100000                                                  516 ns          500 ns      1000000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100                                                    183 ns          173 ns      4977778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/1000                                                   203 ns          184 ns      4072727
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/10000                                                  265 ns          258 ns      2240000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RangeSearch/100000                                                 677 ns          680 ns       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/1000                                              0.346 us        0.342 us      1600000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::FrustumCulling/10000                                              1.37 us         1.32 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100                                   50.3 us         44.4 us        18667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/1000                                  2173 us         1535 us          560
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/10000                                29290 us        21959 us           37
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<SeqExec>/100000                              387843 us       359375 us            2
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100                                   40.0 us         39.1 us        20364
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/1000                                   902 us          854 us          896
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/10000                                 6734 us         3397 us          345
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection<ParExec>/100000                              104807 us        61080 us           11
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100                             33.4 us         24.1 us        29867
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/1000                             780 us          252 us         3098
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/10000                          12110 us         5729 us           90
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::CollisionDetection__WithOtherTree/100000                        519928 us       164062 us            4
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100                                           1.80 us        0.531 us      1000000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/1000                                          1.77 us        0.753 us       746667
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/10000                                         3.59 us         1.57 us       407273
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedFirst/100000                                        6.30 us         3.67 us       165926
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100                                            0.583 us        0.572 us      1120000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/1000                                           0.752 us        0.698 us       896000
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/10000                                           1.34 us         1.35 us       497778
Benchmarks::DynamicBoxTree__MBR_Loose_WithReverseMap::RayIntersectedAll/100000                                          4.94 us         5.00 us       100000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                    1.19 us         1.22 us       497778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                    1.75 us         1.72 us       344615
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                    3.32 us         3.30 us       213333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                   5.68 us         5.72 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                  51.6 us         50.0 us        10000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                  727 us          732 us          896
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                7385 us         7292 us           90
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                              79515 us        79861 us            9
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10                                    1.69 us         1.65 us       407273
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/20                                    3.61 us         3.49 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/50                                    12.1 us         10.0 us        64000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100                                   16.7 us         15.0 us        40727
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                  65.4 us         60.0 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                  363 us          360 us         1867
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                2821 us         2663 us          264
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                              28155 us        27644 us           26
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10              0.869 us        0.879 us       746667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20               1.70 us         1.69 us       407273
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50               3.11 us         3.14 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100              5.84 us         5.86 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000             52.2 us         51.6 us        11200
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000             704 us          711 us         1120
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000           7273 us         7254 us          112
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000         79667 us        78125 us            9
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10               1.66 us         1.50 us       407273
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20               3.68 us         3.49 us       224000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50               12.4 us         10.3 us        89600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100              15.9 us         14.1 us        49778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000             65.4 us         61.0 us         8960
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000             385 us          384 us         2036
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000           2906 us         2781 us          236
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000         29313 us        26442 us           26
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100                                            165 ns          164 ns      4480000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/1000                                           197 ns          186 ns      3446154
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/10000                                          702 ns          711 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::PickSearch/100000                                        7728 ns         7673 ns        89600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100                                           100 ns          103 ns      7466667
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/1000                                          127 ns          126 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/10000                                         647 ns          642 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RangeSearch/100000                                       5937 ns         5999 ns       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/1000                                     0.465 us        0.471 us      1493333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::FrustumCulling/10000                                     3.28 us         3.30 us       203636
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                          3.32 us         3.37 us       213333
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                          127 us          126 us         4978
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                        5730 us         5781 us          100
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                     219496 us       223958 us            3
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                          3.66 us         3.69 us       194783
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                          127 us          128 us         5600
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                        1990 us         1689 us          407
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                      86204 us        31994 us           21
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                    13.3 us         13.2 us        49778
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                    143 us          141 us         4978
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                  5944 us         5859 us          112
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000               549693 us       546875 us            1
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100                                 0.441 us        0.439 us      1600000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                 1.28 us         1.28 us       560000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                6.72 us         6.70 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedFirst/100000                               72.0 us         71.5 us         8960
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100                                   0.449 us        0.449 us      1600000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/1000                                   1.27 us         1.28 us       560000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/10000                                  6.78 us         6.84 us       112000
Benchmarks::StaticBoxTree__MinPoint_Regular_WithoutReverseMap::RayIntersectedAll/100000                                 80.3 us         78.5 us         8960
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                     0.731 us        0.732 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                      1.38 us         1.38 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                      2.29 us         2.30 us       298667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                     4.77 us         4.76 us       144516
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                    44.4 us         42.5 us        15448
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                    649 us          656 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                  7258 us         7254 us          112
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                83610 us        83333 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10                                      1.66 us         1.56 us       560000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/20                                      2.86 us         2.51 us       280000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/50                                      8.81 us         8.72 us        89600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100                                     18.8 us         13.4 us        37333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                    76.8 us         72.5 us        11200
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                    463 us          459 us         1600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                  3907 us         3753 us          179
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                42626 us        42763 us           19
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                0.735 us        0.750 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                 1.55 us         1.53 us       448000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                 2.31 us         2.30 us       298667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                5.03 us         5.00 us       100000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000               45.7 us         45.5 us        15448
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000               665 us          670 us         1120
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000             7405 us         7394 us          112
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000           82715 us        83333 us            9
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                 1.43 us         1.38 us       497778
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                 2.86 us         2.68 us       280000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                 8.49 us         7.85 us        89600
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                18.6 us         15.6 us        56000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000               77.7 us         68.0 us         8960
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000               463 us          414 us         1659
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000             4115 us         4047 us          166
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000           43493 us        42969 us           16
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100                                              175 ns          176 ns      4072727
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/1000                                             196 ns          195 ns      3200000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/10000                                            329 ns          330 ns      2133333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::PickSearch/100000                                           574 ns          578 ns      1000000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100                                             131 ns          131 ns      5600000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/1000                                            197 ns          195 ns      3446154
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/10000                                           623 ns          628 ns      1120000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RangeSearch/100000                                         6348 ns         6278 ns       112000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/1000                                       0.770 us        0.767 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::FrustumCulling/10000                                       2.52 us         2.49 us       263529
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                            37.1 us         36.8 us        18667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                           1229 us         1228 us          560
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                         24402 us        24554 us           28
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                       458334 us       460938 us            2
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                            38.0 us         37.7 us        18667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                           1238 us         1228 us          560
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                          8385 us         2545 us          264
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                       128431 us       121875 us            5
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                      19.9 us         19.5 us        34462
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                      174 us          173 us         4073
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                    5153 us         5156 us          100
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                 119935 us       119792 us            6
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100                                   0.491 us        0.500 us      1000000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                  0.861 us        0.858 us       746667
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                  1.98 us         1.97 us       373333
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                 5.19 us         5.16 us       100000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100                                     0.490 us        0.497 us      1445161
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/1000                                    0.817 us        0.820 us       896000
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/10000                                    2.12 us         2.13 us       344615
Benchmarks::StaticBoxTree__MinPoint_Loose_WithoutReverseMap::RayIntersectedAll/100000                                   8.12 us         8.16 us        74667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10                                        0.961 us        0.952 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/20                                         2.20 us         2.20 us       320000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/50                                         3.85 us         3.85 us       186667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100                                        7.04 us         7.11 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000                                       56.1 us         56.2 us        10000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/10000                                       741 us          725 us         1120
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/100000                                     8133 us         7812 us           90
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, SeqExec>/1000000                                  106415 us        96354 us            6
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10                                         1.81 us         1.73 us       407273
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/20                                         4.79 us         4.74 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/50                                         9.81 us         8.54 us        64000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100                                        20.6 us         18.6 us        34462
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000                                       88.9 us         83.7 us         7467
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/10000                                       455 us          433 us         1659
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/100000                                     3305 us         3374 us          213
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create<3, ParExec>/1000000                                   35065 us        34226 us           21
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                   0.998 us         1.00 us       746667
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                    2.20 us         2.20 us       320000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                    3.81 us         3.84 us       179200
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                   7.08 us         6.98 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                  56.1 us         56.2 us        10000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                  731 us          725 us         1120
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                7676 us         7812 us           90
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000             100794 us       101562 us            6
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                    1.65 us         1.57 us       448000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                    4.31 us         4.33 us       165926
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                    9.84 us         9.42 us        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                   20.3 us         19.0 us        56000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                  87.6 us         83.7 us         7467
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                  458 us          426 us         1723
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                3118 us         3138 us          249
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000              34562 us        33717 us           19
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100                                                 163 ns          157 ns      4072727
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/1000                                                206 ns          205 ns      3733333
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/10000                                               652 ns          637 ns      1079054
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::PickSearch/100000                                             7589 ns         7673 ns        89600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100                                                107 ns          105 ns      6400000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/1000                                               133 ns          132 ns      4977778
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/10000                                              606 ns          600 ns      1120000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RangeSearch/100000                                            5739 ns         5720 ns       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/1000                                          0.432 us        0.433 us      1659259
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::FrustumCulling/10000                                          3.02 us         3.07 us       224000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100                               3.12 us         3.14 us       224000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/1000                               103 us          103 us         6400
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/10000                             5320 us         5312 us          100
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<SeqExec>/100000                          206060 us       208333 us            3
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100                               3.54 us         3.53 us       194783
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/1000                               101 us          103 us         7467
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/10000                             1625 us         1395 us          448
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection<ParExec>/100000                           69274 us        30649 us           26
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100                         11.8 us         12.0 us        64000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                         111 us          109 us         5600
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                       4399 us         4492 us          160
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                    462318 us       460938 us            2
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100                                      0.420 us        0.414 us      1659259
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/1000                                      1.10 us         1.10 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/10000                                     5.99 us         6.00 us       112000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedFirst/100000                                    70.0 us         69.8 us         8960
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100                                        0.423 us        0.414 us      1659259
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/1000                                        1.09 us         1.10 us       640000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/10000                                       6.03 us         6.09 us       100000
Benchmarks::StaticBoxTree__MBR_Regular_WithoutReverseMap::RayIntersectedAll/100000                                      76.9 us         76.7 us         8960
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10                                          0.825 us        0.820 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/20                                           1.99 us         1.97 us       373333
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/50                                           3.05 us         3.07 us       224000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100                                          6.18 us         6.14 us       112000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000                                         50.5 us         50.8 us        14452
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/10000                                         698 us          711 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/100000                                       8482 us         8333 us           75
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, SeqExec>/1000000                                    114892 us       117188 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10                                           1.53 us         1.44 us       497778
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/20                                           4.31 us         4.19 us       179200
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/50                                           9.31 us         8.37 us        74667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100                                          18.0 us         14.2 us        37333
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000                                         99.6 us         80.2 us         8960
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/10000                                         564 us          544 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/100000                                       4161 us         4088 us          172
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create<3, ParExec>/1000000                                     43358 us        42708 us           15
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10                     0.842 us        0.837 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/20                      1.90 us         1.90 us       344615
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/50                      2.97 us         2.98 us       235789
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100                     6.29 us         6.28 us       112000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000                    50.0 us         49.7 us        14452
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/10000                    689 us          698 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/100000                  7873 us         7986 us           90
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, SeqExec>/1000000               105665 us       104167 us            6
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10                      1.62 us         1.50 us       407273
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/20                      3.73 us         3.58 us       165926
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/50                      9.10 us         8.54 us        89600
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100                     17.4 us         16.7 us        37333
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000                    99.6 us         92.4 us         8960
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/10000                    522 us          516 us         1000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/100000                  4214 us         4087 us          195
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::Create__OversizedModelspace<3, ParExec>/1000000                42665 us        40799 us           18
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100                                                   154 ns          153 ns      4480000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/1000                                                  163 ns          165 ns      4072727
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/10000                                                 215 ns          210 ns      3200000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::PickSearch/100000                                                303 ns          300 ns      2240000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100                                                  105 ns          107 ns      7466667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/1000                                                 113 ns          115 ns      6400000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/10000                                                163 ns          165 ns      4072727
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RangeSearch/100000                                               429 ns          430 ns      1600000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/1000                                            0.282 us        0.283 us      2488889
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::FrustumCulling/10000                                            1.02 us        0.984 us       746667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100                                 33.6 us         33.7 us        21333
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/1000                                 674 us          670 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/10000                              11995 us        11998 us           56
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<SeqExec>/100000                            218088 us       223958 us            3
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100                                 34.3 us         34.5 us        20364
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/1000                                 678 us          684 us         1120
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/10000                               4059 us         1945 us          747
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection<ParExec>/100000                             67195 us        66761 us           11
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100                           13.1 us         13.1 us        56000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/1000                           120 us          120 us         6400
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/10000                         2161 us         2174 us          345
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::CollisionDetection__WithOtherTree/100000                       45095 us        44792 us           15
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100                                        0.356 us        0.353 us      1947826
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/1000                                       0.413 us        0.414 us      1659259
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/10000                                      0.757 us        0.767 us       896000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedFirst/100000                                      1.49 us         1.50 us       448000
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100                                          0.366 us        0.361 us      1947826
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/1000                                         0.421 us        0.414 us      1659259
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/10000                                        0.836 us        0.816 us       746667
Benchmarks::StaticBoxTree__MBR_Loose_WithoutReverseMap::RayIntersectedAll/100000                                        2.84 us         2.83 us       248889
`;