using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

using Microsoft.Build.BuildEngine;
using Microsoft.Build.Framework;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests
{
    [TestFixture]
    public class BuildScripts
    {
        //! \todo 이 기능을 여기저기서 쓰니 따로 정리하자.
        private static List<string> SearchFile(string rootDir, string pattern)
        {
            List<string> files = new List<string>();
            SearchFile(rootDir, pattern, files);
            return files;
        }

        private static void SearchFile(string rootDir, string pattern, List<string> files)
        {
            foreach (string dir in Directory.GetDirectories(rootDir))
            {
                foreach (string file in Directory.GetFiles(dir, pattern))
                {
                    files.Add(file);
                }
                SearchFile(dir, pattern, files);
            }
        }

        private static bool RunBuildScript(string projectFile)
        {
            string winDir = Environment.GetEnvironmentVariable("windir");
            string binPath = Path.Combine(winDir, @"Microsoft.NET\Framework\v2.0.50727");

            Engine engine = new Engine(binPath);
            
            ILogger logger = new ConsoleLogger(LoggerVerbosity.Diagnostic);
            engine.RegisterLogger(logger);
        
            Project project = new Project(engine);
            project.Load(projectFile);
       
            if(project.Build() == false)
                return false;

            return true;
        }

        [Test]
        public void Execute()
        {
            List<string> files = SearchFile(TaskUtility.BaseDirectory, "*.msbuild.xml");
            CollectionAssert.IsNotEmpty(files);

            files.ForEach(
                file => Assert.IsTrue(File.Exists(file))
                );

            files.ForEach(
                file => Assert.IsTrue(RunBuildScript(file))
                );
        }
    }
}
