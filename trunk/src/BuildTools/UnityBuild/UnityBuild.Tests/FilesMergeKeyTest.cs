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

            FileType fileUsingStdAfx2 = FindFile(vcProject, "Dummy.cpp");
            FilesMergeKey keyUsingStdAfx2 = new FilesMergeKey(fileUsingStdAfx2);

            Assert.AreNotEqual(keyUsingStdAfx2, null);
            Assert.AreEqual(keyUsingStdAfx2, keyUsingStdAfx2);

            FileType fileUsingStdAfx2_2 = FindFile(vcProject, "Dummy.cpp");
            FilesMergeKey keyUsingStdAfx2_2 = new FilesMergeKey(fileUsingStdAfx2_2);

            Assert.AreEqual(keyUsingStdAfx2_2, keyUsingStdAfx2);
        }       

        [Test]
        public void Merge()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            FileType fileUsingStdAfx2 = FindFile(vcProject, "Dummy.cpp");
            FilesMergeKey keyUsingStdAfx2 = new FilesMergeKey(fileUsingStdAfx2);
            
            FileType fileUsingNoStdAfx = FindFile(vcProject, "NoPrecompiledHeader.cpp");
            FilesMergeKey keyUsingNoStdAfx = new FilesMergeKey(fileUsingNoStdAfx);

            Assert.AreNotEqual(keyUsingStdAfx2, keyUsingNoStdAfx);
        }

        [Test]
        public void GroupBy()
        {
            VcProject vcProject = GetEarlgreyVcProject();            

            FileType fileUsingStdAfx2 = FindFile(vcProject, "Dummy.cpp");
            FilesMergeKey keyUsingStdAfx2 = new FilesMergeKey(fileUsingStdAfx2);
            KeyValuePair<FilesMergeKey, FileType> pairUsingStdAfx = new KeyValuePair<FilesMergeKey, FileType>(keyUsingStdAfx2, fileUsingStdAfx2);


            FileType fileUsingNoStdAfx = FindFile(vcProject, "NoPrecompiledHeader.cpp");
            FilesMergeKey keyUsingNoStdAfx = new FilesMergeKey(fileUsingNoStdAfx);
            KeyValuePair<FilesMergeKey, FileType> pairUsingNoStdAfx = new KeyValuePair<FilesMergeKey, FileType>(keyUsingNoStdAfx, fileUsingNoStdAfx);

            List<KeyValuePair<FilesMergeKey, FileType>> pairs = new List<KeyValuePair<FilesMergeKey, FileType>>();
            pairs.Add(pairUsingStdAfx);
            pairs.Add(pairUsingNoStdAfx);

            IEnumerable<IGrouping<FilesMergeKey, KeyValuePair<FilesMergeKey, FileType>>> result = pairs.GroupBy(pair => pair.Key);
            Assert.AreEqual(2, result.Count());
            Assert.IsTrue(result.All(grouping => grouping.Count() == 1));
        }
    }
}

