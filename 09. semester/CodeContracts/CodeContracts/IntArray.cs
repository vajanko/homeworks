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
        private const int MIN_SIZE = 0;

        [ContractInvariantMethod]
        private void invariant()
        {
            Contract.Invariant(data != null);
            Contract.Invariant(_length >= MIN_SIZE && _length <= data.Length);
            Contract.Invariant(data.Length >= MIN_DATA_SIZE);

            Contract.Invariant(AllocatedLength == data.Length);
            Contract.Invariant(Length == _length);
            //Contract.Invariant(Length == Size());

            // this would be nice
            //Contract.Invariant(UsedSize == 0 || Contract.ForAll(0, Size(), i => Get(i) == Data[i]));

            Contract.Invariant(0 <= Length && Length <= AllocatedLength);
        }

        [ContractAbbreviator]
        private void Ensures_LengthsUnchanged()
        {
            Contract.Ensures(Contract.OldValue(Length) == Length);
            Contract.Ensures(Contract.OldValue(AllocatedLength) == AllocatedLength);
        }
        [ContractAbbreviator]
        private void Ensures_IndexInBounds(int index)
        {
            Contract.Requires(index >= 0 && index < Length && index < AllocatedLength, "Index out of bounds");
        }

        private int[] data;
        private int _length;

        [Pure]
        public int AllocatedLength { get { return data.Length; } }
        public int Length 
        { 
            [Pure]
            get { return _length; } 
            private set 
            {
                Contract.Requires(value >= MIN_SIZE && value <= AllocatedLength);
                _length = value; 
            }
        }
        public int this[int index]
        {
            [Pure]
            get 
            {
                Ensures_IndexInBounds(index);
                return data[index];
            }
            private set
            {
                Ensures_IndexInBounds(index);
                Contract.Ensures(Contract.OldValue(Length) == Length);
                Contract.Ensures(Contract.OldValue(AllocatedLength) == AllocatedLength);
                Contract.Ensures(this[index] == value);

                data[index] = value;
            }
        }

        public IntArray()
        {
            Contract.Ensures(Length == MIN_SIZE);
            Contract.Ensures(AllocatedLength == MIN_DATA_SIZE);

            data = new int[MIN_DATA_SIZE];
            _length = MIN_SIZE;
        }

        [Pure]
        public int Size()
        {
            Contract.Ensures(Contract.Result<int>() == Length);

            return Length;
        }
        [Pure]
        public int Get(int index)
        {
            Ensures_IndexInBounds(index);
            Contract.Ensures(Contract.Result<int>() == this[index]);

            return data[index];
        }

        public void Add(int val)
        {
            // size of used array elements increased by 1
            Contract.Ensures(Contract.OldValue<int>(Length) + 1 == Length);

            // if Length reaches its limit (AllocatedLength) the data[] size will be
            // doubled, otherwise it stay unchanged
            // !! Notice that AllocatedLength must be always greater than zero otherwise this
            // ensure does not hold !! (it is an invariant)
            Contract.Ensures(
                (Contract.OldValue<int>(Length) < Contract.OldValue<int>(AllocatedLength) &&
                    Contract.OldValue<int>(AllocatedLength) == AllocatedLength) ||
                (Contract.OldValue<int>(Length) == Contract.OldValue<int>(AllocatedLength) &&
                    Contract.OldValue<int>(AllocatedLength) * 2 == AllocatedLength));

            // value is added at the end
            Contract.Ensures(Get(Contract.OldValue(Length)) == val);

            // previously used part of the array wasn't changed
            //Contract.Ensures(Contract.OldValue(Length) == 0 ||
            //    Contract.ForAll(0, Contract.OldValue(Length), i => Contract.OldValue(Get(i)) == Get(i)));

            if (AllocatedLength == Length)
            {
                int[] tmp = data;
                data = new int[AllocatedLength * 2];
                Array.Copy(tmp, data, tmp.Length);
            }

            data[Length] = val;
            Length++;
        }

        public void Remove(int index)
        {
            Ensures_IndexInBounds(index);

            // size of used array elements decreased by 1
            Contract.Ensures(Contract.OldValue(Length) - 1 == Length);

            Contract.Ensures(
                (Length <= Contract.OldValue<int>(AllocatedLength) / 4 &&
                    Contract.OldValue<int>(AllocatedLength) / 2 >= MIN_DATA_SIZE &&
                    Contract.OldValue<int>(AllocatedLength) / 2 == AllocatedLength) ||
                ((Length > Contract.OldValue<int>(AllocatedLength) / 4 ||
                    Contract.OldValue<int>(AllocatedLength) / 2 < MIN_DATA_SIZE) &&
                    Contract.OldValue<int>(AllocatedLength) == AllocatedLength));

            Length--;
            for (int i = index; i < Length; i++)
                this[i] = this[i + 1];

            if (Length <= AllocatedLength / 4 && AllocatedLength / 2 >= MIN_DATA_SIZE)
            {
                int[] tmp = data;
                data = new int[AllocatedLength / 2];
                Array.Copy(tmp, data, AllocatedLength);
            }
        }

        public void Clear()
        {
            Contract.Ensures(Length == MIN_SIZE);
            Contract.Ensures(AllocatedLength == MIN_DATA_SIZE);

            Length = MIN_SIZE;
            data = new int[MIN_DATA_SIZE];
        }

        public void RemoveAll(int val)
        {
            Ensures_LengthsUnchanged();
            
            //int pushLeft = 0;
            //int idx = 0;
            //while (idx + pushLeft < Length)
            //{
            //    Contract.Assert(idx >= pushLeft);

            //    if (data[idx] == val)
            //        pushLeft++;
            //    else
            //        data[idx - pushLeft] = data[idx];
            //    idx++;
            //}
            int right = 0;
            int left = 0;
            while (right < Length)
            {
                Contract.Assert(left <= right);

                if (data[right] == val)
                {
                    right++;
                }
                else
                {
                    data[left] = data[right];
                    left++;
                    right++;
                }
                
            }

            Length = left;

            // all values has been removed
            Contract.Assume(Contract.ForAll(0, Length, i => Get(i) != val));
        }

        public void Sort()
        {
            Ensures_LengthsUnchanged();

            Array.Sort(data);

            // data are really sorted
            Contract.Assume(Length == 0 || Contract.ForAll(0, Length - 1, i => Get(i) <= Get(i + 1)));

            //Contract.Assert(Length == 0 || Contract.ForAll(0, Length - 1, i => Get(i) <= Get(i + 1)));
        }

        [Pure]
        public int GetHigher(int val)
        {
            // there is no higher value than int.MaxValue
            Contract.Requires(val < int.MaxValue, "There is no higher value than int.MaxValue");
            // there exists an item that is higher value than given "val"
            Contract.Requires(Length > 0 && Contract.Exists(0, Length, i => val < this[i]));
            Contract.Ensures(Contract.Result<int>() > int.MinValue);
            // returned value is higher than "val" ...
            //Contract.Ensures(Contract.Result<int>() > val);
            // ... and it is minimal such value
            //Contract.Ensures(Contract.ForAll(0, UsedSize, i => Get(i) <= val || Get(i) >= Contract.Result<int>()));

            int res = data.Where((v, i) => i < Length && v > val).OrderBy(v => v).First();

            // returned value is higher than "val" ...
            Contract.Assume(res > val);
            // ... and it is minimal such value
            Contract.Assume(Contract.ForAll(0, Length, i => this[i] <= val || this[i] >= res));

            return res;
        }

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
        //    Contract.Requires(Contract.Exists(0, Length, i => val < this[i]));
        //    // all returned values are higher than "val"
        //    Contract.Ensures(Contract.ForAll(Contract.Result<int[]>(), v => v > val));
        //    // result is non-empty
        //    Contract.Ensures(Contract.Result<int[]>().Any());

        //    return data.Where((v, i) => i < Length && v > val).ToArray();
        //}
        //public int Min(int[] values)
        //{
        //    Contract.Requires(values.Any());
        //    Contract.Ensures(Contract.ForAll(values, v => v >= Contract.Result<int>()));
        //    Contract.Ensures(values.Contains(Contract.Result<int>()));

        //    return 
        //}

       
        
    }
}
