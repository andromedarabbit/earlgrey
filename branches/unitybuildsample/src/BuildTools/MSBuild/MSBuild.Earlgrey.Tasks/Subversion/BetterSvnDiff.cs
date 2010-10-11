using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    public class BetterSvnDiff : SvnDiff
    {
        public BetterSvnDiff()
        {
        }

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
