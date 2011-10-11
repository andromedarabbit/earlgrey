using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2008.Tests
{
    [TestFixture]
    public class EncodingTest
    {
        [Test]
        public void StringToEncodingObject()
        {
            Encoding encoding = System.Text.Encoding.GetEncoding("UTF-8");
            Assert.IsNotNull(encoding);
        }

    }
}
