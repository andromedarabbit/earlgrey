using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using MSBuild.Community.Tasks.Subversion;

    public class SvnDelete : SvnClient
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="T:SvnDelete"/> class.
        /// </summary>
        public SvnDelete()
        {
            base.Command = "delete";

            base.NonInteractive = true;
            base.NoAuthCache = true;
        }

        [Required]
        public string PathOrUrl { get; set; }


        protected override string GenerateSvnCommand()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
          
            builder.AppendSwitch(base.Command);
            builder.AppendFileNameIfNotNull(Path.GetFullPath(PathOrUrl));

            
            return builder.ToString();
        }

        /// <summary>
        /// Indicates whether all task paratmeters are valid.
        /// </summary>
        /// <returns>
        /// true if all task parameters are valid; otherwise, false.
        /// </returns>
        protected override bool ValidateParameters()
        {
            return base.ValidateParameters();
        }
    }
}
