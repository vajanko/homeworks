using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeContracts
{
    class IntArray
    {
        private const int MIN_DATA_SIZE = 4;

        [ContractInvariantMethod]
        private void invariant()
        {
            Contract.Invariant(Data != null);
            Contract.Invariant(UsedSize >= 0);
            Contract.Invariant(UsedSize <= Data.Length);
            Contract.Invariant(Data.Length > 0);
        }

        public int[] Data;
        public int UsedSize;

        public IntArray()
        {
            Contract.Ensures(UsedSize == 0);
            Contract.Ensures(Data.Length == MIN_DATA_SIZE);

            Data = new int[MIN_DATA_SIZE];
            UsedSize = 0;
        }

        [Pure]
        public int Get(int index)
        {
            Contract.Requires(index >= 0 && index < UsedSize, "Index out of bounds");
            Contract.Ensures(Contract.Result<int>() == Data[index]);

            return Data[index];
        }

        public void Add(int val)
        {
            // size of used array elements increased by 1
            Contract.Ensures(Contract.OldValue<int>(UsedSize) + 1 == UsedSize);

            // if UsedSize reaches its limit (Data.Length) the Data size will be
            // doubled, otherwise it stay unchanged
            // !! Notice that Data.Length must be always greater than zero otherwise this
            // ensure does not hold !! (it is an invariant)
            Contract.Ensures(
                (Contract.OldValue<int>(UsedSize) < Contract.OldValue<int>(Data.Length) &&
                    Contract.OldValue<int>(Data.Length) == Data.Length) ||
                (Contract.OldValue<int>(UsedSize) == Contract.OldValue<int>(Data.Length) &&
                    Contract.OldValue<int>(Data.Length) * 2 == Data.Length));

            // previously used part of the array wasn't changed
            //Contract.Ensures(Contract.OldValue(UsedSize) == 0 ||
            //    Contract.ForAll(0, Contract.OldValue(UsedSize), i => Contract.OldValue(Data[i]) == Data[i]));
            //Contract.Ensures(Data[Contract.OldValue(UsedSize)] == val);
            //Contract.Ensures(Get(Contract.OldValue(UsedSize)) == val);

            if (Data.Length == UsedSize)
            {
                int[] tmp = Data;
                Data = new int[Data.Length * 2];
                Array.Copy(tmp, Data, tmp.Length);
            }

            int index = UsedSize;
            Data[index] = val;
            UsedSize++;
        }

        public void Remove(int index)
        {
            // index is in array bounds - this also covers the case when array is empty (UsedSize == 0)
            Contract.Requires(index >= 0 && index < UsedSize, "Index out of bounds");

            // size of used array elements decreased by 1
            Contract.Ensures(Contract.OldValue(UsedSize) - 1 == UsedSize);

            Contract.Ensures(
                (UsedSize <= Contract.OldValue<int>(Data.Length) / 4 && 
                    Contract.OldValue<int>(Data.Length) / 2 >= MIN_DATA_SIZE &&
                    Contract.OldValue<int>(Data.Length) / 2 == Data.Length) ||
                ((UsedSize > Contract.OldValue<int>(Data.Length) / 4 ||
                    Contract.OldValue<int>(Data.Length) / 2 < MIN_DATA_SIZE) &&
                    Contract.OldValue<int>(Data.Length) == Data.Length));

            UsedSize--;

            if (UsedSize <= Data.Length / 4 && Data.Length / 2 >= MIN_DATA_SIZE)
            {
                int[] tmp = Data;
                Data = new int[Data.Length / 2];
                Array.Copy(tmp, Data, Data.Length);
            }
        }

        public void Sort()
        {
            // number of elements stay unchanged
            Contract.Ensures(Contract.OldValue(UsedSize) == UsedSize);
            Contract.Ensures(Contract.OldValue(Data.Length) == Data.Length);

            // data are really sorted
            // TODO: 
            //Contract.Ensures(Contract.ForAll(0, UsedSize, i => i == UsedSize - 1 || Data[i] <= Data[i + 1]));
            
            Array.Sort(Data);

            //Contract.Assert(Contract.ForAll(0, UsedSize, i => i == UsedSize || Get(i) <= Get(i + 1)));
        }

        [Pure]
        public int GetHigher(int val)
        {
            // there is no higher value than int.MaxValue
            Contract.Requires(val < int.MaxValue);
            // there exists an item that is higher value than given "val"
            Contract.Requires(Contract.Exists(0, UsedSize, i => val < Data[i]));
            // returned value is higher than "val" ...
            Contract.Ensures(Contract.Result<int>() > val);
            // ... and it is minimal such value
            Contract.Ensures(Contract.ForAll(0, UsedSize, i => Data[i] <= val || Data[i] >= Contract.Result<int>()));

            int res = int.MaxValue;
            for (int i = 0; i < UsedSize; i++)
            {
                if (Data[i] > val && Data[i] < res)
                    res = Data[i];
            }

            return GetHighers(val).Min();

            //return res;
            //int? res = Data.Where((v, i) => i < UsedSize && v > val).OrderBy(v => v).FirstOrDefault();
            //if (res.HasValue)
            //    return res.Value;

            //return val + 1;  // this should never happen

            //return Data.Where(v => v > val).OrderBy(v => v).First();
        }
        [Pure]
        public int[] GetHighers(int val)
        {
            // there is no higher value than int.MaxValue
            Contract.Requires(val < int.MaxValue);
            // there exists at least one item that is higher value than given "val"
            Contract.Requires(Contract.Exists(0, UsedSize, i => val < Data[i]));
            // all returned values are higher than "val"
            Contract.Ensures(Contract.ForAll(Contract.Result<int[]>(), v => v > val));
            // result is non-empty
            Contract.Ensures(Contract.Result<int[]>().Any());

            return Data.Where((v, i) => i < UsedSize && v > val).ToArray();
        }
        //public int Min(int[] values)
        //{
        //    Contract.Requires(values.Any());
        //    Contract.Ensures(Contract.ForAll(values, v => v >= Contract.Result<int>()));
        //    Contract.Ensures(values.Contains(Contract.Result<int>()));

        //    return 
        //}

        //public void RemoveAll(int val)
        //{
        //    // 
        //    Contract.Ensures(Contract.OldValue(Data.Length) == Data.Length);
        //    // number of elements could be decreased
        //    //Contract.Ensures(Contract.OldValue(UsedSize) <= UsedSize);


        //    //Contract.Ensures(Contract.OldValue(UsedSize) -
        //    //    Contract.OldValue(Data).Where((v, i) => i < Contract.OldValue(UsedSize) && v == val).Count()
        //    //    == UsedSize);

        //    // all values has been removed
        //    // Contract.Ensures(Contract.ForAll(0, UsedSize, i => Data[i] != val));

        //    //Contract.OldValue(Data).Where((v, i) => i < Contract.OldValue(UsedSize) && v == val).Count()
        //    //Contract.ForAll(0, Contract.OldValue(UsedSize), i => Data[i])

        //    //Contract.Ensures(Contract.ForAll(0, UsedSize, i => Data[i] != val));

        //    int pushLeft = 0;
        //    for (int i = 0; i < UsedSize; i++)
        //    {
        //        if (Data[i] == val)
        //            pushLeft++;
        //        else
        //            Data[i - pushLeft] = Data[i];
        //    }
        //    UsedSize -= pushLeft;

        //    Contract.Assert(Contract.ForAll(0, UsedSize, i => Data[i] != val), "Value missed to be removed");
        //}
        //public void RemoveFirstFrom(int index, int val, ref int pushLeft)
        //{
        //    Contract.Requires(index >= 0 && index < UsedSize, "Index out of bounds");
        //    Contract.Requires(Contract.ForAll(0, index - 1, i => Data[i] != val));
        //    Contract.Ensures(Contract.ForAll(0, index, i => Data[i] != val));

        //    while ()
        //    Data[index] = Data[index + pushLeft];
        //}
        
    }
}
