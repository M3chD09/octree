#include "octree.h"

Octree::Octree()
{
    locations.reserve(getOctantCount(depth));
    locations.push_back(QVector3D(0, 0, 0));
    locations.push_back(QVector3D(-0.5f, -0.5f, -0.5f));
    locations.push_back(QVector3D(0.5f, -0.5f, -0.5f));
    locations.push_back(QVector3D(-0.5f, 0.5f, -0.5f));
    locations.push_back(QVector3D(0.5f, 0.5f, -0.5f));
    locations.push_back(QVector3D(-0.5f, -0.5f, 0.5f));
    locations.push_back(QVector3D(0.5f, -0.5f, 0.5f));
    locations.push_back(QVector3D(-0.5f, 0.5f, 0.5f));
    locations.push_back(QVector3D(0.5f, 0.5f, 0.5f));

    reset();
}

Octree::~Octree()
{
}

void Octree::reset()
{
    depth = 1;
    data.resize(getOctantCount(depth), OctreeNode { OctreeGray });
    for (size_t i = getOctantCount(depth - 1); i < data.size(); i++) {
        data[i] = OctreeNode { OctreeBlack };
    }
}

bool Octree::isMaxLeaf(size_t index)
{
    return maxDepth == getOctantDepth(index);
}

size_t Octree::getLeafCountMax(uint8_t depth)
{
    return (size_t)std::pow(8, depth);
}

size_t Octree::getOctantCount(uint8_t depth)
{
    return (std::pow(8, depth + 1) - 1) / 7;
}

size_t Octree::getOctantParent(size_t index)
{
    return (index - 1) / 8;
}

uint8_t Octree::getOctantDepth(size_t index)
{
    return (uint8_t)(std::log(7.0f * index + 1.0f) / std::log(8.0f));
}

uint8_t Octree::getOctantDirection(size_t index)
{
    uint8_t a = index % 8;
    if (a == 0)
        return 8;
    return a;
}

float Octree::getOctantVoxelSize(size_t index)
{
    return std::pow(0.5f, getOctantDepth(index) - 1);
}

QVector3D Octree::getOctantLocation(size_t index)
{
    if (index <= 8) {
        return locations[index];
    }

    if (index >= locations.size()) {
        locations.resize(index + 1, QVector3D(0, 0, 0));
    } else if (locations[index] != QVector3D(0, 0, 0)) {
        return locations[index];
    }

    QVector3D location = getOctantLocation(getOctantParent(index))
        + getOctantVoxelSize(index) * getOctantLocation(getOctantDirection(index));
    locations[index] = location;
    return location;
}

QRectF Octree::getOctantRect(size_t index)
{
    float voxelSize = getOctantVoxelSize(index);
    QRectF rect = QRectF(0, 0, voxelSize, voxelSize);
    rect.moveCenter(getOctantLocation(index).toPointF());
    return rect;
}

void Octree::subdivide(size_t index)
{
    if (isMaxLeaf(index)) {
        return;
    }

    if (depth != maxDepth) {
        depth++;
        data.resize(getOctantCount(depth), OctreeNode { OctreeWhite });
    }

    data[index] = OctreeNode { OctreeGray };
    for (size_t i = 8 * index + 1; i <= 8 * index + 8; i++) {
        data[i] = OctreeNode { OctreeBlack };
    }
}

void Octree::calculateVoxels()
{
    voxelLocations.clear();
    voxelSizes.clear();
    voxelLocations.reserve(getLeafCountMax(depth));
    voxelSizes.reserve(getLeafCountMax(depth));

    for (size_t i = 0; i < getOctantCount(depth); i++) {
        if (data[i].color == OctreeBlack) {
            voxelLocations.push_back(getOctantLocation(i));
            voxelSizes.push_back(getOctantVoxelSize(i));
        }
    }
}
