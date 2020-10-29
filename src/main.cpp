#include "utils/shader.h"
#include "utils/model.h"

#include "elements/torus.h"
#include "elements/car.h"
#include "elements/controller.h"
#include "elements/window.h"
#include "utils/opengl.h"


int main() {
    // Windows
    OpenGl open_gl = OpenGl("Task-3-3D-scene");

    // Shaders
    Shader torus_shader = Shader("assets/torus/torus.vs", "assets/torus/torus.fs");
    Shader skybox_shader = Shader("assets/skybox/skybox.vs", "assets/skybox/skybox.fs");
    Shader car_shader = Shader("assets/car/car.vs", "assets/car/car.fs");

    // Elements
    Skybox skybox = Skybox(skybox_shader);
    Torus torus = Torus(torus_shader);
    Car car = Car(car_shader);

    Controller controller = Controller(torus);

    // Render
    Window window = Window(skybox, torus, car, controller, open_gl.window());
    window.render();

    return 0;
}
