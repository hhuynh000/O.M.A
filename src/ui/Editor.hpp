#pragma once

#include "imgui.h"
#include "imnodes.h"
#include "graph.hpp"

namespace ui
{
    struct UiNode
    {
        int id {0};
        float value {0.0f};
    };

    class Editor
    {
    public:
        Editor() = default;
        void show();

    private:
        core::Graph m_graph;
        std::vector<UiNode> m_nodes;
    };
}

