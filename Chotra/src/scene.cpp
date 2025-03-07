
#include "scene.h"

#include "scene_object.h"
#include "scene_light.h"
#include "environment.h"
#include "shader.h"
#include "resource_manager.h"
#include "scene_collection.h"

namespace Chotra {

    Scene::Scene(std::string path) {

        LoadSceneFromFile(path);
    }

    Scene::~Scene() {

    }

    void Scene::AddEnvironment(std::string path) {

        environment = std::make_shared<Environment>(path, sceneSuns[0]);
    }
    /*
        void Scene::AddSceneObject(unsigned int geometryIndex, unsigned int materialIndex, std::string name, glm::vec3 position, glm::vec3 angle,
            glm::vec3 scale, glm::vec3 velocity, glm::vec3 rVelocity, bool visible, float brightness) {

            sceneObjects.push_back(std::make_shared<SceneObject>(geometryIndex, materialIndex, name, position, angle, // TO DO: materials
                scale, velocity, rVelocity, visible, brightness));
        }
        */
    void Scene::AddSceneLight(unsigned int geometryIndex, unsigned int materialIndex, std::string name, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, glm::vec3 velocity, glm::vec3 rVelocity,
        bool visible, float brightness, glm::vec3 color, float intensity) {

        sceneLights.push_back(std::make_shared<SceneLight>(geometryIndex, materialIndex, name, position, angle,
            scale, velocity, rVelocity, visible, brightness, color, intensity));
    }

    void Scene::AddSceneSun(unsigned int geometryIndex, unsigned int materialIndex, std::string name, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, glm::vec3 velocity, glm::vec3 rVelocity,
        int visible, float brightness, glm::vec3 color, float intensity) {

        sceneSuns.push_back(std::make_shared<SceneLight>(geometryIndex, materialIndex, name, position, angle,
            scale, velocity, rVelocity, visible, brightness, color, intensity));
    }

    void Scene::UpdateCollection(float deltaTime, std::shared_ptr<SceneCollection> currentCollection) {

        currentCollection->UpdateCollection(deltaTime);

        for (unsigned int i = 0; i < currentCollection->sceneCollections.size(); ++i) {
            UpdateCollection(deltaTime, currentCollection->sceneCollections[i]);
        }
    }

    void Scene::Update(float deltaTime) {
        float dt = deltaTime * 50.0f;

        UpdateCollection(dt, rootCollection);

        for (unsigned int i = 0; i < sceneLights.size(); ++i) {
            if (sceneLights[i]->visible) {
                sceneLights[i]->position += sceneLights[i]->velocity * dt;
                sceneLights[i]->angle += sceneLights[i]->rVelocity * dt;
                sceneLights[i]->UpdateModelMatrix();
            }
        }

    }

    void Scene::DrawSceneCollection(Shader& shader, std::shared_ptr<SceneCollection> currentCollection) {

        glCullFace(GL_BACK);
        for (unsigned int i = 0; i < currentCollection->sceneObjects.size(); ++i) {
            if (currentCollection->sceneObjects[i]->visible) {
                currentCollection->sceneObjects[i]->Draw(shader);
            }
        }
        for (unsigned int i = 0; i < currentCollection->sceneCollections.size(); ++i) {
            DrawSceneCollection(shader, currentCollection->sceneCollections[i]);
        }

    }

    void Scene::DrawSceneLights(Shader& shader) {

        for (unsigned int i = 0; i < sceneLights.size(); ++i) {
            if (sceneLights[i]->visible) {
                shader.Use();
                shader.SetVec3("lightsColor", sceneLights[i]->color * sceneLights[i]->intensity);
                sceneLights[i]->Draw(shader);
            }
        }

    }

    void Scene::LoadSceneFromFile(std::string const& path) {

        std::ifstream level_file(path);
        if (!level_file) {
            std::cout << "The level file could not open for writing!" << std::endl;
        }
        else {
            rootCollection = std::make_shared<SceneCollection>("root");
            std::vector<std::shared_ptr<SceneCollection>> collectionStack;
            collectionStack.push_back(rootCollection);

            while (level_file) {

                std::string s;
                level_file >> s;

                if (s == "ObjModel") {
                    std::string model_path;
                    level_file >> model_path;
                    ResourceManager::AddGeometry(model_path);

                }
                else if (s == "Material") {
                    std::string material_path;
                    level_file >> material_path;
                    ResourceManager::AddMaterial(material_path);

                }
                else if (s == "BeginCollection") {

                    std::string name;
                    level_file >> name;

                    glm::vec3 position;
                    level_file >> position.x >> position.y >> position.z;

                    glm::vec3 angle;
                    level_file >> angle.x >> angle.y >> angle.z;

                    glm::vec3 scale;
                    level_file >> scale.x >> scale.y >> scale.z;

                    glm::vec3 velocity;
                    level_file >> velocity.x >> velocity.y >> velocity.z;

                    glm::vec3 rVelocity;
                    level_file >> rVelocity.x >> rVelocity.y >> rVelocity.z;

                    int visible;
                    level_file >> visible;

                    float brightness;
                    level_file >> brightness;

                    collectionStack.push_back(collectionStack[collectionStack.size() - 1]->AddSceneCollection(name, position, angle, // TO DO: materials 
                        scale, velocity, rVelocity, visible, brightness));

                }
                else if (s == "EndCollection") {

                    collectionStack.pop_back();

                }
                else if (s == "SceneObject") {
                    std::string name;
                    level_file >> name;

                    std::string meshType;
                    level_file >> meshType;

                    unsigned int geometryIndex;
                    level_file >> geometryIndex;

                    unsigned int materialIndex;
                    level_file >> materialIndex;

                    glm::vec3 position;
                    level_file >> position.x >> position.y >> position.z;

                    glm::vec3 angle;
                    level_file >> angle.x >> angle.y >> angle.z;

                    glm::vec3 scale;
                    level_file >> scale.x >> scale.y >> scale.z;

                    glm::vec3 velocity;
                    level_file >> velocity.x >> velocity.y >> velocity.z;

                    glm::vec3 rVelocity;
                    level_file >> rVelocity.x >> rVelocity.y >> rVelocity.z;

                    int visible;
                    level_file >> visible;

                    float brightness;
                    level_file >> brightness;

                    if (meshType == "OBJModel") {
                        collectionStack[collectionStack.size() - 1]->AddSceneObject(geometryIndex, materialIndex, name, position, angle, // TO DO: materials 
                            scale, velocity, rVelocity, visible, brightness);
                    }

                }
                else if (s == "SceneLight") {
                    std::string name;
                    level_file >> name;

                    std::string meshType;
                    level_file >> meshType;

                    unsigned int geometryIndex;
                    level_file >> geometryIndex;

                    unsigned int materialIndex;
                    level_file >> materialIndex;

                    glm::vec3 position;
                    level_file >> position.x >> position.y >> position.z;

                    glm::vec3 angle;
                    level_file >> angle.x >> angle.y >> angle.z;

                    glm::vec3 scale;
                    level_file >> scale.x >> scale.y >> scale.z;

                    glm::vec3 velocity;
                    level_file >> velocity.x >> velocity.y >> velocity.z;

                    glm::vec3 rVelocity;
                    level_file >> rVelocity.x >> rVelocity.y >> rVelocity.z;

                    int visible;
                    level_file >> visible;

                    float brightness;
                    level_file >> brightness;

                    glm::vec3 color;
                    level_file >> color.x >> color.y >> color.z;

                    float intensity;
                    level_file >> intensity;

                    if (meshType == "OBJModel") {
                        AddSceneLight(geometryIndex, materialIndex, name, position, angle,
                            scale, velocity, rVelocity, visible, brightness, color, intensity);
                    }

                }
                else if (s == "SceneSun") {
                    std::string name;
                    level_file >> name;

                    std::string meshType;
                    level_file >> meshType;

                    unsigned int geometryIndex;
                    level_file >> geometryIndex;

                    unsigned int materialIndex;
                    level_file >> materialIndex;

                    glm::vec3 position;
                    level_file >> position.x >> position.y >> position.z;

                    glm::vec3 angle;
                    level_file >> angle.x >> angle.y >> angle.z;

                    glm::vec3 scale;
                    level_file >> scale.x >> scale.y >> scale.z;

                    glm::vec3 velocity;
                    level_file >> velocity.x >> velocity.y >> velocity.z;

                    glm::vec3 rVelocity;
                    level_file >> rVelocity.x >> rVelocity.y >> rVelocity.z;

                    int visible;
                    level_file >> visible;

                    float brightness;
                    level_file >> brightness;

                    glm::vec3 color;
                    level_file >> color.x >> color.y >> color.z;

                    float intensity;
                    level_file >> intensity;

                    if (meshType == "OBJModel") {
                        AddSceneSun(geometryIndex, materialIndex, name, position, angle,
                            scale, velocity, rVelocity, visible, brightness, color, intensity);
                    }

                }
                else if (s == "Environment") {
                    std::string environment_path;
                    level_file >> environment_path;
                    AddEnvironment(environment_path);

                }
            }
        }
    }

} // namspace Chotra


