#ifndef REARRANGE_BONES_H
#define REARRANGE_BONES_H

#include <map>
#include "Skeleton.h"
#include "AnimatedMesh.h"

typedef std::map<int, int> JointMap;

JointMap RearrangeSkeleton(Skeleton& skeleton);
void     RearrangeMesh(AnimatedMesh& mesh, JointMap& jointMap);

#endif
