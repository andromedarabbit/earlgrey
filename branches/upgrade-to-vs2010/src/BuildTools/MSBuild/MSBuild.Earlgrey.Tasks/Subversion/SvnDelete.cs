using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using MSBuild.Community.Tasks.Subversion;

    /// <summary>
    /// Delete an item from a working copy or the repository.
    /// </summary>
    /// <example>
    /// <code title="Delete an item from a working copy and revert it." lang="xml" source=".\Samples\msbuild-SvnDelete-SvnRevert.xml" />
    /// </example>
    /// <remarks></remarks>
    /// <inheritdoc />
    public class SvnDelete : SvnClient
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="T:SvnDelete"/> class.
        /// </summary>
        /// <inheritdoc />
        public SvnDelete()
        {
            base.Command = "delete";

            base.NonInteractive = true;
            base.NoAuthCache = true;
        }

        /// <summary>
        /// [Required] Gets or sets the local path or the repository URL of the working copy.
        /// </summary>
        /// <value>The path or URL.</value>
        /// <remarks></remarks>
        [Required]
        public string PathOrUrl { get; set; }


        /// <summary>
        /// Generates the SVN command.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        /// <inheritdoc />
        protected override string GenerateSvnCommand()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
          
            builder.AppendSwitch(base.Command);
            builder.AppendFileNameIfNotNull(Path.GetFullPath(PathOrUrl));

            
            return builder.ToString();
        }
    }
}
