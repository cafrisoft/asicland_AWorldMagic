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
        private const string _VS2022ProjectPathFilename = "C:\\Asicland\\github\\AWorldMagic\\Build\\VS2022\\UI\\UI.vcxproj";
        private const string _VS2022ProjectFilterPathFilename = "C:\\Asicland\\github\\AWorldMagic\\Build\\VS2022\\UI\\UI.vcxproj.filters";
        #endregion

        public Class1()
        {
            string[] sentense = this.Get_UI_Sentense();

            foreach (string s in sentense)
            {
                Debug.WriteLine(s);
            }

            // Generate VS2022 project file
            this.Generte_VS2022Project(sentense);
            
            // Generate filter files
            this.Generate_FilterFiles(sentense);
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

        void Generte_VS2022Project(string[] strings)
        {
            try
            {
                if (!File.Exists(_VS2022ProjectPathFilename))
                {
                    Debug.WriteLine($"VS2022 project file not found: {_VS2022ProjectPathFilename}");
                    throw new InvalidOperationException();
                }

                // Read the existing project file
                string[] projectLines = File.ReadAllLines(_VS2022ProjectPathFilename);
                
                // First, remove any existing duplicates from the project file (both ClCompile and ClInclude)
                List<string> cleanedProjectLines = RemoveAllDuplicateItems(projectLines);
                
                // Then, remove ALL existing UI-related files to start fresh
                cleanedProjectLines = RemoveAllUIFiles(cleanedProjectLines);
                
                // Find insertion points for both ClCompile and ClInclude sections
                int clCompileInsertIndex = -1;
                int clIncludeInsertIndex = -1;
                
                for (int i = 0; i < cleanedProjectLines.Count; i++)
                {
                    if (cleanedProjectLines[i].Contains("<ClCompile Include="))
                    {
                        clCompileInsertIndex = i;
                    }
                    else if (cleanedProjectLines[i].Contains("<ClInclude Include="))
                    {
                        clIncludeInsertIndex = i;
                    }
                }

                if (clCompileInsertIndex == -1)
                {
                    Debug.WriteLine("Could not find ClCompile section in project file");
                    throw new InvalidOperationException();
                }

                // Get all existing file paths after cleaning
                HashSet<string> existingPaths = GetExistingFilePaths(cleanedProjectLines.ToArray());
                
                // Process each UI string and insert in the appropriate section
                foreach (string uiFile in strings)
                {
                    // Convert UI path to relative path format for VS2022 project
                    string relativePath = $"..\\..\\..\\{uiFile}";
                    string normalizedPath = relativePath.Replace("\\", "/");
                    
                    // Check if this file path already exists in the project
                    if (!existingPaths.Contains(normalizedPath))
                    {
                        // Determine file type and add to appropriate section
                        if (uiFile.EndsWith(".cpp", StringComparison.OrdinalIgnoreCase))
                        {
                            // Add to ClCompile section
                            string clCompileLine = $"    <ClCompile Include=\"{relativePath}\" />";
                            cleanedProjectLines.Insert(clCompileInsertIndex + 1, clCompileLine);
                            clCompileInsertIndex++;
                            Debug.WriteLine($"Added to ClCompile: {clCompileLine}");
                        }
                        else if (uiFile.EndsWith(".h", StringComparison.OrdinalIgnoreCase))
                        {
                            // Add to ClInclude section
                            string clIncludeLine = $"    <ClInclude Include=\"{relativePath}\" />";
                            
                            // If ClInclude section doesn't exist, create it
                            if (clIncludeInsertIndex == -1)
                            {
                                // Find where to insert ClInclude section (usually after ClCompile)
                                int insertAfterClCompile = -1;
                                for (int i = clCompileInsertIndex; i < cleanedProjectLines.Count; i++)
                                {
                                    if (cleanedProjectLines[i].Contains("</ItemGroup>"))
                                    {
                                        insertAfterClCompile = i;
                                        break;
                                    }
                                }
                                
                                if (insertAfterClCompile != -1)
                                {
                                    // Insert ClInclude section
                                    cleanedProjectLines.Insert(insertAfterClCompile + 1, "  <ItemGroup>");
                                    cleanedProjectLines.Insert(insertAfterClCompile + 2, clIncludeLine);
                                    cleanedProjectLines.Insert(insertAfterClCompile + 3, "  </ItemGroup>");
                                    clIncludeInsertIndex = insertAfterClCompile + 2;
                                }
                            }
                            else
                            {
                                cleanedProjectLines.Insert(clIncludeInsertIndex + 1, clIncludeLine);
                                clIncludeInsertIndex++;
                            }
                            
                            Debug.WriteLine($"Added to ClInclude: {clIncludeLine}");
                        }
                        
                        // Add to existing paths to avoid duplicates within the same run
                        existingPaths.Add(normalizedPath);
                    }
                    else
                    {
                        Debug.WriteLine($"Already exists, skipping: {uiFile}");
                    }
                }

                // Log the final state for debugging
                LogProjectFileState(cleanedProjectLines);
                
                // Write the updated project file
                File.WriteAllLines(_VS2022ProjectPathFilename, cleanedProjectLines.ToArray());
                Debug.WriteLine($"VS2022 project file updated successfully: {_VS2022ProjectPathFilename}");
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"Error generating VS2022 project: {ex.Message}");
            }
        }


        void Generate_FilterFiles(string[] strings)
        {
            try
            {
                if (!File.Exists(_VS2022ProjectFilterPathFilename))
                {
                    Debug.WriteLine($"VS2022 project filter file not found: {_VS2022ProjectFilterPathFilename}");
                    return;
                }

                // Read the existing filter file
                string[] filterLines = File.ReadAllLines(_VS2022ProjectFilterPathFilename);
                
                // First, remove any existing duplicates from the filter file
                List<string> cleanedFilterLines = RemoveAllDuplicateItems(filterLines);
                
                // Find insertion points for both ClCompile and ClInclude sections BEFORE removing UI files
                int clCompileInsertIndex = -1;
                int clIncludeInsertIndex = -1;
                
                for (int i = 0; i < cleanedFilterLines.Count; i++)
                {
                    if (cleanedFilterLines[i].Contains("<ClCompile Include="))
                    {
                        clCompileInsertIndex = i;
                    }
                    else if (cleanedFilterLines[i].Contains("<ClInclude Include="))
                    {
                        clIncludeInsertIndex = i;
                    }
                }

                if (clCompileInsertIndex == -1)
                {
                    Debug.WriteLine("Could not find ClCompile section in filter file");
                    return;
                }

                // Then, remove ALL existing UI-related files to start fresh
                cleanedFilterLines = RemoveAllUIFiles(cleanedFilterLines);
                
                // After removing UI files, we need to find a good insertion point
                // Look for the end of the ItemGroup that contains ClCompile items
                int insertPoint = -1;
                for (int i = 0; i < cleanedFilterLines.Count; i++)
                {
                    if (cleanedFilterLines[i].Contains("</ItemGroup>"))
                    {
                        // Check if this ItemGroup contains ClCompile items
                        for (int j = i; j >= 0; j--)
                        {
                            if (cleanedFilterLines[j].Contains("<ItemGroup>"))
                            {
                                // Found the start of this ItemGroup, check if it's the ClCompile one
                                bool hasClCompile = false;
                                for (int k = j; k <= i; k++)
                                {
                                    if (cleanedFilterLines[k].Contains("<ClCompile"))
                                    {
                                        hasClCompile = true;
                                        break;
                                    }
                                }
                                if (hasClCompile)
                                {
                                    insertPoint = i; // End of ClCompile ItemGroup
                                    break;
                                }
                            }
                        }
                        if (insertPoint != -1) break;
                    }
                }
                
                if (insertPoint == -1)
                {
                    Debug.WriteLine("Could not find insertion point in filter file");
                    return;
                }
                
                // Update insertion indices to point to the correct location
                clCompileInsertIndex = insertPoint;
                clIncludeInsertIndex = insertPoint;

                // Get all existing file paths after cleaning
                HashSet<string> existingPaths = GetExistingFilePaths(cleanedFilterLines.ToArray());
                
                // Create proper ItemGroup structure for all UI files
                CreateProperFilterStructure(cleanedFilterLines, strings);

                // Create the complete filter structure for UI files
                CreateFilterStructure(cleanedFilterLines, strings);

                // Log the final state for debugging
                LogFilterFileState(cleanedFilterLines);
                
                // Write the updated filter file
                File.WriteAllLines(_VS2022ProjectFilterPathFilename, cleanedFilterLines.ToArray());
                Debug.WriteLine($"VS2022 project filter file updated successfully: {_VS2022ProjectFilterPathFilename}");
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"Error generating filter files: {ex.Message}");
            }
        }

        // Helper method to extract filter path from UI file path
        private string GetFilterPath(string uiFilePath)
        {
            // Keep the "UI/" prefix and get the directory path
            if (uiFilePath.StartsWith("UI/", StringComparison.OrdinalIgnoreCase))
            {
                // Get the directory path (remove filename but keep UI/ prefix)
                int lastSlashIndex = uiFilePath.LastIndexOf('/');
                if (lastSlashIndex > 3) // More than just "UI/"
                {
                    string filterPath = uiFilePath.Substring(0, lastSlashIndex);
                    // Convert forward slashes to backslashes for Windows compatibility
                    string windowsFilterPath = filterPath.Replace('/', '\\');
                    Debug.WriteLine($"File: {uiFilePath} -> Filter: {filterPath} -> Windows: {windowsFilterPath}");
                    return windowsFilterPath;
                }
                else
                {
                    // File is directly under UI/ (no subdirectory)
                    Debug.WriteLine($"File: {uiFilePath} -> Filter: UI");
                    return "UI";
                }
            }
            Debug.WriteLine($"File: {uiFilePath} -> Filter: UI (default)");
            return "UI"; // Default filter if no subdirectory
        }

        // Helper method to log the current state of the filter file for debugging
        private void LogFilterFileState(List<string> filterLines)
        {
            Debug.WriteLine("=== Current Filter File State ===");
            int clCompileCount = 0;
            int clIncludeCount = 0;
            int filterCount = 0;
            int otherCount = 0;
            
            foreach (string line in filterLines)
            {
                if (line.Contains("<ClCompile Include="))
                {
                    clCompileCount++;
                    Debug.WriteLine($"ClCompile: {line.Trim()}");
                }
                else if (line.Contains("<ClInclude Include="))
                {
                    clIncludeCount++;
                    Debug.WriteLine($"ClInclude: {line.Trim()}");
                }
                else if (line.Contains("<Filter Include="))
                {
                    filterCount++;
                    Debug.WriteLine($"Filter: {line.Trim()}");
                }
                else if (line.Contains("Include="))
                {
                    otherCount++;
                    Debug.WriteLine($"Other: {line.Trim()}");
                }
            }
            
            Debug.WriteLine($"=== Summary: ClCompile={clCompileCount}, ClInclude={clIncludeCount}, Filter={filterCount}, Other={otherCount} ===");
        }

        // Helper method to log the current state of the project file for debugging
        private void LogProjectFileState(List<string> projectLines)
        {
            Debug.WriteLine("=== Current Project File State ===");
            int clCompileCount = 0;
            int clIncludeCount = 0;
            int qtMocCount = 0;
            int otherCount = 0;
            
            foreach (string line in projectLines)
            {
                if (line.Contains("<ClCompile Include="))
                {
                    clCompileCount++;
                    Debug.WriteLine($"ClCompile: {line.Trim()}");
                }
                else if (line.Contains("<ClInclude Include="))
                {
                    clIncludeCount++;
                    Debug.WriteLine($"ClInclude: {line.Trim()}");
                }
                else if (line.Contains("<QtMoc Include="))
                {
                    qtMocCount++;
                    Debug.WriteLine($"QtMoc: {line.Trim()}");
                }
                else if (line.Contains("Include="))
                {
                    otherCount++;
                    Debug.WriteLine($"Other: {line.Trim()}");
                }
            }
            
            Debug.WriteLine($"=== Summary: ClCompile={clCompileCount}, ClInclude={clIncludeCount}, QtMoc={qtMocCount}, Other={otherCount} ===");
        }

        // Helper method to remove ALL existing UI-related files from the project
        private List<string> RemoveAllUIFiles(List<string> projectLines)
        {
            List<string> cleanedLines = new List<string>();
            
            foreach (string line in projectLines)
            {
                // Check if this line contains a UI-related file path
                if (line.Contains("Include=\"") && line.Contains("UI/"))
                {
                    Debug.WriteLine($"Removing existing UI file: {line}");
                    continue; // Skip this line
                }
                
                // Keep all other lines
                cleanedLines.Add(line);
            }
            
            Debug.WriteLine($"Removed all existing UI files from project");
            return cleanedLines;
        }

        // Helper method to remove ALL duplicate items regardless of item type
        private List<string> RemoveAllDuplicateItems(string[] projectLines)
        {
            List<string> cleanedLines = new List<string>();
            HashSet<string> seenPaths = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            
            for (int i = 0; i < projectLines.Length; i++)
            {
                string line = projectLines[i];
                
                // Check for any Include attribute (covers all item types: ClCompile, ClInclude, QtMoc, etc.)
                if (line.Contains("Include=\"") && (line.Contains("<") && line.Contains(">")))
                {
                    // Extract the file path from the Include attribute
                    int startIndex = line.IndexOf("Include=\"") + 9;
                    int endIndex = line.IndexOf("\"", startIndex);
                    if (startIndex > 8 && endIndex > startIndex)
                    {
                        string filePath = line.Substring(startIndex, endIndex - startIndex);
                        // Normalize path separators for comparison
                        string normalizedPath = filePath.Replace("\\", "/");
                        
                        if (!seenPaths.Contains(normalizedPath))
                        {
                            seenPaths.Add(normalizedPath);
                            cleanedLines.Add(line);
                            Debug.WriteLine($"Kept: {line}");
                        }
                        else
                        {
                            Debug.WriteLine($"Removed duplicate: {line}");
                        }
                    }
                    else
                    {
                        // If we can't parse the path, keep the line to be safe
                        cleanedLines.Add(line);
                    }
                }
                else
                {
                    // Keep all non-Include lines
                    cleanedLines.Add(line);
                }
            }
            
            Debug.WriteLine($"Cleaned project file: removed {projectLines.Length - cleanedLines.Count} duplicate lines");
            return cleanedLines;
        }

        // Helper method to remove duplicate ClCompile items from project file
        private List<string> RemoveDuplicateClCompileItems(string[] projectLines)
        {
            List<string> cleanedLines = new List<string>();
            HashSet<string> seenPaths = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            
            for (int i = 0; i < projectLines.Length; i++)
            {
                string line = projectLines[i];
                
                if (line.Contains("<ClCompile Include="))
                {
                    // Extract the file path from the Include attribute
                    int startIndex = line.IndexOf("Include=\"") + 9;
                    int endIndex = line.IndexOf("\"", startIndex);
                    if (startIndex > 8 && endIndex > startIndex)
                    {
                        string filePath = line.Substring(startIndex, endIndex - startIndex);
                        // Normalize path separators for comparison
                        string normalizedPath = filePath.Replace("\\", "/");
                        
                        if (!seenPaths.Contains(normalizedPath))
                        {
                            seenPaths.Add(normalizedPath);
                            cleanedLines.Add(line);
                            Debug.WriteLine($"Kept: {line}");
                        }
                        else
                        {
                            Debug.WriteLine($"Removed duplicate: {line}");
                        }
                    }
                    else
                    {
                        // If we can't parse the path, keep the line to be safe
                        cleanedLines.Add(line);
                    }
                }
                else
                {
                    // Keep all non-ClCompile lines
                    cleanedLines.Add(line);
                }
            }
            
            Debug.WriteLine($"Cleaned project file: removed {projectLines.Length - cleanedLines.Count} duplicate lines");
            return cleanedLines;
        }

        // Helper method to extract existing file paths from project file
        private HashSet<string> GetExistingFilePaths(string[] projectLines)
        {
            HashSet<string> existingPaths = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            
            foreach (string line in projectLines)
            {
                // Check for any Include attribute (covers all item types: ClCompile, ClInclude, QtMoc, etc.)
                if (line.Contains("Include=\"") && (line.Contains("<") && line.Contains(">")))
                {
                    // Extract the file path from the Include attribute
                    int startIndex = line.IndexOf("Include=\"") + 9;
                    int endIndex = line.IndexOf("\"", startIndex);
                    if (startIndex > 8 && endIndex > startIndex)
                    {
                        string filePath = line.Substring(startIndex, endIndex - startIndex);
                        // Normalize path separators for comparison
                        string normalizedPath = filePath.Replace("\\", "/");
                        existingPaths.Add(normalizedPath);
                        Debug.WriteLine($"Found existing path: {normalizedPath}");
                    }
                }
            }
            
            return existingPaths;
        }

        // Helper method to create proper ItemGroup structure for UI files
        private void CreateProperFilterStructure(List<string> filterLines, string[] uiFiles)
        {
            // Find where to insert new ItemGroups (after the last existing ItemGroup)
            int insertIndex = -1;
            for (int i = filterLines.Count - 1; i >= 0; i--)
            {
                if (filterLines[i].Contains("</ItemGroup>"))
                {
                    insertIndex = i + 1;
                    break;
                }
            }

            if (insertIndex == -1)
            {
                Debug.WriteLine("Could not find end of ItemGroup section in filter file");
                return;
            }

            // Separate files by type
            List<string> cppFiles = new List<string>();
            List<string> hFiles = new List<string>();

            foreach (string uiFile in uiFiles)
            {
                if (uiFile.EndsWith(".cpp", StringComparison.OrdinalIgnoreCase))
                {
                    cppFiles.Add(uiFile);
                }
                else if (uiFile.EndsWith(".h", StringComparison.OrdinalIgnoreCase))
                {
                    hFiles.Add(uiFile);
                }
            }

            // Create ClCompile ItemGroup if there are .cpp files
            if (cppFiles.Count > 0)
            {
                filterLines.Insert(insertIndex, "  <ItemGroup>");
                insertIndex++;
                
                foreach (string uiFile in cppFiles)
                {
                    // Convert forward slashes to backslashes for Windows paths
                    string windowsPath = uiFile.Replace('/', '\\');
                    string relativePath = $"..\\..\\..\\{windowsPath}";
                    string clCompileEntry = $"    <ClCompile Include=\"{relativePath}\">";
                    string filterEntry = $"      <Filter>{GetFilterPath(uiFile)}</Filter>";
                    string endEntry = "    </ClCompile>";
                    
                    filterLines.Insert(insertIndex, clCompileEntry);
                    filterLines.Insert(insertIndex + 1, filterEntry);
                    filterLines.Insert(insertIndex + 2, endEntry);
                    insertIndex += 3;
                    
                    Debug.WriteLine($"Added ClCompile entry: {uiFile} -> {relativePath}");
                }
                
                filterLines.Insert(insertIndex, "  </ItemGroup>");
                insertIndex++;
            }

            // Create ClInclude ItemGroup if there are .h files
            if (hFiles.Count > 0)
            {
                filterLines.Insert(insertIndex, "  <ItemGroup>");
                insertIndex++;
                
                foreach (string uiFile in hFiles)
                {
                    // Convert forward slashes to backslashes for Windows paths
                    string windowsPath = uiFile.Replace('/', '\\');
                    string relativePath = $"..\\..\\..\\{windowsPath}";
                    string clIncludeEntry = $"    <ClInclude Include=\"{relativePath}\">";
                    string filterEntry = $"      <Filter>{GetFilterPath(uiFile)}</Filter>";
                    string endEntry = "    </ClInclude>";
                    
                    filterLines.Insert(insertIndex, clIncludeEntry);
                    filterLines.Insert(insertIndex + 1, filterEntry);
                    filterLines.Insert(insertIndex + 2, endEntry);
                    insertIndex += 3;
                    
                    Debug.WriteLine($"Added ClInclude entry: {uiFile} -> {relativePath}");
                }
                
                filterLines.Insert(insertIndex, "  </ItemGroup>");
                insertIndex++;
            }
            
            Debug.WriteLine($"Created ItemGroups: {cppFiles.Count} .cpp files, {hFiles.Count} .h files");
        }

        // Helper method to create the complete filter structure for UI files
        private void CreateFilterStructure(List<string> filterLines, string[] uiFiles)
        {
            // Find where to insert Filter entries (usually after the last ItemGroup)
            int insertIndex = -1;
            for (int i = filterLines.Count - 1; i >= 0; i--)
            {
                if (filterLines[i].Contains("</ItemGroup>"))
                {
                    insertIndex = i + 1;
                    Debug.WriteLine($"Found ItemGroup end at line {i}, will insert at {insertIndex}");
                    break;
                }
            }

            if (insertIndex == -1)
            {
                // If no ItemGroup found, try to insert before the closing </Project> tag
                for (int i = filterLines.Count - 1; i >= 0; i--)
                {
                    if (filterLines[i].Contains("</Project>"))
                    {
                        insertIndex = i;
                        Debug.WriteLine($"No ItemGroup found, will insert before </Project> at line {i}");
                        break;
                    }
                }
            }

            if (insertIndex == -1)
            {
                Debug.WriteLine("Could not find insertion point for Filter entries");
                return;
            }

            // Collect all unique filter paths from UI files
            HashSet<string> allFilterPaths = new HashSet<string>();
            Debug.WriteLine("=== Collecting Filter Paths ===");
            foreach (string uiFile in uiFiles)
            {
                string filterPath = GetFilterPath(uiFile);
                if (!string.IsNullOrEmpty(filterPath))
                {
                    allFilterPaths.Add(filterPath);
                    Debug.WriteLine($"Added filter path: {filterPath} for file: {uiFile}");
                    
                    // Only add parent paths that are actually needed (have files in them)
                    string[] pathParts = filterPath.Split('\\');
                    string currentPath = "";
                    for (int i = 0; i < pathParts.Length - 1; i++) // Don't add the last part (file level)
                    {
                        if (i == 0)
                        {
                            currentPath = pathParts[i];
                        }
                        else
                        {
                            currentPath = currentPath + "\\" + pathParts[i];
                        }
                        allFilterPaths.Add(currentPath);
                        Debug.WriteLine($"  Added parent path: {currentPath}");
                    }
                }
            }

            // Check which filter entries already exist
            HashSet<string> existingFilters = new HashSet<string>();
            foreach (string line in filterLines)
            {
                if (line.Contains("<Filter Include="))
                {
                    int startIndex = line.IndexOf("Include=\"") + 9;
                    int endIndex = line.IndexOf("\"", startIndex);
                    if (startIndex > 8 && endIndex > startIndex)
                    {
                        string existingFilter = line.Substring(startIndex, endIndex - startIndex);
                        existingFilters.Add(existingFilter);
                    }
                }
            }

            // Add missing filter entries in hierarchical order (parent folders first)
            var sortedFilterPaths = allFilterPaths.OrderBy(p => p.Split('\\').Length).ThenBy(p => p).ToList();
            
            Debug.WriteLine($"=== Creating Filter Entries ===");
            Debug.WriteLine($"Insertion point: {insertIndex}");
            Debug.WriteLine($"Existing filters: {string.Join(", ", existingFilters)}");
            
            // Start ItemGroup for Filter entries
            filterLines.Insert(insertIndex, "  <ItemGroup>");
            insertIndex++;
            
            foreach (string filterPath in sortedFilterPaths)
            {
                if (!existingFilters.Contains(filterPath))
                {
                    string filterEntry = $"    <Filter Include=\"{filterPath}\">";
                    string uniqueIdEntry = $"      <UniqueIdentifier>{{{Guid.NewGuid()}}}</UniqueIdentifier>";
                    string endFilterEntry = "    </Filter>";
                    
                    Debug.WriteLine($"Inserting at position {insertIndex}:");
                    Debug.WriteLine($"  {filterEntry}");
                    Debug.WriteLine($"  {uniqueIdEntry}");
                    Debug.WriteLine($"  {endFilterEntry}");
                    
                    filterLines.Insert(insertIndex, filterEntry);
                    filterLines.Insert(insertIndex + 1, uniqueIdEntry);
                    filterLines.Insert(insertIndex + 2, endFilterEntry);
                    insertIndex += 3;
                    
                    Debug.WriteLine($"Added Filter entry: {filterPath}");
                }
                else
                {
                    Debug.WriteLine($"Filter already exists: {filterPath}");
                }
            }
            
            // End ItemGroup for Filter entries
            filterLines.Insert(insertIndex, "  </ItemGroup>");
            insertIndex++;
            
            Debug.WriteLine($"Total filter paths processed: {allFilterPaths.Count}");
            Debug.WriteLine($"Filter paths: {string.Join(", ", sortedFilterPaths)}");
            
            // Log the final state of filter entries
            Debug.WriteLine("=== Final Filter Entries in File ===");
            int filterCount = 0;
            for (int i = 0; i < filterLines.Count; i++)
            {
                if (filterLines[i].Contains("<Filter Include="))
                {
                    filterCount++;
                    Debug.WriteLine($"Line {i}: {filterLines[i].Trim()}");
                }
            }
            Debug.WriteLine($"Total Filter Include entries found: {filterCount}");
        }

        // Helper method to add Filter entries with UniqueIdentifier (legacy - no longer used)
        private void AddFilterEntries(List<string> filterLines)
        {
            Debug.WriteLine("AddFilterEntries method called (legacy method)");
            // This method is no longer used - CreateFilterStructure handles everything
        }

    }  // end of 'internal class Class1'

} // end of 'namespace QtProConvertToVSProject.ProToVCProj_UI'


