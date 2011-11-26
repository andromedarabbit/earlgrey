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
    /// <summary>
    /// A wrapper calss for 'SqlCmd.exe'.
    /// </summary>
    /// <remarks>
    /// This tasks can be used to execute some DDL scripts on the target database.
    /// 
    /// See <a href="http://msdn.microsoft.com/en-us/library/ms162773%28v=SQL.90%29.aspx">sqlcmd Utility</a> for more details. 
    /// MSDN library describes how to use the sqlcmd utility.
    /// </remarks>
    /// <example>
    /// <code title="Run some DDL scripts using sqlcmd utility." lang="xml" source=".\Samples\msbuild-sqlcmd.xml" />
    /// </example>
    public class SqlCmd : AbstractToolTask
    {
        /// <inheritdoc />
        protected override string ToolName
        {
            get { return @"SqlCmd.exe"; }
        }


        // TODO :  private const string InstallPathKey = @"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\90\Tools\PublishingWizard\1.2";

        /// <inheritdoc />
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

        /// <inheritdoc />
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

        /// <summary>
        /// Gets or sets the user login ID.
        /// </summary>
        /// <value>The user login ID.</value>
        /// <remarks>-U switch</remarks>
        public string LoginId { get; set; }

        /// <summary>
        /// Gets or sets the password.
        /// </summary>
        /// <value>The password.</value>
        /// <remarks>-P switch</remarks>
        public string Password { get; set; }

        /// <summary>
        /// Gets or sets the server.
        /// </summary>
        /// <value>The server.</value>
        /// <remarks>-S switch</remarks>
        public string Server { get; set; }

        /// <summary>
        /// Gets or sets the name of the host.
        /// </summary>
        /// <value>The name of the host.</value>
        /// <remarks>-H switch</remarks>
        public string HostName { get; set; }

        /// <summary>
        /// Gets or sets the name of the db.
        /// </summary>
        /// <value>The name of the db.</value>
        /// <remarks>-d switch</remarks>
        public string DbName { get; set; }

        /// <summary>
        /// Gets or sets the input file.
        /// </summary>
        /// <value>The input file.</value>
        /// <remarks>-i switch</remarks>
        public ITaskItem InputFile { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether [trusted connection].
        /// </summary>
        /// <value><c>true</c> if [trusted connection]; otherwise, <c>false</c>.</value>
        /// <remarks>-E switch</remarks>
        public bool TrustedConnection { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to [stop on error].
        /// </summary>
        /// <value><c>true</c> if you want to [stop on error]; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool StopOnError { get; set; }

        /// <summary>
        /// Gets or sets the query.
        /// </summary>
        /// <value>The query.</value>
        /// <remarks>-Q switch</remarks>
        public string Query { get; set; }


    }
}
