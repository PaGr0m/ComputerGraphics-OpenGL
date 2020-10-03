#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


class Model {
private:
    std::vector<Texture> textures_loaded_;
    std::vector<Mesh> meshes_;
    std::string directory_;
    bool gamma_correction_;

public:
    explicit Model(const std::string &path, bool gamma = false) : gamma_correction_(gamma) {
        load_model(path);
    }

    void draw(Shader &shader) {
        for (auto &mesh : meshes_) mesh.draw(shader);
    }

private:
    void load_model(std::string const &path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate |
                                                       aiProcess_GenSmoothNormals |
                                                       aiProcess_FlipUVs |
                                                       aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        directory_ = path.substr(0, path.find_last_of('/'));

        process_node(scene->mRootNode, scene);
    }

    void process_node(aiNode *node, const aiScene *scene) {
        for (GLuint i = 0; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes_.push_back(process_mesh(mesh, scene));
        }

        for (GLuint i = 0; i < node->mNumChildren; i++) {
            process_node(node->mChildren[i], scene);
        }
    }

    Mesh process_mesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex{};
            glm::vec3 vector = glm::vec3(
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
            );

            // Positions
            vertex.position = vector;

            // Normals
            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }

            // Texture coordinates
            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texture_coords = vec;

                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.bitangent = vector;
            } else {
                vertex.texture_coords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (GLuint j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. Diffuse maps
        std::vector<Texture> diffuse_maps = load_material_textures(material,
                                                                   aiTextureType_DIFFUSE,
                                                                   "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        // 2. Specular maps
        std::vector<Texture> specular_maps = load_material_textures(material,
                                                                    aiTextureType_SPECULAR,
                                                                    "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

        // 3. Normal maps
        std::vector<Texture> normal_maps = load_material_textures(material,
                                                                  aiTextureType_HEIGHT,
                                                                  "texture_normal");
        textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

        // 4. Height maps
        std::vector<Texture> height_maps = load_material_textures(material,
                                                                  aiTextureType_AMBIENT,
                                                                  "texture_height");
        textures.insert(textures.end(), height_maps.begin(), height_maps.end());

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture>
    load_material_textures(aiMaterial *material, aiTextureType texture_type, const std::string &type_name) {
        std::vector<Texture> textures;
        for (GLuint i = 0; i < material->GetTextureCount(texture_type); ++i) {
            aiString path;
            material->GetTexture(texture_type, i, &path);

            bool is_loaded = false;
            for (auto &texture_path : textures_loaded_) {
                if (std::strcmp(texture_path.path.data(), path.C_Str()) == 0) {
                    textures.push_back(texture_path);
                    is_loaded = true;
                    break;
                }
            }

            if (!is_loaded) {
                Texture texture;
                texture.id = load_texture_from_file(path.C_Str(), directory_, gamma_correction_);
                texture.type = type_name;
                texture.path = path.C_Str();
                textures.push_back(texture);
                textures_loaded_.push_back(texture);
            }
        }

        return textures;
    }

    static GLuint load_texture_from_file(const char *path, const std::string &dir, bool gamma) {
        std::string filename = std::string(path);
        filename = dir + '/' + filename;

        GLuint textureID;
        glGenTextures(1, &textureID);

        GLint width, height, nrComponents;
        GLubyte *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1) format = GL_RED;
            else if (nrComponents == 3) format = GL_RGB;
            else if (nrComponents == 4) format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};
