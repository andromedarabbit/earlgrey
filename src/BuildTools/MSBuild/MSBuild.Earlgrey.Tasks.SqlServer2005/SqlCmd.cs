using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{
    public class SqlCmd : AbstractToolTask
    {
        protected override bool ValidateParameters()
        {
            return base.ValidateParameters();
        }
        
        public override bool Execute()
        {
            return base.Execute();
        }

        protected override string ToolName
        {
            get { return @"SqlCmd.exe"; }
        }


        // TODO :  private const string InstallPathKey = @"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\90\Tools\PublishingWizard\1.2";

        protected override string GenerateFullPathToTool()
        {
            IEnumerable<string> filesFound = FileSearch.FindAll(ToolName);
            string fileFound = filesFound.First(
                filePath => filePath.EndsWith(@"90\Tools\Binn\SQLCMD.EXE", StringComparison.CurrentCultureIgnoreCase)
                );

            if(string.IsNullOrEmpty(fileFound))
                throw new FileNotFoundException();

            return fileFound;
        }

        protected override string GenerateCommandLineCommands()
        {
            CommandLineBuilder builder = new CommandLineBuilder();

            if (TrustedConnection == true)
                builder.AppendSwitch("-E");

            builder.AppendSwitchIfNotNull("-U", LoginId);
            builder.AppendSwitchIfNotNull("-P", Password);
            builder.AppendSwitchIfNotNull("-S", Server);
            builder.AppendSwitchIfNotNull("-H", HostName);
            builder.AppendSwitchIfNotNull("-d", DbName);
            builder.AppendSwitchIfNotNull("-i", InputFile);
            builder.AppendSwitchIfNotNull("-Q", Query);


            if (StopOnError == true)
                builder.AppendSwitch("-b");

            return builder.ToString();
        }

        public string LoginId { get; set; }
        public string Password { get; set; }
        public string Server { get; set; }
        public string HostName { get; set; }
        public string DbName { get; set; }
        public ITaskItem InputFile { get; set; }

        public bool TrustedConnection { get; set; }

        public bool StopOnError { get; set; }

        public string Query { get; set; }


    }
}
