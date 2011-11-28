using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using MSBuild.Community.Tasks.Subversion;

    /// <summary>
    /// Undo all local edits.
    /// </summary>
    /// <example>
    /// <code title="Update working copy to a different URL and revert it." lang="xml" source=".\Samples\msbuild-SvnSwitch-SvnRevert.xml" />
    /// <code title="Delete an item from a working copy and revert it." lang="xml" source=".\Samples\msbuild-SvnDelete-SvnRevert.xml" />
    /// </example>
    /// <remarks>
    /// Reverts any local changes to a file or directory and resolves any conflicted states. 
    /// svn revert will not only revert the contents of an item in your working copy, but also any property changes. 
    /// Finally, you can use it to undo any scheduling operations that you may have done (e.g. files scheduled for addition or deletion can be “unscheduled”).
    /// </remarks>
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
        /// <remarks></remarks>
        /// <inheritdoc />
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
        /// <returns>true if all task parameters are valid; otherwise, false.</returns>
        /// <remarks></remarks>
        protected override bool ValidateParameters()
        {
            if(Paths == null || Paths.Length == 0)
            {
                Log.LogError(Properties.Resources.ParameterRequired, "SvnRevert", "Paths");
                return false;
            }

            return base.ValidateParameters();
        }


        /// <summary>
        /// Gets or sets a value indicating whether this <see cref="SvnRevert"/> is recursive.
        /// </summary>
        /// <value><c>true</c> if recursive; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool Recursive
        {
            get { return _recursive; }
            set { _recursive = value; }
        }

        /// <summary>
        /// Gets or sets the paths of items to revert.
        /// </summary>
        /// <value>The paths of items to revert.</value>
        /// <remarks></remarks>
        public ITaskItem[] Paths { get; set; }

        /// <summary>
        /// Generates the SVN arguments.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
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
