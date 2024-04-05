#ifndef SOLARSYSTEM_OBJECTSHADER_H
#define SOLARSYSTEM_OBJECTSHADER_H

#include "Shader.h"

namespace Solar {

    class ObjectShader : public Shader {
    public:
        ObjectShader();
        void draw(const Buffer &buffer, Allocation alloc, const float transform[9]) const;
    };

}

#endif //SOLARSYSTEM_OBJECTSHADER_H
