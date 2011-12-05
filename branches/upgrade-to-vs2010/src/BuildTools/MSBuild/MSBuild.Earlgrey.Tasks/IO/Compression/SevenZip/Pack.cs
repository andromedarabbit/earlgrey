using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.SevenZip
{
    /// <summary>
    /// Compress a folder and its sub-folders and sub-files by using 7-zip.
    /// </summary>
    /// <example>
    /// <code title="Compress a folder and unzip it into the other path." lang="xml" source=".\Samples\msbuild-zip-a-folder-using-7-zip.xml" />
    /// </example>
    /// <remarks></remarks>
    public class Pack : AbstractToolTask, IPack
    {
        private string _password;
        private ITaskItem _srcFolder;

        /// <summary>
        /// Gets or sets a value indicating whether [ignore warning].
        /// </summary>
        /// <value><c>true</c> if [ignore warning]; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool IgnoreWarning { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="T:System.Object"/> class.
        /// </summary>
        /// <remarks></remarks>
        public Pack()
        {
            Overwrite = false;
            ContainsRootDir = true;
            ZipLevel = FromCompressionLevel(CompressionLevel.Default);
            UseLargePages = true;
        }

        /// <summary>
        /// Indicates whether all task paratmeters are valid.
        /// </summary>
        /// <returns>true if all task parameters are valid; otherwise, false.</returns>
        /// <remarks></remarks>
        protected override bool ValidateParameters()
        {
            if (base.ValidateParameters() == false)
                return false;

            if (TaskItemHelper.FolderExists(SrcFolder) == false)
            {
                Log.LogError(
                    string.Format("Source folder path '{0}' not found!", SrcFolder.ItemSpec)
                    );
                return false;
            }

            if (TaskItemHelper.FileExists(ZipFilePath) == true && Overwrite == false)
            {
                Log.LogError(
                    string.Format("Destination file path '{0}' already exists!", ZipFilePath.ItemSpec)
                    );
                return false;
            }

            return true;
        }

        /// <summary>
        /// Returns a string value containing the command line arguments to pass directly to the executable file.
        /// </summary>
        /// <returns>A string value containing the command line arguments to pass directly to the executable file.</returns>
        /// <remarks></remarks>
        protected override string GenerateCommandLineCommands()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
            builder.AppendSwitch("a");

            builder.AppendFileNameIfNotNull(InternalZipFilePath);
            if (ContainsRootDir)
                builder.AppendFileNameIfNotNull(InternalSrcFolder);
            else
                builder.AppendFileNameIfNotNull(InternalSrcFolder + @"\*");

            builder.AppendSwitch(UseLargePagesSwitch);

            builder.AppendSwitch("-mx=" + ZipLevel);
            
            // 7z a archive.7z -psecret);
            if (string.IsNullOrEmpty(Password) == false)
            {                
                builder.AppendSwitch("-p" + Password);
            }

            if (HasExcludePattern())
            {
                builder.AppendSwitch("-r0");

                foreach (var exclude in Excludes)
                {
                    builder.AppendSwitch("-x!" + exclude);
                }
            }

            return builder.ToString();
        }

        /// <summary>
        /// Runs the exectuable file with the specified task parameters.
        /// </summary>
        /// <returns>true if the task runs successfully; otherwise, false.</returns>
        /// <remarks></remarks>
        public override bool Execute()
        {
            if (TaskItemHelper.FileExists(ZipFilePath) == true && Overwrite == true)
            {
                TaskItemHelper.FileDelete(ZipFilePath);
            }

            return base.Execute();
        }

        /// <summary>
        /// Creates a temporary response (.rsp) file and runs the executable file.
        /// </summary>
        /// <param name="pathToTool">The path to the executable file.</param>
        /// <param name="responseFileCommands">The command line arguments to place in the .rsp file.</param>
        /// <param name="commandLineCommands">The command line arguments to pass directly to the executable file.</param>
        /// <returns>The returned exit code of the executable file. If the task logged errors, but the executable returned an exit code of 0, this method returns -1.</returns>
        /// <remarks></remarks>
        protected override int ExecuteTool(string pathToTool, string responseFileCommands, string commandLineCommands)
        {
            Log.LogMessage("Running " + pathToTool + " " + commandLineCommands);
            return HandleExitCode(
                base.ExecuteTool(pathToTool, responseFileCommands, commandLineCommands)
                );
        }

        /// <summary>
        /// Handles the exit code.
        /// </summary>
        /// <param name="retValue">The ret value.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private int HandleExitCode(int retValue)
        {
            EExitCode exitCode = (EExitCode)(retValue);
            if (exitCode == EExitCode.WarningNonFatalError && IgnoreWarning == true)
            {
                Log.LogWarning(ToolName + " returned with an exit code " + retValue + ", but it was ignored.");
                return 0;
            }
            return retValue;
        }

        /// <summary>
        /// Gets the internal zip level.
        /// </summary>
        /// <remarks></remarks>
        private CompressionLevel InternalZipLevel
        {
            get { return ToCompressionLevel(ZipLevel); }
        }

        /// <summary>
        /// From the compression level.
        /// </summary>
        /// <param name="level">The level.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private static int FromCompressionLevel(CompressionLevel level)
        {
            return (int)level;
        }

        /// <summary>
        /// Toes the compression level.
        /// </summary>
        /// <param name="level">The level.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private static CompressionLevel ToCompressionLevel(int level)
        {
            return (CompressionLevel)level;
        }

        /// <summary>
        /// Gets the internal SRC folder.
        /// </summary>
        /// <remarks></remarks>
        private string InternalSrcFolder
        {
            get { return TaskItemHelper.GetFullPath(SrcFolder); }
        }

        /// <summary>
        /// Gets the internal zip file path.
        /// </summary>
        /// <remarks></remarks>
        private string InternalZipFilePath
        {
            get { return TaskItemHelper.GetFullPath(ZipFilePath); }
        }

        /// <summary>
        /// Gets the name of the executable file to run.
        /// </summary>
        /// <returns>
        /// The name of the executable file to run.
        ///   </returns>
        /// <remarks></remarks>
        protected override string ToolName
        {
            get { return @"7za.exe"; }
        }

        /// <summary>
        /// Determines whether [has exclude pattern].
        /// </summary>
        /// <returns><c>true</c> if [has exclude pattern]; otherwise, <c>false</c>.</returns>
        /// <remarks></remarks>
        private bool HasExcludePattern()
        {
            return Excludes != null && Excludes.Length > 0;
        }

        /// <inheritdoc />
        [Required]
        public virtual ITaskItem SrcFolder
        {
            get { return _srcFolder; } 
            set 
            { 
                value.ItemSpec = value.ItemSpec.TrimEnd('\\');
                _srcFolder = value;
            }
        }

        /// <inheritdoc />
        public virtual string[] Excludes { get; set; }

        /// <inheritdoc />
        [Required]
        public virtual ITaskItem ZipFilePath { get; set; }

        /// <inheritdoc />
        public virtual bool Overwrite { get; set; }

        /// <inheritdoc />
        public virtual int ZipLevel { get; set; }

        // TODO: 공백 못 들어가게 막자.
        /// <inheritdoc />
        public virtual string Password 
        { 
            get { return _password; }
            set 
            {
                _password = value.Trim();
            }
        }

        /// <inheritdoc />
        public virtual bool ContainsRootDir { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether [use large pages].
        /// </summary>
        /// <value><c>true</c> if [use large pages]; otherwise, <c>false</c>. </value>
        /// <remarks>Turn on -slp switch of 7za.exe. See 7-zip user guide if you want details.</remarks>
        public bool UseLargePages { get; set; }

        /// <summary>
        /// Gets the use large pages switch.
        /// </summary>
        /// <remarks></remarks>
        private string UseLargePagesSwitch
        {
            get
            {
                if (UseLargePages)
                    return "-slp";
                return "-slp-";
            }
        }
    }
}
