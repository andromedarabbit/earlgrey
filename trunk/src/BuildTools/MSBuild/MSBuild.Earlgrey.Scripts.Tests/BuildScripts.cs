using System;
using System.Collections.Generic;
using System.IO;
using Earlgrey;
using MSBuild.Earlgrey.TestSupport;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Scripts.Tests
{
    [TestFixture]
    public class BuildScripts
    {
        [Test]
        public void Execute()
        {
            string scriptFolder = Path.Combine(TaskUtility.ThisAssemblyDirectory, "Scripts");
            List<string> files = FileSearch.Search(scriptFolder, "*.msbuild.xml");

            BuildScriptRunner runner = new BuildScriptRunner();
            runner.Execute(files);
        }
    }
}
