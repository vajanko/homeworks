using System;
using System.Collections.Generic;
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

        //[ContractAbbreviator]
        //private void dataLengthIncrease()
        //{
        //    Contract.Ensures(
        //        (Contract.OldValue<int>(UsedSize) < Contract.OldValue<int>(Data.Length) &&
        //            Contract.OldValue<int>(Data.Length) == Data.Length) ||
        //        (Contract.OldValue<int>(UsedSize) == Contract.OldValue<int>(Data.Length) &&
        //            Contract.OldValue<int>(Data.Length) * 2 == Data.Length));
        //}

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
            // index is in array bounds
            Contract.Requires(index >= 0 && index < UsedSize, "Index out of bounds");

            // size of used array elements decreased by 1
            Contract.Ensures(Contract.OldValue(UsedSize) - 1 == UsedSize);

            //Contract.Ensures(
            //    (Contract.OldValue<int>(UsedSize) < Contract.OldValue<int>(Data.Length) &&
            //        Contract.OldValue<int>(Data.Length) == Data.Length) ||
            //    (Contract.OldValue<int>(UsedSize) == Contract.OldValue<int>(Data.Length) &&
            //        Contract.OldValue<int>(Data.Length) * 2 == Data.Length));

            //if (Data.Length == UsedSize)
            //{
            //    int[] tmp = Data;
            //    Data = new int[Data.Length * 2];
            //    Array.Copy(tmp, Data, tmp.Length);
            //}

            UsedSize--;
        }

        //public void RemoveAll(int val)
        //{
        //    Contract.Ensures(Contract.OldValue(val) == val);
        //    Contract.Ensures(Contract.OldValue(Data.Length) == Data.Length);

        //    //Contract.Ensures(Contract.OldValue(UsedSize) -
        //    //    Contract.OldValue(Data).Where((v, i) => i < Contract.OldValue(UsedSize) && v == val).Count()
        //    //    == UsedSize);
            
            
            
        //    //Contract.OldValue(Data).Where((v, i) => i < Contract.OldValue(UsedSize) && v == val).Count()
        //    //Contract.ForAll(0, Contract.OldValue(UsedSize), i => Data[i])
        //    //Contract.Ensures(Contract.ForAll(0, UsedSize, i => Data[i] != val));

        //    int padLeft = 0;
        //    for (int i = 0; i < UsedSize; i++)
        //    {
        //        if (Data[i] == val)
        //        {
        //            padLeft++;
        //        }
        //        else
        //        {
        //            Data[i - padLeft] = Data[i];
        //        }
        //    }
        //    UsedSize -= padLeft;
        //}
    }
}
