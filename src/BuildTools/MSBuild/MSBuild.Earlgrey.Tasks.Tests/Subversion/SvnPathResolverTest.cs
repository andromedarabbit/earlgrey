﻿using System;
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
            const string repositoryPath = @"https://earlgrey.googlecode.com/svn/trunk/src/msbuild.xml";
            string localPath = Path.GetFullPath(
                Path.Combine(TaskUtility.BaseDirectory, @"..\..\")
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
            resolver.BuildEngine = new MSBuild.Community.Tasks.Tests.MockBuild();
            resolver.LocalPath = localPath;
            resolver.RepositoryPaths = new string[] {
                                                        repositoryPath
                                                    };
            return resolver;
        }

        [Test]
        public void ExecuteWithLocalPathUnversioned()
        {
            const string repositoryPath = @"https://earlgrey.googlecode.com/svn/trunk/src/msbuild.xml";
            string localPath = TaskUtility.BaseDirectory;

            SvnPathResolver resolver = CreateResolver(localPath, repositoryPath);
            Assert.IsFalse(resolver.Execute());
        }
    }
}
                                                      