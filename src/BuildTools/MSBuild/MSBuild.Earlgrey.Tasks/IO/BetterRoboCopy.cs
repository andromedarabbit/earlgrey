using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Microsoft.Build.Utilities;
using Earlgrey;

namespace MSBuild.Earlgrey.Tasks.IO
{
    /// <summary>
    /// Task wrapping the Window Resource Kit Robocopy.exe command.
    /// </summary>
    /// <example>
    /// <code lang="xml" title="Deploy website to web server.">
    /// <![CDATA[
    /// <BetterRoboCopy 
    ///     SourceFolder="$(MSBuildProjectDirectory)" 
    ///     DestinationFolder="\\server\webroot\" 
    ///     Mirror="true"
    ///     ExcludeFolders=".svn;obj;Test"
    ///     ExcludeFiles="*.cs;*.resx;*.csproj;*.webinfo;*.log"
    ///     NoJobHeader="true"
    /// />  
    /// ]]>
    /// </code>
    /// <code lang="xml" title="Copy a source folder with all sub items into a desination one." source=".\Samples\msbuild-betterrobocopy.xml" />
    /// </example>
    /// <remarks>
    /// The difference between <see cref="MSBuild.Community.Tasks.RoboCopy"/> and <see cref="BetterRoboCopy"/> is the way <see cref="BetterRoboCopy"/> handles exit code. Following exit codes are handled as success.
    /// <list type="bullet">
    /// <item>
    /// <description>Return Code 0. No errors occurred, and no copying was done. The source and destination directory trees are completely synchronized.</description>
    /// </item>
    /// <item>
    /// <description>Return Code 1. One or more files were copied successfully (that is, new files have arrived).</description>
    /// </item>
    /// <item>
    /// <description>Return Code 2. Some Extra files or directories were detected. Examine the output log. Some housekeeping may be needed.</description>
    /// </item>
    /// <item>
    /// <description>Return Code 4. Some Mismatched files or directories were detected. Examine the output log. Housekeeping is probably necessary.</description>
    /// </item>
    /// <item>
    /// <description>Return Code 8. Some files or directories could not be copied (copy errors occurred and the retry limit was exceeded). Check these errors further.</description>
    /// </item>
    /// <item>
    /// <description>Return Code 16. Serious error. RoboCopy did not copy any files. This is either a usage error or an error due to insufficient access privileges on the source or destination directories.</description>
    /// </item>
    /// </list>
    /// </remarks>
    public class BetterRoboCopy : MSBuild.Community.Tasks.RoboCopy
    {
        /*  
          /MAXAGE:n :: 최대 파일 사용 기간 - n일/날짜보다 오래된 파일을 제외합니다.
          /MINAGE:n :: 최소 파일 사용 기간 - n일/날짜보다 최신 파일을 제외합니다.
          /MAXLAD:n :: 최대 마지막 액세스 날짜 - n 이후에 사용되지 않은 파일을 제외합니다.
          /MINLAD:n :: 최소 마지막 액세스 날짜 - n 이후에 사용된 파일을 제외합니다.
                       n이 1900보다 크면 n은 n일과 같고 그렇지 않으면 n은 YYYYMMDD 날짜입니다.
         * */

        private const string MAXAGE_SWITCH_NAME = "/MAXAGE:";
        private const string MINAGE_SWITCH_NAME = "/MINAGE:";
        private const string MAXLAD_SWITCH_NAME = "/MAXLAD:";
        private const string MINLAD_SWITCH_NAME = "/MINLAD:";

        private const string NUMBER_OF_RETRIES_SWITCH_NAME = "/R:";

        private string _maxAgeString;
        private string _minAgeString;
        private string _maxLadString;
        private string _minLadString;

        private int _numberOfRetries;

        /// <summary>
        /// MAXimum file AGE - exclude files older than n days/date.
        /// </summary>
        /// <value>Gets or sets the max age. </value>
        /// <remarks>/MAXAGE:n switch.</remarks>
        public string MaxAge
        {
            get { return _maxAgeString; }
            set { _maxAgeString = value; }
        }

        /// <summary>
        /// <![CDATA[
        /// MINimum file AGE - exclude files older than n days/date. 
        /// (If n < 1900 then n = no of days, else n = YYYYMMDD date).
        /// ]]>
        /// </summary>
        /// <value>Gets or sets the min age. </value>
        /// <remarks>/MINAGE:n switch.</remarks>
        public string MinAge
        {
            get { return _minAgeString; }
            set { _minAgeString = value; }
        }

        /// <summary>
        /// MAXimum Last Access Date - exclude files unused since n.
        /// </summary>
        /// <value>Gets or sets the max lad.</value>
        /// <remarks>/MAXLAD:n switch.</remarks>
        public string MaxLad
        {
            get { return _maxLadString; }
            set { _maxLadString = value; }
        }

        /// <summary>
        /// <![CDATA[
        /// MINimum Last Access Date - exclude files used since n.
        /// (If n < 1900 then n = n days, else n = YYYYMMDD date).
        /// ]]>
        /// </summary>
        /// <value>Gets or sets the max lad.</value>
        /// <remarks>/MINLAD:n switch.</remarks>
        public string MinLad
        {
            get { return _minLadString; }
            set { _minLadString = value; }
        }


        /// <summary>
        /// Number of Retries on failed copies - default is 1 million.
        /// </summary>
        /// <value>Gets or sets the number of retries.</value>
        /// <remarks>/R:n switch.</remarks>
        public int NumberOfRetries
        {
            get { return _numberOfRetries; }
            set { _numberOfRetries = value; }
        }


        /// <summary>
        /// Gets or sets a value indicating whether to exclude hidden files/folders. Default value is <c>true</c>.
        /// </summary>
        /// <value>If <c>true</c>, hidden files/folders will not be copied.</value>
        /// <remarks></remarks>
        public bool ExcludeHidden { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to exclude system files/folders. Default value is <c>true</c>.
        /// </summary>
        /// <value>If <c>true</c>, system files/folders will not be copied.</value>
        /// <remarks></remarks>
        /// 
        public bool ExcludeSystem { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to exclude temporary files/folders. Default value is <c>true</c>.
        /// </summary>
        /// <value>If <c>true</c>, temporary files/folders will not be copied.</value>
        /// <remarks></remarks>
        public bool ExcludeTemporary { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="BetterRoboCopy"/> class.
        /// </summary>
        /// <remarks></remarks>
        public BetterRoboCopy()
            : base()
        {
            this._numberOfRetries = -1;
            // base.ExcluedAttributes = "HS"; // Hidden, System
            this.ExcludeHidden = true;
            this.ExcludeSystem = true;
            this.ExcludeTemporary = true;
        }

        /// <inheritdoc />
        protected override string GenerateCommandLineCommands()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
            builder.AppendSwitchIfNotNull(MAXAGE_SWITCH_NAME, _maxAgeString);
            builder.AppendSwitchIfNotNull(MINAGE_SWITCH_NAME, _minAgeString);
            builder.AppendSwitchIfNotNull(MAXLAD_SWITCH_NAME, _maxLadString);
            builder.AppendSwitchIfNotNull(MINLAD_SWITCH_NAME, _minLadString);

            if (_numberOfRetries > -1)
            {
                builder.AppendSwitch(NUMBER_OF_RETRIES_SWITCH_NAME + _numberOfRetries);
            }

            if (string.IsNullOrEmpty(ExcluedAttributes))
            {
                ExcluedAttributes = string.Empty;
                if (ExcludeHidden)
                    ExcluedAttributes += "H";

                if (ExcludeSystem)
                    ExcluedAttributes += "S";

                if (ExcludeTemporary)
                    ExcluedAttributes += "T";
            }

            string commands = base.GenerateCommandLineCommands();
            return commands + " " + builder.ToString();
        }

        /// <inheritdoc />
        protected override string GenerateFullPathToTool()
        {
            string fullPathToTool = base.GenerateFullPathToTool();

            if (FileSearch.Exists(fullPathToTool))
                return fullPathToTool;

            return ToolsSearch.FindExternalTool(ToolName);
        }

        //! \ref http://ss64.com/nt/robocopy-exit.html
        [Flags]
        private enum EExitCode
        {
            NoErrorsNoFilesCopied = 0,
            FilesCopiedSuccessful = 1,
            ExtraFilesOrDirsDetected = 2,
            MismatchedFilesOrDirsDetected = 4,
            SomeFilesOrDirsNotCopied = 8,
            SeriousErrorOcurred = 16,
            All = NoErrorsNoFilesCopied
                | FilesCopiedSuccessful
                | ExtraFilesOrDirsDetected
                | MismatchedFilesOrDirsDetected
                | SomeFilesOrDirsNotCopied
                | SeriousErrorOcurred
        }

        //! \todo 사용자가 원한다면 일부 종료 코드를 오류 처리하게 하자.
        /// <inheritdoc />
        protected override int ExecuteTool(string pathToTool, string responseFileCommands, string commandLineCommands)
        {
            Log.LogMessage("Running " + pathToTool + " " + commandLineCommands);
            return HandleExitCode(
                base.ExecuteTool(pathToTool, responseFileCommands, commandLineCommands)
                );
        }

        private int HandleExitCode(int retValue)
        {
            EExitCode exitCode = (EExitCode)(retValue);
            if (exitCode != 0 && (EExitCode.All & exitCode) == 0) // 원래 All과 비트연산만 해보면 되지만 이 경우엔 종료코드 0도 성공취급해야 하므로 조건문이 복잡하다.
            {
                Log.LogError("Unknown exit code '{0}' returned!", retValue);
                return retValue;
            }

            if ((exitCode & EExitCode.NoErrorsNoFilesCopied) != 0)
                Log.LogMessage("Return Code 0. No errors occurred, and no copying was done. The source and destination directory trees are completely synchronized.");

            if ((exitCode & EExitCode.FilesCopiedSuccessful) != 0)
                Log.LogMessage("Return Code 1. One or more files were copied successfully (that is, new files have arrived).");

            if ((exitCode & EExitCode.ExtraFilesOrDirsDetected) != 0)
                Log.LogMessage("Return Code 2. Some Extra files or directories were detected. Examine the output log. Some housekeeping may be needed.");

            if ((exitCode & EExitCode.MismatchedFilesOrDirsDetected) != 0)
                Log.LogMessage("Return Code 4. Some Mismatched files or directories were detected. Examine the output log. Housekeeping is probably necessary.");

            if ((exitCode & EExitCode.SomeFilesOrDirsNotCopied) != 0)
                Log.LogError("Return Code 8. Some files or directories could not be copied (copy errors occurred and the retry limit was exceeded). Check these errors further.");

            if ((exitCode & EExitCode.SeriousErrorOcurred) != 0)
                Log.LogError("Return Code 16. Serious error. RoboCopy did not copy any files. This is either a usage error or an error due to insufficient access privileges on the source or destination directories.");

            if (exitCode < EExitCode.MismatchedFilesOrDirsDetected)
                return 0; // 성공

            return retValue;
        }
    }

}
