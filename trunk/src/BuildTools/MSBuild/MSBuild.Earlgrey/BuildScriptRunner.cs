using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

using Microsoft.Build.BuildEngine;
using Microsoft.Build.Framework;
using NUnit.Framework;

namespace MSBuild.Earlgrey
{
    public class BuildScriptRunner
    {
        private static readonly string TestCaseTargetPrefix;

        static BuildScriptRunner()
        {
            TestCaseTargetPrefix = "TestCase-";
        }

        public BuildScriptRunner()
        {
            ContinueOnError = false;
        }

        private bool ContinueOnError { get; set; }

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

        public void Execute(List<string> files)
        {
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
