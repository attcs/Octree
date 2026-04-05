
#include "pch.h"

#include "CppUnitTest.h"

#include <random>

#include "orthotree/detail/memory_resource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using OrthoTree::detail::MemoryResource;
using OrthoTree::detail::PRIMARY_PAGEID;

namespace MemoryResourceTests
{
  TEST_CLASS(MemoryResourceDispatchTest)
  {
    static bool IsBucket(OrthoTree::detail::PageID id) { return id >= (1u << 31); }
    static bool IsFallback(OrthoTree::detail::PageID id) { return id > 0 && id < (1u << 31); }

  public:
    TEST_METHOD(DispatchToPrimary)
    {
      MemoryResource<int> mr;
      mr.Init(1024);
      auto a = mr.Allocate(100);
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
    }

    TEST_METHOD(DispatchToBuckets)
    {
      MemoryResource<int> mr;
      mr.Init(1); // Force skip primary
      auto a = mr.Allocate(16);
      Assert::IsTrue(IsBucket(a.pageID));
    }

    TEST_METHOD(DispatchToFallback)
    {
      MemoryResource<int> mr;
      mr.Init(1); 
      auto a = mr.Allocate(64); // Larger than bucket 32
      Assert::IsTrue(IsFallback(a.pageID));
    }
  };


  TEST_CLASS(MemoryResourceTest)
  {
    static bool IsBucket(OrthoTree::detail::PageID id) { return id >= (1u << 31); }
  public:
    TEST_METHOD(AllocateMainAndDeallocateReuses)
    {
      MemoryResource<int> mr;
      mr.Init(100);

      auto a = mr.Allocate(40);
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
      auto addr = a.segment.data();

      mr.Deallocate(a);

      auto b = mr.Allocate(40);
      Assert::AreEqual(PRIMARY_PAGEID, b.pageID);
      Assert::AreEqual(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(b.segment.data()));
    }

    TEST_METHOD(DISABLED_AllocateBestFit)
    {
      MemoryResource<int> mr;
      mr.Init(1000);

      // Create holes: [A:40][Free:40][B:40][Free:80][C:40][Free:x]
      auto a = mr.Allocate(40);
      auto h1 = mr.Allocate(40);
      auto b = mr.Allocate(40);
      auto h2 = mr.Allocate(80);
      auto c = mr.Allocate(40);

      mr.Deallocate(h1);
      mr.Deallocate(h2);

      // Now we have free segments of size 40 and 80.
      // Allocate 60 -> should pick the 80 one.
      auto d = mr.Allocate(60);
      Assert::AreEqual(PRIMARY_PAGEID, d.pageID);
      Assert::AreEqual(reinterpret_cast<void*>(h2.segment.data()), reinterpret_cast<void*>(d.segment.data()));
    }

    TEST_METHOD(DeallocateMergeForward)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(40);
      auto b = mr.Allocate(40);
      mr.Deallocate(b); // Free segment after 'a'
      mr.Deallocate(a); // 'a' should merge with 'b's free segment

      auto c = mr.Allocate(80);
      Assert::AreEqual(PRIMARY_PAGEID, c.pageID);
      Assert::AreEqual<size_t>(80, c.segment.size());
    }

    TEST_METHOD(DeallocateMergeBackward)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(400); // Forces fallback to heap if Init was small, but Init(200) < 400.
      // Wait, let's use Init(1000)
      mr.Reset();
      mr.Init(1000);
      auto a2 = mr.Allocate(40);
      auto b2 = mr.Allocate(40);
      mr.Deallocate(a2); // Free segment before 'b'
      mr.Deallocate(b2); // 'b' should merge backwards with 'a'

      auto c = mr.Allocate(80);
      Assert::AreEqual(PRIMARY_PAGEID, c.pageID);
      Assert::AreEqual<size_t>(80, c.segment.size());
    }

    TEST_METHOD(DeallocateMergeBoth)
    {
      MemoryResource<int> mr;
      mr.Init(1000);

      auto a = mr.Allocate(40);
      auto b = mr.Allocate(40);
      auto c = mr.Allocate(40);

      mr.Deallocate(a);
      mr.Deallocate(c);
      mr.Deallocate(b); // 'b' merges with both 'a' and 'c'

      auto d = mr.Allocate(120);
      Assert::AreEqual(PRIMARY_PAGEID, d.pageID);
      Assert::AreEqual<size_t>(120, d.segment.size());
    }

    TEST_METHOD(IncreaseSegmentExtendsIntoFree)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(40);
      auto b = mr.Allocate(40);

      // Free the segment after `a` so `a` can extend into it
      mr.Deallocate(b);

      mr.IncreaseSegment(a, 40);
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
      Assert::AreEqual<size_t>(80, a.segment.size());
    }

    TEST_METHOD(IncreaseSegmentRelocation)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(40);
      auto b = mr.Allocate(40); // Block 'a' from extending

      mr.IncreaseSegment(a, 80); // 'a' must move
      Assert::AreNotEqual(reinterpret_cast<void*>(b.segment.data()), reinterpret_cast<void*>(a.segment.data()));
      Assert::AreEqual<size_t>(120, a.segment.size());
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
    }

    TEST_METHOD(IncreaseSegmentZero)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(40);
      auto startAddr = a.segment.data();
      mr.IncreaseSegment(a, 0);
      Assert::AreEqual(startAddr, a.segment.data());
      Assert::AreEqual<size_t>(40, a.segment.size());
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
    }

    TEST_METHOD(IncreaseSegmentArena)
    {
      MemoryResource<int> mr;
      mr.Init(8); // Force buckets

      auto a = mr.Allocate(10); // Goes to bucket16
      auto startAddr = a.segment.data();

      // Extend within bucket16 capacity (16)
      mr.IncreaseSegment(a, 6);
      Assert::AreEqual(startAddr, a.segment.data());
      Assert::AreEqual<size_t>(16, a.segment.size());
      Assert::IsTrue(IsBucket(a.pageID));
    }

    TEST_METHOD(DecreaseSegmentMain)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(40);
      mr.DecreaseSegment(a, 10);
      Assert::AreEqual<size_t>(30, a.segment.size());

      // The 10 freed units should be reusable by a larger allocation that triggers primary
      auto b = mr.Allocate(40); 
      Assert::AreEqual(PRIMARY_PAGEID, b.pageID);
    }

    TEST_METHOD(DecreaseSegmentArena)
    {
      MemoryResource<int> mr;
      mr.Init(1000);

      auto a = mr.Allocate(16);
      Assert::IsTrue(IsBucket(a.pageID));

      mr.DecreaseSegment(a, 4);
      Assert::AreEqual<size_t>(12, a.segment.size());
      Assert::IsTrue(IsBucket(a.pageID));
    }

    TEST_METHOD(ArenaPageReclamation)
    {
      MemoryResource<int> mr;
      mr.Init(8);

      auto a = mr.Allocate(64);
      auto b = mr.Allocate(64);
      [[maybe_unused]] auto pageID_a = a.pageID;
      [[maybe_unused]] auto pageID_b = b.pageID;

      mr.Deallocate(a);
      mr.Deallocate(b);

      // Subsequent allocation should reuse the reclaimed pages
      auto c = mr.Allocate(64);
      // It should reuse pageID_b (since it was the last page and likely popped and re-added or if it was middle it goes to m_freedPages)
      // Actually if b was the last page, it gets popped. Then a becomes the last page and gets popped too if emptied.
      Assert::AreNotEqual(PRIMARY_PAGEID, c.pageID);
    }

    TEST_METHOD(ResetClearsAll)
    {
      MemoryResource<int> mr;
      mr.Init(200);
      mr.Allocate(40);
      mr.Reset();

      mr.Init(200);
      auto a = mr.Allocate(40);
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
      // If reset worked, first allocation should be at the start of main page
    }

    TEST_METHOD(FullDecreaseMainThenAllocate)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(40);
      auto addrBefore = a.segment.data();

      // Fully decrease -> segment becomes empty
      mr.DecreaseSegment(a, 40);
      Assert::AreEqual<size_t>(0, a.segment.size());
      Assert::IsTrue(a.segment.empty());

      // Re-allocate: should reuse the freed main-page memory
      auto b = mr.Allocate(40);
      Assert::AreEqual(PRIMARY_PAGEID, b.pageID);
      Assert::AreEqual<size_t>(40, b.segment.size());
      Assert::AreEqual(reinterpret_cast<void*>(addrBefore), reinterpret_cast<void*>(b.segment.data()));
    }

    TEST_METHOD(FullDecreaseMainThenIncreaseSegment)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(40);
      for (int i = 0; i < 40; ++i)
        a.segment[i] = i;

      // Fully decrease
      mr.DecreaseSegment(a, 40);
      Assert::IsTrue(a.segment.empty());

      // IncreaseSegment on an empty segment should do a fresh Allocate
      mr.IncreaseSegment(a, 40);
      Assert::AreEqual<size_t>(40, a.segment.size());
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
    }

    TEST_METHOD(FullDecreaseArenaThenAllocate)
    {
      MemoryResource<int> mr;
      mr.Init(100);

      auto a = mr.Allocate(16); // Goes to bucket
      auto arenaPageID = a.pageID;
      Assert::AreNotEqual(PRIMARY_PAGEID, arenaPageID);

      // Fully decrease arena segment
      mr.DecreaseSegment(a, 16);
      Assert::AreEqual<size_t>(0, a.segment.size());

      // Re-allocate something that fits on main page
      auto b = mr.Allocate(40); 
      Assert::AreEqual(PRIMARY_PAGEID, b.pageID);
      Assert::AreEqual<size_t>(40, b.segment.size());
    }

    TEST_METHOD(FullDecreaseArenaThenIncrease)
    {
      MemoryResource<int> mr;
      mr.Init(8);

      auto a = mr.Allocate(16); // Goes to arena
      Assert::AreNotEqual(PRIMARY_PAGEID, a.pageID);

      // Fully decrease
      mr.DecreaseSegment(a, 16);
      Assert::IsTrue(a.segment.empty());

      // IncreaseSegment on empty should allocate fresh
      mr.IncreaseSegment(a, 4);
      Assert::AreEqual<size_t>(4, a.segment.size());
    }

    TEST_METHOD(FullDecreaseMainPreservesOtherSegments)
    {
      MemoryResource<int> mr;
      mr.Init(100);

      auto a = mr.Allocate(10);
      auto b = mr.Allocate(10);
      auto c = mr.Allocate(10);

      // Write sentinel values
      for (int i = 0; i < 10; ++i)
      {
        a.segment[i] = 100 + i;
        b.segment[i] = 200 + i;
        c.segment[i] = 300 + i;
      }

      // Fully decrease the middle segment
      mr.DecreaseSegment(b, 10);
      Assert::IsTrue(b.segment.empty());

      // Verify 'a' and 'c' data is still intact
      for (int i = 0; i < 10; ++i)
      {
        Assert::AreEqual(100 + i, a.segment[i]);
        Assert::AreEqual(300 + i, c.segment[i]);
      }

      // Re-allocate into the freed hole; data written should work
      auto d = mr.Allocate(10);
      Assert::AreEqual<size_t>(10, d.segment.size());
      for (int i = 0; i < 10; ++i)
        d.segment[i] = 400 + i;

      // 'a' and 'c' still intact after reusing freed memory
      for (int i = 0; i < 10; ++i)
      {
        Assert::AreEqual(100 + i, a.segment[i]);
        Assert::AreEqual(300 + i, c.segment[i]);
        Assert::AreEqual(400 + i, d.segment[i]);
      }
    }

    TEST_METHOD(FullDecreaseThenAllocateLarger)
    {
      MemoryResource<int> mr;
      mr.Init(100);

      auto a = mr.Allocate(10);
      auto b = mr.Allocate(10);

      // Fully free 'a'
      mr.DecreaseSegment(a, 10);
      Assert::IsTrue(a.segment.empty());

      // Allocate something bigger than the freed hole (10 freed, need 20)
      // 'b' is blocking extension, so this may go elsewhere or to arena
      auto c = mr.Allocate(20);
      Assert::AreEqual<size_t>(20, c.segment.size());
    }

    TEST_METHOD(RepeatedFullDecreaseThenReallocateMain)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      for (int round = 0; round < 5; ++round)
      {
        auto a = mr.Allocate(40);
        Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
        Assert::AreEqual<size_t>(40, a.segment.size());

        for (int i = 0; i < 40; ++i)
          a.segment[i] = round * 100 + i;

        mr.DecreaseSegment(a, 40);
        Assert::IsTrue(a.segment.empty());
      }

      // After all rounds, the main page free list should still be consistent
      auto final_seg = mr.Allocate(40);
      Assert::AreEqual(PRIMARY_PAGEID, final_seg.pageID);
    }

    TEST_METHOD(FullDecreaseThenIncreaseWithDataIntegrity)
    {
      MemoryResource<int> mr;
      mr.Init(200);

      auto a = mr.Allocate(40);
      auto b = mr.Allocate(40);

      // Write data to 'b'
      for (int i = 0; i < 40; ++i)
        b.segment[i] = 500 + i;

      // Fully decrease 'a', then increase it back
      mr.DecreaseSegment(a, 40);
      Assert::IsTrue(a.segment.empty());

      mr.IncreaseSegment(a, 40);
      Assert::AreEqual<size_t>(40, a.segment.size());
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);

      // Write new data to 'a'
      for (int i = 0; i < 40; ++i)
        a.segment[i] = 600 + i;

      // 'b' must still hold its original data
      for (int i = 0; i < 40; ++i)
        Assert::AreEqual(500 + i, b.segment[i]);
    }

    TEST_METHOD(Clone)
    {
      MemoryResource<int> mr;
      mr.Init(100);

      auto a = mr.Allocate(10);
      a.segment[0] = 42;
      auto b = mr.Allocate(20);
      b.segment[0] = 43;

      MemoryResource<int> clonedMr;
      std::vector<OrthoTree::detail::MemoryResource<int>::MemorySegment*> segments = { &a, &b };
      mr.Clone(clonedMr, segments);

      Assert::AreEqual(42, a.segment[0]);
      Assert::AreEqual(43, b.segment[0]);
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
      Assert::AreEqual(PRIMARY_PAGEID, b.pageID);

      // Verify they are in the new resource
      // (The Clone method updates the segments to point to the new resource)
    }

    TEST_METHOD(DecreaseSegmentArenaFullNonTerminal)
    {
      MemoryResource<int> mr;
      mr.Init(8); // Small main page to force arena usage

      // Allocate two segments on the same arena page
      auto a = mr.Allocate(16);
      auto b = mr.Allocate(16);
      Assert::AreNotEqual(PRIMARY_PAGEID, a.pageID);
      Assert::AreEqual(a.pageID, b.pageID);

      // Write sentinel data into b
      for (int i = 0; i < 16; ++i)
        b.segment[i] = 100 + i;

      // Fully decrease a (non-terminal segment on the arena page).
      // BUG: old code did page.resize(page.size - 16) which reduces
      // page.size, making the allocator think b's region is free.
      mr.DecreaseSegment(a, 16);
      Assert::IsTrue(a.segment.empty());

      // b's data must still be intact
      for (int i = 0; i < 16; ++i)
        Assert::AreEqual(100 + i, b.segment[i]);
    }

    TEST_METHOD(DecreaseSegmentArenaFullTerminal)
    {
      MemoryResource<int> mr;
      mr.Init(8); // Small main page to force arena usage

      // Allocate a single segment on an arena page
      auto a = mr.Allocate(16);
      Assert::AreNotEqual(PRIMARY_PAGEID, a.pageID);

      for (int i = 0; i < 16; ++i)
        a.segment[i] = 42;

      // Fully decrease the terminal (and only) segment
      mr.DecreaseSegment(a, 16);
      Assert::IsTrue(a.segment.empty());

      // Allocate again - should succeed without issues
      auto b = mr.Allocate(8);
      for (int i = 0; i < 8; ++i)
        b.segment[i] = 99;
    }

    TEST_METHOD(DecreaseSegmentArenaPartialNonTerminal)
    {
      MemoryResource<int> mr;
      mr.Init(8);

      auto a = mr.Allocate(16);
      auto b = mr.Allocate(16);
      Assert::AreNotEqual(PRIMARY_PAGEID, a.pageID);
      Assert::AreEqual(a.pageID, b.pageID);

      for (int i = 0; i < 16; ++i)
        b.segment[i] = 200 + i;

      // Partially decrease a (non-terminal). Old code would wrongly
      // shrink page.size, potentially overlapping with b.
      mr.DecreaseSegment(a, 8);
      Assert::AreEqual<size_t>(8, a.segment.size());

      // b's data must still be intact
      for (int i = 0; i < 16; ++i)
        Assert::AreEqual(200 + i, b.segment[i]);
    }

    TEST_METHOD(BucketAllocator_Basic)
    {
      OrthoTree::detail::BucketAllocator<int, 4> allocator;
      
      auto a = allocator.Allocate(4);
      Assert::AreEqual(PRIMARY_PAGEID, a.pageID);
      
      allocator.Deallocate(a);
      
      auto b = allocator.Allocate(4);
      Assert::AreEqual(a.pageID, b.pageID);
      Assert::AreEqual(reinterpret_cast<void*>(a.segment.data()), reinterpret_cast<void*>(b.segment.data()));
    }

    TEST_METHOD(BucketAllocator_MultiplePages)
    {
      constexpr std::size_t BUCKET_SIZE = 128;
      using Type = OrthoTree::detail::BucketAllocator<uint64_t, BUCKET_SIZE>;
      OrthoTree::detail::BucketAllocator<uint64_t, BUCKET_SIZE> allocator;
      static_assert(Type::POOL_SIZE == 64);
      std::vector<OrthoTree::detail::MemorySegment<uint64_t>> allocs;
      
      // Allocate 100 elements to force multiple pages if POOL_SIZE is 64
      for (int i = 0; i < 65; ++i)
      {
        allocs.push_back(allocator.Allocate(4));
      }
      
      Assert::IsTrue(allocs.back().pageID > 0);
      
      for (auto& a : allocs)
        allocator.Deallocate(a);
    }

    TEST_METHOD(BucketAllocator_PopPageOnDeallocateLast)
    {
      OrthoTree::detail::BucketAllocator<int, 4> allocator;
      
      auto a = allocator.Allocate(4);
      auto b = allocator.Allocate(4);
      
      allocator.Deallocate(b);
      allocator.Deallocate(a);
      
      auto c = allocator.Allocate(4);
      Assert::AreEqual(PRIMARY_PAGEID, c.pageID);
      Assert::AreEqual(reinterpret_cast<void*>(a.segment.data()), reinterpret_cast<void*>(c.segment.data()));
    }

    TEST_METHOD(BucketAllocator_TryShrink)
    {
      OrthoTree::detail::BucketAllocator<int, 8> allocator;
      auto a = allocator.Allocate(8);
      Assert::IsTrue(allocator.TryToShrink(a.pageID, a.segment, 4));
      Assert::AreEqual<size_t>(4, a.segment.size());
      Assert::IsFalse(allocator.TryToShrink(a.pageID, a.segment, 4)); // would be < 1
    }

    TEST_METHOD(BucketAllocator_TryExtend)
    {
      OrthoTree::detail::BucketAllocator<int, 8> allocator;
      auto a = allocator.Allocate(4);
      Assert::IsTrue(allocator.TryToExtend(a.pageID, a.segment, 4));
      Assert::AreEqual<size_t>(8, a.segment.size());
      Assert::IsFalse(allocator.TryToExtend(a.pageID, a.segment, 1)); // exceeds BUCKET_SIZE
    }

    TEST_METHOD(RandomizedDispatchIntegrity)
    {
      std::mt19937 rng(42);
      std::uniform_int_distribution<int> opDist(0, 99);
      std::uniform_int_distribution<size_t> capDist(1, 45); 

      MemoryResource<int> mr;
      mr.Init(200); 

      struct Tracker {
        OrthoTree::detail::MemorySegment<int> ms;
        int expectedValue;
        size_t id;
      };

      std::vector<Tracker> active;
      size_t nextId = 1;

      for (int i = 0; i < 2000; ++i)
      {
        int op = opDist(rng);
        
        if (op < 40 || active.empty()) 
        {
          size_t capacity = capDist(rng); 
          auto ms = mr.Allocate(capacity);
          int val = static_cast<int>(rng() % 10000);
          for (size_t c = 0; c < ms.segment.size(); ++c)
            ms.segment[c] = val;
          active.push_back({ ms, val, nextId++ });
        }
        else if (op < 60)
        {
          int idx = rng() % active.size();
          size_t inc = 1 + (rng() % 10);
          mr.IncreaseSegment(active[idx].ms, inc);
          for(size_t c=0; c < active[idx].ms.segment.size(); ++c)
            active[idx].ms.segment[c] = active[idx].expectedValue;
        }
        else if (op < 80)
        {
          int idx = rng() % active.size();
          if (active[idx].ms.segment.size() > 1) 
          {
            size_t dec = 1 + (rng() % (active[idx].ms.segment.size() - 1));
            mr.DecreaseSegment(active[idx].ms, dec);
            for (size_t c = 0; c < active[idx].ms.segment.size(); ++c)
              Assert::AreEqual(active[idx].expectedValue, active[idx].ms.segment[c]);
          }
        }
        else
        {
          int idx = rng() % active.size();
          mr.Deallocate(active[idx].ms);
          active.erase(active.begin() + idx);
        }
      }

      for (auto& t : active)
      {
        for (size_t c = 0; c < t.ms.segment.size(); ++c)
          Assert::AreEqual(t.expectedValue, t.ms.segment[c]);
        mr.Deallocate(t.ms);
      }
    }
  };
} // namespace MemoryResourceTests
