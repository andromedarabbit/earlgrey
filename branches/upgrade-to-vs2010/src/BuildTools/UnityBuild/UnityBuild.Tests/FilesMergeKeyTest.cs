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
    public class FilesMergeKeyTest : AbstractTest
    {
        [Test]
        public void EqualsTest()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            IFileType fileUsingStdAfx2 = FindFile(vcProject, "Dummy.cpp");
            FilesMergeKey keyUsingStdAfx2 = new FilesMergeKey(fileUsingStdAfx2);

            Assert.AreNotEqual(keyUsingStdAfx2, null);
            Assert.AreEqual(keyUsingStdAfx2, keyUsingStdAfx2);

            IFileType fileUsingStdAfx2_2 = FindFile(vcProject, "Dummy.cpp");
            FilesMergeKey keyUsingStdAfx2_2 = new FilesMergeKey(fileUsingStdAfx2_2);

            Assert.AreEqual(keyUsingStdAfx2_2, keyUsingStdAfx2);
        }       

        [Test]
        public void Merge()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            IFileType fileUsingStdAfx2 = FindFile(vcProject, "Dummy.cpp");
            FilesMergeKey keyUsingStdAfx2 = new FilesMergeKey(fileUsingStdAfx2);

            IFileType fileUsingNoStdAfx = FindFile(vcProject, "NoPrecompiledHeader.cpp");
            FilesMergeKey keyUsingNoStdAfx = new FilesMergeKey(fileUsingNoStdAfx);

            Assert.AreNotEqual(keyUsingStdAfx2, keyUsingNoStdAfx);
        }

        [Test]
        public void GroupBy()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            IFileType fileUsingStdAfx2 = FindFile(vcProject, "Dummy.cpp");
            FilesMergeKey keyUsingStdAfx2 = new FilesMergeKey(fileUsingStdAfx2);
            KeyValuePair<FilesMergeKey, IFileType> pairUsingStdAfx = new KeyValuePair<FilesMergeKey, IFileType>(keyUsingStdAfx2, fileUsingStdAfx2);


            IFileType fileUsingNoStdAfx = FindFile(vcProject, "NoPrecompiledHeader.cpp");
            FilesMergeKey keyUsingNoStdAfx = new FilesMergeKey(fileUsingNoStdAfx);
            KeyValuePair<FilesMergeKey, IFileType> pairUsingNoStdAfx = new KeyValuePair<FilesMergeKey, IFileType>(keyUsingNoStdAfx, fileUsingNoStdAfx);

            List<KeyValuePair<FilesMergeKey, IFileType>> pairs = new List<KeyValuePair<FilesMergeKey, IFileType>>();
            pairs.Add(pairUsingStdAfx);
            pairs.Add(pairUsingNoStdAfx);

            IEnumerable<IGrouping<FilesMergeKey, KeyValuePair<FilesMergeKey, IFileType>>> result = pairs.GroupBy(pair => pair.Key);
            Assert.AreEqual(2, result.Count());
            Assert.IsTrue(result.All(grouping => grouping.Count() == 1));
        }
    }
}

