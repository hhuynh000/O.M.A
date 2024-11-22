#include "Editor.hpp"

using namespace ui;

void Editor::show()
{
    
    if (ImGui::Begin("Node Editor", nullptr, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Style"))
            {
                if (ImGui::MenuItem("Classic"))
                {
                    ImNodes::StyleColorsClassic();
                }

                if (ImGui::MenuItem("Dark"))
                {
                    ImNodes::StyleColorsDark();
                }

                if (ImGui::MenuItem("Light"))
                {
                    ImNodes::StyleColorsLight();
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenuBar();

        ImNodes::BeginNodeEditor();

        const bool add_node = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                                ImNodes::IsEditorHovered() &&
                                ImGui::IsKeyReleased(ImGuiKey_A);
        
        if (add_node)
        {
            const core::Node node{0.0f};
            const int node_id = m_graph.insert_node(node);
            ImNodes::SetNodeScreenSpacePos(node_id, ImGui::GetMousePos());
        }

        for (const auto& [id, node] : m_graph.get_nodes_map())
        {
            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("ID: %d", id);
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(id);
            ImGui::TextUnformatted("Input");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(id);
            ImGui::TextUnformatted("Output");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();
        }

        for (const auto& [id, edge] : m_graph.get_links_map())
        {
            ImNodes::Link(edge.id, edge.from, edge.to);
        }

        ImNodes::EndNodeEditor();

        int from = 0;
        int to = 0;
        if (ImNodes::IsLinkCreated(&from, &to))
        {
            printf("Link create %d %d\n", from, to);
            m_graph.insert_link(from, to);
        }

        int link_id = 0;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            m_graph.erase_link(link_id);
        }

    }
    ImGui::End();
}