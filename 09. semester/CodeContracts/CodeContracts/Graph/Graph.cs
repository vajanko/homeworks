using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeContracts
{
    class Graph
    {
        private Dictionary<int, Node> nodes = new Dictionary<int, Node>();

        public Node Create(int id)
        {
            Node node = new Node() { Id = id };
            nodes.Add(id, node);
            return node;
        }
        public Node Get(int id)
        {
            return nodes[id];
        }
        
    }

    static class NodeExtension
    {
        public static Node Connect(this Node node, params Node[] neighbors)
        {
            node.Neighbors = neighbors;
            return node;
        }
    }
}
