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
            IntCollection col = new IntCollection();

            Contract.Assert(col.Size() == 0);
            col.Add(1);
            //Contract.Assert(col.Size() == 2);
            int val = col.Get(2);
            int size = col.Size();
        }
    }
}
