using System;
using System.Collections.Generic;
using System.IO;
using Earlgrey;
using MSBuild.Earlgrey.TestSupport;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005.Tests
{
    [TestFixture]
    public class BuildScripts
    {
        [Test]
        public void Execute()
        {
            string testScriptsFolder = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"Temp\BuildScripts\MSBuild.Earlgrey.Tasks.SqlServer2005.Tests"
                );
            List<string> files = FileSearch.Search(testScriptsFolder, "*.msbuild.xml");

            BuildScriptRunner runner = new BuildScriptRunner();
            runner.Execute(files);
        }
    }
}
