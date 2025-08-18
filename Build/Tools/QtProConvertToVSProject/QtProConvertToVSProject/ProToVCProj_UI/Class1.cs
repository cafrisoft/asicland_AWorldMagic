using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace QtProConvertToVSProject.ProToVCProj_UI
{
    internal class Class1
    {
        #region FIELD
        private const string _ProPathFilename = "C:\\Asicland\\github\\AWorldMagic\\BaseLayOut.pro";
        #endregion

        public Class1()
        {
            string[] sentense = this.Get_UI_Sentense();

            foreach (string s in sentense)
            {
                Debug.WriteLine(s);
            }
        }

        string[] Get_UI_Sentense()
        {
            List<string> strings = new List<string>();

            try
            {
                if (!File.Exists(_ProPathFilename))
                {
                    Debug.WriteLine($"File not found: {_ProPathFilename}");
                    throw new InvalidOperationException();
                }

                string content = File.ReadAllText(_ProPathFilename);
                // Split by lines instead of sentences for .pro files, as they contain file lists and config
                string[] lines = content.Split(new char[] { '\n', '\r' }, StringSplitOptions.RemoveEmptyEntries);
                
                int uiSentenceCount = 0;
                List<string> uiSentences = new List<string>();

                foreach (string line in lines)
                {
                    // Trim leading and trailing spaces to handle lines like "   UI/DockWidgetHandler.cpp"
                    string trimmedLine = line.Trim();
                    if (trimmedLine.StartsWith("UI", StringComparison.OrdinalIgnoreCase))
                    {
                        // Clean up trailing backslash and quote characters, then remove any remaining trailing spaces
                        string cleanLine = trimmedLine.TrimEnd('\\', '"').TrimEnd();
                        
                        uiSentences.Add(cleanLine);
                        uiSentenceCount++;
                        //Debug.WriteLine($"UI Line {uiSentenceCount}: {cleanLine}");

                        strings.Add(cleanLine);
                    }
                }

                //Debug.WriteLine($"\nTotal number of lines starting with 'UI': {uiSentenceCount}");
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"Error reading file: {ex.Message}");
            }

            return strings.ToArray();
        }
    }
}
