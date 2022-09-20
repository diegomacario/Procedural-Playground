#ifndef VERLET_SYSTEM_H
#define VERLET_SYSTEM_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

class VerletSystem
{
public:

   VerletSystem() = default;

   void AddPoint(const glm::vec3& pos, const std::string& name);
   void AddBone(int indexOfPointA, int indexOfPointB, const std::string& name);
   void DrawBones(const glm::vec3& color);
   void StartSim(float step);
   void EnforceDistanceConstraints();
   void EndSim();
   void Step(float step);

   struct Point
   {
      glm::vec3   bindPos; // Initial position, good for skinning
      glm::vec3   currPos; // Current position
      glm::vec3   oldPos;  // Last frame position
      glm::vec3   tempPos; // Used as storage during frame update
      float       mass;
      bool        pinned;  // If pinned, can't move at all
      std::string name;
   };

   struct Bone
   {
      glm::ivec2  pointIndices;
      glm::vec2   length; // length[0] is minimum length, length[1] is maximum length
      std::string name;
      bool        enabled;
   };

   std::vector<Point> mPoints;
   std::vector<Bone>  mBones;
};

#endif
