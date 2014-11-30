using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeContracts
{
    class Node
    {
        public int Id { get; set; }
        public Node[] Neighbors { get; set; }

        public Node()
        {
            Neighbors = new Node[] { };
        }
        public override string ToString()
        {
            return string.Format("{0} -> {1}", Id, string.Join(", ", Neighbors.Select(n => n.Id)));
        }
    }
}
