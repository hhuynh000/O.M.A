#include "graph.hpp"

using namespace core;


bool core::link_contain_node(const Link& edge, int node_id)
{
    return edge.from == node_id || edge.to == node_id;
}

const NodesMap& Graph::get_nodes_map() const
{
    return m_nodes;
}

Node& Graph::get_node(const int node_id)
{
    assert(m_nodes.contains(node_id));
    return m_nodes.at(node_id);
}

const Node& Graph::get_node(const int node_id) const
{
    assert(m_nodes.contains(node_id));
    return m_nodes.at(node_id);
}

const std::vector<int>& Graph::get_node_links(const int node_id) const
{
    assert(m_node_links.contains(node_id));
    return m_node_links.at(node_id);
}

std::vector<int> Graph::get_node_in_links(const int node_id) const
{
    assert(m_node_links.contains(node_id));
    std::vector<int> in_links;
    for (const int& link_id : m_node_links.at(node_id))
    {   
        const Link& link = get_link(link_id);
        if (link.from == node_id)
        {
            in_links.push_back(link_id);
        }
    }

    return in_links;
}

std::vector<int> Graph::get_node_out_links(const int node_id) const
{
    assert(m_node_links.contains(node_id));
    std::vector<int> out_links;
    for (const int& link_id : m_node_links.at(node_id))
    {   
        const Link& link = get_link(link_id);
        if (link.to == node_id)
        {
            out_links.push_back(link_id);
        }
    }

    return out_links;
}

const Link& Graph::get_link(const int link_id) const
{
    assert(m_links.contains(link_id));
    return m_links.at(link_id);
}

const LinksMap& Graph::get_links_map() const
{
    return m_links;
}

size_t Graph::get_node_links_count(const int node_id) const
{
    assert(m_node_links.contains(node_id));
    return m_node_links.at(node_id).size();
}

int Graph::insert_node(const Node& node)
{
    const int id = m_current_id++;
    m_nodes.insert({id, node});
    m_node_links.insert({id, std::vector<int>()});
    printf("Node ID %d", id);

    return id;
}

void Graph::erase_node(const int node_id)
{
    std::vector<int> edges_to_remove;

    for (const auto& [_, edge] : m_links)
    {
        if (link_contain_node(edge, node_id))
        {
            edges_to_remove.push_back(edge.id);
        }
    }

    for (const int edge_id : edges_to_remove)
    {
        erase_link(edge_id);
    }

    m_nodes.erase(node_id);
    m_node_links.erase(node_id);
}

int Graph::insert_link(const int from, const int to)
{
    const int id = m_current_id++;
    assert(!m_links.contains(id));
    assert(m_pins.contains(from));
    assert(m_nodes.contains(to));
    m_links.insert({id, Link{id, from, to}});

    assert(m_node_links.contains(from));
    m_node_links.at(from).push_back(to);
    printf("Link ID: %d", id);

    return id;
}

void Graph::erase_link(const int edge_id)
{
    assert(m_links.contains(edge_id));
    const Link& edge = m_links.at(edge_id);

    assert(m_node_links.contains(edge.from));
    const size_t edge_count = m_node_links.at(edge.from).size();
    assert(edge_count > 0);

    std::vector<int> neighbor = m_node_links.at(edge.from);
    auto iter = std::find(neighbor.begin(), neighbor.end(), edge.to);
    assert(iter != neighbor.end());
    neighbor.erase(iter);
    
    m_links.erase(edge_id);
}
