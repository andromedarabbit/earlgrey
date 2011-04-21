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
    public class Where : AbstractTask
    {
        private readonly List<ITaskItem> _pathsFound;

        public Where()
        {
            _pathsFound = new List<ITaskItem>();
        }

        [Required]
        public string FileName { get; set; }

        [Output]
        public ITaskItem[] PathsFound 
        { 
            get { return _pathsFound.ToArray(); }
        }
    
        protected override bool ValidateParameters()
        {
            if (string.IsNullOrEmpty(FileName))
            {
                Log.LogError("FileName is empty!");
                return false;
            }
            return true;
        }

        protected override bool ExecuteCommand()
        {
            IEnumerable<string> pathsFound = FileSearch.FindAll(FileName);
            foreach (var path in pathsFound)
            {
                Debug.Assert(string.IsNullOrEmpty(path) == false);
                _pathsFound.Add(new TaskItem(path));
            }

            return true;
        }
    }
}
