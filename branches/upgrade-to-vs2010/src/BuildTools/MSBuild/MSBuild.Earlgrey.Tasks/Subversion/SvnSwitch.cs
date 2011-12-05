using System;
using System.Collections.Generic;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using MSBuild.Community.Tasks.Subversion;

    /// <summary>
    /// Update working copy to a different URL.
    /// </summary>
    /// <example>
    /// <code title="Update working copy to a different URL and revert it." lang="xml" source=".\Samples\msbuild-SvnSwitch-SvnRevert.xml" />
    /// </example>
    /// <remarks>
    /// <see cref="SvnClient.RepositoryPath"/> and <see cref="SvnClient.LocalPath"/> are required to switch to the different URL.
    /// </remarks>
    public class SvnSwitch : SvnClient
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="T:SvnSwitch"/> class.
        /// </summary>
        /// <remarks></remarks>
        /// <inheritdoc />
        public SvnSwitch()
        {
            base.Command = "switch";

            base.NonInteractive = true;
            base.NoAuthCache = true;
        }

        /// <summary>
        /// Indicates whether all task parameters are valid.
        /// </summary>
        /// <returns>true if all task parameters are valid; otherwise, false.</returns>
        /// <remarks></remarks>
        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            if (string.IsNullOrEmpty(base.RepositoryPath))
            {
                Log.LogError(Properties.Resources.ParameterRequired, "SvnSwitch", "RepositoryPath");
                return false;
            }

            if (string.IsNullOrEmpty(base.LocalPath))
            {
                Log.LogError(Properties.Resources.ParameterRequired, "SvnSwitch", "LocalPath");
                return false;
            }
            return base.ValidateParameters();
        }
    }
}
