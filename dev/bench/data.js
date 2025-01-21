window.BENCHMARK_DATA = {
  "lastUpdate": 1737502818293,
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
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.0010114303615341167,
            "unit": "ms/iter",
            "extra": "iterations: 692936\ncpu: 0.0010114291983675289 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.0060298060475795865,
            "unit": "ms/iter",
            "extra": "iterations: 115848\ncpu: 0.00602941718458651 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.05527085816833935,
            "unit": "ms/iter",
            "extra": "iterations: 12677\ncpu: 0.05526690147511227 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.5439695458368706,
            "unit": "ms/iter",
            "extra": "iterations: 1189\ncpu: 0.5439304062237001 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.0010383953088055583,
            "unit": "ms/iter",
            "extra": "iterations: 673304\ncpu: 0.0010383854811496626 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.003406407226472905,
            "unit": "ms/iter",
            "extra": "iterations: 207155\ncpu: 0.0034063260601964866 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.03123453558822296,
            "unit": "ms/iter",
            "extra": "iterations: 22381\ncpu: 0.031231658281578176 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.35133872522898835,
            "unit": "ms/iter",
            "extra": "iterations: 198700\ncpu: 0.35132979735279307 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.0008195836124745225,
            "unit": "ms/iter",
            "extra": "iterations: 854406\ncpu: 0.0008195648029157286 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.005643966324375828,
            "unit": "ms/iter",
            "extra": "iterations: 124066\ncpu: 0.005643852554285722 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.05159658206853424,
            "unit": "ms/iter",
            "extra": "iterations: 12782\ncpu: 0.05159687052104583 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5448122687266221,
            "unit": "ms/iter",
            "extra": "iterations: 1068\ncpu: 0.5447911039325799 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.0008238704794447589,
            "unit": "ms/iter",
            "extra": "iterations: 852799\ncpu: 0.0008238370413192614 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.004031556677475439,
            "unit": "ms/iter",
            "extra": "iterations: 173314\ncpu: 0.004031468081055302 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.005428114417957655,
            "unit": "ms/iter",
            "extra": "iterations: 126606\ncpu: 0.00542803960317849 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.36209813193014007,
            "unit": "ms/iter",
            "extra": "iterations: 1948\ncpu: 0.362086539014371 ms\nthreads: 1"
          }
        ]
      }
    ]
  }
}