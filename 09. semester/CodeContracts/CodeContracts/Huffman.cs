using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeContracts
{
    /// <summary>
    /// Huffman encoding tools
    /// </summary>
    [ContractVerification(false)]
    class Huffman
    {
        /// <summary>
        /// Get last 8 bits from given integer number as char
        /// </summary>
        public static char GetChar(int val)
        {
            return (char)(val & 0xff);
        }
        /// <summary>
        /// Get first 24 bits from given integer number as int
        /// </summary>
        public static int GetCount(int val)
        {
            return val >> 8;
        }
        /// <summary>
        /// Create single integer storing 24 bit integer and 8 bit char value
        /// </summary>
        /// <param name="ch">char value to be combined with int</param>
        /// <param name="count">int value to be combined with char</param>
        public static int CreateItem(char ch, int count)
        {
            return (count << 8) | (int)ch;
        }
        /// <summary>
        /// Read given text file and creat a historgram useful for huffman encoding
        /// </summary>
        /// <param name="filename">Full path to the file</param>
        /// <returns>Histogram of given text file</returns>
        public static Histogram CreateHistogram(string filename)
        {
            // collection of characters and its occurences stored in a single int
            IntArray hist = new IntArray();
            // initialize empty tables
            for (char ch = (char)0; ch < 256; ch++)
                hist.Add(CreateItem(ch, 0));    // zero occurences of each character

            // collection of character from the input file stored as integers
            // that is because we only have int collection
            IntArray text = new IntArray();
            char[] buff = new char[4];

            using (var reader = File.OpenText(filename))
            {
                while (!reader.EndOfStream)
                {
                    // read a peace of text and put it to the IntArray buffer
                    int readLength = reader.Read(buff, 0, buff.Length);
                    for (int i = 0; i < readLength; i++)
                        text.Add((int)buff[i]);

                    // put same characters together so that we can count them easily
                    text.Sort();

                    // now "text" works as a queue, characters are processed and removed from the begining
                    while (text.Size() > 0)
                    {
                        int firstChar = text.Get(0);

                        // count occurence of the first character
                        int count = 0;
                        while (count < text.Size() && text.Get(count) == firstChar)
                            count++;

                        // iterrate over the hist collection and find current number of occurences of firstChar
                        // notice that hist is initialized with zero for each character
                        for (int i = 0; i < hist.Size(); i++)
                        {
                            // this is very inefficient but we don't have any "IndexOf" method

                            int item = hist.Get(i);
                            if (GetChar(item) == firstChar)
                            {
                                count += GetCount(item);    // total number of firstChar occurences
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

            return new Histogram(hist);
        }
    }
}
