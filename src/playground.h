#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Context.hpp>
#include <Magnum/ImGuiIntegration/Widgets.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/VertexColorGL.h>

using namespace Magnum;
using namespace Math::Literals;

/*
 * Also see:
 * - https://pthom.github.io/imgui_bundle/manual.html
 * -
 * https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/dpi_aware.h
 */
class Playground : public Platform::Application {
public:
  explicit Playground(const Arguments &arguments);

  void viewportEvent(ViewportEvent &event) override;

  void keyPressEvent(KeyEvent &event) override;
  void keyReleaseEvent(KeyEvent &event) override;

  void mousePressEvent(MouseEvent &event) override;
  void mouseReleaseEvent(MouseEvent &event) override;
  void mouseMoveEvent(MouseMoveEvent &event) override;
  void mouseScrollEvent(MouseScrollEvent &event) override;
  void textInputEvent(TextInputEvent &event) override;

private:
  void drawEvent() override;

  void initTriangle();
  void initCube();
  void initScene();
  void initGUI();

  void startDrawing();
  void endDrawing();
  void drawTriangle();
  void drawCube();
  void drawScene();
  void drawGUI();
  void resetFeatures();

  GL::Mesh _triangle_mesh;
  Shaders::VertexColorGL2D _vcolgl2d_shader;

  ImGuiIntegration::Context _imgui{NoCreate};
  bool _showDemoWindow = true;
  bool _showAnotherWindow = false;
  Color4 _clearColor = 0x72909aff_rgbaf;
  Float _floatValue = 0.0f;
};
