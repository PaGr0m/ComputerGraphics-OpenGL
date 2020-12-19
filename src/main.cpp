#include "utils/shader.h"
#include "utils/settings.h"
#include "elements/metaballs.h"
#include "elements/cubemap.h"
#include "elements/window.h"
#include "elements/opengl.h"

int main() {
    // OpenGl
    auto open_gl = OpenGl(Settings::WINDOW_NAME);

    // Shader
    auto cubemap_shader = Shader(Settings::CUBEMAP_SHADER_VERTEX, Settings::CUBEMAP_SHADER_FRAGMENT);
    auto metaballs_shader = Shader(Settings::METABALLS_SHADER_VERTEX,
                                   Settings::METABALLS_SHADER_GEOMETRY,
                                   Settings::METABALLS_SHADER_FRAGMENT);

    // Elements
    auto cubemap = Cubemap(cubemap_shader);
    auto metaballs = Metaballs(metaballs_shader);

    // Render window
    auto window = Window(cubemap, metaballs, open_gl.window());
    window.render();

    return 0;
}
