using System;
using System.Collections.Generic;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using MSBuild.Community.Tasks.Subversion;

    public class SvnSwitch : SvnClient
    {
          /// <summary>
        /// Initializes a new instance of the <see cref="T:SvnSwitch"/> class.
        /// </summary>
        public SvnSwitch()
        {
            base.Command = "switch";

            base.NonInteractive = true;
            base.NoAuthCache = true;
        }

        /// <summary>
        /// Indicates whether all task paratmeters are valid.
        /// </summary>
        /// <returns>
        /// true if all task parameters are valid; otherwise, false.
        /// </returns>
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
