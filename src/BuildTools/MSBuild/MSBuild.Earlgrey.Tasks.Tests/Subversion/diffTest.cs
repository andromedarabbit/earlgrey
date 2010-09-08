using System;
using System.IO;
using System.Xml.Serialization;

using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Subversion
{
    [TestFixture]
    public class DiffTest
    {
        private static readonly string SampleDiffFilePath;

        static DiffTest()
        {
            string sampleDiffFilePath = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"Subversion\Sample\DiffXmlSummaryExample.xml"
                );

            SampleDiffFilePath = sampleDiffFilePath;
        }

        [Test]
        public void DeserializeSampleFile()
        {
            using (TextReader tr = new StreamReader(SampleDiffFilePath))
            {
                XmlSerializer ser = new XmlSerializer(typeof(diff));
                diff diffObj = ser.Deserialize(tr) as diff;
                Assert.IsNotNull(diffObj);
            }
            
        }
    }
}
