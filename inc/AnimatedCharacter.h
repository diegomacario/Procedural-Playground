#ifndef ANIMATED_CHARACTER_H
#define ANIMATED_CHARACTER_H

#include <memory>

#include "Shader.h"
#include "AnimatedMesh.h"
#include "VerletSystem.h"
#include "Window.h"
#include "Line.h"

struct DisplayBone
{
   Transform transform;
   Q::quat bind_rot;
   glm::vec3 bind_pos;
   unsigned int joint_index;

   void Bind(Transform bindTransform, unsigned int jointIndex)
   {
      transform = bindTransform;
      bind_pos = bindTransform.position;
      bind_rot = bindTransform.rotation;
      joint_index = jointIndex;
   }
};

struct DisplayBody
{
   DisplayBone chest;
   DisplayBone arm_top_l;
   DisplayBone arm_bottom_l;
   DisplayBone arm_top_r;
   DisplayBone arm_bottom_r;
   DisplayBone head;
   DisplayBone belly;
   DisplayBone pelvis;
   DisplayBone leg_top_l;
   DisplayBone leg_bottom_l;
   DisplayBone leg_top_r;
   DisplayBone leg_bottom_r;
};

struct MovementSystem
{
   glm::vec3 target_com;
   // 0 is right hand
   // 1 is left hand
   // 2 is right foot
   // 3 is left foot
   std::array<glm::vec3, 4> limb_targets;
   VerletSystem simple_rig;
   float body_compress_amount = 0.0f;
};

class AnimatedCharacter
{
public:

   AnimatedCharacter();
   ~AnimatedCharacter();

   void      initialize(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader);

   // Apply actual controls and physics
   void      Step(float step, const std::shared_ptr<Window>& window);

   // Prepare to draw next frame
   void      Update();

   void      render(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader, const glm::mat4& viewMatrix, const glm::mat4& perspectiveProjectionMatrix);

   glm::vec3 getPosition() const { return simple_pos; }

private:

   void      load(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader);

   glm::vec3 hexToColor(int hex);

   // ---

   float GetAngleGivenSides(float a, float b, float c);

   // Solve two bone IK problems
   void ApplyTwoBoneIK(int start_id,
                       int end_id,
                       const glm::vec3& forward,
                       const std::array<glm::vec3, 3>& ik,
                       DisplayBone& top,
                       DisplayBone& bottom,
                       const std::vector<VerletSystem::Point>& points,
                       const glm::vec3& old_axis,
                       const glm::vec3& axis);

   // Calculate bone transform that matches orientation of top and bottom points, and looks in the character "forward" direction
   void ApplyBound(DisplayBone& part, const glm::vec3& forward, const glm::vec3& bind_forward, int start, int end);

   // Get height of branch at given x coordinate
   float BranchHeight(float x, int start_id, int end_id);

   // Get height of entire branch terrain at given x coordinate
   float BranchesHeight(float x);

   float MoveTowards(float a, float b, float max_dist);

   glm::vec3 MoveTowards(const glm::vec3& a, const glm::vec3& b, float max_dist);

   void PreventHandsFromCrossingBody(VerletSystem& rig);

   // ---

   Skeleton                  mBaseSkeleton;
   std::vector<AnimatedMesh> mMeshes;
   std::vector<glm::vec3>    mBaseColors;

   Skeleton                  mPointsBaseSkeleton;
   std::vector<AnimatedMesh> mPointsMeshes;
   std::vector<glm::vec3>    mPointsBaseColors;

   Pose                      mCurrentPose;

   // ---

   DisplayBody display_body;

   // Particle simulation systems
   VerletSystem complete;
   VerletSystem branches;

   // Initial point positions for IK
   // 0 is shoulder
   // 1 is elbow
   // 2 is grip
   std::array<glm::vec3, 3> arm_ik;
   // 0 is hip
   // 1 is knee
   // 2 is foot
   std::array<glm::vec3, 3> leg_ik;

   MovementSystem walk;
   MovementSystem display;

   // Simple rig point ids
   // I just remembered the int values and used them directly, but this is good for reference
   const int p_shoulder_r = 0;
   const int p_hand_r = 1;
   const int p_shoulder_l = 2;
   const int p_hand_l = 3;
   const int p_base = 4;

   // Simple character particle information
   glm::vec3 simple_pos;
   glm::vec3 simple_vel = glm::vec3(0.0f, 0.0f, 0.0f);

   // Time marker in animation sequences (so cycles can speed up and slow down without losing continuity)
   float walk_time = 0.0f;

   glm::vec3 look_target; // For head look IK

   float body_compress_amount = 0.0f; // Used to shorten the distance between neck and hips if necessary, e.g. during quadruped gallop
   float skate_amount = 0.0f; // Used to interpolate into and out of sliding state

   // Various parameters that were being used to tune an animation
   float base_walk_height = 0.7f;
   float tilt_offset = 0.81f;
   float gallop_offset = 0.55f; // For biped gallop
   float quad_gallop_offset = 0.25f; // For quadruped gallop
   float gallop_stride = 1.0f;
   float gallop_stride_height = 0.2f;
   float gallop_hip_rotate = -1.3f;
   float gallop_height_offset = 0.6f;
   float gallop_height = 0.012f;
   float gallop_height_base = 0.8f;
   float gallop_lean = 1.5f;
   float gallop_arm_stride_height = 0.4f;
   float gallop_arm_stride = 0.4f;
   float quad_amount = 0.0f;
   float gallop_amount = 0.0f;
   float quad_gallop_body_compress_offset = 0.4f;
   float quad_gallop_body_compress_amount = 0.15f;

   // ---

   std::shared_ptr<Shader> mLineShader;
   std::vector<Line>       mLines;
};

#endif
