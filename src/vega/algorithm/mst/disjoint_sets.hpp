//  Copyright (C) Mihai Popescu 2012

#ifndef VEGA_DISJOINT_SETS_HPP
#define VEGA_DISJOINT_SETS_HPP

#include <vector>
#include <boost/graph/properties.hpp>
#include <boost/pending/detail/disjoint_sets.hpp>
#include "detail/disjoint_sets.hpp"

namespace vega
{
    // This is a generalized functor to provide disjoint sets operations
    // with "union by rank" and "path compression".  A disjoint-set data
    // structure maintains a collection S={S1, S2, ..., Sk} of disjoint
    // sets. Each set is identified by a representative, which is some
    // member of of the set. Sets are represented by rooted trees. Two
    // heuristics: "union by rank" and "path compression" are used to
    // speed up the operations.

    // Disjoint Set requires two vertex properties for internal use.  A
    // RankPA and a ParentPA. The RankPA must map Vertex to some Integral type
    // (preferably the size_type associated with Vertex). The ParentPA
    // must map Vertex to Vertex.
    template <class RankPA, class ParentPA, class SizePA,
    class FindCompress = boost::find_with_full_path_compression
    >
    class disjoint_sets {
        typedef disjoint_sets self;

        inline disjoint_sets() {}
    public:
        inline disjoint_sets(RankPA r, ParentPA p, SizePA s) 
            : rank(r), parent(p), size(s) {}

        inline disjoint_sets(const self& c) 
            : rank(c.rank), parent(c.parent), size(c.size) {}

        // Make Set -- Create a singleton set containing vertex x
        template <class Element>
        inline void make_set(Element x)
        {
            put(parent, x, x);
            typedef typename property_traits<RankPA>::value_type R;
            put(rank, x, R());
            typedef typename property_traits<SizePA>::value_type S;
            put(size, x, 1);
        }

        // Link - union the two sets represented by vertex x and y
        template <class Element>
        inline void link(Element x, Element y)
        {
            detail::link_sets(parent, rank, size, x, y, rep);
        }

        template <class Element>
        inline Element get_size(Element x)
        {
            return get(size, x);
        }

        // Union-Set - union the two sets containing vertex x and y 
        template <class Element>
        inline void union_set(Element x, Element y)
        {
            link(find_set(x), find_set(y));
        }

        // Find-Set - returns the Element representative of the set
        // containing Element x and applies path compression.
        template <class Element>
        inline Element find_set(Element x)
        {
            return rep(parent, x);
        }

        template <class ElementIterator>
        inline std::size_t count_sets(ElementIterator first, ElementIterator last)
        {
            std::size_t count = 0;  
            for ( ; first != last; ++first)
                if (get(parent, *first) == *first)
                    ++count;
            return count;
        }

        template <class ElementIterator>
        inline void normalize_sets(ElementIterator first, ElementIterator last)
        {
            for (; first != last; ++first) 
                boost::detail::normalize_node(parent, *first);
        }    

        template <class ElementIterator>
        inline void compress_sets(ElementIterator first, ElementIterator last)
        {
            for (; first != last; ++first) 
                boost::detail::find_representative_with_full_compression(parent, *first);
        }    
    protected:
        RankPA rank;
        ParentPA parent;
        SizePA size;
        FindCompress rep;
    };


} // namespace vega


#endif // VEGA_DISJOINT_SETS_HPP