window.BENCHMARK_DATA = {
  "lastUpdate": 1737327665998,
  "repoUrl": "https://github.com/attcs/Octree",
  "entries": {
    "C++ Benchmark": [
      {
        "commit": {
          "author": {
            "email": "att.csikos@gmail.com",
            "name": "attcs",
            "username": "attcs"
          },
          "committer": {
            "email": "att.csikos@gmail.com",
            "name": "attcs",
            "username": "attcs"
          },
          "distinct": true,
          "id": "bfba1aba526632baae97a80c01e9d2408624a474",
          "message": "Minor changes",
          "timestamp": "2025-01-19T22:58:20+01:00",
          "tree_id": "b9607909b199058bd74945610468f910e7652806",
          "url": "https://github.com/attcs/Octree/commit/bfba1aba526632baae97a80c01e9d2408624a474"
        },
        "date": 1737324024910,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 3717.1772041367703,
            "unit": "ns/iter",
            "extra": "iterations: 183235\ncpu: 3716.9407864218074 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 1590.31736478156,
            "unit": "ns/iter",
            "extra": "iterations: 441785\ncpu: 1590.2963455074303 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/10",
            "value": 344.21204681941794,
            "unit": "ns/iter",
            "extra": "iterations: 2026510\ncpu: 344.16638506595086 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/20",
            "value": 466.99701697365,
            "unit": "ns/iter",
            "extra": "iterations: 1522950\ncpu: 466.93193735841606 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/50",
            "value": 1359.8910880113929,
            "unit": "ns/iter",
            "extra": "iterations: 514195\ncpu: 1359.8377697177132 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/100",
            "value": 2192.795507178512,
            "unit": "ns/iter",
            "extra": "iterations: 318241\ncpu: 2192.7130413743057 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/1000",
            "value": 33082.769559901964,
            "unit": "ns/iter",
            "extra": "iterations: 21268\ncpu: 33080.07649990599 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/10000",
            "value": 523401.84265474416,
            "unit": "ns/iter",
            "extra": "iterations: 1341\ncpu: 523374.2117822518 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/100000",
            "value": 4694870.466666809,
            "unit": "ns/iter",
            "extra": "iterations: 150\ncpu: 4694540.939999996 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/1000000",
            "value": 50432050.071427576,
            "unit": "ns/iter",
            "extra": "iterations: 14\ncpu: 50428587.07142856 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/10",
            "value": 1735.7274927179017,
            "unit": "ns/iter",
            "extra": "iterations: 401670\ncpu: 1735.6095152737298 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/20",
            "value": 2532.842072729548,
            "unit": "ns/iter",
            "extra": "iterations: 275019\ncpu: 2532.615499292775 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/50",
            "value": 4598.392851759792,
            "unit": "ns/iter",
            "extra": "iterations: 152597\ncpu: 4597.994344580828 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/100",
            "value": 6306.293418515616,
            "unit": "ns/iter",
            "extra": "iterations: 110978\ncpu: 6305.12201517417 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/1000",
            "value": 47839.69412702689,
            "unit": "ns/iter",
            "extra": "iterations: 15052\ncpu: 47795.56264948183 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/10000",
            "value": 361161.42480620433,
            "unit": "ns/iter",
            "extra": "iterations: 1935\ncpu: 360884.75762273907 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/100000",
            "value": 2210317.452830124,
            "unit": "ns/iter",
            "extra": "iterations: 318\ncpu: 2210251.490566035 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/1000000",
            "value": 25318690.85185092,
            "unit": "ns/iter",
            "extra": "iterations: 27\ncpu: 25192196.51851848 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 3465.3647504775795,
            "unit": "ns/iter",
            "extra": "iterations: 202607\ncpu: 3465.222326967973 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 6735.863175059995,
            "unit": "ns/iter",
            "extra": "iterations: 104250\ncpu: 6735.590465227822 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 18021.71685029623,
            "unit": "ns/iter",
            "extra": "iterations: 38937\ncpu: 18020.439967126356 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 43382.16888010438,
            "unit": "ns/iter",
            "extra": "iterations: 16189\ncpu: 43379.91531286678 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 394928.2685393301,
            "unit": "ns/iter",
            "extra": "iterations: 1780\ncpu: 394916.58764044923 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 3075010.623893908,
            "unit": "ns/iter",
            "extra": "iterations: 226\ncpu: 3074909.592920366 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 518.1301856459404,
            "unit": "ns/iter",
            "extra": "iterations: 1344818\ncpu: 518.1233014430206 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 1719.1116492523906,
            "unit": "ns/iter",
            "extra": "iterations: 409434\ncpu: 1719.0259724400023 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 3101.0273485513194,
            "unit": "ns/iter",
            "extra": "iterations: 227800\ncpu: 3100.845083406487 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 9967.239003717285,
            "unit": "ns/iter",
            "extra": "iterations: 71024\ncpu: 9966.541915408905 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 142857.7158671622,
            "unit": "ns/iter",
            "extra": "iterations: 4878\ncpu: 142844.95920459175 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1719351.0466830216,
            "unit": "ns/iter",
            "extra": "iterations: 407\ncpu: 1719195.1031941026 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 640.3006070602903,
            "unit": "ns/iter",
            "extra": "iterations: 1096102\ncpu: 640.2539891360459 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 1220.3881258677252,
            "unit": "ns/iter",
            "extra": "iterations: 574762\ncpu: 1220.391758675762 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 3080.7206391313835,
            "unit": "ns/iter",
            "extra": "iterations: 227684\ncpu: 3080.573720595228 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 6088.7110255920925,
            "unit": "ns/iter",
            "extra": "iterations: 115114\ncpu: 6088.519458971129 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 62323.26113268926,
            "unit": "ns/iter",
            "extra": "iterations: 11071\ncpu: 62318.39671213096 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1070411.971962637,
            "unit": "ns/iter",
            "extra": "iterations: 642\ncpu: 1070334.054517132 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 388699.7267668381,
            "unit": "ns/iter",
            "extra": "iterations: 1797\ncpu: 388661.25208680955 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "att.csikos@gmail.com",
            "name": "attcs",
            "username": "attcs"
          },
          "committer": {
            "email": "att.csikos@gmail.com",
            "name": "attcs",
            "username": "attcs"
          },
          "distinct": true,
          "id": "7a56a882784eb186b397551abb9249d5db502f8f",
          "message": "Add Point:: InsertUnique, FrustumCulling",
          "timestamp": "2025-01-19T23:57:32+01:00",
          "tree_id": "4ed70f351529a7716699e883f5fa2a6e3146f2d5",
          "url": "https://github.com/attcs/Octree/commit/7a56a882784eb186b397551abb9249d5db502f8f"
        },
        "date": 1737327665179,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1781.9087388342036,
            "unit": "ns/iter",
            "extra": "iterations: 394286\ncpu: 1781.9074199946235 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 1567.2734218867695,
            "unit": "ns/iter",
            "extra": "iterations: 447338\ncpu: 1567.2562871922355 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/10",
            "value": 289.08160476892783,
            "unit": "ns/iter",
            "extra": "iterations: 2424025\ncpu: 289.07779746496016 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/20",
            "value": 350.4888275164583,
            "unit": "ns/iter",
            "extra": "iterations: 2016830\ncpu: 350.48408046290467 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/50",
            "value": 1121.9346374198408,
            "unit": "ns/iter",
            "extra": "iterations: 623476\ncpu: 1121.8148044190957 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/100",
            "value": 1724.230193734622,
            "unit": "ns/iter",
            "extra": "iterations: 406071\ncpu: 1724.1922250049865 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/1000",
            "value": 28226.93767134243,
            "unit": "ns/iter",
            "extra": "iterations: 24804\ncpu: 28226.272939848408 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/10000",
            "value": 474136.28299317503,
            "unit": "ns/iter",
            "extra": "iterations: 1470\ncpu: 474129.5693877556 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/100000",
            "value": 4082583.441860273,
            "unit": "ns/iter",
            "extra": "iterations: 172\ncpu: 4082518.9825581396 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<false>/1000000",
            "value": 42914465.437497996,
            "unit": "ns/iter",
            "extra": "iterations: 16\ncpu: 42899405.06250001 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/10",
            "value": 1636.4445861179922,
            "unit": "ns/iter",
            "extra": "iterations: 428999\ncpu: 1635.4656327870214 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/20",
            "value": 2487.6887962529404,
            "unit": "ns/iter",
            "extra": "iterations: 281388\ncpu: 2486.050897692867 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/50",
            "value": 4486.568161795522,
            "unit": "ns/iter",
            "extra": "iterations: 155505\ncpu: 4485.552509565604 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/100",
            "value": 6038.314332064673,
            "unit": "ns/iter",
            "extra": "iterations: 115887\ncpu: 6027.4362784436535 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/1000",
            "value": 45990.8343752041,
            "unit": "ns/iter",
            "extra": "iterations: 15197\ncpu: 45938.15687306702 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/10000",
            "value": 341052.4456892344,
            "unit": "ns/iter",
            "extra": "iterations: 2053\ncpu: 340832.33414515306 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/100000",
            "value": 2019115.3439306838,
            "unit": "ns/iter",
            "extra": "iterations: 346\ncpu: 2018882.5520231193 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<true>/1000000",
            "value": 20433172.58823449,
            "unit": "ns/iter",
            "extra": "iterations: 34\ncpu: 20426438.647058833 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 3141.049392410943,
            "unit": "ns/iter",
            "extra": "iterations: 222848\ncpu: 3140.9208743179247 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 6103.006074770219,
            "unit": "ns/iter",
            "extra": "iterations: 114243\ncpu: 6102.760466724446 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 16337.191214108505,
            "unit": "ns/iter",
            "extra": "iterations: 42978\ncpu: 16336.63139280563 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 40278.517221517024,
            "unit": "ns/iter",
            "extra": "iterations: 17362\ncpu: 40277.00984909582 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 361629.7561486113,
            "unit": "ns/iter",
            "extra": "iterations: 1911\ncpu: 361607.94557822985 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2821502.504032297,
            "unit": "ns/iter",
            "extra": "iterations: 248\ncpu: 2821465.0201612795 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 452.451371158865,
            "unit": "ns/iter",
            "extra": "iterations: 1548690\ncpu: 452.44304282974633 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 1569.8747572815291,
            "unit": "ns/iter",
            "extra": "iterations: 445990\ncpu: 1569.867423036389 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 2773.435691555314,
            "unit": "ns/iter",
            "extra": "iterations: 251079\ncpu: 2773.4659449814544 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 9226.224695318202,
            "unit": "ns/iter",
            "extra": "iterations: 76063\ncpu: 9225.566306877168 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 139733.90176423188,
            "unit": "ns/iter",
            "extra": "iterations: 4988\ncpu: 139712.5314755415 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1680918.3349281691,
            "unit": "ns/iter",
            "extra": "iterations: 418\ncpu: 1680887.188995221 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 2563.759152201788,
            "unit": "ns/iter",
            "extra": "iterations: 274169\ncpu: 2563.7630877305664 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 9603.383339516726,
            "unit": "ns/iter",
            "extra": "iterations: 72771\ncpu: 9603.484835992362 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 30224.81389844986,
            "unit": "ns/iter",
            "extra": "iterations: 23240\ncpu: 30224.814802065157 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 136352.9491658495,
            "unit": "ns/iter",
            "extra": "iterations: 5095\ncpu: 136349.16761530857 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 29643978.8750007,
            "unit": "ns/iter",
            "extra": "iterations: 24\ncpu: 29642589.37500001 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 5835341220.999965,
            "unit": "ns/iter",
            "extra": "iterations: 1\ncpu: 5835165748.999998 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 510.1620870803146,
            "unit": "ns/iter",
            "extra": "iterations: 1373996\ncpu: 510.1413388394122 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 1018.8607747161512,
            "unit": "ns/iter",
            "extra": "iterations: 686858\ncpu: 1018.8274869041428 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 2495.43610698097,
            "unit": "ns/iter",
            "extra": "iterations: 281807\ncpu: 2495.362336634659 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 4860.906526877645,
            "unit": "ns/iter",
            "extra": "iterations: 144694\ncpu: 4860.664146405513 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 50491.87395143473,
            "unit": "ns/iter",
            "extra": "iterations: 13590\ncpu: 50490.45467255341 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 996162.288065821,
            "unit": "ns/iter",
            "extra": "iterations: 729\ncpu: 996147.6474622762 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 19569.214694312148,
            "unit": "ns/iter",
            "extra": "iterations: 41431\ncpu: 19569.266587820817 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 371744.38620688947,
            "unit": "ns/iter",
            "extra": "iterations: 1885\ncpu: 371724.4461538459 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 1474.1338421357686,
            "unit": "ns/iter",
            "extra": "iterations: 472482\ncpu: 1474.0939929986853 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 25858.562027849235,
            "unit": "ns/iter",
            "extra": "iterations: 27004\ncpu: 25858.23181750836 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}