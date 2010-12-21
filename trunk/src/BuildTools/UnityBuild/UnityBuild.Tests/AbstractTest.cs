﻿using System;
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
            Revert();

            if(Directory.Exists(_TempDir))
                Directory.Delete(_TempDir, true);

            Directory.CreateDirectory(_TempDir);
        }

        [TearDown]
        public virtual void TearDown()
        {
            if (Directory.Exists(_TempDir))
                Directory.Delete(_TempDir, true);

            Revert();
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


        protected static Project GetProject(string projectName)
        {
            SolutionFile slnFile = SolutionFile.FromFile(SolutionFilePath);

            var result = from project in slnFile.Projects
                         where project.ProjectName.Equals(projectName, StringComparison.CurrentCultureIgnoreCase)
                         select project
                ;

            var earlgreyProject = result.First();
            Assert.IsNotNull(earlgreyProject);
            return earlgreyProject;
        }

        protected static VcProject GetVcProject(string projectName)
        {
            VcProject vcProject = new VcProject(GetProject(projectName));
            vcProject.Load();
            return vcProject;
        }

        protected static Project GetEarlgreyProject()
        {
            return GetProject("Earlgrey");
        }

        protected static VcProject GetEarlgreyVcProject()
        {
            return GetVcProject("Earlgrey");
        }


        protected static FileType FindFile(VcProject vcProject, string fileName)
        {
            return FindFile(vcProject.Details.Files, fileName);
        }

        private static FileType FindFile(IEnumerable<object> items, string fileName)
        {
            foreach(object item in items)
            {
                if(item is FileType)
                {
                    FileType file = (FileType) item;
                    if(file.RelativePath.EndsWith(fileName) == true)
                        return file;
                }

                if(item is FilterType)
                {
                    FilterType filter = (FilterType) item;
                    FileType fileFound = FindFile(filter.Items, fileName);
                    if (fileFound != null)
                        return fileFound;
                }
            }

            return null;
        }

        protected static FilterType FindFilter(VcProject vcProject, string fileName)
        {
            return FindFilter(vcProject.Details.Files, fileName);
        }

        private static FilterType FindFilter(IEnumerable<object> items, string filterName)
        {
            foreach (object item in items)
            {            
                if (item is FilterType)
                {
                    FilterType filter = (FilterType)item;
                    if(filter.Name.Equals(filterName, StringComparison.CurrentCultureIgnoreCase) == true)
                        return filter;

                    FilterType filterFound = FindFilter(filter.Items, filterName);
                    if (filterFound != null)
                        return filterFound;
                }
            }

            return null;
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
