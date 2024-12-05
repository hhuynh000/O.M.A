#include "graph.hpp"

using namespace core;


bool core::link_contain_node(const Link& link, int node_id)
{
    return link.from == node_id || link.to == node_id;
}

std::span<const Node> Graph::nodes() const
{
    return m_nodes.elements();
}

std::span<const int> Graph::uids() const
{
    return m_nodes.uids();
}

UidMap<Node>::iterator Graph::get_node(const int node_id)
{
    assert(m_nodes.contains(node_id));
    return m_nodes.at(node_id);
}

UidMap<Node>::const_iterator Graph::get_node(const int node_id) const
{
    assert(m_nodes.contains(node_id));
    return m_nodes.at(node_id);
}

const Links& Graph::get_node_links(const int node_id) const
{
    assert(m_nodes.contains(node_id));
    return m_nodes.at(node_id)->links;
}

const Pins& Graph::get_node_pins(const int node_id) const
{
    assert(m_nodes.contains(node_id));
    UidMap<Node>::const_iterator node = m_nodes.at(node_id);
    return node->pins;
}

UidMap<Link>::const_iterator Graph::get_link(const int link_id) const
{
    assert(m_links.contains(link_id));
    return m_links.at(link_id);
}

UidMap<Pin>::const_iterator Graph::get_pin(const int pin_id) const
{
    assert(m_pins.contains(pin_id));
    return m_pins.at(pin_id);
}

std::span<const Link> Graph::links() const
{
    return m_links.elements();
}

size_t Graph::get_node_links_count(const int node_id) const
{
    assert(m_nodes.contains(node_id));
    return m_nodes.at(node_id)->links.size();
}

int Graph::insert_node(Node& node)
{
    const int node_uid = m_current_id++;
    assert(!m_nodes.contains(node_uid));
    m_nodes.insert(node_uid, Node{0.0});

    int pin_uid = m_current_id++;
    m_nodes.at(node_uid)->pins.push_back(pin_uid);
    m_pins.insert(pin_uid, Pin{pin_uid, PinType::Input});
    m_pin_to_node.insert(pin_uid, node_uid);
    printf("IUD: %d\n", pin_uid);
    node.pins.push_back(pin_uid);

    pin_uid = m_current_id++;
    m_nodes.at(node_uid)->pins.push_back(pin_uid);
    m_pins.insert(pin_uid, Pin{pin_uid, PinType::Output});
    m_pin_to_node.insert(pin_uid, node_uid);
    printf("IUD: %d\n", pin_uid);
    node.pins.push_back(pin_uid);

    return node_uid;
}

void Graph::erase_node(const int node_id)
{
    std::vector<int> links_to_remove;

    for (const Link& link : m_links.elements())
    {
        if (link_contain_node(link, node_id))
        {
            links_to_remove.push_back(link.id);
        }
    }

    for (const int link_id : links_to_remove)
    {
        erase_link(link_id);
    }

    m_nodes.erase(node_id);
}

int Graph::insert_link(const int from, const int to)
{
    const int id = m_current_id++;
    assert(!m_links.contains(id));
    assert(m_pins.contains(from));
    assert(m_pins.contains(to));
    m_links.insert(id, Link{id, from, to});

    UidMap<int>::const_iterator node_uid = m_pin_to_node.at(from);
    assert(m_nodes.contains(*node_uid));

    printf("Node Uid: %d\n", *node_uid);
    m_nodes.at(*node_uid)->links.push_back(to);

    return id;
}

void Graph::erase_link(const int link_id)
{
    assert(m_links.contains(link_id));
    UidMap<Link>::const_iterator link = m_links.at(link_id);

    UidMap<int>::const_iterator from_node = m_pin_to_node.at(link->from);
    assert(m_nodes.contains(*from_node));
    const size_t link_count = m_nodes.at(*from_node)->links.size();
    assert(link_count > 0);

    std::vector<int>& neighbor = m_nodes.at(*from_node)->links;
    auto iter = std::find(neighbor.begin(), neighbor.end(), link->to);
    assert(iter != neighbor.end());
    neighbor.erase(iter);
    
    m_links.erase(link_id);
}
