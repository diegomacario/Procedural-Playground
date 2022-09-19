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

void AnimatedCharacter::load(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader)
{
   // Load the animated character
   cgltf_data* data = LoadGLTFFile("resources/models/gibbon/gibbon.glb");
   mBaseSkeleton    = LoadSkeleton(data);
   mMeshes          = LoadStaticMeshes(data);
   FreeGLTFFile(data);
   
   // Rearrange the skeleton
   JointMap jointMap = RearrangeSkeleton(mBaseSkeleton);
   
   // Configure the VAOs of the animated meshes
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
      Transform globalTransform = mBaseSkeleton.GetBindPose().GetGlobalTransform(nodeIndex);
      staticMeshWithoutUVsShader->setUniformMat4("model", transformToMat4(globalTransform));
      staticMeshWithoutUVsShader->setUniformVec3("baseColor", mBaseColors[i]);

      mMeshes[i].Render();
   }

   // Loop over the point meshes and render each one
   for (unsigned int i = 0,
        size = static_cast<unsigned int>(mPointsMeshes.size());
        i < size;
        ++i)
   {
      unsigned int nodeIndex = mPointsMeshes[i].GetNodeIndex();
      Transform offset;
      offset.position = glm::vec3(0.0f, 0.0f, -0.2f);
      Transform globalTransform = mPointsBaseSkeleton.GetBindPose().GetGlobalTransform(nodeIndex);
      Transform combined = combine(offset, globalTransform);
      staticMeshWithoutUVsShader->setUniformMat4("model", transformToMat4(combined));
      staticMeshWithoutUVsShader->setUniformVec3("baseColor", mPointsBaseColors[i]);

      mPointsMeshes[i].Render();
   }

   staticMeshWithoutUVsShader->use(false);
}

glm::vec3 AnimatedCharacter::hexToColor(int hex)
{
   float r = static_cast<float>(((hex >> 16) & 0xff)) / 255.0f;
   float g = static_cast<float>(((hex >> 8) & 0xff)) / 255.0f;
   float b = static_cast<float>((hex & 0xff)) / 255.0f;

   return glm::vec3(r, g, b);
}
