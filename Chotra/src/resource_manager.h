#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Chotra {

    class MaterialTexture;
    class Material;
    class ObjModel;
    class Scene;
    class Camera;
    class Renderer;

    class ResourceManager {
    public:
        static unsigned int AddTexture(std::string path);
        static void ChangeTexture(unsigned int i, std::string path);
        static unsigned int& GetTextureId(unsigned int i);
        static unsigned int GetTexturesCount();
        static std::string GetTexturePath(unsigned int i);

        static unsigned int AddMaterial(std::string path);
        static void MakeMaterialIcon(unsigned int i);
        static void DeleteMaterialIcon(unsigned int i);
        static void SetMaterialIcon(unsigned int i, unsigned int icon);
        static void ChangeMaterialSource(unsigned int i, std::string path);
        static void ChangeComponentIndex(unsigned int i, std::string componentsName, int chosed);
        static std::shared_ptr<Material> GetMaterial(unsigned int i);
        static unsigned int GetMaterialsCount();
        static std::string GetMaterialName(unsigned int i);
        static unsigned int& GetMaterialIcon(unsigned int);
        static std::string GetMaterialPath(unsigned int i);

        static unsigned int AddGeometry(std::string path);
        static void MakeGeometryIcon(unsigned int i);
        static void DeleteGeometryIcon(unsigned int i);
        static void SetGeometryIcon(unsigned int i, unsigned int icon);
        static void ChangeGeometrySource(unsigned int i, std::string path);
        static std::string GetGeometryName(unsigned int i);
        static unsigned int& GetGeometryIcon(unsigned int);
        static std::string GetGeometryPath(unsigned int i);
        static unsigned int GetGeometriesCount();
        static unsigned int GetGeometryVerticesCount(unsigned int i);
        static unsigned int GetGeometryVAO(unsigned int i);

        static void MakeScene(std::string path);
        static std::shared_ptr<Scene> GetScene();
        static void UpdateScene(float deltaTime);

        static void MakeCamera(glm::vec3 position);
        static std::shared_ptr<Camera> GetCamera();

        static void MakeMiniScene(std::string path);
        static void MakeMiniCamera(glm::vec3 position);

        static std::shared_ptr<Scene> scene;
        static std::shared_ptr<Camera> camera;

        static std::shared_ptr<Scene> miniScene;
        static std::shared_ptr<Camera> miniCamera;

        static std::shared_ptr<Renderer> p_renderer; //For rendering Igcons of GUI

    private:
                
        static std::vector<std::shared_ptr<MaterialTexture>> textures;
        static std::vector<std::shared_ptr<Material>> materials;
        static std::vector<std::shared_ptr<ObjModel>> geometries;

    };


} // namespace Chotra
#endif
