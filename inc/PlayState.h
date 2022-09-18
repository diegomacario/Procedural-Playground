#ifndef MODEL_VIEWER_STATE_H
#define MODEL_VIEWER_STATE_H

#include "state.h"
#include "finite_state_machine.h"
#include "window.h"
#include "Camera3.h"
#include "texture.h"
#include "AnimatedMesh.h"
#include "SkeletonViewer.h"
#include "Clip.h"
#include "TrackVisualizer.h"

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
   void update(float deltaTime) override;
   void render() override;
   void exit() override;

private:

   void loadCharacter();
   void loadGround();

   void configureLights(const std::shared_ptr<Shader>& shader);

   void userInterface();

   void resetScene();

   void resetCamera();

   std::shared_ptr<FiniteStateMachine> mFSM;

   std::shared_ptr<Window>             mWindow;

   Camera3                             mCamera3;

   std::vector<AnimatedMesh>           mGroundMeshes;
   std::shared_ptr<Texture>            mGroundTexture;
   std::shared_ptr<Shader>             mGroundShader;

   std::shared_ptr<Shader>             mAnimatedMeshShader;
   std::shared_ptr<Texture>            mCharacterTexture;
   Skeleton                            mCharacterBaseSkeleton;
   Skeleton                            mCharacterSkeleton;
   std::vector<AnimatedMesh>           mCharacterMeshes;
   std::vector<FastClip>               mCharacterClips;
   std::string                         mCharacterClipNames;

   unsigned int                        mCurrentClipIndex;
   float                               mPlaybackTime;
   Pose                                mPose;
   std::vector<glm::mat4>              mPosePalette;
   std::vector<glm::mat4>              mSkinMatrices;
   Transform                           mModelTransform;
   float                               mJointScaleFactor;

   int                                 mSelectedClip;
   float                               mSelectedPlaybackSpeed;
   bool                                mDisplayGround;
   bool                                mDisplayGraphs;
   bool                                mDisplayMesh;
   bool                                mDisplayBones;
   bool                                mDisplayJoints;
#ifndef __EMSCRIPTEN__
   bool                                mWireframeModeForCharacter;
   bool                                mWireframeModeForJoints;
   bool                                mPerformDepthTesting;
#endif
   bool                                mFillEmptyTilesWithRepeatedGraphs;

#ifndef __EMSCRIPTEN__
   bool                                mPause = false;
#endif

   SkeletonViewer                      mSkeletonViewer;
   TrackVisualizer                     mTrackVisualizer;
};

#endif
