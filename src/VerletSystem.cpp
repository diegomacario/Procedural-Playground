#include "VerletSystem.h"

void VerletSystem::AddPoint(const glm::vec3& pos, const std::string& name)
{
   Point point;
   point.bindPos = pos;
   point.currPos = pos;
   point.oldPos  = pos;
   point.pinned  = false;
   point.name    = name;
   point.mass    = 1.0f;
   mPoints.push_back(point);
}

void VerletSystem::AddBone(int indexOfPointA, int indexOfPointB, const std::string& name)
{
   Bone bone;
   bone.pointIndices[0] = indexOfPointA;
   bone.pointIndices[1] = indexOfPointB;
   bone.length[0]       = glm::distance(mPoints[indexOfPointB].currPos, mPoints[indexOfPointA].currPos);
   bone.length[1]       = bone.length[0];
   bone.name            = name;
   bone.enabled         = true;
   mBones.push_back(bone);
}

void VerletSystem::DrawBones(const std::shared_ptr<Shader>& lineShader, const glm::vec3& color)
{
   // Update the bone lines
   mBoneLines.clear();
   for(int i = 0; i < mBones.size(); ++i)
   {
      if (mBones[i].enabled)
      {
         mBoneLines.emplace_back(mPoints[mBones[i].pointIndices[0]].currPos,
                                 mPoints[mBones[i].pointIndices[1]].currPos,
                                 color);
      }
   }

   // Loop over the bone lines and render each one
   for (unsigned int i = 0,
        size = static_cast<unsigned int>(mBoneLines.size());
        i < size;
        ++i)
   {
      mBoneLines[i].render(lineShader);
   }
}

void VerletSystem::StartSim(float step)
{
   // Apply gravity and previous velocity, and store position in temp
   float timeSquared = step * step;
   glm::vec3 acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
   for (int i = 0; i < mPoints.size(); ++i)
   {
      Point& point = mPoints[i];
      point.tempPos = point.currPos;
      if (!point.pinned)
      {
         point.currPos = point.currPos + (point.currPos - point.oldPos) +  acceleration * timeSquared;
      }
   }
}

 void VerletSystem::EnforceDistanceConstraints()
 {
    for (int i = 0; i < mBones.size(); ++i)
    {
       if (!mBones[i].enabled)
       {
          continue;
       }

       Bone& bone = mBones[i];
       int numPinned = 0;
       if (mPoints[bone.pointIndices[0]].pinned) { ++numPinned; }
       if (mPoints[bone.pointIndices[1]].pinned) { ++numPinned; }

       // If both points are pinned, we can't move either of them, so we don't do anything
       if (numPinned < 2)
       {
          float currLen = glm::distance(mPoints[bone.pointIndices[0]].currPos, mPoints[bone.pointIndices[1]].currPos);
          if (currLen != 0.0f)
          {
             if (numPinned == 1)
             {
                // Only one point is pinned, so we move the free one
                int pinned = bone.pointIndices[1];
                int unpinned = bone.pointIndices[0];
                if (mPoints[bone.pointIndices[0]].pinned)
                {
                   pinned = bone.pointIndices[0];
                   unpinned = bone.pointIndices[1];
                }

                Point& unpinnedPoint = mPoints[unpinned];
                if (currLen < bone.length[0])
                {
                   unpinnedPoint.currPos = mPoints[pinned].currPos + (mPoints[unpinned].currPos - mPoints[pinned].currPos) / currLen * bone.length[0];
                }
                else if (currLen > bone.length[1])
                {
                   unpinnedPoint.currPos = mPoints[pinned].currPos + (mPoints[unpinned].currPos - mPoints[pinned].currPos) / currLen * bone.length[1];
                }
             }
             else
             {
                // Both points are unpinned, so we move both of them
                glm::vec3 offset = (mPoints[bone.pointIndices[1]].currPos - mPoints[bone.pointIndices[0]].currPos) / currLen;
                float relMass = mPoints[bone.pointIndices[1]].mass / (mPoints[bone.pointIndices[0]].mass + mPoints[bone.pointIndices[1]].mass);
                glm::vec3 mid = mPoints[bone.pointIndices[0]].currPos * (1.0f-relMass) + mPoints[bone.pointIndices[1]].currPos * relMass;
                if (currLen < bone.length[0])
                {
                   mPoints[bone.pointIndices[0]].currPos = mid - offset * bone.length[0] * relMass;
                   mPoints[bone.pointIndices[1]].currPos = mid + offset * bone.length[0] * (1.0f-relMass);
                }
                else if (currLen > bone.length[1])
                {
                   mPoints[bone.pointIndices[0]].currPos = mid - offset * bone.length[1] * relMass;
                   mPoints[bone.pointIndices[1]].currPos = mid + offset * bone.length[1] * (1.0f-relMass);
                }
             }
          }
       }
    }
 }

 void VerletSystem::EndSim()
 {
    for(int i=0; i < mPoints.size(); ++i)
    {
       Point& point = mPoints[i];
       point.oldPos = point.tempPos;
    }
 }

 void VerletSystem::Step(float step)
 {
    StartSim(step);
    EnforceDistanceConstraints();
    EndSim();
 }
