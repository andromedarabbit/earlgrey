using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO
{
    public class ForFiles : AbstractToolTask
    {
        public string PathName { get; set; }

        public string SearchMask { get; set; }

        public string Command { get; set; }

        public string Date { get; set; }

        public bool Recursive { get; set; }

        [Output]
        public ITaskItem[] FilesFound
        {
            get { return _filesFound.ToArray(); }
        }


        private readonly List<TaskItem> _filesFound;

        public ForFiles()
            : base()
        {
            _filesFound = new List<TaskItem>();

            PathName = TaskUtility.ThisAssemblyDirectory;
            // SearchMask = "*";
            Recursive = false;
        }

        protected override string ToolName
        {
            get { return "ForFiles.exe"; }
        }

        protected override bool ValidateParameters()
        {
            return base.ValidateParameters();
        }

        protected override string GenerateCommandLineCommands()
        {
            if (FileVersion.FileMajorPart == 1)
                return GenerateCommandLineCommandsForVersion1();

            if (FileVersion.FileMajorPart == 6)
                return GenerateCommandLineCommandsForVersion6();

            Log.LogWarning("Unknown version {0} of '{1}' is being used.", FileVersion.ToString(), GenerateFullPathToTool());
            return GenerateCommandLineCommandsForVersion6();
        }

        private string GenerateCommandLineCommandsForVersion1()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
            if (string.IsNullOrEmpty(PathName) == false)
                builder.AppendSwitchIfNotNull("-P", PathName);

            if (string.IsNullOrEmpty(SearchMask) == false)
                builder.AppendSwitchIfNotNull("-M", SearchMask);

            if (string.IsNullOrEmpty(Command) == false)
                builder.AppendSwitchIfNotNull("-C", Command);

            if (string.IsNullOrEmpty(Date) == false)
                builder.AppendSwitchIfNotNull("-D", Date);

            if (Recursive == true)
                builder.AppendSwitch("-S");

            string commands = base.GenerateCommandLineCommands();
            return commands + " " + builder.ToString();
        }

        private string GenerateCommandLineCommandsForVersion6()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
            if (string.IsNullOrEmpty(PathName) == false)
                builder.AppendSwitchIfNotNull("/P ", PathName);

            if (string.IsNullOrEmpty(SearchMask) == false)
                builder.AppendSwitchIfNotNull("/M ", SearchMask);

            if (string.IsNullOrEmpty(Command) == false)
                builder.AppendSwitchIfNotNull("/C ", Command);

            if (string.IsNullOrEmpty(Date) == false)
                builder.AppendSwitchIfNotNull("/D ", Date);

            if(Recursive == true)
                builder.AppendSwitch("/S ");

            string commands = base.GenerateCommandLineCommands();
            return commands + " " + builder.ToString();
        }

        private FileVersionInfo FileVersion
        {
            get
            {
                string fullPathToTool = GenerateFullPathToTool();
                return FileVersionInfo.GetVersionInfo(fullPathToTool);
            }
        }

        protected override string GenerateFullPathToTool()
        {
            string fullPathToTool = base.GenerateFullPathToTool();

            if (FileSearch.Exists(fullPathToTool))
                return fullPathToTool;

            return ToolsSearch.FindExternalTool(ToolName);
        }

        //public override bool Execute()
        //{
        //    return base.Execute();
        //}


        protected override void LogEventsFromTextOutput(string singleLine, MessageImportance messageImportance)
        {
            base.LogEventsFromTextOutput(singleLine, messageImportance);


            string unquotedLine = singleLine.Trim('\"');
            if(string.IsNullOrEmpty(unquotedLine))
                return;

            TaskItem taskItem = new TaskItem(unquotedLine);
            _filesFound.Add(taskItem);
        }

        
    }
}
