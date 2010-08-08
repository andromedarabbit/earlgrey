using System;
using System.IO;
using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Subversion
{
    [TestFixture]
    public class SvnPathResolverTest
    {

        [Test]
        public void Execute()
        {
            // string 
            const string repositoryPath = @"https://earlgrey.googlecode.com/svn/trunk/src/msbuild.xml";
            // const string expectedLocalPath = @"D:\My Workspace\MyCombatArms\trunk\src\BuildTools\MSBuild.Earlgrey.Tasks\bin\MSBuild.Earlgrey.Tasks.Targets";
            string localPath = Path.GetFullPath(
                Path.Combine(TaskUtility.BaseDirectory, @"..\..\")
                );

            SvnPathResolver resolver = new SvnPathResolver();
            resolver.BuildEngine = new MSBuild.Community.Tasks.Tests.MockBuild();
            resolver.LocalPath = localPath;
            resolver.RepositoryPaths = new string[] {
                repositoryPath
            };
                        
            Assert.IsTrue(resolver.Execute());
            Assert.AreEqual(1, resolver.LocalPaths.Length);
            Assert.IsTrue(
                resolver.LocalPaths[0].EndsWith(@"src\msbuild.xml")
                );
            // Assert.AreEqual(expectedLocalPath, resolver.LocalPaths[0]);
            
        }
    }
}
                                                      