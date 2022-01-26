#ifndef UTIL_HPP
#define UTIL_HPP
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "raylib.h"

/* bool float_eq(float a, float b); */
inline bool float_eq(float a, float b) {
    return fabs(a - b) < std::numeric_limits<float>::epsilon();
}

struct Region {
    Vector3 center;
    Vector3 half_dimension;
    bool contains(Vector3 point) {
        bool X = (center.x - half_dimension.x) <= point.x &&
                 point.x < (center.x + half_dimension.x);
        bool Y = (center.y - half_dimension.y) <= point.y &&
                 point.y < (center.y + half_dimension.y);
        bool Z = (center.z - half_dimension.z) <= point.z &&
                 point.z < (center.z + half_dimension.z);
        return X && Y && Z;
    }
    bool intersects(Region other) {
        std::vector<Vector3> ps;
        ps.push_back(Vector3{other.center.x + other.half_dimension.x,
                             other.center.y + other.half_dimension.y,
                             other.center.z + other.half_dimension.z});
        ps.push_back(Vector3{other.center.x - other.half_dimension.x,
                             other.center.y + other.half_dimension.y,
                             other.center.z + other.half_dimension.z});
        ps.push_back(Vector3{other.center.x + other.half_dimension.x,
                             other.center.y - other.half_dimension.y,
                             other.center.z + other.half_dimension.z});
        ps.push_back(Vector3{other.center.x - other.half_dimension.x,
                             other.center.y - other.half_dimension.y,
                             other.center.z + other.half_dimension.z});
        ps.push_back(Vector3{other.center.x + other.half_dimension.x,
                             other.center.y + other.half_dimension.y,
                             other.center.z - other.half_dimension.z});
        ps.push_back(Vector3{other.center.x - other.half_dimension.x,
                             other.center.y + other.half_dimension.y,
                             other.center.z - other.half_dimension.z});
        ps.push_back(Vector3{other.center.x + other.half_dimension.x,
                             other.center.y - other.half_dimension.y,
                             other.center.z - other.half_dimension.z});
        ps.push_back(Vector3{other.center.x - other.half_dimension.x,
                             other.center.y - other.half_dimension.y,
                             other.center.z - other.half_dimension.z});
        for (auto p : ps) {
            if (contains(p)) return true;
        }
        return false;
    }
    Vector3 quarterDimension() {
        return Vector3{half_dimension.x / 2.f, half_dimension.y / 2.f,
                       half_dimension.z / 2.f};
    }
};

/*
template <class T>
class OctTree {
    using X = std::pair<Vector3, T>;  // pair of position and abstract data
   private:
    const static int limit = 2;
    static std::map<int, X> all;  // static mapping from (id -> X),
                                  //  facilitates efficient access by object id

    std::vector<int> points;  // the points local to this particular region
                              //  a store of their id
    Region region;            // the 3d rectangular region

    std::unique_ptr<OctTree<T>> A;
    std::unique_ptr<OctTree<T>> B;
    std::unique_ptr<OctTree<T>> C;
    std::unique_ptr<OctTree<T>> D;
    std::unique_ptr<OctTree<T>> E;
    std::unique_ptr<OctTree<T>> F;
    std::unique_ptr<OctTree<T>> G;
    std::unique_ptr<OctTree<T>> H;

    void subdivide() {  // instantiate the subregions
        if (A == nullptr) {
            auto x = region.center.x + region.half_dimension.x;
            auto y = region.center.y + region.half_dimension.y;
            auto z = region.center.z + region.half_dimension.z;
            auto r = Region{Vector3{x, y, z}, region.quarterDimension()};
            A = std::make_unique<OctTree<T>>(r);
        }
        if (B == nullptr) {
            auto x = region.center.x - region.half_dimension.x;
            auto y = region.center.y + region.half_dimension.y;
            auto z = region.center.z + region.half_dimension.z;
            auto r = Region{Vector3{x, y, z}, region.quarterDimension()};
            B = std::make_unique<OctTree<T>>(r);
        }
        if (C == nullptr) {
            auto x = region.center.x + region.half_dimension.x;
            auto y = region.center.y - region.half_dimension.y;
            auto z = region.center.z + region.half_dimension.z;
            auto r = Region{Vector3{x, y, z}, region.quarterDimension()};
            C = std::make_unique<OctTree<T>>(r);
        }
        if (D == nullptr) {
            auto x = region.center.x - region.half_dimension.x;
            auto y = region.center.y - region.half_dimension.y;
            auto z = region.center.z + region.half_dimension.z;
            auto r = Region{Vector3{x, y, z}, region.quarterDimension()};
            D = std::make_unique<OctTree<T>>(r);
        }
        if (E == nullptr) {
            auto x = region.center.x + region.half_dimension.x;
            auto y = region.center.y + region.half_dimension.y;
            auto z = region.center.z - region.half_dimension.z;
            auto r = Region{Vector3{x, y, z}, region.quarterDimension()};
            E = std::make_unique<OctTree<T>>(r);
        }
        if (F == nullptr) {
            auto x = region.center.x - region.half_dimension.x;
            auto y = region.center.y + region.half_dimension.y;
            auto z = region.center.z - region.half_dimension.z;
            auto r = Region{Vector3{x, y, z}, region.quarterDimension()};
            F = std::make_unique<OctTree<T>>(r);
        }
        if (G == nullptr) {
            auto x = region.center.x + region.half_dimension.x;
            auto y = region.center.y - region.half_dimension.y;
            auto z = region.center.z - region.half_dimension.z;
            auto r = Region{Vector3{x, y, z}, region.quarterDimension()};
            G = std::make_unique<OctTree<T>>(r);
        }
        if (H == nullptr) {
            auto x = region.center.x - region.half_dimension.x;
            auto y = region.center.y - region.half_dimension.y;
            auto z = region.center.z - region.half_dimension.z;
            auto r = Region{Vector3{x, y, z}, region.quarterDimension()};
            H = std::make_unique<OctTree<T>>(r);
        }
    }

   public:
    OctTree(Region region) { this->region = region; }
    void insert(T val, int id, float x, float y, float z) {
        // the root region must contain the point
        if (region.contains(Vector3{x, y, z})) {
            // the region must have available space
            if (points.size() < limit) {
                all[id] = std::make_pair(Vector3{x, y, z}, val);
                points.push_back(id);
            } else {
                subdivide();
                A.value().insert(val, id, x, y, z);
                B.value().insert(val, id, x, y, z);
                C.value().insert(val, id, x, y, z);
                D.value().insert(val, id, x, y, z);
                E.value().insert(val, id, x, y, z);
                F.value().insert(val, id, x, y, z);
                G.value().insert(val, id, x, y, z);
                H.value().insert(val, id, x, y, z);
            }
        }
    }
    void insert(T val, int id, Vector3 position) {
        insert(val, id, position.x, position.y, position.z);
    }
    bool contains(float x, float y, float z) {
        if (region.contains(Vector3{x, y, z})) {
            for (auto &id : points) {
                auto eps = std::numeric_limits<float>::epsilon();
                auto X =
                    all[id].x - eps < all[id].x && all[id].x < all[id].x + eps;
                auto Y =
                    all[id].y - eps < all[id].y && all[id].y < all[id].y + eps;
                auto Z =
                    all[id].z - eps < all[id].z && all[id].z < all[id].z + eps;
                if (X && Y && Z) {
                    return true;
                }
            }
            if (A.value().contains(x, y, z)) {
                return true;
            } else if (B.value().contains(x, y, z)) {
                return true;
            } else if (C.value().contains(x, y, z)) {
                return true;
            } else if (D.value().contains(x, y, z)) {
                return true;
            } else if (E.value().contains(x, y, z)) {
                return true;
            } else if (F.value().contains(x, y, z)) {
                return true;
            } else if (G.value().contains(x, y, z)) {
                return true;
            } else if (H.value().contains(x, y, z)) {
                return true;
            }
        }
        return false;
    }
    bool contains(Vector3 position) {
        return contains(position.x, position.y, position.z);
    }
    std::optional<X> get(int id) {
        if (all.contains(id)) {
            return all[id];
        } else {
            return {};
        }
    }
    bool contains(int id);
    void erase(float x, float y, float z);
    void erase(Vector3 position);
    void erase(int id);
    std::vector<X> query(Region region);
};
*/
#endif
