#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include "../utils/shader.h"
#include "../utils/model.h"

class Object {
private:
    Shader shader_;
    Model model_;

public:
    explicit Object(const Shader &shader, const std::string &path_to_model)
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
