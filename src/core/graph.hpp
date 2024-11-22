#pragma once

#include <vector>
#include <map>
#include <span>
#include <ranges>
#include <cassert>

namespace core
{

    enum class GraphError
    {
        InvalidNode
    };

    typedef std::vector<int> NodePins;

    struct Node
    {
        float value {0.0f};
        NodePins input_pins;
        NodePins output_pins;
    };

    typedef std::map<int, Node> NodesMap;

    struct Link
    {
        int id {0};
        int from {0};
        int to {0};
    };
    
    typedef std::map<int, Link> LinksMap;

    enum class PinType
    {
        None,
        Input,
        Output
    };
    
    struct Pin
    {
        int id {0};
        PinType type {PinType::None};
    };

    typedef std::map<int, int> PinsMap; 

    bool link_contain_node(const Link& link, int node_id);


    class Graph
    {
    public:
        Graph() = default;
        
        const NodesMap& get_nodes_map() const;

        Node& get_node(int node_id);
        const Node& get_node(int node_id) const;

        const std::vector<int>& get_node_links(int node_id) const;
        size_t get_node_links_count(int node_id) const;
        std::vector<int> get_node_in_links(int node_id) const;
        std::vector<int> get_node_out_links(int node_id) const;
        
        const Link& get_link(int link_id) const;

        const LinksMap& get_links_map() const;

        int insert_node(const Node& node);
        void erase_node(int node_id);

        int insert_link(int from, int to);
        void erase_link(int link_id);

    private:
        int m_current_id {1};
        NodesMap m_nodes;
        LinksMap m_links;
        PinsMap m_pins;

        std::map<int, std::vector<int>> m_node_links;
    };
}
