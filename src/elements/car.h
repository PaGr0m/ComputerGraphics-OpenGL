#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include "../utils/shader.h"
#include "../utils/model.h"

class Car {
private:
    const std::string path_to_model = "assets/aircraft/piper_pa18.obj";
//    const std::string path_to_model = "assets/test/volley_ball_OBJ/volley_ball_OBJ.obj";

private:
    Shader shader_;
    Model model_;

public:
    explicit Car(const Shader &shader)
            : shader_(shader),
              model_(Model(path_to_model)) {}

public:
    void render(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
        shader_.use();

        shader_.set_uniform("u_model", glm::value_ptr(model));
        shader_.set_uniform("u_view", glm::value_ptr(view));
        shader_.set_uniform("u_projection", glm::value_ptr(projection));

        model_.draw(shader_);
    }
};
