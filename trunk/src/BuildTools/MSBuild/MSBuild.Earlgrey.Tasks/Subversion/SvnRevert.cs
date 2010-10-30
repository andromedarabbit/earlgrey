using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using MSBuild.Community.Tasks.Subversion;

    public class SvnRevert : SvnClient
    {
        private static readonly string Quot;
        private bool _recursive;

        static SvnRevert()
        {
            Quot = "\"";
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="T:SvnRevert"/> class.
        /// </summary>
        public SvnRevert()
        {
            base.Command = "revert";

            base.NonInteractive = true;
            base.NoAuthCache = true;

            this._recursive = false;
        }

        /// <summary>
        /// Indicates whether all task paratmeters are valid.
        /// </summary>
        /// <returns>
        /// true if all task parameters are valid; otherwise, false.
        /// </returns>
        protected override bool ValidateParameters()
        {
            if(Paths == null || Paths.Length == 0)
            {
                Log.LogError(Properties.Resources.ParameterRequired, "SvnRevert", "Paths");
                return false;
            }

            return base.ValidateParameters();
        }
               

        public bool Recursive
        {
            get { return _recursive; }
            set { _recursive = value; }
        }

        public ITaskItem[] Paths { get; set; }

        protected override string GenerateSvnArguments()
        {   
            StringBuilder builder = new StringBuilder();
            
            if (_recursive)
                builder.Append(" --recursive");

            builder.Append(base.GenerateSvnArguments());
            builder.Append(" ");

            foreach(ITaskItem path in Paths)
            {
                string targetPath = path.ItemSpec.TrimEnd('\\');

                if (path.ItemSpec.StartsWith(Quot) == false)
                    builder.Append(Quot);

                builder.Append(targetPath);

                if (path.ItemSpec.EndsWith(Quot) == false)
                    builder.Append(Quot);

                builder.Append(" ");
            }

            return builder.ToString();
        }

    }
}
