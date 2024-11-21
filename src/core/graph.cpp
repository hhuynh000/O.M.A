#include "graph.hpp"

using namespace flow;

void Editor::show(const char* editor_name)
{
    ImNodes::EditorContextSet(context);

    ImGui::Begin(editor_name);
    ImGui::TextUnformatted("A -- add node");

    ImNodes::BeginNodeEditor();

    const bool add_node = ImGuiFocusedFlags_RootAndChildWindows &&
                            ImNodes::IsEditorHovered() &&
                            ImGui::IsKeyReleased(ImGuiKey_A);

    if (add_node)
    {
        const int node_id = ++m_current_id;
        ImNodes::SetNodeScreenSpacePos(node_id, ImGui::GetMousePos());
        ImNodes::SnapNodeToGrid(node_id);
        m_nodes.push_back(Node{node_id, 0.0f});
    }

    std::vector<int> ids_to_remove;
    for (Node& node : m_nodes)
    {
        ImNodes::BeginNode(node.id);
        
        const bool remove_node = ImNodes::IsNodeSelected(node.id) &&  ImGui::IsKeyReleased(ImGuiKey_D);
        if (remove_node)
        {
            ids_to_remove.push_back(node.id);
        }

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("node");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(node.id << 8);
        ImGui::TextUnformatted("input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginStaticAttribute(node.id << 16);
        ImGui::PushItemWidth(120.0f);
        ImGui::DragFloat("value", &node.value, 0.01f);
        ImGui::PopItemWidth();
        ImNodes::EndStaticAttribute();

        ImNodes::BeginOutputAttribute(node.id << 24);
        const float text_width = ImGui::CalcTextSize("value").x;
        ImGui::Indent(120.f + ImGui::CalcTextSize("value").x - text_width);
        ImGui::TextUnformatted("output");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
    }

    for (const Link& link : m_links)
    {
        ImNodes::Link(link.id, link.start, link.end);
    }

    for (int& id : ids_to_remove)
    {
        auto iter = std::remove_if(m_nodes)
    }

    ImNodes::EndNodeEditor();

    {
        Link link;
        if (ImNodes::IsLinkCreated(&link.start, &link.end))
        {
            link.id = ++m_current_id;
            m_links.push_back(link);
        }
    }

    {
        int link_id = 0;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            auto iter = std::find_if(
                m_links.begin(),
                m_links.end(),
                [link_id](const Link& link) -> bool {
                    return link.id == link_id;
                }
            );
            assert(iter != m_links.end());
            m_links.erase(iter);
        }
    }

    ImGui::End();
}

void flow::initialize_node_editor(Editor& editor)
{
    editor.context = ImNodes::EditorContextCreate();
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

    ImNodesIO& io = ImNodes::GetIO();
    io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    io.MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyCtrl;

    ImNodesStyle& style = ImNodes::GetStyle();
    style.Flags |= ImNodesStyleFlags_GridLinesPrimary | ImNodesStyleFlags_GridSnapping;
}

void flow::show_node_editor(Editor& editor)
{
    editor.show("editor");
}

void flow::shutdown_node_editor(Editor& editor)
{
    ImNodes::PopAttributeFlag();
    ImNodes::EditorContextFree(editor.context);
}
