using TestCommon.Net;
using Microsoft.VisualStudio.TestTools.UnitTesting;
namespace TestCommonTest
{
    
    
    /// <summary>
    ///This is a test class for InputStreamTest and is intended
    ///to contain all InputStreamTest Unit Tests
    ///</summary>
    [TestClass()]
    public class InputStreamTest
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
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest9()
        {
            InputStream target = new InputStream(new PacketBuffer());
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest8()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            uint Value = 0; // TODO: Initialize to an appropriate value
            uint ValueExpected = 0; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(out Value);
            Assert.AreEqual(ValueExpected, Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest7()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            long Value = 0; // TODO: Initialize to an appropriate value
            long ValueExpected = 0; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(out Value);
            Assert.AreEqual(ValueExpected, Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest6()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            short Value = 0; // TODO: Initialize to an appropriate value
            short ValueExpected = 0; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(out Value);
            Assert.AreEqual(ValueExpected, Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest5()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            int Value = 0; // TODO: Initialize to an appropriate value
            int ValueExpected = 0; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(out Value);
            Assert.AreEqual(ValueExpected, Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest4()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            char Value = '\0'; // TODO: Initialize to an appropriate value
            char ValueExpected = '\0'; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(out Value);
            Assert.AreEqual(ValueExpected, Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest3()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            Readable Value = null; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest2()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            double Value = 0F; // TODO: Initialize to an appropriate value
            double ValueExpected = 0F; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(out Value);
            Assert.AreEqual(ValueExpected, Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest1()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            ulong Value = 0; // TODO: Initialize to an appropriate value
            ulong ValueExpected = 0; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(out Value);
            Assert.AreEqual(ValueExpected, Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for Read
        ///</summary>
        [TestMethod()]
        public void ReadTest()
        {
            InputStream target = new InputStream(new PacketBuffer()); // TODO: Initialize to an appropriate value
            float Value = 0F; // TODO: Initialize to an appropriate value
            float ValueExpected = 0F; // TODO: Initialize to an appropriate value
            bool expected = false; // TODO: Initialize to an appropriate value
            bool actual;
            actual = target.Read(out Value);
            Assert.AreEqual(ValueExpected, Value);
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for InputStream Constructor
        ///</summary>
        [TestMethod()]
        public void InputStreamConstructorTest()
        {
            InputStream target = new InputStream(new PacketBuffer());
            Assert.Inconclusive("TODO: Implement code to verify target");
        }
    }
}
