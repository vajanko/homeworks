using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeContracts
{
    class Huffman
    {
        public static char GetChar(int val)
        {
            return (char)(val & 0xff);
        }
        public static int GetCount(int val)
        {
            return val >> 8;
        }
        public static int CreateItem(char ch, int count)
        {
            return (count << 8) | (int)ch;
        }

        public static Histogram ReadHistogram(string filename)
        {
            IntCollection hist = new IntCollection();
            // initialize empty tables
            for (char ch = (char)0; ch < 256; ch++)
            {
                // zero occurances of each character
                hist.Add(CreateItem(ch, 0));
            }

            IntCollection text = new IntCollection();
            char[] buff = new char[4];

            using (var reader = File.OpenText(filename))
            {
                while (!reader.EndOfStream)
                {
                    // read a peace of text and put it to the IntCollection buffer
                    int readLength = reader.Read(buff, 0, buff.Length);
                    for (int i = 0; i < readLength; i++)
                        text.Add((int)buff[i]);

                    // put same characters together so that thay can be counted easily
                    text.Sort();

                    while (text.Size() > 0)
                    {
                        int firstChar = text.Get(0);

                        // count occurance of the first character
                        int count = 0;
                        while (count < text.Size() && text.Get(count) == firstChar)
                            count++;

                        // get number of occurances of the first character processed in the previous buffer
                        for (int i = 0; i < hist.Size(); i++)
                        {
                            int item = hist.Get(i);
                            if (GetChar(item) == firstChar)
                            {
                                count += GetCount(item);
                                hist.Remove(i);
                                break;
                            }
                        }

                        // in this container we can not update already inserted value
                        // therefore we need to remove previously added value and add a new one at the end
                        hist.Add(CreateItem((char)firstChar, count));

                        // remove processed character from the text buffer (there can be multiple)
                        text.RemoveAll(firstChar);
                    }

                    // this isn't necessary as text buffer must be empty already
                    text.Clear();
                }
            }

            // remove all characters without occurance
            //int idx = 0;
            //while (idx < hist.Size())
            //{
            //    int item = hist.Get(idx);
            //    if (GetCount(item) == 0)
            //    {
            //        hist.RemoveAll(item);
            //    }
            //    else
            //    {
            //        idx++;
            //    }
            //}

            return new Histogram(hist);
        }
    }
}
