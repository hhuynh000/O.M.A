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
            core::Node node{0.0f};
            const int node_id = m_graph.insert_node(node);
            ImNodes::SetNodeScreenSpacePos(node_id, ImGui::GetMousePos());
        }

        for (int node_uid : m_graph.uids())
        {
            ImNodes::BeginNode(node_uid);
            
            ImNodes::BeginNodeTitleBar();
            ImGui::Text("ID: %d", node_uid);
            ImNodes::EndNodeTitleBar();

            core::UidMap<core::Node>::iterator node = m_graph.get_node(node_uid);

            for (int pin_uid : node->pins)
            {
                core::UidMap<core::Pin>::const_iterator pin = m_graph.get_pin(pin_uid);
                if (pin->type == core::PinType::Input)
                {
                    ImNodes::BeginInputAttribute(pin_uid);
                    ImGui::TextUnformatted("Input");
                    ImNodes::EndInputAttribute();
                }
                else
                {
                    ImNodes::BeginOutputAttribute(pin_uid);
                    ImGui::TextUnformatted("Output");
                    ImNodes::EndOutputAttribute();
                }
            }

            ImNodes::EndNode();
        }

        for (const core::Link& link : m_graph.links())
        {
            ImNodes::Link(link.id, link.from, link.to);
        }

        ImNodes::EndNodeEditor();

        int from = 0;
        int to = 0;
        if (ImNodes::IsLinkCreated(&from, &to))
        {
            printf("Link create %d %d\n", from, to);
            m_graph.insert_link(from, to);
        }

        {
            int link_id = 0;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
                m_graph.erase_link(link_id);
            }
        }

        {
            const int num_selected = ImNodes::NumSelectedLinks();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                std::vector<int> selected_links;
                selected_links.resize(static_cast<size_t>(num_selected));
                ImNodes::GetSelectedLinks(selected_links.data());
                for (const int link_id : selected_links)
                {
                    m_graph.erase_link(link_id);
                }
            }
        }

        {
            const int num_selected = ImNodes::NumSelectedNodes();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                std::vector<int> selected_nodes;
                selected_nodes.resize(static_cast<int>(num_selected));
                ImNodes::GetSelectedNodes(selected_nodes.data());
                for (const int node_id : selected_nodes)
                {
                    m_graph.erase_node(node_id);
                }
            }
        }

    }
    ImGui::End();
}