using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CWDev.SLNTools.Core;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks;
using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    public class AbstractTest
    {
        private readonly string _tempDir;
        private readonly string _thisDir;
        private readonly string _solutionFileDir;
        private readonly string _sampleVcProjFilePath;
        private readonly string _trunkFolderPath;
        private readonly string _vendorFolderPath;

        private readonly string _earlgreyProjectFIlePath;


        protected AbstractTest()
        {
            _thisDir = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"Temp\UnityBuildTestResources"
                );

            _tempDir = Path.Combine(_thisDir, "Temp");

            _solutionFileDir = Path.Combine(
                TaskUtility.BaseDirectory
                , @"..\..\UnityBuild\UnitTestSample\src"
            );
            _solutionFileDir = Path.GetFullPath(_solutionFileDir);

			//_solutionFilePath = Path.Combine(
			//    _solutionFileDir
			//    , SolutionFileName
			//);
            // _solutionFilePath = Path.GetFullPath(_solutionFilePath);

            _sampleVcProjFilePath = Path.Combine(
               TaskUtility.BaseDirectory
               , @"..\..\UnityBuild\UnitTestSample\src\Earlgrey\"
           );
        	_sampleVcProjFilePath = Path.Combine(_sampleVcProjFilePath, SampleVcProjFileName);
            _sampleVcProjFilePath = Path.GetFullPath(_sampleVcProjFilePath);

            _trunkFolderPath = Path.Combine(_solutionFileDir, @"..\");
            _trunkFolderPath = Path.GetFullPath(_trunkFolderPath);

            _vendorFolderPath = Path.Combine(_trunkFolderPath, @"vendor");
            // _vendorFolderPath = Path.GetFullPath(_vendorFolderPath);

			Project earlgreyProject = GetEarlgreyProject(SolutionFilePath);
            _earlgreyProjectFIlePath = Path.Combine(_solutionFileDir, earlgreyProject.RelativePath);
        }

        [SetUp]
        public virtual void SetUp()
        {
            if (Directory.Exists(_tempDir))
                Directory.Delete(_tempDir, true);

            Directory.CreateDirectory(_tempDir);
        }

        [TearDown]
        public virtual void TearDown()
        {
            if (Directory.Exists(_tempDir))
                Directory.Delete(_tempDir, true);

            Revert();
        }

        internal static void Revert()
        {
            string srcPath = Path.GetFullPath(
              Path.Combine(TaskUtility.ThisAssemblyDirectory, @"..\..\UnityBuild\UnitTestSample\src\")
              );

            Revert(srcPath);


            string venderPath = Path.GetFullPath(
             Path.Combine(srcPath, @"..\vendor\")
             );

            Revert(venderPath);
        }

        private static void Revert(string path)
        {
            SvnRevert cmd = new SvnRevert();
            cmd.BuildEngine = new MockBuildEngine();

            cmd.Paths = new ITaskItem[1];
            cmd.Paths[0] = new TaskItem(path);

            cmd.Recursive = true;
            Assert.IsTrue(cmd.Execute());
        }


		protected static Project GetProject(string solutionFilePath, string projectName)
        {
            SolutionFile slnFile = SolutionFile.FromFile(solutionFilePath);

            var result = from project in slnFile.Projects
                         where project.ProjectName.Equals(projectName, StringComparison.CurrentCultureIgnoreCase)
                         select project
                ;

            var earlgreyProject = result.First();
            Assert.IsNotNull(earlgreyProject);
            return earlgreyProject;
        }

		protected VcProject GetVcProject(string projectName)
        {
			return GetVcProject(SolutionFilePath, projectName);
        }

		protected static VcProject GetVcProject(string solutionFilePath, string projectName)
        {
			VcProject vcProject = new VcProject(GetProject(solutionFilePath, projectName));
            vcProject.Load();
            return vcProject;
        }

		protected Project GetEarlgreyProject()
		{
			return GetProject(SolutionFilePath, "Earlgrey");
		}

        protected static Project GetEarlgreyProject(string solutionFilePath)
        {
			return GetProject(solutionFilePath, "Earlgrey");
        }

        protected VcProject GetEarlgreyVcProject()
        {
            return GetVcProject(SolutionFilePath, "Earlgrey");
        }


        protected static FileType FindFile(VcProject vcProject, string fileName)
        {
			return vcProject.FindFile(fileName);
        }

        protected static FilterType FindFilter(VcProject vcProject, string fileName)
        {
			return vcProject.FindFilter(fileName);
        }

        public string TempDir
        {
            get { return _tempDir; }
        }

        public string ThisDir
        {
            get { return _thisDir; }
        }

        public string SolutionFileDir
        {
            get { return _solutionFileDir; }
        }

    	public string SolutionFileName
    	{
			get 
			{ 
				return "Earlgrey.sln";
			}
    	}

        public string SolutionFilePath
        {
            get
            {
				return Path.Combine(SolutionFileDir, SolutionFileName);
            }
        }

        public string EarlgreySolutionFileText
        {
            get { return File.ReadAllText(SolutionFilePath); }
        }
		
		public string SampleVcProjFileName
		{
			get { return "Earlgrey.vcproj"; }
		}

        public string SampleVcProjFilePath
        {
            get { return _sampleVcProjFilePath; }
        }

        public string TrunkFolderPath
        {
            get { return _trunkFolderPath; }
        }

        public string VendorFolderPath
        {
            get { return _vendorFolderPath; }
        }

        public string EarlgreyProjectFileText
        {
            get { return File.ReadAllText(_earlgreyProjectFIlePath); }
        }

    }
}
