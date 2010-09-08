using System;
using System.IO;
using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Subversion
{
    [TestFixture]
    public class SvnPathResolverTest
    {
        // [Ignore]
        [Test]
        public void Execute()
        {
            const string repositoryPath = @"https://earlgrey.googlecode.com/svn/trunk/src/msbuild.xml";
            string localPath = Path.GetFullPath(
                Path.Combine(TaskUtility.ThisAssemblyDirectory, @"..\..\")
                );

            SvnPathResolver resolver = CreateResolver(localPath, repositoryPath);

            Assert.IsTrue(resolver.Execute());
            Assert.AreEqual(1, resolver.LocalPaths.Length);
            Assert.IsTrue(
                resolver.LocalPaths[0].EndsWith(@"src\msbuild.xml")
                );
        }

        private static SvnPathResolver CreateResolver(string localPath, string repositoryPath)
        {
            SvnPathResolver resolver = new SvnPathResolver();
            resolver.BuildEngine = new MockBuildEngine();
            resolver.LocalPath = localPath;
            resolver.RepositoryPaths = new [] { repositoryPath };
            return resolver;
        }

        // [Ignore]
        [Test]
        public void ExecuteWithLocalPathUnversioned()
        {
            const string repositoryPath = @"https://earlgrey.googlecode.com/svn/trunk/src/msbuild.xml";
            string localPath = TaskUtility.ThisAssemblyDirectory;

            SvnPathResolver resolver = CreateResolver(localPath, repositoryPath);
            Assert.IsFalse(resolver.Execute());
        }
    }
}
                                                      