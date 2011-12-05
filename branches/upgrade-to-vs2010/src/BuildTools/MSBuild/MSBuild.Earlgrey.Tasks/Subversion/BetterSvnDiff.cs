using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    /// <summary>
    /// This is a better version of <see cref="SvnDiff"/>. 
    /// <see cref="BetterSvnDiff"/> handles everything more delicately especially when you use "svn:externals" to retrieve source codes from other repositories.
    /// One shortcoming is that <see cref="BetterSvnDiff"/> is slower than <see cref="SvnDiff"/>.
    /// </summary>
    /// <example>
    /// <code title="Display the differences between two paths." lang="xml" source=".\Samples\msbuild-BetterSvnDiff.xml" />
    /// </example>
    /// <remarks></remarks>
    /// <inheritdoc/>
    public class BetterSvnDiff : SvnDiff
    {
        /// <summary>
        /// Executes this instance.
        /// </summary>
        /// <returns></returns>
        /// <inheritdoc/>
        /// <remarks></remarks>
        public override bool Execute()
        {
            if(base.Execute() == false)
                return false;

            SvnPathResolver pathResolver = CreatePathResolver();
            if (pathResolver.Execute() == false)
            {
                Log.LogError("Path resolving failed!");
                return false;
            }

            string[] localPaths = pathResolver.LocalPaths;
            if (ItemsChanged.Count != localPaths.Length)
                throw new Exception("Path resolving failed!");

            for (int i = 0; i < localPaths.Length; i++)
            {
                string localPath = pathResolver.LocalPaths[i];
                ItemsChanged[i].SetMetadata("LocalPath", localPath);
            }

            return true;
        }

        /// <summary>
        /// Creates the path resolver.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        private SvnPathResolver CreatePathResolver()
        {
            SvnPathResolver pathResolver = new SvnPathResolver();
            pathResolver.BuildEngine = this.BuildEngine;
            pathResolver.LocalPath = this.LocalPath;
            pathResolver.Username = this.Username;
            pathResolver.Password = this.Password;
            pathResolver.RepositoryPaths = ItemsChanged.Select(item => item.ItemSpec).ToArray();
            return pathResolver;
        }
    }
}
