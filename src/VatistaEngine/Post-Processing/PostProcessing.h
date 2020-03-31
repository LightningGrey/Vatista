#pragma once
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Shader.h"
#include "FrameBuffer.h"


namespace Vatista {
    struct PostPass {
        Shader::Sptr Shader;
        FrameBuffer::Sptr Output;
    };

//    struct PostProcessing {
//        Mesh::Sptr fullscreenQuad;
//        std::vector<PostPass> myPasses;
//    };
}