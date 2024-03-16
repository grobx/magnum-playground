#include "playground.h"

using WindowFlag = Platform::Sdl2Application::Configuration::WindowFlag;

/**
 * TODO (milestone #1):
 * - [x] create a simple Sdl2Application
 * - [x] add ImGuiIntegration
 * - [x] use the Triangle example
 * - [ ] use the Texture example (optional)
 * - [ ] use the Primitive example
 * - [ ] use the Model Viewer example
 * - [x] add EigenIntegration
 * - [ ] create the first Scene
 * - [ ] put the table on the scene (optional)
 * - [-] add obj importer
 * - [ ] look for some obj file
 * - [ ] put some obj on the scene
 * - [ ] start manipulating the object using eigen
 */
Playground::Playground(const Arguments& arguments) :
    Platform::Application{arguments,
        Configuration{}
            .setTitle("Playground")
            .addWindowFlags(WindowFlag::FullscreenDesktop)} {
    initTriangle();
    initCube();
    initScene();
    initGUI();
}

// From https://doc.magnum.graphics/magnum/examples-triangle.html
void Playground::initTriangle() {
    using namespace Math::Literals;

    struct TriangleVertex {
        Vector2 position;
        Color3  color;
    };

    const TriangleVertex vertices[]{
        {{-0.5f, -0.5f}, 0xff0000_rgbf}, /* Left vertex, red color */
        {{0.5f, -0.5f}, 0x00ff00_rgbf},  /* Right vertex, green color */
        {{0.0f, 0.5f}, 0x0000ff_rgbf}    /* Top vertex, blue color */
    };

    _triangle_mesh.setCount(Containers::arraySize(vertices))
        .addVertexBuffer(GL::Buffer{vertices}, 0,
            Shaders::VertexColorGL2D::Position{},
            Shaders::VertexColorGL2D::Color3{});
}

void Playground::drawTriangle() {
    _vcolgl2d_shader.draw(_triangle_mesh);
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
}

// From https://doc.magnum.graphics/magnum/examples-imgui.html
void Playground::initGUI() {
    _imgui = ImGuiIntegration::Context(Vector2{windowSize()} / dpiScaling(),
        windowSize(), framebufferSize());

    // auto font = "/usr/share/fonts/truetype/hack/Hack-Regular.ttf";
    auto font = "/usr/share/fonts/TTF/Hack-Regular.ttf";
    ImGui::GetIO().Fonts->Clear();
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font, 32.0f);

    _imgui.relayout(windowSize());

    /* Set up proper blending to be used by ImGui. There's a great chance
     you'll need this exact behavior for the rest of your scene. If not, set
     this only for the drawFrame() call. */
    GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
        GL::Renderer::BlendEquation::Add);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
        GL::Renderer::BlendFunction::OneMinusSourceAlpha);

#if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
    /* Have some sane speed, please */
    setMinimalLoopPeriod(16);
#endif
}

void Playground::drawGUI() {
    _imgui.newFrame();

    /* Enable text input, if needed */
    if (ImGui::GetIO().WantTextInput && !isTextInputActive())
        startTextInput();
    else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
        stopTextInput();

    /* 1. Show a simple window. */
    {
        ImGui::Begin("Main Window");
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("Float", &_floatValue, 0.0f, 1.0f);
        if (ImGui::ColorEdit3("Clear Color", _clearColor.data()))
            GL::Renderer::setClearColor(_clearColor);
        if (ImGui::Button("Test Window"))
            _showDemoWindow ^= true;
        if (ImGui::Button("Another Window"))
            _showAnotherWindow ^= true;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0 / Double(ImGui::GetIO().Framerate),
            Double(ImGui::GetIO().Framerate));
        ImGui::End();
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
    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

    _imgui.drawFrame();
}

// Draw Algorithm
void Playground::resetFeatures() {
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::Blending);
}

void Playground::startDrawing() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
}

void Playground::drawEvent() {
    startDrawing();

    drawTriangle();
    resetFeatures();

    drawCube();
    resetFeatures();

    drawScene();
    resetFeatures();

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
    GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

    _imgui.relayout(Vector2{event.windowSize()} / event.dpiScaling(),
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
