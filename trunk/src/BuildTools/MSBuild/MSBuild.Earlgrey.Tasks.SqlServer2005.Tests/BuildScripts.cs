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
            var assembly = this.GetType().Assembly;
            string thisAssemblyName = assembly.GetName().Name;
         
            string testScriptsFolder = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"Temp\BuildScripts\" + thisAssemblyName
                );
            List<string> files = FileSearch.Search(testScriptsFolder, "*.msbuild.xml");

            BuildScriptRunner runner = new BuildScriptRunner();
            runner.Execute(files);
        }
    }
}
