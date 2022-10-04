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
      mLines.emplace_back(branches.mPoints[i].bindPos, branches.mPoints[i + 1].bindPos, glm::vec3(0.0f, 1.0f, 0.0f));
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

   // Loop over the lines and render each one
   for (unsigned int i = 0,
        size = static_cast<unsigned int>(mLines.size());
        i < size;
        ++i)
   {
      mLines[i].render(mLineShader);
   }

   mLineShader->use(false);
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
   return 0.0f;
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

}

// Calculate bone transform that matches orientation of top and bottom points, and looks in the character "forward" direction
void AnimatedCharacter::ApplyBound(DisplayBone& part, const glm::vec3& forward, const glm::vec3& bind_forward, int start, int end)
{

}

// Get height of branch at given x coordinate
float AnimatedCharacter::BranchHeight(float x, int start_id, int end_id)
{
   return 0.0f;
}

// Get height of entire branch terrain at given x coordinate
float AnimatedCharacter::BranchesHeight(float x)
{
   return 0.0f;
}

// Prepare to draw next frame
void AnimatedCharacter::Update()
{
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
   return 0.0f;
}

glm::vec3 AnimatedCharacter::MoveTowards(const glm::vec3& a, const glm::vec3& b, float max_dist)
{
   return glm::vec3(0.0f);
}

void AnimatedCharacter::PreventHandsFromCrossingBody(VerletSystem& rig)
{

}

// Apply actual controls and physics
void AnimatedCharacter::Step(float step, const std::shared_ptr<Window>& window)
{

}
