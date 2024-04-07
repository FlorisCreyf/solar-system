#ifndef SOLARSYSTEM_OBJECTSHADER_H
#define SOLARSYSTEM_OBJECTSHADER_H

#include "Shader.h"

namespace Solar {

    class ObjectShader : public Shader {
    public:
        ObjectShader();
        void draw(const Buffer &buffer, Allocation alloc, Color color, const float transform[9]) const;
    };

}

#endif //SOLARSYSTEM_OBJECTSHADER_H
