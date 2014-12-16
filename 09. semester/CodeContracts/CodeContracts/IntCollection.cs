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
        //[ContractInvariantMethod]
        //private void invariant()
        //{
        //    Contract.Invariant(data != null);
        //    Contract.Invariant(Size() >= 0);
        //    //Contract.Invariant(Size() == usedSize);
        //}

        //private int[] data;
        //private int usedSize;

        //public IntCollection()
        //{
        //    Contract.Ensures(Size() == 0);

        //    data = new int[4];
        //    usedSize = 0;
        //}

        //// Basic queries

        //[Pure]
        //public int Size()
        //{
        //    Contract.Ensures(Contract.Result<int>() >= 0);
        //    //Contract.Ensures(Contract.Result<int>() == usedSize);

        //    return usedSize;
        //    //return data.Count;
        //}
        //[Pure]
        //public int Get(int index)
        //{
        //    // given index does not exceeds array bounds
        //    Contract.Requires(index >= 0 && index < Size(), "Index out of range.");

        //    return data[index];
        //}
        //public void Set(int index, int val)
        //{
        //    Contract.Requires(index >= 0 && index < Size(), "Index out of range.");
        //    Contract.Ensures(Contract.OldValue(Size()) == Size());

        //    data[index] = val;
        //}
        //[Pure]
        //public int GetHigher(int val)
        //{
        //    // there exists an item that is smaller than given value "val"
        //    Contract.Requires(Contract.Exists(0, Size(), i => val <  Get(i)));
        //    // returned value is higher than "val"
        //    Contract.Ensures(Contract.Result<int>() > val && 
        //        // and it is minimal such value
        //        Contract.ForAll(0, Size(), i => Get(i) <= val || Get(i) >= Contract.Result<int>()));

        //    return data.Where(i => i > val).OrderBy(i => i).First();
        //}

        // Commands
        //public void Add(int val)
        //{
        //    // size increases by 1
        //    Contract.Ensures((Contract.OldValue(Size()) + 1) == Size());
        //    // value is added at the end
        //    //Contract.Ensures(Get(Contract.OldValue(Size())) == val);

        //    int index = usedSize;
        //    usedSize++;
        //    Set(index, val);
        //    //data[usedSize] = val;
        //}
        //public void Remove(int index)
        //{
        //    // this condition also cover the case when collection is empty
        //    // can not call Remove on empty collection
        //    Contract.Requires(index >= 0 && index < Size());
        //    // size decreases by 1
        //    Contract.Ensures(Contract.OldValue(Size()) - 1 == Size());

        //    data.RemoveAt(index);
        //}
        //public void RemoveAll(int val)
        //{
        //    // size decreases by count of "val" items in the collection
        //    //Contract.Ensures(Contract.OldValue(Size()) - data.Count(v => v == val) == Size());

        //    data.RemoveAll(i => i == val);
        //}
        //public void Sort()
        //{
        //    // all items are sorted properly
        //    //Contract.Ensures(Contract.ForAll(0, Size() - 1, i => Get(i) <= Get(i + 1)));

        //    data.Sort();
        //}
        //public void Clear()
        //{
        //    Contract.Ensures(Size() == 0);

        //    data.Clear();
        //}

        //public override string ToString()
        //{
        //    return string.Format("[{0}]", string.Join(", ", data));
        //}
        
    }
}
