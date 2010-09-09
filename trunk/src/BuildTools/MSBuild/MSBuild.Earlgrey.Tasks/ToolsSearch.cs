using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace MSBuild.Earlgrey.Tasks
{
    internal static class ToolsSearch
    {
        public static string FindExternalTool(string toolName)
        {
            try
            {
                return FileSearch.FindFirst(toolName);
            }
            catch (FileNotFoundException)
            {
                Uri currentAssemblyUri = new Uri(System.Reflection.Assembly.GetExecutingAssembly().CodeBase);
                string currentAssemblyPath = currentAssemblyUri.LocalPath;
                string baseDirectory = Directory.GetParent(currentAssemblyPath).FullName;
                string externalToolsRoot = Path.Combine(baseDirectory, @"ExternalTools\");

                return FileSearch.FindFirst(externalToolsRoot, toolName);
            }            
        }
    }
}
