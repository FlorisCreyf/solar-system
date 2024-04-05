#ifndef SOLARSYSTEM_BACKGROUNDSHADER_H
#define SOLARSYSTEM_BACKGROUNDSHADER_H

#include "Shader.h"

namespace Solar {

    class BackgroundShader : public Shader {
    public:
        BackgroundShader(int width, int height);
        void draw(const Buffer &buffer, Allocation alloc, int x, int y) const;

    private:
        int width;
        int height;
    };

}

#endif //SOLARSYSTEM_BACKGROUNDSHADER_H
