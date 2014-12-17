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
            //Contract.Invariant(UsedSize == 0 || Contract.ForAll(0, UsedSize, i => Get(i) == Data[i]));

            Contract.Invariant(Size() == UsedSize);
            Contract.Invariant(0 <= Size() && Size() <= Data.Length);
        }

        [ContractAbbreviator]
        private void dataUnchanged()
        {
            Contract.Ensures(Contract.OldValue(UsedSize) == UsedSize);
            Contract.Ensures(Contract.OldValue(Data) == Data);
            Contract.Ensures(Contract.OldValue(Data.Length) == Data.Length);
        }

        public int[] Data;
        public int UsedSize;

        public IntArray()
        {
            Contract.Ensures(Size() == 0);
            Contract.Ensures(UsedSize == 0);
            Contract.Ensures(Data.Length == MIN_DATA_SIZE);

            Data = new int[MIN_DATA_SIZE];
            UsedSize = 0;
        }

        [Pure]
        public int Size() 
        {
            Contract.Ensures(Contract.Result<int>() == UsedSize);

            return UsedSize;
        }

        [Pure]
        public int Get(int index)
        {
            // notice that it was impossible to use Size() instead of UsedSize here
            Contract.Requires(index >= 0 && index < UsedSize, "Index out of bounds");
            Contract.Ensures(Contract.Result<int>() == Data[index]);

            return Data[index];
        }
        public void Set(int index, int val)
        {
            Contract.Requires(index >= 0 && index < UsedSize, "Index out of bounds");
            Contract.Ensures(Get(index) == val && val == Data[index]);
            Contract.Ensures(Contract.OldValue(UsedSize) == UsedSize);
            Contract.Ensures(Contract.OldValue(Data.Length) == Data.Length);
            Contract.Ensures(Data != null);

            Data[index] = val;
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

            // value is added at the end
            Contract.Ensures(Get(Contract.OldValue(UsedSize)) == val);

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
            for (int i = index; i < UsedSize; i++)
                //Set(i, Get(i + 1));
                Data[i] = Data[i + 1];
            
            
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
            //Contract.Ensures(Contract.OldValue(UsedSize) == UsedSize);
            //Contract.Ensures(Contract.OldValue(Data.Length) == Data.Length);

            // data are really sorted
            // TODO: 
            Contract.Ensures(Contract.ForAll(0, UsedSize, i => i == UsedSize - 1 || Get(i) <= Get(i + 1)));

            for (int i = 0; i < UsedSize; i++)
            {
                for (int j = 0; j < UsedSize - 1; j++)
                {
                    SortPair(j, j + 1);
                }
            }
            //Array.Sort(Data);
        }
        public void SortPair(int i, int j)
        {
            Contract.Requires(i >= 0 && i < UsedSize, "Index out of bounds");
            Contract.Requires(j >= 0 && j < UsedSize, "Index out of bounds");
            Contract.Requires(i < j);
            Contract.Ensures(Contract.OldValue(UsedSize) == UsedSize);
            Contract.Ensures(Contract.OldValue(Data.Length) == Data.Length);
            Contract.Ensures(Get(i) <= Get(j));
            

            if (Get(i) > Get(j))
            {
                int tmp = Data[i];
                Data[i] = Data[j];
                Data[j] = tmp;
            }
        }

        //[Pure]
        //public int GetHigher(int val)
        //{
        //    // there is no higher value than int.MaxValue
        //    Contract.Requires(val < int.MaxValue);
        //    // there exists an item that is higher value than given "val"
        //    Contract.Requires(Contract.Exists(0, UsedSize, i => val < Get(i)));
        //    // returned value is higher than "val" ...
        //    Contract.Ensures(Contract.Result<int>() > val);
        //    // ... and it is minimal such value
        //    Contract.Ensures(Contract.ForAll(0, UsedSize, i => Get(i) <= val || Get(i) >= Contract.Result<int>()));


        //    return val + 1;
        //    //int res = int.MaxValue;
        //    //for (int i = 0; i < UsedSize; i++)
        //    //{
        //    //    if (Data[i] > val && Data[i] < res)
        //    //        res = Data[i];
        //    //}

        //    //return GetHighers(val).Min();
        //}
        //[Pure]
        //public int SelectHigher(int val, int i, int j)
        //{
        //    Contract.Requires(i >= 0 && i < UsedSize, "Index out of bounds");
        //    Contract.Requires(j >= 0 && j < UsedSize, "Index out of bounds");
        //    Contract.Requires(i < j);
        //}
        //[Pure]
        //public int[] GetHighers(int val)
        //{
        //    // there is no higher value than int.MaxValue
        //    Contract.Requires(val < int.MaxValue);
        //    // there exists at least one item that is higher value than given "val"
        //    Contract.Requires(Contract.Exists(0, UsedSize, i => val < Data[i]));
        //    // all returned values are higher than "val"
        //    Contract.Ensures(Contract.ForAll(Contract.Result<int[]>(), v => v > val));
        //    // result is non-empty
        //    Contract.Ensures(Contract.Result<int[]>().Any());

        //    return Data.Where((v, i) => i < UsedSize && v > val).ToArray();
        //}
        //public int Min(int[] values)
        //{
        //    Contract.Requires(values.Any());
        //    Contract.Ensures(Contract.ForAll(values, v => v >= Contract.Result<int>()));
        //    Contract.Ensures(values.Contains(Contract.Result<int>()));

        //    return 
        //}

        public void RemoveAll(int val)
        {
            // 
            //Contract.Ensures(Contract.OldValue(Data.Length) == Data.Length);
            // number of elements could be decreased
            //Contract.Ensures(Contract.OldValue(UsedSize) <= UsedSize);


            //Contract.Ensures(Contract.OldValue(UsedSize) -
            //    Contract.OldValue(Data).Where((v, i) => i < Contract.OldValue(UsedSize) && v == val).Count()
            //    == UsedSize);

            // all values has been removed
            // Contract.Ensures(Contract.ForAll(0, UsedSize, i => Data[i] != val));

            //Contract.OldValue(Data).Where((v, i) => i < Contract.OldValue(UsedSize) && v == val).Count()
            //Contract.ForAll(0, Contract.OldValue(UsedSize), i => Data[i])

            Contract.Ensures(Contract.ForAll(0, UsedSize, i => Get(i) != val));

            for (int i = UsedSize - 1; i >= 0; i--)
                if (Get(i) == val)
                    Set(i, val + 1);
                    //RemoveAt(i, val);
            //int pushLeft = 0;
            //for (int i = 0; i < UsedSize; i++)
            //{
            //    if (Data[i] == val)
            //        pushLeft++;
            //    else
            //        Data[i - pushLeft] = Data[i];
            //}
            //UsedSize -= pushLeft;

        }
        //public void RemoveAt(int index, int val)
        //{
        //    Contract.Requires(UsedSize > 0);
        //    Contract.Requires(Get(index) == val);
        //    Contract.Requires(index >= 0 && index < UsedSize, "Index out of bounds");
        //    Contract.Requires(index == UsedSize - 1 || Contract.ForAll(index + 1, UsedSize, i => Get(i) != val));

        //    // size of used array elements decreased by 1
        //    Contract.Ensures(Contract.OldValue(Size()) - 1 == Size());
        //    Contract.Ensures(UsedSize == index || Contract.ForAll(index, UsedSize, i => Get(i) != val));

        //    for (int i = index; i < UsedSize - 1; i++)
        //        //Data[i] = Data[i + 1];
        //        Data[i] = Get(i + 1);
        //        //Set(i, Get(i + 1));
        //        //Copy(i, i + 1);

        //    UsedSize--;
        //}
        public void Rewrite(int i, int j)
        {
            Contract.Requires(i >= 0 && i < UsedSize, "Index out of bounds");
            Contract.Requires(j >= 0 && j < UsedSize, "Index out of bounds");
            Contract.Requires(Get(j) != Get(i));
            Contract.Ensures(Contract.OldValue(Get(i)) != Get(i));

            if (Get(i) == Get(j))
                Set(i, Get(i) + 1);
            else
                Set(i, Get(j));
        }
        //public void RemoveAndPushLeft(int pos, int val)
        //{
        //    Contract.Requires(pos >= 0 && pos < UsedSize);
        //    Contract.Requires(UsedSize == pos || Contract.ForAll(0, pos, i => Get(i) != val));
        //    Contract.Ensures(Contract.OldValue(UsedSize) - 1 == UsedSize);
        //    Contract.Ensures(UsedSize == pos || Contract.ForAll(0, pos, i => Get(i) != val));

            
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
