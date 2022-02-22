#ifndef OCTREE_H
#define OCTREE_H

#include <QRectF>
#include <QVector3D>
#include <vector>

enum OctreeColor {
    OctreeBlack,
    OctreeGray,
    OctreeWhite,
};

struct OctreeNode {
    OctreeColor color;
};

class Octree {
public:
    Octree();
    ~Octree();

    const uint8_t maxDepth = 7;

    uint8_t depth = 1;
    std::vector<OctreeNode> data;
    std::vector<QVector3D> locations;

    std::vector<QVector3D> voxelLocations;
    std::vector<float> voxelSizes;

    void reset();
    bool isMaxLeaf(size_t index);
    size_t getLeafCountMax(uint8_t depth);
    size_t getOctantCount(uint8_t depth);
    size_t getOctantParent(size_t index);
    uint8_t getOctantDepth(size_t index);
    uint8_t getOctantDirection(size_t index);
    float getOctantVoxelSize(size_t index);
    QVector3D getOctantLocation(size_t index);
    QRectF getOctantRect(size_t index);

    void subdivide(size_t index);
    void calculateVoxels();
};

#endif
