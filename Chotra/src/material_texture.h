#ifndef MATERIAL_TEXTURE_H
#define MATERIAL_TEXTURE_H

#include <string>

#include "texture.h"

namespace Chotra {

    class MaterialTexture : public Texture {
    public:
        std::string path;
        MaterialTexture(std::string path);

    protected:
        
        virtual void GenerateTexture() override;

    };

} // namspace Chotra

#endif
