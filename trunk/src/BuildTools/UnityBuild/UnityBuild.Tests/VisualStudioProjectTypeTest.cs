using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace UnityBuild.Tests
{
    using NUnit.Framework;

    [TestFixture]
    public class VisualStudioProjectTypeTest
    {
        // private static readonly string sampleVcProjFileName;
        private static readonly string SampleVcProjFilePath;

        // private static readonly string xsdFileName;
     //   private static readonly string XsdFilePath;


        static VisualStudioProjectTypeTest()
        {
            // sampleVcProjFileName = "Earlgrey.vcproj";

            SampleVcProjFilePath = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory
                , @"..\..\..\Earlgrey\Earlgrey.vcproj"
            );
            SampleVcProjFilePath = Path.GetFullPath(SampleVcProjFilePath);
//             // xsdFileName = "VisualStudioProjectType.xsd";
//             XsdFilePath = Path.Combine(
//                 AppDomain.CurrentDomain.BaseDirectory
//                 , @"..\..\..\VCProject\VisualStudioProjectType.xsd"
//             );
        }

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
            // VisualStudioProjectType project = null;

            // string requestXsdNameSpace = string.Empty;
     
            // Create a validating XML reader and validate...
//             var context = new XmlParserContext(null, null, "", XmlSpace.None);
//             // var vr = new XmlValidatingReader(SampleVcProjFilePath, XmlNodeType.Element, context);
//             var vr = new XmlValidatingReader(SampleVcProjFilePath, XmlNodeType.Element, context);
//             vr.ValidationType = ValidationType.Schema;
//             vr.Schemas.Add(requestXsdNameSpace, XsdFilePath);
//             vr.ValidationEventHandler += new ValidationEventHandler(ValidationEventHandler);
//             while (vr.Read())
//             {
           // }

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

        static void ValidationEventHandler(object sender, ValidationEventArgs e)
        {
            throw new NotImplementedException();
        }
        
    }
}
