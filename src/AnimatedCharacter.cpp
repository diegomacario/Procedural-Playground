#include <glm/ext/scalar_constants.hpp>

#include "resource_manager.h"
#include "shader_loader.h"
#include "GLTFLoader.h"
#include "RearrangeBones.h"
#include "AnimatedCharacter.h"

AnimatedCharacter::AnimatedCharacter()
{
   mBaseColors = { hexToColor(0xFF7300),         // Belly
                   hexToColor(0xFFFF00),         // Chest
                   hexToColor(0xE01171),         // Left shoulder
                   hexToColor(0x071A52),         // Right shoulder
                   hexToColor(0x0F0766),         // Left thumb
                   hexToColor(0x0F0766),         // Left hand
                   hexToColor(0x59057B),         // Left forearm
                   hexToColor(0xA7FF83),         // Right thumb
                   hexToColor(0xA7FF83),         // Right hand
                   hexToColor(0x17B978),         // Right forearm
                   hexToColor(0x808080),         // Head
                   hexToColor(0xFF0000),         // Hip
                   hexToColor(0x59057B),         // Left foot
                   hexToColor(0xAB0E86),         // Left calf
                   hexToColor(0x17B978),         // Right foot
                   hexToColor(0x086972),         // Right calf
                   hexToColor(0xE01171),         // Left thigh
                   hexToColor(0x071A52),         // Right thigh
                   hexToColor(0xAB0E86),         // Left bicep
                   hexToColor(0x086972) };       // Right bicep

   mPointsBaseColors = { hexToColor(0x086972),   // Right bicep
                         hexToColor(0x17B978),   // Right foot
                         hexToColor(0xA7FF83),   // Right hand
                         hexToColor(0x000000),   // Hip
                         hexToColor(0x808080),   // Head
                         hexToColor(0x071A52),   // Right thigh
                         hexToColor(0x086972),   // Right knee
                         hexToColor(0xFFFF00),   // Neck
                         hexToColor(0xFF0000),   // Belly
                         hexToColor(0x071A52),   // Right shoulder
                         hexToColor(0xFF7300) }; // Chest

   mLineShader = ResourceManager<Shader>().loadUnmanagedResource<ShaderLoader>("resources/shaders/line.vert",
                                                                               "resources/shaders/line.frag");
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
   else { std::cout << "Couldn't find joint: " << "display_gibbon" << '\n'; }
   simple_pos[1] = 0.0f;
   simple_pos[2] = 0.0f;

   // Init hand positions
   for (int i = 0; i < 4; ++i)
   {
      display.limb_targets[i] = simple_pos;
      walk.limb_targets[i] = simple_pos;
   }

   // Get transforms of each skeleton point
   Transform root;
   found = mPointsBaseSkeleton.GetJointIndex("points", jointIndex);
   if (found) { root = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "points" << '\n'; }

   Transform neck;
   found = mPointsBaseSkeleton.GetJointIndex("neck", jointIndex);
   if (found) { neck = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "neck" << '\n'; }

   Transform stomach;
   found = mPointsBaseSkeleton.GetJointIndex("stomach", jointIndex);
   if (found) { stomach = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "stomach" << '\n'; }

   Transform pelvis;
   found = mPointsBaseSkeleton.GetJointIndex("pelvis", jointIndex);
   if (found) { pelvis = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "pelvis" << '\n'; }

   Transform groin;
   found = mPointsBaseSkeleton.GetJointIndex("groin", jointIndex);
   if (found) { groin = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "groin" << '\n'; }

   Transform head;
   found = mPointsBaseSkeleton.GetJointIndex("head", jointIndex);
   if (found) { head = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "head" << '\n'; }

   Transform shoulder;
   found = mPointsBaseSkeleton.GetJointIndex("shoulder", jointIndex);
   if (found) { shoulder = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "shoulder" << '\n'; }

   Transform elbow;
   found = mPointsBaseSkeleton.GetJointIndex("elbow", jointIndex);
   if (found) { elbow = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "elbow" << '\n'; }

   Transform grip;
   found = mPointsBaseSkeleton.GetJointIndex("grip", jointIndex);
   if (found) { grip = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "grip" << '\n'; }

   Transform hip;
   found = mPointsBaseSkeleton.GetJointIndex("hip", jointIndex);
   if (found) { hip = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "hip" << '\n'; }

   Transform knee;
   found = mPointsBaseSkeleton.GetJointIndex("knee", jointIndex);
   if (found) { knee = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "knee" << '\n'; }

   Transform foot;
   found = mPointsBaseSkeleton.GetJointIndex("foot", jointIndex);
   if (found) { foot = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex); }
   else { std::cout << "Couldn't find joint: " << "foot" << '\n'; }

   // Set up bind poses for each bone
   found = mBaseSkeleton.GetJointIndex("DEF-head", jointIndex);
   if (found) { display_body.head.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-head" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-chest", jointIndex);
   if (found) { display_body.chest.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-chest" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-belly", jointIndex);
   if (found) { display_body.belly.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-belly" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-pelvis", jointIndex);
   if (found) { display_body.pelvis.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-pelvis" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-upper_arm_L", jointIndex);
   if (found) { display_body.arm_top_l.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-upper_arm_L" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-forearm_L", jointIndex);
   if (found) { display_body.arm_bottom_l.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-forearm_L" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-upper_arm_R", jointIndex);
   if (found) { display_body.arm_top_r.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-upper_arm_R" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-forearm_R", jointIndex);
   if (found) { display_body.arm_bottom_r.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-forearm_R" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-thigh_L", jointIndex);
   if (found) { display_body.leg_top_l.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-thigh_L" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-shin_L", jointIndex);
   if (found) { display_body.leg_bottom_l.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-shin_L" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-thigh_R", jointIndex);
   if (found) { display_body.leg_top_r.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-thigh_R" << '\n'; }

   found = mBaseSkeleton.GetJointIndex("DEF-shin_R", jointIndex);
   if (found) { display_body.leg_bottom_r.Bind(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex), jointIndex); }
   else { std::cout << "Couldn't find joint: " << "DEF-shin_R" << '\n'; }

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
   for (int i = 0; i < 2; ++i)
   {
      VerletSystem& new_simple_rig = (i == 0) ? display.simple_rig : walk.simple_rig;

      new_simple_rig.AddPoint(shoulder.position, "shoulder_r");
      new_simple_rig.AddPoint(grip.position, "hand_r");
      new_simple_rig.AddPoint((shoulder.position + glm::vec3(1.0f, 0.0f, 0.0f) * (neck.position[0] - shoulder.position[0]) * 2.0f), "shoulder_l");
      new_simple_rig.AddPoint((grip.position + glm::vec3(1.0f, 0.0f, 0.0f) * (neck.position[0] - grip.position[0]) * 2.0f), "hand_l");
      new_simple_rig.AddPoint(glm::vec3(neck.position[0], hip.position[1], neck.position[2]), "body");
      new_simple_rig.mPoints[0].mass = 2.0f;
      new_simple_rig.mPoints[2].mass = 2.0f;
      new_simple_rig.mPoints[4].mass = 4.0f;

      new_simple_rig.AddBone(0, 1, "arm_r");
      new_simple_rig.mBones[new_simple_rig.mBones.size() - 1].length[1] = measured_arm_length; // Max length
      new_simple_rig.mBones[new_simple_rig.mBones.size() - 1].length[0] *= 0.4f;               // Min length
      new_simple_rig.AddBone(2, 3, "arm_l");
      new_simple_rig.mBones[new_simple_rig.mBones.size() - 1].length[1] = measured_arm_length; // Max length
      new_simple_rig.mBones[new_simple_rig.mBones.size() - 1].length[0] *= 0.4f;               // Min length
      new_simple_rig.AddBone(0, 2, "tri_top");
      new_simple_rig.AddBone(0, 4, "tri_r");
      new_simple_rig.AddBone(2, 4, "tri_l");
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
   complete.mBones[complete.mBones.size() - 1].length[1] = measured_arm_length; // Max length
   complete.mBones[complete.mBones.size() - 1].length[0] *= 0.4f;               // Min length
   complete.AddBone(2, 3, "arm_l");
   complete.mBones[complete.mBones.size() - 1].length[1] = measured_arm_length; // Max length
   complete.mBones[complete.mBones.size() - 1].length[0] *= 0.4f;               // Min length
   complete.AddBone(5, 6, "head");
   complete.AddBone(6, 7, "chest");
   complete.AddBone(7, 8, "belly");
   complete.AddBone(8, 9, "pelvis");
   complete.AddBone(10, 11, "leg_r");
   complete.mBones[complete.mBones.size() - 1].length[0] *= 0.4f; // Min length
   complete.AddBone(12, 13, "leg_l");
   complete.mBones[complete.mBones.size() - 1].length[0] *= 0.4f; // Min length

   // Create random branch terrain
   srand(static_cast<unsigned>(glfwGetTime()));

   int num_segments = 80;
   float x = 0;
   float y = 0;
   for (int i = 0; i < num_segments + 1; ++i)
   {
      branches.AddPoint(glm::vec3(x, y, 0), "branch");
      x -= 2.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (6.0f - 2.0f)));
      //y += -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - -3.0f)));
      //y = glm::clamp(y, -2.5f, 2.5f); // Make sure we stay on screen
   }

   for (int i = 0; i < num_segments; ++i)
   {
      branches.AddBone(i, i + 1, "branch");
      mTerrainLines.emplace_back(branches.mPoints[i].bindPos, branches.mPoints[i + 1].bindPos, glm::vec3(0.0f, 1.0f, 0.0f));
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

   mLineShader->use(true);
   mLineShader->setUniformMat4("projectionView", perspectiveProjectionMatrix * viewMatrix);

   // Loop over the terrain lines and render each one
   for (unsigned int i = 0,
        size = static_cast<unsigned int>(mTerrainLines.size());
        i < size;
        ++i)
   {
      mTerrainLines[i].render(mLineShader);
   }

   // Loop over the debug lines and render each one
   for (unsigned int i = 0,
        size = static_cast<unsigned int>(mDebugLines.size());
        i < size;
        ++i)
   {
      mDebugLines[i].render(mLineShader);
   }

   mLineShader->use(false);
}

glm::vec3 AnimatedCharacter::getCOM() const
{
   float total_mass = 0.0f;
   glm::vec3 com(0.0f, 0.0f, 0.0f);
   const std::vector<VerletSystem::Point>& points = display.simple_rig.mPoints;
   for (int i = 0; i < points.size(); ++i)
   {
      com += points[i].currPos * points[i].mass;
      total_mass += points[i].mass;
   }
   com /= total_mass;
   return com;
}

glm::vec3 AnimatedCharacter::hexToColor(int hex)
{
   float r = static_cast<float>(((hex >> 16) & 0xff)) / 255.0f;
   float g = static_cast<float>(((hex >> 8) & 0xff)) / 255.0f;
   float b = static_cast<float>((hex & 0xff)) / 255.0f;

   return glm::vec3(r, g, b);
}

// Use law of cosines to find angles of triangle
float AnimatedCharacter::GetAngleGivenSides(float a, float b, float c)
{
   float top = (c * c - a * a - b * b);

   float divisor = (-2.0f * a * b);
   if (divisor == 0.0f)
   {
      return 0.0f;
   }

   return glm::acos(glm::clamp(top / divisor, -1.0f, 1.0f));
}

// Solve two bone IK problems
// Arguments when dealing with arms:
//    start_id = shoulder
//    end_id   = hand
//    forward actually points backwards
//    arm_ik   = original shoulder, elbow and grip positions
//    top      = visual bicep
//    bottom   = visual forearm
//    old_axis = elbow's bind space axis of rotation
//    axis     = elbow's world space axis of rotation
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
   const VerletSystem::Point& start = points[start_id]; // shoulder
   const VerletSystem::Point& end   = points[end_id];   // hand

   // Get sides of triangle formed by upper and lower limb
   float dist_a     = glm::distance(ik[0], ik[1]);               // Distance from bind shoulder to bind elbow
   float dist_b     = glm::distance(ik[1], ik[2]);               // Distance from bind elbow to bind grip
   float dist_c     = glm::distance(start.currPos, end.currPos); // Distance from complete.shoulder to complete.hand
   float old_dist_c = glm::distance(ik[0], ik[2]);               // Distance from bind shoulder to bind hand

   // Note that dist_a and dist_b are always the same because of the length constraint between shoulder-elbow and elbow-hand
   // The only distance that can change is the shoulder-hand distance
   // That's why the computations below work

   // Get angles of triangle
   /*
                    /* <- shoulder
                   /Y|
      dist_a ->   /  |
                 /   |
      elbow  -> *X   | <- dist_c
                 \   |
      dist_b ->   \  |
                   \ |
                    \* <- hand

      X = hinge angle
      Y = base angle
   */
   float old_hinge_angle = GetAngleGivenSides(dist_a, dist_b, old_dist_c); // Bind angle between bicep and forearm
   float hinge_angle     = GetAngleGivenSides(dist_a, dist_b, dist_c);     // Angle between bicep and forearm
   float old_base_angle  = GetAngleGivenSides(old_dist_c, dist_a, dist_b); // Bind angle between bicep and shoulder-hand line
   float base_angle      = GetAngleGivenSides(dist_c, dist_a, dist_b);     // Angle between bicep and shoulder-hand line

   // Apply rotation of entire arm (shoulder->hand)
   // With only this rotation the arms are stiff (i.e. they rotate around the shoulder, but they don't bend at the elbows)
   // (end.pos - start.pos) = (hand - shoulder) = points downwards
   // forward points backwards
   Q::quat base_rotation = Q::inverse(Q::lookRotation(end.bindPos - start.bindPos, glm::vec3(0.0f, 0.0f, 1.0f))) * Q::lookRotation(end.currPos - start.currPos, forward);
   // Apply additional rotation from IK
   base_rotation = Q::inverse(Q::angleAxis(old_base_angle, old_axis)) * base_rotation * Q::angleAxis(base_angle, axis);

   // Apply base and hinge rotations to actual display bones
   // bind_bicep + (shoulder - bind_shoulder) = move bicep into position below the shoulder
   top.transform.position = top.bind_pos + (start.currPos - start.bindPos);
   // orient the bicep
   top.transform.rotation = top.bind_rot * base_rotation;

   // Move forearm into position below the arm
   bottom.transform.position = top.transform.position + Q::inverse(top.bind_rot) * top.transform.rotation * (bottom.bind_pos - top.bind_pos);
   bottom.transform.rotation = bottom.bind_rot * Q::inverse(Q::angleAxis(old_hinge_angle, old_axis)) * base_rotation * Q::angleAxis(hinge_angle, axis);
}

// Calculate bone transform that matches orientation of top and bottom points, and looks in the character "forward" direction
// start = top point
// end   = bottom point
void AnimatedCharacter::ApplyBound(DisplayBone& part, const glm::vec3& forward, const glm::vec3& bind_forward, int start, int end)
{
   // Get mid point and "up" direction (from start to end point)
   // The "up" direction actually points down, since it goes from start (top) to end (bottom)
   // I think that's why the forward and bind_forward directions that this function receives are negated
   glm::vec3 up       = glm::normalize(complete.mPoints[end].currPos - complete.mPoints[start].currPos);
   glm::vec3 bind_up  = glm::normalize(complete.mPoints[end].bindPos - complete.mPoints[start].bindPos);
   glm::vec3 mid      = (complete.mPoints[end].currPos + complete.mPoints[start].currPos) / 2.0f;
   glm::vec3 bind_mid = (complete.mPoints[end].bindPos + complete.mPoints[start].bindPos) / 2.0f;

   // Apply rotations
   // This quaternion has the same purpose as body_rotation
   // This code is actually the same as the code in the beginning of the Update function
   Q::quat rotation = Q::inverse(Q::lookRotation(bind_up, bind_forward)) * Q::lookRotation(up, forward);
   part.transform.rotation = part.bind_rot * rotation;
   part.transform.position = mid + (rotation * (part.bind_pos - bind_mid));
}

// Get height of branch at given x coordinate
float AnimatedCharacter::BranchHeight(float x, int start_id, int end_id)
{
   const VerletSystem::Point& start = branches.mPoints[start_id];
   const VerletSystem::Point& end   = branches.mPoints[end_id];
   float branch_t = (x - start.bindPos[0]) / (end.bindPos[0] - start.bindPos[0]);
   return glm::mix(start.currPos[1], end.currPos[1], branch_t);
}

// Get height of entire branch terrain at given x coordinate
float AnimatedCharacter::BranchesHeight(float x)
{
   for (int i = 0; i < branches.mBones.size(); ++i)
   {
      const glm::ivec2& point_ids = branches.mBones[i].pointIndices;
      if (x <= branches.mPoints[point_ids[0]].currPos[0] && x > branches.mPoints[point_ids[1]].currPos[0])
      {
         return BranchHeight(x, point_ids[0], point_ids[1]);
      }
   }

   // If not on terrain, extend horizontally forever
   if (x > 0.0f)
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
   // At this point the step function has already been called, so the display rig has been updated
   // The point of the Update function is to update the complete rig and then the display_body

   // Here we update the complete rig using the simple rig
   { // Use "arms" rig to drive full body IK rig
      const std::vector<VerletSystem::Point>& points = display.simple_rig.mPoints;

      // Calculate midpoint and orientation of body triangle
      // Center of chest's triangle
      glm::vec3 bind_mid     = (points[0].bindPos + points[2].bindPos + points[4].bindPos) / 3.0f;
      glm::vec3 mid          = (points[0].currPos + points[2].currPos + points[4].currPos) / 3.0f;
      // Forward vector of chest's triangle
      glm::vec3 forward      = glm::normalize(glm::cross(points[4].currPos - points[0].currPos, points[2].currPos - points[0].currPos));
      glm::vec3 bind_forward = glm::normalize(glm::cross(points[4].bindPos - points[0].bindPos, points[2].bindPos - points[0].bindPos));
      // Up vector that points from body to mid point between the shoulders
      glm::vec3 up           = glm::normalize((points[0].currPos + points[2].currPos) / 2.0f - points[4].currPos);
      glm::vec3 bind_up      = glm::normalize((points[0].bindPos + points[2].bindPos) / 2.0f - points[4].bindPos);

      // Copy hand and shoulder positions from simple rig
      for (int i = 0; i < 4; ++i)
      {
         complete.mPoints[i].currPos = points[i].currPos;
         complete.mPoints[i].pinned = true;
      }

      // Starting with a simple rig (a triangle with two sticks for the arms),
      // we want to create a complete rig (a spine with two sticks for the arms and two sticks for the legs)
      // The strategy is the following: move the mid point of the chest's triangle along (bind_pos[i] - bind_mid) for every point in the complete rig
      // and rotate those vectors to match the orientation of the chest's triangle

      // body_rotation is the quaternion that rotates (bind_pos[i] - bind_mid) so that it has the orientation of the chest's triangle
      // I see it as transforming the vector to the inverse bind space, and then back to world space with the chest's orientation
      // lookRot(fwd, up) * lookRot(bindFwd, bindUp)^-1 * bindVec
      Q::quat body_rotation = Q::inverse(Q::lookRotation(bind_forward, bind_up)) * Q::lookRotation(forward, up);

      // Set up spine, head and leg positions based on body rotation
      // 5  = head
      // 6  = neck
      // 7  = stomach
      // 8  = pelvis (hip)
      // 9  = groin
      // 10 = hip right
      // 11 = foot right
      // 12 = hip left
      // 13 = foot left
      for (int i = 5; i < 14; ++i)
      {
         complete.mPoints[i].currPos = mid + (body_rotation * (complete.mPoints[i].bindPos - bind_mid));
         complete.mPoints[i].pinned = true;
      }

      // Apply body compression
      // 7 = stomach
      // 8 = pelvis (hip)
      // 9 = groin
      complete.mPoints[7].pinned = false;
      complete.mPoints[8].pinned = false;
      glm::vec3 old_hip = complete.mPoints[9].currPos; // This should be called old_groin
      for (int i = 7; i <= 9; ++i)
      {
         // Lerp between points 7, 8 and 9 and point 6, which is the neck
         // The higher the body compression, the closer points 7, 8 and 9 are to the neck
         complete.mPoints[i].currPos = glm::mix(complete.mPoints[i].currPos, complete.mPoints[6].currPos, body_compress_amount);
      }
      // Move the stomach and the pelvis (hip) back
      complete.mPoints[7].currPos -= forward * body_compress_amount * 0.2f;
      complete.mPoints[8].currPos -= forward * body_compress_amount * 0.2f;

      // Move the legs towards the new position of the groin
      for (int i = 10; i < 14; ++i)
      {
         complete.mPoints[i].currPos += complete.mPoints[9].currPos - old_hip;
      }

      // Move feet to foot targets
      for (int i = 0; i < 2; ++i)
      {
         complete.mPoints[11 + i * 2].currPos = display.limb_targets[2 + i];
      }

      // Enforce bone length constraints
      for (int i = 0; i < 2; ++i)
      {
         complete.EnforceDistanceConstraints();
      }
   }

   // Here we update the display_body using the complete rig
   { // Apply full body IK rig to visual deformation bones
      const std::vector<VerletSystem::Point>& points = complete.mPoints;

      // Get torso orientation and position
      // Note that here we are working with the complete rig, not the simple rig
      // 0 = right shoulder
      // 2 = left shoulder
      // 9 = groin
      // Center of torso
      glm::vec3 bind_mid     = (points[0].bindPos + points[2].bindPos + points[9].bindPos) / 3.0f;
      glm::vec3 mid          = (points[0].currPos + points[2].currPos + points[9].currPos) / 3.0f;
      // Forward vector of torso negated, so it points backwards
      glm::vec3 forward      = -glm::normalize(glm::cross(points[9].currPos - points[0].currPos, points[2].currPos - points[0].currPos));
      glm::vec3 bind_forward = -glm::normalize(glm::cross(points[9].bindPos - points[0].bindPos, points[2].bindPos - points[0].bindPos));
      // Up vector that points from groin to mid point between the shoulders
      glm::vec3 up           = glm::normalize((points[0].currPos + points[2].currPos) / 2.0f - points[9].currPos);
      glm::vec3 bind_up      = glm::normalize((points[0].bindPos + points[2].bindPos) / 2.0f - points[9].bindPos);

      // Apply core bones
      // 5 = head
      // 6 = neck
      ApplyBound(display_body.head, forward, bind_forward, 5, 6);
      // 6 = neck
      // 7 = stomach
      ApplyBound(display_body.chest, forward, bind_forward, 6, 7);
      // 7 = stomach
      // 8 = pelvis (hip)
      ApplyBound(display_body.belly, forward, bind_forward, 7, 8);
      // 8 = pelvis (hip)
      // 9 = groin
      ApplyBound(display_body.pelvis, forward, bind_forward, 8, 9);

      // Arm IK
      for (int i = 0; i < 2; ++i)
      {
         DisplayBone& top    = (i == 0) ? display_body.arm_top_r : display_body.arm_top_l;       // Bicep
         DisplayBone& bottom = (i == 0) ? display_body.arm_bottom_r : display_body.arm_bottom_l; // Forearm

         // 0 = right shoulder
         // 1 = right hand
         // 2 = left shoulder
         // 3 = left hand
         int start_id = i * 2;     // Shoulder
         int end_id   = i * 2 + 1; // Hand
         const VerletSystem::Point& start = points[start_id]; // Shoulder point
         const VerletSystem::Point& end   = points[end_id];   // Hand point

         // Adjust elbow target position
         float ik_driver = 1.0f;
         float ik_forward_amount = -ik_driver * 0.8f;
         float ik_up_amount = 0.1f + ik_driver * 0.5f;
         // Start at mid point between the shoulders, move up by ik_up_amount and move forward by ik_forward_amount
         glm::vec3 elbow_point      = ((points[2].currPos + points[0].currPos) * 0.5f + up * ik_up_amount + forward * ik_forward_amount);
         glm::vec3 bind_elbow_point = ((points[2].bindPos + points[0].bindPos) * 0.5f + bind_up * ik_up_amount + bind_forward * ik_forward_amount);

         // ((mid point between hand and shoulder) - elbow IK target) cross (shoulder - hand)
         // This looks to me like the axis of rotation of the elbow
         // It points outwards from the body (remember to use your left hand when computing cross products in a left-handed coordinate system like Unity's)
         // old_axis is bind space
         // axis is world space
         glm::vec3 old_axis = glm::normalize(glm::cross((end.bindPos + start.bindPos) * 0.5f - bind_elbow_point, start.bindPos - end.bindPos));
         glm::vec3 axis     = glm::normalize(glm::cross((end.currPos + start.currPos) * 0.5f - elbow_point, start.currPos - end.currPos));

         // start_id = shoulder
         // end_id   = hand
         // forward actually points backwards
         // arm_ik   = original shoulder, elbow and grip positions
         // top      = visual bicep
         // bottom   = visual forearm
         // old_axis = elbow's bind space axis of rotation
         // axis     = elbow's world space axis of rotation
         ApplyTwoBoneIK(start_id, end_id, forward, arm_ik, top, bottom, complete.mPoints, old_axis, axis);
      }

      // Leg IK
      for (int i = 0; i < 2; ++i)
      {
         DisplayBone& top    = (i == 0) ? display_body.leg_top_r : display_body.leg_top_l;       // Thigh
         DisplayBone& bottom = (i == 0) ? display_body.leg_bottom_r : display_body.leg_bottom_l; // Calf

         // 10 = right hip
         // 11 = right foot
         // 12 = left hip
         // 13 = left foot
         int start = i * 2 + 10; // Hip
         int end = i * 2 + 1 + 10; // Foot

         // Points downwards from hip to foot
         glm::vec3 leg_dir = points[end].currPos - points[start].currPos;

         // Get knee direction
         glm::vec2 leg_dir_flat = glm::normalize(glm::vec2(glm::dot(leg_dir, forward), glm::dot(leg_dir, up)));
         glm::vec3 leg_forward  = leg_dir_flat[0] * up + leg_dir_flat[1] * -forward;

         // Get base whole-leg rotation
         Q::quat bind_rotation = Q::lookRotation(points[end].bindPos - points[start].bindPos, glm::vec3(0.0f, 0.0f, 1.0f));
         Q::quat rotation      = bind_rotation * Q::lookRotation(leg_dir, leg_forward);

         // Get knee bend axis
         glm::vec3 old_axis = bind_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
         glm::vec3 axis     = rotation * glm::vec3(1.0f, 0.0f, 0.0f);

         ApplyTwoBoneIK(start, end, leg_forward, leg_ik, top, bottom, complete.mPoints, old_axis, axis);
      }

      // Head look
      // head_look_y: 50 = max look down, -70 = max look up
      // head_look_x: -90 to 90

      // Get head target in head transform space
      glm::vec3 target = glm::normalize(inverseTransformPoint(display_body.head.transform, look_target));
      // Using sin here is not correct (should be asin or something), but looks ok so keeping it for now
      float head_look_y = glm::sin(target.x);
      // Flatten look direction to solve other rotation axis
      glm::vec3 temp = target;
      temp.x = 0.0f;
      temp = glm::normalize(temp);
      float head_look_x = -glm::sin(temp.y);

      // Apply head transform
      display_body.head.transform.rotation = Q::angleAxis(head_look_y, glm::vec3(0.0f, 1.0f, 0.0f)) * Q::angleAxis(head_look_x, glm::vec3(1.0f, 0.0f, 0.0f)) * display_body.head.transform.rotation;
      if (glm::degrees(head_look_y) > 0.0f)
      {
         // It's important not to take the scale into account here, which is why we use TransformDirection instead of TransformVector
         display_body.head.transform.position = display_body.head.transform.position + ((transformDirection(display_body.head.transform, glm::vec3(1.0f, 0.0f, 0.0f))) * glm::degrees(head_look_y) * -0.001f);
      }
   }

   // Update the pose
   unsigned int jointIndex = 0;
   bool found = mBaseSkeleton.GetJointIndex("display_gibbon", jointIndex);
   Transform inverseBaseTransform;
   if (found) { inverseBaseTransform = inverse(mBaseSkeleton.GetBindPose().GetGlobalTransform(jointIndex)); }
   else { std::cout << "Couldn't find joint: " << "display_gibbon" << '\n'; }
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

float AnimatedCharacter::MoveTowardsF(float a, float b, float max_dist)
{
   float len = glm::distance(a, b);
   if (len < max_dist)
   {
      return b;
   }
   else
   {
      return a + ((b - a) / len) * max_dist;
   }
}

glm::vec3 AnimatedCharacter::MoveTowardsVec(const glm::vec3& a, const glm::vec3& b, float max_dist)
{
   float len = glm::distance(a, b);
   if (len < max_dist)
   {
      return b;
   }
   else
   {
      return a + ((b - a) / len) * max_dist;
   }
}

void AnimatedCharacter::PreventHandsFromCrossingBody(VerletSystem& rig)
{
   for (int i = 0; i < 2; ++i)
   {
      // For the right hand this vector points from the left shoulder to the right shoulder
      // For the left hand it points in the opposite direction
      glm::vec3 side_dir = glm::normalize(rig.mPoints[0].currPos - rig.mPoints[2].currPos) * (1.0f - i * 2.0f);
      float shoulder_d   = glm::dot(rig.mPoints[i * 2].currPos, side_dir);
      float hand_d       = glm::dot(rig.mPoints[i * 2 + 1].currPos, side_dir);
      float new_d        = glm::max(hand_d, shoulder_d);
      rig.mPoints[i * 2 + 1].currPos += (new_d - hand_d) * side_dir;
   }
}

// Apply actual controls and physics
void AnimatedCharacter::Step(float step, const std::shared_ptr<Window>& window)
{
   // Transform controls to axes
   float horz_input = 0.0f;
   if (window->keyIsPressed(GLFW_KEY_D))
   {
      horz_input = -1.0f; // Move to the right (along the -X direction)
   }
   if (window->keyIsPressed(GLFW_KEY_A))
   {
      horz_input = 1.0f; // Move to the left (along the +X direction)
   }

   // Max speed of 7 m/s while running
   float max_speed = 7.0f;

   // Player can influence horizontal velocity
   simple_vel[0] += horz_input * step * 5.0f;
   simple_vel[0] = glm::clamp(simple_vel[0], -max_speed, max_speed);

   // Don't allow speed < 1.0 m/s, don't need to worry about idle animations in an endless runner
   if (horz_input == 0.0f && glm::abs(simple_vel[0]) < 1.0f)
   {
      simple_vel[0] = MoveTowardsF(simple_vel[0], simple_vel[0] <= 0.0f ? -1.0f : 1.0f, step);
   }

   // Smooth out vertical position on branch by checking height forwards and back
   // Smoothing helps make the motion look good at the points where the slope of the terrain changes abruptly
   // It makes the height of the character not match the height of the branches at those points
   // In points like this one: /\ -> The height of the character is lower than the corner
   // In points like this one: \/ -> the height of the character is higher than the corner
   glm::vec3 future_pos   = simple_pos + simple_vel * 0.1f;
   future_pos[1]          = BranchesHeight(future_pos[0]);
   glm::vec3 past_pos     = simple_pos + simple_vel * -0.1f;
   past_pos[1]            = BranchesHeight(past_pos[0]);
   glm::vec3 smoothed_pos = (future_pos + past_pos + simple_pos) / 3.0f;

   // Get slope and use it to modify running speed
   glm::vec3 slope_vec    = normalizeWithZeroLengthCheck(future_pos - simple_pos);
   float slope_speed_mult = glm::abs(slope_vec[0]);

   // Apply modified running speed to position
   glm::vec3 effective_vel = simple_vel * slope_speed_mult;
   simple_pos += effective_vel * step;

   simple_pos[1] = BranchesHeight(simple_pos[0]);
   simple_vel[1] = 0.0f;

   // If on ground, look in the direction you are moving
   // Here we calculate the forward vector of the chest's triangle
   glm::vec3 forward = glm::normalize(glm::cross(display.simple_rig.mPoints[4].currPos - display.simple_rig.mPoints[0].currPos, display.simple_rig.mPoints[2].currPos - display.simple_rig.mPoints[0].currPos));
   look_target = display_body.head.transform.position + forward * 0.1f;
   look_target += future_pos - past_pos;

   { // Run animation
      // TODO: Compare Unity's Time.time with glfwGetTime()
      float time = static_cast<float>(glfwGetTime());

      // Vary between different gaits based on speed and time
      // y\ =\sin(x\cdot2.3)+\sin(x\cdot1.7)\ \left\{0<y\ <\ 1\right\}\ \left\{x\ >0\right\}
      quad_amount = glm::clamp((glm::sin(time * 2.3f) + glm::sin(time * 1.7f)), 0.0f, 1.0f);

      // Determine how far to lean forwards
      // y\ =\ \sin(x)*0.2+0.3\ \left\{x\ >0\right\}
      float walk_lean = glm::sin(time) * 0.2f + 0.3f;
      float lean = walk_lean;

      // Adjust stride frequency based on speed
      // y\ =\ 8\ /\ (\pi\ *\ 2)\ *\ (\operatorname{abs}(x)+1.0)^{0.4}\ \left\{1\ <\ x\ <\ 7\right\}
      float speed_mult = 8.0f / (glm::pi<float>() * 2.0f) * glm::pow((glm::abs(effective_vel[0]) + 1.0f), 0.4f);
      // When the character moves faster, we increase the walk_time faster
      walk_time += step * speed_mult;

      // Compress body during quadruped gallop
      walk.body_compress_amount = 0.0f;

      // Adjust COM height based on gait
      glm::vec3 target_com = simple_pos;
      target_com[1] = smoothed_pos[1];
      // walk_height is what raises the COM from the floor. Notice how it draws a sine wave
      float walk_height = base_walk_height + glm::sin((walk_time + 0.25f) * glm::pi<float>() * 4.0f) * glm::abs(effective_vel[0]) * 0.015f / speed_mult + glm::abs(effective_vel[0]) * 0.01f;
      target_com[1] += walk_height;
      // The target COM moves up and down. The lower it is (i.e. the closer it is to simple_pos), the more the character leans forward
      target_com[1] = glm::mix(target_com[1], simple_pos[1], glm::abs(lean) * 0.15f);

      // Get ground slope again for use later
      glm::vec3 left = simple_pos + glm::vec3(0.1f, 0.0f, 0.0f);
      glm::vec3 right = simple_pos - glm::vec3(0.1f, 0.0f, 0.0f);
      left[1] = BranchesHeight(left[0]);
      right[1] = BranchesHeight(right[0]);
      // move_dir always points to the left (i.e. down the +X axis)
      glm::vec3 move_dir = glm::normalize(left - right);

      { // Simulate the walk simple rig
         VerletSystem& rig = walk.simple_rig;

         // Apply gravity and velocity to all the points in the walk simple rig
         rig.StartSim(step);

         for (int j = 0; j < 4; ++j)
         {
            // Calculate the COM
            float total_mass = 0.0f;
            glm::vec3 com = glm::vec3(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < rig.mPoints.size(); ++i)
            {
               // 1 is the right hand and 3 is the left hand
               // We don't want to consider those points when calculating the COM
               // We only consider the points that make up the chest's triangle (0 - right should, 2 - left shoulder and 4 - body)
               if (i != 1 && i != 3)
               {
                  com += rig.mPoints[i].currPos * rig.mPoints[i].mass;
                  total_mass += rig.mPoints[i].mass;
               }
            }

            // Adjust all free points to match target COM
            com /= total_mass;
            glm::vec3 offset = target_com - com;
            for (int i = 0; i < rig.mPoints.size(); ++i)
            {
               if (i != 1 && i != 3)
               {
                  rig.mPoints[i].currPos += offset * 0.2f;
               }
            }

            // Apply torque to keep torso upright and forward-facing
            float step_sqrd = step * step;
            float force = 20.0f;
            // Calculate the forward vector of the chest's triangle
            glm::vec3 forward2 = glm::normalize(glm::cross(rig.mPoints[4].currPos - rig.mPoints[0].currPos, rig.mPoints[2].currPos - rig.mPoints[0].currPos));
            glm::vec3 flat_forward = glm::normalize(glm::vec3(forward2[0], 0.0f, forward2[2]));
            // flat_foward is simply the forward vector of the chest's triangle with no Y component, so it's always perfectly aligned with the +X and -X axes
            // Multiplying flat_forward by lean causes the length of the vector to shrink when the character is upright, and to grow when the character is leaning
            // Adding (0, 1, 0) just makes the vector point upwards diagonally 
            glm::vec3 top_force = (lean * flat_forward + glm::vec3(0.0f, 1.0f, 0.0f)) * force;
            // These three lines are what keep the chest upright
            // The torque is carefully balanced here
            rig.mPoints[4].currPos += step_sqrd * -top_force;       // Body
            rig.mPoints[0].currPos += step_sqrd * top_force * 0.5f; // Right shoulder
            rig.mPoints[2].currPos += step_sqrd * top_force * 0.5f; // Left shoulder
            // These two lines are what make the character look forward when it moves. Without them, the character walks sideways
            rig.mPoints[0].currPos[2] += step_sqrd * effective_vel[0] * 2.0f; // Right shoulder
            rig.mPoints[2].currPos[2] -= step_sqrd * effective_vel[0] * 2.0f; // Left shoulder

            // Add rotational force to body if needed
            // This is what makes the chest sway from side to side (i.e. rotate around the Z axis)
            for (int i = 0; i < 2; ++i)
            {
               float walk_rotate = (glm::cos((walk_time + tilt_offset) * glm::pi<float>() * 2.0f + glm::pi<float>() * i)) * 0.2f;
               float rotate = walk_rotate;
               // Points 0 and 2 are modified here (right shoulder and left shoulder, respectively)
               rig.mPoints[i * 2].currPos[0] += step_sqrd * -3.0f * rotate * effective_vel[0] / speed_mult;
            }

            // Move arms out to sides
            float speed = glm::abs(effective_vel[0]) / max_speed;
            for (int i = 0; i < 2; ++i)
            {
               // This is what causes the arms to flail
               float arms_up = glm::abs(speed * (glm::sin(time * ((i == 1) ? 2.5f : 2.3f)) * 0.3f + 0.7f));
               // Points 1 and 3 are modified here (right hand and left hand, respectively)
               // They are moved along the vector that connects the shoulders (points 0 and 2)
               rig.mPoints[1 + i * 2].currPos += step_sqrd * (rig.mPoints[0].currPos - rig.mPoints[2].currPos) * (1.5f + speed * 2.0f + arms_up * 2.0f) * (1.0f - i * 2.0f) * 2.0f;
               rig.mPoints[1 + i * 2].currPos[1] += step_sqrd * 10.0f * arms_up * arms_up;
               // Bone 0 is the right arm, while bone 1 is the left arm
               // Here we update the maximum length
               rig.mBones[i].length[1] = rig.mBones[0].length[0] / 0.4f * (glm::mix(0.95f, 0.8f, glm::min(speed * 0.25f, 1.0f) + glm::sin(arms_up * glm::pi<float>()) * 0.1f));
            }

            // This prevents the arms from colliding with the body and penetrating it when the character turns
            // TODO: I still don't understand the math behind this function
            PreventHandsFromCrossingBody(rig);

            // Make sure hands don't go through floor
            for (int i = 0; i < 2; ++i)
            {
               // Points 1 and 3 are modified here (right hand and left hand, respectively)
               rig.mPoints[i * 2 + 1].currPos[1] = glm::max(rig.mPoints[i * 2 + 1].currPos[1], BranchesHeight(rig.mPoints[i * 2 + 1].currPos[0]));
            }

            for (int i = 0; i < 2; ++i)
            {
               rig.EnforceDistanceConstraints();
            }
         }

         rig.EndSim();

         // Calculate leg targets
         // 2 is right foot
         // 3 is left foot
         for (int i = 0; i < 2; ++i)
         {
            float offset = glm::mix(gallop_offset, quad_gallop_offset, quad_amount);
            float time_val = walk_time * glm::pi<float>() * 2.0f + glm::pi<float>() * i * offset;
            walk.limb_targets[2 + i] = simple_pos;
            // This vector points in the movement direction. Note how it's length depends on the effective velocity. That causes the strides to shorten or lengthen depending on the velocity of the character
            walk.limb_targets[2 + i] += (move_dir * (glm::cos(walk_time * glm::pi<float>() * 2.0f + glm::pi<float>() * i)) * 0.2f - 0.03f) * effective_vel[0] / speed_mult;
            // This vector points from the left shoulder to the right shoulder for the right foot, and in the opposite direction for the left foot
            walk.limb_targets[2 + i] += (rig.mPoints[0].currPos - rig.mPoints[2].currPos) * (1.0f - 2.0f * i) * (0.3f);
            // The height of the target matches the height of the branch
            // It float aboves the ground in front and behind the character, and it sinks in the middle
            walk.limb_targets[2 + i][1] = BranchesHeight(walk.limb_targets[2 + i][0]);
            walk.limb_targets[2 + i][1] += (-glm::sin(walk_time * glm::pi<float>() * 2.0f + glm::pi<float>() * i) + 1.0f) * 0.2f * (glm::pow(glm::abs(effective_vel[0]) + 1.0f, 0.3f) - 1.0f) * (1.0f);
         }
      }
   }

   { // Combine source rigs into display rig
      // Interpolate between source rigs
      // Since we only support running right now, there is no interpolation
      // The walk right is simply copied into the display rig
      for (int i = 0; i < display.simple_rig.mPoints.size(); ++i)
      {
         display.simple_rig.mPoints[i].oldPos = display.simple_rig.mPoints[i].currPos;
         display.simple_rig.mPoints[i].currPos = walk.simple_rig.mPoints[i].currPos;
      }
      for (int i = 0; i < 2; ++i)
      {
         display.simple_rig.EnforceDistanceConstraints();
      }
      for (int i = 0; i < 4; ++i)
      {
         display.limb_targets[i] = walk.limb_targets[i];
      }
      body_compress_amount = walk.body_compress_amount;
   }
}
