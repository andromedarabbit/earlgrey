﻿using System;
using System.Collections.Generic;
using Earlgrey;
using MSBuild.Earlgrey.TestSupport;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2008.Tests
{
    [TestFixture]
    public class BuildScripts
    {
        [Test]
        public void Execute()
        {
            List<string> files = FileSearch.Search(TaskUtility.ThisAssemblyDirectory, "*.msbuild.xml");

            BuildScriptRunner runner = new BuildScriptRunner();
            runner.Execute(files);
        }
    }
}
