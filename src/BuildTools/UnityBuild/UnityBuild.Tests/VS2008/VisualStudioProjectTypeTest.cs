using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace UnityBuild.VS2008.Tests
{
    using UnityBuild.Tests;
    using NUnit.Framework;

    [TestFixture]
    public class VisualStudioProjectTypeTest : AbstractTest
    {
        private static string ReadVcProjText(string path)
        {
            using (var sr = new StreamReader(path, true))
            {
                return sr.ReadToEnd();
            }
        }

        [Test]
        public void ReadVcProjectTest()
        {
            string text = ReadVcProjText(SampleVcProjFilePath);
            Assert.IsNotEmpty(text);
        }

        [Test]
        public void DeserializeVcProjectTest()
        {
            // Read in the input XML in a TextReader...
            using (TextReader tr = new StreamReader(SampleVcProjFilePath))
            {
                // Now deserialize.
                var project = (VisualStudioProjectType)(
                    new XmlSerializer(typeof (VisualStudioProjectType))
                ).Deserialize(tr);

                Assert.IsNotNull(project);
            }
        }
        
    }
}
