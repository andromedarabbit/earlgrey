using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using Earlgrey;
using MSBuild.Earlgrey.TestSupport;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005.Tests
{
    [TestFixture]
    public class BuildScripts
    {
        private static readonly string _TestRootFolder;
        private static readonly string _TestScriptsFolder;

        static BuildScripts()
        {
            var assembly = Assembly.GetExecutingAssembly();
            string thisAssemblyName = assembly.GetName().Name;

            _TestRootFolder = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"Temp"
                );
            _TestScriptsFolder = Path.Combine(
                _TestRootFolder
                , @"BuildScripts\" + thisAssemblyName
                );
        }

        public static string TestRootFolder
        {
            get { return _TestRootFolder; }
        }

        public static string TestScriptsFolder
        {
            get { return _TestScriptsFolder; }
        }

        [Test]
        public void Execute()
        {

            List<string> files = FileSearch.Search(_TestScriptsFolder, "*.msbuild.xml");

            BuildScriptRunner runner = new BuildScriptRunner();
            runner.Execute(files);
        }
    }
}
