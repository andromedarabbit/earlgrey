using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [SetUpFixture]
    public class TestSetUp
    {
        [SetUp]
        public void RunBeforeAnyTests()
        {
            AbstractTest.Revert();
        }

        [TearDown]
        public void RunAfterAnyTests()
        {
        // ...
        }
    }
}
