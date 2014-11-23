using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics.Contracts;

namespace CodeContracts
{
    public class IntCollection
    {
        [ContractInvariantMethod]
        private void invariant()
        {
            Contract.Invariant(data != null);
            Contract.Invariant(Size() >= 0);
        }
        [ContractAbbreviator]
        private void indexInBounds(int index)
        {
             Contract.Requires(index >= 0 && index < Size());
        }

        private readonly List<int> data = new List<int>();

        [Pure]
        public int Size()
        {
            return data.Count;
        }

        public void Add(int val)
        {
            Contract.Ensures(Contract.OldValue(Size()) + 1 == Size());

            data.Add(val);
        }
        [Pure]
        public int Get(int index)
        {
            Contract.Requires(index >= 0 && index < Size(), "Index in bounds");

            return data[index];
        }
        [Pure]
        public int GetHigher(int val)
        {
            if (data.All(i => i <= val))
                return val;

            return data.First(i => i > val);
        }
        public void Remove(int index)
        {
            // this condition also cover the case when collection is empty
            Contract.Requires(index >= 0 && index < Size());
            Contract.Ensures(Contract.OldValue(Size()) + 1 == Size());

            data.RemoveAt(index);
        }
        public void RemoveAll(int val)
        {
            data.RemoveAll(i => i == val);
        }
        public void Sort()
        {
            Contract.Ensures( data.Where((i, v) => i < Size() - 1 && v < Get(i + 1)).Count() == 0 );

            data.Sort();
        }
        public void Clear()
        {
            Contract.Ensures(Size() == 0);

            data.Clear();
        }
        
    }
}
