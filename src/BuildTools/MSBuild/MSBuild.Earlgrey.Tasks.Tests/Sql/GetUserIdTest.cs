﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.Sql;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Sql
{
    [TestFixture]
    public class GetUserIdTest
    {
        [Test]
        public void Run()
        {
            GetUserId instance = new GetUserId();
            instance.BuildEngine = new MockBuildEngine();
            instance.ConnectionString = @"Data Source=localhost\SQLEXPRESS; Initial Catalog=master; Integrated Security=SSPI";
            
            List<ITaskItem> names = new List<ITaskItem>();
            names.Add(new TaskItem("dbo"));
            
            instance.Names = names.ToArray();

            Assert.IsTrue(instance.Execute());
            
            ITaskItem[] identities = instance.Ids;
            CollectionAssert.IsNotEmpty(identities);

            Assert.AreEqual(names.Count, identities.Length);

            Assert.IsTrue(
                identities.All(item => int.Parse(item.GetMetadata("Id")) != 0)
                );
        }
    }
}
