using TestCommon.Net;
using Microsoft.VisualStudio.TestTools.UnitTesting;
namespace TestCommonTest
{
    
    
    /// <summary>
    ///This is a test class for BufferTest and is intended
    ///to contain all BufferTest Unit Tests
    ///</summary>
    [TestClass()]
    public class BufferTest
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
        ///A test for CanWrite
        ///</summary>
        [TestMethod()]
        public void CanWriteTest()
        {
            PacketBuffer target = new PacketBuffer();

            Assert.IsFalse(target.CanWrite(2048));

            byte[] data = new byte[512];
            Assert.IsTrue(target.Append(data, 512));
            Assert.IsFalse(target.CanWrite(513));
            Assert.IsTrue(target.CanWrite(512));
        }
    }
}
