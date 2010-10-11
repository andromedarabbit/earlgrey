using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class NumberProviderTest
    {
        [Test]
        public void GetCurrentNumber()
        {
            NumberProvider provider = new NumberProvider();
            Assert.AreEqual(0, provider.No);
            Assert.AreEqual(0, provider.No);

            provider.Next();
            Assert.AreEqual(1, provider.No);
            Assert.AreEqual(1, provider.No);

            provider.Next();
            Assert.AreEqual(2, provider.No);

            provider.Next();
            Assert.AreEqual(3, provider.No);
        }

        [Test]
        public void GetCurrentNumberString()
        {
            NumberProvider provider = new NumberProvider();
            Assert.AreEqual("00000", provider.NoString);

            provider.SetNo(3456);
            Assert.AreEqual("03456", provider.NoString);

            provider.SetNo(99999);
            Assert.AreEqual("99999", provider.NoString);
        }

        [Test]
        [ExpectedException(typeof(OverflowException))]
        public void OverflowExceptionThrown()
        {
            NumberProvider provider = new NumberProvider();
            provider.SetNo(99999);
            Assert.AreEqual("99999", provider.NoString);
            
            provider.Next();
        }
    }
}
