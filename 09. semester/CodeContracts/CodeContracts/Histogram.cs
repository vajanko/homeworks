using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeContracts
{
    /// <summary>
    /// Encapsulates a collection of characters and its occurences in some peace of text.
    /// </summary>
    [ContractVerification(false)]
    class Histogram
    {
        private readonly IntArray data;

        /// <summary>
        /// Create a new histogram initialized with data representation as described in <see cref="Huffman"/>
        /// </summary>
        public Histogram(IntArray data)
        {
            Contract.Requires(data != null);

            this.data = data;
        }
        /// <summary>
        /// Sort historgam table by number of character occurences. Characters with the same number
        /// of occurences are sorted by the character ordinal value.
        /// </summary>
        public void Sort()
        {
            // this is special value so that GetHigher method won't throw an exception
            data.Add(int.MaxValue);

            int curItem = 0;
            // -1 is because of the int.MaxValue
            while (curItem != int.MaxValue)
            //for (int i = 0; i < data.Size() - 1; i++)
            {
                // get next value in the ordering
                curItem = data.GetHigher(curItem);
                // remove value from its current position ...
                data.RemoveAll(curItem);
                // ... and place it at the end
                data.Add(curItem);
            }

            // this was just auxiliary value and can be removed at the end
            data.RemoveAll(int.MaxValue);
        }

        public override string ToString()
        {
            StringBuilder str = new StringBuilder();
            for (int i = 0; i < data.Size(); i++)
            {
                int item = data.Get(i);
                if (Huffman.GetCount(item) > 0)
                    str.AppendFormat("{0}: {1}\n", Huffman.GetChar(item), Huffman.GetCount(item));
            }
            return str.ToString();
        }
    }
}
