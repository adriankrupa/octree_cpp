//
// Created by Adrian Krupa on 11/2/17.
//

#ifndef OCTREE_CPP_OCTREE_CPP_H
#define OCTREE_CPP_OCTREE_CPP_H

#include <type_traits>
#include <memory>

namespace AKOctree {

    template<class Precision = float>
    struct OctreeVec3 {

        static_assert(std::is_arithmetic<Precision>::value, "Precision must be arithmetic!");

        Precision x = Precision(0);
        Precision y = Precision(0);
        Precision z = Precision(0);

        OctreeVec3() : x(0), y(0), z(0) {}

        explicit OctreeVec3(Precision x) : x(x), y(x), z(x) {}

        OctreeVec3(Precision x, Precision y, Precision z) : x(x), y(y), z(z) {}

        OctreeVec3 &operator+=(const OctreeVec3 &rhs);

        OctreeVec3 &operator-=(const OctreeVec3 &rhs);
    };

    template<class LeafDataType, class NodeDataType = LeafDataType, class Precision = float>
    class OctreeAgent {
    public:
        virtual ~OctreeAgent() = default;

        virtual bool isItemOverlappingCell(const LeafDataType *item,
                                           const OctreeVec3<Precision> &cellCenter,
                                           const Precision &cellRadius) const = 0;
    protected:
        OctreeAgent() = default;
    };

    template<class LeafDataType, class NodeDataType = LeafDataType, class Precision = float>
    class Octree {

        static_assert(std::is_arithmetic<Precision>::value, "Precision must be arithmetic!");

    public:
        explicit Octree(uint maxItemsPerCell,
                        uint threads = 1) : Octree(maxItemsPerCell,
                                                   OctreeVec3<Precision>(),
                                                   Precision(1),
                                                   threads) {}

        Octree(uint maxItemsPerCell,
               OctreeVec3<Precision> center,
               Precision radius,
               uint threads);

        const uint getMaxItemsPerCell() const;
        void insert(const std::shared_ptr<LeafDataType> item, const std::shared_ptr<OctreeAgent<LeafDataType, NodeDataType, Precision>> agent);

    private:
        OctreeVec3<Precision> center;
        Precision radius;
        const uint maxItemsPerCell = 1;

    private:
        uint threads = 1;
    };


    template<class P> // P=Precision
    OctreeVec3<P>& OctreeVec3<P>::operator+=(const OctreeVec3<P>& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    template<class P> // P=Precision
    OctreeVec3<P>& OctreeVec3<P>::operator-=(const OctreeVec3<P>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    template<class P> // P=Precision
    OctreeVec3<P> operator+(OctreeVec3<P> lhs, const OctreeVec3<P>& rhs) {
        lhs += rhs;
        return lhs;
    }

    template<class P> // P=Precision
    OctreeVec3<P> operator/(OctreeVec3<P> lhs, const P& rhs) {
        lhs.x /= rhs;
        lhs.y /= rhs;
        lhs.z /= rhs;
        return lhs;
    }

    template<class P> // P=Precision
    OctreeVec3<P> operator-(OctreeVec3<P> lhs, const OctreeVec3<P>& rhs) {
        lhs -= rhs;
        return lhs;
    }


    template<class L, class N, class P> //L=LeafDataType N=NodeDataType P=Precision
    Octree<L, N, P>::Octree(unsigned int maxItemsPerCell,
                            OctreeVec3<P> center,
                            P radius,
                            unsigned int threads) : center(center),
                                                          radius(radius),
                                                          maxItemsPerCell(maxItemsPerCell),
                                                          threads(threads) {
    }

    template<class L, class N, class P>
    const uint Octree<L, N, P>::getMaxItemsPerCell() const {
        return maxItemsPerCell;
    }

    template<class L, class N, class P>
    void Octree<L, N, P>::insert(const std::shared_ptr<L> item,
                        const std::shared_ptr<OctreeAgent<L, N, P>> agent) {
        if (agent->isItemOverlappingCell(item, center, radius)) {
            if(root->insert(item, agent)) {
                itemsCount.fetch_add(1);
            }
        }
    }
}

#endif //OCTREE_CPP_OCTREE_CPP_H
