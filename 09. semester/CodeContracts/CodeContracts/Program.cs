﻿using System;
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
            Huffman.Histogram("file.txt");

            Console.ReadKey();


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
