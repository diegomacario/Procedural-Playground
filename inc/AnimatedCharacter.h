#ifndef ANIMATED_CHARACTER_H
#define ANIMATED_CHARACTER_H

#include <memory>

#include "Shader.h"
#include "AnimatedMesh.h"

class AnimatedCharacter
{
public:

   AnimatedCharacter();
   ~AnimatedCharacter();

   void      load(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader);

   void      render(const std::shared_ptr<Shader>& staticMeshWithoutUVsShader, const glm::mat4& viewMatrix, const glm::mat4& perspectiveProjectionMatrix);

private:

   glm::vec3 hexToColor(int hex);

   Skeleton                  mBaseSkeleton;
   std::vector<AnimatedMesh> mMeshes;
   std::vector<glm::vec3>    mBaseColors;
};

#endif
