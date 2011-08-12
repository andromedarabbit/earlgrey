using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Earlgrey;
using NUnit.Framework;

namespace Earlgrey.Tests
{
    [TestFixture]
    public class FileSearchTest
    {
        [Test]
        public void Exists()
        {
            Assert.IsTrue(
                FileSearch.Exists("notepad.exe")
                );

            Assert.IsFalse(
                FileSearch.Exists("CannotPossiblyExist.exe")
                );
        }
    }
}
