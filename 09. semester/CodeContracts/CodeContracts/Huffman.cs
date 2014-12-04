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
        public static void Histogram(string filename)
        {
            // initialize empty table of characters and zero counts
            // this is necessary because of the GetHigher method
            // notice that there is not characted with value 256
            IntCollection counts = new IntCollection();
            IntCollection chars = new IntCollection();
            for (char ch = (char)0; ch < 256; ch++)
            {
                chars.Add((int)ch);
                counts.Add(0);
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
                        //int nextChar = text.GetHigher(firstChar);
                        //chars.GetHigher(firstChar);

                        // get number of occurances of the first character processed in the previous buffer
                        for (int i = 0; i < chars.Size(); i++)
                        {
                            if (chars.Get(i) == firstChar)
                            {
                                count += counts.Get(i);
                                chars.Remove(i);
                                counts.Remove(i);

                                break;
                            }
                        }

                        // in this container we can not update already inserted value
                        // therefore we need to remove previously added value and add a new one at the end
                        chars.Add(firstChar);
                        counts.Add(count);

                        // remove processed character from the text buffer
                        text.RemoveAll(firstChar);
                    }

                    // this isn't necessary as text buffer must be empty already
                    text.Clear();
                }
            }

            for (int i = 0; i < chars.Size(); i++)
            {
                if (counts.Get(i) > 0)
                    Console.WriteLine("{0}: {1}", (char)chars.Get(i), counts.Get(i));
            }
        }
    }
}
