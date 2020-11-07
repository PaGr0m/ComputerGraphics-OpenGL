#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include "../utils/shader.h"
#include "../settings.h"

class Light {
private:
    Shader shader_;

    GLuint depth_map_FBO{};
    GLuint depth_map{};

    glm::mat4 light_projection{};
    glm::mat4 light_view{};

public:
    explicit Light(const Shader &shader) : shader_(shader) {
        create_depth_texture();
    }

    void render(glm::mat4 &model) {
        light_projection = glm::ortho(
                -6.0f, 6.0f,
                -6.0f, 6.0f,
                Settings::LIGHT_Z_NEAR, Settings::LIGHT_Z_FAR
        );

        light_view = glm::lookAt(
                Settings::LIGHT_POSITION,
                glm::vec3(0.0f),
                glm::vec3(0.0, 1.0, 0.0)
        );

        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
        glViewport(0, 0, Settings::SHADOW_WIDTH, Settings::SHADOW_HEIGHT);
        glClear(GLuint(GL_DEPTH_BUFFER_BIT));

        shader_.use();
        shader_.set_uniform("u_view", glm::value_ptr(light_view));
        shader_.set_uniform("u_projection", glm::value_ptr(light_projection));
    }

    void activate() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(Settings::GL_TEXTURE_DEPTH_MAP);
        glBindTexture(GL_TEXTURE_2D, depth_map);
    }

    [[nodiscard]]
    glm::mat4 projection() const {
        return light_projection;
    }

    [[nodiscard]]
    glm::mat4 view() const {
        return light_view;
    }

private:
    void create_depth_texture() {
        glGenFramebuffers(1, &depth_map_FBO);

        glGenTextures(1, &depth_map);
        glBindTexture(GL_TEXTURE_2D, depth_map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     Settings::SHADOW_WIDTH, Settings::SHADOW_HEIGHT,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};
