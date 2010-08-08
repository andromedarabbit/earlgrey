using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Diagnostics;

using Microsoft.Build.Framework;
using MSBuild.Community.Tasks.Subversion;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    public class SvnPathResolver 
        : Microsoft.Build.Utilities.ToolTask 
    {
        private readonly List<string> _repositoryPaths;
        private readonly List<string> _localPaths;
        private readonly SvnInfo _svnInfo;

        private class Roots
        {
            public string RepositoryRoot
            {
                get;
                set;
            }

            public string LocalRoot
            {
                get;
                set;
            }

            public string RepositoryPathOfLocalRoot
            {
                get; set;
            }

            public bool IsValid
            {
                get
                {
                    return
                        string.IsNullOrEmpty(this.LocalRoot) == false
                        && Directory.Exists(this.LocalRoot) == true
                        && string.IsNullOrEmpty(this.RepositoryRoot) == false
                        && string.IsNullOrEmpty(this.RepositoryPathOfLocalRoot) == false
                        ;
                }
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="T:SvnPathResolver"/> class.
        /// </summary>
        public SvnPathResolver()
        {   
            _repositoryPaths = new List<string>();
            _localPaths = new List<string>();
            _svnInfo = new SvnInfo();
        }

        protected override string GenerateFullPathToTool()
        {
            Debug.Assert(string.IsNullOrEmpty(_svnInfo.ToolPath) == false);
            return _svnInfo.ToolPath;
        }

        protected override string ToolName
        {
            get
            {
                Debug.Assert(_svnInfo != null);
                return Path.GetFileName(_svnInfo.ToolPath);
            }
        }

        public string LocalPath
        {
            get; set;
        }

        public string Username
        {
            get;
            set;
        }

        public string Password
        {
            get;
            set;
        }


        /// <summary>
        /// Indicates whether all task paratmeters are valid.
        /// </summary>
        /// <returns>
        /// true if all task parameters are valid; otherwise, false.
        /// </returns>
        protected override bool ValidateParameters()
        {
            if (_repositoryPaths.Count == 0)
            {
                Log.LogError(MSBuild.Community.Tasks.Properties.Resources.ParameterRequired, "SvnPathResolver", "RepositoryPaths");
                return false;
            }

            if(string.IsNullOrEmpty(LocalPath))
            {
                Log.LogError(MSBuild.Community.Tasks.Properties.Resources.ParameterRequired, "SvnPathResolver", "LocalPath");
                return false;
            }

            Trace.Assert(_localPaths.Count == 0);

            return base.ValidateParameters();
        }

        public override bool Execute()
        {
            if (string.IsNullOrEmpty(LocalPath))
            {
                LocalPath = AppDomain.CurrentDomain.BaseDirectory;
            }
            
            if(ValidateParameters() == false)
                return false;

            // Get local root path
            Roots roots = GetRoots();
            if (roots == null)
            {
                Log.LogError("Couldn't find local root from the local path \"{0}\"!", LocalPath);
                return false;
            }
            
            string repositoryRoot = roots.RepositoryPathOfLocalRoot;
            foreach(var repositoryPath in _repositoryPaths)
            {
                string relativePath = repositoryPath.Substring(repositoryRoot.Length, repositoryPath.Length - repositoryRoot.Length);
                relativePath = relativePath.TrimStart(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar);
                
                _localPaths.Add(
                    Path.GetFullPath( Path.Combine(roots.LocalRoot, relativePath) )
                    );
            }


            return true;
        }

        private Roots GetRoots()
        {
            SvnInfo svnInfo = null;
            Roots roots = new Roots();
            
            string path = LocalPath + @"\"; // 뒤에 '\'를 붙이면 do while 대신 while 로 처리 가능하다.
            while ((path = GetNextPath(path)) != null)
            {
                svnInfo = CreateSvnInfoInstance(path); // _svnInfo 인스턴스를 재활용하면 좋지만 SvnInfo 클래스가 1회 실행을 전제로 설계되었다.                
                if (svnInfo.Execute() == false)
                    break;
                
                roots.RepositoryRoot = svnInfo.RepositoryRoot; // 한번만 해도 될 듯 하나 귀찮으니......
                roots.LocalRoot = path;
            }

            svnInfo = CreateSvnInfoInstance(roots.LocalRoot);
            if (svnInfo.Execute() == false)
            {
                throw new Exception(
                    string.Format("Couldn't find repository path of local root path '{0}'", roots.LocalRoot)
                    );
            }
            roots.RepositoryPathOfLocalRoot = svnInfo.RepositoryPath;

            if(roots.IsValid)
                return roots;
            
            return null;
        }

        private SvnInfo CreateSvnInfoInstance(string localPath)
        {
            SvnInfo svnInfo = new SvnInfo();

            // 일부러 실패할 때까지 svn info  를 실행하는데 빌드 엔진에 실패를 보고하면 안 된다(정상적인 상황이므로).
            // 따라서 MockBuildEngine 인스턴스에 보고하게 한다.                 
            svnInfo.BuildEngine = new MockBuildEngine();
            svnInfo.Username = Username;
            svnInfo.Password = Password;
            svnInfo.LocalPath = localPath;

            return svnInfo;
        }

        private string GetNextPath(string path)
        {
            DirectoryInfo parent = Directory.GetParent(path);
            if (parent == null || parent.Exists == false)
                return null;

            return parent.FullName;
        }

        public string[] RepositoryPaths
        {
            get 
            { 
                return _repositoryPaths.ToArray<string>(); 
            }
            set 
            { 
                foreach(string path in value)
                {
                    _repositoryPaths.Add(path);
                }                 
            }
        }

        [Output]
        public string[] LocalPaths
        {
            get 
            {
                return _localPaths.ToArray<string>();
            }
        }
    }
}
