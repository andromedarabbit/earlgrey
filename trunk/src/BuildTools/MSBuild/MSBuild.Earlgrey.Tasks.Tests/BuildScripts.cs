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
        private static readonly string TestCaseTargetPrefix;

        static BuildScripts()
        {
            TestCaseTargetPrefix = "TestCase-";
        }

        public BuildScripts()
        {
            ContinueOnError = false;
        }

        private bool ContinueOnError { get; set; }

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

        private bool RunBuildScript(string projectFile)
        {
            string winDir = Environment.GetEnvironmentVariable("windir");
            string binPath = Path.Combine(winDir, @"Microsoft.NET\Framework\v2.0.50727");

            Engine engine = new Engine(binPath);
            
            ILogger logger = new ConsoleLogger(LoggerVerbosity.Diagnostic);
            engine.RegisterLogger(logger);
        
            Project project = new Project(engine);
            project.Load(projectFile);

            bool successful = true;

            foreach (Target target in project.Targets)
            {
                string targetName = target.Name;
                if (targetName.StartsWith(TestCaseTargetPrefix, StringComparison.CurrentCultureIgnoreCase) == false)
                    continue;

                if (RunTarget(project, targetName) == false)
                {
                    if (ContinueOnError == false)
                        return false;
                    successful = false;
                }
            }
            
            return successful;
        }

        private static bool RunTarget(Project project, string targetName)
        {
            if (targetName.EndsWith("-ExpectTrue"))
                return project.Build(targetName) == true;
                    
            if (targetName.EndsWith("-ExpectFalse"))
                return project.Build(targetName) == false;

            throw new Exception(
                string.Format("Unexpected target name '{0}'", targetName)
                );
        }

        [Test]
        public void Execute()
        {
            List<string> files = SearchFile(TaskUtility.ThisAssemblyDirectory, "*.msbuild.xml");
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
