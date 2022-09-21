# define M_PI           3.14159265358979323846  /* pi */

#include "GLTFLoader.h"
#include "RearrangeBones.h"
#include "AnimatedCharacter.h"

AnimatedCharacter::AnimatedCharacter()
{
   mBaseColors = { glm::vec3(1.0f, 0.45f, 0.0f), // Belly
                   glm::vec3(1.0f, 1.0f, 0.0f),  // Chest
                   hexToColor(0xE01171),         // Left shoulder
                   hexToColor(0x071A52),         // Right shoulder
                   hexToColor(0x0F0766),         // Left thumb
                   hexToColor(0x0F0766),         // Left hand
                   hexToColor(0x59057B),         // Left forearm
                   hexToColor(0xA7FF83),         // Right thumb
                   hexToColor(0xA7FF83),         // Right hand
                   hexToColor(0x17B978),         // Right forearm
                   glm::vec3(0.5f, 0.5f, 0.5f),  // Head
                   glm::vec3(1.0f, 0.0f, 0.0f),  // Hip
                   hexToColor(0x59057B),         // Left foot
                   hexToColor(0xAB0E86),         // Left calf
                   hexToColor(0x17B978),         // Right foot
                   hexToColor(0x086972),         // Right calf
                   hexToColor(0xE01171),         // Left thigh
                   hexToColor(0x071A52),         // Right thigh
                   hexToColor(0xAB0E86),         // Left bicep
                   hexToColor(0x086972) };       // Right bicep

   mPointsBaseColors = { hexToColor(0x086972),           // Right bicep
                         hexToColor(0x17B978),           // Right foot
                         hexToColor(0xA7FF83),           // Right hand
                         glm::vec3(0.0f, 0.0f, 0.0f),    // Hip
                         glm::vec3(0.5f, 0.5f, 0.5f),    // Head
                         hexToColor(0x071A52),           // Right thigh
                         hexToColor(0x086972),           // Right knee
                         glm::vec3(1.0f, 1.0f, 0.0f),    // Neck
                         glm::vec3(1.0f, 0.0f, 0.0f),    // Belly
                         hexToColor(0x071A52),           // Right shoulder
                         glm::vec3(1.0f, 0.45f, 0.0f) }; // Chest
}

AnimatedCharacter::~AnimatedCharacter()
{

}

void AnimatedCharacter::initialize(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader)
{
   load(staticMeshWithoutUVsShader);

   // Starting point
   unsigned int jointIndex = 0;
   bool found = mBaseSkeleton.GetJointIndex("display_gibbon", jointIndex);
   if (found) { simple_pos = mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex).position; }
   simple_pos[1] = 0.0f;
   simple_pos[2] = 0.0f;

   // Init hand positions
   for (int i = 0; i < 4; ++i) {
      display.limb_targets[i] = simple_pos;
      walk.limb_targets[i] = simple_pos;
   }

   // Get transforms of each skeleton point
   Transform root;
   found = mPointsBaseSkeleton.GetJointIndex("points", jointIndex);
   if (found) { root = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform neck;
   found = mPointsBaseSkeleton.GetJointIndex("neck", jointIndex);
   if (found) { neck = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform stomach;
   found = mPointsBaseSkeleton.GetJointIndex("stomach", jointIndex);
   if (found) { stomach = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform pelvis;
   found = mPointsBaseSkeleton.GetJointIndex("pelvis", jointIndex);
   if (found) { pelvis = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform groin;
   found = mPointsBaseSkeleton.GetJointIndex("groin", jointIndex);
   if (found) { groin = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform head;
   found = mPointsBaseSkeleton.GetJointIndex("head", jointIndex);
   if (found) { head = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform shoulder;
   found = mPointsBaseSkeleton.GetJointIndex("shoulder", jointIndex);
   if (found) { shoulder = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform elbow;
   found = mPointsBaseSkeleton.GetJointIndex("elbow", jointIndex);
   if (found) { elbow = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform grip;
   found = mPointsBaseSkeleton.GetJointIndex("grip", jointIndex);
   if (found) { grip = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform hip;
   found = mPointsBaseSkeleton.GetJointIndex("hip", jointIndex);
   if (found) { hip = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform knee;
   found = mPointsBaseSkeleton.GetJointIndex("knee", jointIndex);
   if (found) { knee = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   Transform foot;
   found = mPointsBaseSkeleton.GetJointIndex("foot", jointIndex);
   if (found) { foot = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }

   // Set up bind poses for each bone
   found = mBaseSkeleton.GetJointIndex("DEF-head", jointIndex);
   if (found) { display_body.head.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-chest", jointIndex);
   if (found) { display_body.chest.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-belly", jointIndex);
   if (found) { display_body.belly.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-pelvis", jointIndex);
   if (found) { display_body.pelvis.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-upper_arm_L", jointIndex);
   if (found) { display_body.arm_top_l.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-forearm_L", jointIndex);
   if (found) { display_body.arm_bottom_l.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-upper_arm_R", jointIndex);
   if (found) { display_body.arm_top_r.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-forearm_R", jointIndex);
   if (found) { display_body.arm_bottom_r.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-thigh_L", jointIndex);
   if (found) { display_body.leg_top_l.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-shin_L", jointIndex);
   if (found) { display_body.leg_bottom_l.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-thigh_R", jointIndex);
   if (found) { display_body.leg_top_r.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   found = mBaseSkeleton.GetJointIndex("DEF-shin_R", jointIndex);
   if (found) { display_body.leg_bottom_r.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }

   // Adjust elbow to match arm transform
   elbow.position = display_body.arm_bottom_r.transform.position;

   // Set up initial IK poses (just used to get bone lengths later)
   arm_ik[0] = shoulder.position;
   arm_ik[1] = elbow.position;
   arm_ik[2] = grip.position;

   leg_ik[0] = hip.position;
   leg_ik[1] = display_body.leg_bottom_r.transform.position;
   leg_ik[2] = foot.position;

   float measured_arm_length = glm::distance(shoulder.position, elbow.position) + glm::distance(elbow.position, grip.position);

   // Set up movement system particles and bones
   for (int i = 0; i < 2; ++i) {
      VerletSystem* new_simple_rig;
      switch (i) {
      case 0:  new_simple_rig = &(display.simple_rig); break;
      default: new_simple_rig = &(walk.simple_rig); break;
      }

      new_simple_rig->AddPoint(shoulder.position, "shoulder_r");
      new_simple_rig->AddPoint(grip.position, "hand_r");
      new_simple_rig->AddPoint((shoulder.position + glm::vec3(1.0f, 0.0f, 0.0f) * (neck.position[0] - shoulder.position[0]) * 2.0f), "shoulder_l");
      new_simple_rig->AddPoint((grip.position + glm::vec3(1.0f, 0.0f, 0.0f) * (neck.position[0] - grip.position[0]) * 2.0f), "hand_l");
      new_simple_rig->AddPoint(glm::vec3(neck.position[0], hip.position[1], neck.position[2]), "body");
      new_simple_rig->mPoints[0].mass = 2.0f;
      new_simple_rig->mPoints[2].mass = 2.0f;
      new_simple_rig->mPoints[4].mass = 4.0f;

      new_simple_rig->AddBone(0, 1, "arm_r");
      new_simple_rig->mBones[new_simple_rig->mBones.size() - 1].length[1] = measured_arm_length; // Max length
      new_simple_rig->mBones[new_simple_rig->mBones.size() - 1].length[0] *= 0.4f; // Min lenght - Allow arm to flex
      new_simple_rig->AddBone(2, 3, "arm_l");
      new_simple_rig->mBones[new_simple_rig->mBones.size() - 1].length[1] = measured_arm_length;
      new_simple_rig->mBones[new_simple_rig->mBones.size() - 1].length[0] *= 0.4f;
      new_simple_rig->AddBone(0, 2, "tri_top");
      new_simple_rig->AddBone(0, 4, "tri_r");
      new_simple_rig->AddBone(2, 4, "tri_l");
   }

   // Set up full-body IK particles and bones
   complete.AddPoint(shoulder.position, "shoulder_r");
   complete.AddPoint(grip.position, "hand_r");
   complete.AddPoint((shoulder.position + glm::vec3(1.0f, 0.0f, 0.0f) * (neck.position[0] - shoulder.position[0]) * 2.0f), "shoulder_l");
   complete.AddPoint((grip.position + glm::vec3(1.0f, 0.0f, 0.0f) * (neck.position[0] - grip.position[0]) * 2.0f), "hand_l");
   complete.AddPoint(glm::vec3(neck.position[0], hip.position[1], neck.position[2]), "body");
   complete.AddPoint(head.position, "head");
   complete.AddPoint(neck.position, "neck");
   complete.AddPoint(stomach.position, "stomach"); // 7
   complete.AddPoint(pelvis.position, "hip"); // 8
   complete.AddPoint(groin.position, "groin");
   complete.AddPoint(hip.position, "hip_r");
   complete.AddPoint(foot.position, "foot_r");
   complete.AddPoint(hip.position + glm::vec3(1.0f, 0.0f, 0.0f) * (neck.position[0] - hip.position[0]) * 2.0f, "hip_l");
   complete.AddPoint(foot.position + glm::vec3(1.0f, 0.0f, 0.0f) * (neck.position[0] - foot.position[0]) * 2.0f, "foot_l");

   complete.AddBone(0, 1, "arm_r");
   complete.mBones[complete.mBones.size() - 1].length[1] = measured_arm_length;
   complete.mBones[complete.mBones.size() - 1].length[0] *= 0.4f;
   complete.AddBone(2, 3, "arm_l");
   complete.mBones[complete.mBones.size() - 1].length[1] = measured_arm_length;
   complete.mBones[complete.mBones.size() - 1].length[0] *= 0.4f;
   complete.AddBone(5, 6, "head");
   complete.AddBone(6, 7, "chest");
   complete.AddBone(7, 8, "belly");
   complete.AddBone(8, 9, "pelvis");
   complete.AddBone(10, 11, "leg_r");
   complete.mBones[complete.mBones.size() - 1].length[0] *= 0.4f;
   complete.AddBone(12, 13, "leg_l");
   complete.mBones[complete.mBones.size() - 1].length[0] *= 0.4f;

   // Create random branch 'terrain'
   srand(static_cast<unsigned>(glfwGetTime()));

   int num_segments = 40;
   float x = 0;
   float y = 0;
   for (int i = 0; i < num_segments + 1; ++i) {
      branches.AddPoint(glm::vec3(x, y, 0), "branch");
      x += 2.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (6.0f - 2.0f)));
      //y += -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - -3.0f)));
      //y = glm::clamp(y, -2.5f, 2.5f); // Make sure we stay on screen
   }
   for (int i = 0; i < num_segments; ++i) {
      branches.AddBone(i, i + 1, "branch");
   }

   // Set the initial pose
   mCurrentPose = mBaseSkeleton.GetBindPose();
}

void AnimatedCharacter::load(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader)
{
   // Load the character
   cgltf_data* data = LoadGLTFFile("resources/models/gibbon/gibbon.glb");
   mBaseSkeleton    = LoadSkeleton(data);
   mMeshes          = LoadStaticMeshes(data);
   FreeGLTFFile(data);
   
   // Rearrange the skeleton
   JointMap jointMap = RearrangeSkeleton(mBaseSkeleton);
   
   // Configure the VAOs of the character
   int positionsAttribLocOfAnimatedShader  = staticMeshWithoutUVsShader->getAttributeLocation("position");
   int normalsAttribLocOfAnimatedShader    = staticMeshWithoutUVsShader->getAttributeLocation("normal");
   for (unsigned int i = 0,
        size = static_cast<unsigned int>(mMeshes.size());
        i < size;
        ++i)
   {
      mMeshes[i].ConfigureVAO(positionsAttribLocOfAnimatedShader,
                              normalsAttribLocOfAnimatedShader,
                              -1,
                              -1,
                              -1);

      mMeshes[i].SetNodeIndex(jointMap[mMeshes[i].GetNodeIndex()]);
   }

   // Load the points
   data = LoadGLTFFile("resources/models/points/points.glb");
   mPointsBaseSkeleton = LoadSkeleton(data);
   mPointsMeshes = LoadStaticMeshes(data);
   FreeGLTFFile(data);

   // Rearrange the skeleton
   jointMap = RearrangeSkeleton(mPointsBaseSkeleton);

   // Configure the VAOs of the points
   for (unsigned int i = 0,
      size = static_cast<unsigned int>(mPointsMeshes.size());
      i < size;
      ++i)
   {
      mPointsMeshes[i].ConfigureVAO(positionsAttribLocOfAnimatedShader,
                                    normalsAttribLocOfAnimatedShader,
                                    -1,
                                    -1,
                                    -1);

      mPointsMeshes[i].SetNodeIndex(jointMap[mPointsMeshes[i].GetNodeIndex()]);
   }
}

void AnimatedCharacter::render(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader, const glm::mat4& viewMatrix, const glm::mat4& perspectiveProjectionMatrix)
{
   staticMeshWithoutUVsShader->use(true);
   staticMeshWithoutUVsShader->setUniformMat4("view", viewMatrix);
   staticMeshWithoutUVsShader->setUniformMat4("projection", perspectiveProjectionMatrix);

   // Loop over the character meshes and render each one
   for (unsigned int i = 0,
      size = static_cast<unsigned int>(mMeshes.size());
      i < size;
      ++i)
   {
      unsigned int nodeIndex = mMeshes[i].GetNodeIndex();
      Transform globalTransform = mCurrentPose.GetGlobalTransform(nodeIndex);
      staticMeshWithoutUVsShader->setUniformMat4("model", transformToMat4(globalTransform));
      staticMeshWithoutUVsShader->setUniformVec3("baseColor", mBaseColors[i]);

      mMeshes[i].Render();
   }

   // Loop over the points meshes and render each one
   /*
   for (unsigned int i = 0,
        size = static_cast<unsigned int>(mPointsMeshes.size());
        i < size;
        ++i)
   {
      unsigned int nodeIndex = mPointsMeshes[i].GetNodeIndex();
      //Transform offset;
      //offset.position = glm::vec3(0.0f, 0.0f, -0.2f);
      Transform globalTransform = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(nodeIndex);
      //Transform combined = combine(offset, globalTransform);
      staticMeshWithoutUVsShader->setUniformMat4("model", transformToMat4(globalTransform));
      staticMeshWithoutUVsShader->setUniformVec3("baseColor", mPointsBaseColors[i]);

      mPointsMeshes[i].Render();
   }
   */

   staticMeshWithoutUVsShader->use(false);
}

glm::vec3 AnimatedCharacter::hexToColor(int hex)
{
   float r = static_cast<float>(((hex >> 16) & 0xff)) / 255.0f;
   float g = static_cast<float>(((hex >> 8) & 0xff)) / 255.0f;
   float b = static_cast<float>((hex & 0xff)) / 255.0f;

   return glm::vec3(r, g, b);
}

float AnimatedCharacter::GetAngleGivenSides(float a, float b, float c)
{
   float top = (c * c - a * a - b * b);
   float divisor = (-2.0f * a * b);
   if (divisor == 0.0f) {
      return 0.0f;
   }

   // TODO: Make sure we are passing the right thing to glm::acos, and that it's returning the right thing
   return glm::acos(glm::clamp(top / divisor, -1.0f, 1.0f));
}

// Solve two bone IK problems
void AnimatedCharacter::ApplyTwoBoneIK(int start_id,
                                       int end_id,
                                       const glm::vec3& forward,
                                       const std::array<glm::vec3, 3>& ik,
                                       DisplayBone& top,
                                       DisplayBone& bottom,
                                       const std::vector<VerletSystem::Point>& points,
                                       const glm::vec3& old_axis,
                                       const glm::vec3& axis)
{
   const VerletSystem::Point& start = points[start_id];
   const VerletSystem::Point& end = points[end_id];

   // Get sides of triangle formed by upper and lower limb
   float dist_a     = glm::distance(ik[0], ik[1]);
   float dist_b     = glm::distance(ik[1], ik[2]);
   float dist_c     = glm::distance(start.currPos, end.currPos);
   float old_dist_c = glm::distance(ik[0], ik[2]);

   // Get angles of triangle
   float old_hinge_angle = GetAngleGivenSides(dist_a, dist_b, old_dist_c);
   float hinge_angle     = GetAngleGivenSides(dist_a, dist_b, dist_c);
   float old_base_angle  = GetAngleGivenSides(old_dist_c, dist_a, dist_b);
   float base_angle      = GetAngleGivenSides(dist_c, dist_a, dist_b);

   // TODO: The quaternion magic below might not work properly with Q::quat

   // Apply rotation of entire arm (shoulder->hand)
   Q::quat base_rotation = Q::lookRotation(end.currPos - start.currPos, forward) *
      Q::inverse(Q::lookRotation(end.bindPos - start.bindPos, glm::vec3(0.0f, 0.0f, 1.0f)));
   // Apply additional rotation from IK
   base_rotation = Q::angleAxis(base_angle, axis) * base_rotation *
      Q::inverse(Q::angleAxis(old_base_angle, old_axis));

   // Apply base and hinge rotations to actual display bones
   top.transform.position = top.bind_pos + (start.currPos - start.bindPos);
   top.transform.rotation = base_rotation * top.bind_rot;

   bottom.transform.position = top.transform.position + top.transform.rotation * Q::inverse(top.bind_rot) * (bottom.bind_pos - top.bind_pos);
   bottom.transform.rotation = Q::angleAxis(hinge_angle, axis) * base_rotation *
      Q::inverse(Q::angleAxis(old_hinge_angle, old_axis)) * bottom.bind_rot;
}

// Calculate bone transform that matches orientation of top and bottom points, and looks in the character "forward" direction
void AnimatedCharacter::ApplyBound(DisplayBone& part, const glm::vec3& forward, const glm::vec3& bind_forward, int start, int end)
{
   // Get midpoint and "up" direction (from start to end point)
   glm::vec3 up = glm::normalize(complete.mPoints[end].currPos - complete.mPoints[start].currPos);
   glm::vec3 bind_up = glm::normalize(complete.mPoints[end].bindPos - complete.mPoints[start].bindPos);
   glm::vec3 mid = (complete.mPoints[end].currPos + complete.mPoints[start].currPos) / 2.0f;
   glm::vec3 bind_mid = (complete.mPoints[end].bindPos + complete.mPoints[start].bindPos) / 2.0f;

   // TODO: The quaternion magic below might not work properly with Q::quat

   // Apply rotations
   Q::quat rotation = Q::inverse(Q::lookRotation(bind_up, bind_forward)) * Q::lookRotation(up, forward);
   part.transform.rotation = part.bind_rot * rotation;
   part.transform.position = mid + (rotation * (part.bind_pos - bind_mid));
}

// Get height of branch at given x coordinate
float AnimatedCharacter::BranchHeight(float x, int start_id, int end_id)
{
   const VerletSystem::Point& start = branches.mPoints[start_id];
   const VerletSystem::Point& end = branches.mPoints[end_id];
   float branch_t = (x - start.bindPos[0]) / (end.bindPos[0] - start.bindPos[0]);
   return glm::mix(start.currPos[1], end.currPos[1], branch_t);
}

// Get height of entire branch terrain at given x coordinate
float AnimatedCharacter::BranchesHeight(float x)
{
   for (int i = 0; i < branches.mBones.size(); ++i)
   {
      const glm::ivec2& point_ids = branches.mBones[i].pointIndices;
      if (x >= branches.mPoints[point_ids[0]].currPos[0] && x < branches.mPoints[point_ids[1]].currPos[0])
      {
         return BranchHeight(x, point_ids[0], point_ids[1]);
      }
   }

   // If not on terrain, extend horizontally forever
   if (x < 0.0f)
   {
      return branches.mPoints[0].currPos[1];
   }
   else
   {
      return branches.mPoints[branches.mPoints.size() - 1].currPos[1];
   }
}

// Prepare to draw next frame
void AnimatedCharacter::Update()
{
   // Use "arms" rig to derive full body IK rig
   {
      const std::vector<VerletSystem::Point>& points = display.simple_rig.mPoints;

      // Calculate midpoint and orientation of body triangle
      glm::vec3 bind_mid = (points[0].bindPos + points[2].bindPos + points[4].bindPos) / 3.0f;
      glm::vec3 mid = (points[0].currPos + points[2].currPos + points[4].currPos) / 3.0f;
      glm::vec3 forward = glm::normalize(glm::cross(points[0].currPos - points[2].currPos, points[0].currPos - points[4].currPos));
      glm::vec3 bind_forward = glm::normalize(glm::cross(points[0].bindPos - points[2].bindPos, points[0].bindPos - points[4].bindPos));
      glm::vec3 up = glm::normalize((points[0].currPos + points[2].currPos) / 2.0f - points[4].currPos);
      glm::vec3 bind_up = glm::normalize((points[0].bindPos + points[2].bindPos) / 2.0f - points[4].bindPos);

      // Copy hand and shoulder positions from simple rig
      for (int i = 0; i < 4; ++i) {
         complete.mPoints[i].currPos = points[i].currPos;
         complete.mPoints[i].pinned = true;
      }

      // TODO: The quaternion magic below might not work properly with Q::quat

      Q::quat body_rotation = Q::lookRotation(forward, up) * Q::inverse(Q::lookRotation(bind_forward, bind_up));

      // Set up spine, head and leg positions based on body rotation
      for (int i = 5; i < 14; ++i) {
         complete.mPoints[i].currPos = mid + (body_rotation * (complete.mPoints[i].bindPos - bind_mid));
         complete.mPoints[i].pinned = true;
      }

      // Apply body compression
      complete.mPoints[7].pinned = false;
      complete.mPoints[8].pinned = false;
      glm::vec3 old_hip = complete.mPoints[9].currPos;
      for (int i = 7; i <= 9; ++i) {
         complete.mPoints[i].currPos = glm::mix(complete.mPoints[i].currPos, complete.mPoints[6].currPos, body_compress_amount);
      }
      complete.mPoints[7].currPos -= forward * body_compress_amount * 0.2f;
      complete.mPoints[8].currPos -= forward * body_compress_amount * 0.2f;

      for (int i = 10; i < 14; ++i) {
         complete.mPoints[i].currPos += complete.mPoints[9].currPos - old_hip;
      }

      // Move feet to foot targets
      for (int i = 0; i < 2; ++i) {
         complete.mPoints[11 + i * 2].currPos = display.limb_targets[2 + i];
      }

      // Enforce bone length constraints
      for (int i = 0; i < 2; ++i) {
         complete.EnforceDistanceConstraints();
      }
   }

   // Apply full body IK rig to visual deformation bones
   {
      const std::vector<VerletSystem::Point>& points = complete.mPoints;

      // Get torso orientation and position
      glm::vec3 bind_mid     = (points[0].bindPos + points[2].bindPos + points[9].bindPos) / 3.0f;
      glm::vec3 mid          = (points[0].currPos + points[2].currPos + points[9].currPos) / 3.0f;
      glm::vec3 forward      = -glm::normalize(glm::cross(points[0].currPos - points[2].currPos, points[0].currPos - points[9].currPos));
      glm::vec3 bind_forward = -glm::normalize(glm::cross(points[0].bindPos - points[2].bindPos, points[0].bindPos - points[9].bindPos));
      glm::vec3 up           = glm::normalize((points[0].currPos + points[2].currPos) / 2.0f - points[9].currPos);
      glm::vec3 bind_up      = glm::normalize((points[0].bindPos + points[2].bindPos) / 2.0f - points[9].bindPos);

      // Apply core bones
      ApplyBound(display_body.head, forward, bind_forward, 5, 6);
      ApplyBound(display_body.chest, forward, bind_forward, 6, 7);
      ApplyBound(display_body.belly, forward, bind_forward, 7, 8);
      ApplyBound(display_body.pelvis, forward, bind_forward, 8, 9);

      // Arm IK
      for (int i = 0; i < 2; ++i) {
         DisplayBone& top = (i == 0) ? display_body.arm_top_r : display_body.arm_top_l;
         DisplayBone& bottom = (i == 0) ? display_body.arm_bottom_r : display_body.arm_bottom_l;

         int start_id = i * 2;
         int end_id = i * 2 + 1;
         const VerletSystem::Point& start = points[start_id];
         const VerletSystem::Point& end = points[end_id];

         // Adjust elbow target position
         float ik_driver = 1.0f;
         float ik_forward_amount = -ik_driver * 0.8f;
         float ik_up_amount = 0.1f + ik_driver * 0.5f;
         glm::vec3 elbow_point = ((points[2].currPos + points[0].currPos) * 0.5f + up * ik_up_amount + forward * ik_forward_amount);
         glm::vec3 bind_elbow_point = ((points[2].bindPos + points[0].bindPos) * 0.5f + bind_up * ik_up_amount + bind_forward * ik_forward_amount);

         glm::vec3 old_axis = glm::normalize(glm::cross((end.bindPos + start.bindPos) * 0.5f - bind_elbow_point, start.bindPos - end.bindPos));
         glm::vec3 axis = glm::normalize(glm::cross((end.currPos + start.currPos) * 0.5f - elbow_point, start.currPos - end.currPos));

         //ApplyTwoBoneIK(start_id, end_id, forward, arm_ik, top, bottom, complete.mPoints, old_axis, axis);
      }

      // Leg IK
      for (int i = 0; i < 2; ++i) {
         DisplayBone& top = (i == 0) ? display_body.leg_top_r : display_body.leg_top_l;
         DisplayBone& bottom = (i == 0) ? display_body.leg_bottom_r : display_body.leg_bottom_l;

         int start = i * 2 + 10;
         int end = i * 2 + 1 + 10;

         glm::vec3 leg_dir = points[end].currPos - points[start].currPos;

         // Get knee direction
         glm::vec2 leg_dir_flat = glm::normalize(glm::vec2(glm::dot(leg_dir, forward), glm::dot(leg_dir, up)));
         glm::vec3 leg_forward = leg_dir_flat[0] * up + leg_dir_flat[1] * -forward;

         // Get base whole-leg rotation
         Q::quat bind_rotation = Q::lookRotation(points[end].bindPos - points[start].bindPos, glm::vec3(0.0f, 0.0f, 1.0f));
         Q::quat rotation = Q::lookRotation(leg_dir, leg_forward) * bind_rotation;

         // Get knee bend axis
         glm::vec3 old_axis = bind_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
         glm::vec3 axis = rotation * glm::vec3(1.0f, 0.0f, 0.0f);

         //ApplyTwoBoneIK(start, end, leg_forward, leg_ik, top, bottom, complete.mPoints, old_axis, axis);
      }

      // Head look            
      // head_look_y: 50 = max look down, -70 = max look up
      // head_look_x: -90 to 90

      // TODO: I'm not sure if what I'm doing here is correct

      // Get head target in head transform space
      glm::vec3 target = glm::normalize(inverseTransformPoint(display_body.head.transform, look_target));
      // Using sin here is not correct (should be asin or something), but looks ok so keeping it for now
      float head_look_y = glm::degrees(glm::sin(target.x));
      // Flatten look direction to solve other rotation axis
      glm::vec3 temp = target;
      temp.x = 0.0f;
      temp = glm::normalize(temp);
      float head_look_x = glm::degrees(-glm::sin(temp.y));
      
      // Apply head transform
      display_body.head.transform.rotation = display_body.head.transform.rotation * Q::angleAxis(glm::radians(head_look_x), glm::vec3(1.0f, 0.0f, 0.0f)) * Q::angleAxis(glm::radians(head_look_y), glm::vec3(0.0f, 1.0f, 0.0f));
      if (head_look_y > 0.0f) {
         display_body.head.transform.position = display_body.head.transform.position + ((transformVector(display_body.head.transform, glm::vec3(1.0f, 0.0f, 0.0f))) * head_look_y * -0.001f);
      }
   }

   // Update the pose

   unsigned int jointIndex = 0;
   bool found = mBaseSkeleton.GetJointIndex("display_gibbon", jointIndex);
   Transform inverseBaseTransform;
   if (found) { inverseBaseTransform = inverse(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex)); }

   mCurrentPose.SetLocalTransform(display_body.chest.joint_index,        combine(inverseBaseTransform, display_body.chest.transform));
   mCurrentPose.SetLocalTransform(display_body.arm_top_l.joint_index,    combine(inverseBaseTransform, display_body.arm_top_l.transform));
   mCurrentPose.SetLocalTransform(display_body.arm_bottom_l.joint_index, combine(inverseBaseTransform, display_body.arm_bottom_l.transform));
   mCurrentPose.SetLocalTransform(display_body.arm_top_r.joint_index,    combine(inverseBaseTransform, display_body.arm_top_r.transform));
   mCurrentPose.SetLocalTransform(display_body.arm_bottom_r.joint_index, combine(inverseBaseTransform, display_body.arm_bottom_r.transform));
   mCurrentPose.SetLocalTransform(display_body.head.joint_index,         combine(inverseBaseTransform, display_body.head.transform));
   mCurrentPose.SetLocalTransform(display_body.belly.joint_index,        combine(inverseBaseTransform, display_body.belly.transform));
   mCurrentPose.SetLocalTransform(display_body.pelvis.joint_index,       combine(inverseBaseTransform, display_body.pelvis.transform));
   mCurrentPose.SetLocalTransform(display_body.leg_top_l.joint_index,    combine(inverseBaseTransform, display_body.leg_top_l.transform));
   mCurrentPose.SetLocalTransform(display_body.leg_bottom_l.joint_index, combine(inverseBaseTransform, display_body.leg_bottom_l.transform));
   mCurrentPose.SetLocalTransform(display_body.leg_top_r.joint_index,    combine(inverseBaseTransform, display_body.leg_top_r.transform));
   mCurrentPose.SetLocalTransform(display_body.leg_bottom_r.joint_index, combine(inverseBaseTransform, display_body.leg_bottom_r.transform));
}

float AnimatedCharacter::MoveTowards(float a, float b, float max_dist)
{
   float len = glm::distance(a, b);
   if (len < max_dist)
   {
      return b;
   }
   else
   {
      return a + (b - a) / len * max_dist;
   }
}

glm::vec3 AnimatedCharacter::MoveTowards(const glm::vec3& a, const glm::vec3& b, float max_dist)
{
   float len = glm::distance(a, b);
   if (len < max_dist)
   {
      return b;
   }
   else
   {
      return a + (b - a) / len * max_dist;
   }
}

void AnimatedCharacter::PreventHandsFromCrossingBody(VerletSystem& rig)
{
   for (int i = 0; i < 2; ++i)
   {
      glm::vec3 side_dir = glm::normalize(rig.mPoints[0].currPos - rig.mPoints[2].currPos) * static_cast<float>(1 - i * 2);
      float shoulder_d = glm::dot(rig.mPoints[i * 2].currPos, side_dir);
      float hand_d = glm::dot(rig.mPoints[i * 2 + 1].currPos, side_dir);
      float new_d = glm::max(hand_d, shoulder_d);
      rig.mPoints[i * 2 + 1].currPos += (new_d - hand_d) * side_dir;
   }
}

// Apply actual controls and physics
void AnimatedCharacter::Step(float step, const std::shared_ptr<Window>& window)
{
   // Transform controls to axes
   float horz_input = 0.0f;
   float vert_input = 0.0f;
   if (window->keyIsPressed(GLFW_KEY_D)) {
      horz_input = 1.0f;
   }
   if (window->keyIsPressed(GLFW_KEY_A)) {
      horz_input = -1.0f;
   }

   // Max speed of 7 m/s while running
   float max_speed = 7.0f;

   // Player can influence horizontal velocity
   simple_vel[0] += horz_input * step * 5.0f;
   simple_vel[0] = glm::clamp(simple_vel[0], -max_speed, max_speed);

   // Don't allow speed < 1.0 m/s, don't need to worry about idle animations in an endless runner
   if (horz_input == 0.0f && glm::abs(simple_vel[0]) < 1.0f) {
      simple_vel[0] = MoveTowards(simple_vel[0], simple_vel[0] >= 0.0f ? 1.0f : -1.0f, step);
   }

   // Smooth out position on branch by checking height forwards and back
   glm::vec3 future_pos = simple_pos + simple_vel * 0.1f;
   future_pos[1] = BranchesHeight(future_pos[0]);
   glm::vec3 past_pos = simple_pos + simple_vel * -0.1f;
   past_pos[1] = BranchesHeight(past_pos[0]);
   glm::vec3 smoothed_pos = (future_pos + past_pos + simple_pos) / 3.0f;

   // Get slope and use it to modify running speed
   glm::vec3 slope_vec = normalizeWithZeroLengthCheck(future_pos - simple_pos);
   float slope_speed_mult = glm::abs(slope_vec[0]);

   // Apply modified running speed to position
   glm::vec3 effective_vel = simple_vel * slope_speed_mult;
   simple_pos += effective_vel * step;

   simple_pos[1] = BranchesHeight(simple_pos[0]);
   simple_vel[1] = 0.0f;

   // If on ground, look in the direction you are moving
   glm::vec3 forward = glm::normalize(glm::cross(display.simple_rig.mPoints[0].currPos - display.simple_rig.mPoints[2].currPos, display.simple_rig.mPoints[0].currPos - display.simple_rig.mPoints[4].currPos));
   look_target[2] += forward[2];
   look_target = display_body.head.transform.position + forward * 0.1f;
   look_target += future_pos - past_pos;

   // Run animation
   {
      float time = static_cast<float>(glfwGetTime());

      // Vary between different gaits based on speed and time
      gallop_amount = glm::clamp(glm::abs(simple_vel[0]) / 4.0f + (glm::sin(time * 0.7f) - 1.0f) * 0.7f, 0.0f, 1.0f);
      quad_amount = glm::clamp((glm::sin(time * 2.3f) + glm::sin(time * 1.7f)), 0.0f, 1.0f);

      // Determine how far to lean forwards
      float walk_lean = glm::sin(time) * 0.2f + 0.3f;
      float gallop_lean = glm::sin(time) * 0.2f + 0.8f + quad_amount * 0.07f * glm::abs(effective_vel[0]);
      float lean = glm::mix(walk_lean, gallop_lean, gallop_amount);

      // Adjust stride frequency based on speed
      float speed_mult = 8.0f / (static_cast<float>(M_PI) * 2.0f) * glm::pow((glm::abs(effective_vel[0]) + 1.0f), 0.4f);
      walk_time += step * speed_mult;

      // Start sliding if above threshold speed and slope 
      float target_skate_amount = 0.0f;
      if (slope_vec[1] < -0.5f && glm::abs(effective_vel[0]) > 3.0f) {
         target_skate_amount = 1.0f;
      }
      skate_amount = MoveTowards(skate_amount, target_skate_amount, step * 3.0f);
      quad_amount = glm::mix(quad_amount, 0.0f, skate_amount);

      // Compress body during quadruped gallop
      walk.body_compress_amount = glm::mix((glm::sin((walk_time + quad_gallop_body_compress_offset) * static_cast<float>(M_PI) * 2.0f) + 1.0f) * quad_gallop_body_compress_amount * quad_amount * gallop_amount,
         0.1f,
         skate_amount);

      // Adjust COM height based on gait
      glm::vec3 target_com = simple_pos;
      target_com[1] = smoothed_pos[1];
      float walk_height = base_walk_height + glm::sin((walk_time + 0.25f) * static_cast<float>(M_PI) * 4.0f) * glm::abs(effective_vel[0]) * 0.015f / speed_mult + glm::abs(effective_vel[0]) * 0.01f;
      float gallop_height_ = glm::sin((walk_time + gallop_height_offset) * static_cast<float>(M_PI) * 2.0f) * gallop_height * glm::abs(effective_vel[0]) + gallop_height_base * (0.5f + glm::min(0.5f, glm::abs(effective_vel[0]) * 0.1f));
      target_com[1] += glm::mix(walk_height, gallop_height_, gallop_amount);
      target_com[1]  = glm::mix(target_com[1], simple_pos[1] + 0.5f, skate_amount);
      target_com[1]  = glm::mix(target_com[1], simple_pos[1], glm::abs(lean) * 0.15f);
      target_com[1]  = glm::mix(target_com[1], smoothed_pos[1], 0.0f);

      // Get ground slope again for use later
      glm::vec3 left = simple_pos - glm::vec3(0.1f, 0.0f, 0.0f);
      glm::vec3 right = simple_pos + glm::vec3(0.1f, 0.0f, 0.0f);
      left[1] = BranchesHeight(left[0]);
      right[1] = BranchesHeight(right[0]);
      glm::vec3 move_dir = glm::normalize(right - left);

      VerletSystem& rig = walk.simple_rig;

      // Apply quadruped run effect to arms
      if (gallop_amount * quad_amount > 0.0f) {
         for (int i = 0; i < 2; ++i) {
            // Get target position for hand
            walk.limb_targets[i] = rig.mPoints[i * 2].currPos;
            walk.limb_targets[i][1] = BranchesHeight(display.limb_targets[i][0]);
            float time_val = walk_time * static_cast<float>(M_PI) * 2.0f;
            walk.limb_targets[i][1] += (-glm::sin(time_val) + 0.5f) * gallop_arm_stride_height;
            walk.limb_targets[i] += move_dir * (glm::cos(time_val) + 0.5f) * gallop_arm_stride * effective_vel[0] / speed_mult;

            // Move hand towards target
            float pull_strength = gallop_amount * quad_amount * glm::min(1.0f, glm::abs(effective_vel[0]) * 0.2f);
            rig.mPoints[i * 2 + 1].currPos = MoveTowards(rig.mPoints[i * 2 + 1].currPos, walk.limb_targets[i], step * 0.5f * pull_strength * 4.0f);
         }
      }

      rig.StartSim(step);
      for (int j = 0; j < 4; ++j) {
         // Adjust all free points to match target COM
         float total_mass = 0.0f;
         glm::vec3 com = glm::vec3(0.0f, 0.0f, 0.0f);
         for (int i = 0; i < rig.mPoints.size(); ++i) {
            if (i != 1 && i != 3) {
               com += rig.mPoints[i].currPos * rig.mPoints[i].mass;
               total_mass += rig.mPoints[i].mass;
            }
         }
         com /= total_mass;
         glm::vec3 offset = target_com - com;
         for (int i = 0; i < rig.mPoints.size(); ++i) {
            if (i != 1 && i != 3) {
               rig.mPoints[i].currPos += offset * 0.2f;
            }
         }

         // Apply torque to keep torso upright and forward-facing
         float step_sqrd = step * step;
         float force = 20.0f;
         glm::vec3 forward2 = glm::normalize(glm::cross(rig.mPoints[0].currPos - rig.mPoints[2].currPos, rig.mPoints[0].currPos - rig.mPoints[4].currPos));
         glm::vec3 flat_forward = glm::normalize(glm::vec3(forward2[0], 0.0f, forward2[2]));
         glm::vec3 top_force = (lean * flat_forward + glm::vec3(0.0f, 1.0f, 0.0f)) * force;
         rig.mPoints[4].currPos += step_sqrd * -top_force;
         rig.mPoints[0].currPos += step_sqrd * top_force * 0.5f;
         rig.mPoints[2].currPos += step_sqrd * top_force * 0.5f;
         rig.mPoints[0].currPos[2] -= step_sqrd * effective_vel[0] * 2.0f * (1.0f - skate_amount);
         rig.mPoints[2].currPos[2] += step_sqrd * effective_vel[0] * 2.0f * (1.0f - skate_amount);

         // Add rotational force to body if needed
         for (int i = 0; i < 2; ++i) {
            float walk_rotate = (glm::cos((walk_time + tilt_offset) * static_cast<float>(M_PI) * 2.0f + static_cast<float>(M_PI) * i)) * 0.2f;
            float gallop_rotate = (glm::cos(static_cast<float>(M_PI) * i)) * (gallop_hip_rotate * (1.0f - quad_amount));
            float rotate = glm::mix(walk_rotate, gallop_rotate, gallop_amount);
            rig.mPoints[i * 2].currPos[0] += step_sqrd * -3.0f * rotate * effective_vel[0] / speed_mult;
         }

         // Move arms out to sides
         float speed = glm::abs(effective_vel[0]) / max_speed;
         for (int i = 0; i < 2; ++i) {
            float arms_up = glm::abs(speed * (glm::sin(time * ((i == 1) ? 2.5f : 2.3f)) * 0.3f + 0.7f)) * (1.0f - gallop_amount);
            rig.mPoints[1 + i * 2].currPos += step_sqrd * (rig.mPoints[0].currPos - rig.mPoints[2].currPos) * (1.5f + speed * 2.0f + arms_up * 2.0f) * static_cast<float>(1 - i * 2) * 2.0f;
            rig.mPoints[1 + i * 2].currPos[1] += step_sqrd * 10.0f * arms_up * arms_up;
            rig.mBones[i].length[1] = rig.mBones[0].length[0] / 0.4f * glm::mix((glm::mix(0.95f, 0.8f, glm::min(speed * 0.25f, 1.0f) + glm::sin(arms_up * static_cast<float>(M_PI)) * 0.1f)), 1.0f, gallop_amount);
         }

         PreventHandsFromCrossingBody(rig);

         // Make sure hands don't go through floor
         for (int i = 0; i < 2; ++i) {
            rig.mPoints[i * 2 + 1].currPos[1] = glm::max(rig.mPoints[i * 2 + 1].currPos[1], BranchesHeight(rig.mPoints[i * 2 + 1].currPos[0]));
         }

         for (int i = 0; i < 2; ++i) {
            rig.EnforceDistanceConstraints();
         }
      }
      rig.EndSim();

      // Calculate leg targets
      for (int i = 0; i < 2; ++i) {
         float offset = glm::mix(gallop_offset, quad_gallop_offset, quad_amount);
         float time_val = walk_time * static_cast<float>(M_PI) * 2.0f + static_cast<float>(M_PI) * i * offset;
         walk.limb_targets[2 + i] = simple_pos;
         walk.limb_targets[2 + i] += (move_dir * (glm::cos(walk_time * static_cast<float>(M_PI) * 2.0f + static_cast<float>(M_PI) * i)) * 0.2f - 0.03f) * effective_vel[0] / speed_mult * (1.0f - skate_amount) * (1.0f - gallop_amount);
         walk.limb_targets[2 + i] += (move_dir * (glm::cos(time_val)) * 0.2f - 0.03f) * effective_vel[0] / speed_mult * (1.0f - skate_amount) * gallop_stride * (1.0f - skate_amount) * gallop_amount;
         walk.limb_targets[2 + i] += (rig.mPoints[0].currPos - rig.mPoints[2].currPos) * (1.0f - 2.0f * i) * (0.3f + 0.3f * skate_amount);
         walk.limb_targets[2 + i][1] = BranchesHeight(walk.limb_targets[2 + i][0]);
         walk.limb_targets[2 + i][1] += (-glm::sin(walk_time * static_cast<float>(M_PI) * 2.0f + static_cast<float>(M_PI) * i) + 1.0f) * 0.2f * (glm::pow(glm::abs(effective_vel[0]) + 1.0f, 0.3f) - 1.0f) * (1.0f - skate_amount) * (1.0f - gallop_amount);
         walk.limb_targets[2 + i][1] += (-glm::sin(time_val) + 1.0f) * gallop_stride_height * (glm::pow(glm::abs(effective_vel[0]) + 1.0f, 0.3f) - 1.0f) * (1.0f - skate_amount) * gallop_amount;
      }
   }

   { // Combine source rigs into display rig
       // Interpolate between source rigs
      for (int i = 0; i < display.simple_rig.mPoints.size(); ++i) {
         display.simple_rig.mPoints[i].oldPos = display.simple_rig.mPoints[i].currPos;
         display.simple_rig.mPoints[i].currPos = walk.simple_rig.mPoints[i].currPos;
      }
      for (int i = 0; i < 2; ++i) {
         display.simple_rig.EnforceDistanceConstraints();
      }
      for (int i = 0; i < 4; ++i) {
         display.limb_targets[i] = walk.limb_targets[i];
      }
      body_compress_amount = walk.body_compress_amount;
   }

   // TODO: Move game camera to track character

   // Move game camera to track character       
   //{
   //   var cam_pos = Camera.main.transform.position;
   //   // Get COM
   //   float total_mass = 0.0f;
   //   var com = new float3(0.0f, 0.0f, 0.0f);
   //   var points = display.simple_rig.points;
   //   for (int i = 0; i < points.Count; ++i) {
   //      com += points[i].pos * points[i].mass;
   //      total_mass += points[i].mass;
   //   }
   //   com /= total_mass;
   //   // Track COM position
   //   cam_pos[0] = com[0] + simple_vel[0] * 0.1f;
   //   Camera.main.transform.position = cam_pos;
   //}
}
