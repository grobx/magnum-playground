#include <ImGuiFileDialog.h>
#include <Magnum/EigenIntegration/DynamicMatrixIntegration.h>
#include <Magnum/EigenIntegration/Integration.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Context.hpp>
#include <Magnum/ImGuiIntegration/Widgets.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/Trade/MeshData.h>
#include <MagnumPlugins/ObjImporter/ObjImporter.h>
#include <pwd.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

namespace C = Corrade;
namespace M = Magnum;
using namespace M::Math::Literals;

/*
 * Also see:
 * - https://pthom.github.io/imgui_bundle/manual.html
 * - https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/dpi_aware.h
 */
class Playground : public M::Platform::Application {
public:
    explicit Playground(const Arguments& arguments);

    void viewportEvent(ViewportEvent& event) override;

    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;

    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void mouseScrollEvent(MouseScrollEvent& event) override;
    void textInputEvent(TextInputEvent& event) override;

private:
    void drawEvent() override;

    void initTriangle();
    void initCube();
    void initScene();
    void initGUI();

    std::string homeDir();
    void        openFile(std::string fileName);

    void startDrawing();
    void endDrawing();
    void drawTriangle();
    void drawCube();
    void drawScene();
    void resetFeatures();

    void drawGUI();
    void BeginMainPanel();
    void EndMainPanel();

    M::GL::Mesh                 _triangleMesh;
    M::Shaders::VertexColorGL2D _vertexColor2Shader;
    M::Shaders::PhongGL         _phongShader;

    M::ImGuiIntegration::Context _imgui{M::NoCreate};
    bool                         _showDemoWindow    = true;
    bool                         _showAnotherWindow = false;
    M::Color4                    _clearColor        = 0x72909aff_rgbaf;
    M::Float                     _floatValue        = 0.0f;

    std::string _assetsDir = "Projects/Plan99/magnum-playground/assets";
    std::optional<std::string>                                 _fileOpened;
    std::vector<Magnum::GL::Mesh>                              _meshes;
    C::PluginManager::Manager<Magnum::Trade::AbstractImporter> _manager;
};
