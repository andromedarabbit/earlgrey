using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class BuilderTest : AbstractTest
    {
        [Test]
        public void Run()
        {
            using(Builder builder = new Builder(SolutionFilePath))
            {
                builder.Open();
                builder.Dispose();
            }
            
        }
    }
}
