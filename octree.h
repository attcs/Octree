#ifndef N_ARY_TREE_GUARD
#define N_ARY_TREE_GUARD

#include <array>
#include <bitset>
#include <optional>
#include <span>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <numeric>
#include <concepts>

#include <assert.h>


#ifndef autoc
#define autoc auto const
#define undef_autoc
#endif

#ifndef autoce
#define autoce auto constexpr
#define undef_autoce
#endif

namespace NTree
{
  constexpr int pow_ce(int a, unsigned char e) { return e == 0 ? 1 : a * pow_ce(a, e - 1); }

  namespace
  {
    using std::array;
    using std::bitset;
    using std::span;
    using std::vector;
    using std::unordered_map;
    using std::queue;
  }

  // Morton Z-order id
  using location_id_type = uint32_t;
  
  // Grid id
  using grid_id_type = uint32_t;

  // Type of the dimension
  using dim_type = uint8_t;
  
  using bucket_id_type = uint8_t;

  using max_noelement = uint16_t;


  template<dim_type nDimension, typename geometry_type = double>
  using Point = array<geometry_type, nDimension>;

  template <dim_type nDimension, typename geometry_type = double>
  struct BoundingBox
  {
    Point<nDimension, geometry_type> Min;
    Point<nDimension, geometry_type> Max;
  };


  // Adaptor concepts

  template <class adaptor_type, typename point_type, typename box_type, typename geometry_type = double>
  concept AdaptorBasicsConcept =
    requires (point_type & pt, dim_type iDimension) { {adaptor_type::point_comp(pt, iDimension)}->std::convertible_to<geometry_type&>; }
    &&requires (point_type const& pt, dim_type iDimension) { {adaptor_type::point_comp_c(pt, iDimension)}->std::convertible_to<geometry_type const&>; }
    && requires (box_type & box) { { adaptor_type::box_min(box) }->std::convertible_to<point_type &>; }
    && requires (box_type & box) { { adaptor_type::box_max(box) }->std::convertible_to<point_type &>; }
    && requires (box_type const& box) { { adaptor_type::box_min_c(box) }->std::convertible_to<point_type const&>; }
    && requires (box_type const& box) { { adaptor_type::box_max_c(box) }->std::convertible_to<point_type const&>; }
  ;

  template <class adaptor_type, typename point_type, typename box_type, typename geometry_type = double>
  concept AdaptorConcept =
    requires { AdaptorBasicsConcept<adaptor_type, point_type, box_type, geometry_type>; }
    && requires (box_type const& box, point_type const& pt) { { adaptor_type::does_box_contain_point(box, pt)}->std::convertible_to<bool>; }
    && requires (box_type const& e1, box_type const& e2, bool e1_must_contain_e2) { { adaptor_type::are_boxes_overlapped(e1, e2, e1_must_contain_e2)}->std::convertible_to<bool>; }
    && requires (span<point_type const> const& vPoint) { { adaptor_type::box_of_points(vPoint)}->std::convertible_to<box_type>; }
    && requires (span<box_type const> const& vBox) { { adaptor_type::box_of_boxes(vBox)}->std::convertible_to<box_type>; }
  ;


  // Adaptors

  template <dim_type nDimension, typename point_type, typename box_type, typename geometry_type = double>
  struct AdaptorGeneralBasics
  {
    static constexpr geometry_type& point_comp(point_type& pt, dim_type iDimension) { return pt[iDimension]; }
    static constexpr geometry_type const& point_comp_c(point_type const& pt, dim_type iDimension) { return pt[iDimension]; }

    static constexpr point_type& box_min(box_type& box) { return box.Min; }
    static constexpr point_type& box_max(box_type& box) { return box.Max; }
    static constexpr point_type const& box_min_c(box_type const& box) { return box.Min; }
    static constexpr point_type const& box_max_c(box_type const& box) { return box.Max; }
  };


  template <dim_type nDimension, typename point_type, typename box_type, typename adaptor_basics_type, typename geometry_type = double>
  struct AdaptorGeneralBase : adaptor_basics_type
  {
    using base = adaptor_basics_type;
    static_assert(AdaptorBasicsConcept<base, point_type, box_type, geometry_type>);


    static constexpr bool does_box_contain_point(box_type const& box, point_type const& pt)
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (!(base::point_comp_c(base::box_min_c(box), iDimension) <= base::point_comp_c(pt, iDimension) && base::point_comp_c(pt, iDimension) <= base::point_comp_c(base::box_max_c(box), iDimension)))
          return false;

      return true;
    }

    static constexpr bool does_box_contain_point_strict(box_type const& box, point_type const& pt)
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (!(base::point_comp_c(base::box_min_c(box), iDimension) < base::point_comp_c(pt, iDimension) && base::point_comp_c(pt, iDimension) < base::point_comp_c(base::box_max_c(box), iDimension)))
          return false;

      return true;
    }

    static constexpr bool are_boxes_overlapped(box_type const& e1, box_type const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false)
    {
      return e1_must_contain_e2
        ? does_box_contain_point(e1, base::box_min_c(e2)) && does_box_contain_point(e1, base::box_max_c(e2))
        : fOverlapPtTouchAllowed 
          ? does_box_contain_point(e1, base::box_min_c(e2)) || does_box_contain_point(e1, base::box_max_c(e2)) || does_box_contain_point(e2, base::box_max_c(e1))
          : does_box_contain_point_strict(e1, base::box_min_c(e2)) || does_box_contain_point_strict(e1, base::box_max_c(e2)) || are_boxes_overlapped(e1, e2) || are_boxes_overlapped(e2, e1)
        ;
    }

    static consteval box_type box_inverted_init()
    {
      auto ext = box_type{};
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        base::point_comp(base::box_min(ext), iDimension) = +std::numeric_limits<geometry_type>::infinity();
        base::point_comp(base::box_max(ext), iDimension) = -std::numeric_limits<geometry_type>::infinity();
      }

      return ext;
    }

    static box_type box_of_points(span<point_type const> const& vPoint)
    {
      autoce extC = box_inverted_init();
      auto ext = extC;
      for (autoc& pt : vPoint)
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          if (base::point_comp_c(base::box_min_c(ext), iDimension) > base::point_comp_c(pt, iDimension))
            base::point_comp(base::box_min(ext), iDimension) = base::point_comp_c(pt, iDimension);

          if (base::point_comp_c(base::box_max_c(ext), iDimension) < base::point_comp_c(pt, iDimension))
            base::point_comp(base::box_max(ext), iDimension) = base::point_comp_c(pt, iDimension);
        }

      return ext;
    }

    static box_type box_of_boxes(span<box_type const> const& vExtent)
    {
      autoce extC = box_inverted_init();
      auto ext = extC;
      for (autoc& e : vExtent)
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          if (base::point_comp_c(base::box_min_c(ext), iDimension) > base::point_comp_c(base::box_min_c(e), iDimension))
            base::point_comp(base::box_min(ext), iDimension) = base::point_comp_c(base::box_min_c(e), iDimension);

          if (base::point_comp_c(base::box_max_c(ext), iDimension) < base::point_comp_c(base::box_max_c(e), iDimension))
            base::point_comp(base::box_max(ext), iDimension) = base::point_comp_c(base::box_max_c(e), iDimension);
        }

      return ext;
    }
  };

  template<dim_type nDimension, typename point_type, typename box_type, typename geometry_type = double>
  using AdaptorGeneral = AdaptorGeneralBase<nDimension, point_type, box_type, AdaptorGeneralBasics<nDimension, point_type, box_type, geometry_type>, geometry_type>;
  

  // Grid
  
  template <dim_type nDimension, typename point_type, typename adaptor_type>
  constexpr array<double, nDimension> resolve_grid_get_rasterizer(point_type const& p0, point_type const& p1, uint16_t n_divide)
  {
    auto aRasterizer = array<double, nDimension>{};
    for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      aRasterizer[iDimension] = static_cast<double>(n_divide) / (adaptor_type::point_comp_c(p1, iDimension) - adaptor_type::point_comp_c(p0, iDimension));

    return aRasterizer;
  }

  template <dim_type nDimension, typename point_type, typename adaptor_type>
  constexpr array<grid_id_type, nDimension> resolve_grid_id_point(point_type const& pe, point_type const& p0, point_type const& p1, array<double, nDimension> const& aRasterizer, uint16_t maxSlot)
  {
    auto aid = array<grid_id_type, nDimension>{};
    for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      aid[iDimension] = std::min<grid_id_type>(maxSlot, static_cast<grid_id_type>(static_cast<double>(adaptor_type::point_comp_c(pe, iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * aRasterizer[iDimension]));

    return aid;
  }

  template <dim_type nDimension, typename point_type, typename adaptor_type>
  constexpr array<grid_id_type, nDimension> resolve_grid_id_point_limit(point_type const& pe, point_type const& p0, point_type const& p1, array<double, nDimension> const& aRasterizer)
  {
    auto aid = array<grid_id_type, nDimension>{};
    for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
    {
      autoc id = static_cast<double>(adaptor_type::point_comp_c(pe, iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * aRasterizer[iDimension];
      aid[iDimension] = static_cast<grid_id_type>(id == floor(id) ? id - 1 : id);
    }
    return aid;
  }


  template <dim_type nDimension, typename point_type, typename adaptor_type>
  auto resolve_grid_id(span<point_type const> const& points, point_type const& p0, point_type const& p1, uint16_t n_divide)
  {
    autoc aRasterizer = resolve_grid_get_rasterizer<nDimension, point_type, adaptor_type>(p0, p1, n_divide);

    autoc maxSlot = n_divide - 1;

    autoc n = points.size();
    auto aid = vector<array<grid_id_type, nDimension>>(n);
    for (size_t i = 0; i < n; ++i)
      aid[i] = resolve_grid_id_point<nDimension, point_type, adaptor_type>(points[i], p0, p1, aRasterizer, maxSlot);

    return aid;
  }


  template <dim_type nDimension, typename box_type, typename point_type, typename adaptor_type>
  auto resolve_grid_id(span<box_type const> const& extents, point_type const& p0, point_type const& p1, uint16_t n_divide)
  {
    autoc aRasterizer = resolve_grid_get_rasterizer<nDimension, point_type, adaptor_type>(p0, p1, n_divide);

    autoc maxSlot = n_divide - 1;

    autoc n = extents.size();
    auto aid = vector<array<array<grid_id_type, nDimension>, 2>>(n);
    for (size_t i = 0; i < n; ++i)
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        aid[i][0][iDimension] = static_cast<grid_id_type>(static_cast<double>(adaptor_type::point_comp_c(adaptor_type::box_min_c(extents[i]), iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * aRasterizer[iDimension]);

      aid[i][1] = resolve_grid_id_point_limit<nDimension, point_type, adaptor_type>(adaptor_type::box_max_c(extents[i]), p0, p1, aRasterizer);
    }
    return aid;
  }


  // Location code

  namespace
  {
    template<dim_type nDimension>
    constexpr bucket_id_type GetChildPartOfLocation(location_id_type loc)
    {
      autoce maskLastBits1 = pow_ce(2, nDimension) - 1;

      return loc & maskLastBits1; //!
    }

    static constexpr location_id_type Part1By2(location_id_type n)
    {
      // n = ----------------------9876543210 : Bits initially
      // n = ------98----------------76543210 : After (1)
      // n = ------98--------7654--------3210 : After (2)
      // n = ------98----76----54----32----10 : After (3)
      // n = ----9--8--7--6--5--4--3--2--1--0 : After (4)
      n = (n ^ (n << 16)) & 0xff0000ff; // (1)
      n = (n ^ (n << 8)) & 0x0300f00f; // (2)
      n = (n ^ (n << 4)) & 0x030c30c3; // (3)
      n = (n ^ (n << 2)) & 0x09249249; // (4)
      return n;
    }

    // Separates low 16 bits of input by one bit
    static constexpr location_id_type Part1By1(location_id_type n)
    {
      // n = ----------------fedcba9876543210 : Bits initially
      // n = --------fedcba98--------76543210 : After (1)
      // n = ----fedc----ba98----7654----3210 : After (2)
      // n = --fe--dc--ba--98--76--54--32--10 : After (3)
      // n = -f-e-d-c-b-a-9-8-7-6-5-4-3-2-1-0 : After (4)
      n = (n ^ (n << 8)) & 0x00ff00ff; // (1)
      n = (n ^ (n << 4)) & 0x0f0f0f0f; // (2)
      n = (n ^ (n << 2)) & 0x33333333; // (3)
      n = (n ^ (n << 1)) & 0x55555555; // (4)
      return n;
    }
  }

  static constexpr location_id_type Morton(array<location_id_type, 3> const& xyz)
  {
    // z--z--z--z--z--z--z--z--z--z-- : Part1By2(z) << 2
    // -y--y--y--y--y--y--y--y--y--y- : Part1By2(y) << 1
    // --x--x--x--x--x--x--x--x--x--x : Part1By2(x)
    // zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx : Final result
    return (Part1By2(xyz[2]) << 2) + (Part1By2(xyz[1]) << 1) + Part1By2(xyz[0]);
  }

  static constexpr location_id_type Morton(array<location_id_type, 2> const& xy)
  {
    return (Part1By1(xy[1]) << 1) + Part1By1(xy[0]);
  }

  template<dim_type nDimension>
  static constexpr location_id_type Morton(array<location_id_type, nDimension> const& aidGrid)
  {
    location_id_type msb = aidGrid[0];
    for (dim_type iDimension = 1; iDimension < nDimension; ++iDimension)
      msb |= aidGrid[iDimension];

    location_id_type id = 0;
    for (location_id_type mask = 1, shift = 0, i = 0; msb; mask <<= 1, msb >>= 1, ++i)
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension, ++shift)
        id |= (aidGrid[iDimension] & mask) << (shift - i);

    return id;
  }


  // NTrees

  // NTreeLinear: Non-owning base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
  class NTreeLinear
  {
  protected:
    static bucket_id_type constexpr _nChild = pow_ce(2, nDimension);

    using _Ad = adaptor_type;
    static_assert(AdaptorConcept<_Ad, point_type, box_type, geometry_type>);

  public:
    using entity_id_type = size_t;
    using tree_id_type = location_id_type; // same as the location_id_type, but depth is signed by a sentinel bit.

    struct Node
    {
    private:
      using flag_child_exist = std::conditional<_nChild <= 8, uint8_t, bitset<_nChild>>::type;
      flag_child_exist _hasChildK = 0;

    public:
      vector<entity_id_type> vid;
      box_type box;

      constexpr void EnableChild(bucket_id_type iChild) { _hasChildK |= (1 << iChild); }
      constexpr bool HasChild(bucket_id_type iChild) const { return _hasChildK & (1 << iChild); }
      constexpr bool IsAnyChildExist() const { return _hasChildK > 0; }
    };


  protected: // member variables
    unordered_map<tree_id_type, Node> _nodes;
    box_type _box = {};
    bucket_id_type _nDepthMax = 0;
    bucket_id_type _nRasterResolutionMax = 0;
    max_noelement _nElementMax = 11;

    static_assert(sizeof(_nRasterResolutionMax) <= sizeof(grid_id_type));

  protected: // aid struct to partitioning
    struct _NodePartitioner
    {
      tree_id_type kNode = 0;
      Node* pNode = nullptr;
      bucket_id_type nDepth;
      location_id_type idLocationBegin;
      vector<entity_id_type>::iterator itBegin;
      vector<entity_id_type>::iterator itEnd;
    };

  protected: // aid functions
    static inline vector<entity_id_type> _generatePointId(size_t n)
    {
      auto vidPoint = vector<entity_id_type>(n);
      std::iota(begin(vidPoint), end(vidPoint), 0);
      return vidPoint;
    }

    inline Node& _createChild(Node& nodeParent, bucket_id_type iChild, tree_id_type kChild)
    {
      nodeParent.EnableChild(iChild);

      auto& nodeChild = _nodes[kChild];

      {
        auto flagChild = iChild;
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          autoc fGreater = (flagChild & 0x0001);
          _Ad::point_comp(_Ad::box_min(nodeChild.box), iDimension) = fGreater ? (_Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) + _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension)) / geometry_type(2) : _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension);
          _Ad::point_comp(_Ad::box_max(nodeChild.box), iDimension) = fGreater ? _Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) : ((_Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) + _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension)) / geometry_type(2));
          flagChild >>= 1;
        }
      }
      return nodeChild;
    }

  public:
    static constexpr size_t EstimateNodeNumber(size_t n, bucket_id_type nDepthMax, max_noelement nElementMax)
    {
      assert(nElementMax > 0);
      assert(nDepthMax > 0);

      autoc nDepthEstimated = std::min(nDepthMax, static_cast<bucket_id_type>(ceil((log2(static_cast<float>(n) / static_cast<float>(nElementMax)) + 1) / static_cast<float>(nDimension))));
      autoc nNodeEstimated = (pow_ce(_nChild, nDepthEstimated + 1) - 1) / (_nChild - 1);
      return static_cast<size_t>(1.5 * nNodeEstimated);
    }

    static constexpr tree_id_type GetHash(bucket_id_type level, tree_id_type key)
    {
      assert(key < (tree_id_type(1) << (level * nDimension)));
      return (1 << (level * nDimension)) | key;
    }

    static constexpr bucket_id_type GetDepth(tree_id_type key)
    {
      // Keep shifting off three bits at a time, increasing depth counter
      for (bucket_id_type d = 0; key; d++, key >>= nDimension)
        if (key == 1) // If only sentinel bit remains, exit with node depth
          return d;

      assert(false); // Bad key
      return 0;
    }

    static constexpr tree_id_type RemoveSentinelBit(tree_id_type key, std::optional<bucket_id_type> const& onDepth = std::nullopt)
    {
      autoc nDepth = onDepth.value_or(GetDepth(key));
      return key - (1 << nDepth);

    }

    size_t GetSize() const { return _nodes.size(); }
    auto const& GetBox() const { return _box; }
    auto const& Get() const { return _nodes; }
    auto const& Get(tree_id_type key) const { return _nodes.at(key); }
    auto GetDepthMax() const { return _nDepthMax; }
    auto GetResolutionMax() const { return _nRasterResolutionMax; }


    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    void Init(box_type const& box, bucket_id_type nDepthMax, max_noelement nElementMax = 11)
    {
      assert(this->_nodes.empty()); // To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already builded tree is wanted to be reset, use the Clear() function before init.
      assert(nDepthMax > 1);
      assert(nElementMax > 1);
      assert(CHAR_BIT * sizeof(_nRasterResolutionMax) >= _nDepthMax);

      this->_box = box;
      this->_nDepthMax = nDepthMax;
      this->_nRasterResolutionMax = pow_ce(2, nDepthMax);
      this->_nElementMax = nElementMax;

      auto& nodeRoot = this->_nodes[GetHash(0, 0)];
      nodeRoot.box = box;
    }


    // Visit nodes with special selection and procedure
    template<typename fnSelector, typename fnProcedure>
    void VisitNodes(location_id_type kRoot, fnSelector const& selector, fnProcedure const& procedure) const
    {
      auto q = queue<tree_id_type>();
      for (q.push(kRoot); !q.empty(); q.pop())
      {
        autoc& key = q.front();
        autoc& node = _nodes.at(key);
        procedure(key, _nodes.at(key));

        autoc flagPrefix = key << nDimension;
        for (bucket_id_type iChild = 0; iChild < _nChild; ++iChild)
        {
          if (node.HasChild(iChild))
          {
            autoc kChild = flagPrefix | iChild;
            if (selector(kChild, _nodes.at(kChild)))
              q.push(kChild);
          }
        }
      }
    }


    // Visit nodes with special selection and procedure and if unconditional selection is fulfilled descendants will not be test with selector
    template<typename fnSelector, typename fnSelectorUnconditional, typename fnProcedure>
    void VisitNodes(location_id_type kRoot, fnSelector const& selector, fnSelectorUnconditional const& selectorUnconditional, fnProcedure const& procedure) const
    {
      struct _search
      {
        tree_id_type key;
        Node const& pNode;
        bucket_id_type nDepth;
        bool fUnconditional;
      };

      autoc nDepthRoot = GetDepth(kRoot);
      auto q = queue<_search>();
      for (q.push({ kRoot, _nodes.at(kRoot), nDepthRoot, false }); !q.empty(); q.pop())
      {
        autoc& item = q.front();
        procedure(item.pNode, item.fUnconditional);

        bucket_id_type const nDepthChild = item.nDepth + 1;
        autoc flagPrefix = item.key << nDimension;
        for (bucket_id_type iChild = 0; iChild < _nChild; ++iChild)
        {
          if (item.pNode.HasChild(iChild))
          {
            autoc kChild = GetHash(nDepthChild, flagPrefix + iChild);
            autoc& pNodeChild = _nodes.at(kChild);
            if (item.fUnconditional)
              q.push({ kChild, pNodeChild, nDepthChild, true });
            else if (selector(pNodeChild))
              q.push({ kChild, pNodeChild, nDepthChild, selectorUnconditional(pNodeChild) });
          }
        }
      }
    }


    bool EraseId(entity_id_type id)
    {
      return std::ranges::any_of(_nodes, [&](auto& pairNode) { return erase(pairNode.second.vid, id); });
    }


    void UpdateIndexes(unordered_map<entity_id_type, entity_id_type> const& vIndexOldNew)
    {
      autoc itEnd = std::end(vIndexOldNew);
      std::ranges::for_each(_nodes, [&](auto& node)
      {
        std::ranges::for_each(node.vid, [&](auto& id)
        {
          autoc it = vIndexOldNew.find(id);
          if (it != itEnd)
            id = it->second;
        });

        std::erase_if(node.vid, [](autoc id) { return id == std::numeric_limits<entity_id_type>::max(); });
      });
    }


    box_type GetExtent(tree_id_type key) const
    {
      auto e = box_type();
      _Ad::box_min(e) = _Ad::box_min_c(_box);

      auto aSize = array<geometry_type, nDimension>();
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        aSize[iDimension] = _Ad::point_comp_c(_Ad::box_max_c(_box), iDimension) - _Ad::point_comp_c(_Ad::box_min_c(_box), iDimension);

      autoc nDepth = GetDepth(key);
      autoc nRasterResolution = pow_ce(2, nDepth);
      autoc rMax = 1.0 / static_cast<double>(nRasterResolution);

      auto keyShifted = key;// RemoveSentinelBit(key, nDepth);
      for (bucket_id_type iDepth = 0; iDepth < nDepth; ++iDepth)
      {
        autoc r = rMax * (1 << iDepth);
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension, keyShifted >>= 1)
          _Ad::point_comp(_Ad::box_min(e), iDimension) += static_cast<geometry_type>((aSize[iDimension] * r)) * (keyShifted & 0x00000001);
      }

      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        _Ad::point_comp(_Ad::box_max(e), iDimension) = _Ad::point_comp_c(_Ad::box_min_c(e), iDimension) + static_cast<geometry_type>(aSize[iDimension] * rMax);

      return e;
    }


    void Clear()
    {
      erase_if(_nodes, [](autoc& p) { return p != GetHash(0, 0); });
      _nodes[GetHash(0, 0)].vid.clear();
    }


    // Doubles the handled space relative to the root. iRootNew defines the relative location in the new space
    //!IMPLEMENT void Extend(location_id_type iRootNew = 0) {}
  };



  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
  class NTreePoint : public NTreeLinear<nDimension, point_type, box_type, adaptor_type, geometry_type>
  {
  private:
    using base = NTreeLinear<nDimension, point_type, box_type, adaptor_type, geometry_type>;
    using base::_NodePartitioner;
    using base::_Ad;
  public:
    using base::entity_id_type;
    using base::tree_id_type;
    using base::Node;

  private:
    static inline vector<location_id_type> _getLocationId(vector<array<grid_id_type, nDimension>> const& vidGrid)
    {
      auto vidLocation = vector<location_id_type>(vidGrid.size());
      std::ranges::transform(vidGrid, begin(vidLocation), [](autoc& aGrid) { return Morton(aGrid); });
      return vidLocation;
    }

    void _addNode(_NodePartitioner& ns, bucket_id_type iChild, location_id_type nLocationStep, vector<entity_id_type>::iterator const& itBegin, vector<entity_id_type>::iterator const& itEnd, queue<_NodePartitioner>& q)
    {
      if (itBegin == itEnd)
        return;

      autoc kChild = (ns.kNode << nDimension) | iChild;
      auto& nodeChild = this->_createChild(*ns.pNode, iChild, kChild);

      autoc nElement = distance(itBegin, itEnd);
      if (nElement < base::_nElementMax || ns.nDepth == this->_nDepthMax)
        nodeChild.vid.insert(end(nodeChild.vid), itBegin, itEnd);
      else
      {
        autoc idLocation = ns.idLocationBegin + iChild * nLocationStep;
        q.emplace(_NodePartitioner{ kChild, &nodeChild, bucket_id_type(ns.nDepth + 1), idLocation, itBegin, itEnd });
      }
    }


  public:

    // Create
    static NTreePoint Create(span<point_type const> const& vpt, bucket_id_type nDepthMax, std::optional<box_type> const& oextent = std::nullopt, max_noelement nElementMaxInNode = 11)
    {
      autoc n = vpt.size();

      auto tree = NTreePoint{};
      tree.Init(oextent.value_or(_Ad::box_of_points(vpt)), nDepthMax, nElementMaxInNode);
      tree._nodes.reserve(base::EstimateNodeNumber(n, nDepthMax, nElementMaxInNode));

      autoce kRoot = base::GetHash(0, 0);
      auto& nodeRoot = tree._nodes.at(kRoot);

      autoc idLocationMax = pow_ce(tree._nRasterResolutionMax, nDimension);
      autoc aidGrid = resolve_grid_id<nDimension, point_type, _Ad>(vpt, _Ad::box_min_c(tree._box), _Ad::box_max_c(tree._box), tree._nRasterResolutionMax);
      autoc aidLocation = _getLocationId(aidGrid);

      auto vidPoint = base::_generatePointId(n);
      auto q = std::queue<_NodePartitioner>();
      auto nsRoot = _NodePartitioner{ kRoot, &nodeRoot, bucket_id_type(1), location_id_type(0), std::begin(vidPoint), std::end(vidPoint) };
      for (q.push(nsRoot); !q.empty(); q.pop())
      {
        auto& ns = q.front();
        autoc nPart = pow_ce(base::_nChild, ns.nDepth);
        autoc nLocationStep = idLocationMax / nPart;

        auto itEndPrev = ns.itBegin;
        for (bucket_id_type iChild = 1; iChild < base::_nChild; ++iChild)
        {
          autoc idLocationSplit = ns.idLocationBegin + iChild * nLocationStep;
          auto itSplit = std::partition(itEndPrev, ns.itEnd, [&](autoc idPoint) { return aidLocation[idPoint] < idLocationSplit; });
          tree._addNode(ns, iChild - 1, nLocationStep, itEndPrev, itSplit, q);
          itEndPrev = itSplit;
        }

        tree._addNode(ns, base::_nChild - 1, nLocationStep, itEndPrev, ns.itEnd, q);
      }

      return tree;
    }

  public: // edit functions

    // Insert item into a node. If fInsertToLeaf is true: The smallest node will be chosen by the max depth. If fInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(entity_id_type id, point_type const& pt, bool fInsertToLeaf = false)
    {
      if (!_Ad::does_box_contain_point(this->_box, pt))
        return false;

      autoc kNodeSmallest = FindSmallestNode(pt);
      if (!kNodeSmallest)
        return false;

      autoc aGrid = resolve_grid_id<nDimension, point_type, _Ad>(vector{ pt }, _Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->_nRasterResolutionMax).at(0);
      autoc idLocation = Morton(aGrid);
      auto kNode = this->GetHash(this->_nDepthMax, idLocation);

      if (kNode == kNodeSmallest)
      {
        this->_nodes.at(kNode).vid.emplace_back(id);
        return true;
      }

      if (fInsertToLeaf)
      {
        auto& nodeNew = this->_nodes[kNode];
        nodeNew.vid.emplace_back(id);
        nodeNew.box = this->GetExtent(kNode);

        // Create all child between the new (kNode) and the smallest existing one (kNodeSmallest)
        auto kNodeParent = kNode;
        do
        {
          autoc iChild = GetChildPartOfLocation<nDimension>(kNodeParent);
          kNodeParent = kNodeParent >>= nDimension;
          assert(kNodeParent);
          auto& nodeParent = this->_nodes[kNodeParent];
          nodeParent.EnableChild(iChild);
          nodeParent.box = this->GetExtent(kNodeParent);
        } while (kNodeParent != kNodeSmallest);
      }
      else
      {
        autoc itNode = this->_nodes.find(kNodeSmallest);
        if (itNode->second.IsAnyChildExist())
        {
          autoc nDepth = this->GetDepth(kNodeSmallest);
          autoc kNodeChild = kNode << (nDimension * (this->_nDepthMax - nDepth - 1));
          autoc iChild = GetChildPartOfLocation<nDimension>(kNodeChild);
          auto& nodeChild = this->_createChild(itNode->second, iChild, kNodeChild);
          nodeChild.vid.emplace_back(id);
        }
        else
          itNode->second.vid.emplace_back(id);
      }

      return true;
    }

    bool Erase(entity_id_type id, point_type const& pt)
    {
      autoc kOld = FindSmallestNode(pt);
      if (!kOld)
        return false; // old box is not in the handled space domain

      return this->_nodes.at(kOld).vid.erase(id); // if 0, id was not registered previously.
    }

    bool Update(entity_id_type id, point_type const& ptNew)
    {
      if (!this->EraseId(id))
        return false;

      return this->Insert(id, ptNew);
    }

    void Update(entity_id_type id, point_type const& ptOld, point_type const& ptNew)
    {
      if (!this->EraseId(id, ptOld))
        return false;

      return this->Insert(id, ptNew);
    }

  public:

    // Find smallest node which contains the box
    tree_id_type FindSmallestNode(point_type const& pt) const
    {
      autoc aGrid = resolve_grid_id<nDimension, point_type, _Ad>(vector{ pt }, _Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->_nRasterResolutionMax).at(0);
      autoc idLocation = Morton(aGrid);

      for (auto kSmallestNode = this->GetHash(this->_nDepthMax, idLocation); kSmallestNode; kSmallestNode >>= nDimension)
        if (this->_nodes.contains(kSmallestNode))
          return kSmallestNode;

      return 0; // Not found
    }

    // Range search
    vector<entity_id_type> RangeSearch(box_type const& range, span<point_type const> const& vpt) const
    {
      auto vidFound = vector<entity_id_type>();

      this->VisitNodes(base::GetHash(0, 0)
        , [&](Node const& pNode)
          {
            return are_boxes_overlapped(range, pNode.box, false);
          }
        , [&](Node const& pNode)
          {
            return are_boxes_overlapped(range, pNode.box, true);
          }
        , [&](Node const& pNode, bool fUnconditional)
          {
            if (fUnconditional)
              vidFound.insert(end(vidFound), begin(pNode.vid), end(pNode.vid));
            else
              std::ranges::copy_if(pNode.vid, back_inserter(vidFound), [&](autoc id) { return does_box_contain_point(range, vpt[id]); });
          }
      );

      return vidFound;
    }

  };

  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
  class NTreeBoundingBox : public NTreeLinear<nDimension, point_type, box_type, adaptor_type, geometry_type>
  {
  private:
    using base = NTreeLinear<nDimension, point_type, box_type, adaptor_type, geometry_type>;
    using base::_NodePartitioner;
    using base::_Ad;

  public:
    using base::entity_id_type;
    using base::tree_id_type;
    using base::Node;

  private:
    static inline vector<array<location_id_type, 2>> _getLocationId(vector<array<array<grid_id_type, nDimension>, 2>> const& vidGrid)
    {
      auto vidLocation = vector<array<location_id_type, 2>>(vidGrid.size());
      std::ranges::transform(vidGrid, begin(vidLocation), [](autoc& aGrid)->array<location_id_type, 2> { return { Morton(aGrid[0]), Morton(aGrid[1]) }; });
      return vidLocation;
    }


    void _addNode(_NodePartitioner& ns, bucket_id_type iChild, location_id_type nLocationStep, vector<entity_id_type>::iterator itBegin, vector<entity_id_type>::iterator itIntersected, vector<entity_id_type>::iterator itEnd, queue<_NodePartitioner>& q)
    {
      if (itBegin == itEnd)
        return;

      ns.pNode->EnableChild(iChild);
      ns.pNode->vid.insert(end(ns.pNode->vid), itBegin, itIntersected);

      autoc kChild = (ns.kNode << nDimension) | iChild;
      auto& nodeChild = this->_createChild(*ns.pNode, iChild, kChild);

      autoc nElement = distance(itIntersected, itEnd);
      if (nElement < base::_nElementMax || ns.nDepth == this->_nDepthMax)
        nodeChild.vid.insert(end(nodeChild.vid), itIntersected, itEnd);
      else
      {
        autoc idLocation = ns.idLocationBegin + iChild * nLocationStep;
        q.emplace(_NodePartitioner{ kChild, &nodeChild, bucket_id_type(ns.nDepth + 1), idLocation, itIntersected, itEnd });
      }
    }


  public:
    static NTreeBoundingBox Create(span<box_type const> const& vExtent, bucket_id_type nDepthMax, std::optional<box_type> const& oextent = std::nullopt, max_noelement nElementMaxInNode = 11)
    {
      auto tree = NTreeBoundingBox{};
      tree.Init(oextent.value_or(_Ad::box_of_boxes(vExtent))
        , nDepthMax
        , nElementMaxInNode
      );
     
      autoc n = vExtent.size();
      tree._nodes.reserve(base::EstimateNodeNumber(n, nDepthMax, nElementMaxInNode));

      autoce kRoot = base::GetHash(0, 0);
      auto& nodeRoot = tree._nodes.at(kRoot);

      autoc idLocationMax = pow_ce(tree._nRasterResolutionMax, nDimension);
      autoc aidGrid = resolve_grid_id<nDimension, box_type, point_type, _Ad>(vExtent, _Ad::box_min(tree._box), _Ad::box_max(tree._box), tree.GetResolutionMax());
      autoc aidLocation = _getLocationId(aidGrid);

      auto vidPoint = base::_generatePointId(n);
      auto q = std::queue<_NodePartitioner>();
      auto nsRoot = _NodePartitioner{ kRoot, &nodeRoot, bucket_id_type(1), location_id_type(0), std::begin(vidPoint), std::end(vidPoint) };
      for (q.push(nsRoot); !q.empty(); q.pop())
      {
        auto& ns = q.front();
        autoc nPart = pow_ce(base::_nChild, ns.nDepth);
        autoc nLocationStep = idLocationMax / nPart;

        auto itEndPrev = ns.itBegin;
        for (bucket_id_type iChild = 1; iChild < base::_nChild; ++iChild)
        {
          autoc idLocationSplit = ns.idLocationBegin + iChild * nLocationStep;
          auto itSplitIntersected = std::partition(itEndPrev, ns.itEnd, [&](autoc idExt) { return aidLocation[idExt][0] < idLocationSplit && idLocationSplit <= aidLocation[idExt][1]; });
          auto itSplit = std::partition(itSplitIntersected, ns.itEnd, [&](autoc idExt) { return aidLocation[idExt][0] < idLocationSplit; });
          tree._addNode(ns, iChild - 1, nLocationStep, itEndPrev, itSplitIntersected, itSplit, q);
          itEndPrev = itSplit;
        }

        // Last item
        tree._addNode(ns, base::_nChild - 1, nLocationStep, itEndPrev, itEndPrev, ns.itEnd, q);
      }

      return tree;
    }

  public:

    // Find smallest node which contains the box
    tree_id_type FindSmallestNode(box_type const& box) const
    {
      autoc aGrid = resolve_grid_id({ box }, this->_box.Min, this->_box.Max, this->_nRasterResolutionMax).at(0);
      auto idLocationMin = Morton(aGrid[0]);
      auto idLocationMax = Morton(aGrid[1]);

      auto nDepth = this->_nDepthMax;
      for (auto flagDiffOfLocation = idLocationMin ^ idLocationMax; flagDiffOfLocation; flagDiffOfLocation >>= nDimension, --nDepth)
        idLocationMin >>= nDimension;

      for (auto kSmallestNode = GetHash(nDepth, idLocationMin); kSmallestNode; kSmallestNode >>= nDimension)
        if (this->_nodes.find(kSmallestNode))
          return kSmallestNode;

      return 0; // Not found
    }


    bool Insert(entity_id_type id, box_type const& ext, bool fInsertToLeaf = false)
    {
      if (!_Ad::are_boxes_overlapped(this->_box, ext))
        return false;

      autoc kNew = FindSmallestNode(ext);
      if (!kNew)
        return false; // new box is not in the handled space domain

      autoc aGrid = resolve_grid_id({ ext }, this->_box.Min, this->_box.Max, this->_nRasterResolutionMax).at(0);
      auto idLocationMin = Morton(aGrid[0]);
      auto idLocationMax = Morton(aGrid[1]);
      autoc idLocation = idLocationMin & idLocationMax;

      autoce maskLastBits1 = base::_nChild - 1;

      auto iChildMin = GetChildPartOfLocation<nDimension>(idLocationMin);
      auto iChildMax = GetChildPartOfLocation<nDimension>(idLocationMax);

      auto idLocationTree = idLocation;
      auto nDepthMaxBox = this->_nDepthMax;
      for (; nDepthMaxBox > 0 && (maskLastBits1 & idLocationTree) == 0 && iChildMin != iChildMax; --nDepthMaxBox)
      {
        idLocationMin >>= nDimension;
        idLocationMax >>= nDimension;

        iChildMin = GetChildPartOfLocation<nDimension>(idLocationMin);
        iChildMax = GetChildPartOfLocation<nDimension>(idLocationMax);

        idLocationTree >>= nDimension;
      }

      auto iChild = numeric_limits<bucket_id_type>::max();
      autoc itEnd = end(this->_nodes);
      for (auto iDepth = nDepthMaxBox; iDepth >= 0; --iDepth, iChild = GetChildPartOfLocation<nDimension>(idLocationTree), idLocationTree >>= nDimension)
      {
        autoc kNode = GetHash(iDepth, idLocationTree);
        autoc itNode = this->_nodes.find(kNode);
        if (itNode == itEnd)
          continue;

        if (itNode.second.IsAnyChildExist() && iChild != numeric_limits<bucket_id_type>::max())
        {
          auto& nodeChild = _createChild(itNode->second, iChild, (kNode << nDimension) | iChild);
          nodeChild.vid.emplace_back(id);
        }
        else
          itNode.second.vid.emplace_back(id);

        break;
      }
      //! fInsertToLeaf
      return true;
    }


    bool Erase(entity_id_type id, box_type const& ext)
    {
      autoc kOld = FindSmallestNode(ext);
      if (!kOld)
        return false; // old box is not in the handled space domain

      return this->_nodes.at(kOld).vid.erase(id); // if 0, id was not registered previously.
    }


    bool Update(entity_id_type id, box_type const& extNew)
    {
      if (!this->EraseId(id))
        return false;

      return this->Insert(id, extNew);
    }


    bool Update(entity_id_type id, box_type const& extOld, box_type const& extNew)
    {
      if (!this->Erase(id, extOld))
        return false; // id was not registered previously.

      return this->Insert(id, extNew);
    }



  public:
    vector<entity_id_type> PickSearch(point_type const& ptPick, span<box_type const> const& vExtent) const
    {
      autoc aGrid = resolve_grid_id({ ptPick }, this->_box.Min, this->_box.Max, this->_nRasterResolutionMax).at(0);
      autoc idLocation = Morton(aGrid);

      auto kNode = GetHash(this->_nDepthMax, idLocation);

      auto vidFound = vector<entity_id_type>();
      vidFound.reserve(100);
      autoc itEnd = std::end(this->_nodes);
      for (auto iDepth = this->_nDepthMax; iDepth >= 0; --iDepth, kNode >>= nDimension)
      {
        autoc itNode = this->_nodes.find(kNode);
        if (itNode == itEnd)
          continue;

        std::ranges::copy_if(itNode->second.vid, [&](autoc id) { return does_box_contain_point(vExtent[id], ptPick); });
      }

      return vidFound;
    }

  private: // Range search helper
    void _rangeSearchRec(vector<entity_id_type>& vidFound, tree_id_type kCurrent, Node const& pNode, bucket_id_type nDepth, box_type const& range, span<box_type const> const& vExtent, bool fFullyContained, bool fUnconditional) const
    {
      ++nDepth;
      autoc flagPrefix = kCurrent << nDimension;

      if (fUnconditional)
      {
        vidFound.insert(end(vidFound), begin(pNode.vid), end(pNode.vid));

        for (bucket_id_type i = 0; i < base::_nChild; ++i)
        {
          if (pNode.HasChild(i))
          {
            autoc kChild = flagPrefix | i;
            _rangeSearchRec(vidFound, kChild, this->_nodes.at(kChild), nDepth, range, vExtent, fFullyContained, true);
          }
        }
      }
      else
      {
        std::ranges::copy_if(pNode.vid, back_inserter(vidFound), [&](autoc id) { return are_boxes_overlapped(range, vExtent[id], fFullyContained); });
        for (bucket_id_type i = 0; i < base::_nChild; ++i)
        {
          if (pNode.HasChild(i))
          {
            autoc kChild = flagPrefix | i;
            autoc& pNodeChild = this->_nodes.at(kChild);
            if (are_boxes_overlapped(range, pNodeChild.box, false))
              _rangeSearchRec(vidFound, kChild, pNodeChild, nDepth, range, vExtent, fFullyContained, are_boxes_overlapped(range, pNodeChild.box, true));
          }
        }
      }
    }

  public:
    // Range search
    vector<entity_id_type> RangeSearch(box_type const& range, span<box_type const> const& vExtent, bool fFullyContained = true) const
    {
      auto vidFound = vector<entity_id_type>();
      vidFound.reserve(100);
      autoce kRoot = base::GetHash(0, 0);
      _rangeSearchRec(vidFound, kRoot, this->_nodes.at(kRoot), 0, range, vExtent, fFullyContained, false);

      return vidFound;
    }

    // Collision detection: Returns all overlapping boxes from the source trees.
    static vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(NTreeBoundingBox const& tL, span<box_type const> const& vExtentL, NTreeBoundingBox const& tR, span<box_type const> const& vExtentR)
    {
      autoce kRoot = tL.GetHash(0, 0);
      auto vResult = vector<std::pair<entity_id_type, entity_id_type>>{};
      vResult.reserve(vExtentL.size() / 10);

      using iter_type = decltype(tL._nodes)::const_iterator;
      using iter_and_ent_type = std::pair<iter_type, vector<entity_id_type>>;

      auto q = queue<std::pair<iter_and_ent_type, iter_and_ent_type>>{};
      for (q.emplace(std::pair{ tL._nodes.find(kRoot), vector<entity_id_type>{} }, std::pair{ tR._nodes.find(kRoot), vector<entity_id_type>{} }); !q.empty(); q.pop())
      {
        autoc& [itDL, itDR] = q.front();
        autoc& [itNodeL, vidL] = itDL;
        autoc& [itNodeR, vidR] = itDR;

        autoc& nodeL = itNodeL->second;
        autoc flagPrefixL = itNodeL->first << nDimension;

        autoc& nodeR = itNodeR->second;
        autoc flagPrefixR = itNodeR->first << nDimension;

        // Check the current ascendant content 
        {
          for (autoc idL : vidL)
            for (autoc idR : nodeR.vid)
              if (_Ad::are_boxes_overlapped(vExtentL[idL], vExtentR[idR], false))
                vResult.emplace_back(idL, idR);

          for (autoc idR : vidR)
            for (autoc idL : nodeL.vid)
              if (_Ad::are_boxes_overlapped(vExtentL[idL], vExtentR[idR], false))
                vResult.emplace_back(idL, idR);

          for (autoc idL : nodeL.vid)
            for (autoc idR : nodeR.vid)
              if (_Ad::are_boxes_overlapped(vExtentL[idL], vExtentR[idR], false))
                vResult.emplace_back(idL, idR);
        }


        auto vidLChild = vidL;

        auto vidRChild = vidR;
        vidRChild.insert(vidRChild.end(), nodeR.vid.begin(), nodeR.vid.end());

        // Check the child nodes extents
        if (nodeL.IsAnyChildExist())
        {
          vidLChild.insert(vidLChild.end(), nodeL.vid.begin(), nodeL.vid.end());

          for (bucket_id_type iChildL = 0; iChildL < base::_nChild; ++iChildL)
          {
            if (!nodeL.HasChild(iChildL))
              continue;

            autoc& itNodeLL = tL._nodes.find(flagPrefixL | iChildL);
            if (nodeR.IsAnyChildExist())
            {

              for (bucket_id_type iChildR = 0; iChildR < base::_nChild; ++iChildR)
              {
                if (!nodeR.HasChild(iChildR))
                  continue;

                autoc itNodeRR = tR._nodes.find(flagPrefixR | iChildR);
                if (_Ad::are_boxes_overlapped(itNodeLL->second.box, itNodeRR->second.box, false))
                  q.emplace(std::pair{ itNodeLL, vidLChild }, std::pair{ itNodeRR, vidRChild });
              }
            }
            else
            {
              if (_Ad::are_boxes_overlapped(itNodeLL->second.box, itNodeR->second.box, false))
                q.emplace(std::pair{ itNodeLL, vidLChild }, itDR);
            }
          }
        }
        else
        {
          for (bucket_id_type iChildR = 0; iChildR < base::_nChild; ++iChildR)
          {
            if (!nodeR.HasChild(iChildR))
              continue;

            autoc itNodeRR = tR._nodes.find(flagPrefixR | iChildR);
            if (_Ad::are_boxes_overlapped(itNodeL->second.box, itNodeRR->second.box, false))
              q.emplace(itDL, std::pair{ itNodeRR, vidRChild });
          }
        }
      }
      return vResult;
    }

    vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(span<box_type const> const& vExtentL, NTreeBoundingBox const& tR, span<box_type const> const& vExtentR) const
    {
      return CollisionDetection(*this, vExtentL, tR, vExtentR);
    }
  };

  // Aliases
  using Point1D = NTree::Point<1>;
  using Point2D = NTree::Point<2>;
  using Point3D = NTree::Point<3>;

  using BoundingBox1D = NTree::BoundingBox<1>;
  using BoundingBox2D = NTree::BoundingBox<2>;
  using BoundingBox3D = NTree::BoundingBox<3>;

  // Dualtree for points
  using DualtreePoint = NTree::NTreePoint<1, Point1D, BoundingBox1D>;

  // Dualtree for bounding boxes
  using DualtreeBox = NTree::NTreeBoundingBox<1, Point1D, BoundingBox1D>;

  // Quadtree for points
  using QuadtreePoint = NTree::NTreePoint<2, Point2D, BoundingBox2D>;

  // Quadtree for bounding boxes
  using QuadtreeBox = NTree::NTreeBoundingBox<2, Point2D, BoundingBox2D>;

  // Octree for points
  using OctreePoint = NTree::NTreePoint<3, Point3D, BoundingBox3D>;

  // Octree for bounding boxes
  using OctreeBox = NTree::NTreeBoundingBox<3, Point3D, BoundingBox3D>;

  // Hexatree for points
  using HexatreePoint = NTree::NTreePoint<4, NTree::Point<4>, NTree::BoundingBox<4>>;
}





#ifdef undef_autoc
#undef autoc
#undef undef_autoc
#endif

#ifdef undef_autoce
#undef autoce
#undef undef_autoce
#endif

#endif // N_ARY_TREE_GUARD