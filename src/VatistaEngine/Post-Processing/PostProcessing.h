#pragma once
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Shader.h"
#include "FrameBuffer.h"


namespace Vatista {
    struct PostPass {
        SharedPtr(PostPass);
        Vatista::Shader::Sptr Shader;
        Vatista::FrameBuffer::Sptr Output;
    };

//    struct PostProcessing {
//        Mesh::Sptr fullscreenQuad;
//        std::vector<PostPass> myPasses;
//    };
}