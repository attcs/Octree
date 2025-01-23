window.BENCHMARK_DATA = {
  "lastUpdate": 1737590890035,
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
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.001009521263506736,
            "unit": "ms/iter",
            "extra": "iterations: 694100\ncpu: 0.0010095035153435965 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.006017530057609626,
            "unit": "ms/iter",
            "extra": "iterations: 116127\ncpu: 0.0060172673021777175 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.05598743515849794,
            "unit": "ms/iter",
            "extra": "iterations: 12492\ncpu: 0.05598153306115959 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.7464959908842035,
            "unit": "ms/iter",
            "extra": "iterations: 1097\ncpu: 0.746445029170455 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.001043626815952418,
            "unit": "ms/iter",
            "extra": "iterations: 677537\ncpu: 0.0010436064805316907 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.0018174389080827442,
            "unit": "ms/iter",
            "extra": "iterations: 386696\ncpu: 0.0018174182639592966 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.031693962284676684,
            "unit": "ms/iter",
            "extra": "iterations: 22060\ncpu: 0.03169121822302925 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.3811855233441929,
            "unit": "ms/iter",
            "extra": "iterations: 1842\ncpu: 0.3811653007600192 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.0008187252160606266,
            "unit": "ms/iter",
            "extra": "iterations: 854737\ncpu: 0.0008186716487060052 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.005732590388212704,
            "unit": "ms/iter",
            "extra": "iterations: 122433\ncpu: 0.005732174250406618 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.05245422655607941,
            "unit": "ms/iter",
            "extra": "iterations: 13383\ncpu: 0.05245317238287764 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5769607891036893,
            "unit": "ms/iter",
            "extra": "iterations: 1138\ncpu: 0.5768978884007139 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.0008320451206580255,
            "unit": "ms/iter",
            "extra": "iterations: 850586\ncpu: 0.0008319812247085998 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.003993176820512886,
            "unit": "ms/iter",
            "extra": "iterations: 175500\ncpu: 0.003992855566951369 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.005534207989002734,
            "unit": "ms/iter",
            "extra": "iterations: 126574\ncpu: 0.005534055279915583 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.2114179345000025,
            "unit": "ms/iter",
            "extra": "iterations: 10000\ncpu: 0.21140757610000377 ms\nthreads: 1"
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
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100",
            "value": 0.001002239813348904,
            "unit": "ms/iter",
            "extra": "iterations: 696058\ncpu: 0.0010021671872171669 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/1000",
            "value": 0.006102956575195655,
            "unit": "ms/iter",
            "extra": "iterations: 114681\ncpu: 0.0061027833642886735 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/10000",
            "value": 0.05542570258894547,
            "unit": "ms/iter",
            "extra": "iterations: 12592\ncpu: 0.05542455320838693 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<0>/100000",
            "value": 0.5917741420000198,
            "unit": "ms/iter",
            "extra": "iterations: 1000\ncpu: 0.5917250800000033 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100",
            "value": 0.0010365901666599955,
            "unit": "ms/iter",
            "extra": "iterations: 679647\ncpu: 0.0010364805612325145 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/1000",
            "value": 0.00335437342289309,
            "unit": "ms/iter",
            "extra": "iterations: 205598\ncpu: 0.003354017543944989 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/10000",
            "value": 0.03142133486109587,
            "unit": "ms/iter",
            "extra": "iterations: 24873\ncpu: 0.03141822811884386 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedFirst<2>/100000",
            "value": 0.35320552089783264,
            "unit": "ms/iter",
            "extra": "iterations: 2584\ncpu: 0.35318714473684487 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100",
            "value": 0.00042952988144128715,
            "unit": "ms/iter",
            "extra": "iterations: 1633790\ncpu: 0.0004295134858213069 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/1000",
            "value": 0.005689922230053319,
            "unit": "ms/iter",
            "extra": "iterations: 123441\ncpu: 0.005689658217285943 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/10000",
            "value": 0.051570398228783884,
            "unit": "ms/iter",
            "extra": "iterations: 13550\ncpu: 0.05156899594095903 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<0>/100000",
            "value": 0.5435868221113732,
            "unit": "ms/iter",
            "extra": "iterations: 1203\ncpu: 0.5435769418121367 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100",
            "value": 0.0008256186222047787,
            "unit": "ms/iter",
            "extra": "iterations: 856644\ncpu: 0.0008255954959119246 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/1000",
            "value": 0.003952510869809687,
            "unit": "ms/iter",
            "extra": "iterations: 177832\ncpu: 0.003952444520671218 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/10000",
            "value": 0.03532696610279432,
            "unit": "ms/iter",
            "extra": "iterations: 92515\ncpu: 0.035326230967950754 ms\nthreads: 1"
          },
          {
            "name": "Benchmarks::Box::RayIntersectedAll<2>/100000",
            "value": 0.36318420010368196,
            "unit": "ms/iter",
            "extra": "iterations: 1929\ncpu: 0.363172067392438 ms\nthreads: 1"
          }
        ]
      }
    ]
  }
}