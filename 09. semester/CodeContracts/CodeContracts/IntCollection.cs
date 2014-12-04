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

        private readonly List<int> data = new List<int>();

        [Pure]
        public int Size()
        {
            return data.Count;
        }

        public void Add(int val)
        {
            // size increases by 1
            Contract.Ensures(Contract.OldValue(Size()) + 1 == Size());
            // value is added at the end
            Contract.Ensures(Get(Size()) == val);

            data.Add(val);
        }
        [Pure]
        public int Get(int index)
        {
            // given index does not exceeds array bounds
            Contract.Requires(index >= 0 && index < Size(), "Index in bounds");

            return data[index];
        }
        [Pure]
        public int GetHigher(int val)
        {
            // there exists an item that is smaller than given value "val"
            Contract.Requires(Contract.Exists(0, Size(), i => Get(i) < val));
            // returned value is higher than "val"
            Contract.Ensures(Contract.Result<int>() > val);

            return data.First(i => i > val);
        }
        public void Remove(int index)
        {
            // this condition also cover the case when collection is empty
            // can not call Remove on empty collection
            Contract.Requires(index >= 0 && index < Size());
            // size decreases by 1
            Contract.Ensures(Contract.OldValue(Size()) + 1 == Size());

            data.RemoveAt(index);
        }
        public void RemoveAll(int val)
        {
            // size decreases by count of "val" items in the collection
            Contract.Ensures(Contract.OldValue(Size()) - data.Count(v => v == val) == Size());

            data.RemoveAll(i => i == val);
        }
        public void Sort()
        {
            // all items are sorted properly
            Contract.ForAll(0, Size() - 1, i => Get(i) <= Get(i + 1));
            //Contract.Ensures( data.Where((i, v) => i < Size() - 1 && v < Get(i + 1)).Count() == 0 );

            data.Sort();
        }
        public void Clear()
        {
            Contract.Ensures(Size() == 0);

            data.Clear();
        }
        
    }
}
