using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Net
{

    /// <summary>
    /// Run scripting commands against the remote FTP server by using WinSCP.
    /// </summary>
    /// <example>
    /// <code title="Run scripting commands embedded in the the build script itself against the remote FTP server." lang="xml" source=".\Samples\msbuild-WinScp-using-embeded-script-text.xml" />
    /// <code title="Run scripting commands using a text file against the remote FTP server." lang="xml" source=".\Samples\msbuild-WinScp-using-script-file.xml" />
    /// </example>
    /// <remarks></remarks>
    /// <inheritdoc />
    public class WinScp : AbstractWinScp
    {
        /// <summary>
        /// Gets or sets the script text.
        /// </summary>
        /// <value>The script text.</value>
        /// <remarks></remarks>
        public string ScriptText { get; set; }

        /// <summary>
        /// Gets or sets the script file.
        /// </summary>
        /// <value>The script file.</value>
        /// <remarks></remarks>
        public string ScriptFile { get; set; }

        protected override bool ValidateParameters()
        {
            if(base.ValidateParameters() == false)
                return false;

            if(string.IsNullOrEmpty(ScriptFile) == false && string.IsNullOrEmpty(ScriptText) == false)
            {
                Log.LogError("You can't set both of 'ScriptFile' and 'ScriptText' at the same time!");
                return false;
            }

            if (string.IsNullOrEmpty(ScriptFile) == true && string.IsNullOrEmpty(ScriptText) == true)
            {
                Log.LogError("At least one of 'ScriptFile' and 'ScriptText' should be set!");
                return false;
            }

            return base.ValidateParameters();
        }

        protected override string Scripts
        {
            get
            {
                if (string.IsNullOrEmpty(ScriptFile) == false)
                {
                    return File.ReadAllText(ScriptFile);
                }
                return ScriptText;
            }
        }
        
    }
}
