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
            //// from given text file create a historgram of character occurences
            //Histogram hist = Huffman.CreateHistogram("file.txt");
            //Console.WriteLine("Histrogram:\n{0}", hist);

            //// sort historgram table by the number of occurences
            //hist.Sort();

            //Console.WriteLine("Sorted histrogram:\n{0}", hist);

            //Console.WriteLine("Press any key to continue ... :(Do you know where the \"any\" key is? :)");
            //Console.ReadKey();


            IntArray arr = new IntArray();
            Contract.Assert(arr.UsedSize == 0);
            arr.Add(100);
            Contract.Assert(arr.UsedSize == 1);
            arr.Remove(0);
            Contract.Assert(arr.UsedSize == 0);
            arr.Add(100);
            arr.Add(200);
            arr.Add(100);
            arr.Add(300);
            arr.Add(100);
            arr.Add(100);
            //arr.RemoveAll(100);
            //Contract.Assert(arr.Get(0) == 100);

            //IntCollection col = new IntCollection();

            //Contract.Assert(col.Size() == 0);
            //col.Add(100);
            //Contract.Assert(col.Size() == 1);
            //col.Add(200);
            //Contract.Assert(col.Size() == 2);

            //Contract.Assert(col.Get(0) == 200);
            //int x = col.GetHigher(100);

            //col.Add(200);
            //Contract.Assert(col.Size() == 2);
            //Contract.Assert(col.Get(0) == 100);
            //Contract.Assert(col.Get(1) == 200);

            //col.Remove(0);
            //Contract.Assert(col.Size() == 1);
            //Contract.Assert(col.Get(0) == 100);

            //Contract.Assert(col.Size() == 0);
            //col.Add(1);
            ////Contract.Assert(col.Size() == 2);
            //int val = col.Get(2);
            //int size = col.Size();
        }
    }
}
