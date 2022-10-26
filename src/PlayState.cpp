#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "resource_manager.h"
#include "shader_loader.h"
#include "texture_loader.h"
#include "GLTFLoader.h"
#include "RearrangeBones.h"
#include "PlayState.h"

PlayState::PlayState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
                     const std::shared_ptr<Window>&             window)
   : mFSM(finiteStateMachine)
   , mWindow(window)
   , mCamera3(4.0f, 0.0f, glm::vec3(0.0f), Q::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, 0.0f, 90.0f, 45.0f, 1280.0f / 720.0f, 0.1f, 130.0f, 0.25f)
{
   mStaticMeshShader = ResourceManager<Shader>().loadUnmanagedResource<ShaderLoader>("resources/shaders/static_mesh.vert",
                                                                                     "resources/shaders/ambient_diffuse_illumination.frag");
   configureLights(mStaticMeshShader);

   mStaticMeshWithoutUVsShader = ResourceManager<Shader>().loadUnmanagedResource<ShaderLoader>("resources/shaders/static_mesh_without_uvs.vert",
                                                                                               "resources/shaders/ambient_diffuse_illumination_without_uvs.frag");
   configureLights(mStaticMeshWithoutUVsShader);

   mAnimatedCharacter.initialize(mStaticMeshWithoutUVsShader);

   loadGround();
}

void PlayState::initializeState()
{
   mDisplayGround = true;
}

void PlayState::enter()
{
   initializeState();
   resetCamera();
   resetScene();
}

void PlayState::processInput()
{
   // Close the game
   if (mWindow->keyIsPressed(GLFW_KEY_ESCAPE)) { mWindow->setShouldClose(true); }

#ifndef __EMSCRIPTEN__
   // Make the game full screen or windowed
   if (mWindow->keyIsPressed(GLFW_KEY_F) && !mWindow->keyHasBeenProcessed(GLFW_KEY_F))
   {
      mWindow->setKeyAsProcessed(GLFW_KEY_F);
      mWindow->setFullScreen(!mWindow->isFullScreen());
   }

   // Change the number of samples used for anti aliasing
   if (mWindow->keyIsPressed(GLFW_KEY_1) && !mWindow->keyHasBeenProcessed(GLFW_KEY_1))
   {
      mWindow->setKeyAsProcessed(GLFW_KEY_1);
      mWindow->setNumberOfSamples(1);
   }
   else if (mWindow->keyIsPressed(GLFW_KEY_2) && !mWindow->keyHasBeenProcessed(GLFW_KEY_2))
   {
      mWindow->setKeyAsProcessed(GLFW_KEY_2);
      mWindow->setNumberOfSamples(2);
   }
   else if (mWindow->keyIsPressed(GLFW_KEY_4) && !mWindow->keyHasBeenProcessed(GLFW_KEY_4))
   {
      mWindow->setKeyAsProcessed(GLFW_KEY_4);
      mWindow->setNumberOfSamples(4);
   }
   else if (mWindow->keyIsPressed(GLFW_KEY_8) && !mWindow->keyHasBeenProcessed(GLFW_KEY_8))
   {
      mWindow->setKeyAsProcessed(GLFW_KEY_8);
      mWindow->setNumberOfSamples(8);
   }

   // Reset the camera
   if (mWindow->keyIsPressed(GLFW_KEY_R)) { resetCamera(); }
#endif

   // Orient the camera
   if (mWindow->mouseMoved() && mWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
   {
      mCamera3.processMouseMovement(mWindow->getCursorXOffset(), mWindow->getCursorYOffset());
      mWindow->resetMouseMoved();
   }

   // Adjust the distance between the player and the camera
   if (mWindow->scrollWheelMoved())
   {
      mCamera3.processScrollWheelMovement(mWindow->getScrollYOffset());
      mWindow->resetScrollWheelMoved();
   }

   mAnimatedCharacter.clearDebugLines();
}

void PlayState::update(float deltaTime)
{
   mAnimatedCharacter.Update();

   glm::vec3 playerCOM = mAnimatedCharacter.getCOM();
   glm::vec3 adjustedPlayerPos(playerCOM[0] + mAnimatedCharacter.getVelocity()[0] * 0.1f, playerCOM[1], 0.0f);
   mCamera3.processPlayerMovement(adjustedPlayerPos, Q::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

   mStaticMeshWithoutUVsShader->use(true);
   mStaticMeshWithoutUVsShader->setUniformVec3("pointLights[0].worldPos", glm::vec3(mAnimatedCharacter.getPosition().x, 2.0f, 10.0f));
   mStaticMeshWithoutUVsShader->setUniformVec3("pointLights[1].worldPos", glm::vec3(mAnimatedCharacter.getPosition().x, 2.0f, -10.0f));
   mStaticMeshWithoutUVsShader->use(false);
}

void PlayState::fixedUpdate()
{
   mAnimatedCharacter.Step(1.0f / 50.0f, mWindow);
}

void PlayState::render()
{
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();

   userInterface();

#ifndef __EMSCRIPTEN__
   mWindow->bindMultisampleFramebuffer();
#endif

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   /*
   if (mDisplayGround)
   {
      mStaticMeshShader->use(true);

      glm::mat4 modelMatrix(1.0f);
      modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
      mStaticMeshShader->setUniformMat4("model",      modelMatrix);
      mStaticMeshShader->setUniformMat4("view",       mCamera3.getViewMatrix());
      mStaticMeshShader->setUniformMat4("projection", mCamera3.getPerspectiveProjectionMatrix());
      mGroundTexture->bind(0, mStaticMeshShader->getUniformLocation("diffuseTex"));

      // Loop over the ground meshes and render each one
      for (unsigned int i = 0,
         size = static_cast<unsigned int>(mGroundMeshes.size());
         i < size;
         ++i)
      {
         mGroundMeshes[i].Render();
      }

      mGroundTexture->unbind(0);
      mStaticMeshShader->use(false);
   }
   */

   mAnimatedCharacter.render(mStaticMeshWithoutUVsShader, mCamera3.getViewMatrix(), mCamera3.getPerspectiveProjectionMatrix());

   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifndef __EMSCRIPTEN__
   mWindow->generateAntiAliasedImage();
#endif

   mWindow->swapBuffers();
   mWindow->pollEvents();
}

void PlayState::exit()
{

}

void PlayState::loadGround()
{
   // Load the texture of the ground
   mGroundTexture = ResourceManager<Texture>().loadUnmanagedResource<TextureLoader>("resources/models/table/wooden_floor.jpg");

   // Load the ground
   cgltf_data* data = LoadGLTFFile("resources/models/table/wooden_floor.glb");
   mGroundMeshes = LoadStaticMeshes(data);
   FreeGLTFFile(data);

   int positionsAttribLocOfStaticShader = mStaticMeshShader->getAttributeLocation("position");
   int normalsAttribLocOfStaticShader   = mStaticMeshShader->getAttributeLocation("normal");
   int texCoordsAttribLocOfStaticShader = mStaticMeshShader->getAttributeLocation("texCoord");
   for (unsigned int i = 0,
        size = static_cast<unsigned int>(mGroundMeshes.size());
        i < size;
        ++i)
   {
      mGroundMeshes[i].ConfigureVAO(positionsAttribLocOfStaticShader,
                                    normalsAttribLocOfStaticShader,
                                    texCoordsAttribLocOfStaticShader,
                                    -1,
                                    -1);
   }
}

void PlayState::configureLights(const std::shared_ptr<Shader>& shader)
{
   shader->use(true);
   shader->setUniformVec3("pointLights[0].worldPos", glm::vec3(0.0f, 2.0f, 10.0f));
   shader->setUniformVec3("pointLights[0].color", glm::vec3(1.0f, 0.95f, 0.9f));
   shader->setUniformFloat("pointLights[0].constantAtt", 1.0f);
   shader->setUniformFloat("pointLights[0].linearAtt", 0.01f);
   shader->setUniformFloat("pointLights[0].quadraticAtt", 0.0f);
   shader->setUniformVec3("pointLights[1].worldPos", glm::vec3(0.0f, 2.0f, -10.0f));
   shader->setUniformVec3("pointLights[1].color", glm::vec3(1.0f, 0.95f, 0.9f));
   shader->setUniformFloat("pointLights[1].constantAtt", 1.0f);
   shader->setUniformFloat("pointLights[1].linearAtt", 0.01f);
   shader->setUniformFloat("pointLights[1].quadraticAtt", 0.0f);
   shader->setUniformInt("numPointLightsInScene", 2);
   shader->use(false);
}

#ifdef __EMSCRIPTEN__
EM_JS(void, openReadme, (), {
   window.open("https://github.com/diegomacario/Procedural-Playground/blob/main/README.md");
});
#endif

void PlayState::userInterface()
{
   ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Appearing);

   char title[64];
   snprintf(title, 64, "Procedural Playground (%.1f FPS)###ProceduralPlayground", ImGui::GetIO().Framerate);
   ImGui::Begin(title, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

//#ifdef __EMSCRIPTEN__
//   ImGui::Text("Click the button below to learn more about this\n"
//               "project:");
//
//   if (ImGui::Button("Open README"))
//   {
//      openReadme();
//   }
//#endif

   if (ImGui::CollapsingHeader("Controls", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
   {
      ImGui::BulletText("Hold the left mouse button and move the mouse\n"
                        "to rotate the camera around the character.");
      ImGui::BulletText("Use the scroll wheel to zoom in and out.");
#ifndef __EMSCRIPTEN__
      ImGui::BulletText("Press the R key to reset the camera.");
#endif
   }

   //if (ImGui::CollapsingHeader("Settings", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
   //{
   //   ImGui::Checkbox("Display Ground", &mDisplayGround);
   //}

   ImGui::End();
}

void PlayState::resetScene()
{

}

void PlayState::resetCamera()
{
   mCamera3.reposition(4.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), Q::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, 0.0f, 90.0f);
   mCamera3.processMouseMovement(180.0f / 0.25f, 0.0f);
}
