using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.IO.IniFile;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.IO.IniFile
{
    [TestFixture]
    public class GetSectionsTest
    {
        [Test]
        public void ReadSectionsWithBlanks()
        {
            GetSections reader = new GetSections();
            reader.BuildEngine = new MockBuildEngine();

            string iniFilePath = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"IO\BuildScripts\IniFileTest.Blanks.msbuild.ini"
                );

            reader.FilePath = new TaskItem(iniFilePath);
            Assert.IsTrue(reader.Execute());
        }
    }
}
