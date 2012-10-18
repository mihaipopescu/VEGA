//  Copyright (C) Mihai Popescu 2012
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef VEGA_DETAIL_DISJOINT_SETS_HPP
#define VEGA_DETAIL_DISJOINT_SETS_HPP


namespace vega
{
namespace detail
{

/* the postcondition of link sets is:
 component_representative(i) == component_representative(j)
 */
template <class ParentPA, class RankPA, class SizePA, class Vertex, 
          class ComponentRepresentative>
inline void
link_sets(ParentPA p, RankPA rank, SizePA s, Vertex i, Vertex j,
          ComponentRepresentative comp_rep)
{
  i = comp_rep(p, i);
  j = comp_rep(p, j);
  if (i == j) return;
  if (get(rank, i) > get(rank, j)) {
    put(p, j, i);
    put(s, i, get(s, i) + get(s, j));
  }
  else {
    put(p, i, j);
    put(s, j, get(s, i) + get(s, j));
    if (get(rank, i) == get(rank, j)) 
      put(rank, j, get(rank, j) + 1);
  }
}

    } // namespace detail
} // namespace vega


#endif // VEGA_DETAIL_DISJOINT_SETS_HPP