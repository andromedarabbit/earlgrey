using TestCommon.Net;
using Microsoft.VisualStudio.TestTools.UnitTesting;
namespace TestCommonTest
{
    
    
    /// <summary>
    ///This is a test class for StreamTest and is intended
    ///to contain all StreamTest Unit Tests
    ///</summary>
    [TestClass()]
    public class StreamTest
    {


        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        // 
        //You can use the following additional attributes as you write your tests:
        //
        //Use ClassInitialize to run code before running the first test in the class
        //[ClassInitialize()]
        //public static void MyClassInitialize(TestContext testContext)
        //{
        //}
        //
        //Use ClassCleanup to run code after all tests in a class have run
        //[ClassCleanup()]
        //public static void MyClassCleanup()
        //{
        //}
        //
        //Use TestInitialize to run code before running each test
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
        //}
        //
        //Use TestCleanup to run code after each test has run
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        ///A test for Write
        ///</summary>
        [TestMethod()]
        public void WriteTest()
        {
            OutputStream target = new OutputStream(new Client());
            Assert.IsTrue(target.Write('a'));
            Assert.IsTrue(target.Write(-123));
            Assert.IsTrue(target.Write((uint)33));
            Assert.IsTrue(target.Write((long)7342743));
            Assert.IsTrue(target.Write((ulong)8329849));
            Assert.IsTrue(target.Write(1.2f));
            Assert.IsTrue(target.Write((double)1.234f));
            Assert.IsTrue(target.Write("Hello!"));
        }

        /// <summary>
        ///A test for Stream Constructor
        ///</summary>
        [TestMethod()]
        public void StreamConstructorTest()
        {
            OutputStream target = new OutputStream(new Client());
            Assert.IsNotNull(target);
        }
    }
}
