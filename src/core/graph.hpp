#pragma once

#include <vector>
#include <map>
#include <span>
#include <ranges>
#include <cassert>

#include "uid_map.hpp"

namespace core
{

    enum class GraphError
    {
        InvalidNode
    };

    typedef std::vector<int> Pins;
    typedef std::vector<int> Links;

    struct Node
    {
        float value {0.0f};
        Pins pins;
        Links links;
    };

    struct Link
    {
        int id {0};
        int from {0};
        int to {0};
    };
    
    enum class PinType
    {
        Input,
        Output
    };

    struct Pin
    {
        int id {0};
        PinType type {PinType::Input};
    };

    bool link_contain_node(const Link& link, int node_id);

    class Graph
    {
    public:
        Graph() = default;
        
        std::span<const Node> nodes() const;
        std::span<const int> uids() const;

        UidMap<Node>::iterator get_node(int node_id);
        UidMap<Node>::const_iterator get_node(int node_id) const;

        const std::vector<int>& get_node_links(int node_id) const;
        size_t get_node_links_count(int node_id) const;
        const std::vector<int>& get_node_pins(int node_id) const;
        
        UidMap<Link>::const_iterator get_link(int link_id) const;
        UidMap<Pin>::const_iterator get_pin(int pin_id) const;

        std::span<const Link> links() const;

        int insert_node(Node& node);
        void erase_node(int node_id);

        int insert_link(int from, int to);
        void erase_link(int link_id);

    private:
        int m_current_id {1};
        UidMap<Node> m_nodes;
        UidMap<Link> m_links;
        UidMap<Pin> m_pins;

        UidMap<int> m_pin_to_node;
    };
}
