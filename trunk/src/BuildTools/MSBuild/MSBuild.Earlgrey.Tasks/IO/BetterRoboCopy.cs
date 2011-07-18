using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Microsoft.Build.Utilities;
using Earlgrey;

namespace MSBuild.Earlgrey.Tasks.IO
{
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

        public string MaxAge
        {
            get { return _maxAgeString; }
            set { _maxAgeString = value; }
        }

        public string MinAge
        {
            get { return _minAgeString; }
            set { _minAgeString = value; }
        }

        public string MaxLad
        {
            get { return _maxLadString; }
            set { _maxLadString = value; }
        }

        public string MinLad
        {
            get { return _minLadString; }
            set { _minLadString = value; }
        }

        public int NumberOfRetries
        {
            get { return _numberOfRetries; }
            set { _numberOfRetries = value; }
        }


        public bool ExcludeHidden { get; set; }
        public bool ExcludeSystem { get; set; }
        public bool ExcludeTemporary { get; set; }

        public BetterRoboCopy()
            : base()
        {
            this._numberOfRetries = -1;
            // base.ExcluedAttributes = "HS"; // Hidden, System
            this.ExcludeHidden = true;
            this.ExcludeSystem = true;
            this.ExcludeTemporary = true;
        }

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
