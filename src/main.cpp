#include "utils/shader.h"
#include "utils/model.h"

#include "elements/torus.h"
#include "elements/object.h"
#include "elements/controller.h"
#include "elements/window.h"
#include "utils/opengl.h"


int main() {
    // Window
    auto open_gl = OpenGl(Settings::WINDOW_NAME);

    // Shaders
    auto torus_shader = Shader(Settings::TORUS_SHADER_VERTEX, Settings::TORUS_SHADER_FRAGMENT);
    auto cubemap_shader = Shader(Settings::CUBEMAP_SHADER_VERTEX, Settings::CUBEMAP_SHADER_FRAGMENT);
    auto object_shader = Shader(Settings::OBJECT_SHADER_VERTEX, Settings::OBJECT_SHADER_FRAGMENT);
    auto light_shader = Shader(Settings::LIGHT_SHADER_VERTEX, Settings::LIGHT_SHADER_FRAGMENT);

    // Elements
    auto cubemap = Cubemap(cubemap_shader, Settings::CUBEMAP_TEXTURES);
    auto object = Object(object_shader, Settings::PATH_TO_OBJECT);
    auto torus = Torus(torus_shader);
    auto light = Light(light_shader);
    auto controller = Controller(torus);

    // Render
    auto window = Window(cubemap, torus, object, controller, light, open_gl.window());
    window.render();

    return 0;
}
