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
        public void ExcludedFromBuildTest()
        {
            FileType throwErrorCpp = GetThrowErrorCpp();
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild);
        }


        [Test]
        public void IsSrcFileTest()
        {
            FileType throwErrorCpp = GetThrowErrorCpp();
            Assert.IsTrue(throwErrorCpp.IsSrcFile);
        }

        private static FileType GetThrowErrorCpp()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Load();

            var details = vcProject.Details;

            var filters = from fileOrFilter in details.Files
                          where fileOrFilter is FilterType
                          select (FilterType)fileOrFilter;

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
    }
}
