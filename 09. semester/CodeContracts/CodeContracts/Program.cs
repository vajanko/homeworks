using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics.Contracts;

namespace CodeContracts
{
    class Program
    {
        static void Main(string[] args)
        {
            Graph graph = new Graph();

            var first = 
                graph.Create(1).Connect(
                    graph.Create(2).Connect(
                        graph.Get(1)), 
                    graph.Create(3).Connect(
                        graph.Create(4),
                        graph.Create(5).Connect(
                            graph.Get(2)),
                        graph.Create(6).Connect(
                            graph.Get(1))
                        )
                    );

            IntCollection stack = new IntCollection();
            stack.Add(first.Id);

            IntCollection searchPath = new IntCollection();

            while (stack.Size() > 0)
            {
                int id = stack.Get(stack.Size() - 1);
                searchPath.Add(id);

                Node node = graph.Get(id);
                foreach(Node n in node.Neighbors)
                {
                    if (searchPath.c)
                }

            }


            //IntCollection col = new IntCollection();

            //Contract.Assert(col.Size() == 0);

            //col.Add(100);
            //Contract.Assert(col.Size() == 1);
            //Contract.Assert(col.Get(0) == 100);

            //col.Add(200);
            //Contract.Assert(col.Size() == 2);
            //Contract.Assert(col.Get(0) == 100);
            //Contract.Assert(col.Get(1) == 100);

            

            //Contract.Assert(col.Size() == 0);
            //col.Add(1);
            ////Contract.Assert(col.Size() == 2);
            //int val = col.Get(2);
            //int size = col.Size();
        }
    }
}
