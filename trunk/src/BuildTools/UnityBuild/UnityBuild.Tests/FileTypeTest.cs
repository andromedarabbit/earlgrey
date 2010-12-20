using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class FileTypeTest : AbstractTest
    {
        [Test]
        public void ExcludedFromBuild()
        {
            const string configurationPlatform = "Debug|Win32";

            FileType throwErrorCpp = GetThrowErrorCpp();
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(configurationPlatform));
        }       

        [Test]
        public void IncludeFromBuild()
        {
            const string configurationPlatform = "Debug|Win32";

            FileType throwErrorCpp = GetThrowErrorCpp();
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(configurationPlatform));

            throwErrorCpp.IncludeInBuild(configurationPlatform);
            Assert.IsFalse(throwErrorCpp.ExcludedFromBuild(configurationPlatform));

            throwErrorCpp.ExcludeFromBuild(configurationPlatform);
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(configurationPlatform));
        }

        [Test]
        public void IsSrcFile()
        {
            FileType throwErrorCpp = GetThrowErrorCpp();
            Assert.IsTrue(throwErrorCpp.IsSrcFile);

            FileType earlgreyH = GetEarlgreyH();
            Assert.IsFalse(earlgreyH.IsSrcFile);
        }

        private static FileType GetEarlgreyH()
        {
            IEnumerable<FilterType> filters = GetRootFilters();

            var headerFileFilter = filters.Where(filter => filter.Name == "Header Files").First();
            var result = from fileOrFilter in headerFileFilter.Items
                         where fileOrFilter is FileType
                               && ((FileType) fileOrFilter).RelativePath.EndsWith("Earlgrey.h")
                         select (FileType) fileOrFilter
                ;

            return result.First();
        }

        private static FileType GetThrowErrorCpp()
        {
            IEnumerable<FilterType> filters = GetRootFilters();

            var sourceFileFilter = filters.Where(filter => filter.Name == "Source Files").First();
            var sourceFileSubFilters = from fileOrFilter in sourceFileFilter.Items
                                       where fileOrFilter is FilterType
                                       select (FilterType)fileOrFilter;

            var excludedFilter = sourceFileSubFilters.Where(filter => filter.Name == "UnityBuildTest-Excluded").First();

            var throwErrorCppResult = from file in excludedFilter.Items
                                      where file is FileType
                                      select (FileType)file
                                ;

            return throwErrorCppResult.First();
        }

        private static IEnumerable<FilterType> GetRootFilters()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Load();

            var details = vcProject.Details;

            return from fileOrFilter in details.Files
                   where fileOrFilter is FilterType
                   select (FilterType)fileOrFilter;
        }
    }
}
