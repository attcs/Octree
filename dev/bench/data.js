window.BENCHMARK_DATA = {
  "lastUpdate": 1737324025156,
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
      }
    ]
  }
}