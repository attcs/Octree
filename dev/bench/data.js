window.BENCHMARK_DATA = {
  "lastUpdate": 1739053450042,
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
          "id": "21f37fde90567742958a1398fe806ea8fbf7c837",
          "message": "CMakelists: -mavx2 -O3 -ffast-math",
          "timestamp": "2025-01-22T00:32:16+01:00",
          "tree_id": "cd4f1d7ec8dbe78f12a9e42e5c853d7f91551286",
          "url": "https://github.com/attcs/Octree/commit/21f37fde90567742958a1398fe806ea8fbf7c837"
        },
        "date": 1737502817456,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1910.0308624279783,
            "unit": "ns/iter",
            "extra": "iterations: 369770\ncpu: 1909.8945533710144 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 2183.7069078033246,
            "unit": "ns/iter",
            "extra": "iterations: 320261\ncpu: 2183.3082267275754 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.00028125099573002657,
            "unit": "ms/iter",
            "extra": "iterations: 2494401\ncpu: 0.0002812443640777887 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003383910015949499,
            "unit": "ms/iter",
            "extra": "iterations: 2070278\ncpu: 0.0003383897727744778 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0011432701499592226,
            "unit": "ms/iter",
            "extra": "iterations: 613500\ncpu: 0.001143265339853301 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0017934978330713576,
            "unit": "ms/iter",
            "extra": "iterations: 392491\ncpu: 0.0017934933310572723 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.030473685751577852,
            "unit": "ms/iter",
            "extra": "iterations: 22985\ncpu: 0.030472983728518598 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.824613637323929,
            "unit": "ms/iter",
            "extra": "iterations: 852\ncpu: 0.8245861760563384 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 10.259642588235268,
            "unit": "ms/iter",
            "extra": "iterations: 68\ncpu: 10.259394691176478 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 67.92863269999998,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 67.9289227 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0016924370830318522,
            "unit": "ms/iter",
            "extra": "iterations: 413251\ncpu: 0.0016917604022736808 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.0025023092702647766,
            "unit": "ms/iter",
            "extra": "iterations: 280499\ncpu: 0.0024975676882983584 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.004643607441314331,
            "unit": "ms/iter",
            "extra": "iterations: 151613\ncpu: 0.004622429818023511 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.006141381549750987,
            "unit": "ms/iter",
            "extra": "iterations: 114470\ncpu: 0.006134973748580413 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04720250995589996,
            "unit": "ms/iter",
            "extra": "iterations: 14966\ncpu: 0.04717562494988639 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6193766053097363,
            "unit": "ms/iter",
            "extra": "iterations: 1130\ncpu: 0.6193195141592923 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.687843847619152,
            "unit": "ms/iter",
            "extra": "iterations: 105\ncpu: 6.6871235619047695 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 33.155965952380214,
            "unit": "ms/iter",
            "extra": "iterations: 21\ncpu: 33.150730333333335 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.0033169734079246194,
            "unit": "ms/iter",
            "extra": "iterations: 211642\ncpu: 0.003316813567250359 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006467384372832841,
            "unit": "ms/iter",
            "extra": "iterations: 108145\ncpu: 0.006466874492579405 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.016993408820829635,
            "unit": "ms/iter",
            "extra": "iterations: 41402\ncpu: 0.01699204011883484 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.04162931950626093,
            "unit": "ms/iter",
            "extra": "iterations: 16851\ncpu: 0.04162655088718776 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.4047733171576971,
            "unit": "ms/iter",
            "extra": "iterations: 1731\ncpu: 0.40474716926632104 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.9260027016808166,
            "unit": "ms/iter",
            "extra": "iterations: 238\ncpu: 2.925893441176471 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.0004556526870454719,
            "unit": "ms/iter",
            "extra": "iterations: 1529989\ncpu: 0.00045565147331124567 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0015776666817094945,
            "unit": "ms/iter",
            "extra": "iterations: 443180\ncpu: 0.00157765814567445 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0028334261565293012,
            "unit": "ms/iter",
            "extra": "iterations: 245973\ncpu: 0.0028334042598171265 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009485605828671705,
            "unit": "ms/iter",
            "extra": "iterations: 74734\ncpu: 0.00948524635373456 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14179469305923448,
            "unit": "ms/iter",
            "extra": "iterations: 4913\ncpu: 0.1417890372481174 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7417074108910902,
            "unit": "ms/iter",
            "extra": "iterations: 404\ncpu: 1.7416636237623775 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.002546929436250025,
            "unit": "ms/iter",
            "extra": "iterations: 276275\ncpu: 0.002546898908696055 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.009423554240366146,
            "unit": "ms/iter",
            "extra": "iterations: 74345\ncpu: 0.009423156580805723 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.029534497370630495,
            "unit": "ms/iter",
            "extra": "iterations: 22629\ncpu: 0.029534103672278902 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.09374047084174461,
            "unit": "ms/iter",
            "extra": "iterations: 7425\ncpu: 0.09374016983164933 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 2.1641231300310104,
            "unit": "ms/iter",
            "extra": "iterations: 323\ncpu: 2.164083582043356 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 23.454103266666,
            "unit": "ms/iter",
            "extra": "iterations: 30\ncpu: 23.454239500000067 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0005201304696046553,
            "unit": "ms/iter",
            "extra": "iterations: 1340979\ncpu: 0.0005201167132371202 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0010459436550508214,
            "unit": "ms/iter",
            "extra": "iterations: 677665\ncpu: 0.0010459229575085116 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.0025467748001708154,
            "unit": "ms/iter",
            "extra": "iterations: 271732\ncpu: 0.002546760613398492 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.005211791965628649,
            "unit": "ms/iter",
            "extra": "iterations: 139650\ncpu: 0.005211665392051553 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.05298130387000541,
            "unit": "ms/iter",
            "extra": "iterations: 12739\ncpu: 0.052981008163906385 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 0.9511952278308348,
            "unit": "ms/iter",
            "extra": "iterations: 733\ncpu: 0.9511830477489701 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.627667069977015,
            "unit": "ns/iter",
            "extra": "iterations: 39287355\ncpu: 17.627585466112482 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.25256710154615,
            "unit": "ns/iter",
            "extra": "iterations: 22358874\ncpu: 31.251142432306825 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 46.42027613349597,
            "unit": "ns/iter",
            "extra": "iterations: 15305423\ncpu: 46.4193337877692 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 88.20921142632443,
            "unit": "ns/iter",
            "extra": "iterations: 7760188\ncpu: 88.20395485263981 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 280.3227878259063,
            "unit": "ns/iter",
            "extra": "iterations: 2493226\ncpu: 280.3105017355024 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2153.8797383501815,
            "unit": "ns/iter",
            "extra": "iterations: 320734\ncpu: 2153.7294112878376 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.017331035308020604,
            "unit": "ms/iter",
            "extra": "iterations: 40614\ncpu: 0.017330660092579064 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.37682908135777493,
            "unit": "ms/iter",
            "extra": "iterations: 1856\ncpu: 0.37681744989224086 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.001517223535874033,
            "unit": "ms/iter",
            "extra": "iterations: 462409\ncpu: 0.0015171797780752531 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.026054660801529374,
            "unit": "ms/iter",
            "extra": "iterations: 28246\ncpu: 0.026054144409827933 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0003647087291177399,
            "unit": "ms/iter",
            "extra": "iterations: 1903709\ncpu: 0.0003646959136086447 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.000527952079320937,
            "unit": "ms/iter",
            "extra": "iterations: 1328174\ncpu: 0.000527925395317178 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0017235660822381505,
            "unit": "ms/iter",
            "extra": "iterations: 404897\ncpu: 0.0017235458919182011 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.0032792474296111803,
            "unit": "ms/iter",
            "extra": "iterations: 212711\ncpu: 0.0032791877288903715 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03589382251702892,
            "unit": "ms/iter",
            "extra": "iterations: 19523\ncpu: 0.03589342882753648 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0036441477762348,
            "unit": "ms/iter",
            "extra": "iterations: 697\ncpu: 1.0036402166427556 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.679311430555831,
            "unit": "ms/iter",
            "extra": "iterations: 72\ncpu: 9.67935173611107 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 99.13840614285618,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 99.1368782857143 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0018781257253176155,
            "unit": "ms/iter",
            "extra": "iterations: 370872\ncpu: 0.0018767631959274536 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.0028046444335732943,
            "unit": "ms/iter",
            "extra": "iterations: 249388\ncpu: 0.002800775398174743 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005457657082945191,
            "unit": "ms/iter",
            "extra": "iterations: 128760\ncpu: 0.005449771870145973 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.00871554626332308,
            "unit": "ms/iter",
            "extra": "iterations: 80593\ncpu: 0.008710853163426078 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.04949604278150161,
            "unit": "ms/iter",
            "extra": "iterations: 14165\ncpu: 0.049429135051182294 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.4779733572895087,
            "unit": "ms/iter",
            "extra": "iterations: 1461\ncpu: 0.4779720294318935 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.052837476744048,
            "unit": "ms/iter",
            "extra": "iterations: 172\ncpu: 4.05039205232555 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 42.58219258823334,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 42.5441730000002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007670121995558354,
            "unit": "ms/iter",
            "extra": "iterations: 913148\ncpu: 0.0007670018956401498 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.0022292959639837596,
            "unit": "ms/iter",
            "extra": "iterations: 315856\ncpu: 0.002229134064257123 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.005128222345992098,
            "unit": "ms/iter",
            "extra": "iterations: 136454\ncpu: 0.005127834229850358 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.014898707565985152,
            "unit": "ms/iter",
            "extra": "iterations: 47132\ncpu: 0.014898567194263022 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23486517619368977,
            "unit": "ms/iter",
            "extra": "iterations: 2974\ncpu: 0.23486464794889253 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 3.0314395474138287,
            "unit": "ms/iter",
            "extra": "iterations: 232\ncpu: 3.0314343577586556 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.514160375001097,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.513247500000023 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 311.16099550001763,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 311.15592500000133 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.0040695423907657746,
            "unit": "ms/iter",
            "extra": "iterations: 171535\ncpu: 0.004069491514851192 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009112601327346839,
            "unit": "ms/iter",
            "extra": "iterations: 76845\ncpu: 0.009112051818595897 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.01841014637101111,
            "unit": "ms/iter",
            "extra": "iterations: 37972\ncpu: 0.018409756662804068 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04235715511191622,
            "unit": "ms/iter",
            "extra": "iterations: 16530\ncpu: 0.042355022928009495 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3682945034889954,
            "unit": "ms/iter",
            "extra": "iterations: 1863\ncpu: 0.36829251798174684 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.828924983805659,
            "unit": "ms/iter",
            "extra": "iterations: 247\ncpu: 2.8289152145748724 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.55454351724225,
            "unit": "ms/iter",
            "extra": "iterations: 29\ncpu: 24.554160344827626 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 279.1658636666625,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 278.9855176666644 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0015653435502617453,
            "unit": "ms/iter",
            "extra": "iterations: 448243\ncpu: 0.0015652838973503162 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.003062736770477417,
            "unit": "ms/iter",
            "extra": "iterations: 227786\ncpu: 0.0030627195657327647 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006309442834443445,
            "unit": "ms/iter",
            "extra": "iterations: 110766\ncpu: 0.0063092616506870685 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011680995845776731,
            "unit": "ms/iter",
            "extra": "iterations: 59939\ncpu: 0.011680601227915009 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.12650641261521892,
            "unit": "ms/iter",
            "extra": "iterations: 5533\ncpu: 0.12650161648291985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0787507969230494,
            "unit": "ms/iter",
            "extra": "iterations: 650\ncpu: 1.078718779999996 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.0025670567334276787,
            "unit": "ms/iter",
            "extra": "iterations: 272714\ncpu: 0.002566954993142982 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.0071954714560156915,
            "unit": "ms/iter",
            "extra": "iterations: 97341\ncpu: 0.00719539736596085 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.016925893398320863,
            "unit": "ms/iter",
            "extra": "iterations: 41444\ncpu: 0.016925649116880524 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.038651319158359336,
            "unit": "ms/iter",
            "extra": "iterations: 18060\ncpu: 0.038650512679955555 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.38918838650304644,
            "unit": "ms/iter",
            "extra": "iterations: 1793\ncpu: 0.3891912035694407 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.17662540639278,
            "unit": "ms/iter",
            "extra": "iterations: 219\ncpu: 3.176515470319644 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005480431918109221,
            "unit": "ms/iter",
            "extra": "iterations: 1237943\ncpu: 0.0005480406181867838 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017193907589935665,
            "unit": "ms/iter",
            "extra": "iterations: 409068\ncpu: 0.0017194011777992876 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.003128347043288201,
            "unit": "ms/iter",
            "extra": "iterations: 223779\ncpu: 0.0031283062619816672 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007410196577850561,
            "unit": "ms/iter",
            "extra": "iterations: 94502\ncpu: 0.007409882203551269 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.11727844580127368,
            "unit": "ms/iter",
            "extra": "iterations: 5978\ncpu: 0.11726442857142705 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 1.6151885368663983,
            "unit": "ms/iter",
            "extra": "iterations: 434\ncpu: 1.6151086912442454 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.001688873773109056,
            "unit": "ms/iter",
            "extra": "iterations: 414666\ncpu: 0.0016887713509185628 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.003280023528748168,
            "unit": "ms/iter",
            "extra": "iterations: 212761\ncpu: 0.0032798165594257736 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008080793677087413,
            "unit": "ms/iter",
            "extra": "iterations: 85119\ncpu: 0.008080435590173721 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.018448571755076894,
            "unit": "ms/iter",
            "extra": "iterations: 37628\ncpu: 0.018448286409056925 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.3061009391951033,
            "unit": "ms/iter",
            "extra": "iterations: 2286\ncpu: 0.30607329221347457 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 4.276295963855576,
            "unit": "ms/iter",
            "extra": "iterations: 166\ncpu: 4.275859596385525 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005206326692658232,
            "unit": "ms/iter",
            "extra": "iterations: 1329935\ncpu: 0.0005206193949328319 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0011919208331983117,
            "unit": "ms/iter",
            "extra": "iterations: 586559\ncpu: 0.0011919161090359414 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0035199368372431285,
            "unit": "ms/iter",
            "extra": "iterations: 199168\ncpu: 0.00351989639901995 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.007711787582417861,
            "unit": "ms/iter",
            "extra": "iterations: 91000\ncpu: 0.007711461857142849 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.1736488171574934,
            "unit": "ms/iter",
            "extra": "iterations: 3905\ncpu: 0.17363831677336777 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 773.4709750000093,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 773.4350859999921 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011613977328701034,
            "unit": "ms/iter",
            "extra": "iterations: 594849\ncpu: 0.0011613176201019123 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.0040290910441414095,
            "unit": "ms/iter",
            "extra": "iterations: 176365\ncpu: 0.004028959118872773 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.01482112128675809,
            "unit": "ms/iter",
            "extra": "iterations: 46691\ncpu: 0.014819805251547418 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.04426395877608431,
            "unit": "ms/iter",
            "extra": "iterations: 15622\ncpu: 0.044261194661374584 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.314311804182434,
            "unit": "ms/iter",
            "extra": "iterations: 526\ncpu: 1.3141867110266254 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 136.72277324999982,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 136.71115274999934 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 70.90226971840171,
            "unit": "ns/iter",
            "extra": "iterations: 9871577\ncpu: 70.90063532908603 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 377.9918918801071,
            "unit": "ns/iter",
            "extra": "iterations: 1834334\ncpu: 377.96740342816344 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10933.956246195172,
            "unit": "ns/iter",
            "extra": "iterations: 64063\ncpu: 10933.620451743003 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 136523.88028714736,
            "unit": "ns/iter",
            "extra": "iterations: 5154\ncpu: 136523.32169188838 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 56.37505545786643,
            "unit": "ns/iter",
            "extra": "iterations: 12426101\ncpu: 56.37084802384867 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 148.45414831512693,
            "unit": "ns/iter",
            "extra": "iterations: 4744155\ncpu: 148.44755114451308 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2284.327275041536,
            "unit": "ns/iter",
            "extra": "iterations: 306412\ncpu: 2284.115413234454 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 40082.8386442636,
            "unit": "ns/iter",
            "extra": "iterations: 17260\ncpu: 40081.4239860949 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 81.15846262553461,
            "unit": "ns/iter",
            "extra": "iterations: 8673957\ncpu: 81.15332125810622 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 415.73898843064774,
            "unit": "ns/iter",
            "extra": "iterations: 1673558\ncpu: 415.7324526547553 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10688.426258058204,
            "unit": "ns/iter",
            "extra": "iterations: 65458\ncpu: 10688.367686149753 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 130348.81297497576,
            "unit": "ns/iter",
            "extra": "iterations: 5395\ncpu: 130341.23688600653 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 91.07020140215195,
            "unit": "ns/iter",
            "extra": "iterations: 7795001\ncpu: 91.06633225576367 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 219.88206227751232,
            "unit": "ns/iter",
            "extra": "iterations: 3178932\ncpu: 219.8773685627762 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3566.090773420128,
            "unit": "ns/iter",
            "extra": "iterations: 194991\ncpu: 3566.006072075145 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 45163.94370093684,
            "unit": "ns/iter",
            "extra": "iterations: 15542\ncpu: 45163.190129971335 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.0026278691105072385,
            "unit": "ms/iter",
            "extra": "iterations: 261992\ncpu: 0.0026278404455097673 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.06036274163251517,
            "unit": "ms/iter",
            "extra": "iterations: 11712\ncpu: 0.06035977262636676 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.0023471898606565273,
            "unit": "ms/iter",
            "extra": "iterations: 300552\ncpu: 0.0023471319705076375 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07024578657866032,
            "unit": "ms/iter",
            "extra": "iterations: 9999\ncpu: 0.07024412541254199 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.002440237504113344,
            "unit": "ms/iter",
            "extra": "iterations: 285654\ncpu: 0.0024401382861784335 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5220623654708431,
            "unit": "ms/iter",
            "extra": "iterations: 1338\ncpu: 0.5220603116591945 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 38.588999611110665,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 38.58635366666697 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6215.13274900002,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6214.836626000022 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.005469179287104607,
            "unit": "ms/iter",
            "extra": "iterations: 127901\ncpu: 0.005468738258496718 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6001047171109156,
            "unit": "ms/iter",
            "extra": "iterations: 1163\ncpu: 0.6000913147033429 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 39.03996161111283,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.03772022222225 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6293.896607000022,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6293.546598000006 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005590512395200522,
            "unit": "ms/iter",
            "extra": "iterations: 127267\ncpu: 0.005589567287670732 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.62376960320284,
            "unit": "ms/iter",
            "extra": "iterations: 1124\ncpu: 0.6237662482206373 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 63.40810627272669,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 63.40600972727243 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 22770.159000999967,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 22754.326961000002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.013793478106978582,
            "unit": "ms/iter",
            "extra": "iterations: 50290\ncpu: 0.013792477192284639 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7133358353658236,
            "unit": "ms/iter",
            "extra": "iterations: 984\ncpu: 0.7132697479674767 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 50.01609228571624,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 50.01574042857117 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 16760.20004000003,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 16697.29134900001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.008091923907959533,
            "unit": "ms/iter",
            "extra": "iterations: 87657\ncpu: 0.008091692266447494 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 2.1513727125381,
            "unit": "ms/iter",
            "extra": "iterations: 327\ncpu: 2.151001342507582 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 192.97827850000715,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 192.9682597500033 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 23388.90473699996,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23388.244806000017 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.012902770126433118,
            "unit": "ms/iter",
            "extra": "iterations: 54021\ncpu: 0.012902642176190957 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.7398888304239195,
            "unit": "ms/iter",
            "extra": "iterations: 401\ncpu: 1.7398517905237152 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 126.99228783333183,
            "unit": "ms/iter",
            "extra": "iterations: 6\ncpu: 126.98904200000054 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 13503.891786999986,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 13503.126084000001 ms\nthreads: 1"
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
          "id": "55f2e39e975049a22084ee97fdb4c14f55661869",
          "message": "Different seed",
          "timestamp": "2025-01-22T00:58:37+01:00",
          "tree_id": "80099852e5a1a421181ba62ded3e827f122af1da",
          "url": "https://github.com/attcs/Octree/commit/55f2e39e975049a22084ee97fdb4c14f55661869"
        },
        "date": 1737504291252,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1955.3127600965956,
            "unit": "ns/iter",
            "extra": "iterations: 371016\ncpu: 1954.9166343230481 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 2193.1687387772363,
            "unit": "ns/iter",
            "extra": "iterations: 306853\ncpu: 2193.1610380214634 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.00028594620565876175,
            "unit": "ms/iter",
            "extra": "iterations: 2445945\ncpu: 0.0002859262346455051 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003385735239597925,
            "unit": "ms/iter",
            "extra": "iterations: 2066170\ncpu: 0.00033850672548725407 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0011552336749045646,
            "unit": "ms/iter",
            "extra": "iterations: 607194\ncpu: 0.0011551453456391206 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.001755691884128577,
            "unit": "ms/iter",
            "extra": "iterations: 398269\ncpu: 0.0017555507508744078 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.02993902681171825,
            "unit": "ms/iter",
            "extra": "iterations: 23348\ncpu: 0.02993712433613159 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.808711979214763,
            "unit": "ms/iter",
            "extra": "iterations: 866\ncpu: 0.8086437355658201 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 10.193315073529163,
            "unit": "ms/iter",
            "extra": "iterations: 68\ncpu: 10.192743955882355 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 70.24955366666556,
            "unit": "ms/iter",
            "extra": "iterations: 9\ncpu: 70.24035277777783 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0016841062453497285,
            "unit": "ms/iter",
            "extra": "iterations: 412611\ncpu: 0.0016829219022275238 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.0024766337890797627,
            "unit": "ms/iter",
            "extra": "iterations: 282979\ncpu: 0.0024756383335865872 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.004608909936271295,
            "unit": "ms/iter",
            "extra": "iterations: 151737\ncpu: 0.004606931223103128 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.00617400760899314,
            "unit": "ms/iter",
            "extra": "iterations: 113287\ncpu: 0.006170763370907506 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04688822836746859,
            "unit": "ms/iter",
            "extra": "iterations: 14989\ncpu: 0.04681904042964849 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6255346927223806,
            "unit": "ms/iter",
            "extra": "iterations: 1113\ncpu: 0.6252697358490559 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.765948451923046,
            "unit": "ms/iter",
            "extra": "iterations: 104\ncpu: 6.765390067307697 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 36.439835684210436,
            "unit": "ms/iter",
            "extra": "iterations: 19\ncpu: 36.43535505263155 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.003293402533354593,
            "unit": "ms/iter",
            "extra": "iterations: 213314\ncpu: 0.003293197900747253 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.0063783752243346935,
            "unit": "ms/iter",
            "extra": "iterations: 109769\ncpu: 0.0063779792655485725 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.016750008014353814,
            "unit": "ms/iter",
            "extra": "iterations: 41800\ncpu: 0.016748406291866004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.041305307135713794,
            "unit": "ms/iter",
            "extra": "iterations: 16999\ncpu: 0.041247190952408905 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.39237240380312693,
            "unit": "ms/iter",
            "extra": "iterations: 1788\ncpu: 0.39234561689038033 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.8631112734694155,
            "unit": "ms/iter",
            "extra": "iterations: 245\ncpu: 2.8628092530612337 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.0004526319801872727,
            "unit": "ms/iter",
            "extra": "iterations: 1548096\ncpu: 0.00045258448184092037 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.001580224385003092,
            "unit": "ms/iter",
            "extra": "iterations: 443051\ncpu: 0.0015801070079968242 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0028104478987495025,
            "unit": "ms/iter",
            "extra": "iterations: 249994\ncpu: 0.002810241021784534 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009318040331374247,
            "unit": "ms/iter",
            "extra": "iterations: 75202\ncpu: 0.009317522260046245 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14173095415067555,
            "unit": "ms/iter",
            "extra": "iterations: 4951\ncpu: 0.14172194546556305 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.6861275169082535,
            "unit": "ms/iter",
            "extra": "iterations: 414\ncpu: 1.6859497850241556 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.002650070121004167,
            "unit": "ms/iter",
            "extra": "iterations: 263958\ncpu: 0.00264984092166178 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.00974676768224804,
            "unit": "ms/iter",
            "extra": "iterations: 71880\ncpu: 0.009746364092932681 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.030473033111719915,
            "unit": "ms/iter",
            "extra": "iterations: 23013\ncpu: 0.030471557293703554 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.09592844137457078,
            "unit": "ms/iter",
            "extra": "iterations: 7275\ncpu: 0.0959243832302406 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 2.1679827523219863,
            "unit": "ms/iter",
            "extra": "iterations: 323\ncpu: 2.167859216718257 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 23.133163299999637,
            "unit": "ms/iter",
            "extra": "iterations: 30\ncpu: 23.130786000000114 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.000478796369350547,
            "unit": "ms/iter",
            "extra": "iterations: 1461116\ncpu: 0.00047877467086802114 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0007957950541728093,
            "unit": "ms/iter",
            "extra": "iterations: 879408\ncpu: 0.000795719516993256 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.0022127890911638475,
            "unit": "ms/iter",
            "extra": "iterations: 313984\ncpu: 0.002212629914263148 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004636954454898586,
            "unit": "ms/iter",
            "extra": "iterations: 152091\ncpu: 0.004636754357588569 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06205197402125656,
            "unit": "ms/iter",
            "extra": "iterations: 11009\ncpu: 0.06204356798982604 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.0131984746008835,
            "unit": "ms/iter",
            "extra": "iterations: 689\ncpu: 1.013134502177073 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.762981735876714,
            "unit": "ns/iter",
            "extra": "iterations: 39597632\ncpu: 17.761419041421526 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 30.747848641008925,
            "unit": "ns/iter",
            "extra": "iterations: 22862177\ncpu: 30.74696473568563 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 46.27070259341432,
            "unit": "ns/iter",
            "extra": "iterations: 15136763\ncpu: 46.26700246281167 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 91.51780003570532,
            "unit": "ns/iter",
            "extra": "iterations: 7780799\ncpu: 91.451910787054 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 249.6906110657631,
            "unit": "ns/iter",
            "extra": "iterations: 2706910\ncpu: 249.67752972947056 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2004.0346142657029,
            "unit": "ns/iter",
            "extra": "iterations: 347169\ncpu: 2004.0258029950883 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.017667871111221464,
            "unit": "ms/iter",
            "extra": "iterations: 40469\ncpu: 0.01766675783933379 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.3766131299677946,
            "unit": "ms/iter",
            "extra": "iterations: 1862\ncpu: 0.37661172073039806 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015218017045195162,
            "unit": "ms/iter",
            "extra": "iterations: 460071\ncpu: 0.001521672402737839 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.02486216681296239,
            "unit": "ms/iter",
            "extra": "iterations: 28481\ncpu: 0.02486209322004144 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.000353597207073328,
            "unit": "ms/iter",
            "extra": "iterations: 1993178\ncpu: 0.0003535684133579656 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005429128860051578,
            "unit": "ms/iter",
            "extra": "iterations: 1292146\ncpu: 0.0005428692082783198 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.001715132136943017,
            "unit": "ms/iter",
            "extra": "iterations: 408228\ncpu: 0.0017150131250183646 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.0033570174922766976,
            "unit": "ms/iter",
            "extra": "iterations: 207806\ncpu: 0.0033567752374811127 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.0369716609532898,
            "unit": "ms/iter",
            "extra": "iterations: 18903\ncpu: 0.03696769401682292 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0111108155619017,
            "unit": "ms/iter",
            "extra": "iterations: 694\ncpu: 1.0110200072046107 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.88043071830957,
            "unit": "ms/iter",
            "extra": "iterations: 71\ncpu: 9.879411309859085 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 101.38775814285898,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 101.37526385714223 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0018896951403593774,
            "unit": "ms/iter",
            "extra": "iterations: 371653\ncpu: 0.0018892733140859817 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.002785835928525883,
            "unit": "ms/iter",
            "extra": "iterations: 250720\ncpu: 0.0027848751874601394 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005471392551754451,
            "unit": "ms/iter",
            "extra": "iterations: 128057\ncpu: 0.0054703484307769374 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.008946683395315203,
            "unit": "ms/iter",
            "extra": "iterations: 78249\ncpu: 0.008944163081956343 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.050340309744109454,
            "unit": "ms/iter",
            "extra": "iterations: 13834\ncpu: 0.05032823369958078 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.48584003250345315,
            "unit": "ms/iter",
            "extra": "iterations: 1446\ncpu: 0.48530028699861544 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.158574550295879,
            "unit": "ms/iter",
            "extra": "iterations: 169\ncpu: 4.156193698224857 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 43.222566437499665,
            "unit": "ms/iter",
            "extra": "iterations: 16\ncpu: 43.1996056874997 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007618914299851678,
            "unit": "ms/iter",
            "extra": "iterations: 921562\ncpu: 0.0007618781178043359 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.0022549429366424067,
            "unit": "ms/iter",
            "extra": "iterations: 311233\ncpu: 0.0022547583482471715 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.00517282916335105,
            "unit": "ms/iter",
            "extra": "iterations: 135732\ncpu: 0.005172320182418194 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.014734172821363994,
            "unit": "ms/iter",
            "extra": "iterations: 47633\ncpu: 0.014732769613503328 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23504106429779764,
            "unit": "ms/iter",
            "extra": "iterations: 2955\ncpu: 0.23502468866328352 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 3.01694864655168,
            "unit": "ms/iter",
            "extra": "iterations: 232\ncpu: 3.016514581896564 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.69671383333387,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.69274058333345 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 307.22213650000185,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 307.1864930000032 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.004178457398918791,
            "unit": "ms/iter",
            "extra": "iterations: 170383\ncpu: 0.0041170135752979994 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009251137194003722,
            "unit": "ms/iter",
            "extra": "iterations: 76308\ncpu: 0.009241926704932624 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.01854257077673669,
            "unit": "ms/iter",
            "extra": "iterations: 37915\ncpu: 0.018520586786232435 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04255648391599646,
            "unit": "ms/iter",
            "extra": "iterations: 16476\ncpu: 0.04248683321194487 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3763631799141003,
            "unit": "ms/iter",
            "extra": "iterations: 1862\ncpu: 0.37573597583243606 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.854172178861821,
            "unit": "ms/iter",
            "extra": "iterations: 246\ncpu: 2.8540525691056473 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 25.841654499998867,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 25.80920257142846 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 285.94730766667453,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 285.93215166666636 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.001576590736452464,
            "unit": "ms/iter",
            "extra": "iterations: 445229\ncpu: 0.0015763119900096196 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.0030749403737084314,
            "unit": "ms/iter",
            "extra": "iterations: 227450\ncpu: 0.0030746861024400687 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006374305441297253,
            "unit": "ms/iter",
            "extra": "iterations: 110617\ncpu: 0.006373206749414693 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011600756174877994,
            "unit": "ms/iter",
            "extra": "iterations: 60568\ncpu: 0.011600242652885985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.1266672202100627,
            "unit": "ms/iter",
            "extra": "iterations: 5522\ncpu: 0.12665727055414505 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0895474775193286,
            "unit": "ms/iter",
            "extra": "iterations: 645\ncpu: 1.089454398449615 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.0026097599701965105,
            "unit": "ms/iter",
            "extra": "iterations: 268425\ncpu: 0.00260939799944122 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.007251601500573394,
            "unit": "ms/iter",
            "extra": "iterations: 96763\ncpu: 0.007251336554261489 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.0168253652132364,
            "unit": "ms/iter",
            "extra": "iterations: 41855\ncpu: 0.016823520487396753 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.038717334893710716,
            "unit": "ms/iter",
            "extra": "iterations: 18158\ncpu: 0.0387159746117407 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.3936071104260247,
            "unit": "ms/iter",
            "extra": "iterations: 1784\ncpu: 0.39358876905829765 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.2220902614677813,
            "unit": "ms/iter",
            "extra": "iterations: 218\ncpu: 3.2218282018348923 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.000549138053255734,
            "unit": "ms/iter",
            "extra": "iterations: 1274117\ncpu: 0.0005491163574459785 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017164986804742417,
            "unit": "ms/iter",
            "extra": "iterations: 399007\ncpu: 0.0017164007523677528 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.003167026737919569,
            "unit": "ms/iter",
            "extra": "iterations: 221558\ncpu: 0.0031668334025401743 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.0076149503883027325,
            "unit": "ms/iter",
            "extra": "iterations: 92196\ncpu: 0.0076141101132371 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.11813291648276962,
            "unit": "ms/iter",
            "extra": "iterations: 5891\ncpu: 0.11812740349685807 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 1.6310022587412445,
            "unit": "ms/iter",
            "extra": "iterations: 429\ncpu: 1.6308353636363704 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0016796616041367942,
            "unit": "ms/iter",
            "extra": "iterations: 417907\ncpu: 0.001679550660792942 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.003713316847226305,
            "unit": "ms/iter",
            "extra": "iterations: 193011\ncpu: 0.00371308904673825 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008205148849412815,
            "unit": "ms/iter",
            "extra": "iterations: 84609\ncpu: 0.008204738443900872 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.018381696098509842,
            "unit": "ms/iter",
            "extra": "iterations: 38088\ncpu: 0.01838055227368232 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.30947901576182524,
            "unit": "ms/iter",
            "extra": "iterations: 2284\ncpu: 0.3094586274080544 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 4.22528418562898,
            "unit": "ms/iter",
            "extra": "iterations: 167\ncpu: 4.225222520958027 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005277231900400606,
            "unit": "ms/iter",
            "extra": "iterations: 1288523\ncpu: 0.0005277173639896196 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0012011410725989548,
            "unit": "ms/iter",
            "extra": "iterations: 585904\ncpu: 0.001201122013162578 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0035200860526792476,
            "unit": "ms/iter",
            "extra": "iterations: 198332\ncpu: 0.00352007248452092 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.00773323671923176,
            "unit": "ms/iter",
            "extra": "iterations: 90808\ncpu: 0.007732956083164394 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.18153114050438035,
            "unit": "ms/iter",
            "extra": "iterations: 3886\ncpu: 0.18152430030879915 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 793.2005650000065,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 793.1676639999949 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011184090265790078,
            "unit": "ms/iter",
            "extra": "iterations: 623913\ncpu: 0.0011183642575166629 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.003924926552815974,
            "unit": "ms/iter",
            "extra": "iterations: 178128\ncpu: 0.003924859207985303 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.015176270476350652,
            "unit": "ms/iter",
            "extra": "iterations: 47738\ncpu: 0.0151758457413381 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.04426063040720725,
            "unit": "ms/iter",
            "extra": "iterations: 15766\ncpu: 0.04425872624635294 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.3290140558766668,
            "unit": "ms/iter",
            "extra": "iterations: 519\ncpu: 1.3290129884393223 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 140.03082499999664,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 140.02519225000043 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 71.13094265529747,
            "unit": "ns/iter",
            "extra": "iterations: 9958932\ncpu: 71.12913262185147 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 372.08620492092064,
            "unit": "ns/iter",
            "extra": "iterations: 1885020\ncpu: 372.0725965772249 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 11164.970159267797,
            "unit": "ns/iter",
            "extra": "iterations: 62599\ncpu: 11164.944248310541 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 141154.3258426931,
            "unit": "ns/iter",
            "extra": "iterations: 4895\ncpu: 141148.0138917281 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 55.75254655340824,
            "unit": "ns/iter",
            "extra": "iterations: 12527815\ncpu: 55.752302057462224 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 148.24760270639516,
            "unit": "ns/iter",
            "extra": "iterations: 4726580\ncpu: 148.243312289223 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2296.3477798018,
            "unit": "ns/iter",
            "extra": "iterations: 302473\ncpu: 2296.3025394002398 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 39449.718953130774,
            "unit": "ns/iter",
            "extra": "iterations: 17538\ncpu: 39448.16067966714 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 80.31529644226599,
            "unit": "ns/iter",
            "extra": "iterations: 8690596\ncpu: 80.31366767020482 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 413.4558797857716,
            "unit": "ns/iter",
            "extra": "iterations: 1638342\ncpu: 413.43358285388035 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10696.932629983461,
            "unit": "ns/iter",
            "extra": "iterations: 64643\ncpu: 10696.534350200345 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 131841.55785674,
            "unit": "ns/iter",
            "extra": "iterations: 5263\ncpu: 131837.66634999076 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 89.79301254992339,
            "unit": "ns/iter",
            "extra": "iterations: 7794231\ncpu: 89.79074253765359 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 220.6516246604527,
            "unit": "ns/iter",
            "extra": "iterations: 3199130\ncpu: 220.650685967749 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3595.8819774827098,
            "unit": "ns/iter",
            "extra": "iterations: 194429\ncpu: 3595.6735157820567 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 46539.52478231624,
            "unit": "ns/iter",
            "extra": "iterations: 14930\ncpu: 46538.36108506367 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.0026226652104200396,
            "unit": "ms/iter",
            "extra": "iterations: 265065\ncpu: 0.0026226585101767922 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.06202807011513702,
            "unit": "ms/iter",
            "extra": "iterations: 11638\ncpu: 0.0620240157243507 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.0023550725731776596,
            "unit": "ms/iter",
            "extra": "iterations: 296983\ncpu: 0.002354922769990163 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07118835283761371,
            "unit": "ms/iter",
            "extra": "iterations: 9656\ncpu: 0.0711845311723303 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.002376278263910917,
            "unit": "ms/iter",
            "extra": "iterations: 294501\ncpu: 0.0023761202814251733 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5361571298002982,
            "unit": "ms/iter",
            "extra": "iterations: 1302\ncpu: 0.5361216251920152 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 39.57764200000232,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.574507444445175 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6326.945323000018,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6326.466113999999 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.0053087350462066,
            "unit": "ms/iter",
            "extra": "iterations: 130502\ncpu: 0.0053079582228625155 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6088043379790767,
            "unit": "ms/iter",
            "extra": "iterations: 1148\ncpu: 0.6087878275261293 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 40.05060055555355,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 40.05049722222351 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6394.280632999994,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6388.634731000025 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005588193512304035,
            "unit": "ms/iter",
            "extra": "iterations: 129630\ncpu: 0.005588003286276403 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.631450722272324,
            "unit": "ms/iter",
            "extra": "iterations: 1109\ncpu: 0.6314478584310192 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 65.72478890908748,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 65.71408554545532 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 26010.682376000033,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 25972.314688000013 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.013765863091841612,
            "unit": "ms/iter",
            "extra": "iterations: 50837\ncpu: 0.013763423058008763 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7128768160569159,
            "unit": "ms/iter",
            "extra": "iterations: 984\ncpu: 0.7127921646341483 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 50.82821392857032,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 50.822395071427145 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 18157.557760000032,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 18089.541829000013 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.008276989440127929,
            "unit": "ms/iter",
            "extra": "iterations: 85323\ncpu: 0.00827663501048941 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 2.1878741281248892,
            "unit": "ms/iter",
            "extra": "iterations: 320\ncpu: 2.187665200000044 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 194.78976699998896,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 194.7755092499932 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 24305.00282600002,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 24304.038368999984 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.012535490655977836,
            "unit": "ms/iter",
            "extra": "iterations: 55383\ncpu: 0.012535279526208176 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.740985496259409,
            "unit": "ms/iter",
            "extra": "iterations: 401\ncpu: 1.7408621147131655 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 129.9823693999997,
            "unit": "ms/iter",
            "extra": "iterations: 5\ncpu: 129.9768236000034 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 13560.687196999992,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 13560.082443999989 ms\nthreads: 1"
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
          "id": "3c19ac236f0b0e2bdde97857f7045c6edc3a4165",
          "message": "Merge branch 'google_benchmark'\n\n# Conflicts:\n#\tREADME.md\n#\tbenchmarks/manual/benchmarks.cpp\n#\toctree.h",
          "timestamp": "2025-01-23T01:02:07+01:00",
          "tree_id": "97bbb8f42513110e6bdafee65cb06cf87c018c0c",
          "url": "https://github.com/attcs/Octree/commit/3c19ac236f0b0e2bdde97857f7045c6edc3a4165"
        },
        "date": 1737590889222,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1810.2663498407885,
            "unit": "ns/iter",
            "extra": "iterations: 393276\ncpu: 1810.2004266723627 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 2186.386938658265,
            "unit": "ns/iter",
            "extra": "iterations: 320304\ncpu: 2186.373763674509 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.0002744066379802126,
            "unit": "ms/iter",
            "extra": "iterations: 2548938\ncpu: 0.0002743962673866528 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.00034111644958728096,
            "unit": "ms/iter",
            "extra": "iterations: 2049711\ncpu: 0.00034110531435895106 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0011609773215934512,
            "unit": "ms/iter",
            "extra": "iterations: 606480\ncpu: 0.001160949498746867 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0017956186137874652,
            "unit": "ms/iter",
            "extra": "iterations: 390979\ncpu: 0.0017955240665099657 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.030322030251810028,
            "unit": "ms/iter",
            "extra": "iterations: 23073\ncpu: 0.030320992588740086 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.8139580846867541,
            "unit": "ms/iter",
            "extra": "iterations: 862\ncpu: 0.8139361368909516 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 10.139884235293968,
            "unit": "ms/iter",
            "extra": "iterations: 68\ncpu: 10.139547676470583 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 69.67919188889167,
            "unit": "ms/iter",
            "extra": "iterations: 9\ncpu: 69.67699566666674 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0016959724759708597,
            "unit": "ms/iter",
            "extra": "iterations: 411168\ncpu: 0.0016958840741497366 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.002518280451179154,
            "unit": "ms/iter",
            "extra": "iterations: 278027\ncpu: 0.002518073054055905 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.004662436163223595,
            "unit": "ms/iter",
            "extra": "iterations: 150689\ncpu: 0.004659563047070456 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.0062587970343657645,
            "unit": "ms/iter",
            "extra": "iterations: 111477\ncpu: 0.006233384760982081 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.047172810827170036,
            "unit": "ms/iter",
            "extra": "iterations: 14870\ncpu: 0.04703994418291858 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6238938364611148,
            "unit": "ms/iter",
            "extra": "iterations: 1119\ncpu: 0.6237999177837367 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.652304247618953,
            "unit": "ms/iter",
            "extra": "iterations: 105\ncpu: 6.651839276190482 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 33.17462861904747,
            "unit": "ms/iter",
            "extra": "iterations: 21\ncpu: 33.17022490476196 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.003278450687815848,
            "unit": "ms/iter",
            "extra": "iterations: 213720\ncpu: 0.0032784017218790967 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006395857994213875,
            "unit": "ms/iter",
            "extra": "iterations: 109573\ncpu: 0.006395715650753375 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.016629407767911837,
            "unit": "ms/iter",
            "extra": "iterations: 42122\ncpu: 0.016629133421964763 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.04157398076695594,
            "unit": "ms/iter",
            "extra": "iterations: 16898\ncpu: 0.04157327571310205 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.4011079353176683,
            "unit": "ms/iter",
            "extra": "iterations: 1747\ncpu: 0.40109323239839806 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.92847487500012,
            "unit": "ms/iter",
            "extra": "iterations: 240\ncpu: 2.928357516666669 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.00046027099978079595,
            "unit": "ms/iter",
            "extra": "iterations: 1523754\ncpu: 0.0004602505384727461 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0016103926400668123,
            "unit": "ms/iter",
            "extra": "iterations: 435167\ncpu: 0.001610348974072028 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.002866848174148156,
            "unit": "ms/iter",
            "extra": "iterations: 245584\ncpu: 0.0028667906622581313 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.00939521206949818,
            "unit": "ms/iter",
            "extra": "iterations: 74419\ncpu: 0.009394681976377004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14528968610141865,
            "unit": "ms/iter",
            "extra": "iterations: 4871\ncpu: 0.14528268528023044 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7412183366583007,
            "unit": "ms/iter",
            "extra": "iterations: 401\ncpu: 1.7410907980049863 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.0025917627907666866,
            "unit": "ms/iter",
            "extra": "iterations: 269804\ncpu: 0.0025915576418437114 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.009822934927495652,
            "unit": "ms/iter",
            "extra": "iterations: 71720\ncpu: 0.009822199860568872 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.029855311839909793,
            "unit": "ms/iter",
            "extra": "iterations: 23387\ncpu: 0.0298531370419464 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.09439411363942557,
            "unit": "ms/iter",
            "extra": "iterations: 7427\ncpu: 0.09438667927830836 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 2.158890843077013,
            "unit": "ms/iter",
            "extra": "iterations: 325\ncpu: 2.1587747353846196 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 23.360924466667637,
            "unit": "ms/iter",
            "extra": "iterations: 30\ncpu: 23.35946006666655 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004909096642189357,
            "unit": "ms/iter",
            "extra": "iterations: 1426644\ncpu: 0.0004908893466064402 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0007997641848782685,
            "unit": "ms/iter",
            "extra": "iterations: 862362\ncpu: 0.0007997363125926251 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.002230683884323897,
            "unit": "ms/iter",
            "extra": "iterations: 313496\ncpu: 0.0022305675096332806 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004865951053012807,
            "unit": "ms/iter",
            "extra": "iterations: 146342\ncpu: 0.004865719089529992 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06374426671059803,
            "unit": "ms/iter",
            "extra": "iterations: 10622\ncpu: 0.063740737902466 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.0953202304074667,
            "unit": "ms/iter",
            "extra": "iterations: 638\ncpu: 1.0952473385579922 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.724032771865947,
            "unit": "ns/iter",
            "extra": "iterations: 39549899\ncpu: 17.723358787844173 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.272055781684564,
            "unit": "ns/iter",
            "extra": "iterations: 22502297\ncpu: 31.27024018925726 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 45.448960578854205,
            "unit": "ns/iter",
            "extra": "iterations: 15306404\ncpu: 45.447483158030245 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 87.71100944669458,
            "unit": "ns/iter",
            "extra": "iterations: 7999303\ncpu: 87.70469139623752 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 293.43439167742434,
            "unit": "ns/iter",
            "extra": "iterations: 2348795\ncpu: 293.4202869982277 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 1835.5044410058124,
            "unit": "ns/iter",
            "extra": "iterations: 335397\ncpu: 1835.385122705336 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.017288278039864057,
            "unit": "ms/iter",
            "extra": "iterations: 40487\ncpu: 0.017287646306221628 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.3760312721893563,
            "unit": "ms/iter",
            "extra": "iterations: 1859\ncpu: 0.3760147563206019 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.001531172338818401,
            "unit": "ms/iter",
            "extra": "iterations: 458086\ncpu: 0.001531073457822342 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.026511946672117568,
            "unit": "ms/iter",
            "extra": "iterations: 26909\ncpu: 0.026497056561001864 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0003501114997409957,
            "unit": "ms/iter",
            "extra": "iterations: 2003906\ncpu: 0.00035009912840223035 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005221043755561831,
            "unit": "ms/iter",
            "extra": "iterations: 1346206\ncpu: 0.0005220902157619222 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0016818332852950107,
            "unit": "ms/iter",
            "extra": "iterations: 416334\ncpu: 0.00168182110036652 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.003314164618284529,
            "unit": "ms/iter",
            "extra": "iterations: 212200\ncpu: 0.0033140230254476875 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.037062432305418065,
            "unit": "ms/iter",
            "extra": "iterations: 18938\ncpu: 0.03706154578096949 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0129436159419967,
            "unit": "ms/iter",
            "extra": "iterations: 690\ncpu: 1.0129108999999998 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.856128366197295,
            "unit": "ms/iter",
            "extra": "iterations: 71\ncpu: 9.856051760563409 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 100.97988257142886,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 100.97879599999972 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0019014003437643318,
            "unit": "ms/iter",
            "extra": "iterations: 367112\ncpu: 0.0019012597708601125 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.0028427071690817107,
            "unit": "ms/iter",
            "extra": "iterations: 246224\ncpu: 0.0028425269185782254 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005558735331763488,
            "unit": "ms/iter",
            "extra": "iterations: 126566\ncpu: 0.005558441074222126 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.009096837618139985,
            "unit": "ms/iter",
            "extra": "iterations: 77133\ncpu: 0.009096237148820912 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.05110091706853646,
            "unit": "ms/iter",
            "extra": "iterations: 13686\ncpu: 0.051056788396902114 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.49495744500002764,
            "unit": "ms/iter",
            "extra": "iterations: 1400\ncpu: 0.49451821571428256 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.142677499999758,
            "unit": "ms/iter",
            "extra": "iterations: 168\ncpu: 4.139684880952372 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 42.7795364375001,
            "unit": "ms/iter",
            "extra": "iterations: 16\ncpu: 42.771587249999854 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007671063401351983,
            "unit": "ms/iter",
            "extra": "iterations: 913766\ncpu: 0.0007670821140204295 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.002272069460898451,
            "unit": "ms/iter",
            "extra": "iterations: 307252\ncpu: 0.0022720235344277683 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.005135630871251054,
            "unit": "ms/iter",
            "extra": "iterations: 136172\ncpu: 0.005135159555562107 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.014813165287489116,
            "unit": "ms/iter",
            "extra": "iterations: 46767\ncpu: 0.014811446960463607 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23867288239292703,
            "unit": "ms/iter",
            "extra": "iterations: 2942\ncpu: 0.2386632076818487 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 3.011706047210397,
            "unit": "ms/iter",
            "extra": "iterations: 233\ncpu: 3.0114946824034243 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.027432708332412,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.02619312499984 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 304.17694050001387,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 304.14221949999387 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.004086378413349578,
            "unit": "ms/iter",
            "extra": "iterations: 171569\ncpu: 0.004085955976895587 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009161180139263064,
            "unit": "ms/iter",
            "extra": "iterations: 76402\ncpu: 0.009159657875448323 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.01838929648738777,
            "unit": "ms/iter",
            "extra": "iterations: 38376\ncpu: 0.01838383359391282 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.042246107063149574,
            "unit": "ms/iter",
            "extra": "iterations: 16579\ncpu: 0.04223882158151896 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.37388202157497274,
            "unit": "ms/iter",
            "extra": "iterations: 1854\ncpu: 0.3738191019417439 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.827620246963446,
            "unit": "ms/iter",
            "extra": "iterations: 247\ncpu: 2.8275007692307925 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.788742178569937,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 24.78757546428564 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 281.3729906666822,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 280.9311923333316 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0015495783063723255,
            "unit": "ms/iter",
            "extra": "iterations: 451764\ncpu: 0.0015495235587607641 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.003027276821312397,
            "unit": "ms/iter",
            "extra": "iterations: 231001\ncpu: 0.003027215778286711 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006328378919327601,
            "unit": "ms/iter",
            "extra": "iterations: 111116\ncpu: 0.006327490136433985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011624068759243536,
            "unit": "ms/iter",
            "extra": "iterations: 60181\ncpu: 0.011623031156012674 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.12690451505804823,
            "unit": "ms/iter",
            "extra": "iterations: 5512\ncpu: 0.1268870067126268 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0938523092948553,
            "unit": "ms/iter",
            "extra": "iterations: 624\ncpu: 1.093693270833344 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.002571345805305039,
            "unit": "ms/iter",
            "extra": "iterations: 272344\ncpu: 0.00257129251608265 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.007237266591107874,
            "unit": "ms/iter",
            "extra": "iterations: 97055\ncpu: 0.007237127165009593 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.01710924439631083,
            "unit": "ms/iter",
            "extra": "iterations: 41089\ncpu: 0.01710789031127541 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.039212921189135226,
            "unit": "ms/iter",
            "extra": "iterations: 17929\ncpu: 0.03921168860505324 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.39420911423748883,
            "unit": "ms/iter",
            "extra": "iterations: 1777\ncpu: 0.39420135565560105 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.2708471830983505,
            "unit": "ms/iter",
            "extra": "iterations: 213\ncpu: 3.2707395164318993 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005641205835886678,
            "unit": "ms/iter",
            "extra": "iterations: 1247591\ncpu: 0.0005641097491084809 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017345304876543659,
            "unit": "ms/iter",
            "extra": "iterations: 405246\ncpu: 0.0017344662846764574 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0031699083320443425,
            "unit": "ms/iter",
            "extra": "iterations: 219826\ncpu: 0.0031698483436900116 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007594792610963639,
            "unit": "ms/iter",
            "extra": "iterations: 93544\ncpu: 0.007594323933122382 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.119897522979669,
            "unit": "ms/iter",
            "extra": "iterations: 5853\ncpu: 0.11989199982914797 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 1.6627884180521795,
            "unit": "ms/iter",
            "extra": "iterations: 421\ncpu: 1.6627251781472583 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0016798176404072213,
            "unit": "ms/iter",
            "extra": "iterations: 407689\ncpu: 0.001679765931874527 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.003288446155073458,
            "unit": "ms/iter",
            "extra": "iterations: 213112\ncpu: 0.00328730312699429 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.00819202183159149,
            "unit": "ms/iter",
            "extra": "iterations: 84877\ncpu: 0.00819159457803649 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.01841357948839776,
            "unit": "ms/iter",
            "extra": "iterations: 37842\ncpu: 0.018412857407113785 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.3068495710537617,
            "unit": "ms/iter",
            "extra": "iterations: 2287\ncpu: 0.306835373414955 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 4.290892216049416,
            "unit": "ms/iter",
            "extra": "iterations: 162\ncpu: 4.290807314814797 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005203329693485756,
            "unit": "ms/iter",
            "extra": "iterations: 1342547\ncpu: 0.0005203086312806984 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0011912411442520443,
            "unit": "ms/iter",
            "extra": "iterations: 588262\ncpu: 0.0011911943368770974 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0035393225155626435,
            "unit": "ms/iter",
            "extra": "iterations: 197268\ncpu: 0.0035390456688362743 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.008097279420998487,
            "unit": "ms/iter",
            "extra": "iterations: 90777\ncpu: 0.008096842944798694 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.17593563991769445,
            "unit": "ms/iter",
            "extra": "iterations: 3888\ncpu: 0.17592347736625658 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 774.2293779999727,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 774.1975650000086 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011314164514299647,
            "unit": "ms/iter",
            "extra": "iterations: 617150\ncpu: 0.0011313764838369945 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.003971709232116257,
            "unit": "ms/iter",
            "extra": "iterations: 177045\ncpu: 0.0039715635968257034 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.01461045994680443,
            "unit": "ms/iter",
            "extra": "iterations: 47749\ncpu: 0.014610292655343568 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.04629223534921081,
            "unit": "ms/iter",
            "extra": "iterations: 14948\ncpu: 0.04628981087770936 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.3766219120000187,
            "unit": "ms/iter",
            "extra": "iterations: 500\ncpu: 1.376578660000007 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 140.33102300000166,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 140.32785775000178 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 72.94761463159993,
            "unit": "ns/iter",
            "extra": "iterations: 9592774\ncpu: 72.94336445328545 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 379.47952954199144,
            "unit": "ns/iter",
            "extra": "iterations: 1846881\ncpu: 379.47549517267123 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10908.230106182931,
            "unit": "ns/iter",
            "extra": "iterations: 64040\ncpu: 10907.89472204878 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 135985.76531610635,
            "unit": "ns/iter",
            "extra": "iterations: 5109\ncpu: 135982.97298884473 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 55.95382299999045,
            "unit": "ns/iter",
            "extra": "iterations: 12574745\ncpu: 55.95161182194936 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 157.27131490224946,
            "unit": "ns/iter",
            "extra": "iterations: 4438444\ncpu: 157.26325667283476 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2308.07694190222,
            "unit": "ns/iter",
            "extra": "iterations: 298290\ncpu: 2308.007559757256 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 38652.246658564676,
            "unit": "ns/iter",
            "extra": "iterations: 18106\ncpu: 38651.467248425826 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 79.80721248805088,
            "unit": "ns/iter",
            "extra": "iterations: 8524437\ncpu: 79.80419258186716 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 413.7727675997896,
            "unit": "ns/iter",
            "extra": "iterations: 1692976\ncpu: 413.7537513821777 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10702.00019886795,
            "unit": "ns/iter",
            "extra": "iterations: 65370\ncpu: 10701.510830656287 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 130313.1997011032,
            "unit": "ns/iter",
            "extra": "iterations: 5353\ncpu: 130304.13618531838 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 94.39308877010198,
            "unit": "ns/iter",
            "extra": "iterations: 7468280\ncpu: 94.38870864509622 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 225.86207908440966,
            "unit": "ns/iter",
            "extra": "iterations: 3124914\ncpu: 225.85264682483677 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3608.21179641182,
            "unit": "ns/iter",
            "extra": "iterations: 194805\ncpu: 3608.0545519878833 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 45779.11265673962,
            "unit": "ns/iter",
            "extra": "iterations: 15312\ncpu: 45776.819292058826 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.0026462306070056886,
            "unit": "ms/iter",
            "extra": "iterations: 268383\ncpu: 0.002646156239404113 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.061780937886069115,
            "unit": "ms/iter",
            "extra": "iterations: 11656\ncpu: 0.06177822777968349 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.0023532337938984146,
            "unit": "ms/iter",
            "extra": "iterations: 290585\ncpu: 0.0023531335444018386 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07135664424057046,
            "unit": "ms/iter",
            "extra": "iterations: 9810\ncpu: 0.07135417104995044 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.0023538838633971257,
            "unit": "ms/iter",
            "extra": "iterations: 297417\ncpu: 0.002353745337354667 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5345332448354814,
            "unit": "ms/iter",
            "extra": "iterations: 1307\ncpu: 0.534497500382541 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 39.47711044444683,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.47514588888781 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6291.920875000016,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6291.661992999991 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.00526344180958094,
            "unit": "ms/iter",
            "extra": "iterations: 129798\ncpu: 0.005260098144809729 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6049552523768142,
            "unit": "ms/iter",
            "extra": "iterations: 1157\ncpu: 0.6049483197925575 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 39.87038644444536,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.86996222222356 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6346.3992769999895,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6344.286103999992 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005530554383046828,
            "unit": "ms/iter",
            "extra": "iterations: 127457\ncpu: 0.005529001169021635 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.625711963261652,
            "unit": "ms/iter",
            "extra": "iterations: 1116\ncpu: 0.6257067983870929 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 63.18947209091034,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 63.18753336363546 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 23938.245673999972,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23912.69970800002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.013667765418762011,
            "unit": "ms/iter",
            "extra": "iterations: 50912\ncpu: 0.013666512413576406 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7108155257836472,
            "unit": "ms/iter",
            "extra": "iterations: 989\ncpu: 0.7107183720930262 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 50.013991785712214,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 50.00752135714289 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 17192.192750000002,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 17130.93684199998 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.008284907960669514,
            "unit": "ms/iter",
            "extra": "iterations: 84616\ncpu: 0.008284274451640428 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 2.1845955514017024,
            "unit": "ms/iter",
            "extra": "iterations: 321\ncpu: 2.184485819314604 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 193.6697414999884,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 193.63807624999652 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 23971.83408999996,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23970.592135000003 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.012441087962470824,
            "unit": "ms/iter",
            "extra": "iterations: 56490\ncpu: 0.01244050196494977 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.752037149253766,
            "unit": "ms/iter",
            "extra": "iterations: 402\ncpu: 1.7508174701492354 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 128.38242000000264,
            "unit": "ms/iter",
            "extra": "iterations: 5\ncpu: 128.38117619999707 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 13367.031039999973,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 13366.372124999998 ms\nthreads: 1"
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
          "id": "4e80160c8dfc0ea24a5969d1c6de6531e3bb7005",
          "message": "Improve GetNearestNeighbors() performance",
          "timestamp": "2025-01-23T01:15:41+01:00",
          "tree_id": "1673e3114246aa33c2fb99b06ffdd2a42b3dba24",
          "url": "https://github.com/attcs/Octree/commit/4e80160c8dfc0ea24a5969d1c6de6531e3bb7005"
        },
        "date": 1737591701105,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1827.213146306809,
            "unit": "ns/iter",
            "extra": "iterations: 384473\ncpu: 1827.0778182083006 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 2183.754933298431,
            "unit": "ns/iter",
            "extra": "iterations: 320982\ncpu: 2183.4526858203885 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.00027466127360762174,
            "unit": "ms/iter",
            "extra": "iterations: 2549388\ncpu: 0.00027459192990631485 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003389758923964442,
            "unit": "ms/iter",
            "extra": "iterations: 2062088\ncpu: 0.0003389083516319382 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.001160608361312706,
            "unit": "ms/iter",
            "extra": "iterations: 609378\ncpu: 0.001160353992103423 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0018070586990627472,
            "unit": "ms/iter",
            "extra": "iterations: 387536\ncpu: 0.0018070025649230001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.02934642090939618,
            "unit": "ms/iter",
            "extra": "iterations: 23884\ncpu: 0.029345504647462738 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.7948830158550373,
            "unit": "ms/iter",
            "extra": "iterations: 883\ncpu: 0.7948460758776897 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 9.905163757142889,
            "unit": "ms/iter",
            "extra": "iterations: 70\ncpu: 9.905013542857155 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 67.27295379999987,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 67.27010619999997 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0016732615098908898,
            "unit": "ms/iter",
            "extra": "iterations: 417858\ncpu: 0.0016731295368283018 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.0024989742082763933,
            "unit": "ms/iter",
            "extra": "iterations: 282106\ncpu: 0.0024979337483073713 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.004592027995395546,
            "unit": "ms/iter",
            "extra": "iterations: 152025\ncpu: 0.0045913161914158855 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.00617590384952507,
            "unit": "ms/iter",
            "extra": "iterations: 114092\ncpu: 0.006162032368614794 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04732026440266769,
            "unit": "ms/iter",
            "extra": "iterations: 14841\ncpu: 0.04710736399164482 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6212207209920229,
            "unit": "ms/iter",
            "extra": "iterations: 1129\ncpu: 0.6211369920283427 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.673111339622469,
            "unit": "ms/iter",
            "extra": "iterations: 106\ncpu: 6.673043292452827 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 34.31884405000005,
            "unit": "ms/iter",
            "extra": "iterations: 20\ncpu: 34.31654439999985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.003351663220377874,
            "unit": "ms/iter",
            "extra": "iterations: 209404\ncpu: 0.0033516288991614286 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.00647718986477121,
            "unit": "ms/iter",
            "extra": "iterations: 108335\ncpu: 0.00647703436562513 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.016872613006525195,
            "unit": "ms/iter",
            "extra": "iterations: 41533\ncpu: 0.01687241704187028 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.041893239935394636,
            "unit": "ms/iter",
            "extra": "iterations: 16717\ncpu: 0.04189211610934961 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.39947344412770647,
            "unit": "ms/iter",
            "extra": "iterations: 1754\ncpu: 0.39946672519954335 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.8875364338842635,
            "unit": "ms/iter",
            "extra": "iterations: 242\ncpu: 2.8874636115702375 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.00045501581687955523,
            "unit": "ms/iter",
            "extra": "iterations: 1534247\ncpu: 0.00045501616297766997 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0016083448913694473,
            "unit": "ms/iter",
            "extra": "iterations: 435743\ncpu: 0.0016083180315002237 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.002806197385746855,
            "unit": "ms/iter",
            "extra": "iterations: 249096\ncpu: 0.002806201119247187 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009376574077544985,
            "unit": "ms/iter",
            "extra": "iterations: 74692\ncpu: 0.009376298331815969 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.142452632590474,
            "unit": "ms/iter",
            "extra": "iterations: 4891\ncpu: 0.14245071498671022 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7023326593673938,
            "unit": "ms/iter",
            "extra": "iterations: 411\ncpu: 1.7022972019464744 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.0011804648404896989,
            "unit": "ms/iter",
            "extra": "iterations: 591874\ncpu: 0.0011804375475185584 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.0028305695670398727,
            "unit": "ms/iter",
            "extra": "iterations: 245727\ncpu: 0.0028268811038266116 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.0054078273163887325,
            "unit": "ms/iter",
            "extra": "iterations: 129717\ncpu: 0.005407684667391328 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.020091114514890894,
            "unit": "ms/iter",
            "extra": "iterations: 35157\ncpu: 0.020091150439457375 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.7324102258403201,
            "unit": "ms/iter",
            "extra": "iterations: 952\ncpu: 0.7323825126050432 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 10.00667202857122,
            "unit": "ms/iter",
            "extra": "iterations: 70\ncpu: 10.006408757142841 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004964717518712697,
            "unit": "ms/iter",
            "extra": "iterations: 1274403\ncpu: 0.0004964724949643075 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0008222072752846998,
            "unit": "ms/iter",
            "extra": "iterations: 868777\ncpu: 0.0008221945769743044 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.002247167653955717,
            "unit": "ms/iter",
            "extra": "iterations: 309602\ncpu: 0.0022471730996569865 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004682782072455307,
            "unit": "ms/iter",
            "extra": "iterations: 151376\ncpu: 0.004682745626783661 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06220528326612977,
            "unit": "ms/iter",
            "extra": "iterations: 10912\ncpu: 0.062205378115835576 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.0060565589080332,
            "unit": "ms/iter",
            "extra": "iterations: 696\ncpu: 1.006027225574713 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.753306780148165,
            "unit": "ns/iter",
            "extra": "iterations: 39376068\ncpu: 17.75284299590297 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.435446093412395,
            "unit": "ns/iter",
            "extra": "iterations: 22434382\ncpu: 31.435143700414837 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 45.176080021796345,
            "unit": "ns/iter",
            "extra": "iterations: 15422698\ncpu: 45.17546138814372 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 89.66794338648167,
            "unit": "ns/iter",
            "extra": "iterations: 7874868\ncpu: 89.66398357407446 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 255.15056127668728,
            "unit": "ns/iter",
            "extra": "iterations: 2691097\ncpu: 255.15074150058365 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2027.871369054332,
            "unit": "ns/iter",
            "extra": "iterations: 328117\ncpu: 2027.824769822959 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.01759676675946168,
            "unit": "ms/iter",
            "extra": "iterations: 39873\ncpu: 0.017596795575953488 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.37972197295834526,
            "unit": "ms/iter",
            "extra": "iterations: 1849\ncpu: 0.3796909675500276 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015384444211115793,
            "unit": "ms/iter",
            "extra": "iterations: 457152\ncpu: 0.0015383849966750629 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.02579929505689967,
            "unit": "ms/iter",
            "extra": "iterations: 28120\ncpu: 0.025796714509246123 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.00036308667428069084,
            "unit": "ms/iter",
            "extra": "iterations: 1935903\ncpu: 0.0003630680622944459 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005219640492387219,
            "unit": "ms/iter",
            "extra": "iterations: 1345201\ncpu: 0.0005219066920110819 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.001735693292636247,
            "unit": "ms/iter",
            "extra": "iterations: 404943\ncpu: 0.0017355975211326103 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.0033411896875835158,
            "unit": "ms/iter",
            "extra": "iterations: 209592\ncpu: 0.0033408548894995984 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03684966218860715,
            "unit": "ms/iter",
            "extra": "iterations: 18907\ncpu: 0.03684796731369361 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0049711090116284,
            "unit": "ms/iter",
            "extra": "iterations: 688\ncpu: 1.0048450625000023 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.884368361111248,
            "unit": "ms/iter",
            "extra": "iterations: 72\ncpu: 9.883807736111095 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 100.56501728571529,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 100.56067142857168 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0019244683429213718,
            "unit": "ms/iter",
            "extra": "iterations: 362952\ncpu: 0.0019242333063325138 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.002866869268869377,
            "unit": "ms/iter",
            "extra": "iterations: 245305\ncpu: 0.0028667778194492468 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005476370785108134,
            "unit": "ms/iter",
            "extra": "iterations: 127613\ncpu: 0.005470254284438102 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.008880990347880877,
            "unit": "ms/iter",
            "extra": "iterations: 79050\ncpu: 0.008877103504111257 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.049202334340311564,
            "unit": "ms/iter",
            "extra": "iterations: 14234\ncpu: 0.04919620458058208 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.4913044535864936,
            "unit": "ms/iter",
            "extra": "iterations: 1422\ncpu: 0.49110409845288333 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.07778558235283,
            "unit": "ms/iter",
            "extra": "iterations: 170\ncpu: 4.074989670588239 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 41.695496235293454,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 41.69406623529426 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007628412114550828,
            "unit": "ms/iter",
            "extra": "iterations: 918895\ncpu: 0.000762822395377058 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.0024529213769354753,
            "unit": "ms/iter",
            "extra": "iterations: 268916\ncpu: 0.0024527356758244395 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.005339574023621931,
            "unit": "ms/iter",
            "extra": "iterations: 131404\ncpu: 0.005339492473592862 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.015039199353955101,
            "unit": "ms/iter",
            "extra": "iterations: 46746\ncpu: 0.015038528366063356 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23704683872042154,
            "unit": "ms/iter",
            "extra": "iterations: 3001\ncpu: 0.23703729856714784 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 3.008820443965462,
            "unit": "ms/iter",
            "extra": "iterations: 232\ncpu: 3.0087031896552316 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.619747666667234,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.618172541666727 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 309.3921265000006,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 309.3843289999967 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.0041201437180316,
            "unit": "ms/iter",
            "extra": "iterations: 170090\ncpu: 0.00411947742371688 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009207661020089575,
            "unit": "ms/iter",
            "extra": "iterations: 75562\ncpu: 0.009199869590534883 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.01858930842977857,
            "unit": "ms/iter",
            "extra": "iterations: 37759\ncpu: 0.018570573373235734 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04273744353270145,
            "unit": "ms/iter",
            "extra": "iterations: 16452\ncpu: 0.042666070629710506 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.37195487055702087,
            "unit": "ms/iter",
            "extra": "iterations: 1885\ncpu: 0.37192928594164726 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.870068021276646,
            "unit": "ms/iter",
            "extra": "iterations: 235\ncpu: 2.869572331914885 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 25.131117749999987,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 25.130237964285737 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 284.6007913333324,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 282.8481763333362 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0015628317701394444,
            "unit": "ms/iter",
            "extra": "iterations: 450360\ncpu: 0.0015625703725908112 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.003062422401421142,
            "unit": "ms/iter",
            "extra": "iterations: 228548\ncpu: 0.003061838160911478 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006327647313590056,
            "unit": "ms/iter",
            "extra": "iterations: 111059\ncpu: 0.006327010237801614 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011596852262372252,
            "unit": "ms/iter",
            "extra": "iterations: 60357\ncpu: 0.011595938863760542 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.12515186580783647,
            "unit": "ms/iter",
            "extra": "iterations: 5589\ncpu: 0.12514982680264936 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0685153633587803,
            "unit": "ms/iter",
            "extra": "iterations: 655\ncpu: 1.0684975648854897 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.0025414537054874774,
            "unit": "ms/iter",
            "extra": "iterations: 275119\ncpu: 0.002541352451121162 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.0071249627444196755,
            "unit": "ms/iter",
            "extra": "iterations: 98294\ncpu: 0.007124771868069318 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.016741722956600653,
            "unit": "ms/iter",
            "extra": "iterations: 41683\ncpu: 0.01674085257778941 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.03824043484434694,
            "unit": "ms/iter",
            "extra": "iterations: 18310\ncpu: 0.038239226925177845 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.3886072567792007,
            "unit": "ms/iter",
            "extra": "iterations: 1807\ncpu: 0.3885821361372406 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.1654209321265925,
            "unit": "ms/iter",
            "extra": "iterations: 221\ncpu: 3.165259846153853 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005494136651932059,
            "unit": "ms/iter",
            "extra": "iterations: 1275679\ncpu: 0.0005493823171816726 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017057656875232694,
            "unit": "ms/iter",
            "extra": "iterations: 410597\ncpu: 0.0017057040358307725 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0031196401443355504,
            "unit": "ms/iter",
            "extra": "iterations: 224754\ncpu: 0.003119502415974766 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.0073754837277637715,
            "unit": "ms/iter",
            "extra": "iterations: 94609\ncpu: 0.007375287573063819 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.11685108421228815,
            "unit": "ms/iter",
            "extra": "iterations: 5973\ncpu: 0.11685117261007838 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 1.604949866972494,
            "unit": "ms/iter",
            "extra": "iterations: 436\ncpu: 1.604878341743142 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0017281279217057656,
            "unit": "ms/iter",
            "extra": "iterations: 406569\ncpu: 0.0017281131173306236 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.0033082991752617125,
            "unit": "ms/iter",
            "extra": "iterations: 211946\ncpu: 0.003308242132429921 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008178951454723672,
            "unit": "ms/iter",
            "extra": "iterations: 85652\ncpu: 0.008178799164059296 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.018763146413119006,
            "unit": "ms/iter",
            "extra": "iterations: 37442\ncpu: 0.018761974333635783 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.3062834239510526,
            "unit": "ms/iter",
            "extra": "iterations: 2288\ncpu: 0.3062681608391656 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 4.200956843373533,
            "unit": "ms/iter",
            "extra": "iterations: 166\ncpu: 4.20080426506024 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005212303817434033,
            "unit": "ms/iter",
            "extra": "iterations: 1345354\ncpu: 0.0005212154867789403 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0011922218579346804,
            "unit": "ms/iter",
            "extra": "iterations: 588363\ncpu: 0.0011921760698752233 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.003530886986767473,
            "unit": "ms/iter",
            "extra": "iterations: 198375\ncpu: 0.0035307970863264365 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.007856964870988283,
            "unit": "ms/iter",
            "extra": "iterations: 90922\ncpu: 0.007856472107960692 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.17518157047668298,
            "unit": "ms/iter",
            "extra": "iterations: 3902\ncpu: 0.17518005715018145 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 784.934757000002,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 784.9098250000139 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011618667172763107,
            "unit": "ms/iter",
            "extra": "iterations: 615166\ncpu: 0.0011618140388122823 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.003973467961793947,
            "unit": "ms/iter",
            "extra": "iterations: 175993\ncpu: 0.003973289494468532 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.0147371865532089,
            "unit": "ms/iter",
            "extra": "iterations: 47595\ncpu: 0.01473623523479336 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.04479441035655736,
            "unit": "ms/iter",
            "extra": "iterations: 15874\ncpu: 0.04479203458485536 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.2986300586011148,
            "unit": "ms/iter",
            "extra": "iterations: 529\ncpu: 1.2985913289224948 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 139.80586499999959,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 139.80307950000181 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 71.62208813446367,
            "unit": "ns/iter",
            "extra": "iterations: 9767235\ncpu: 71.62214854050333 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 376.4351659717474,
            "unit": "ns/iter",
            "extra": "iterations: 1860196\ncpu: 376.4244423705926 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10984.261469982504,
            "unit": "ns/iter",
            "extra": "iterations: 63579\ncpu: 10984.142672895143 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 138100.37531931722,
            "unit": "ns/iter",
            "extra": "iterations: 5089\ncpu: 138096.16172136198 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 56.18874310453557,
            "unit": "ns/iter",
            "extra": "iterations: 12414684\ncpu: 56.18764658045338 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 152.33696247635996,
            "unit": "ns/iter",
            "extra": "iterations: 4603658\ncpu: 152.33733001017717 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2277.6965139964814,
            "unit": "ns/iter",
            "extra": "iterations: 301434\ncpu: 2277.6157201908113 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 39134.63838788726,
            "unit": "ns/iter",
            "extra": "iterations: 17964\ncpu: 39133.51347138707 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 78.67477976927796,
            "unit": "ns/iter",
            "extra": "iterations: 8933245\ncpu: 78.67320699253173 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 383.0046422009999,
            "unit": "ns/iter",
            "extra": "iterations: 1819611\ncpu: 382.99814740623094 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10834.676866527403,
            "unit": "ns/iter",
            "extra": "iterations: 64478\ncpu: 10834.190731722449 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 132342.71881448498,
            "unit": "ns/iter",
            "extra": "iterations: 5331\ncpu: 132342.75089101514 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 93.54978188875342,
            "unit": "ns/iter",
            "extra": "iterations: 7664896\ncpu: 93.54822426814259 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 223.40842046787077,
            "unit": "ns/iter",
            "extra": "iterations: 3186165\ncpu: 223.40600753571042 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3606.3674736668418,
            "unit": "ns/iter",
            "extra": "iterations: 194052\ncpu: 3606.2087327107834 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 45547.305896885606,
            "unit": "ns/iter",
            "extra": "iterations: 15381\ncpu: 45547.352447824305 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.0026622343789401686,
            "unit": "ms/iter",
            "extra": "iterations: 261730\ncpu: 0.002662223642685203 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.06025824195347742,
            "unit": "ms/iter",
            "extra": "iterations: 11651\ncpu: 0.06025757042313966 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.002368121189233526,
            "unit": "ms/iter",
            "extra": "iterations: 295249\ncpu: 0.002368039207584178 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07019311926697785,
            "unit": "ms/iter",
            "extra": "iterations: 9986\ncpu: 0.07018972261165722 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.002509912306003168,
            "unit": "ms/iter",
            "extra": "iterations: 275971\ncpu: 0.002509793217403248 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.524267085949152,
            "unit": "ms/iter",
            "extra": "iterations: 1338\ncpu: 0.5242484402092814 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 38.771336500000565,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 38.76817055555648 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6295.254905999968,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6294.059931999982 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.005492182762201177,
            "unit": "ms/iter",
            "extra": "iterations: 126153\ncpu: 0.005492189357367685 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6001618113207642,
            "unit": "ms/iter",
            "extra": "iterations: 1166\ncpu: 0.6000953987993098 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 38.99743122221998,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 38.995448611111605 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6369.155235999983,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6368.738581999992 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.00557217182487207,
            "unit": "ms/iter",
            "extra": "iterations: 126239\ncpu: 0.005571602587156151 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.6303699321880999,
            "unit": "ms/iter",
            "extra": "iterations: 1106\ncpu: 0.630238996383348 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 63.89007490908873,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 63.881195454544326 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 24886.20631600003,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 24867.547762000017 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.01345611559479618,
            "unit": "ms/iter",
            "extra": "iterations: 51421\ncpu: 0.013455250150716747 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7241055634674934,
            "unit": "ms/iter",
            "extra": "iterations: 969\ncpu: 0.7241008988648056 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 50.80355199999888,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 50.80258335714153 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 17454.863405000026,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 17392.230418999985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.008090938882421246,
            "unit": "ms/iter",
            "extra": "iterations: 87618\ncpu: 0.008090345636741396 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 2.148270972392669,
            "unit": "ms/iter",
            "extra": "iterations: 326\ncpu: 2.14787516564418 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 193.54521024999372,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 193.52477099999987 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 23649.285331999978,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23647.671309000005 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.012933616498366655,
            "unit": "ms/iter",
            "extra": "iterations: 53872\ncpu: 0.012932308768933747 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.7455535610973076,
            "unit": "ms/iter",
            "extra": "iterations: 401\ncpu: 1.7454602319202095 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 128.5516388000019,
            "unit": "ms/iter",
            "extra": "iterations: 5\ncpu: 128.54816260000348 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 13348.101911000014,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 13347.015040000002 ms\nthreads: 1"
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
          "id": "8fa7d18ff2fcfcd7a7c7eaa3d2f6514d453be6f6",
          "message": "Improve GetNearestNeighbors() performance #2",
          "timestamp": "2025-01-24T00:57:12+01:00",
          "tree_id": "d1b1885e70d294c0c852d76ff11f532e6833e7eb",
          "url": "https://github.com/attcs/Octree/commit/8fa7d18ff2fcfcd7a7c7eaa3d2f6514d453be6f6"
        },
        "date": 1737676978414,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1808.2226227778428,
            "unit": "ns/iter",
            "extra": "iterations: 394452\ncpu: 1808.2047929786133 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 2188.279719688491,
            "unit": "ns/iter",
            "extra": "iterations: 319359\ncpu: 2188.274005116499 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.00027928147813855017,
            "unit": "ms/iter",
            "extra": "iterations: 2510536\ncpu: 0.0002792782593039894 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.00033760151836807447,
            "unit": "ms/iter",
            "extra": "iterations: 2072620\ncpu: 0.00033760086557111307 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0011202084950672094,
            "unit": "ms/iter",
            "extra": "iterations: 624998\ncpu: 0.001120120205184657 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.001726858880575004,
            "unit": "ms/iter",
            "extra": "iterations: 406280\ncpu: 0.0017267767106429047 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.028584561846405548,
            "unit": "ms/iter",
            "extra": "iterations: 24480\ncpu: 0.028583596037581704 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.7942025238095077,
            "unit": "ms/iter",
            "extra": "iterations: 882\ncpu: 0.794163107709751 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 9.933260347826247,
            "unit": "ms/iter",
            "extra": "iterations: 69\ncpu: 9.933218942028976 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 66.3695405999988,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 66.36625229999993 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0016720295341496872,
            "unit": "ms/iter",
            "extra": "iterations: 418160\ncpu: 0.0016719515376889208 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.002480552101338264,
            "unit": "ms/iter",
            "extra": "iterations: 281749\ncpu: 0.0024805202928848014 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.0045927783694455285,
            "unit": "ms/iter",
            "extra": "iterations: 152488\ncpu: 0.004592772165678616 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.00624051633252125,
            "unit": "ms/iter",
            "extra": "iterations: 112077\ncpu: 0.006240462690828627 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.046675420039946854,
            "unit": "ms/iter",
            "extra": "iterations: 15020\ncpu: 0.04666908521970707 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6169541222515653,
            "unit": "ms/iter",
            "extra": "iterations: 1137\ncpu: 0.6167528390501331 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.609032443396434,
            "unit": "ms/iter",
            "extra": "iterations: 106\ncpu: 6.608612924528303 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 32.42933866666709,
            "unit": "ms/iter",
            "extra": "iterations: 21\ncpu: 32.428178904761786 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.0033061784217762345,
            "unit": "ms/iter",
            "extra": "iterations: 213113\ncpu: 0.0033060508650340514 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006386889189630723,
            "unit": "ms/iter",
            "extra": "iterations: 110098\ncpu: 0.006386757216298217 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.017170502792206107,
            "unit": "ms/iter",
            "extra": "iterations: 41007\ncpu: 0.017169717974979907 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.041906387967672654,
            "unit": "ms/iter",
            "extra": "iterations: 16705\ncpu: 0.041905188805746674 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.40667069441211534,
            "unit": "ms/iter",
            "extra": "iterations: 1718\ncpu: 0.40665927357392323 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.9343136945606787,
            "unit": "ms/iter",
            "extra": "iterations: 239\ncpu: 2.934252154811716 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.00045954549424590446,
            "unit": "ms/iter",
            "extra": "iterations: 1534216\ncpu: 0.0004595409518607562 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.001582178902721534,
            "unit": "ms/iter",
            "extra": "iterations: 443716\ncpu: 0.0015821509028297405 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.002820308983894555,
            "unit": "ms/iter",
            "extra": "iterations: 248055\ncpu: 0.002820268053455892 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009379176360200287,
            "unit": "ms/iter",
            "extra": "iterations: 74603\ncpu: 0.009378885741860212 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14158627214040576,
            "unit": "ms/iter",
            "extra": "iterations: 4957\ncpu: 0.1415827137381479 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7364202711441794,
            "unit": "ms/iter",
            "extra": "iterations: 402\ncpu: 1.7363805348258685 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.001147227063739521,
            "unit": "ms/iter",
            "extra": "iterations: 610736\ncpu: 0.0011471855498938997 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.0027328978042988456,
            "unit": "ms/iter",
            "extra": "iterations: 255226\ncpu: 0.0027327764491078526 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.005336078724244956,
            "unit": "ms/iter",
            "extra": "iterations: 131624\ncpu: 0.005336027867258256 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.017788170861666282,
            "unit": "ms/iter",
            "extra": "iterations: 39412\ncpu: 0.017788263193951114 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.4317147295132398,
            "unit": "ms/iter",
            "extra": "iterations: 1623\ncpu: 0.43170314664202064 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 5.9693666974791855,
            "unit": "ms/iter",
            "extra": "iterations: 119\ncpu: 5.969242436974823 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004878610325974189,
            "unit": "ms/iter",
            "extra": "iterations: 1434286\ncpu: 0.00048784110351770936 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0007836258062921108,
            "unit": "ms/iter",
            "extra": "iterations: 889876\ncpu: 0.0007836238082609268 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.0022300159289324107,
            "unit": "ms/iter",
            "extra": "iterations: 313392\ncpu: 0.002230007048680228 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.0047778807181775955,
            "unit": "ms/iter",
            "extra": "iterations: 147206\ncpu: 0.0047777145292990444 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06347649632145207,
            "unit": "ms/iter",
            "extra": "iterations: 10602\ncpu: 0.06347565100924324 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.0387776796267596,
            "unit": "ms/iter",
            "extra": "iterations: 643\ncpu: 1.036242090202184 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.751530232843287,
            "unit": "ns/iter",
            "extra": "iterations: 39503792\ncpu: 17.7512634230153 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.193065738269365,
            "unit": "ns/iter",
            "extra": "iterations: 22521417\ncpu: 31.192653197620768 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 45.23536481200505,
            "unit": "ns/iter",
            "extra": "iterations: 15472572\ncpu: 45.233470169018716 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 88.77627989070068,
            "unit": "ns/iter",
            "extra": "iterations: 7879247\ncpu: 88.77421103818638 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 254.16447799093993,
            "unit": "ns/iter",
            "extra": "iterations: 2780220\ncpu: 254.1636938803418 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2064.1593608621724,
            "unit": "ns/iter",
            "extra": "iterations: 332886\ncpu: 2064.1221979897136 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.009659662019392322,
            "unit": "ms/iter",
            "extra": "iterations: 71368\ncpu: 0.009659625126106899 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.21646335323230292,
            "unit": "ms/iter",
            "extra": "iterations: 3233\ncpu: 0.21646096133622117 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015454789463262557,
            "unit": "ms/iter",
            "extra": "iterations: 464622\ncpu: 0.0015454494513819847 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.02527943262867841,
            "unit": "ms/iter",
            "extra": "iterations: 27957\ncpu: 0.02527859029938834 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.00034693101304756176,
            "unit": "ms/iter",
            "extra": "iterations: 2018918\ncpu: 0.00034692414699358617 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005392143302267327,
            "unit": "ms/iter",
            "extra": "iterations: 1298193\ncpu: 0.0005391972341554771 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0017412387800773922,
            "unit": "ms/iter",
            "extra": "iterations: 403501\ncpu: 0.001741208286968307 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.003388964107245775,
            "unit": "ms/iter",
            "extra": "iterations: 208120\ncpu: 0.003388779444551226 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03765854099596068,
            "unit": "ms/iter",
            "extra": "iterations: 18575\ncpu: 0.037656261641992 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0264745668135462,
            "unit": "ms/iter",
            "extra": "iterations: 681\ncpu: 1.0264290602055763 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.933248314285947,
            "unit": "ms/iter",
            "extra": "iterations: 70\ncpu: 9.933050585714247 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 101.35934742856989,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 101.3564628571422 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.001905299771915321,
            "unit": "ms/iter",
            "extra": "iterations: 366969\ncpu: 0.0019052329215819399 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.002780343331745536,
            "unit": "ms/iter",
            "extra": "iterations: 251940\ncpu: 0.002780303000714456 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005389888816119361,
            "unit": "ms/iter",
            "extra": "iterations: 129785\ncpu: 0.005389771198520628 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.00849888831702952,
            "unit": "ms/iter",
            "extra": "iterations: 82188\ncpu: 0.008498541611914132 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.0491876606589705,
            "unit": "ms/iter",
            "extra": "iterations: 14204\ncpu: 0.049164646508026065 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.47974505234532844,
            "unit": "ms/iter",
            "extra": "iterations: 1471\ncpu: 0.47894032766825556 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.060695479768983,
            "unit": "ms/iter",
            "extra": "iterations: 173\ncpu: 4.059609104046232 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 41.495351823527926,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 41.477533235293734 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007598024682576955,
            "unit": "ms/iter",
            "extra": "iterations: 927942\ncpu: 0.0007597524478900699 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.0021238681453502124,
            "unit": "ms/iter",
            "extra": "iterations: 330925\ncpu: 0.002123690352798963 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.005228578654534447,
            "unit": "ms/iter",
            "extra": "iterations: 134080\ncpu: 0.005228541147076368 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.014838049490229707,
            "unit": "ms/iter",
            "extra": "iterations: 47080\ncpu: 0.014837839358538711 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23931571629502466,
            "unit": "ms/iter",
            "extra": "iterations: 2915\ncpu: 0.23931282435677625 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 2.978287850427163,
            "unit": "ms/iter",
            "extra": "iterations: 234\ncpu: 2.9782262051281854 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 28.95114854166536,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 28.951271416666795 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 302.75351500000625,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 302.7503899999999 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.004031943492798758,
            "unit": "ms/iter",
            "extra": "iterations: 175252\ncpu: 0.004031697013443518 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009150295978461372,
            "unit": "ms/iter",
            "extra": "iterations: 77259\ncpu: 0.009142560323069215 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.018281933478821732,
            "unit": "ms/iter",
            "extra": "iterations: 38033\ncpu: 0.01827667210054438 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04215123412579045,
            "unit": "ms/iter",
            "extra": "iterations: 16615\ncpu: 0.04211542100511615 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3682601685985035,
            "unit": "ms/iter",
            "extra": "iterations: 1898\ncpu: 0.36825892781875436 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.8155007148593523,
            "unit": "ms/iter",
            "extra": "iterations: 249\ncpu: 2.815469289156609 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.68286251724231,
            "unit": "ms/iter",
            "extra": "iterations: 29\ncpu: 24.683033344827706 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 282.55633300000227,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 282.5338159999973 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0015661284029981934,
            "unit": "ms/iter",
            "extra": "iterations: 447069\ncpu: 0.0015660414902397527 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.003114759582697348,
            "unit": "ms/iter",
            "extra": "iterations: 230624\ncpu: 0.0031139305709726563 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006352150209830011,
            "unit": "ms/iter",
            "extra": "iterations: 111757\ncpu: 0.0063521122524763385 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011558123089075053,
            "unit": "ms/iter",
            "extra": "iterations: 60769\ncpu: 0.011558009478517227 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.12547068753373464,
            "unit": "ms/iter",
            "extra": "iterations: 5559\ncpu: 0.12546767116387977 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0644586980272872,
            "unit": "ms/iter",
            "extra": "iterations: 659\ncpu: 1.06445428376329 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.0025329011688279713,
            "unit": "ms/iter",
            "extra": "iterations: 278313\ncpu: 0.002532849902807234 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.007044817764104052,
            "unit": "ms/iter",
            "extra": "iterations: 99459\ncpu: 0.007044536009813066 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.016759215541540726,
            "unit": "ms/iter",
            "extra": "iterations: 41862\ncpu: 0.016759140318188284 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.03839146952386341,
            "unit": "ms/iter",
            "extra": "iterations: 18293\ncpu: 0.03838931935713121 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.3844406750684743,
            "unit": "ms/iter",
            "extra": "iterations: 1825\ncpu: 0.3844121156164373 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.1687143828828988,
            "unit": "ms/iter",
            "extra": "iterations: 222\ncpu: 3.168621963963935 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005524815887387171,
            "unit": "ms/iter",
            "extra": "iterations: 1242772\ncpu: 0.0005524710968705538 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.001702951519755056,
            "unit": "ms/iter",
            "extra": "iterations: 408125\ncpu: 0.001702934510260349 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0031680651009495767,
            "unit": "ms/iter",
            "extra": "iterations: 221794\ncpu: 0.0031680562323597914 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007481579157423514,
            "unit": "ms/iter",
            "extra": "iterations: 94211\ncpu: 0.0074813939773486875 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.11703692666666636,
            "unit": "ms/iter",
            "extra": "iterations: 6000\ncpu: 0.11703287649999794 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 1.6140697747126633,
            "unit": "ms/iter",
            "extra": "iterations: 435\ncpu: 1.6139564919540292 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0017095577849389177,
            "unit": "ms/iter",
            "extra": "iterations: 413966\ncpu: 0.001709486385355331 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.0032796697626804264,
            "unit": "ms/iter",
            "extra": "iterations: 213931\ncpu: 0.003279494883864438 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.00815157864250048,
            "unit": "ms/iter",
            "extra": "iterations: 85628\ncpu: 0.008151327941794814 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.018581213946201155,
            "unit": "ms/iter",
            "extra": "iterations: 37659\ncpu: 0.01858007891871811 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.3054105928509209,
            "unit": "ms/iter",
            "extra": "iterations: 2294\ncpu: 0.3054003788142964 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 4.186293970237879,
            "unit": "ms/iter",
            "extra": "iterations: 168\ncpu: 4.186066410714316 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005209438127800541,
            "unit": "ms/iter",
            "extra": "iterations: 1337208\ncpu: 0.0005209176777285194 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.001195910866285719,
            "unit": "ms/iter",
            "extra": "iterations: 583999\ncpu: 0.001195901534077977 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0035212669024422202,
            "unit": "ms/iter",
            "extra": "iterations: 198211\ncpu: 0.0035212584316713508 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.007652999090500948,
            "unit": "ms/iter",
            "extra": "iterations: 91259\ncpu: 0.007653032073549043 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.17449263155203074,
            "unit": "ms/iter",
            "extra": "iterations: 3911\ncpu: 0.17448666070058985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 775.3379689999633,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 775.3151480000042 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011212491096869225,
            "unit": "ms/iter",
            "extra": "iterations: 556265\ncpu: 0.001121232362273389 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.003976480869001216,
            "unit": "ms/iter",
            "extra": "iterations: 178872\ncpu: 0.003976336777136756 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.014531744493574868,
            "unit": "ms/iter",
            "extra": "iterations: 48171\ncpu: 0.01453069907205571 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.044253249479329425,
            "unit": "ms/iter",
            "extra": "iterations: 15845\ncpu: 0.044251839318396186 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.3598971394891957,
            "unit": "ms/iter",
            "extra": "iterations: 509\ncpu: 1.359812408644399 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 138.08079324999767,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 138.06553699999924 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 71.46468024731654,
            "unit": "ns/iter",
            "extra": "iterations: 9836493\ncpu: 71.46294548270455 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 376.33611895641553,
            "unit": "ns/iter",
            "extra": "iterations: 1860984\ncpu: 376.31469695602203 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 11140.610833733585,
            "unit": "ns/iter",
            "extra": "iterations: 62490\ncpu: 11140.453256520985 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 138815.5003969743,
            "unit": "ns/iter",
            "extra": "iterations: 5038\ncpu: 138812.26379515821 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 55.95445574100718,
            "unit": "ns/iter",
            "extra": "iterations: 12505967\ncpu: 55.95159302755241 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 149.76759837259212,
            "unit": "ns/iter",
            "extra": "iterations: 4708603\ncpu: 149.7602127425064 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2221.1636909823987,
            "unit": "ns/iter",
            "extra": "iterations: 311581\ncpu: 2221.031102666747 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 38443.90888119886,
            "unit": "ns/iter",
            "extra": "iterations: 18207\ncpu: 38443.314988740545 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 79.90630825823703,
            "unit": "ns/iter",
            "extra": "iterations: 8843170\ncpu: 79.90142946477397 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 384.5331424745178,
            "unit": "ns/iter",
            "extra": "iterations: 1810064\ncpu: 384.5256709155031 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10737.355032679614,
            "unit": "ns/iter",
            "extra": "iterations: 65025\ncpu: 10736.54380622833 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 131829.93320825257,
            "unit": "ns/iter",
            "extra": "iterations: 5330\ncpu: 131824.90243902282 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 92.11436653962221,
            "unit": "ns/iter",
            "extra": "iterations: 7424829\ncpu: 92.10771790165039 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 218.23573797781327,
            "unit": "ns/iter",
            "extra": "iterations: 3219880\ncpu: 218.22723300247392 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3587.246654290315,
            "unit": "ns/iter",
            "extra": "iterations: 196072\ncpu: 3586.997047003254 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 45379.09622616884,
            "unit": "ns/iter",
            "extra": "iterations: 15422\ncpu: 45379.27266243157 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.0026605249684459624,
            "unit": "ms/iter",
            "extra": "iterations: 263833\ncpu: 0.00266051545485213 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.06420989242772088,
            "unit": "ms/iter",
            "extra": "iterations: 10895\ncpu: 0.06420821725562097 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.002371166916905198,
            "unit": "ms/iter",
            "extra": "iterations: 294386\ncpu: 0.0023710816343168063 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07040774507401887,
            "unit": "ms/iter",
            "extra": "iterations: 9795\ncpu: 0.07040678570699348 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.0023822270352198646,
            "unit": "ms/iter",
            "extra": "iterations: 294919\ncpu: 0.002382199271664415 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5343074636572088,
            "unit": "ms/iter",
            "extra": "iterations: 1307\ncpu: 0.5343057329762746 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 39.44887450000086,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.448626055554804 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6252.879835000044,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6252.73292899999 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.0054997334016312505,
            "unit": "ms/iter",
            "extra": "iterations: 126895\ncpu: 0.005499588336813974 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6011221752577035,
            "unit": "ms/iter",
            "extra": "iterations: 1164\ncpu: 0.6011139243986345 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 39.73088449999788,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.72753044444409 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6300.146875999985,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6299.907566999991 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005485153040174884,
            "unit": "ms/iter",
            "extra": "iterations: 128463\ncpu: 0.005484349252313959 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.6225639484902288,
            "unit": "ms/iter",
            "extra": "iterations: 1126\ncpu: 0.6225502584369617 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 63.18754572727027,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 63.18515445454409 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 23607.69099700002,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23581.64217699999 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.013174274813999722,
            "unit": "ms/iter",
            "extra": "iterations: 53629\ncpu: 0.013171341307874508 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7287196577868966,
            "unit": "ms/iter",
            "extra": "iterations: 976\ncpu: 0.7286936413934563 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 50.02738749999886,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 50.026379285715244 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 17566.488428000015,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 17502.40125800002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.00753904961706059,
            "unit": "ms/iter",
            "extra": "iterations: 93879\ncpu: 0.007538651540813001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 2.1406533669724763,
            "unit": "ms/iter",
            "extra": "iterations: 327\ncpu: 2.1403979969418647 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 191.90594849999343,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 191.88054674999933 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 23250.706854999975,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23247.789057999995 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.013043250590665145,
            "unit": "ms/iter",
            "extra": "iterations: 53753\ncpu: 0.013042234070656591 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.7439472835821077,
            "unit": "ms/iter",
            "extra": "iterations: 402\ncpu: 1.7439210248756238 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 127.58077899999307,
            "unit": "ms/iter",
            "extra": "iterations: 5\ncpu: 127.58052259999886 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 13490.494138000031,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 13490.163874000018 ms\nthreads: 1"
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
          "id": "bc898ae042824b9c899a361a0ed7229735ab26cd",
          "message": "Benchmarks: Bugfix",
          "timestamp": "2025-01-24T20:34:42+01:00",
          "tree_id": "bbece46e372df13e93de81b4e198e8fbab92e0f8",
          "url": "https://github.com/attcs/Octree/commit/bc898ae042824b9c899a361a0ed7229735ab26cd"
        },
        "date": 1737747679972,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1786.7608896961478,
            "unit": "ns/iter",
            "extra": "iterations: 391999\ncpu: 1786.7062186383127 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 2188.8134572298363,
            "unit": "ns/iter",
            "extra": "iterations: 320913\ncpu: 2188.7181260964812 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.0002731699008670964,
            "unit": "ms/iter",
            "extra": "iterations: 2563024\ncpu: 0.00027314966305426713 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003390651502135477,
            "unit": "ms/iter",
            "extra": "iterations: 2066225\ncpu: 0.000339052441529843 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0011431643632477053,
            "unit": "ms/iter",
            "extra": "iterations: 615679\ncpu: 0.0011431034808723377 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0017963122531065065,
            "unit": "ms/iter",
            "extra": "iterations: 389591\ncpu: 0.001796218085120035 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.029544256875711727,
            "unit": "ms/iter",
            "extra": "iterations: 23743\ncpu: 0.029542360148254232 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.7969982107061483,
            "unit": "ms/iter",
            "extra": "iterations: 878\ncpu: 0.7969405683371301 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 9.988564323529491,
            "unit": "ms/iter",
            "extra": "iterations: 68\ncpu: 9.988007573529428 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 69.63315133333481,
            "unit": "ms/iter",
            "extra": "iterations: 9\ncpu: 69.62593666666668 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0016704393693197663,
            "unit": "ms/iter",
            "extra": "iterations: 419103\ncpu: 0.0016695123561511129 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.0024853203631233153,
            "unit": "ms/iter",
            "extra": "iterations: 282108\ncpu: 0.002484008319508841 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.004630633570115238,
            "unit": "ms/iter",
            "extra": "iterations: 151194\ncpu: 0.004629151242774182 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.00618020598554118,
            "unit": "ms/iter",
            "extra": "iterations: 112872\ncpu: 0.006178213826281092 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.046345224985120184,
            "unit": "ms/iter",
            "extra": "iterations: 15121\ncpu: 0.04632911401362345 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6199601858406973,
            "unit": "ms/iter",
            "extra": "iterations: 1130\ncpu: 0.6198689123893808 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.663877942857187,
            "unit": "ms/iter",
            "extra": "iterations: 105\ncpu: 6.663767323809535 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 35.995563349999316,
            "unit": "ms/iter",
            "extra": "iterations: 20\ncpu: 35.987068150000034 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.0033259790766822475,
            "unit": "ms/iter",
            "extra": "iterations: 209527\ncpu: 0.0033258890644165214 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006483757058321683,
            "unit": "ms/iter",
            "extra": "iterations: 107816\ncpu: 0.006483445499740288 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.01699367271831503,
            "unit": "ms/iter",
            "extra": "iterations: 40595\ncpu: 0.016993536864145762 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.04183671630824386,
            "unit": "ms/iter",
            "extra": "iterations: 16740\ncpu: 0.04183558488649951 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.40099161146131623,
            "unit": "ms/iter",
            "extra": "iterations: 1745\ncpu: 0.4009872280802292 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.891327508264408,
            "unit": "ms/iter",
            "extra": "iterations: 242\ncpu: 2.8912543388429737 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.0004555993806471146,
            "unit": "ms/iter",
            "extra": "iterations: 1537734\ncpu: 0.0004555939232663135 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0016080399837810383,
            "unit": "ms/iter",
            "extra": "iterations: 436527\ncpu: 0.0016080216321098064 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0028212778312505806,
            "unit": "ms/iter",
            "extra": "iterations: 248309\ncpu: 0.002821253768490067 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.00951849348527527,
            "unit": "ms/iter",
            "extra": "iterations: 73449\ncpu: 0.009518010837451824 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14352147201797494,
            "unit": "ms/iter",
            "extra": "iterations: 4896\ncpu: 0.1431468088235299 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.6732035894988242,
            "unit": "ms/iter",
            "extra": "iterations: 419\ncpu: 1.6731753174224357 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.0011539862208029372,
            "unit": "ms/iter",
            "extra": "iterations: 607510\ncpu: 0.0011539715675462105 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.0028265715355983894,
            "unit": "ms/iter",
            "extra": "iterations: 252273\ncpu: 0.002826491198027527 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.00541351971753562,
            "unit": "ms/iter",
            "extra": "iterations: 129149\ncpu: 0.0054133183609629335 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.018274041874531095,
            "unit": "ms/iter",
            "extra": "iterations: 38687\ncpu: 0.01817971605448864 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.432063441012981,
            "unit": "ms/iter",
            "extra": "iterations: 1619\ncpu: 0.4320448678196424 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 5.7924918943089825,
            "unit": "ms/iter",
            "extra": "iterations: 123\ncpu: 5.791987292682924 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004960940287461487,
            "unit": "ms/iter",
            "extra": "iterations: 1412015\ncpu: 0.0004960769198627478 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0007933969142689477,
            "unit": "ms/iter",
            "extra": "iterations: 887310\ncpu: 0.0007933731311492034 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.0022760574314938343,
            "unit": "ms/iter",
            "extra": "iterations: 309534\ncpu: 0.0022758709511717526 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004657760115453213,
            "unit": "ms/iter",
            "extra": "iterations: 150364\ncpu: 0.004657604200473517 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06143874684004716,
            "unit": "ms/iter",
            "extra": "iterations: 10997\ncpu: 0.06143474984086599 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 0.990799118571439,
            "unit": "ms/iter",
            "extra": "iterations: 700\ncpu: 0.9907581014285783 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.716238834277455,
            "unit": "ns/iter",
            "extra": "iterations: 39557002\ncpu: 17.715059371789497 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 30.990432814152985,
            "unit": "ns/iter",
            "extra": "iterations: 22612292\ncpu: 30.98950951102175 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 45.145018597745604,
            "unit": "ns/iter",
            "extra": "iterations: 15469078\ncpu: 45.1424530925505 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 89.42802350246421,
            "unit": "ns/iter",
            "extra": "iterations: 7819094\ncpu: 89.42539058873017 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 245.4830367819874,
            "unit": "ns/iter",
            "extra": "iterations: 2798054\ncpu: 245.47366741313758 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2031.1926324994638,
            "unit": "ns/iter",
            "extra": "iterations: 333926\ncpu: 2031.1032114899822 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.009741702975484974,
            "unit": "ms/iter",
            "extra": "iterations: 72526\ncpu: 0.00974109553815191 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.2148220117284,
            "unit": "ms/iter",
            "extra": "iterations: 3240\ncpu: 0.21480637314814754 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015069071486695702,
            "unit": "ms/iter",
            "extra": "iterations: 463289\ncpu: 0.0015068127302828229 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.024733086814116942,
            "unit": "ms/iter",
            "extra": "iterations: 27173\ncpu: 0.024732556876311117 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0003952514058664236,
            "unit": "ms/iter",
            "extra": "iterations: 1778085\ncpu: 0.00039524860566283185 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005257662241149781,
            "unit": "ms/iter",
            "extra": "iterations: 1338902\ncpu: 0.000525753504737462 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0017236298070174947,
            "unit": "ms/iter",
            "extra": "iterations: 406099\ncpu: 0.0017236155026237538 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.0033764600044670933,
            "unit": "ms/iter",
            "extra": "iterations: 205948\ncpu: 0.003376407224153666 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03714215274450405,
            "unit": "ms/iter",
            "extra": "iterations: 18783\ncpu: 0.037141599797689336 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0087419373177866,
            "unit": "ms/iter",
            "extra": "iterations: 686\ncpu: 1.0087345903790144 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.885327676056317,
            "unit": "ms/iter",
            "extra": "iterations: 71\ncpu: 9.885150183098595 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 100.71285985713985,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 100.70938400000021 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0019052016567440988,
            "unit": "ms/iter",
            "extra": "iterations: 367347\ncpu: 0.0019017735547044023 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.0027978316857761956,
            "unit": "ms/iter",
            "extra": "iterations: 250573\ncpu: 0.0027920882297773454 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005498929418442551,
            "unit": "ms/iter",
            "extra": "iterations: 128603\ncpu: 0.00549852855687654 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.008904826040527556,
            "unit": "ms/iter",
            "extra": "iterations: 78662\ncpu: 0.008900701393302934 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.04873953358725833,
            "unit": "ms/iter",
            "extra": "iterations: 14440\ncpu: 0.04865398054016609 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.4790925099656178,
            "unit": "ms/iter",
            "extra": "iterations: 1455\ncpu: 0.4790723134020625 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.098209847953255,
            "unit": "ms/iter",
            "extra": "iterations: 171\ncpu: 4.098120421052667 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 42.2629460000006,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 42.16376970588238 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007841395987989296,
            "unit": "ms/iter",
            "extra": "iterations: 892171\ncpu: 0.0007841238888060729 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.0022187410481383806,
            "unit": "ms/iter",
            "extra": "iterations: 316275\ncpu: 0.002218570547782802 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.005181558505698174,
            "unit": "ms/iter",
            "extra": "iterations: 135662\ncpu: 0.005180973876251274 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.014773943228468717,
            "unit": "ms/iter",
            "extra": "iterations: 47013\ncpu: 0.014773669431859286 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23799259078589916,
            "unit": "ms/iter",
            "extra": "iterations: 2952\ncpu: 0.23799060806232997 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 2.9916344487179893,
            "unit": "ms/iter",
            "extra": "iterations: 234\ncpu: 2.9915414444443966 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.08674341666521,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.085890875000093 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 302.9614049999907,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 302.9555485000017 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.004018435296616795,
            "unit": "ms/iter",
            "extra": "iterations: 174164\ncpu: 0.004017912875221073 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009165831655334378,
            "unit": "ms/iter",
            "extra": "iterations: 76480\ncpu: 0.009165588206066938 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.01858207146852849,
            "unit": "ms/iter",
            "extra": "iterations: 37541\ncpu: 0.018579361950933355 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04251264373747918,
            "unit": "ms/iter",
            "extra": "iterations: 16471\ncpu: 0.04249204711310824 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3706453946117326,
            "unit": "ms/iter",
            "extra": "iterations: 1893\ncpu: 0.3704827839408343 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.8114397349397047,
            "unit": "ms/iter",
            "extra": "iterations: 249\ncpu: 2.8112515783132825 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.656564206895446,
            "unit": "ms/iter",
            "extra": "iterations: 29\ncpu: 24.6558842413794 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 280.440495666672,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 279.4908603333302 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0015547823909520082,
            "unit": "ms/iter",
            "extra": "iterations: 449871\ncpu: 0.0015547389585014192 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.003009147497344535,
            "unit": "ms/iter",
            "extra": "iterations: 232533\ncpu: 0.0030091348926819216 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006225609632978613,
            "unit": "ms/iter",
            "extra": "iterations: 111928\ncpu: 0.006225567766778687 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011490623895985218,
            "unit": "ms/iter",
            "extra": "iterations: 60914\ncpu: 0.011490554716485545 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.12584811111111288,
            "unit": "ms/iter",
            "extra": "iterations: 5562\ncpu: 0.12584841639697972 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0690492914109981,
            "unit": "ms/iter",
            "extra": "iterations: 652\ncpu: 1.0690086365030862 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.002536381595357492,
            "unit": "ms/iter",
            "extra": "iterations: 277430\ncpu: 0.0025363876545434925 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.007013546144374637,
            "unit": "ms/iter",
            "extra": "iterations: 99893\ncpu: 0.007013416185318293 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.016716203977973538,
            "unit": "ms/iter",
            "extra": "iterations: 42132\ncpu: 0.01671549572771288 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.0383000193770867,
            "unit": "ms/iter",
            "extra": "iterations: 18269\ncpu: 0.03829867261481239 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.386788699889866,
            "unit": "ms/iter",
            "extra": "iterations: 1816\ncpu: 0.3867840622246714 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.1496594439461725,
            "unit": "ms/iter",
            "extra": "iterations: 223\ncpu: 3.149632457399117 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005593461583810348,
            "unit": "ms/iter",
            "extra": "iterations: 1280671\ncpu: 0.0005593386630914601 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017147339503773157,
            "unit": "ms/iter",
            "extra": "iterations: 365398\ncpu: 0.0017147026009994766 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.003131735938305434,
            "unit": "ms/iter",
            "extra": "iterations: 223035\ncpu: 0.0031317123186943743 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.0074330394587426175,
            "unit": "ms/iter",
            "extra": "iterations: 94225\ncpu: 0.00743299025736264 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.11693624916443156,
            "unit": "ms/iter",
            "extra": "iterations: 5984\ncpu: 0.11693396925133902 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 1.6107934367816032,
            "unit": "ms/iter",
            "extra": "iterations: 435\ncpu: 1.6107810298850622 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0017047761840147965,
            "unit": "ms/iter",
            "extra": "iterations: 414184\ncpu: 0.001704706345488961 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.003299093121319114,
            "unit": "ms/iter",
            "extra": "iterations: 212250\ncpu: 0.0032989792131919715 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.00833391455632753,
            "unit": "ms/iter",
            "extra": "iterations: 84781\ncpu: 0.00833381630318119 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.018831204189327728,
            "unit": "ms/iter",
            "extra": "iterations: 37142\ncpu: 0.018830975391739717 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.30732713022222113,
            "unit": "ms/iter",
            "extra": "iterations: 2250\ncpu: 0.3073214764444477 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 4.194476425149657,
            "unit": "ms/iter",
            "extra": "iterations: 167\ncpu: 4.194481934131808 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005217960722511636,
            "unit": "ms/iter",
            "extra": "iterations: 1343314\ncpu: 0.0005217869411023787 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0011924296213580766,
            "unit": "ms/iter",
            "extra": "iterations: 586570\ncpu: 0.0011924123327138937 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0035161961260918783,
            "unit": "ms/iter",
            "extra": "iterations: 197475\ncpu: 0.003516072996581863 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.007687557081979349,
            "unit": "ms/iter",
            "extra": "iterations: 87059\ncpu: 0.007687294903456388 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.17500817722493237,
            "unit": "ms/iter",
            "extra": "iterations: 3899\ncpu: 0.17500119594768002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 779.4084910000265,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 779.3947480000014 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0012552282481329907,
            "unit": "ms/iter",
            "extra": "iterations: 599098\ncpu: 0.0012551720269471979 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.004460799645935212,
            "unit": "ms/iter",
            "extra": "iterations: 175674\ncpu: 0.004460599604950116 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.01477991399191704,
            "unit": "ms/iter",
            "extra": "iterations: 47263\ncpu: 0.014779522861434982 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.04448411964344629,
            "unit": "ms/iter",
            "extra": "iterations: 15145\ncpu: 0.044483428458236704 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.313284543520274,
            "unit": "ms/iter",
            "extra": "iterations: 517\ncpu: 1.3132838684719654 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 136.44398324998974,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 136.44177875000096 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 70.93037943560503,
            "unit": "ns/iter",
            "extra": "iterations: 9862833\ncpu: 70.92859891270588 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 377.84868705908605,
            "unit": "ns/iter",
            "extra": "iterations: 1840715\ncpu: 377.84088737256786 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10873.85961005339,
            "unit": "ns/iter",
            "extra": "iterations: 64214\ncpu: 10873.74636372118 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 135867.60220459802,
            "unit": "ns/iter",
            "extra": "iterations: 5171\ncpu: 135860.29530071648 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 56.21478500937357,
            "unit": "ns/iter",
            "extra": "iterations: 12445403\ncpu: 56.21369496833423 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 148.65896802594122,
            "unit": "ns/iter",
            "extra": "iterations: 4671474\ncpu: 148.65639753105825 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2259.144327511993,
            "unit": "ns/iter",
            "extra": "iterations: 306920\ncpu: 2259.0066564577155 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 38000.03175124512,
            "unit": "ns/iter",
            "extra": "iterations: 18267\ncpu: 37998.62276235856 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 79.32765011438032,
            "unit": "ns/iter",
            "extra": "iterations: 8837128\ncpu: 79.32777379709813 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 387.136442337597,
            "unit": "ns/iter",
            "extra": "iterations: 1806602\ncpu: 387.1265453043892 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10722.147329988871,
            "unit": "ns/iter",
            "extra": "iterations: 65187\ncpu: 10721.728105297025 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 130478.99907011206,
            "unit": "ns/iter",
            "extra": "iterations: 5377\ncpu: 130474.98028640349 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 90.36502261466562,
            "unit": "ns/iter",
            "extra": "iterations: 7745416\ncpu: 90.36179412442222 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 216.22670513105027,
            "unit": "ns/iter",
            "extra": "iterations: 3235895\ncpu: 216.2175787533273 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3567.84289216682,
            "unit": "ns/iter",
            "extra": "iterations: 194968\ncpu: 3567.7581962167596 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 44817.868545802696,
            "unit": "ns/iter",
            "extra": "iterations: 15610\ncpu: 44816.440422806394 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.0026264277823256467,
            "unit": "ms/iter",
            "extra": "iterations: 266306\ncpu: 0.002626355876322685 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.060746840750438985,
            "unit": "ms/iter",
            "extra": "iterations: 11460\ncpu: 0.060745569458987896 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.002341703683088266,
            "unit": "ms/iter",
            "extra": "iterations: 296436\ncpu: 0.0023415947928051933 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07139924884650906,
            "unit": "ms/iter",
            "extra": "iterations: 9753\ncpu: 0.07139733394852933 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.0024621020365979207,
            "unit": "ms/iter",
            "extra": "iterations: 285427\ncpu: 0.002462032975156603 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.525997647103067,
            "unit": "ms/iter",
            "extra": "iterations: 1329\ncpu: 0.5259822957110719 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 38.65470927777892,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 38.65386705555491 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6091.0986229999935,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6090.921586999997 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.005790379181686424,
            "unit": "ms/iter",
            "extra": "iterations: 118732\ncpu: 0.005789737206481696 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6020238113695326,
            "unit": "ms/iter",
            "extra": "iterations: 1161\ncpu: 0.6019857166235932 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 39.00873338888763,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.00713061111105 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6162.8899230000225,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6161.090736000006 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005574345914561292,
            "unit": "ms/iter",
            "extra": "iterations: 127624\ncpu: 0.005573493339810598 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.6271298597122068,
            "unit": "ms/iter",
            "extra": "iterations: 1112\ncpu: 0.6270501474820153 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 63.733005181818086,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 63.72573227272679 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 24306.942924,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 24262.732459999996 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.013585110943055121,
            "unit": "ms/iter",
            "extra": "iterations: 50909\ncpu: 0.013581604431436738 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7159677981462196,
            "unit": "ms/iter",
            "extra": "iterations: 971\ncpu: 0.7159519001029633 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 49.62167742857154,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 49.62000385714305 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 16884.732518000023,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 16812.753452999998 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.007379478461443154,
            "unit": "ms/iter",
            "extra": "iterations: 96896\ncpu: 0.00737896312541303 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 2.2715911779142095,
            "unit": "ms/iter",
            "extra": "iterations: 326\ncpu: 2.2714491288343406 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 192.62880600000187,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 192.61733124999836 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 23730.884262000018,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23729.898270999995 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.012761533332124774,
            "unit": "ms/iter",
            "extra": "iterations: 55187\ncpu: 0.012760819504593478 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.7398789950372304,
            "unit": "ms/iter",
            "extra": "iterations: 403\ncpu: 1.7397806947891126 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 129.56129680000004,
            "unit": "ms/iter",
            "extra": "iterations: 5\ncpu: 129.5556260000012 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 13630.28280399999,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 13629.388926000018 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.0006807991594493088,
            "unit": "ms/iter",
            "extra": "iterations: 1034084\ncpu: 0.0006807643102494638 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.005126441941631381,
            "unit": "ms/iter",
            "extra": "iterations: 136957\ncpu: 0.005125946793518957 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.05070767147203935,
            "unit": "ms/iter",
            "extra": "iterations: 13804\ncpu: 0.05070129795711485 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.5401310355761593,
            "unit": "ms/iter",
            "extra": "iterations: 1293\ncpu: 0.5401003735498822 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.0005038237367186857,
            "unit": "ms/iter",
            "extra": "iterations: 1393217\ncpu: 0.0005037960848884196 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.0018807360173705293,
            "unit": "ms/iter",
            "extra": "iterations: 378112\ncpu: 0.0018807373873349286 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.019933091211983056,
            "unit": "ms/iter",
            "extra": "iterations: 35116\ncpu: 0.019932770389565885 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.21032990464768017,
            "unit": "ms/iter",
            "extra": "iterations: 3335\ncpu: 0.21032754452773766 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.0006385700310935235,
            "unit": "ms/iter",
            "extra": "iterations: 1102803\ncpu: 0.000638555027507171 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.005009327817416073,
            "unit": "ms/iter",
            "extra": "iterations: 139596\ncpu: 0.005009277679876163 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.049898625088691084,
            "unit": "ms/iter",
            "extra": "iterations: 14094\ncpu: 0.04989642322974408 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5243719578630621,
            "unit": "ms/iter",
            "extra": "iterations: 1329\ncpu: 0.5243664070729922 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.000443716666506068,
            "unit": "ms/iter",
            "extra": "iterations: 1556685\ncpu: 0.00044370526214358697 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.0022712488583877637,
            "unit": "ms/iter",
            "extra": "iterations: 305270\ncpu: 0.0022710934254922236 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.02266583303114742,
            "unit": "ms/iter",
            "extra": "iterations: 30886\ncpu: 0.022663897073106964 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.2279285870748461,
            "unit": "ms/iter",
            "extra": "iterations: 2940\ncpu: 0.22792647380953793 ms\nthreads: 1"
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
          "id": "ee6a0ffb49c8944df4721e3d9b363fdd9b140526",
          "message": "Add std::countl_zero() in GetDepthID()",
          "timestamp": "2025-01-25T00:56:11+01:00",
          "tree_id": "94043f65fb84d878d47c3a4a2930302122643722",
          "url": "https://github.com/attcs/Octree/commit/ee6a0ffb49c8944df4721e3d9b363fdd9b140526"
        },
        "date": 1737763671716,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1780.3102639220801,
            "unit": "ns/iter",
            "extra": "iterations: 387425\ncpu: 1780.1659985803708 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 712.6721478974656,
            "unit": "ns/iter",
            "extra": "iterations: 981653\ncpu: 712.6400876888268 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.0002899853096313961,
            "unit": "ms/iter",
            "extra": "iterations: 2413418\ncpu: 0.00028997873306654717 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.00034608665408409733,
            "unit": "ms/iter",
            "extra": "iterations: 2028733\ncpu: 0.0003460867842145813 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0012141836940145671,
            "unit": "ms/iter",
            "extra": "iterations: 574145\ncpu: 0.0012141497130515814 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0018370848147024597,
            "unit": "ms/iter",
            "extra": "iterations: 382304\ncpu: 0.0018368681546622584 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.02953822694282922,
            "unit": "ms/iter",
            "extra": "iterations: 23561\ncpu: 0.02953746479351472 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.8022818990825846,
            "unit": "ms/iter",
            "extra": "iterations: 872\ncpu: 0.8022714564220175 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 10.061939147058585,
            "unit": "ms/iter",
            "extra": "iterations: 68\ncpu: 10.061735279411767 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 67.48918810000077,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 67.48755170000003 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0017237734730164334,
            "unit": "ms/iter",
            "extra": "iterations: 406062\ncpu: 0.0017237388847023327 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.0025598269031532573,
            "unit": "ms/iter",
            "extra": "iterations: 274124\ncpu: 0.0025523822977922364 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.0047236769037138585,
            "unit": "ms/iter",
            "extra": "iterations: 148578\ncpu: 0.004722009247667886 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.006417624582504527,
            "unit": "ms/iter",
            "extra": "iterations: 109582\ncpu: 0.006417287127447934 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04782934815270138,
            "unit": "ms/iter",
            "extra": "iterations: 14643\ncpu: 0.04782766543741035 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6233919500891301,
            "unit": "ms/iter",
            "extra": "iterations: 1122\ncpu: 0.6233470775401071 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.7690532788460605,
            "unit": "ms/iter",
            "extra": "iterations: 104\ncpu: 6.769055634615384 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 33.558220666666976,
            "unit": "ms/iter",
            "extra": "iterations: 21\ncpu: 33.557197047618914 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.0031439905678545734,
            "unit": "ms/iter",
            "extra": "iterations: 223279\ncpu: 0.003143991454637471 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006114836621433695,
            "unit": "ms/iter",
            "extra": "iterations: 114960\ncpu: 0.006114735142658323 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.01592039563537146,
            "unit": "ms/iter",
            "extra": "iterations: 43990\ncpu: 0.01591999499886337 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.04019596846768961,
            "unit": "ms/iter",
            "extra": "iterations: 17379\ncpu: 0.040195610046608106 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.3951685292779981,
            "unit": "ms/iter",
            "extra": "iterations: 1759\ncpu: 0.3951688573052872 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.84582181632649,
            "unit": "ms/iter",
            "extra": "iterations: 245\ncpu: 2.845715273469392 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.0004555334927856968,
            "unit": "ms/iter",
            "extra": "iterations: 1540690\ncpu: 0.00045549937820067753 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0015844916782404054,
            "unit": "ms/iter",
            "extra": "iterations: 442094\ncpu: 0.001584434832863596 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0027971702038569564,
            "unit": "ms/iter",
            "extra": "iterations: 250764\ncpu: 0.0027970380317748973 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009214818829865875,
            "unit": "ms/iter",
            "extra": "iterations: 75752\ncpu: 0.009214595363818762 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14080610929853654,
            "unit": "ms/iter",
            "extra": "iterations: 4904\ncpu: 0.14080460256933147 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7241363333333481,
            "unit": "ms/iter",
            "extra": "iterations: 408\ncpu: 1.7240774901960747 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.0011580233050145303,
            "unit": "ms/iter",
            "extra": "iterations: 603132\ncpu: 0.00115798944012256 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.002739905845764832,
            "unit": "ms/iter",
            "extra": "iterations: 256271\ncpu: 0.0027398813170432775 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.005321791088179398,
            "unit": "ms/iter",
            "extra": "iterations: 131017\ncpu: 0.005321796919483754 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.0176645960754523,
            "unit": "ms/iter",
            "extra": "iterations: 39495\ncpu: 0.01766461541967343 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.43338603390874886,
            "unit": "ms/iter",
            "extra": "iterations: 1622\ncpu: 0.4333769845869287 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 6.046053085470037,
            "unit": "ms/iter",
            "extra": "iterations: 117\ncpu: 6.045914333333343 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004878127851782217,
            "unit": "ms/iter",
            "extra": "iterations: 1435725\ncpu: 0.00048779209597938044 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0009808114577184702,
            "unit": "ms/iter",
            "extra": "iterations: 882619\ncpu: 0.000980796688038665 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.002265423410376741,
            "unit": "ms/iter",
            "extra": "iterations: 310734\ncpu: 0.0022654012499436983 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004816253562973562,
            "unit": "ms/iter",
            "extra": "iterations: 147419\ncpu: 0.004816256466262825 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06202049092878481,
            "unit": "ms/iter",
            "extra": "iterations: 11079\ncpu: 0.0620192745735175 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.0856163750000203,
            "unit": "ms/iter",
            "extra": "iterations: 640\ncpu: 1.0855749109375035 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.71153750938381,
            "unit": "ns/iter",
            "extra": "iterations: 39444969\ncpu: 17.711364356757542 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.771906306478012,
            "unit": "ns/iter",
            "extra": "iterations: 22126311\ncpu: 31.77164489823895 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 48.15064933061058,
            "unit": "ns/iter",
            "extra": "iterations: 15424423\ncpu: 48.150691860564365 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 89.80816202157285,
            "unit": "ns/iter",
            "extra": "iterations: 7725243\ncpu: 89.80320942655183 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 281.1519649604202,
            "unit": "ns/iter",
            "extra": "iterations: 2468523\ncpu: 281.1416138314294 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2060.267842046017,
            "unit": "ns/iter",
            "extra": "iterations: 348937\ncpu: 2060.17225745621 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.009876016830945238,
            "unit": "ms/iter",
            "extra": "iterations: 71119\ncpu: 0.009876022806844829 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.2165348030163116,
            "unit": "ms/iter",
            "extra": "iterations: 3249\ncpu: 0.21652658048630327 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015263517720978182,
            "unit": "ms/iter",
            "extra": "iterations: 463462\ncpu: 0.0015262608282879651 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.02473348872660187,
            "unit": "ms/iter",
            "extra": "iterations: 28208\ncpu: 0.024733158430232553 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0003682543269825251,
            "unit": "ms/iter",
            "extra": "iterations: 1900805\ncpu: 0.00036824463266878866 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005395260789318154,
            "unit": "ms/iter",
            "extra": "iterations: 1299248\ncpu: 0.000539501169907511 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0018026199490211326,
            "unit": "ms/iter",
            "extra": "iterations: 383688\ncpu: 0.0018025348746898476 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.003353653413299473,
            "unit": "ms/iter",
            "extra": "iterations: 209255\ncpu: 0.0033535315428544258 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03595249160336997,
            "unit": "ms/iter",
            "extra": "iterations: 19353\ncpu: 0.03595015217278973 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0096353063583754,
            "unit": "ms/iter",
            "extra": "iterations: 692\ncpu: 1.0089483063583762 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.759106583333319,
            "unit": "ms/iter",
            "extra": "iterations: 72\ncpu: 9.758357527777761 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 99.92735842857314,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 99.92013728571435 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0019533347022816757,
            "unit": "ms/iter",
            "extra": "iterations: 358913\ncpu: 0.0019529700651689922 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.0028454808264389243,
            "unit": "ms/iter",
            "extra": "iterations: 245729\ncpu: 0.002843243740055121 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005532042602328283,
            "unit": "ms/iter",
            "extra": "iterations: 126871\ncpu: 0.005528579565070039 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.008806317928136938,
            "unit": "ms/iter",
            "extra": "iterations: 79735\ncpu: 0.00879491296168558 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.04806613283345946,
            "unit": "ms/iter",
            "extra": "iterations: 14537\ncpu: 0.04806462048565705 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.4820714038727605,
            "unit": "ms/iter",
            "extra": "iterations: 1446\ncpu: 0.48153815352697427 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.046723537572127,
            "unit": "ms/iter",
            "extra": "iterations: 173\ncpu: 4.0442932543352414 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 41.21526205882313,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 41.20935647058843 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007445981950335868,
            "unit": "ms/iter",
            "extra": "iterations: 917801\ncpu: 0.0007445082812069323 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.002197769210760515,
            "unit": "ms/iter",
            "extra": "iterations: 319092\ncpu: 0.0021973908872676543 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.005276139968542967,
            "unit": "ms/iter",
            "extra": "iterations: 132244\ncpu: 0.005275667183388276 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.015031587083813944,
            "unit": "ms/iter",
            "extra": "iterations: 46794\ncpu: 0.015031214621532538 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.24044554036413704,
            "unit": "ms/iter",
            "extra": "iterations: 2911\ncpu: 0.240442094125729 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 3.020706318965522,
            "unit": "ms/iter",
            "extra": "iterations: 232\ncpu: 3.0206107370689788 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.53318249999981,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.531882708333512 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 308.649486500002,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 308.63590350000436 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.004021483929537961,
            "unit": "ms/iter",
            "extra": "iterations: 171837\ncpu: 0.004016197885205155 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009265609652591716,
            "unit": "ms/iter",
            "extra": "iterations: 75876\ncpu: 0.009260023011228736 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.018675403646594264,
            "unit": "ms/iter",
            "extra": "iterations: 37679\ncpu: 0.018546998248360962 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04271770587517533,
            "unit": "ms/iter",
            "extra": "iterations: 16391\ncpu: 0.0427041425172353 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.37214093238247886,
            "unit": "ms/iter",
            "extra": "iterations: 1893\ncpu: 0.3720716793449551 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.8316564769873853,
            "unit": "ms/iter",
            "extra": "iterations: 239\ncpu: 2.831446937238516 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.82867535714206,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 24.826255071428502 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 280.0510523333249,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 279.8734943333301 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0015452738283677756,
            "unit": "ms/iter",
            "extra": "iterations: 453598\ncpu: 0.0015449250173060858 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.003005577116543623,
            "unit": "ms/iter",
            "extra": "iterations: 232797\ncpu: 0.003005057144207223 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006212112695870381,
            "unit": "ms/iter",
            "extra": "iterations: 113021\ncpu: 0.006211132072800578 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011348016336625768,
            "unit": "ms/iter",
            "extra": "iterations: 61457\ncpu: 0.011347938640024535 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.12488247754222881,
            "unit": "ms/iter",
            "extra": "iterations: 5566\ncpu: 0.12488123517786612 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0690644870228843,
            "unit": "ms/iter",
            "extra": "iterations: 655\ncpu: 1.0690249786259556 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.002574763781976131,
            "unit": "ms/iter",
            "extra": "iterations: 273328\ncpu: 0.002574764656383496 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.007092952624438028,
            "unit": "ms/iter",
            "extra": "iterations: 98764\ncpu: 0.007092894435219254 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.016681061348816594,
            "unit": "ms/iter",
            "extra": "iterations: 42022\ncpu: 0.016680620532102265 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.03853953165526783,
            "unit": "ms/iter",
            "extra": "iterations: 18275\ncpu: 0.03853922779753824 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.383003039538703,
            "unit": "ms/iter",
            "extra": "iterations: 1821\ncpu: 0.3829994854475546 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.1716145772727278,
            "unit": "ms/iter",
            "extra": "iterations: 220\ncpu: 3.1715840227272967 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005620654691349466,
            "unit": "ms/iter",
            "extra": "iterations: 1245228\ncpu: 0.0005620586262114262 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017300587392017888,
            "unit": "ms/iter",
            "extra": "iterations: 403887\ncpu: 0.0017300315632837755 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0031536136970870265,
            "unit": "ms/iter",
            "extra": "iterations: 221945\ncpu: 0.0031536130527833484 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007380010524320121,
            "unit": "ms/iter",
            "extra": "iterations: 94923\ncpu: 0.00737990906313537 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.11708506239545143,
            "unit": "ms/iter",
            "extra": "iterations: 5978\ncpu: 0.11708510889929677 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 1.6138464953704206,
            "unit": "ms/iter",
            "extra": "iterations: 432\ncpu: 1.6138228912037007 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.001654150856651452,
            "unit": "ms/iter",
            "extra": "iterations: 423276\ncpu: 0.00165412128256739 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.003238263200950001,
            "unit": "ms/iter",
            "extra": "iterations: 216367\ncpu: 0.003238217297462154 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008088856685845719,
            "unit": "ms/iter",
            "extra": "iterations: 86900\ncpu: 0.00808879298043733 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.01831760006801919,
            "unit": "ms/iter",
            "extra": "iterations: 38224\ncpu: 0.018317601847007103 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.303245921585516,
            "unit": "ms/iter",
            "extra": "iterations: 2321\ncpu: 0.30324304954761166 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 4.160353577381061,
            "unit": "ms/iter",
            "extra": "iterations: 168\ncpu: 4.160363380952349 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005170442048461785,
            "unit": "ms/iter",
            "extra": "iterations: 1355417\ncpu: 0.0005170411954402223 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0011919574010217239,
            "unit": "ms/iter",
            "extra": "iterations: 587831\ncpu: 0.0011919464063650871 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0035572615985303485,
            "unit": "ms/iter",
            "extra": "iterations: 195930\ncpu: 0.0035572029704486275 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.007652521493823584,
            "unit": "ms/iter",
            "extra": "iterations: 90747\ncpu: 0.007652359493977884 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.16888807729338803,
            "unit": "ms/iter",
            "extra": "iterations: 3739\ncpu: 0.16887904921101862 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 780.2623849999577,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 780.2235780000046 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011472851651394417,
            "unit": "ms/iter",
            "extra": "iterations: 612119\ncpu: 0.0011472259070540172 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.003958797534116561,
            "unit": "ms/iter",
            "extra": "iterations: 176894\ncpu: 0.003958530645471253 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.014741159799354971,
            "unit": "ms/iter",
            "extra": "iterations: 47447\ncpu: 0.014740214639492582 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.043909259179807765,
            "unit": "ms/iter",
            "extra": "iterations: 15850\ncpu: 0.04390641135646722 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.329518982986738,
            "unit": "ms/iter",
            "extra": "iterations: 529\ncpu: 1.3294465085066411 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 139.2014937499937,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 139.19260324999883 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 71.00284325625913,
            "unit": "ns/iter",
            "extra": "iterations: 9853491\ncpu: 70.99757821872525 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 356.32774531425605,
            "unit": "ns/iter",
            "extra": "iterations: 1951079\ncpu: 356.30692093964757 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10932.576836893026,
            "unit": "ns/iter",
            "extra": "iterations: 63817\ncpu: 10931.709920554094 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 137496.0219237535,
            "unit": "ns/iter",
            "extra": "iterations: 5063\ncpu: 137489.24471657243 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 58.51079663697369,
            "unit": "ns/iter",
            "extra": "iterations: 12493520\ncpu: 58.508738450012736 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 149.9035276683717,
            "unit": "ns/iter",
            "extra": "iterations: 4691314\ncpu: 149.89579337473504 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2121.7408736934303,
            "unit": "ns/iter",
            "extra": "iterations: 325652\ncpu: 2121.6944437620477 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 38528.736929094775,
            "unit": "ns/iter",
            "extra": "iterations: 18151\ncpu: 38527.40780122326 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 78.34388807984072,
            "unit": "ns/iter",
            "extra": "iterations: 9010638\ncpu: 78.34247497236012 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 377.1484754452042,
            "unit": "ns/iter",
            "extra": "iterations: 1853000\ncpu: 377.14174150026423 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10843.020162817025,
            "unit": "ns/iter",
            "extra": "iterations: 63384\ncpu: 10842.822983718139 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 131738.9445387684,
            "unit": "ns/iter",
            "extra": "iterations: 5301\ncpu: 131736.12224108775 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 88.74726982401627,
            "unit": "ns/iter",
            "extra": "iterations: 8036112\ncpu: 88.74026780612348 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 216.42260842323913,
            "unit": "ns/iter",
            "extra": "iterations: 3230442\ncpu: 216.40745136424448 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3579.7034283933767,
            "unit": "ns/iter",
            "extra": "iterations: 192510\ncpu: 3579.7038751234168 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 45318.87465307262,
            "unit": "ns/iter",
            "extra": "iterations: 15493\ncpu: 45318.8313431883 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.002637920688712498,
            "unit": "ms/iter",
            "extra": "iterations: 267049\ncpu: 0.0026378534014357444 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.06366210573992406,
            "unit": "ms/iter",
            "extra": "iterations: 11812\ncpu: 0.06366151710125426 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.002369586209330304,
            "unit": "ms/iter",
            "extra": "iterations: 297549\ncpu: 0.002369519820264901 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07062495082627625,
            "unit": "ms/iter",
            "extra": "iterations: 9924\ncpu: 0.07062315840386799 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.002489619540502233,
            "unit": "ms/iter",
            "extra": "iterations: 282787\ncpu: 0.0024895955896133168 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.529470655538711,
            "unit": "ms/iter",
            "extra": "iterations: 1318\ncpu: 0.5294520887708677 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 39.16116566666814,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.160438222222325 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6354.510782000034,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6354.3063660000025 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.00565250570864873,
            "unit": "ms/iter",
            "extra": "iterations: 123672\ncpu: 0.005652156179248446 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6105752053959702,
            "unit": "ms/iter",
            "extra": "iterations: 1149\ncpu: 0.6105593507397559 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 39.5845553888863,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.58286894444364 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6395.183716999953,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6394.94842900001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005512381227476668,
            "unit": "ms/iter",
            "extra": "iterations: 127921\ncpu: 0.0055123770295730104 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.633719249094199,
            "unit": "ms/iter",
            "extra": "iterations: 1104\ncpu: 0.6336080307970956 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 63.166855454550266,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 63.15904672727222 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 23011.255242000003,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 22979.867759 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.013262053895279066,
            "unit": "ms/iter",
            "extra": "iterations: 52769\ncpu: 0.013261620061020978 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7094890884146698,
            "unit": "ms/iter",
            "extra": "iterations: 984\ncpu: 0.7094715823170684 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 49.494044428570305,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 49.49351007142866 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 16836.595173000034,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 16765.951793 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.007540280920804362,
            "unit": "ms/iter",
            "extra": "iterations: 92745\ncpu: 0.007539793110140728 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 2.322544834375151,
            "unit": "ms/iter",
            "extra": "iterations: 320\ncpu: 2.322482184374941 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 196.38168100000541,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 196.3729830000034 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 23874.438823000048,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23873.474932999983 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.013173934970743836,
            "unit": "ms/iter",
            "extra": "iterations: 57082\ncpu: 0.013173176553028706 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.738751087281771,
            "unit": "ms/iter",
            "extra": "iterations: 401\ncpu: 1.7386749800499062 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 128.32367439999643,
            "unit": "ms/iter",
            "extra": "iterations: 5\ncpu: 128.32178259999978 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 13608.489501999997,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 13607.742059000004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.0006349042011290736,
            "unit": "ms/iter",
            "extra": "iterations: 1106036\ncpu: 0.0006348807697036965 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.003937371850847232,
            "unit": "ms/iter",
            "extra": "iterations: 178421\ncpu: 0.003937314677083843 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.03704435326753776,
            "unit": "ms/iter",
            "extra": "iterations: 18745\ncpu: 0.03704329677247218 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.3914430201117111,
            "unit": "ms/iter",
            "extra": "iterations: 1790\ncpu: 0.391437282681565 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.0004355567351169366,
            "unit": "ms/iter",
            "extra": "iterations: 1605399\ncpu: 0.00043553617761067263 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.0011446629560672903,
            "unit": "ms/iter",
            "extra": "iterations: 612300\ncpu: 0.001144652190102885 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.007267235254949651,
            "unit": "ms/iter",
            "extra": "iterations: 96117\ncpu: 0.007266978151627675 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.06733685533141165,
            "unit": "ms/iter",
            "extra": "iterations: 10410\ncpu: 0.06733587444764713 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.000634451594372818,
            "unit": "ms/iter",
            "extra": "iterations: 1101938\ncpu: 0.0006344448952663285 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.004981092482690519,
            "unit": "ms/iter",
            "extra": "iterations: 141540\ncpu: 0.004980796432104039 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.049561552015787144,
            "unit": "ms/iter",
            "extra": "iterations: 14188\ncpu: 0.04956021807160927 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5250258551157497,
            "unit": "ms/iter",
            "extra": "iterations: 1339\ncpu: 0.5249559148618739 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.0004448277245915024,
            "unit": "ms/iter",
            "extra": "iterations: 1516264\ncpu: 0.00044481312159358493 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.0023138662161540473,
            "unit": "ms/iter",
            "extra": "iterations: 304394\ncpu: 0.0023138246713140832 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.022688729724481365,
            "unit": "ms/iter",
            "extra": "iterations: 30887\ncpu: 0.02268787687376523 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.22894170293159535,
            "unit": "ms/iter",
            "extra": "iterations: 3070\ncpu: 0.22893767687295435 ms\nthreads: 1"
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
          "id": "ec633472f38e0ba164b8287d774e4d4ddd96cad5",
          "message": "CollisionDetection() (between different trees) performance improvements",
          "timestamp": "2025-01-25T11:42:59+01:00",
          "tree_id": "082629af99e11f8e98f7cc80a4e06b0e22f5bf5e",
          "url": "https://github.com/attcs/Octree/commit/ec633472f38e0ba164b8287d774e4d4ddd96cad5"
        },
        "date": 1737802342540,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1882.942937324622,
            "unit": "ns/iter",
            "extra": "iterations: 370943\ncpu: 1882.9025537616292 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 719.3344686054547,
            "unit": "ns/iter",
            "extra": "iterations: 974803\ncpu: 719.2195130708459 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.0002754399389137607,
            "unit": "ms/iter",
            "extra": "iterations: 2532813\ncpu: 0.0002754322454914755 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003369257103459415,
            "unit": "ms/iter",
            "extra": "iterations: 2075538\ncpu: 0.00033690878220490303 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0011392796210460095,
            "unit": "ms/iter",
            "extra": "iterations: 614217\ncpu: 0.0011391586214644016 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.001777041649633269,
            "unit": "ms/iter",
            "extra": "iterations: 393833\ncpu: 0.001776903573342 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.02969181753494353,
            "unit": "ms/iter",
            "extra": "iterations: 23610\ncpu: 0.02969048415925456 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.8062771701149252,
            "unit": "ms/iter",
            "extra": "iterations: 870\ncpu: 0.8062294367816095 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 10.053459434782392,
            "unit": "ms/iter",
            "extra": "iterations: 69\ncpu: 10.053370999999983 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 69.30292989999884,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 69.29957849999991 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0017228493238337997,
            "unit": "ms/iter",
            "extra": "iterations: 406335\ncpu: 0.0017221407164039542 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.0025694098813837094,
            "unit": "ms/iter",
            "extra": "iterations: 272391\ncpu: 0.0025687733368576797 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.004715180171450933,
            "unit": "ms/iter",
            "extra": "iterations: 148614\ncpu: 0.004715042223478274 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.006248017666961099,
            "unit": "ms/iter",
            "extra": "iterations: 112017\ncpu: 0.006247401465848925 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04586562677802432,
            "unit": "ms/iter",
            "extra": "iterations: 15326\ncpu: 0.045852448062116614 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6160423664323509,
            "unit": "ms/iter",
            "extra": "iterations: 1138\ncpu: 0.6159625852372579 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.537564726415294,
            "unit": "ms/iter",
            "extra": "iterations: 106\ncpu: 6.537297716981125 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 32.728456857142874,
            "unit": "ms/iter",
            "extra": "iterations: 21\ncpu: 32.72729419047619 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.003192068257232211,
            "unit": "ms/iter",
            "extra": "iterations: 220299\ncpu: 0.0031920187699444806 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006187825790192305,
            "unit": "ms/iter",
            "extra": "iterations: 114276\ncpu: 0.006187715828345404 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.015993133449990714,
            "unit": "ms/iter",
            "extra": "iterations: 43432\ncpu: 0.0159926260821514 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.04045179065873817,
            "unit": "ms/iter",
            "extra": "iterations: 17321\ncpu: 0.040450568789330905 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.4043503569780845,
            "unit": "ms/iter",
            "extra": "iterations: 1734\ncpu: 0.40433789677047377 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.865460520491756,
            "unit": "ms/iter",
            "extra": "iterations: 244\ncpu: 2.8653688155737655 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.000462313998128912,
            "unit": "ms/iter",
            "extra": "iterations: 1516767\ncpu: 0.00046230768799690355 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0015886699101083795,
            "unit": "ms/iter",
            "extra": "iterations: 441198\ncpu: 0.0015886402635551344 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0028397081112273392,
            "unit": "ms/iter",
            "extra": "iterations: 247817\ncpu: 0.0028396469653010127 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009367135062062255,
            "unit": "ms/iter",
            "extra": "iterations: 73233\ncpu: 0.009366952111752869 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.144358287334981,
            "unit": "ms/iter",
            "extra": "iterations: 4848\ncpu: 0.14435204187293685 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7247353027295318,
            "unit": "ms/iter",
            "extra": "iterations: 403\ncpu: 1.7247227741935462 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.001154312949610599,
            "unit": "ms/iter",
            "extra": "iterations: 605941\ncpu: 0.0011542714026613185 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.0027229104975930504,
            "unit": "ms/iter",
            "extra": "iterations: 254887\ncpu: 0.002722921141525462 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.005332807028841209,
            "unit": "ms/iter",
            "extra": "iterations: 131999\ncpu: 0.005332640073030877 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.01780826368259391,
            "unit": "ms/iter",
            "extra": "iterations: 39722\ncpu: 0.017796717083731928 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.43205403209876775,
            "unit": "ms/iter",
            "extra": "iterations: 1620\ncpu: 0.43204682160493835 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 5.911495168067331,
            "unit": "ms/iter",
            "extra": "iterations: 119\ncpu: 5.9113211176470335 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004979793487819723,
            "unit": "ms/iter",
            "extra": "iterations: 1407762\ncpu: 0.0004979709148279337 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0007994363292482102,
            "unit": "ms/iter",
            "extra": "iterations: 870808\ncpu: 0.0007994155140972479 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.0022336874714920588,
            "unit": "ms/iter",
            "extra": "iterations: 306932\ncpu: 0.0022336715461405165 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004780766317971181,
            "unit": "ms/iter",
            "extra": "iterations: 142818\ncpu: 0.004780720861516082 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06292377661143256,
            "unit": "ms/iter",
            "extra": "iterations: 11527\ncpu: 0.06292200026025803 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.0370116358209214,
            "unit": "ms/iter",
            "extra": "iterations: 670\ncpu: 1.0369815388059713 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.705343361344152,
            "unit": "ns/iter",
            "extra": "iterations: 39574883\ncpu: 17.704481577368043 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.07628399278783,
            "unit": "ns/iter",
            "extra": "iterations: 22635758\ncpu: 31.076025066180666 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 45.37836048555863,
            "unit": "ns/iter",
            "extra": "iterations: 15420651\ncpu: 45.37845321834999 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 89.57843459439835,
            "unit": "ns/iter",
            "extra": "iterations: 7765604\ncpu: 89.57791435154277 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 286.9182504956667,
            "unit": "ns/iter",
            "extra": "iterations: 2469911\ncpu: 286.9128976712114 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2086.7896058538354,
            "unit": "ns/iter",
            "extra": "iterations: 328127\ncpu: 2086.7420297628646 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.009639231221938244,
            "unit": "ms/iter",
            "extra": "iterations: 72385\ncpu: 0.009625203564274373 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.21700816067588372,
            "unit": "ms/iter",
            "extra": "iterations: 3255\ncpu: 0.21700023379416228 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015027788625949533,
            "unit": "ms/iter",
            "extra": "iterations: 463775\ncpu: 0.0015027524985176083 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.024712600268418778,
            "unit": "ms/iter",
            "extra": "iterations: 28314\ncpu: 0.02471215253937986 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0003793100516337094,
            "unit": "ms/iter",
            "extra": "iterations: 1844725\ncpu: 0.00037929531393568206 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005867442569098199,
            "unit": "ms/iter",
            "extra": "iterations: 1189647\ncpu: 0.000586723270852613 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.001942742774123709,
            "unit": "ms/iter",
            "extra": "iterations: 365492\ncpu: 0.0019427251759272489 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.003663727042681828,
            "unit": "ms/iter",
            "extra": "iterations: 190081\ncpu: 0.003663639553663964 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03941584675514557,
            "unit": "ms/iter",
            "extra": "iterations: 17782\ncpu: 0.03941510341918829 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.006902932471268,
            "unit": "ms/iter",
            "extra": "iterations: 696\ncpu: 1.0068734669540182 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.702764305555512,
            "unit": "ms/iter",
            "extra": "iterations: 72\ncpu: 9.702781583333339 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 99.15222671428329,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 99.14568800000013 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.001961650655967887,
            "unit": "ms/iter",
            "extra": "iterations: 355429\ncpu: 0.001961611950628691 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.0028565056801238184,
            "unit": "ms/iter",
            "extra": "iterations: 244801\ncpu: 0.0028564063014448306 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005471692205581501,
            "unit": "ms/iter",
            "extra": "iterations: 128066\ncpu: 0.005466238619149485 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.008583994284733299,
            "unit": "ms/iter",
            "extra": "iterations: 81536\ncpu: 0.008578017955259004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.047698634466350705,
            "unit": "ms/iter",
            "extra": "iterations: 14710\ncpu: 0.04760430183548637 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.47915624948875096,
            "unit": "ms/iter",
            "extra": "iterations: 1467\ncpu: 0.47822548466257503 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.091633736842056,
            "unit": "ms/iter",
            "extra": "iterations: 171\ncpu: 4.091593438596536 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 42.13861364705869,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 42.13686829411793 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007493687909857897,
            "unit": "ms/iter",
            "extra": "iterations: 936300\ncpu: 0.0007493615048595447 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.002196470789005166,
            "unit": "ms/iter",
            "extra": "iterations: 317894\ncpu: 0.002196230174209001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.0052155648375090706,
            "unit": "ms/iter",
            "extra": "iterations: 132192\ncpu: 0.005215371104151572 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.01481877444455386,
            "unit": "ms/iter",
            "extra": "iterations: 50635\ncpu: 0.014818197926335527 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23755827319062445,
            "unit": "ms/iter",
            "extra": "iterations: 2943\ncpu: 0.23755862113489398 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 2.9823508893615966,
            "unit": "ms/iter",
            "extra": "iterations: 235\ncpu: 2.9821613021276354 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 28.98485725000081,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 28.98383216666704 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 305.59970400000225,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 305.57675250000216 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.0040717345240794825,
            "unit": "ms/iter",
            "extra": "iterations: 171993\ncpu: 0.004071345636159631 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.0090802188515284,
            "unit": "ms/iter",
            "extra": "iterations: 76641\ncpu: 0.009070752404065793 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.01854676222883874,
            "unit": "ms/iter",
            "extra": "iterations: 37616\ncpu: 0.01852981701935343 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.042791454229854455,
            "unit": "ms/iter",
            "extra": "iterations: 16419\ncpu: 0.042675614105609265 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3695757391991608,
            "unit": "ms/iter",
            "extra": "iterations: 1898\ncpu: 0.3695708645943071 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.840263133064567,
            "unit": "ms/iter",
            "extra": "iterations: 248\ncpu: 2.8401584032258302 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 25.12246167857159,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 25.120337857143024 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 281.6159130000055,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 281.3957526666684 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0015673726781137518,
            "unit": "ms/iter",
            "extra": "iterations: 447158\ncpu: 0.0015672974474346991 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.0030906094702257236,
            "unit": "ms/iter",
            "extra": "iterations: 230702\ncpu: 0.0030905428171407435 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006209430663558529,
            "unit": "ms/iter",
            "extra": "iterations: 112394\ncpu: 0.006209396524725547 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011521294545753237,
            "unit": "ms/iter",
            "extra": "iterations: 60870\ncpu: 0.011521171923771858 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.1258765800539053,
            "unit": "ms/iter",
            "extra": "iterations: 5565\ncpu: 0.12587307475292128 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.068038314554016,
            "unit": "ms/iter",
            "extra": "iterations: 639\ncpu: 1.0680265665101651 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.002517529940032597,
            "unit": "ms/iter",
            "extra": "iterations: 274816\ncpu: 0.0025174068867897443 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.007008873817854654,
            "unit": "ms/iter",
            "extra": "iterations: 100030\ncpu: 0.007008694171748468 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.016585473227583098,
            "unit": "ms/iter",
            "extra": "iterations: 42301\ncpu: 0.016584995532020547 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.037468805755009486,
            "unit": "ms/iter",
            "extra": "iterations: 18662\ncpu: 0.03746846650948488 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.38318698213318964,
            "unit": "ms/iter",
            "extra": "iterations: 1847\ncpu: 0.3831697114239354 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.1312831524663087,
            "unit": "ms/iter",
            "extra": "iterations: 223\ncpu: 3.1312918026905545 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005592707943048527,
            "unit": "ms/iter",
            "extra": "iterations: 1252290\ncpu: 0.0005592588849228254 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017389758664422407,
            "unit": "ms/iter",
            "extra": "iterations: 404499\ncpu: 0.0017389801408656041 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0031068046698423863,
            "unit": "ms/iter",
            "extra": "iterations: 226089\ncpu: 0.003106760740239516 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.00744433809426854,
            "unit": "ms/iter",
            "extra": "iterations: 93329\ncpu: 0.007444150296263816 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.11626528155662699,
            "unit": "ms/iter",
            "extra": "iterations: 6013\ncpu: 0.11626353650424205 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 1.609752694638696,
            "unit": "ms/iter",
            "extra": "iterations: 429\ncpu: 1.6096740139860144 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0016842216071851206,
            "unit": "ms/iter",
            "extra": "iterations: 417861\ncpu: 0.0016841965294679315 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.0032991321556835335,
            "unit": "ms/iter",
            "extra": "iterations: 212174\ncpu: 0.003299022717203782 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008116869044272668,
            "unit": "ms/iter",
            "extra": "iterations: 85380\ncpu: 0.008116813762005078 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.018444487211806308,
            "unit": "ms/iter",
            "extra": "iterations: 38082\ncpu: 0.018443630507851607 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.30356980078124113,
            "unit": "ms/iter",
            "extra": "iterations: 2304\ncpu: 0.3035674496527764 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 4.277863539393958,
            "unit": "ms/iter",
            "extra": "iterations: 165\ncpu: 4.27777376363634 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005202959901614291,
            "unit": "ms/iter",
            "extra": "iterations: 1346538\ncpu: 0.000520297801473114 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.001199724105634874,
            "unit": "ms/iter",
            "extra": "iterations: 586170\ncpu: 0.0011997114318371823 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0035106440965653697,
            "unit": "ms/iter",
            "extra": "iterations: 200900\ncpu: 0.003510654151319103 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.0076811684150587005,
            "unit": "ms/iter",
            "extra": "iterations: 91322\ncpu: 0.00768111599614562 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.17459772510877833,
            "unit": "ms/iter",
            "extra": "iterations: 3907\ncpu: 0.17458773329920652 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 773.6119759999838,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 773.5918399999946 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.001162489179273026,
            "unit": "ms/iter",
            "extra": "iterations: 604257\ncpu: 0.0011624662403580098 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.004033449199421121,
            "unit": "ms/iter",
            "extra": "iterations: 174811\ncpu: 0.004033372282064598 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.015169385804668567,
            "unit": "ms/iter",
            "extra": "iterations: 46311\ncpu: 0.015168682904709432 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.04456507024493247,
            "unit": "ms/iter",
            "extra": "iterations: 15147\ncpu: 0.04456387733544596 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.344825958742655,
            "unit": "ms/iter",
            "extra": "iterations: 509\ncpu: 1.3447845893909613 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 140.2632257499974,
            "unit": "ms/iter",
            "extra": "iterations: 4\ncpu: 140.25128075000026 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 71.01085349315606,
            "unit": "ns/iter",
            "extra": "iterations: 9861157\ncpu: 71.00832843448254 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 377.11679276861975,
            "unit": "ns/iter",
            "extra": "iterations: 1854584\ncpu: 377.0982058510187 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10895.215491992192,
            "unit": "ns/iter",
            "extra": "iterations: 64188\ncpu: 10894.551193369562 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 135723.59759876036,
            "unit": "ns/iter",
            "extra": "iterations: 5164\ncpu: 135722.47192099094 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 55.32123115468458,
            "unit": "ns/iter",
            "extra": "iterations: 12628470\ncpu: 55.3204677209507 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 152.72643871216695,
            "unit": "ns/iter",
            "extra": "iterations: 4532074\ncpu: 152.721050229986 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2280.0814759968684,
            "unit": "ns/iter",
            "extra": "iterations: 305231\ncpu: 2279.984565787915 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 38535.17197627025,
            "unit": "ns/iter",
            "extra": "iterations: 18206\ncpu: 38534.87493134119 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 81.67690798988507,
            "unit": "ns/iter",
            "extra": "iterations: 8597294\ncpu: 81.6734992429017 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 416.5260418484254,
            "unit": "ns/iter",
            "extra": "iterations: 1661902\ncpu: 416.51984834244604 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10638.160684670973,
            "unit": "ns/iter",
            "extra": "iterations: 65725\ncpu: 10637.744343857119 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 129319.09485798771,
            "unit": "ns/iter",
            "extra": "iterations: 5387\ncpu: 129315.20976424852 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 91.02755994512148,
            "unit": "ns/iter",
            "extra": "iterations: 7666053\ncpu: 91.02115625863775 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 222.04076409398175,
            "unit": "ns/iter",
            "extra": "iterations: 3162857\ncpu: 222.03021097697018 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3638.909440390019,
            "unit": "ns/iter",
            "extra": "iterations: 191973\ncpu: 3638.705692988023 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 44936.40263496078,
            "unit": "ns/iter",
            "extra": "iterations: 15560\ncpu: 44934.45674807151 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.002621478137244025,
            "unit": "ms/iter",
            "extra": "iterations: 265177\ncpu: 0.0026213855801972812 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.061203769469868975,
            "unit": "ms/iter",
            "extra": "iterations: 11582\ncpu: 0.06120217855292671 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.0023529610963070042,
            "unit": "ms/iter",
            "extra": "iterations: 299843\ncpu: 0.0023528716161457337 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07080337932437959,
            "unit": "ms/iter",
            "extra": "iterations: 9828\ncpu: 0.07080078673178648 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.0023479673820086377,
            "unit": "ms/iter",
            "extra": "iterations: 299344\ncpu: 0.002347817691351728 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5348972618682911,
            "unit": "ms/iter",
            "extra": "iterations: 1306\ncpu: 0.5348720007657004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 39.59037111110951,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.58777672222248 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6269.981203999976,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6269.6937029999735 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.0054487447454697,
            "unit": "ms/iter",
            "extra": "iterations: 128413\ncpu: 0.005447887441302652 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6059119211438444,
            "unit": "ms/iter",
            "extra": "iterations: 1154\ncpu: 0.6058867738301533 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 39.9154080000004,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.91421366666733 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6320.145735999972,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6319.850149000018 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005545728779186986,
            "unit": "ms/iter",
            "extra": "iterations: 126727\ncpu: 0.005544703922605243 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.6274026753597153,
            "unit": "ms/iter",
            "extra": "iterations: 1112\ncpu: 0.6273137742805776 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 63.46434581818337,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 63.46181754545393 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 23921.610294000006,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23906.016685 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.013167039321031434,
            "unit": "ms/iter",
            "extra": "iterations: 51372\ncpu: 0.01316442669158253 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7238076422680413,
            "unit": "ms/iter",
            "extra": "iterations: 970\ncpu: 0.7237553649484317 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 50.12630757142996,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 50.122880142857134 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 17449.94707400002,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 17389.546815999976 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.005672737800629635,
            "unit": "ms/iter",
            "extra": "iterations: 124863\ncpu: 0.0056725518768569625 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 0.9696969573643046,
            "unit": "ms/iter",
            "extra": "iterations: 516\ncpu: 0.9692959515503649 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 69.19546466666462,
            "unit": "ms/iter",
            "extra": "iterations: 9\ncpu: 69.1824348888872 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 6795.042183000021,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6794.725228999994 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.022880520988712696,
            "unit": "ms/iter",
            "extra": "iterations: 30302\ncpu: 0.022879739885156087 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.4545073617463782,
            "unit": "ms/iter",
            "extra": "iterations: 481\ncpu: 1.4545156382536377 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 97.06665942857333,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 97.06538585714141 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 9110.519592000002,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 9110.066740000007 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.0006341910242147092,
            "unit": "ms/iter",
            "extra": "iterations: 1103792\ncpu: 0.0006341525640700421 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.00392529884249762,
            "unit": "ms/iter",
            "extra": "iterations: 177883\ncpu: 0.003924972538128942 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.037403584896582964,
            "unit": "ms/iter",
            "extra": "iterations: 18711\ncpu: 0.03740231195553439 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.3921797810055739,
            "unit": "ms/iter",
            "extra": "iterations: 1790\ncpu: 0.39216970558658937 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.00043777270852820953,
            "unit": "ms/iter",
            "extra": "iterations: 1571370\ncpu: 0.0004377519756645533 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.0011584789471877604,
            "unit": "ms/iter",
            "extra": "iterations: 611771\ncpu: 0.0011584821542701472 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.007291659529421481,
            "unit": "ms/iter",
            "extra": "iterations: 95882\ncpu: 0.007291326474207866 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.06898441377601254,
            "unit": "ms/iter",
            "extra": "iterations: 10090\ncpu: 0.06898304261645101 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.000639261267248821,
            "unit": "ms/iter",
            "extra": "iterations: 1095365\ncpu: 0.0006392570677354066 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.005008377379999729,
            "unit": "ms/iter",
            "extra": "iterations: 100000\ncpu: 0.005008216119999815 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.05033963317959213,
            "unit": "ms/iter",
            "extra": "iterations: 13876\ncpu: 0.05033972708273254 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5345393616858367,
            "unit": "ms/iter",
            "extra": "iterations: 1305\ncpu: 0.5345204459770247 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.00044768155723034646,
            "unit": "ms/iter",
            "extra": "iterations: 1567167\ncpu: 0.00044767416044364715 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.0022399800686387678,
            "unit": "ms/iter",
            "extra": "iterations: 313526\ncpu: 0.0022399398646363827 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.022752232095793627,
            "unit": "ms/iter",
            "extra": "iterations: 30733\ncpu: 0.022752062506101144 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.23225190365895018,
            "unit": "ms/iter",
            "extra": "iterations: 2979\ncpu: 0.23224747700570833 ms\nthreads: 1"
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
          "id": "0aea1ba643ffcb3bf3c71b04d48dbe57c9cf4a49",
          "message": "Update README and CHANGELOG",
          "timestamp": "2025-02-07T23:45:53+01:00",
          "tree_id": "ca6a1c9ea1ba8a4c1cf0ec409dfb95d2966325c7",
          "url": "https://github.com/attcs/Octree/commit/0aea1ba643ffcb3bf3c71b04d48dbe57c9cf4a49"
        },
        "date": 1738968688773,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1798.0263350557332,
            "unit": "ns/iter",
            "extra": "iterations: 376988\ncpu: 1797.9975728670408 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 727.5113704527464,
            "unit": "ns/iter",
            "extra": "iterations: 980260\ncpu: 727.4887805276152 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetNodeEntities/10000",
            "value": 689.1709659975907,
            "unit": "ns/iter",
            "extra": "iterations: 1018957\ncpu: 689.1472348685959 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000",
            "value": 854.5394623907531,
            "unit": "ns/iter",
            "extra": "iterations: 820224\ncpu: 854.3600711512956 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000",
            "value": 8129.286990488525,
            "unit": "ns/iter",
            "extra": "iterations: 86529\ncpu: 8128.688948213888 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.00028329923800691977,
            "unit": "ms/iter",
            "extra": "iterations: 2473907\ncpu: 0.000283243372123528 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003479607677749002,
            "unit": "ms/iter",
            "extra": "iterations: 2017831\ncpu: 0.000347940850348716 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.001167892017241315,
            "unit": "ms/iter",
            "extra": "iterations: 601346\ncpu: 0.0011678041925946119 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0017791062227820811,
            "unit": "ms/iter",
            "extra": "iterations: 392204\ncpu: 0.0017790719472519404 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.029677023124815006,
            "unit": "ms/iter",
            "extra": "iterations: 23611\ncpu: 0.029676638473592813 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.8003791231471015,
            "unit": "ms/iter",
            "extra": "iterations: 877\ncpu: 0.8003697423033074 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 9.976180855072352,
            "unit": "ms/iter",
            "extra": "iterations: 69\ncpu: 9.976144652173922 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 67.48892699999942,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 67.48564330000004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0016869068849113343,
            "unit": "ms/iter",
            "extra": "iterations: 413542\ncpu: 0.0016846970537454451 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.002499936216200764,
            "unit": "ms/iter",
            "extra": "iterations: 280369\ncpu: 0.0024966861172240912 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.0045931052085386,
            "unit": "ms/iter",
            "extra": "iterations: 152250\ncpu: 0.004591319356321844 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.006226551920240252,
            "unit": "ms/iter",
            "extra": "iterations: 112538\ncpu: 0.006225796415433015 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04581266727616986,
            "unit": "ms/iter",
            "extra": "iterations: 15313\ncpu: 0.0457939720498924 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6147384514435682,
            "unit": "ms/iter",
            "extra": "iterations: 1143\ncpu: 0.6146944505686798 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.555253850467261,
            "unit": "ms/iter",
            "extra": "iterations: 107\ncpu: 6.554997551401851 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 32.308038590908374,
            "unit": "ms/iter",
            "extra": "iterations: 22\ncpu: 32.147304909090934 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.003149631371737263,
            "unit": "ms/iter",
            "extra": "iterations: 222251\ncpu: 0.0031495029853634018 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006096443569507688,
            "unit": "ms/iter",
            "extra": "iterations: 114167\ncpu: 0.006096342244256226 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.016047709228477026,
            "unit": "ms/iter",
            "extra": "iterations: 43615\ncpu: 0.016047317322022248 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.04065865307776916,
            "unit": "ms/iter",
            "extra": "iterations: 17269\ncpu: 0.04065721558862695 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.3921177229994425,
            "unit": "ms/iter",
            "extra": "iterations: 1787\ncpu: 0.3920969026301054 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.8918652809915617,
            "unit": "ms/iter",
            "extra": "iterations: 242\ncpu: 2.891628499999995 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.0004357140868183125,
            "unit": "ms/iter",
            "extra": "iterations: 1608163\ncpu: 0.00043569756859224016 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0015575058800959757,
            "unit": "ms/iter",
            "extra": "iterations: 448292\ncpu: 0.0015574423656902253 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0027302213054545012,
            "unit": "ms/iter",
            "extra": "iterations: 257305\ncpu: 0.0027301105497366932 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009080386377171535,
            "unit": "ms/iter",
            "extra": "iterations: 77238\ncpu: 0.00908001894145369 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.1406962803700796,
            "unit": "ms/iter",
            "extra": "iterations: 4972\ncpu: 0.14069567679002357 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.6972104317073622,
            "unit": "ms/iter",
            "extra": "iterations: 410\ncpu: 1.6972020439024402 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.0011592379622287045,
            "unit": "ms/iter",
            "extra": "iterations: 601461\ncpu: 0.0011591897130487268 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.0027482714777172287,
            "unit": "ms/iter",
            "extra": "iterations: 254636\ncpu: 0.0027481859870560356 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.005388159036881496,
            "unit": "ms/iter",
            "extra": "iterations: 130825\ncpu: 0.005387804441047193 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.017700572417613594,
            "unit": "ms/iter",
            "extra": "iterations: 39721\ncpu: 0.0176993807557715 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.4288416320293449,
            "unit": "ms/iter",
            "extra": "iterations: 1636\ncpu: 0.42881413019559783 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 5.901220391666584,
            "unit": "ms/iter",
            "extra": "iterations: 120\ncpu: 5.901092399999955 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004575937306316137,
            "unit": "ms/iter",
            "extra": "iterations: 1536008\ncpu: 0.0004575705575752212 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0007672802372837105,
            "unit": "ms/iter",
            "extra": "iterations: 915360\ncpu: 0.000767266662296807 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.002178802785896631,
            "unit": "ms/iter",
            "extra": "iterations: 324348\ncpu: 0.002178716566773957 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004617421182560432,
            "unit": "ms/iter",
            "extra": "iterations: 150690\ncpu: 0.004617285493397033 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06541433783657248,
            "unit": "ms/iter",
            "extra": "iterations: 10659\ncpu: 0.06541065043625105 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.0847086868217723,
            "unit": "ms/iter",
            "extra": "iterations: 645\ncpu: 1.0846600775193853 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.831392376384017,
            "unit": "ns/iter",
            "extra": "iterations: 39298828\ncpu: 17.819271709578732 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 30.773987695414927,
            "unit": "ns/iter",
            "extra": "iterations: 22827912\ncpu: 30.772929823805292 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 46.04453868389912,
            "unit": "ns/iter",
            "extra": "iterations: 15413455\ncpu: 46.04216770347694 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 87.61316147026253,
            "unit": "ns/iter",
            "extra": "iterations: 7999547\ncpu: 87.60828394407763 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 252.65209592081396,
            "unit": "ns/iter",
            "extra": "iterations: 2828041\ncpu: 252.63897164150038 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2045.0734174116733,
            "unit": "ns/iter",
            "extra": "iterations: 318671\ncpu: 2044.9746980428206 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.009597403345855666,
            "unit": "ms/iter",
            "extra": "iterations: 71073\ncpu: 0.009596683410015069 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.21475986366421868,
            "unit": "ms/iter",
            "extra": "iterations: 3264\ncpu: 0.21474633333333248 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.001503518407777434,
            "unit": "ms/iter",
            "extra": "iterations: 462875\ncpu: 0.001503490378611937 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.024304882661332426,
            "unit": "ms/iter",
            "extra": "iterations: 28993\ncpu: 0.024304464732866543 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0003765627153757234,
            "unit": "ms/iter",
            "extra": "iterations: 1864184\ncpu: 0.00037654106998021453 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005415245463300419,
            "unit": "ms/iter",
            "extra": "iterations: 1295876\ncpu: 0.0005414925347795674 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0018450609615905714,
            "unit": "ms/iter",
            "extra": "iterations: 380141\ncpu: 0.0018450379701216207 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.003520095359505351,
            "unit": "ms/iter",
            "extra": "iterations: 199246\ncpu: 0.0035199721249108973 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03860817359453962,
            "unit": "ms/iter",
            "extra": "iterations: 18019\ncpu: 0.03860685676230647 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0236783830408933,
            "unit": "ms/iter",
            "extra": "iterations: 684\ncpu: 1.023628298245614 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.957479171427915,
            "unit": "ms/iter",
            "extra": "iterations: 70\ncpu: 9.95687878571435 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 102.23952814285602,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 102.23183642857134 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0019500994894188774,
            "unit": "ms/iter",
            "extra": "iterations: 358611\ncpu: 0.0019454491273273836 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.002865513495089406,
            "unit": "ms/iter",
            "extra": "iterations: 244978\ncpu: 0.0028647126680763097 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005494660213686241,
            "unit": "ms/iter",
            "extra": "iterations: 127383\ncpu: 0.005490392430701123 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.008665250505501564,
            "unit": "ms/iter",
            "extra": "iterations: 80613\ncpu: 0.008661084589334187 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.048143592829617526,
            "unit": "ms/iter",
            "extra": "iterations: 14532\ncpu: 0.04812874793559003 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.48277225205482277,
            "unit": "ms/iter",
            "extra": "iterations: 1460\ncpu: 0.48272448972602394 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.086865508772039,
            "unit": "ms/iter",
            "extra": "iterations: 171\ncpu: 4.086550614035092 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 42.425657062498345,
            "unit": "ms/iter",
            "extra": "iterations: 16\ncpu: 42.414170250000716 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007645473042852398,
            "unit": "ms/iter",
            "extra": "iterations: 916640\ncpu: 0.0007645154586315215 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.0022083069070482305,
            "unit": "ms/iter",
            "extra": "iterations: 318660\ncpu: 0.002208114855959337 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.0052467173060513775,
            "unit": "ms/iter",
            "extra": "iterations: 133618\ncpu: 0.0052460681345327526 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.015067471443584396,
            "unit": "ms/iter",
            "extra": "iterations: 46627\ncpu: 0.0150650789242285 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23852458863558748,
            "unit": "ms/iter",
            "extra": "iterations: 2939\ncpu: 0.23849405137801785 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 3.01601231168842,
            "unit": "ms/iter",
            "extra": "iterations: 231\ncpu: 3.015560186147212 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.362485291668367,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.35915133333289 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 307.4376975000064,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 307.40169850000143 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.0041054388310811695,
            "unit": "ms/iter",
            "extra": "iterations: 170585\ncpu: 0.004104840120760935 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009262219059831213,
            "unit": "ms/iter",
            "extra": "iterations: 75646\ncpu: 0.009233900629246748 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.01847528609379506,
            "unit": "ms/iter",
            "extra": "iterations: 38019\ncpu: 0.01843316323417241 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04279622164475585,
            "unit": "ms/iter",
            "extra": "iterations: 16355\ncpu: 0.04277285509018619 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3715685926712686,
            "unit": "ms/iter",
            "extra": "iterations: 1883\ncpu: 0.371339671800316 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.846833740890543,
            "unit": "ms/iter",
            "extra": "iterations: 247\ncpu: 2.846455732793514 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 25.098834214284288,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 25.09762642857183 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 282.1957736666718,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 282.17530266666796 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0016922811158639486,
            "unit": "ms/iter",
            "extra": "iterations: 407003\ncpu: 0.0016921751633280132 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.003176965122353512,
            "unit": "ms/iter",
            "extra": "iterations: 220141\ncpu: 0.003176779332336976 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006258794966374004,
            "unit": "ms/iter",
            "extra": "iterations: 111967\ncpu: 0.006257881643698585 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.0113158191256391,
            "unit": "ms/iter",
            "extra": "iterations: 61645\ncpu: 0.011314002400843428 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.12481983253161918,
            "unit": "ms/iter",
            "extra": "iterations: 5613\ncpu: 0.12480191537502149 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0612968636363265,
            "unit": "ms/iter",
            "extra": "iterations: 660\ncpu: 1.0611575954545638 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.002739920281259378,
            "unit": "ms/iter",
            "extra": "iterations: 256276\ncpu: 0.0027395598534392576 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.0071533958012891695,
            "unit": "ms/iter",
            "extra": "iterations: 98173\ncpu: 0.0071530985199595365 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.016585186692186903,
            "unit": "ms/iter",
            "extra": "iterations: 42321\ncpu: 0.01658393251577245 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.03774299795070977,
            "unit": "ms/iter",
            "extra": "iterations: 18543\ncpu: 0.03774174788329892 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.38106510506259694,
            "unit": "ms/iter",
            "extra": "iterations: 1837\ncpu: 0.38103816712030464 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.13208901339276,
            "unit": "ms/iter",
            "extra": "iterations: 224\ncpu: 3.131988388392872 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005584370913416815,
            "unit": "ms/iter",
            "extra": "iterations: 1254104\ncpu: 0.0005584023900729197 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.001730605978708546,
            "unit": "ms/iter",
            "extra": "iterations: 406643\ncpu: 0.0017305320834245389 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0030763385450788103,
            "unit": "ms/iter",
            "extra": "iterations: 227435\ncpu: 0.0030760962384857107 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007173860134039201,
            "unit": "ms/iter",
            "extra": "iterations: 93704\ncpu: 0.0071735677665841595 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.06369914367392047,
            "unit": "ms/iter",
            "extra": "iterations: 10844\ncpu: 0.06369756962375497 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 0.7343458079748194,
            "unit": "ms/iter",
            "extra": "iterations: 953\ncpu: 0.7343330094438756 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0016690583528447898,
            "unit": "ms/iter",
            "extra": "iterations: 419414\ncpu: 0.0016690504322697624 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.0033400759350368826,
            "unit": "ms/iter",
            "extra": "iterations: 209350\ncpu: 0.003340009362311958 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008956913754799598,
            "unit": "ms/iter",
            "extra": "iterations: 77871\ncpu: 0.008956870516623576 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.019319365924785933,
            "unit": "ms/iter",
            "extra": "iterations: 36349\ncpu: 0.01931861374453204 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.23734425426620373,
            "unit": "ms/iter",
            "extra": "iterations: 2930\ncpu: 0.23733497235494896 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 2.191971993769474,
            "unit": "ms/iter",
            "extra": "iterations: 321\ncpu: 2.1918696947040672 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005419218537007422,
            "unit": "ms/iter",
            "extra": "iterations: 1294060\ncpu: 0.0005418977589910848 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0012468614142632432,
            "unit": "ms/iter",
            "extra": "iterations: 563615\ncpu: 0.0012468556319473326 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.003675443744792787,
            "unit": "ms/iter",
            "extra": "iterations: 187236\ncpu: 0.003672409750261687 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.007960385517311432,
            "unit": "ms/iter",
            "extra": "iterations: 88699\ncpu: 0.007960215030609128 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.18330159098060347,
            "unit": "ms/iter",
            "extra": "iterations: 3814\ncpu: 0.18329562087047888 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 11.08064679365072,
            "unit": "ms/iter",
            "extra": "iterations: 63\ncpu: 11.080077571428486 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011950106476436135,
            "unit": "ms/iter",
            "extra": "iterations: 605580\ncpu: 0.0011949294065193563 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.003988130297058871,
            "unit": "ms/iter",
            "extra": "iterations: 177103\ncpu: 0.003988004370338153 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.05689734365408255,
            "unit": "ms/iter",
            "extra": "iterations: 18571\ncpu: 0.056893296214527864 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.0943226108000033,
            "unit": "ms/iter",
            "extra": "iterations: 10000\ncpu: 0.09431645239999967 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.9050591500000669,
            "unit": "ms/iter",
            "extra": "iterations: 440\ncpu: 1.9049632704545354 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 80.42997022222507,
            "unit": "ms/iter",
            "extra": "iterations: 9\ncpu: 80.42800955555644 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 78.45419933590185,
            "unit": "ns/iter",
            "extra": "iterations: 8875832\ncpu: 78.44972798043129 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 377.13786201762355,
            "unit": "ns/iter",
            "extra": "iterations: 1844627\ncpu: 377.12485450988646 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10756.413679078581,
            "unit": "ns/iter",
            "extra": "iterations: 65019\ncpu: 10755.683892400672 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 134477.50645099094,
            "unit": "ns/iter",
            "extra": "iterations: 5193\ncpu: 134471.71596379782 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 66.7219430113034,
            "unit": "ns/iter",
            "extra": "iterations: 10470778\ncpu: 66.71610867883996 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 154.5733682865016,
            "unit": "ns/iter",
            "extra": "iterations: 4541315\ncpu: 154.56505681724337 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2248.1098270389457,
            "unit": "ns/iter",
            "extra": "iterations: 308740\ncpu: 2247.9299896353036 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 38138.151967509955,
            "unit": "ns/iter",
            "extra": "iterations: 18221\ncpu: 38135.35717029756 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 85.67941649046483,
            "unit": "ns/iter",
            "extra": "iterations: 8202231\ncpu: 85.67008841862642 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 414.40244251037,
            "unit": "ns/iter",
            "extra": "iterations: 1668693\ncpu: 414.38900205130074 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10878.530396448692,
            "unit": "ns/iter",
            "extra": "iterations: 64876\ncpu: 10877.415377026877 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 130588.80924963734,
            "unit": "ns/iter",
            "extra": "iterations: 5384\ncpu: 130583.74461367165 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 99.68498887966705,
            "unit": "ns/iter",
            "extra": "iterations: 7040707\ncpu: 99.67833770103051 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 218.43173578831394,
            "unit": "ns/iter",
            "extra": "iterations: 3193379\ncpu: 218.4309197248424 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3633.96191326206,
            "unit": "ns/iter",
            "extra": "iterations: 191589\ncpu: 3633.7869606292356 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 45176.60212492206,
            "unit": "ns/iter",
            "extra": "iterations: 15530\ncpu: 45174.73451384501 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.0026303909168986406,
            "unit": "ms/iter",
            "extra": "iterations: 263148\ncpu: 0.0026303375894933707 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.062162019203234914,
            "unit": "ms/iter",
            "extra": "iterations: 11873\ncpu: 0.06216092941969112 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.00235725944031959,
            "unit": "ms/iter",
            "extra": "iterations: 294561\ncpu: 0.0023571720526477605 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07070635582636914,
            "unit": "ms/iter",
            "extra": "iterations: 9929\ncpu: 0.0707033992345646 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.0023970253829440897,
            "unit": "ms/iter",
            "extra": "iterations: 293189\ncpu: 0.0023969688903745386 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5187742553823353,
            "unit": "ms/iter",
            "extra": "iterations: 1347\ncpu: 0.5187515211581434 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 38.543349944442674,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 38.5418206111107 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6225.699695999992,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6225.3195680000035 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.005634383366256831,
            "unit": "ms/iter",
            "extra": "iterations: 124025\ncpu: 0.005634167675871771 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6141654508771928,
            "unit": "ms/iter",
            "extra": "iterations: 1140\ncpu: 0.6141613973684313 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 40.21926447058925,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 40.2183383529408 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6365.163273999997,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6364.88970900001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005977659078487777,
            "unit": "ms/iter",
            "extra": "iterations: 120758\ncpu: 0.005977460921843843 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.6296573176895528,
            "unit": "ms/iter",
            "extra": "iterations: 1108\ncpu: 0.6296303384476462 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 64.04790363636307,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 64.04698818182004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 23072.517593999975,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 23057.783076999982 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.014255945105308688,
            "unit": "ms/iter",
            "extra": "iterations: 49094\ncpu: 0.014255376420743426 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7259318259957629,
            "unit": "ms/iter",
            "extra": "iterations: 954\ncpu: 0.725879975890982 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 51.812608928571535,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 51.81048414285832 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 16713.923709000028,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 16658.202012999995 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.005539189692107844,
            "unit": "ms/iter",
            "extra": "iterations: 127902\ncpu: 0.005538599044581154 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 0.9806715344827973,
            "unit": "ms/iter",
            "extra": "iterations: 522\ncpu: 0.9804456417624487 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 70.09076900000082,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 70.08359819999725 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 6833.144247000007,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6832.625979999989 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.023317714690849485,
            "unit": "ms/iter",
            "extra": "iterations: 30325\ncpu: 0.02331412636438544 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.452383493775892,
            "unit": "ms/iter",
            "extra": "iterations: 482\ncpu: 1.4522674626556078 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 96.90287528571518,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 96.8981814285712 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 8968.033390000017,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 8967.39417500001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.00063335265595237,
            "unit": "ms/iter",
            "extra": "iterations: 1106251\ncpu: 0.0006333081723768002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.004008380445982494,
            "unit": "ms/iter",
            "extra": "iterations: 175657\ncpu: 0.004007764051532358 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.03699100863112368,
            "unit": "ms/iter",
            "extra": "iterations: 19001\ncpu: 0.036989773854008104 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.3912891773835809,
            "unit": "ms/iter",
            "extra": "iterations: 1804\ncpu: 0.3912563896895783 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.00044359494252961534,
            "unit": "ms/iter",
            "extra": "iterations: 1578813\ncpu: 0.0004435787746870642 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.0011493698715858932,
            "unit": "ms/iter",
            "extra": "iterations: 610447\ncpu: 0.001149318194699935 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.0072962995871624385,
            "unit": "ms/iter",
            "extra": "iterations: 96406\ncpu: 0.007295955677032542 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.06837307843329857,
            "unit": "ms/iter",
            "extra": "iterations: 10187\ncpu: 0.06836803936389482 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.0006365512924994673,
            "unit": "ms/iter",
            "extra": "iterations: 1040542\ncpu: 0.0006365130066830445 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.0050331289502047565,
            "unit": "ms/iter",
            "extra": "iterations: 139170\ncpu: 0.005032788467342158 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.050337775802505313,
            "unit": "ms/iter",
            "extra": "iterations: 13894\ncpu: 0.05033472527709992 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5347571498098922,
            "unit": "ms/iter",
            "extra": "iterations: 1315\ncpu: 0.5347333931558794 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.0004500101169288328,
            "unit": "ms/iter",
            "extra": "iterations: 1557785\ncpu: 0.0004499939523104908 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.0023315349928074965,
            "unit": "ms/iter",
            "extra": "iterations: 306563\ncpu: 0.002331486617758796 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.02269413597268392,
            "unit": "ms/iter",
            "extra": "iterations: 31043\ncpu: 0.022692706439455073 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.23030506438895534,
            "unit": "ms/iter",
            "extra": "iterations: 3044\ncpu: 0.2303007575558418 ms\nthreads: 1"
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
          "id": "86c27af977dbd40afd36e156ee4ae2baf84d28fe",
          "message": "Merge branch 'pagedvector'",
          "timestamp": "2025-02-08T00:08:03+01:00",
          "tree_id": "454916c0659723761d2ffcc82c7260f657b5e71a",
          "url": "https://github.com/attcs/Octree/commit/86c27af977dbd40afd36e156ee4ae2baf84d28fe"
        },
        "date": 1738970029149,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1853.496995754461,
            "unit": "ns/iter",
            "extra": "iterations: 389116\ncpu: 1853.4490023540538 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 719.4084974520396,
            "unit": "ns/iter",
            "extra": "iterations: 975869\ncpu: 719.3952651431699 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetNodeEntities/10000",
            "value": 3810.0058449234125,
            "unit": "ns/iter",
            "extra": "iterations: 185289\ncpu: 3809.8653077084973 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000",
            "value": 856.9251374988071,
            "unit": "ns/iter",
            "extra": "iterations: 817098\ncpu: 856.8945267275164 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000",
            "value": 8122.083083417913,
            "unit": "ns/iter",
            "extra": "iterations: 86696\ncpu: 8122.004959859744 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.0003631566057417513,
            "unit": "ms/iter",
            "extra": "iterations: 1918263\ncpu: 0.00036313987028890204 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.00043476602463133045,
            "unit": "ms/iter",
            "extra": "iterations: 1609413\ncpu: 0.00043475823980544475 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0012456742905935603,
            "unit": "ms/iter",
            "extra": "iterations: 524664\ncpu: 0.0012455878447920955 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0018322180246087354,
            "unit": "ms/iter",
            "extra": "iterations: 382466\ncpu: 0.001832130456040535 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.0239330761403984,
            "unit": "ms/iter",
            "extra": "iterations: 29288\ncpu: 0.023931775368751764 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.7611317967391321,
            "unit": "ms/iter",
            "extra": "iterations: 920\ncpu: 0.7610824869565214 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 9.564367788732318,
            "unit": "ms/iter",
            "extra": "iterations: 71\ncpu: 9.56387921126761 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 65.11885820000032,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 65.11497720000001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0018609523039226253,
            "unit": "ms/iter",
            "extra": "iterations: 377096\ncpu: 0.0018591784054988682 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.00264392640385764,
            "unit": "ms/iter",
            "extra": "iterations: 263791\ncpu: 0.002642362017657921 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.00476828307742675,
            "unit": "ms/iter",
            "extra": "iterations: 146603\ncpu: 0.004766072433715545 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.006285232232107031,
            "unit": "ms/iter",
            "extra": "iterations: 111845\ncpu: 0.006269122061781924 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.039513078256952744,
            "unit": "ms/iter",
            "extra": "iterations: 17762\ncpu: 0.03939402893818261 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.5837713062913793,
            "unit": "ms/iter",
            "extra": "iterations: 1208\ncpu: 0.5837471812913889 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.330740567567607,
            "unit": "ms/iter",
            "extra": "iterations: 111\ncpu: 6.330090396396411 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 31.367301500000362,
            "unit": "ms/iter",
            "extra": "iterations: 22\ncpu: 31.338645590909085 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.004049789807996911,
            "unit": "ms/iter",
            "extra": "iterations: 173018\ncpu: 0.0040496323619507646 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.008163789639553267,
            "unit": "ms/iter",
            "extra": "iterations: 83452\ncpu: 0.00816371526146768 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.020081995548692107,
            "unit": "ms/iter",
            "extra": "iterations: 33024\ncpu: 0.020081783309108536 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.047813674218644006,
            "unit": "ms/iter",
            "extra": "iterations: 14654\ncpu: 0.047812784290978794 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.4069955617391302,
            "unit": "ms/iter",
            "extra": "iterations: 1725\ncpu: 0.4069912185507255 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 3.048189484978567,
            "unit": "ms/iter",
            "extra": "iterations: 233\ncpu: 3.0479194635193205 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.0006514048501064109,
            "unit": "ms/iter",
            "extra": "iterations: 1058451\ncpu: 0.0006513484478733553 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0018084758971417952,
            "unit": "ms/iter",
            "extra": "iterations: 387174\ncpu: 0.0018083874201263502 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0031556724316705725,
            "unit": "ms/iter",
            "extra": "iterations: 222781\ncpu: 0.0031555905350994858 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.00944758317628689,
            "unit": "ms/iter",
            "extra": "iterations: 74288\ncpu: 0.009447272547383192 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14524288279147096,
            "unit": "ms/iter",
            "extra": "iterations: 4829\ncpu: 0.14524219610685482 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 3.4187252780488353,
            "unit": "ms/iter",
            "extra": "iterations: 205\ncpu: 3.4186087365853615 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.0013747254201808734,
            "unit": "ms/iter",
            "extra": "iterations: 508293\ncpu: 0.0013747183612601427 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.0030110352797320943,
            "unit": "ms/iter",
            "extra": "iterations: 232258\ncpu: 0.003010809698697131 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.005692967709639199,
            "unit": "ms/iter",
            "extra": "iterations: 122854\ncpu: 0.005692693750305215 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.0188439618801486,
            "unit": "ms/iter",
            "extra": "iterations: 37146\ncpu: 0.01884320002153672 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.45285955749354123,
            "unit": "ms/iter",
            "extra": "iterations: 1548\ncpu: 0.4528374173126599 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 7.959418344827667,
            "unit": "ms/iter",
            "extra": "iterations: 87\ncpu: 7.959182000000045 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0006748374770254116,
            "unit": "ms/iter",
            "extra": "iterations: 1038647\ncpu: 0.0006747866358830278 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0010646062917936045,
            "unit": "ms/iter",
            "extra": "iterations: 655902\ncpu: 0.0010644858744141602 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.0030278196923317372,
            "unit": "ms/iter",
            "extra": "iterations: 231288\ncpu: 0.0030276043979800037 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.006707238635819469,
            "unit": "ms/iter",
            "extra": "iterations: 104385\ncpu: 0.006706795018441307 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.41606675882706123,
            "unit": "ms/iter",
            "extra": "iterations: 1671\ncpu: 0.4160167779772599 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.4864789372549079,
            "unit": "ms/iter",
            "extra": "iterations: 510\ncpu: 1.4863801450980354 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 18.716160383759888,
            "unit": "ns/iter",
            "extra": "iterations: 37545971\ncpu: 18.715475516667333 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.443691675258908,
            "unit": "ns/iter",
            "extra": "iterations: 21958627\ncpu: 31.443471716150864 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 45.16916200189928,
            "unit": "ns/iter",
            "extra": "iterations: 15542935\ncpu: 45.16885298690379 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 93.8746590596114,
            "unit": "ns/iter",
            "extra": "iterations: 7464648\ncpu: 93.76278653728939 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 329.2019233802602,
            "unit": "ns/iter",
            "extra": "iterations: 2155476\ncpu: 329.1875766651989 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2238.579061207948,
            "unit": "ns/iter",
            "extra": "iterations: 308375\ncpu: 2238.565610052692 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.009479694986693458,
            "unit": "ms/iter",
            "extra": "iterations: 74023\ncpu: 0.009479382192021473 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.21580923069824576,
            "unit": "ms/iter",
            "extra": "iterations: 3251\ncpu: 0.21580796985542985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015899590084210813,
            "unit": "ms/iter",
            "extra": "iterations: 439017\ncpu: 0.0015898905600466463 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.026228448871782585,
            "unit": "ms/iter",
            "extra": "iterations: 26502\ncpu: 0.026227978454456215 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0004941966459100296,
            "unit": "ms/iter",
            "extra": "iterations: 1416897\ncpu: 0.0004941798098238633 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0006656133025286139,
            "unit": "ms/iter",
            "extra": "iterations: 1050056\ncpu: 0.0006655902294734752 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.001945404643288384,
            "unit": "ms/iter",
            "extra": "iterations: 360305\ncpu: 0.0019453028545260152 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.003675993777909328,
            "unit": "ms/iter",
            "extra": "iterations: 189004\ncpu: 0.0036759744185308365 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03681657504233712,
            "unit": "ms/iter",
            "extra": "iterations: 18896\ncpu: 0.03681414648602914 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0183545924308266,
            "unit": "ms/iter",
            "extra": "iterations: 687\ncpu: 1.0182813711790353 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.94641275714311,
            "unit": "ms/iter",
            "extra": "iterations: 70\ncpu: 9.945467942857126 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 103.73195771428202,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 103.72572542857178 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0020845844938180104,
            "unit": "ms/iter",
            "extra": "iterations: 335492\ncpu: 0.0020833421571900255 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.0030008330535984695,
            "unit": "ms/iter",
            "extra": "iterations: 234746\ncpu: 0.002986118979663131 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005739115953517316,
            "unit": "ms/iter",
            "extra": "iterations: 122368\ncpu: 0.005725398535564856 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.009043254991406789,
            "unit": "ms/iter",
            "extra": "iterations: 77383\ncpu: 0.009042007882868346 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.04412973870765381,
            "unit": "ms/iter",
            "extra": "iterations: 15940\ncpu: 0.043902558343788506 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.4356690012422467,
            "unit": "ms/iter",
            "extra": "iterations: 1610\ncpu: 0.43531110124223177 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 3.91959630726255,
            "unit": "ms/iter",
            "extra": "iterations: 179\ncpu: 3.9184204022346742 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 39.92315561111089,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 39.916311777777956 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0008792163233043421,
            "unit": "ms/iter",
            "extra": "iterations: 797020\ncpu: 0.0008791507854257109 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.002324802192575436,
            "unit": "ms/iter",
            "extra": "iterations: 302749\ncpu: 0.002324663761069398 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.005379227084845468,
            "unit": "ms/iter",
            "extra": "iterations: 130885\ncpu: 0.005379050380104702 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.012557416877364832,
            "unit": "ms/iter",
            "extra": "iterations: 55767\ncpu: 0.012556546810837841 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.23937622831676136,
            "unit": "ms/iter",
            "extra": "iterations: 2917\ncpu: 0.23936402605416895 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 2.966566400843902,
            "unit": "ms/iter",
            "extra": "iterations: 237\ncpu: 2.966422594936673 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 28.82881654166643,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 28.82827708333302 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 306.06006299998967,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 306.0384164999945 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.004273258847018093,
            "unit": "ms/iter",
            "extra": "iterations: 163784\ncpu: 0.004272461082889642 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009550578668704278,
            "unit": "ms/iter",
            "extra": "iterations: 73282\ncpu: 0.009538285158702102 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.019679136506775247,
            "unit": "ms/iter",
            "extra": "iterations: 35566\ncpu: 0.019669408367542256 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04186427610471512,
            "unit": "ms/iter",
            "extra": "iterations: 16769\ncpu: 0.04179554976444601 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3808637058183682,
            "unit": "ms/iter",
            "extra": "iterations: 1839\ncpu: 0.38049096030451096 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.799312652000026,
            "unit": "ms/iter",
            "extra": "iterations: 250\ncpu: 2.799169536000022 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.67427953571441,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 24.67302721428563 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 274.8874146666746,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 274.6273696666653 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0017303326952071015,
            "unit": "ms/iter",
            "extra": "iterations: 403264\ncpu: 0.0017302254701634663 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.0032954071785251285,
            "unit": "ms/iter",
            "extra": "iterations: 217482\ncpu: 0.0032953536936389757 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.00712541738865804,
            "unit": "ms/iter",
            "extra": "iterations: 98570\ncpu: 0.00712537760982051 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.013790353299814439,
            "unit": "ms/iter",
            "extra": "iterations: 50518\ncpu: 0.013789497525634457 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.13173678103965097,
            "unit": "ms/iter",
            "extra": "iterations: 5348\ncpu: 0.13173253047868205 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.12199406825391,
            "unit": "ms/iter",
            "extra": "iterations: 630\ncpu: 1.1219431682539673 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.0028608970043728665,
            "unit": "ms/iter",
            "extra": "iterations: 247661\ncpu: 0.002860865441066644 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.00824644658731509,
            "unit": "ms/iter",
            "extra": "iterations: 86955\ncpu: 0.008246208832154558 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.02194125010939599,
            "unit": "ms/iter",
            "extra": "iterations: 31994\ncpu: 0.021940867850221635 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.04565719990844742,
            "unit": "ms/iter",
            "extra": "iterations: 15292\ncpu: 0.0456551549829973 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.37577727122262095,
            "unit": "ms/iter",
            "extra": "iterations: 1873\ncpu: 0.37576664121730147 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.2039059269404437,
            "unit": "ms/iter",
            "extra": "iterations: 219\ncpu: 3.2037904703196034 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0007186553756142817,
            "unit": "ms/iter",
            "extra": "iterations: 974577\ncpu: 0.0007186521680688149 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0018764042114967421,
            "unit": "ms/iter",
            "extra": "iterations: 375306\ncpu: 0.0018763394030471277 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.003235792494552656,
            "unit": "ms/iter",
            "extra": "iterations: 216616\ncpu: 0.003235624875355443 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007282482724345207,
            "unit": "ms/iter",
            "extra": "iterations: 96581\ncpu: 0.007282055062589865 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.07603467908091277,
            "unit": "ms/iter",
            "extra": "iterations: 9183\ncpu: 0.0760343307198086 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 0.8230204894366131,
            "unit": "ms/iter",
            "extra": "iterations: 852\ncpu: 0.8230165715962386 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0018265744341403104,
            "unit": "ms/iter",
            "extra": "iterations: 381720\ncpu: 0.0018265223697998738 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.003616552372914353,
            "unit": "ms/iter",
            "extra": "iterations: 194318\ncpu: 0.003616452124867453 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.009147709740073826,
            "unit": "ms/iter",
            "extra": "iterations: 76714\ncpu: 0.009147297624944567 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.020538432001444767,
            "unit": "ms/iter",
            "extra": "iterations: 33236\ncpu: 0.020537709832711533 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.25845663113240996,
            "unit": "ms/iter",
            "extra": "iterations: 2711\ncpu: 0.25844500295094 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 2.9484723755275164,
            "unit": "ms/iter",
            "extra": "iterations: 237\ncpu: 2.948284767932482 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005436298698782872,
            "unit": "ms/iter",
            "extra": "iterations: 1290330\ncpu: 0.0005435968697930028 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0012557821944697985,
            "unit": "ms/iter",
            "extra": "iterations: 556827\ncpu: 0.0012557110089848156 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.003647939022318543,
            "unit": "ms/iter",
            "extra": "iterations: 188364\ncpu: 0.0036476721241851255 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.007920848016444645,
            "unit": "ms/iter",
            "extra": "iterations: 86108\ncpu: 0.007920742033260632 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.17980096469644435,
            "unit": "ms/iter",
            "extra": "iterations: 3739\ncpu: 0.1797953923508934 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 11.087902016129192,
            "unit": "ms/iter",
            "extra": "iterations: 62\ncpu: 11.087598338709695 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0013498306515980542,
            "unit": "ms/iter",
            "extra": "iterations: 518694\ncpu: 0.0013498078423887535 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.0049536234560897795,
            "unit": "ms/iter",
            "extra": "iterations: 141038\ncpu: 0.004953376458826749 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.05465542416588323,
            "unit": "ms/iter",
            "extra": "iterations: 17024\ncpu: 0.05465322027725608 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.1103267782517833,
            "unit": "ms/iter",
            "extra": "iterations: 11486\ncpu: 0.11032619650008754 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 2.233277520547882,
            "unit": "ms/iter",
            "extra": "iterations: 365\ncpu: 2.2331891260273786 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 87.83833800000451,
            "unit": "ms/iter",
            "extra": "iterations: 8\ncpu: 87.83358087499948 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 78.59860767604398,
            "unit": "ns/iter",
            "extra": "iterations: 8894769\ncpu: 78.5927898745887 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 418.0762710648572,
            "unit": "ns/iter",
            "extra": "iterations: 1672810\ncpu: 418.0526114741093 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10830.815555487237,
            "unit": "ns/iter",
            "extra": "iterations: 64556\ncpu: 10830.133511989488 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 135722.01998836402,
            "unit": "ns/iter",
            "extra": "iterations: 5153\ncpu: 135719.4261595173 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 63.297983933226035,
            "unit": "ns/iter",
            "extra": "iterations: 11126318\ncpu: 63.29240670633312 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 170.78562839972747,
            "unit": "ns/iter",
            "extra": "iterations: 4105110\ncpu: 170.77485110021507 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2346.6035503962144,
            "unit": "ns/iter",
            "extra": "iterations: 294784\ncpu: 2346.439470934661 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 39199.69378847373,
            "unit": "ns/iter",
            "extra": "iterations: 17870\ncpu: 39198.056966983764 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 86.44703731009193,
            "unit": "ns/iter",
            "extra": "iterations: 8347178\ncpu: 86.44193330967568 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 396.2324687516623,
            "unit": "ns/iter",
            "extra": "iterations: 1592485\ncpu: 396.21036116510516 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10874.922783884553,
            "unit": "ns/iter",
            "extra": "iterations: 64313\ncpu: 10873.865221650352 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 130831.8828139529,
            "unit": "ns/iter",
            "extra": "iterations: 5359\ncpu: 130828.6142937093 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 93.2944634314846,
            "unit": "ns/iter",
            "extra": "iterations: 7496340\ncpu: 93.29416595298464 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 241.95574894578542,
            "unit": "ns/iter",
            "extra": "iterations: 2868926\ncpu: 241.94746256961614 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3727.483220472374,
            "unit": "ns/iter",
            "extra": "iterations: 187103\ncpu: 3727.4286783215152 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 44584.352802547364,
            "unit": "ns/iter",
            "extra": "iterations: 15700\ncpu: 44582.75012738903 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.002803999396601272,
            "unit": "ms/iter",
            "extra": "iterations: 243620\ncpu: 0.0028039610212627067 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.06462464818322733,
            "unit": "ms/iter",
            "extra": "iterations: 11614\ncpu: 0.06462289176855597 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.0025834935613607483,
            "unit": "ms/iter",
            "extra": "iterations: 271020\ncpu: 0.0025834344882296294 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07129158882681692,
            "unit": "ms/iter",
            "extra": "iterations: 9845\ncpu: 0.07128804733367392 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.002395201247109609,
            "unit": "ms/iter",
            "extra": "iterations: 293639\ncpu: 0.002395096162294537 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5423354728682012,
            "unit": "ms/iter",
            "extra": "iterations: 1290\ncpu: 0.5422834767441891 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 40.24349447058739,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 40.240977058824214 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6290.841561999969,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6290.45352 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.005654615002034067,
            "unit": "ms/iter",
            "extra": "iterations: 130289\ncpu: 0.005653971908603218 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6184000291776911,
            "unit": "ms/iter",
            "extra": "iterations: 1131\ncpu: 0.6183091600353564 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 40.674194882352396,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 40.581444823529594 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6343.1824689999985,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6342.992460999994 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005005640649902325,
            "unit": "ms/iter",
            "extra": "iterations: 138667\ncpu: 0.0050056239696538865 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.6264074605734786,
            "unit": "ms/iter",
            "extra": "iterations: 1116\ncpu: 0.6262909560931822 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 63.99166754545666,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 63.98126509090864 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 24536.18495699999,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 24490.678043000004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.012140846221225438,
            "unit": "ms/iter",
            "extra": "iterations: 57082\ncpu: 0.012138908920500386 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7258973241590204,
            "unit": "ms/iter",
            "extra": "iterations: 981\ncpu: 0.7258518858307799 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 49.897830642858516,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 49.89370557142893 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 17380.356786999982,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 17300.65434300002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.005816675699307833,
            "unit": "ms/iter",
            "extra": "iterations: 120083\ncpu: 0.005816565550494242 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 0.9785244399440779,
            "unit": "ms/iter",
            "extra": "iterations: 716\ncpu: 0.9784685544692479 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 70.06180540000173,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 70.05991999999992 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 7126.5076430000245,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 7123.814409000005 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.02354885558183736,
            "unit": "ms/iter",
            "extra": "iterations: 30017\ncpu: 0.023545144618049907 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.4645769958070822,
            "unit": "ms/iter",
            "extra": "iterations: 477\ncpu: 1.4644804863731862 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 97.20261428571219,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 97.19995542857062 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 8985.01480899995,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 8984.650716000004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.0006412503792834958,
            "unit": "ms/iter",
            "extra": "iterations: 1092191\ncpu: 0.0006412317625763365 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.00398933899820056,
            "unit": "ms/iter",
            "extra": "iterations: 168936\ncpu: 0.003988991748354438 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.03707178557824452,
            "unit": "ms/iter",
            "extra": "iterations: 18902\ncpu: 0.0370680644376254 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.39453729735805815,
            "unit": "ms/iter",
            "extra": "iterations: 1779\ncpu: 0.3945109353569375 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.0004487025483843701,
            "unit": "ms/iter",
            "extra": "iterations: 1563108\ncpu: 0.0004486900271766319 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.00116590618147744,
            "unit": "ms/iter",
            "extra": "iterations: 599711\ncpu: 0.001165810126877769 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.007290960672692687,
            "unit": "ms/iter",
            "extra": "iterations: 95913\ncpu: 0.007290760772783709 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.06813988110212857,
            "unit": "ms/iter",
            "extra": "iterations: 10017\ncpu: 0.06813737675950882 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.0006393710062593673,
            "unit": "ms/iter",
            "extra": "iterations: 1096278\ncpu: 0.0006393221363559216 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.00500937208000039,
            "unit": "ms/iter",
            "extra": "iterations: 100000\ncpu: 0.005008839630000068 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.04965983438631058,
            "unit": "ms/iter",
            "extra": "iterations: 14087\ncpu: 0.04965518903953922 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5335525159817074,
            "unit": "ms/iter",
            "extra": "iterations: 1314\ncpu: 0.5335145258751907 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.00044843169950908933,
            "unit": "ms/iter",
            "extra": "iterations: 1563122\ncpu: 0.00044837069083538355 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.00234072609375851,
            "unit": "ms/iter",
            "extra": "iterations: 297872\ncpu: 0.0023405389093301616 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.023215044348783513,
            "unit": "ms/iter",
            "extra": "iterations: 29967\ncpu: 0.02321165785697651 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.24094096125908468,
            "unit": "ms/iter",
            "extra": "iterations: 2891\ncpu: 0.24092318713248276 ms\nthreads: 1"
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
          "id": "fc5c9e680ec7523291b6d5a9f2d6f0f32c525bdf",
          "message": "Revert \"Merge branch 'pagedvector'\"\n\nThis reverts commit 86c27af977dbd40afd36e156ee4ae2baf84d28fe, reversing\nchanges made to 0aea1ba643ffcb3bf3c71b04d48dbe57c9cf4a49.",
          "timestamp": "2025-02-08T10:05:26+01:00",
          "tree_id": "ca6a1c9ea1ba8a4c1cf0ec409dfb95d2966325c7",
          "url": "https://github.com/attcs/Octree/commit/fc5c9e680ec7523291b6d5a9f2d6f0f32c525bdf"
        },
        "date": 1739005872190,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1804.6661277739593,
            "unit": "ns/iter",
            "extra": "iterations: 389688\ncpu: 1804.5969775820656 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 721.8563536812594,
            "unit": "ns/iter",
            "extra": "iterations: 981132\ncpu: 721.8333088717932 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetNodeEntities/10000",
            "value": 690.2865906195908,
            "unit": "ns/iter",
            "extra": "iterations: 1019018\ncpu: 690.240061510199 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000",
            "value": 853.8891235570571,
            "unit": "ns/iter",
            "extra": "iterations: 819597\ncpu: 853.8334242316648 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000",
            "value": 8133.255554390469,
            "unit": "ns/iter",
            "extra": "iterations: 85833\ncpu: 8132.806205072644 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.00028734490129676653,
            "unit": "ms/iter",
            "extra": "iterations: 2437306\ncpu: 0.0002873205153558888 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003467761187512844,
            "unit": "ms/iter",
            "extra": "iterations: 2015372\ncpu: 0.00034675778069755874 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.001157097060316353,
            "unit": "ms/iter",
            "extra": "iterations: 600915\ncpu: 0.0011570422555602696 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0017681415899949418,
            "unit": "ms/iter",
            "extra": "iterations: 396278\ncpu: 0.001768088150237966 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.029489338069739354,
            "unit": "ms/iter",
            "extra": "iterations: 23717\ncpu: 0.029487949319053804 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.7980512746878663,
            "unit": "ms/iter",
            "extra": "iterations: 881\ncpu: 0.7980306821793411 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 10.013655739130726,
            "unit": "ms/iter",
            "extra": "iterations: 69\ncpu: 10.01346301449275 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 68.46612260000029,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 68.46047669999997 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0017251451086020993,
            "unit": "ms/iter",
            "extra": "iterations: 406530\ncpu: 0.0017248445231594208 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.0025472540827717488,
            "unit": "ms/iter",
            "extra": "iterations: 274997\ncpu: 0.0025460078328127214 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.004677893617305266,
            "unit": "ms/iter",
            "extra": "iterations: 149780\ncpu: 0.004677385939377759 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.006339177698146805,
            "unit": "ms/iter",
            "extra": "iterations: 110789\ncpu: 0.006338266235817631 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04618118010664349,
            "unit": "ms/iter",
            "extra": "iterations: 15191\ncpu: 0.04617078039628731 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6153844096491355,
            "unit": "ms/iter",
            "extra": "iterations: 1140\ncpu: 0.6152282473684211 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.5509766132075296,
            "unit": "ms/iter",
            "extra": "iterations: 106\ncpu: 6.55065416037734 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 34.38620523809548,
            "unit": "ms/iter",
            "extra": "iterations: 21\ncpu: 34.382620619047664 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.0031777792360545357,
            "unit": "ms/iter",
            "extra": "iterations: 220199\ncpu: 0.003177596878278287 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006081725187937398,
            "unit": "ms/iter",
            "extra": "iterations: 115730\ncpu: 0.00608157374060314 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.0160552014223971,
            "unit": "ms/iter",
            "extra": "iterations: 43729\ncpu: 0.016055234123808007 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.04032827635801029,
            "unit": "ms/iter",
            "extra": "iterations: 17452\ncpu: 0.04032721361448541 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.3949574331641247,
            "unit": "ms/iter",
            "extra": "iterations: 1773\ncpu: 0.39492644500845925 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.8976778362068614,
            "unit": "ms/iter",
            "extra": "iterations: 232\ncpu: 2.8973767370689627 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.00043896622161632436,
            "unit": "ms/iter",
            "extra": "iterations: 1590307\ncpu: 0.00043893900863166635 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0015592420413030983,
            "unit": "ms/iter",
            "extra": "iterations: 447812\ncpu: 0.0015591342549998689 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0027255707065557827,
            "unit": "ms/iter",
            "extra": "iterations: 255040\ncpu: 0.0027254049560853167 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009104072551217136,
            "unit": "ms/iter",
            "extra": "iterations: 76732\ncpu: 0.009103628994422118 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.13937196137169816,
            "unit": "ms/iter",
            "extra": "iterations: 5074\ncpu: 0.13937073039022504 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7040769661016753,
            "unit": "ms/iter",
            "extra": "iterations: 413\ncpu: 1.703965307506051 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.0011564070465904129,
            "unit": "ms/iter",
            "extra": "iterations: 606648\ncpu: 0.0011563251918740363 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.0027415100043453977,
            "unit": "ms/iter",
            "extra": "iterations: 255439\ncpu: 0.002741388264908637 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.0053635081491574,
            "unit": "ms/iter",
            "extra": "iterations: 130627\ncpu: 0.005363077120350259 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.017619631056281802,
            "unit": "ms/iter",
            "extra": "iterations: 39071\ncpu: 0.017618173248701047 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.42887090964590546,
            "unit": "ms/iter",
            "extra": "iterations: 1638\ncpu: 0.4288226288156294 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 5.876577764705921,
            "unit": "ms/iter",
            "extra": "iterations: 119\ncpu: 5.875935966386573 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004554204683624835,
            "unit": "ms/iter",
            "extra": "iterations: 1531378\ncpu: 0.0004553849454543532 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0007635898411600947,
            "unit": "ms/iter",
            "extra": "iterations: 914002\ncpu: 0.0007635465896135887 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.002167157102948978,
            "unit": "ms/iter",
            "extra": "iterations: 324316\ncpu: 0.0021670267239359085 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004608980310397547,
            "unit": "ms/iter",
            "extra": "iterations: 150130\ncpu: 0.0046087434157063905 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06489929472813953,
            "unit": "ms/iter",
            "extra": "iterations: 10888\ncpu: 0.06489451570536417 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.086142326625405,
            "unit": "ms/iter",
            "extra": "iterations: 646\ncpu: 1.086040668730655 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.848923473382417,
            "unit": "ns/iter",
            "extra": "iterations: 39211664\ncpu: 17.84789337172743 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 30.925991400172983,
            "unit": "ns/iter",
            "extra": "iterations: 22625803\ncpu: 30.925274784722664 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 45.60195913624961,
            "unit": "ns/iter",
            "extra": "iterations: 15244269\ncpu: 45.60079004116236 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 88.78775014770389,
            "unit": "ns/iter",
            "extra": "iterations: 7924618\ncpu: 88.78655173031727 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 289.05575013808453,
            "unit": "ns/iter",
            "extra": "iterations: 2413429\ncpu: 289.03485372886394 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2109.8865987377453,
            "unit": "ns/iter",
            "extra": "iterations: 322298\ncpu: 2109.6759241447403 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.009686943115220701,
            "unit": "ms/iter",
            "extra": "iterations: 70300\ncpu: 0.009686208179231916 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.2152570939514935,
            "unit": "ms/iter",
            "extra": "iterations: 3257\ncpu: 0.21525119035922652 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015322268446455582,
            "unit": "ms/iter",
            "extra": "iterations: 458706\ncpu: 0.001532230245516738 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.02473103817889186,
            "unit": "ms/iter",
            "extra": "iterations: 28576\ncpu: 0.024729511618140965 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0003688266951750911,
            "unit": "ms/iter",
            "extra": "iterations: 1859100\ncpu: 0.00036881827443386537 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005394568929028249,
            "unit": "ms/iter",
            "extra": "iterations: 1297037\ncpu: 0.0005394486510407936 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0018154009503177357,
            "unit": "ms/iter",
            "extra": "iterations: 389133\ncpu: 0.0018154316621823289 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.003504442491107229,
            "unit": "ms/iter",
            "extra": "iterations: 199317\ncpu: 0.003504277146455144 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03793775289386475,
            "unit": "ms/iter",
            "extra": "iterations: 18401\ncpu: 0.03793692092821042 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.0229158135095138,
            "unit": "ms/iter",
            "extra": "iterations: 681\ncpu: 1.0229173450807603 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.938640400000054,
            "unit": "ms/iter",
            "extra": "iterations: 70\ncpu: 9.938018700000027 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 103.14236214285951,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 103.13073171428654 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.001946151743914463,
            "unit": "ms/iter",
            "extra": "iterations: 351164\ncpu: 0.0019454347484366185 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.0029026532477781034,
            "unit": "ms/iter",
            "extra": "iterations: 240780\ncpu: 0.002901971754298526 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005527995235160407,
            "unit": "ms/iter",
            "extra": "iterations: 126552\ncpu: 0.005523475172261164 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.008811329168590877,
            "unit": "ms/iter",
            "extra": "iterations: 80117\ncpu: 0.00879901325561361 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.04785218215334236,
            "unit": "ms/iter",
            "extra": "iterations: 14647\ncpu: 0.04781433481258988 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.47560765960355855,
            "unit": "ms/iter",
            "extra": "iterations: 1463\ncpu: 0.4755935755297355 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.110848682352874,
            "unit": "ms/iter",
            "extra": "iterations: 170\ncpu: 4.098354558823553 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 42.533655882353365,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 42.53159358823582 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007542175602502077,
            "unit": "ms/iter",
            "extra": "iterations: 928005\ncpu: 0.0007542116076960703 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.002189555135968196,
            "unit": "ms/iter",
            "extra": "iterations: 319891\ncpu: 0.002189535079136322 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.005211372041259886,
            "unit": "ms/iter",
            "extra": "iterations: 133883\ncpu: 0.00521081239589789 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.014940017116790522,
            "unit": "ms/iter",
            "extra": "iterations: 46913\ncpu: 0.014939545712275931 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.2381514877384277,
            "unit": "ms/iter",
            "extra": "iterations: 2936\ncpu: 0.23814169482288958 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 3.010060806866916,
            "unit": "ms/iter",
            "extra": "iterations: 233\ncpu: 3.009909811158802 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.62475570833334,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.623212916666475 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 317.2119405000018,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 317.18080149999395 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.004101832100678692,
            "unit": "ms/iter",
            "extra": "iterations: 170364\ncpu: 0.004101047187199167 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.009134985260002365,
            "unit": "ms/iter",
            "extra": "iterations: 76730\ncpu: 0.009129598383943767 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.018356545813504543,
            "unit": "ms/iter",
            "extra": "iterations: 38242\ncpu: 0.01835149008943044 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.042220593471454355,
            "unit": "ms/iter",
            "extra": "iterations: 16604\ncpu: 0.0421638322693326 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3718651272630351,
            "unit": "ms/iter",
            "extra": "iterations: 1878\ncpu: 0.3717579259850861 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.880156491666478,
            "unit": "ms/iter",
            "extra": "iterations: 240\ncpu: 2.880085083333306 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.635210392856624,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 24.633913214285336 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 282.87399199998237,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 282.8606289999982 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.0015295518308451744,
            "unit": "ms/iter",
            "extra": "iterations: 458231\ncpu: 0.0015293012017955974 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.0029973515739457,
            "unit": "ms/iter",
            "extra": "iterations: 232759\ncpu: 0.0029970352123870722 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006111298488631617,
            "unit": "ms/iter",
            "extra": "iterations: 113341\ncpu: 0.006110433408916405 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011264066205676194,
            "unit": "ms/iter",
            "extra": "iterations: 62185\ncpu: 0.011263109367210856 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.12390456152758013,
            "unit": "ms/iter",
            "extra": "iterations: 5656\ncpu: 0.12390373178925022 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0567060512820077,
            "unit": "ms/iter",
            "extra": "iterations: 663\ncpu: 1.0566248687782716 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.002491454371115261,
            "unit": "ms/iter",
            "extra": "iterations: 281061\ncpu: 0.002491309192666375 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.006994029677820822,
            "unit": "ms/iter",
            "extra": "iterations: 99603\ncpu: 0.006993559541379235 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.016364123468029745,
            "unit": "ms/iter",
            "extra": "iterations: 42837\ncpu: 0.016363012675957873 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.0375854788474974,
            "unit": "ms/iter",
            "extra": "iterations: 18603\ncpu: 0.03758382373810662 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.37818733027027496,
            "unit": "ms/iter",
            "extra": "iterations: 1850\ncpu: 0.3781605081081116 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.11243184753361,
            "unit": "ms/iter",
            "extra": "iterations: 223\ncpu: 3.1122806950673056 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.000544842629141263,
            "unit": "ms/iter",
            "extra": "iterations: 1285956\ncpu: 0.000544801795706846 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.00173361661577628,
            "unit": "ms/iter",
            "extra": "iterations: 403809\ncpu: 0.0017335376106030246 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0030200026536606767,
            "unit": "ms/iter",
            "extra": "iterations: 232509\ncpu: 0.003019700549226042 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007098132710715043,
            "unit": "ms/iter",
            "extra": "iterations: 98937\ncpu: 0.007097666899137716 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.0649786625675444,
            "unit": "ms/iter",
            "extra": "iterations: 10734\ncpu: 0.06497676178498228 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 0.7399835474683558,
            "unit": "ms/iter",
            "extra": "iterations: 948\ncpu: 0.7399425179324988 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0016722705771163296,
            "unit": "ms/iter",
            "extra": "iterations: 417940\ncpu: 0.0016721853998181522 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.003377100104214359,
            "unit": "ms/iter",
            "extra": "iterations: 205346\ncpu: 0.003376928286891407 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008687003532785492,
            "unit": "ms/iter",
            "extra": "iterations: 81239\ncpu: 0.008686437166877904 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.01985513402802727,
            "unit": "ms/iter",
            "extra": "iterations: 36179\ncpu: 0.019853147903479737 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.23958175664621617,
            "unit": "ms/iter",
            "extra": "iterations: 2934\ncpu: 0.23956921438309414 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 2.170143688271728,
            "unit": "ms/iter",
            "extra": "iterations: 324\ncpu: 2.1699102839506503 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005420306999156651,
            "unit": "ms/iter",
            "extra": "iterations: 1292870\ncpu: 0.0005420008345773334 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0012515561018952466,
            "unit": "ms/iter",
            "extra": "iterations: 562735\ncpu: 0.0012515128470772098 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0037773948574190094,
            "unit": "ms/iter",
            "extra": "iterations: 188349\ncpu: 0.0037771750473854096 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.008055245270978725,
            "unit": "ms/iter",
            "extra": "iterations: 87756\ncpu: 0.008055260996399215 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.18432914048257065,
            "unit": "ms/iter",
            "extra": "iterations: 3730\ncpu: 0.1843274243967816 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 11.123470274193616,
            "unit": "ms/iter",
            "extra": "iterations: 62\ncpu: 11.12288083870974 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011696286620690919,
            "unit": "ms/iter",
            "extra": "iterations: 600173\ncpu: 0.0011695894600390346 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.003995641578794585,
            "unit": "ms/iter",
            "extra": "iterations: 175729\ncpu: 0.003995537680178062 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.056202964077406034,
            "unit": "ms/iter",
            "extra": "iterations: 18345\ncpu: 0.05620064475333897 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.09526344329999574,
            "unit": "ms/iter",
            "extra": "iterations: 10000\ncpu: 0.09526265129999985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.9252269538834381,
            "unit": "ms/iter",
            "extra": "iterations: 412\ncpu: 1.9251293009708685 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 80.5893032222179,
            "unit": "ms/iter",
            "extra": "iterations: 9\ncpu: 80.58829988888918 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 72.14851904793112,
            "unit": "ns/iter",
            "extra": "iterations: 9689949\ncpu: 72.14556753601099 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 377.7964325159652,
            "unit": "ns/iter",
            "extra": "iterations: 1827114\ncpu: 377.79122430236856 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10765.900585155,
            "unit": "ns/iter",
            "extra": "iterations: 65111\ncpu: 10765.43324476652 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 135122.8979952834,
            "unit": "ns/iter",
            "extra": "iterations: 5088\ncpu: 135117.4400550313 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 57.54271147403589,
            "unit": "ns/iter",
            "extra": "iterations: 12184782\ncpu: 57.541605258099345 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 156.58888700637576,
            "unit": "ns/iter",
            "extra": "iterations: 4426710\ncpu: 156.57861301056462 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2299.5023111364517,
            "unit": "ns/iter",
            "extra": "iterations: 303314\ncpu: 2299.456757683468 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 38721.62637845109,
            "unit": "ns/iter",
            "extra": "iterations: 18227\ncpu: 38719.29785483024 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 81.71426613667538,
            "unit": "ns/iter",
            "extra": "iterations: 8668794\ncpu: 81.70876467937691 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 416.2802649286584,
            "unit": "ns/iter",
            "extra": "iterations: 1685284\ncpu: 416.25497364242585 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10781.971022526415,
            "unit": "ns/iter",
            "extra": "iterations: 64947\ncpu: 10781.341863365342 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 130645.40790706316,
            "unit": "ns/iter",
            "extra": "iterations: 5337\ncpu: 130635.61963649874 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 92.45035083018406,
            "unit": "ns/iter",
            "extra": "iterations: 7732516\ncpu: 92.4477084819479 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 220.9068130671048,
            "unit": "ns/iter",
            "extra": "iterations: 3139335\ncpu: 220.90707172059913 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3637.649271863817,
            "unit": "ns/iter",
            "extra": "iterations: 191379\ncpu: 3637.5780623787987 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 45733.07992975826,
            "unit": "ns/iter",
            "extra": "iterations: 15376\ncpu: 45728.95727107256 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.002615432272220838,
            "unit": "ms/iter",
            "extra": "iterations: 265696\ncpu: 0.0026151586888774826 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.06513948184418303,
            "unit": "ms/iter",
            "extra": "iterations: 11539\ncpu: 0.065136075309819 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.0024232857632956687,
            "unit": "ms/iter",
            "extra": "iterations: 291486\ncpu: 0.0024230652244018975 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07072231555510909,
            "unit": "ms/iter",
            "extra": "iterations: 9881\ncpu: 0.07072105050096221 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.002316077505155628,
            "unit": "ms/iter",
            "extra": "iterations: 303554\ncpu: 0.0023159941361339197 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5236551031390007,
            "unit": "ms/iter",
            "extra": "iterations: 1338\ncpu: 0.5236194088191327 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 38.69387377777874,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 38.6929095555549 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6272.336929000005,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6271.875164999983 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.005580519051624162,
            "unit": "ms/iter",
            "extra": "iterations: 126026\ncpu: 0.005580405424277494 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6179423551237138,
            "unit": "ms/iter",
            "extra": "iterations: 1132\ncpu: 0.617909696113066 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 40.37787994117787,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 40.376180823530454 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6435.3344379999835,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6433.005946000008 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005867462957434142,
            "unit": "ms/iter",
            "extra": "iterations: 123925\ncpu: 0.005866246850917879 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.6323815909502019,
            "unit": "ms/iter",
            "extra": "iterations: 1105\ncpu: 0.6323765936651513 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 65.54553909090932,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 65.54023499999899 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 26095.513185000014,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 26078.611295000002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.014039755110293083,
            "unit": "ms/iter",
            "extra": "iterations: 49459\ncpu: 0.014039494106229854 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7282986649377358,
            "unit": "ms/iter",
            "extra": "iterations: 964\ncpu: 0.7282741794605805 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 49.71317550000062,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 49.71026607142903 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 17991.19912799995,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 17927.191178000015 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.005556298328075891,
            "unit": "ms/iter",
            "extra": "iterations: 126800\ncpu: 0.005556094463722466 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 0.9807892615385213,
            "unit": "ms/iter",
            "extra": "iterations: 715\ncpu: 0.9807445636363672 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 70.00859540000306,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 70.00440000000197 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 6979.390609000006,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6979.0724249999985 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.02399616599856111,
            "unit": "ms/iter",
            "extra": "iterations: 29187\ncpu: 0.02399490804125097 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.4556266029106262,
            "unit": "ms/iter",
            "extra": "iterations: 481\ncpu: 1.4555265259875045 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 97.54698100000171,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 97.53187385714617 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 9123.962491000042,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 9123.746221000005 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.0006371178185814724,
            "unit": "ms/iter",
            "extra": "iterations: 1104639\ncpu: 0.0006370881790340615 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.003950679001019332,
            "unit": "ms/iter",
            "extra": "iterations: 176580\ncpu: 0.003950644988107299 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.0368525130425617,
            "unit": "ms/iter",
            "extra": "iterations: 18938\ncpu: 0.03684860645263555 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.38494002924946064,
            "unit": "ms/iter",
            "extra": "iterations: 1812\ncpu: 0.38489703697572153 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.00044023710987730935,
            "unit": "ms/iter",
            "extra": "iterations: 1591102\ncpu: 0.00044023399442651655 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.001153331119494782,
            "unit": "ms/iter",
            "extra": "iterations: 609650\ncpu: 0.0011533097744607457 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.0072743617638165526,
            "unit": "ms/iter",
            "extra": "iterations: 95883\ncpu: 0.007274131618743581 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.06806944654150679,
            "unit": "ms/iter",
            "extra": "iterations: 10120\ncpu: 0.0680646082015801 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.0006374540501827496,
            "unit": "ms/iter",
            "extra": "iterations: 1097469\ncpu: 0.0006374381882312783 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.0049964497576620364,
            "unit": "ms/iter",
            "extra": "iterations: 140300\ncpu: 0.004996333734853943 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.05051044510026944,
            "unit": "ms/iter",
            "extra": "iterations: 14062\ncpu: 0.050469804864173046 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5253874686319179,
            "unit": "ms/iter",
            "extra": "iterations: 1323\ncpu: 0.5253839092970549 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.0004481661112788428,
            "unit": "ms/iter",
            "extra": "iterations: 1560171\ncpu: 0.0004481583255938078 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.0022631923674429157,
            "unit": "ms/iter",
            "extra": "iterations: 305114\ncpu: 0.0022630074562293907 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.022683100161393497,
            "unit": "ms/iter",
            "extra": "iterations: 30980\ncpu: 0.022682296513879794 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.22902747976502566,
            "unit": "ms/iter",
            "extra": "iterations: 3064\ncpu: 0.229028013381201 ms\nthreads: 1"
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
          "id": "5f08353fbaca24343d39707ddfd0473860e6dd41",
          "message": "Remove infinity()",
          "timestamp": "2025-02-08T11:02:56+01:00",
          "tree_id": "6cab3d9273bd392033df4e9b7d68e5b758ac6b82",
          "url": "https://github.com/attcs/Octree/commit/5f08353fbaca24343d39707ddfd0473860e6dd41"
        },
        "date": 1739009335383,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1791.8963951488095,
            "unit": "ns/iter",
            "extra": "iterations: 393553\ncpu: 1791.827563250693 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 715.953645926913,
            "unit": "ns/iter",
            "extra": "iterations: 979504\ncpu: 715.7570351933222 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetNodeEntities/10000",
            "value": 688.3117831248501,
            "unit": "ns/iter",
            "extra": "iterations: 1016216\ncpu: 688.1950057861717 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000",
            "value": 860.3495318813441,
            "unit": "ns/iter",
            "extra": "iterations: 814003\ncpu: 860.3547149089131 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000",
            "value": 8008.665472361161,
            "unit": "ns/iter",
            "extra": "iterations: 87359\ncpu: 8008.559060886686 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.00029944283578956367,
            "unit": "ms/iter",
            "extra": "iterations: 2334870\ncpu: 0.0002994415307918642 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003707609590955846,
            "unit": "ms/iter",
            "extra": "iterations: 1920893\ncpu: 0.0003707414301577446 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0012023041852860551,
            "unit": "ms/iter",
            "extra": "iterations: 584739\ncpu: 0.0012022593379268347 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0018753683982104465,
            "unit": "ms/iter",
            "extra": "iterations: 373270\ncpu: 0.0018752967155142401 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.030341392647058478,
            "unit": "ms/iter",
            "extra": "iterations: 23120\ncpu: 0.03034021820934254 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.8014362491428528,
            "unit": "ms/iter",
            "extra": "iterations: 875\ncpu: 0.8013833371428579 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 9.992543753623005,
            "unit": "ms/iter",
            "extra": "iterations: 69\ncpu: 9.991672434782597 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 69.86603210000055,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 69.86337099999993 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.001742144194775249,
            "unit": "ms/iter",
            "extra": "iterations: 400708\ncpu: 0.0017420788953552192 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.0025860809857594608,
            "unit": "ms/iter",
            "extra": "iterations: 270776\ncpu: 0.0025860380646733814 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.004728783130661189,
            "unit": "ms/iter",
            "extra": "iterations: 148269\ncpu: 0.004720561877398521 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.006442154953927766,
            "unit": "ms/iter",
            "extra": "iterations: 108742\ncpu: 0.006441681208732594 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04802126408233252,
            "unit": "ms/iter",
            "extra": "iterations: 14575\ncpu: 0.04800389653516288 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6215000250223569,
            "unit": "ms/iter",
            "extra": "iterations: 1119\ncpu: 0.6214461769436984 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.689283466666785,
            "unit": "ms/iter",
            "extra": "iterations: 105\ncpu: 6.68889557142857 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 34.42879140000059,
            "unit": "ms/iter",
            "extra": "iterations: 20\ncpu: 34.42651604999991 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.0031733360029588655,
            "unit": "ms/iter",
            "extra": "iterations: 219007\ncpu: 0.0031733211404201636 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.006117599489572855,
            "unit": "ms/iter",
            "extra": "iterations: 114414\ncpu: 0.006117388361564151 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.016154509802572707,
            "unit": "ms/iter",
            "extra": "iterations: 43611\ncpu: 0.016153197702414596 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.040410863169436366,
            "unit": "ms/iter",
            "extra": "iterations: 17328\ncpu: 0.04040951950600182 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.39353299550309345,
            "unit": "ms/iter",
            "extra": "iterations: 1779\ncpu: 0.39350955593029746 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.9202918833333293,
            "unit": "ms/iter",
            "extra": "iterations: 240\ncpu: 2.9201918250000074 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.0004366147651241652,
            "unit": "ms/iter",
            "extra": "iterations: 1599824\ncpu: 0.00043660552223244626 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.0015674886509373356,
            "unit": "ms/iter",
            "extra": "iterations: 439772\ncpu: 0.0015674645429904588 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0027166072627676473,
            "unit": "ms/iter",
            "extra": "iterations: 257037\ncpu: 0.0027165564996479152 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.00928328686739626,
            "unit": "ms/iter",
            "extra": "iterations: 76687\ncpu: 0.009282887542869047 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14209868165175232,
            "unit": "ms/iter",
            "extra": "iterations: 4916\ncpu: 0.14208957587469526 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7155340441176496,
            "unit": "ms/iter",
            "extra": "iterations: 408\ncpu: 1.7154638014705867 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.001158950242901739,
            "unit": "ms/iter",
            "extra": "iterations: 602507\ncpu: 0.0011588909954573148 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.0027708556085117657,
            "unit": "ms/iter",
            "extra": "iterations: 253249\ncpu: 0.0027707285438441933 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.005354865577985694,
            "unit": "ms/iter",
            "extra": "iterations: 130678\ncpu: 0.005354897274216028 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.01779343202016821,
            "unit": "ms/iter",
            "extra": "iterations: 39269\ncpu: 0.017792749420662668 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.42905036699506904,
            "unit": "ms/iter",
            "extra": "iterations: 1624\ncpu: 0.4290394242610814 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 5.990772831932718,
            "unit": "ms/iter",
            "extra": "iterations: 119\ncpu: 5.9907428991597 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004548464111799148,
            "unit": "ms/iter",
            "extra": "iterations: 1533178\ncpu: 0.00045483166338155184 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.0007315612219043531,
            "unit": "ms/iter",
            "extra": "iterations: 951171\ncpu: 0.0007315288733571579 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.0021235963105989243,
            "unit": "ms/iter",
            "extra": "iterations: 331978\ncpu: 0.002123520950183447 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.0046160841580404465,
            "unit": "ms/iter",
            "extra": "iterations: 145001\ncpu: 0.004616048999662063 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.06449961382675024,
            "unit": "ms/iter",
            "extra": "iterations: 10863\ncpu: 0.06449715925619061 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.08251479245284,
            "unit": "ms/iter",
            "extra": "iterations: 636\ncpu: 1.082455496855343 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.69012546188498,
            "unit": "ns/iter",
            "extra": "iterations: 39590510\ncpu: 17.68937654503574 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.013388405844847,
            "unit": "ns/iter",
            "extra": "iterations: 22579686\ncpu: 31.012594904995662 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 40.868794398024136,
            "unit": "ns/iter",
            "extra": "iterations: 17092822\ncpu: 40.86684907851947 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 86.53229808769157,
            "unit": "ns/iter",
            "extra": "iterations: 8272626\ncpu: 86.5288751117239 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 249.0867539094994,
            "unit": "ns/iter",
            "extra": "iterations: 2633645\ncpu: 249.0830620679703 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 1903.4120807337608,
            "unit": "ns/iter",
            "extra": "iterations: 368173\ncpu: 1903.374552180639 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.00975745688892573,
            "unit": "ms/iter",
            "extra": "iterations: 72348\ncpu: 0.009757163224968151 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.215826192615378,
            "unit": "ms/iter",
            "extra": "iterations: 3250\ncpu: 0.21581811384615432 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015146402638188262,
            "unit": "ms/iter",
            "extra": "iterations: 445154\ncpu: 0.0015145890231245847 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.026767296789040267,
            "unit": "ms/iter",
            "extra": "iterations: 27811\ncpu: 0.026765123332494332 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.00038714920132995145,
            "unit": "ms/iter",
            "extra": "iterations: 1806378\ncpu: 0.00038713229014082553 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005914832769581423,
            "unit": "ms/iter",
            "extra": "iterations: 1181962\ncpu: 0.0005914496066709423 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0018473341205340395,
            "unit": "ms/iter",
            "extra": "iterations: 378980\ncpu: 0.001847290928281167 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.0035269835200354652,
            "unit": "ms/iter",
            "extra": "iterations: 197998\ncpu: 0.0035268078010889116 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.03843668067135332,
            "unit": "ms/iter",
            "extra": "iterations: 18351\ncpu: 0.03843390507329303 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 1.017326581395337,
            "unit": "ms/iter",
            "extra": "iterations: 688\ncpu: 1.0172308808139445 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 9.848397492957693,
            "unit": "ms/iter",
            "extra": "iterations: 71\ncpu: 9.84796223943663 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 100.4804467142841,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 100.47202514285759 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.002013591002819067,
            "unit": "ms/iter",
            "extra": "iterations: 354022\ncpu: 0.0019774355943980894 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.002910824475106562,
            "unit": "ms/iter",
            "extra": "iterations: 240906\ncpu: 0.00290602024856169 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005581953358401684,
            "unit": "ms/iter",
            "extra": "iterations: 125789\ncpu: 0.00556989798790041 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.008896565171537783,
            "unit": "ms/iter",
            "extra": "iterations: 78700\ncpu: 0.008890065133418038 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.048225060668593715,
            "unit": "ms/iter",
            "extra": "iterations: 14538\ncpu: 0.048162941326179404 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.4752842240216012,
            "unit": "ms/iter",
            "extra": "iterations: 1482\ncpu: 0.4749021018893334 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 4.092627782352686,
            "unit": "ms/iter",
            "extra": "iterations: 170\ncpu: 4.091161429411727 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 41.96262547058977,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 41.957659941176345 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007802435406367606,
            "unit": "ms/iter",
            "extra": "iterations: 896528\ncpu: 0.0007801561713632919 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.0021476041291534605,
            "unit": "ms/iter",
            "extra": "iterations: 325442\ncpu: 0.0021472320720743077 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.0052408636621313785,
            "unit": "ms/iter",
            "extra": "iterations: 134064\ncpu: 0.0052403535177229284 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.014803950605817358,
            "unit": "ms/iter",
            "extra": "iterations: 47374\ncpu: 0.014802823025288398 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.2376563640054086,
            "unit": "ms/iter",
            "extra": "iterations: 2956\ncpu: 0.23764155108254614 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 2.9830159191487353,
            "unit": "ms/iter",
            "extra": "iterations: 235\ncpu: 2.982970519148955 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 29.055948499999335,
            "unit": "ms/iter",
            "extra": "iterations: 24\ncpu: 29.05447320833332 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 306.3400540000032,
            "unit": "ms/iter",
            "extra": "iterations: 2\ncpu: 306.32722450000216 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.0041268949441114425,
            "unit": "ms/iter",
            "extra": "iterations: 169624\ncpu: 0.00412631721336606 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.00920453649908575,
            "unit": "ms/iter",
            "extra": "iterations: 76043\ncpu: 0.009203643833094424 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.01858433300539686,
            "unit": "ms/iter",
            "extra": "iterations: 37609\ncpu: 0.018509352708128306 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.04238275055679417,
            "unit": "ms/iter",
            "extra": "iterations: 16613\ncpu: 0.04235317859507561 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.37059135838150997,
            "unit": "ms/iter",
            "extra": "iterations: 1903\ncpu: 0.3705867703625881 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.8386396275304153,
            "unit": "ms/iter",
            "extra": "iterations: 247\ncpu: 2.838589599190265 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.95985749999972,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 24.900629249999998 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 280.98375833332057,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 280.9506879999996 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.001575133222880755,
            "unit": "ms/iter",
            "extra": "iterations: 444233\ncpu: 0.001574883747943101 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.0030509253254930443,
            "unit": "ms/iter",
            "extra": "iterations: 230266\ncpu: 0.0030505812929394515 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.00618693897076259,
            "unit": "ms/iter",
            "extra": "iterations: 113929\ncpu: 0.00618680732737055 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011231189595542477,
            "unit": "ms/iter",
            "extra": "iterations: 62454\ncpu: 0.011231250200147305 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.124643455837924,
            "unit": "ms/iter",
            "extra": "iterations: 5627\ncpu: 0.1246422139683673 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.0616150560606008,
            "unit": "ms/iter",
            "extra": "iterations: 660\ncpu: 1.0615974848484846 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.0025912219833544125,
            "unit": "ms/iter",
            "extra": "iterations: 274908\ncpu: 0.0025910217781948707 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.007037552644745214,
            "unit": "ms/iter",
            "extra": "iterations: 99934\ncpu: 0.0070373336301960115 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.01646545584815226,
            "unit": "ms/iter",
            "extra": "iterations: 42569\ncpu: 0.016464886819046645 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.03785316805136135,
            "unit": "ms/iter",
            "extra": "iterations: 18536\ncpu: 0.03785245123003906 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.3815569242506816,
            "unit": "ms/iter",
            "extra": "iterations: 1835\ncpu: 0.381536989100816 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.1253039999999066,
            "unit": "ms/iter",
            "extra": "iterations: 222\ncpu: 3.125251139639605 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005598208570359878,
            "unit": "ms/iter",
            "extra": "iterations: 1256890\ncpu: 0.0005597919404243753 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017628626436666202,
            "unit": "ms/iter",
            "extra": "iterations: 399188\ncpu: 0.0017612126391574861 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0030512927563334765,
            "unit": "ms/iter",
            "extra": "iterations: 229925\ncpu: 0.0030512229509622865 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007123799365207187,
            "unit": "ms/iter",
            "extra": "iterations: 99245\ncpu: 0.007123341236334414 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.06340486685246985,
            "unit": "ms/iter",
            "extra": "iterations: 11123\ncpu: 0.06340121478018589 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 0.7346486919740012,
            "unit": "ms/iter",
            "extra": "iterations: 922\ncpu: 0.7346037375271107 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0016639300724922488,
            "unit": "ms/iter",
            "extra": "iterations: 420321\ncpu: 0.001663850892532136 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.003335915805050154,
            "unit": "ms/iter",
            "extra": "iterations: 210844\ncpu: 0.003335736155641177 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008625909349145155,
            "unit": "ms/iter",
            "extra": "iterations: 80617\ncpu: 0.008625760025800986 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.019229717282086597,
            "unit": "ms/iter",
            "extra": "iterations: 36425\ncpu: 0.01922896897735086 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.23536996498316484,
            "unit": "ms/iter",
            "extra": "iterations: 2970\ncpu: 0.23536600101010186 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 2.18991352187512,
            "unit": "ms/iter",
            "extra": "iterations: 320\ncpu: 2.1898094593749917 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005367371598939612,
            "unit": "ms/iter",
            "extra": "iterations: 1305071\ncpu: 0.0005367294706571502 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0012238021724178705,
            "unit": "ms/iter",
            "extra": "iterations: 571437\ncpu: 0.0012237636887355845 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0037970253970475197,
            "unit": "ms/iter",
            "extra": "iterations: 185809\ncpu: 0.003796858069307761 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.007941236655374493,
            "unit": "ms/iter",
            "extra": "iterations: 87376\ncpu: 0.0079410753982788 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.1833569385138666,
            "unit": "ms/iter",
            "extra": "iterations: 3822\ncpu: 0.18335225850340128 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 11.080921492063702,
            "unit": "ms/iter",
            "extra": "iterations: 63\ncpu: 11.080748269841076 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.0011653582017204429,
            "unit": "ms/iter",
            "extra": "iterations: 602398\ncpu: 0.0011652996424290918 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.003903795007789777,
            "unit": "ms/iter",
            "extra": "iterations: 177797\ncpu: 0.003903550161138844 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.05443439736033389,
            "unit": "ms/iter",
            "extra": "iterations: 19851\ncpu: 0.05443172233136833 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.09156672870000193,
            "unit": "ms/iter",
            "extra": "iterations: 10000\ncpu: 0.09156193950000073 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.8749563348214235,
            "unit": "ms/iter",
            "extra": "iterations: 448\ncpu: 1.8748796874999816 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 79.71912855555905,
            "unit": "ms/iter",
            "extra": "iterations: 9\ncpu: 79.71484077777784 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 72.14567483838621,
            "unit": "ns/iter",
            "extra": "iterations: 9722839\ncpu: 72.14248698348203 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 359.658369120627,
            "unit": "ns/iter",
            "extra": "iterations: 1957165\ncpu: 359.6420506191395 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10848.576312728475,
            "unit": "ns/iter",
            "extra": "iterations: 64465\ncpu: 10848.277809664183 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 136989.50748621306,
            "unit": "ns/iter",
            "extra": "iterations: 5076\ncpu: 136987.15681639017 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 57.00980694566421,
            "unit": "ns/iter",
            "extra": "iterations: 12306380\ncpu: 57.00683531631605 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 151.33542608950145,
            "unit": "ns/iter",
            "extra": "iterations: 4632806\ncpu: 151.33124525395596 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 2306.161665391267,
            "unit": "ns/iter",
            "extra": "iterations: 300590\ncpu: 2306.1149539239614 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 39170.910243464285,
            "unit": "ns/iter",
            "extra": "iterations: 17826\ncpu: 39170.41506787795 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 81.89798111465606,
            "unit": "ns/iter",
            "extra": "iterations: 8576069\ncpu: 81.89612280404937 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 392.77297306056107,
            "unit": "ns/iter",
            "extra": "iterations: 1789607\ncpu: 392.77092233098847 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 10994.452966555778,
            "unit": "ns/iter",
            "extra": "iterations: 63508\ncpu: 10993.88248724576 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 131969.88369462782,
            "unit": "ns/iter",
            "extra": "iterations: 5305\ncpu: 131964.42865221435 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 91.38197887285166,
            "unit": "ns/iter",
            "extra": "iterations: 7698342\ncpu: 91.3751983219251 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 218.21877445535867,
            "unit": "ns/iter",
            "extra": "iterations: 3165205\ncpu: 218.20039270758082 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3634.101335579333,
            "unit": "ns/iter",
            "extra": "iterations: 192351\ncpu: 3633.898357689875 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 45141.34927797837,
            "unit": "ns/iter",
            "extra": "iterations: 15512\ncpu: 45140.53958225789 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.0026366700612375465,
            "unit": "ms/iter",
            "extra": "iterations: 266013\ncpu: 0.0026366291121110177 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.06065993224176855,
            "unit": "ms/iter",
            "extra": "iterations: 11482\ncpu: 0.060655916042501375 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.0023518329308481753,
            "unit": "ms/iter",
            "extra": "iterations: 295249\ncpu: 0.0023517216180240557 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.07045131967880129,
            "unit": "ms/iter",
            "extra": "iterations: 9838\ncpu: 0.07044708975401436 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.00235336993538043,
            "unit": "ms/iter",
            "extra": "iterations: 298517\ncpu: 0.0023532543171745526 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5168306476683905,
            "unit": "ms/iter",
            "extra": "iterations: 1351\ncpu: 0.5167930777202063 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 38.741068722221904,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 38.73870033333383 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6206.972003000033,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6206.141973000002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.006007025323694283,
            "unit": "ms/iter",
            "extra": "iterations: 117163\ncpu: 0.006005769483540067 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6222038193950207,
            "unit": "ms/iter",
            "extra": "iterations: 1124\ncpu: 0.6221464145907575 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 40.330955764706204,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 40.325148764706604 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6394.820400000015,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6394.428705000024 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005748307931230046,
            "unit": "ms/iter",
            "extra": "iterations: 123310\ncpu: 0.005747149257967666 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.6356390526315707,
            "unit": "ms/iter",
            "extra": "iterations: 1102\ncpu: 0.6355336361161491 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 64.68877945454469,
            "unit": "ms/iter",
            "extra": "iterations: 11\ncpu: 64.68211200000114 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 24097.494580000046,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 24086.185750999986 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.013728983901477603,
            "unit": "ms/iter",
            "extra": "iterations: 50750\ncpu: 0.013728508985221493 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.7222420258531691,
            "unit": "ms/iter",
            "extra": "iterations: 967\ncpu: 0.7222044064115927 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 49.5694096428565,
            "unit": "ms/iter",
            "extra": "iterations: 14\ncpu: 49.56816500000067 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 16832.191219000037,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 16771.091329000003 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.005685354179189306,
            "unit": "ms/iter",
            "extra": "iterations: 123110\ncpu: 0.005685112955893049 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 0.9913789957385949,
            "unit": "ms/iter",
            "extra": "iterations: 704\ncpu: 0.9913371676136312 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 70.57679069999949,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 70.56774819999987 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 6947.550331000002,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6947.414775999988 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.02322535214072281,
            "unit": "ms/iter",
            "extra": "iterations: 30130\ncpu: 0.023223742449386506 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.464319596234261,
            "unit": "ms/iter",
            "extra": "iterations: 478\ncpu: 1.4641152008368576 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 98.7340992857152,
            "unit": "ms/iter",
            "extra": "iterations: 7\ncpu: 98.73306771428604 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 9101.80421299998,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 9101.524903000012 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.0006428550852599004,
            "unit": "ms/iter",
            "extra": "iterations: 1107672\ncpu: 0.000642845938147759 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.003912152668726309,
            "unit": "ms/iter",
            "extra": "iterations: 176395\ncpu: 0.003911655925621594 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.03848958044561042,
            "unit": "ms/iter",
            "extra": "iterations: 19075\ncpu: 0.03848848377457463 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.3837662705817589,
            "unit": "ms/iter",
            "extra": "iterations: 1822\ncpu: 0.38375116684962 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.0004379206270962961,
            "unit": "ms/iter",
            "extra": "iterations: 1596565\ncpu: 0.0004379173162382954 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.001168152172832141,
            "unit": "ms/iter",
            "extra": "iterations: 603360\ncpu: 0.0011681154932378546 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.007314904640866672,
            "unit": "ms/iter",
            "extra": "iterations: 95995\ncpu: 0.0073146859107244035 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.06803646891617288,
            "unit": "ms/iter",
            "extra": "iterations: 10343\ncpu: 0.06803254278255809 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.0006431471000300592,
            "unit": "ms/iter",
            "extra": "iterations: 1097770\ncpu: 0.0006431305947511725 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.004976939573853873,
            "unit": "ms/iter",
            "extra": "iterations: 141313\ncpu: 0.0049767547288642475 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.04957042751825508,
            "unit": "ms/iter",
            "extra": "iterations: 14107\ncpu: 0.04957069632097721 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5274315067771135,
            "unit": "ms/iter",
            "extra": "iterations: 1328\ncpu: 0.5274069879518034 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.00044788441111972806,
            "unit": "ms/iter",
            "extra": "iterations: 1563204\ncpu: 0.0004478868004431808 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.002282641692712115,
            "unit": "ms/iter",
            "extra": "iterations: 302355\ncpu: 0.0022825822394205173 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.022754077356656834,
            "unit": "ms/iter",
            "extra": "iterations: 30870\ncpu: 0.02275418069322959 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.23074113996076367,
            "unit": "ms/iter",
            "extra": "iterations: 3058\ncpu: 0.23072867069980063 ms\nthreads: 1"
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
          "id": "90c3ab34cb6d0b33d2139d25e8b18c86c0adc2c0",
          "message": "Fixed depth morton ID in box solution",
          "timestamp": "2025-02-08T23:17:13+01:00",
          "tree_id": "44d3ec71186d4b80b83ab27411ce96259d08a4a9",
          "url": "https://github.com/attcs/Octree/commit/90c3ab34cb6d0b33d2139d25e8b18c86c0adc2c0"
        },
        "date": 1739053449131,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "Benchmarks::Base::GetNodeID/1000",
            "value": 1800.9259153369383,
            "unit": "ns/iter",
            "extra": "iterations: 391741\ncpu: 1800.8504011579082 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetDepthID/1000",
            "value": 714.907649599895,
            "unit": "ns/iter",
            "extra": "iterations: 975502\ncpu: 714.8557511927193 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GetNodeEntities/10000",
            "value": 689.193899217007,
            "unit": "ns/iter",
            "extra": "iterations: 1017771\ncpu: 689.18112620619 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetPointGridID/1000",
            "value": 854.7384519784986,
            "unit": "ns/iter",
            "extra": "iterations: 819123\ncpu: 854.681017380784 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Base::GridSpaceIndexing_GetBoxGridID/1000",
            "value": 8155.917288441973,
            "unit": "ns/iter",
            "extra": "iterations: 86312\ncpu: 8155.633295486144 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10",
            "value": 0.000289553427997358,
            "unit": "ms/iter",
            "extra": "iterations: 2420772\ncpu: 0.00028954541485113006 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/20",
            "value": 0.0003541629885031953,
            "unit": "ms/iter",
            "extra": "iterations: 1974808\ncpu: 0.0003541573692227298 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/50",
            "value": 0.0011396570942449568,
            "unit": "ms/iter",
            "extra": "iterations: 613603\ncpu: 0.0011396393531322364 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100",
            "value": 0.0018116152264808472,
            "unit": "ms/iter",
            "extra": "iterations: 387207\ncpu: 0.0018115776935850843 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000",
            "value": 0.02978974119748249,
            "unit": "ms/iter",
            "extra": "iterations: 23516\ncpu: 0.029788993621364113 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/10000",
            "value": 0.7927224920814936,
            "unit": "ms/iter",
            "extra": "iterations: 884\ncpu: 0.7926826131221713 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/100000",
            "value": 9.891371457142993,
            "unit": "ms/iter",
            "extra": "iterations: 70\ncpu: 9.890834514285705 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, false>/1000000",
            "value": 67.80888709999999,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 67.80687679999993 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10",
            "value": 0.0016787438732092197,
            "unit": "ms/iter",
            "extra": "iterations: 417633\ncpu: 0.001678230738471338 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/20",
            "value": 0.002497860587805348,
            "unit": "ms/iter",
            "extra": "iterations: 280535\ncpu: 0.002495804637567505 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/50",
            "value": 0.0045954843254944035,
            "unit": "ms/iter",
            "extra": "iterations: 152445\ncpu: 0.004593692020072816 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100",
            "value": 0.006338141785920997,
            "unit": "ms/iter",
            "extra": "iterations: 110688\ncpu: 0.006333505565192238 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000",
            "value": 0.04641151048718887,
            "unit": "ms/iter",
            "extra": "iterations: 15066\ncpu: 0.04634695320589402 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/10000",
            "value": 0.6115644864864408,
            "unit": "ms/iter",
            "extra": "iterations: 1147\ncpu: 0.6114229267654745 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/100000",
            "value": 6.566339317757164,
            "unit": "ms/iter",
            "extra": "iterations: 107\ncpu: 6.56623751401869 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Create<3, true>/1000000",
            "value": 33.83404315000007,
            "unit": "ms/iter",
            "extra": "iterations: 20\ncpu: 33.833435400000056 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10",
            "value": 0.003212837358624141,
            "unit": "ms/iter",
            "extra": "iterations: 217417\ncpu: 0.0032127846810506996 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/20",
            "value": 0.00621869724925872,
            "unit": "ms/iter",
            "extra": "iterations: 112297\ncpu: 0.006218596525285607 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/50",
            "value": 0.016374906344410908,
            "unit": "ms/iter",
            "extra": "iterations: 43030\ncpu: 0.016374484917499422 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/100",
            "value": 0.04096882635217604,
            "unit": "ms/iter",
            "extra": "iterations: 17213\ncpu: 0.040968116888398166 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/1000",
            "value": 0.39687779523540917,
            "unit": "ms/iter",
            "extra": "iterations: 1763\ncpu: 0.3968742994895063 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertToLeaf/10000",
            "value": 2.882442539094805,
            "unit": "ms/iter",
            "extra": "iterations: 243\ncpu: 2.882330724279832 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10",
            "value": 0.0004346914729086473,
            "unit": "ms/iter",
            "extra": "iterations: 1609998\ncpu: 0.0004346743176078482 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/20",
            "value": 0.00158579162735556,
            "unit": "ms/iter",
            "extra": "iterations: 441987\ncpu: 0.0015857533026989484 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/50",
            "value": 0.0027923799760121663,
            "unit": "ms/iter",
            "extra": "iterations: 254295\ncpu: 0.0027923030260130974 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/100",
            "value": 0.009229225415095767,
            "unit": "ms/iter",
            "extra": "iterations: 75585\ncpu: 0.00922889527022556 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/1000",
            "value": 0.14128617272544808,
            "unit": "ms/iter",
            "extra": "iterations: 4979\ncpu: 0.1412781865836518 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertWithRebalancing/10000",
            "value": 1.7210564160584458,
            "unit": "ms/iter",
            "extra": "iterations: 411\ncpu: 1.7209779051094933 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10",
            "value": 0.0011547035825886713,
            "unit": "ms/iter",
            "extra": "iterations: 605791\ncpu: 0.0011546933694954174 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/20",
            "value": 0.002772212636627421,
            "unit": "ms/iter",
            "extra": "iterations: 251871\ncpu: 0.002772168272647486 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/50",
            "value": 0.005367865330302384,
            "unit": "ms/iter",
            "extra": "iterations: 130653\ncpu: 0.005367685081858074 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/100",
            "value": 0.017892362118419027,
            "unit": "ms/iter",
            "extra": "iterations: 39048\ncpu: 0.017891393413235028 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/1000",
            "value": 0.43272689150653104,
            "unit": "ms/iter",
            "extra": "iterations: 1613\ncpu: 0.43270053068816194 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::InsertUnique/10000",
            "value": 5.892202899159553,
            "unit": "ms/iter",
            "extra": "iterations: 119\ncpu: 5.891867109243717 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10",
            "value": 0.0004724371731971899,
            "unit": "ms/iter",
            "extra": "iterations: 1493829\ncpu: 0.00047242115931608983 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/20",
            "value": 0.000749661036115248,
            "unit": "ms/iter",
            "extra": "iterations: 938274\ncpu: 0.0007496267987815901 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/50",
            "value": 0.00221735151705361,
            "unit": "ms/iter",
            "extra": "iterations: 328169\ncpu: 0.002217236189280524 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/100",
            "value": 0.004607835644744609,
            "unit": "ms/iter",
            "extra": "iterations: 150990\ncpu: 0.004607719299291359 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/1000",
            "value": 0.061072216429010534,
            "unit": "ms/iter",
            "extra": "iterations: 11431\ncpu: 0.06106860782083832 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Update/10000",
            "value": 1.0565102240853719,
            "unit": "ms/iter",
            "extra": "iterations: 656\ncpu: 1.056486611280479 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/1000",
            "value": 17.791533272449843,
            "unit": "ns/iter",
            "extra": "iterations: 39367961\ncpu: 17.790803364187443 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::Contains/10000",
            "value": 31.174150033387257,
            "unit": "ns/iter",
            "extra": "iterations: 22521414\ncpu: 31.173419528631854 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100",
            "value": 45.89787812449667,
            "unit": "ns/iter",
            "extra": "iterations: 15188450\ncpu: 45.89556004727259 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/1000",
            "value": 89.42838383235163,
            "unit": "ns/iter",
            "extra": "iterations: 7819987\ncpu: 89.42647986499219 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/10000",
            "value": 286.162381299774,
            "unit": "ns/iter",
            "extra": "iterations: 2519582\ncpu: 286.1571502733366 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::RangeSearch/100000",
            "value": 2109.53025744597,
            "unit": "ns/iter",
            "extra": "iterations: 334711\ncpu: 2109.4725449716384 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/1000",
            "value": 0.009661843590897822,
            "unit": "ms/iter",
            "extra": "iterations: 71102\ncpu: 0.009661288922955768 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::GetNearestNeighbors/10000",
            "value": 0.21625114768849843,
            "unit": "ms/iter",
            "extra": "iterations: 3223\ncpu: 0.21624101365187703 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/1000",
            "value": 0.0015321108756149622,
            "unit": "ms/iter",
            "extra": "iterations: 456719\ncpu: 0.0015320346493138978 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Point::FrustumCulling/10000",
            "value": 0.02525558720825402,
            "unit": "ms/iter",
            "extra": "iterations: 27721\ncpu: 0.025254128061758193 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10",
            "value": 0.0003731992936948475,
            "unit": "ms/iter",
            "extra": "iterations: 1879924\ncpu: 0.00037317866573329515 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/20",
            "value": 0.0005470305448451155,
            "unit": "ms/iter",
            "extra": "iterations: 1281722\ncpu: 0.0005470070342866864 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/50",
            "value": 0.0017055050819028681,
            "unit": "ms/iter",
            "extra": "iterations: 410181\ncpu: 0.0017054252586053435 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100",
            "value": 0.002979193300839062,
            "unit": "ms/iter",
            "extra": "iterations: 234298\ncpu: 0.002979096163859705 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000",
            "value": 0.033919226824552895,
            "unit": "ms/iter",
            "extra": "iterations: 20690\ncpu: 0.03391836737554369 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/10000",
            "value": 0.7403258048523238,
            "unit": "ms/iter",
            "extra": "iterations: 948\ncpu: 0.7402927858649783 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/100000",
            "value": 7.588853836956535,
            "unit": "ms/iter",
            "extra": "iterations: 92\ncpu: 7.588663478260849 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, false>/1000000",
            "value": 76.33706644444753,
            "unit": "ms/iter",
            "extra": "iterations: 9\ncpu: 76.32965144444468 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10",
            "value": 0.0018799190484595349,
            "unit": "ms/iter",
            "extra": "iterations: 373878\ncpu: 0.0018798280856322112 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/20",
            "value": 0.0026933647444299094,
            "unit": "ms/iter",
            "extra": "iterations: 259420\ncpu: 0.002693211622080023 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/50",
            "value": 0.005029576917002819,
            "unit": "ms/iter",
            "extra": "iterations: 139306\ncpu: 0.005028408690221511 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100",
            "value": 0.007288400004165898,
            "unit": "ms/iter",
            "extra": "iterations: 96014\ncpu: 0.007285495573562228 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000",
            "value": 0.04295143244403813,
            "unit": "ms/iter",
            "extra": "iterations: 16305\ncpu: 0.04290715418583275 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/10000",
            "value": 0.41576867201425544,
            "unit": "ms/iter",
            "extra": "iterations: 1683\ncpu: 0.41502569994057853 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/100000",
            "value": 3.390502246376771,
            "unit": "ms/iter",
            "extra": "iterations: 207\ncpu: 3.3813909613526074 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 0, true>/1000000",
            "value": 34.58085295000046,
            "unit": "ms/iter",
            "extra": "iterations: 20\ncpu: 34.55650469999938 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10",
            "value": 0.0007637528514908053,
            "unit": "ms/iter",
            "extra": "iterations: 914960\ncpu: 0.000763672335402652 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/20",
            "value": 0.00204335018453725,
            "unit": "ms/iter",
            "extra": "iterations: 343833\ncpu: 0.002043223812141359 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/50",
            "value": 0.0042107993830707135,
            "unit": "ms/iter",
            "extra": "iterations: 165335\ncpu: 0.004210521922158056 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100",
            "value": 0.013140062069873453,
            "unit": "ms/iter",
            "extra": "iterations: 53182\ncpu: 0.013139028524688852 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000",
            "value": 0.13417213567251368,
            "unit": "ms/iter",
            "extra": "iterations: 5130\ncpu: 0.13416378654970632 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/10000",
            "value": 2.3772326813558475,
            "unit": "ms/iter",
            "extra": "iterations: 295\ncpu: 2.3770659050847476 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/100000",
            "value": 25.27380757142948,
            "unit": "ms/iter",
            "extra": "iterations: 28\ncpu: 25.272975821428542 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, false>/1000000",
            "value": 260.54141666666436,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 260.5188073333314 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10",
            "value": 0.003987160049068539,
            "unit": "ms/iter",
            "extra": "iterations: 175265\ncpu: 0.003986808159073342 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/20",
            "value": 0.008723754392495724,
            "unit": "ms/iter",
            "extra": "iterations: 80592\ncpu: 0.00872189368671819 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/50",
            "value": 0.017266651132973605,
            "unit": "ms/iter",
            "extra": "iterations: 40557\ncpu: 0.017263954163276248 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100",
            "value": 0.038391948597104965,
            "unit": "ms/iter",
            "extra": "iterations: 18248\ncpu: 0.03838807617273091 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000",
            "value": 0.3356949652677219,
            "unit": "ms/iter",
            "extra": "iterations: 2073\ncpu: 0.33563342305837 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/10000",
            "value": 2.6372019169811347,
            "unit": "ms/iter",
            "extra": "iterations: 265\ncpu: 2.6368474490565843 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/100000",
            "value": 24.52948810344753,
            "unit": "ms/iter",
            "extra": "iterations: 29\ncpu: 24.52805400000004 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Create<3, 2, true>/1000000",
            "value": 277.7269630000016,
            "unit": "ms/iter",
            "extra": "iterations: 3\ncpu: 276.37166900000426 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10",
            "value": 0.001558150026259119,
            "unit": "ms/iter",
            "extra": "iterations: 449368\ncpu: 0.0015581374931014462 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/20",
            "value": 0.0030271808472441893,
            "unit": "ms/iter",
            "extra": "iterations: 231173\ncpu: 0.0030271541616019473 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/50",
            "value": 0.006247107281413615,
            "unit": "ms/iter",
            "extra": "iterations: 111352\ncpu: 0.006246895789927334 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/100",
            "value": 0.011580705743238159,
            "unit": "ms/iter",
            "extra": "iterations: 60889\ncpu: 0.011580034718914653 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/1000",
            "value": 0.1273884093801111,
            "unit": "ms/iter",
            "extra": "iterations: 5501\ncpu: 0.1273836636975092 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<0>/10000",
            "value": 1.089648686821744,
            "unit": "ms/iter",
            "extra": "iterations: 645\ncpu: 1.089615975193802 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10",
            "value": 0.002548369391896278,
            "unit": "ms/iter",
            "extra": "iterations: 277946\ncpu: 0.0025482701388039315 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/20",
            "value": 0.007081447428970899,
            "unit": "ms/iter",
            "extra": "iterations: 98657\ncpu: 0.0070812938159481465 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/50",
            "value": 0.01753749164050081,
            "unit": "ms/iter",
            "extra": "iterations: 37263\ncpu: 0.0175361599173442 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/100",
            "value": 0.037805394518630735,
            "unit": "ms/iter",
            "extra": "iterations: 18572\ncpu: 0.03780473040060336 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/1000",
            "value": 0.39418578741496657,
            "unit": "ms/iter",
            "extra": "iterations: 1764\ncpu: 0.39417552154195323 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertToLeaf<2>/10000",
            "value": 3.2424744027776624,
            "unit": "ms/iter",
            "extra": "iterations: 216\ncpu: 3.2423801388889184 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10",
            "value": 0.0005540129251748788,
            "unit": "ms/iter",
            "extra": "iterations: 1263039\ncpu: 0.0005540039634563947 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/20",
            "value": 0.0017570747803018075,
            "unit": "ms/iter",
            "extra": "iterations: 398046\ncpu: 0.001757044916919149 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/50",
            "value": 0.0030732586746519235,
            "unit": "ms/iter",
            "extra": "iterations: 229087\ncpu: 0.003073137310279521 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/100",
            "value": 0.007071647236933588,
            "unit": "ms/iter",
            "extra": "iterations: 98749\ncpu: 0.007071454161561031 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/1000",
            "value": 0.0654838825572628,
            "unit": "ms/iter",
            "extra": "iterations: 10652\ncpu: 0.06548151633495979 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<0>/10000",
            "value": 0.7630315578602817,
            "unit": "ms/iter",
            "extra": "iterations: 916\ncpu: 0.7629996757641802 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10",
            "value": 0.0016829151142646462,
            "unit": "ms/iter",
            "extra": "iterations: 416183\ncpu: 0.001682885781014573 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/20",
            "value": 0.0033843583846880514,
            "unit": "ms/iter",
            "extra": "iterations: 206895\ncpu: 0.003384235950602976 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/50",
            "value": 0.008802096872760208,
            "unit": "ms/iter",
            "extra": "iterations: 79527\ncpu: 0.008801941667609824 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/100",
            "value": 0.019401507617862558,
            "unit": "ms/iter",
            "extra": "iterations: 36165\ncpu: 0.0194008563804783 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/1000",
            "value": 0.24011579266621638,
            "unit": "ms/iter",
            "extra": "iterations: 2918\ncpu: 0.24011372001370876 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::InsertWithRebalancing<2>/10000",
            "value": 2.2616568612904535,
            "unit": "ms/iter",
            "extra": "iterations: 310\ncpu: 2.261578358064524 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10",
            "value": 0.0005367060530930888,
            "unit": "ms/iter",
            "extra": "iterations: 1304011\ncpu: 0.0005366839727578999 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/20",
            "value": 0.0012335155148295424,
            "unit": "ms/iter",
            "extra": "iterations: 567586\ncpu: 0.001233480595011139 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/50",
            "value": 0.0037458510630282217,
            "unit": "ms/iter",
            "extra": "iterations: 185837\ncpu: 0.0037455983038899524 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/100",
            "value": 0.008339389859181676,
            "unit": "ms/iter",
            "extra": "iterations: 83938\ncpu: 0.00833931545902925 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/1000",
            "value": 0.21718462414649803,
            "unit": "ms/iter",
            "extra": "iterations: 3222\ncpu: 0.21717462414649538 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<0>/10000",
            "value": 14.277867979591965,
            "unit": "ms/iter",
            "extra": "iterations: 49\ncpu: 14.277041734693942 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10",
            "value": 0.00117474862911662,
            "unit": "ms/iter",
            "extra": "iterations: 595419\ncpu: 0.001174660815325013 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/20",
            "value": 0.004093784549366329,
            "unit": "ms/iter",
            "extra": "iterations: 171747\ncpu: 0.004093557372181166 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/50",
            "value": 0.057798917640606295,
            "unit": "ms/iter",
            "extra": "iterations: 18225\ncpu: 0.05779594710562398 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/100",
            "value": 0.1154309644114779,
            "unit": "ms/iter",
            "extra": "iterations: 13347\ncpu: 0.11542555960140874 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/1000",
            "value": 1.84500403303967,
            "unit": "ms/iter",
            "extra": "iterations: 454\ncpu: 1.8448998700440802 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::Update<2>/10000",
            "value": 67.4918961000003,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 67.48516930000079 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100",
            "value": 71.46447554042327,
            "unit": "ns/iter",
            "extra": "iterations: 9760247\ncpu: 71.46024716382614 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/1000",
            "value": 407.6137499861655,
            "unit": "ns/iter",
            "extra": "iterations: 1716409\ncpu: 407.5824474236575 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/10000",
            "value": 10966.700432278614,
            "unit": "ns/iter",
            "extra": "iterations: 64079\ncpu: 10966.09806644927 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<0>/100000",
            "value": 135515.17310664323,
            "unit": "ns/iter",
            "extra": "iterations: 5176\ncpu: 135506.16904945733 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100",
            "value": 56.559599608741976,
            "unit": "ns/iter",
            "extra": "iterations: 12370400\ncpu: 56.558168693009584 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/1000",
            "value": 151.14819352599253,
            "unit": "ns/iter",
            "extra": "iterations: 4617199\ncpu: 151.1452382710806 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/10000",
            "value": 1904.03573583477,
            "unit": "ns/iter",
            "extra": "iterations: 357988\ncpu: 1903.9779769154513 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::PickSearch<2>/100000",
            "value": 33872.315087755356,
            "unit": "ns/iter",
            "extra": "iterations: 20626\ncpu: 33871.81387569106 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100",
            "value": 79.77244115369372,
            "unit": "ns/iter",
            "extra": "iterations: 8853912\ncpu: 79.77040781521242 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/1000",
            "value": 394.8235210775721,
            "unit": "ns/iter",
            "extra": "iterations: 1771577\ncpu: 394.8172853903603 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/10000",
            "value": 11093.027660653557,
            "unit": "ns/iter",
            "extra": "iterations: 63086\ncpu: 11092.639682338578 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<0>/100000",
            "value": 131446.29675483273,
            "unit": "ns/iter",
            "extra": "iterations: 5331\ncpu: 131439.5897580165 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100",
            "value": 89.18824293053108,
            "unit": "ns/iter",
            "extra": "iterations: 7909471\ncpu: 89.18184844473095 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/1000",
            "value": 205.59452979719032,
            "unit": "ns/iter",
            "extra": "iterations: 3400203\ncpu: 205.59138145575486 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/10000",
            "value": 3141.351614494572,
            "unit": "ns/iter",
            "extra": "iterations: 222980\ncpu: 3141.2949143421324 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RangeSearch<2>/100000",
            "value": 38974.05598842462,
            "unit": "ns/iter",
            "extra": "iterations: 17968\ncpu: 38973.008570792816 ns\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/1000",
            "value": 0.002624548089443859,
            "unit": "ms/iter",
            "extra": "iterations: 267252\ncpu: 0.00262445283851952 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<0>/10000",
            "value": 0.060774072636138605,
            "unit": "ms/iter",
            "extra": "iterations: 11771\ncpu: 0.06077355135502433 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/1000",
            "value": 0.0022622179052693365,
            "unit": "ms/iter",
            "extra": "iterations: 310121\ncpu: 0.0022621393101402735 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::FrustumCulling<2>/10000",
            "value": 0.05882284539473823,
            "unit": "ms/iter",
            "extra": "iterations: 11856\ncpu: 0.05882065688259149 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100",
            "value": 0.0023636604891340896,
            "unit": "ms/iter",
            "extra": "iterations: 297260\ncpu: 0.0023635709412635035 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/1000",
            "value": 0.5228357636499815,
            "unit": "ms/iter",
            "extra": "iterations: 1337\ncpu: 0.5228191465968498 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/10000",
            "value": 38.93346422222206,
            "unit": "ms/iter",
            "extra": "iterations: 18\ncpu: 38.93176483333289 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, false>/100000",
            "value": 6175.281002999953,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6175.0719700000045 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100",
            "value": 0.005519309744954099,
            "unit": "ms/iter",
            "extra": "iterations: 126291\ncpu: 0.005518789660387624 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/1000",
            "value": 0.6157060584070448,
            "unit": "ms/iter",
            "extra": "iterations: 1130\ncpu: 0.6156680336283067 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/10000",
            "value": 40.139943588234715,
            "unit": "ms/iter",
            "extra": "iterations: 17\ncpu: 40.13824905882422 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<0, true>/100000",
            "value": 6263.710731000003,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6263.440056000008 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100",
            "value": 0.005823136548795252,
            "unit": "ms/iter",
            "extra": "iterations: 121517\ncpu: 0.0058228083889494886 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/1000",
            "value": 0.5846925658333172,
            "unit": "ms/iter",
            "extra": "iterations: 1200\ncpu: 0.58466943999998 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/10000",
            "value": 54.782834461535074,
            "unit": "ms/iter",
            "extra": "iterations: 13\ncpu: 54.78241361538494 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, false>/100000",
            "value": 20750.457109000025,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 20713.639379 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100",
            "value": 0.014037066954166979,
            "unit": "ms/iter",
            "extra": "iterations: 49855\ncpu: 0.01403635005515976 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/1000",
            "value": 0.6616346654099658,
            "unit": "ms/iter",
            "extra": "iterations: 1061\ncpu: 0.6615978944392016 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/10000",
            "value": 44.26343037500047,
            "unit": "ms/iter",
            "extra": "iterations: 16\ncpu: 44.26140412500068 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection<2, true>/100000",
            "value": 14711.133522000011,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 14663.232904000011 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100",
            "value": 0.005429195231216024,
            "unit": "ms/iter",
            "extra": "iterations: 130138\ncpu: 0.005428920515145293 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/1000",
            "value": 0.9786301904761794,
            "unit": "ms/iter",
            "extra": "iterations: 714\ncpu: 0.9785616442577227 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/10000",
            "value": 70.94528399999831,
            "unit": "ms/iter",
            "extra": "iterations: 10\ncpu: 70.94304860000022 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<0>/100000",
            "value": 6795.2622450000035,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 6794.896827000002 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100",
            "value": 0.022977144780705342,
            "unit": "ms/iter",
            "extra": "iterations: 30598\ncpu: 0.02297635087260564 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/1000",
            "value": 1.3741283031496045,
            "unit": "ms/iter",
            "extra": "iterations: 508\ncpu: 1.374064811023584 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/10000",
            "value": 88.69225725000263,
            "unit": "ms/iter",
            "extra": "iterations: 8\ncpu: 88.67872074999994 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::CollisionDetection_WithOtherTree<2>/100000",
            "value": 7906.543684000042,
            "unit": "ms/iter",
            "extra": "iterations: 1\ncpu: 7904.652683000023 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.0006317979756125338,
            "unit": "ms/iter",
            "extra": "iterations: 1105322\ncpu: 0.0006316355396888669 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.0039057336750838727,
            "unit": "ms/iter",
            "extra": "iterations: 178883\ncpu: 0.0039055516566694768 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.037447335559354214,
            "unit": "ms/iter",
            "extra": "iterations: 18718\ncpu: 0.03744633160594083 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.38570502418912095,
            "unit": "ms/iter",
            "extra": "iterations: 1819\ncpu: 0.3856989593182954 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.0004407028067027034,
            "unit": "ms/iter",
            "extra": "iterations: 1595894\ncpu: 0.0004406819688525672 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.00113898109320337,
            "unit": "ms/iter",
            "extra": "iterations: 614012\ncpu: 0.0011389719630886923 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.0073983959795386935,
            "unit": "ms/iter",
            "extra": "iterations: 94616\ncpu: 0.007398223661959832 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.0670042153375447,
            "unit": "ms/iter",
            "extra": "iterations: 10458\ncpu: 0.06699928944349001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.0006390453292085953,
            "unit": "ms/iter",
            "extra": "iterations: 1097769\ncpu: 0.0006390399892873598 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.004996406487405126,
            "unit": "ms/iter",
            "extra": "iterations: 140056\ncpu: 0.004996197106871525 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.05086845179625661,
            "unit": "ms/iter",
            "extra": "iterations: 13723\ncpu: 0.050867475041901526 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5261014573353088,
            "unit": "ms/iter",
            "extra": "iterations: 1336\ncpu: 0.5260824221557019 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.0004503621681847766,
            "unit": "ms/iter",
            "extra": "iterations: 1561675\ncpu: 0.00045035003890053467 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.002175850552152545,
            "unit": "ms/iter",
            "extra": "iterations: 324548\ncpu: 0.0021758335315577855 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.021270611318932552,
            "unit": "ms/iter",
            "extra": "iterations: 33148\ncpu: 0.021270242789912425 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.19071435760693473,
            "unit": "ms/iter",
            "extra": "iterations: 3694\ncpu: 0.1907127931781259 ms\nthreads: 1"
          }
        ]
      }
    ]
  }
}