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
    /// 
    /// </summary>
    /// <example>
    /// <code title="" lang="xml" source=".\Samples\msbuild-where-cmd.exe.xml" />
    /// </example>        
    /// <remarks></remarks>
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

        /// <summary>
        /// Validates the parameters.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        protected override bool ValidateParameters()
        {
            if (string.IsNullOrEmpty(FileName))
            {
                Log.LogError("FileName is empty!");
                return false;
            }
            return true;
        }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        protected override bool ExecuteCommand()
        {
            IEnumerable<string> pathsFound = FileSearch.FindAll(FileName).Distinct(StringComparer.CurrentCultureIgnoreCase);
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
