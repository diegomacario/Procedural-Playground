#include "GLTFLoader.h"
#include "RearrangeBones.h"
#include "AnimatedCharacter.h"

AnimatedCharacter::AnimatedCharacter()
{

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
   RearrangeSkeleton(mBaseSkeleton);
   
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
   }
}

void AnimatedCharacter::render(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader, const glm::mat4& viewMatrix, const glm::mat4& perspectiveProjectionMatrix)
{
   staticMeshWithoutUVsShader->use(true);

   glm::mat4 modelMatrix(1.0f);
   staticMeshWithoutUVsShader->setUniformMat4("model", modelMatrix);
   staticMeshWithoutUVsShader->setUniformMat4("view", viewMatrix);
   staticMeshWithoutUVsShader->setUniformMat4("projection", perspectiveProjectionMatrix);

   // Loop over the ground meshes and render each one
   for (unsigned int i = 0,
      size = static_cast<unsigned int>(mMeshes.size());
      i < size;
      ++i)
   {
      mMeshes[i].Render();
   }

   staticMeshWithoutUVsShader->use(false);
}
