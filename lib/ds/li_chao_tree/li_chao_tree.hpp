template <typename T>
struct LiChaoTree {
  using Line  = typename T::Line;
  using Value = typename T::Value;
  using Range = typename T::Range;

  struct Node {
    Line line;
    uint32_t l, r;
  };

  uint32_t counter = 0;
  std::vector<Node> t;

  LiChaoTree() { build(); };
  explicit LiChaoTree(uint32_t size) { build(size); }

  int new_node(const Line& line) {
    t.emplace_back(line, 0, 0);
    return counter++;
  }

  void build(uint32_t size = 0) {
    t.clear();
    counter = 0;
    new_node(T::id());
    reserve(size);
  }

  void reserve(uint32_t size) { t.reserve(size); }

  void add(Line x) {
    int k = 0;
    auto [l, r] = T::range_id();
    Value xl = T::evaluate(x, l), xr = T::evaluate(x, r);

    while (true) {
      Line& y = t[k].line;
      Value yl = T::evaluate(y, l), yr = T::evaluate(y, r);
      if (l + 1 == r) {
        if (T::compare(xl, yl)) std::swap(x, y);
        return;
      }

      Range mid = l + (r - l) / 2;
      if (T::compare(xl, yl)) {
        if (T::compare(xr, yr)) {
          std::swap(x, y);
          return;
        } else {
          Value xmid = T::evaluate(x, mid);
          Value ymid = T::evaluate(y, mid);
          if (T::compare(xmid, ymid)) {
            std::swap(x, y);
            k = (t[k].r == 0 ? t[k].r = new_node(T::id()) : t[k].r);
            xl = ymid, xr = yr, l = mid;
          } else {
            k = (t[k].l == 0 ? t[k].l = new_node(T::id()) : t[k].l);
            xr = xmid, r = mid;
          }
        }
      } else {
        if (T::compare(xr, yr)) {
          Value xmid = T::evaluate(x, mid);
          Value ymid = T::evaluate(y, mid);
          if (T::compare(xmid, ymid)) {
            std::swap(x, y);
            k = (t[k].l == 0 ? t[k].l = new_node(T::id()) : t[k].l);
            xl = yl, xr = ymid, r = mid;
          } else {
            k = (t[k].r == 0 ? t[k].r = new_node(T::id()) : t[k].r);
            xl = xmid, l = mid;
          }
        } else {
          return;
        }
      }
    }
  }

  Value get(Range x) const {
    int k = 0;
    auto [l, r] = T::range_id();
    Value res = T::evaluate(t[0].line, x);

    while (true) {
      Range mid = l + (r - l) / 2;
      if (x == mid) [[unlikely]] {
        break;
      } else if (x < mid) {
        k = t[k].l;
        r = mid;
      } else {
        k = t[k].r;
        l = mid;
      }
      if (k == 0) break;
      res = T::op(res, T::evaluate(t[k].line, x));
    }
    return res;
  }
};