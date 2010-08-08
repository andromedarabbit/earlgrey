﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

using Microsoft.Build.Utilities;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO
{
    public class RoboCopy : ToolTask
    {
        [Required]
        public ITaskItem SourceDir { get; set; }

        [Required]
        public ITaskItem DestinationDir { get; set; }

        [Required]
        public ITaskItem[] Files { get; set; }

        public string Options { get; set; }

        protected override string ToolName
        {
            get { return "RoboCopy.exe"; }
        }

        protected override string GenerateFullPathToTool()
        {
            string fullPathToTool = FullPathToTool;
            
            if (File.Exists(fullPathToTool))
                return fullPathToTool;

            return Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory
                , @"ExternalTools\" + ToolName
                );
        }

        private string FullPathToTool
        {
            get
            {
                if (string.IsNullOrEmpty(this.ToolPath) == true)
                    return this.ToolName;

                return Path.Combine(this.ToolPath, this.ToolName);
            }
        }

        protected override string GenerateCommandLineCommands()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
            builder.AppendFileNameIfNotNull(this.SourceDir);
            builder.AppendFileNameIfNotNull(this.DestinationDir);
            builder.AppendFileNamesIfNotNull(this.Files, " ");

            if (!string.IsNullOrEmpty(this.Options))
            {
                builder.AppendSwitch(this.Options);
            }

            return builder.ToString();
        }

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
            //Debug.Assert(string.IsNullOrEmpty(pathToTool) == false);
            //Debug.Assert(string.IsNullOrEmpty(responseFileCommands) == false);
            //Debug.Assert(string.IsNullOrEmpty(commandLineCommands) == false);

            Log.LogMessage("Running " + pathToTool + " " + commandLineCommands);
            return HandleExitCode(
                base.ExecuteTool(pathToTool, responseFileCommands, commandLineCommands)
                );
        }

        private int HandleExitCode(int retValue)
        {
            EExitCode exitCode = (EExitCode) (retValue);
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
