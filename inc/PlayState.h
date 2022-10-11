#ifndef MODEL_VIEWER_STATE_H
#define MODEL_VIEWER_STATE_H

#include "state.h"
#include "finite_state_machine.h"
#include "window.h"
#include "Camera3.h"
#include "texture.h"
#include "AnimatedMesh.h"
#include "AnimatedCharacter.h"
#include "DecalRenderer.h"

class PlayState : public State
{
public:

   PlayState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
             const std::shared_ptr<Window>&             window);
   ~PlayState() = default;

   PlayState(const PlayState&) = delete;
   PlayState& operator=(const PlayState&) = delete;

   PlayState(PlayState&&) = delete;
   PlayState& operator=(PlayState&&) = delete;

   void initializeState();

   void enter() override;
   void processInput() override;
   void fixedUpdate() override;
   void update(float deltaTime) override;
   void render() override;
   void exit() override;

private:

   void loadGround();

   void configureLights(const std::shared_ptr<Shader>& shader);

   void userInterface();

   void resetScene();

   void resetCamera();

   void loadModels();

   void renderWorld();
   void renderNormalsAndDepth();

   std::shared_ptr<FiniteStateMachine> mFSM;

   std::shared_ptr<Window>             mWindow;

   Camera3                             mCamera3;

   std::shared_ptr<Shader>             mStaticMeshShader;
   std::shared_ptr<Shader>             mStaticMeshWithoutUVsShader;

   std::vector<AnimatedMesh>           mGroundMeshes;
   std::shared_ptr<Texture>            mGroundTexture;

   AnimatedCharacter                   mAnimatedCharacter;

   bool                                mDisplayGround;

#ifndef __EMSCRIPTEN__
   bool                                mPause = false;
#endif

   std::shared_ptr<Shader>             mNormalAndDepthShader;
   std::vector<AnimatedMesh>           mPlaneMeshes;
   std::shared_ptr<Texture>            mPlaneTexture;
   std::vector<AnimatedMesh>           mNormalPlaneMeshes;
   Transform                           mPlaneModelTransform;
   std::shared_ptr<DecalRenderer>      mDecalRenderer;

   int                                 mMaxNumDecals = 100;
   float                               mCurrentDecalScale = 1.0f;
   float                               mSelectedDecalScale = 0.1f;
   float                               mDecalNormalThreshold = 89.0f;
   float                               mCurrentDelayBetweenCircles = 0.1f;
   float                               mSelectedDelayBetweenCircles = 0.1f;
   float                               mCurrentDecalBounce = 4.5f;
   float                               mSelectedDecalBounce = 4.5f;
   bool                                mDisplayDecalOBBs = false;
   bool                                mDisplayDiscardedDecalParts = false;
};

#endif
