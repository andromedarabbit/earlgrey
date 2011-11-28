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
    /// <summary>
    /// Using Windows command forfiles.exe, selects a file (or set of files) and executes a command on that file. This is helpful for batch jobs.
    /// </summary>
    /// <example> 
    /// <code title="In this next example the forfiles command would list any file that is older than 30 days." lang="xml" source=".\Samples\msbuild-forfiles-order-than-30-days.xml"/>
    /// <code title="Next, this command will remove the extensions of all files that end with .zip." lang="xml" source=".\Samples\msbuild-forfiles-remote-file-extension-zip.xml" />
    /// </example>
    /// <remarks></remarks>
    public class ForFiles : AbstractToolTask
    {
        /// <summary>
        /// Gets or sets the name of the path to start searching. The default folder is the current working directory (.). 
        /// </summary>
        /// <value>The name of the path.</value>
        /// <remarks>/P switch</remarks>
        public string PathName { get; set; }

        /// <summary>
        /// Gets or sets the search mask. Files will be searched according to a searchmask. The default searchmask is '*'.
        /// </summary>
        /// <value>The search mask.</value>
        /// <remarks>/M switch</remarks>
        public string SearchMask { get; set; }

        /// <summary>
        /// Gets or sets the command. Indicates the command to execute for each file. Command strings should be wrapped in double quotes.
        /// 
        /// The default command is "cmd /c echo @file".
        /// 
        /// The following variables can be used in the command string:
        /// <list type="bullet">
        /// <item>
        /// <description>@file - returns the name of the file.</description>
        /// </item>
        /// <item>
        /// <description>@fname - returns the file name without extension</description>
        /// </item>
        /// <item>
        /// <description>@ext - returns only the extension of the file.</description>
        /// </item>
        /// <item>
        /// <description>@path - returns the full path of the file.</description>
        /// </item>
        /// <item>
        /// <description>@relpath - returns the relative path of the file.</description>
        /// </item>
        /// <item>
        /// <description>@isdir - returns "TRUE" if a file type is a directory, and "FALSE" for files.</description>
        /// </item>
        /// <item>
        /// <description>@fsize - returns the size of the file in bytes</description>
        /// </item>
        /// <item>
        /// <description>@fdate - returns the last modified date of the file.</description>
        /// </item>
        /// <item>
        /// <description>@ftime - returns the last modified time of the file.</description>
        /// </item>
        /// </list>
        /// 
        /// To include special characters in the command line, use the hexadecimal code for the character in 0xHH format (ex. 0x09 for tab). Internal CMD.exe commands should be preceded with "cmd /c".
        /// </summary>
        /// <value>The command.</value>
        /// <remarks>/C switch</remarks>
        public string Command { get; set; }

        /// <summary>
        /// Selects files with a last modified date greater than or equal to (+), or less than or equal to (-), the specified date using the "MM/dd/yyyy" format; or selects files with a last modified date greater than or equal to (+) the current date plus "dd" days, or less than or equal to (-) the current date minus "dd" days. A valid "dd" number of days can be any number in the range of 0 - 32768. "+" is taken as default sign if not specified.
        /// </summary>
        /// <value>Gets or sets the date. </value>
        /// <remarks>/D switch</remarks>
        public string Date { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether this <see cref="ForFiles"/> is to recurse into subdirectories. Like "DIR /S".
        /// </summary>
        /// <value><c>true</c> if recursive; otherwise, <c>false</c>.</value>
        /// <remarks>/S switch</remarks>
        public bool Recursive { get; set; }

        /// <summary>
        /// [Output] Gets the files found.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] FilesFound
        {
            get { return _filesFound.ToArray(); }
        }


        private readonly List<TaskItem> _filesFound;

        /// <summary>
        /// Initializes a new instance of the <see cref="T:System.Object"/> class.
        /// </summary>
        /// <remarks></remarks>
        public ForFiles()
            : base()
        {
            _filesFound = new List<TaskItem>();

            PathName = TaskUtility.ThisAssemblyDirectory;
            // SearchMask = "*";
            Recursive = false;
        }

        /// <inheritdoc />
        protected override string ToolName
        {
            get { return "ForFiles.exe"; }
        }

        /// <inheritdoc />
        protected override string GenerateCommandLineCommands()
        {
            if (FileVersion.FileMajorPart == 1)
                return GenerateCommandLineCommandsForVersion1();

            if (FileVersion.FileMajorPart == 6)
                return GenerateCommandLineCommandsForVersion6();

            Log.LogWarning("Unknown version {0} of '{1}' is being used.", FileVersion.ToString(), GenerateFullPathToTool());
            return GenerateCommandLineCommandsForVersion6();
        }

        /// <summary>
        /// Generates the command line commands for version1.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
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

        /// <summary>
        /// Generates the command line commands for version6.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
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

        /// <summary>
        /// Gets the file version.
        /// </summary>
        /// <remarks></remarks>
        private FileVersionInfo FileVersion
        {
            get
            {
                string fullPathToTool = GenerateFullPathToTool();
                return FileVersionInfo.GetVersionInfo(fullPathToTool);
            }
        }

        /// <inheritdoc />
        protected override string GenerateFullPathToTool()
        {
            string fullPathToTool = base.GenerateFullPathToTool();

            if (FileSearch.Exists(fullPathToTool))
                return fullPathToTool;

            return ToolsSearch.FindExternalTool(ToolName);
        }

        /// <inheritdoc />
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
