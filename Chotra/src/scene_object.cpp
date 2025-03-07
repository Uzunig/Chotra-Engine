#include "scene_object.h"

#include "obj_model.h"
#include "material.h"
#include "environment.h"
#include "shader.h"
#include "material_texture.h"
#include "resource_manager.h"

namespace Chotra {

    SceneObject::SceneObject(unsigned int geometryIndex, unsigned int materialIndex, std::string name, glm::vec3 position, glm::vec3 angle,
        glm::vec3 scale, glm::vec3 velocity, glm::vec3 rVelocity, bool visible, float brightness)
        : geometryIndex(geometryIndex), materialIndex(materialIndex), name(name), position(position), angle(angle),
        scale(scale), velocity(velocity), rVelocity(rVelocity), visible(visible), brightness(brightness), parentMatrix(glm::mat4(1.0)) {

        UpdateModelMatrix();
    }

    void SceneObject::ChangeGeometryIndex(unsigned int i) {
        geometryIndex = i;
    }

    void SceneObject::ChangeMaterialIndex(unsigned int i) {
       materialIndex = i;
    }

    void SceneObject::Draw(Shader& shader) {

        shader.Use();
        shader.SetMat4("model", modelMatrix);
        shader.SetFloat("brightness", brightness);
        
        unsigned int j = 0;
        for (std::map<std::string, unsigned int>::iterator it = ResourceManager::GetMaterial(materialIndex)->components.begin(); it != ResourceManager::GetMaterial(materialIndex)->components.end(); ++it) {
            glActiveTexture(GL_TEXTURE0 + j); // ����� ����������� ���������� ������ ���������� ����

            // ������ ������������� ������� �� ������ ���������� ����
            shader.Use();
            glUniform1i(glGetUniformLocation(shader.ID, (it->first).c_str()), j);
            // � ��������� ��������
            glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTextureId(it->second));
            ++j;
        }


        // ������������ ���
        glBindVertexArray(ResourceManager::GetGeometryVAO(geometryIndex));
        glDrawArrays(GL_TRIANGLES, 0, ResourceManager::GetGeometryVerticesCount(geometryIndex));
        //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
        glBindVertexArray(0);

        // ��������� ������� ��������� ���������� �������� ���������� � �� �������������� ���������
        glActiveTexture(GL_TEXTURE0);
    }

    void SceneObject::UpdateModelMatrix() {
        modelMatrix = glm::translate(parentMatrix, position);

        modelMatrix = modelMatrix * (glm::rotate(glm::mat4(1.0f), glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(angle.z), glm::vec3(0.0f, .0f, 1.0f)));

        modelMatrix = glm::scale(modelMatrix, scale);

    }

} // namspace Chotra
