#pragma once

#include <vector>

#include <imgui.h>
#include <imnodes.h>

namespace flow
{
    struct Node
    {
        int id {0};
        float value {0.0f};
    };

    struct Link
    {
        int id {0};
        int start {0};
        int end {0};
    };

    class Editor
    {
    public:
        Editor() = default;
        void show(const char* editor_name);
        ImNodesEditorContext* context {nullptr};
    
    private:
        std::vector<Node> m_nodes;
        std::vector<Link> m_links;
        int m_current_id {0};
    };

    void initialize_node_editor(Editor& editor);
    void show_node_editor(Editor& editor);
    void shutdown_node_editor(Editor& editor);
}
