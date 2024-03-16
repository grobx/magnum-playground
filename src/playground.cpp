#include "playground.h"

using WindowFlag = M::Platform::Sdl2Application::Configuration::WindowFlag;

/**
 * TODO (milestone #1):
 * - [x] create a simple Sdl2Application
 * - [x] add ImGuiIntegration
 * - [x] use the Triangle example
 * - [ ] use the Texture example (optional)
 * - [ ] use the Primitive example
 * - [ ] use the Model Viewer example
 * - [x] add EigenIntegration
 * - [x] create the first Scene
 * - [ ] put the table on the scene (optional)
 * - [x] add obj importer
 * - [x] look for some obj file
 * - [x] put some obj on the scene
 * - [ ] add Scene Graph
 * - [ ] start manipulating the object using eigen
 */
Playground::Playground(const Arguments& arguments) :
    M::Platform::Application{arguments,
        Configuration{}
            .setTitle("Playground")
            .addWindowFlags(WindowFlag::FullscreenDesktop)} {
    initTriangle();
    initCube();
    initScene();
    initGUI();
}

void Playground::openFile(std::string fileName) {
    auto importer = _manager.loadAndInstantiate("ObjImporter");
    if (!importer->openFile(fileName))
        return;

    _fileOpened = fileName;
    for (int i = 0; i < importer->meshCount(); ++i) {
        auto                       meshData = *importer->mesh(i);
        M::MeshTools::CompileFlags flags;
        if (meshData.hasAttribute(M::Trade::MeshAttribute::Normal))
            flags |= M::MeshTools::CompileFlag::GenerateFlatNormals;
        _meshes.emplace_back(M::MeshTools::compile(meshData, flags));
    }
}

// From https://doc.magnum.graphics/magnum/examples-triangle.html
void Playground::initTriangle() {
    using namespace M::Math::Literals;

    struct TriangleVertex {
        M::Vector2 position;
        M::Color3  color;
    };

    const TriangleVertex vertices[]{
        {{-0.5f, -0.5f}, 0xff0000_rgbf}, /* Left vertex, red color */
        {{0.5f, -0.5f}, 0x00ff00_rgbf},  /* Right vertex, green color */
        {{0.0f, 0.5f}, 0x0000ff_rgbf}    /* Top vertex, blue color */
    };

    _triangleMesh.setCount(C::Containers::arraySize(vertices))
        .addVertexBuffer(M::GL::Buffer{vertices}, 0,
            M::Shaders::VertexColorGL2D::Position{},
            M::Shaders::VertexColorGL2D::Color3{});
}

void Playground::drawTriangle() {
    _vertexColor2Shader.draw(_triangleMesh);
}

// From https://doc.magnum.graphics/magnum/examples-primitives.html
void Playground::initCube() {
}

void Playground::drawCube() {
}

// From https://doc.magnum.graphics/magnum/examples-viewer.html
void Playground::initScene() {
}

void Playground::drawScene() {
    auto _transformation =
        M::Matrix4::rotationX(30.0_degf) * M::Matrix4::rotationY(40.0_degf);

    auto _projection =
        M::Matrix4::perspectiveProjection(35.0_degf,
            M::Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f) *
        M::Matrix4::translation(M::Vector3::zAxis(-10.0f));

    auto _color = M::Color3::fromHsv({35.0_degf, 1.0f, 1.0f});

    for (auto& m : _meshes) {
        _phongShader.setDiffuseColor(_color)
            .setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
            .setTransformationMatrix(_transformation)
            .setNormalMatrix(_transformation.normalMatrix())
            .setProjectionMatrix(_projection)
            .draw(m);
    }
}

// From https://doc.magnum.graphics/magnum/examples-imgui.html
void Playground::initGUI() {
    _imgui =
        M::ImGuiIntegration::Context(M::Vector2{windowSize()} / dpiScaling(),
            windowSize(), framebufferSize());

    // auto font = "/usr/share/fonts/truetype/hack/Hack-Regular.ttf";
    auto font = "/usr/share/fonts/TTF/Hack-Regular.ttf";
    ImGui::GetIO().Fonts->Clear();
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font, 32.0f);

    _imgui.relayout(windowSize());

    /* Set up proper blending to be used by ImGui. There's a great chance
     you'll need this exact behavior for the rest of your scene. If not, set
     this only for the drawFrame() call. */
    M::GL::Renderer::setBlendEquation(M::GL::Renderer::BlendEquation::Add,
        M::GL::Renderer::BlendEquation::Add);
    M::GL::Renderer::setBlendFunction(
        M::GL::Renderer::BlendFunction::SourceAlpha,
        M::GL::Renderer::BlendFunction::OneMinusSourceAlpha);

#if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
    /* Have some sane speed, please */
    setMinimalLoopPeriod(16);
#endif
}

void Playground::BeginMainPanel() {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNav;
    //window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    window_flags |= ImGuiWindowFlags_UnsavedDocument;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, .8f);
    {
        ImGui::SetNextWindowPos({0, 0});
        auto size =
            ImVec2({(float)windowSize()[0] / 4, (float)windowSize()[1]});
        ImGui::SetNextWindowSizeConstraints(size, size);
        ImGui::Begin("Main Window", nullptr, window_flags);
        {
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("Float", &_floatValue, 0.0f, 1.0f);
            if (ImGui::ColorEdit3("Clear Color", _clearColor.data()))
                M::GL::Renderer::setClearColor(_clearColor);
            if (ImGui::Button("Test Window"))
                _showDemoWindow ^= true;
            if (ImGui::Button("Another Window"))
                _showAnotherWindow ^= true;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / M::Double(ImGui::GetIO().Framerate),
                M::Double(ImGui::GetIO().Framerate));
            // file dialog button
            if (ImGui::Button("Open File Dialog")) {
                IGFD::FileDialogConfig config;
                config.path = homeDir().append("/").append(_assetsDir);
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey",
                    "Choose File", ".obj", config);
            }
            // display file dialog
            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                    std::string filePathName =
                        ImGuiFileDialog::Instance()->GetFilePathName();
                    openFile(filePathName);
                }

                // close
                ImGuiFileDialog::Instance()->Close();
            }
            // display message when file opened succesfully
            if (_fileOpened) {
                ImGui::TextWrapped("Loaded %zu meshes from %s!", _meshes.size(),
                    _fileOpened->c_str());
            }
            if (ImGui::Button("Quit Application!")) {
                exit();
            }
        }
    }
}

void Playground::EndMainPanel() {
    ImGui::End();
    ImGui::PopStyleVar(4);
}

void Playground::drawGUI() {
    _imgui.newFrame();

    /* Enable text input, if needed */
    if (ImGui::GetIO().WantTextInput && !isTextInputActive())
        startTextInput();
    else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
        stopTextInput();

    /* 1. Show main panel window. */
    {
        BeginMainPanel();
        EndMainPanel();
    }

    /* 2. Show another simple window, now using an explicit Begin/End pair */
    if (_showAnotherWindow) {
        ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
        ImGui::Begin("Another Window", &_showAnotherWindow);
        ImGui::Text("Hello");
        ImGui::End();
    }

    /* 3. Show the ImGui demo window. Most of the sample code is in
     ImGui::ShowDemoWindow() */
    if (_showDemoWindow) {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow();
    }

    /* Update application cursor */
    _imgui.updateApplicationCursor(*this);

    /* Set appropriate states. If you only draw ImGui, it is sufficient to
     just enable blending and scissor test in the constructor. */
    M::GL::Renderer::enable(M::GL::Renderer::Feature::Blending);
    M::GL::Renderer::enable(M::GL::Renderer::Feature::ScissorTest);
    M::GL::Renderer::disable(M::GL::Renderer::Feature::FaceCulling);
    M::GL::Renderer::disable(M::GL::Renderer::Feature::DepthTest);

    _imgui.drawFrame();
}

std::string Playground::homeDir() {
    struct passwd* pw = getpwuid(getuid());
    return pw->pw_dir;
}

// Draw Algorithm
void Playground::resetFeatures() {
    M::GL::Renderer::disable(M::GL::Renderer::Feature::DepthTest);
    M::GL::Renderer::disable(M::GL::Renderer::Feature::FaceCulling);
    M::GL::Renderer::disable(M::GL::Renderer::Feature::ScissorTest);
    M::GL::Renderer::disable(M::GL::Renderer::Feature::Blending);
}

void Playground::startDrawing() {
    M::GL::defaultFramebuffer.clear(
        M::GL::FramebufferClear::Color | M::GL::FramebufferClear::Depth);
}

void Playground::drawEvent() {
    startDrawing();

    M::GL::Renderer::enable(M::GL::Renderer::Feature::FaceCulling);
    M::GL::Renderer::enable(M::GL::Renderer::Feature::DepthTest);

    drawTriangle();
    drawCube();
    drawScene();

    drawGUI();
    resetFeatures();

    endDrawing();
}

void Playground::endDrawing() {
    swapBuffers();
    redraw();
}

// Input Handling
void Playground::viewportEvent(ViewportEvent& event) {
    M::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

    _imgui.relayout(M::Vector2{event.windowSize()} / event.dpiScaling(),
        event.windowSize(), event.framebufferSize());
}

void Playground::keyPressEvent(KeyEvent& event) {
    if (_imgui.handleKeyPressEvent(event))
        return;
}

void Playground::keyReleaseEvent(KeyEvent& event) {
    if (_imgui.handleKeyReleaseEvent(event))
        return;
}

void Playground::mousePressEvent(MouseEvent& event) {
    if (_imgui.handleMousePressEvent(event))
        return;
}

void Playground::mouseReleaseEvent(MouseEvent& event) {
    if (_imgui.handleMouseReleaseEvent(event))
        return;
}

void Playground::mouseMoveEvent(MouseMoveEvent& event) {
    if (_imgui.handleMouseMoveEvent(event))
        return;
}

void Playground::mouseScrollEvent(MouseScrollEvent& event) {
    if (_imgui.handleMouseScrollEvent(event)) {
        /* Prevent scrolling the page */
        event.setAccepted();
        return;
    }
}

void Playground::textInputEvent(TextInputEvent& event) {
    if (_imgui.handleTextInputEvent(event))
        return;
}
