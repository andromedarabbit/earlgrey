using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using MSBuild.Community.Tasks.Services;

namespace MSBuild.Earlgrey.Tasks.IO
{
    /// <summary>
    /// An alternative version of Where.exe in Windows, but implemented and works in totally different way.
    /// </summary>
    /// <example>
    /// <code title="" lang="xml" source=".\Samples\msbuild-where-cmd.exe.xml" />
    /// </example>        
    /// <remarks>
    /// This searches the file you wand to find using <see cref="FileName"/> and let you know where you can find it. 
    /// Search paths are:
    /// <list type="bullet">
    /// <item>
    /// <description>Base directory.</description>
    /// </item>
    /// <item>
    /// <description>System directory (ex. C:\Windows\system32).</description>
    /// </item>
    /// <item>
    /// <description>Windows directory (ex. C:\Windows).</description>
    /// </item>
    /// <item>
    /// <description>All the paths described in the environment variable PATH.</description>
    /// </item>
    /// </list>
    /// </remarks>
    public class Where : AbstractTask
    {
        private readonly List<ITaskItem> _pathsFound;

        /// <summary>
        /// Initializes a new instance of the <see cref="T:System.Object"/> class.
        /// </summary>
        /// <remarks></remarks>
        public Where()
        {
            _pathsFound = new List<ITaskItem>();
        }

        /// <summary>
        /// [Required] Gets or sets the name of the file you want to find.
        /// </summary>
        /// <value>The name of the file.</value>
        /// <remarks></remarks>
        [Required]
        public string FileName { get; set; }

        /// <summary>
        /// [Output] Gets the paths found.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] PathsFound 
        { 
            get { return _pathsFound.ToArray(); }
        }

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            if (string.IsNullOrEmpty(FileName))
            {
                Log.LogError("FileName is empty!");
                return false;
            }
            return true;
        }

        /// <inheritdoc />
        protected override bool ExecuteCommand()
        {
            IEnumerable<string> pathsFound = FileSearch.FindAll(FileName);
            foreach (var path in pathsFound)
            {
                Debug.Assert(string.IsNullOrEmpty(path) == false);
               
                TaskItem newItem = new TaskItem(path);
                _pathsFound.Add(newItem);
            }

            return true;
        }
    }
}
