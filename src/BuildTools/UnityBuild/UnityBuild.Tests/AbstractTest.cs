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
        private static readonly string _TempDir;
        private static readonly string _ThisDir;
        private static readonly string _SolutionFilePath;
        private static readonly string _SolutionFileDir;
        private static readonly string _SampleVcProjFilePath;
        private static readonly string _TrunkFolderPath;
        private static readonly string _VendorFolderPath;

        private static readonly string _EarlgreyProjectFIlePath;


        static AbstractTest()
        {
            _ThisDir = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"UnityBuildTestResources"
                );

            _TempDir = Path.Combine(_ThisDir, "Temp");

            _SolutionFileDir = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory
                , @"..\..\..\BuildTools\UnityBuild\UnitTestSample\src"
            );
            _SolutionFileDir = Path.GetFullPath(_SolutionFileDir);

            _SolutionFilePath = Path.Combine(
                _SolutionFileDir
                , "Earlgrey.sln"
            );
            // _SolutionFilePath = Path.GetFullPath(_SolutionFilePath);

            _SampleVcProjFilePath = Path.Combine(
               AppDomain.CurrentDomain.BaseDirectory
               , @"..\..\..\BuildTools\UnityBuild\UnitTestSample\src\Earlgrey\Earlgrey.vcproj"
           );
            _SampleVcProjFilePath = Path.GetFullPath(_SampleVcProjFilePath);

            _TrunkFolderPath = Path.Combine(_SolutionFilePath, @"..\");
            _TrunkFolderPath = Path.GetFullPath(_TrunkFolderPath);

            _VendorFolderPath = Path.Combine(_TrunkFolderPath, @"vendor");
            // _VendorFolderPath = Path.GetFullPath(_VendorFolderPath);

            Project earlgreyProject = GetEarlgreyProject();
            _EarlgreyProjectFIlePath = Path.Combine(_SolutionFileDir, earlgreyProject.RelativePath);
        }

        [SetUp]
        public virtual void SetUp()
        {
            if(Directory.Exists(_TempDir))
                Directory.Delete(_TempDir, true);

            Directory.CreateDirectory(_TempDir);
        }

        [TearDown]
        public virtual void TearDown()
        {
            if (Directory.Exists(_TempDir))
                Directory.Delete(_TempDir, true);
        }

        protected static void Revert()
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


        protected static Project GetEarlgreyProject()
        {
            SolutionFile slnFile = SolutionFile.FromFile(SolutionFilePath);

            var result = from project in slnFile.Projects
                         where project.ProjectName == "Earlgrey"
                         select project
                ;

            var earlgreyProject = result.First();
            Assert.IsNotNull(earlgreyProject);
            return earlgreyProject;
        }

        public static string TempDir
        {
            get { return _TempDir; }
        }

        public static string ThisDir
        {
            get { return _ThisDir; }
        }

        public static string SolutionFileDir
        {
            get { return _SolutionFileDir; }
        }

        public static string SolutionFilePath
        {
            get { return _SolutionFilePath;  }
        }

        public static string EarlgreySolutionFileText
        {
            get { return File.ReadAllText(_SolutionFilePath); }
        }

        public static string SampleVcProjFilePath
        {
            get { return _SampleVcProjFilePath; }
        }

        public static string TrunkFolderPath
        {
            get { return _TrunkFolderPath; }
        }

        public static string VendorFolderPath
        {
            get { return _VendorFolderPath; }
        }

        public static string EarlgreyProjectFileText
        {
            get { return File.ReadAllText(_EarlgreyProjectFIlePath); }
        }

    }
}
