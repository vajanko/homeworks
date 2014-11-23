using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeContracts
{
    public class IntCollection
    {
        private List<int> data = new List<int>();

        public int Size()
        {
            return data.Count;
        }

        public void Add(int val)
        {
            data.Add(val);
        }
        public int Get(int index)
        {
            return data[index];
        }
        public int GetHigher(int val)
        {
            if (data.All(i => i <= val))
                return val;

            return data.First(i => i > val);
        }
        public void Remove(int index)
        {
            data.RemoveAt(index);
        }
        public void RemoveAll(int val)
        {
            data.RemoveAll(i => i == val);
        }
        public void Sort()
        {
            data.Sort();
        }
        public void Clear()
        {
            data.Clear();
        }
        
    }
}
