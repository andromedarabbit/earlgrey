using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Earlgrey;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class TextFileAppendTest : AbstractTest
    {
        private readonly string _textFileUtf8Encoded1Path;
        private readonly string _textFileUtf8Encoded2Path;
        private readonly string _textFileAnsiEncoded1Path;
        private readonly string _textFileAnsiEncoded2Path;
        private readonly string _textFileEmptyPath;
        private readonly string _dstFilePath;
        
        private readonly TextFile _textFileUtf8Encoded1;
        private readonly TextFile _textFileUtf8Encoded2;
        private readonly TextFile _textFileAnsiEncoded1;
        private readonly TextFile _textFileAnsiEncoded2;
        private readonly TextFile _textFileEmpty;
        

        public TextFileAppendTest()
        {
            _textFileAnsiEncoded1Path = Path.Combine(ThisDir, "TextFileAnsiEncoded1.txt");
            _textFileAnsiEncoded2Path = Path.Combine(ThisDir, "TextFileAnsiEncoded2.txt");
            _textFileUtf8Encoded1Path = Path.Combine(ThisDir, "TextFileUtf8Encoded1.txt");
            _textFileUtf8Encoded2Path = Path.Combine(ThisDir, "TextFileUtf8Encoded2.txt");
            _textFileEmptyPath = Path.Combine(ThisDir, "TextFileEmpty.txt");
            _dstFilePath = Path.Combine(TempDir, "FileAppendTest.txt");

            // 인코딩 자동 감지 기능을 테스트하기 위해 일부러 지정하지 않는 경우도 있다.
            _textFileAnsiEncoded1 = new TextFile(_textFileAnsiEncoded1Path);
            _textFileAnsiEncoded2 = new TextFile(_textFileAnsiEncoded2Path, Encoding.Default);
            _textFileUtf8Encoded1 = new TextFile(_textFileUtf8Encoded1Path);
            _textFileUtf8Encoded2 = new TextFile(_textFileUtf8Encoded2Path, Encoding.UTF8);

            _textFileEmpty = new TextFile(_textFileEmptyPath, Encoding.UTF8);
        }

        [SetUp]
        public override void SetUp()
        {
            base.SetUp();

            Assert.IsTrue(File.Exists(_textFileAnsiEncoded1Path));
            Assert.IsTrue(File.Exists(_textFileAnsiEncoded1Path));
            Assert.IsTrue(File.Exists(_textFileUtf8Encoded1Path));
            Assert.IsTrue(File.Exists(_textFileUtf8Encoded2Path));
            Assert.IsTrue(File.Exists(_textFileEmptyPath));

            if (File.Exists(_dstFilePath))
                File.Delete(_dstFilePath);
        }

        [Test]
        public void AppendEmptyFile()
        {
            Encoding dstEncoding = Encoding.UTF8;

            TextFile dstFile = new TextFile(_dstFilePath, dstEncoding);

            TextFilesAppend append = new TextFilesAppend(dstFile, _textFileEmpty);
            append.Merge();

            Assert.IsTrue(File.Exists(_dstFilePath));
            // NOTE: UTF8 의 BOM 때문에 dstFile의 길이가 0 가 아닌 3 이 되고 FileAssert.AreEqual 조건은 실패한다.
            // FileAssert.AreEqual(srcFilePath1, dstFilePath);
            Assert.AreEqual(0, File.ReadAllText(_dstFilePath, dstEncoding).Length);
        }

        [Test]
        public void AppendAnsiEncodedFileAndUtf8EncodedFile()
        {
            Encoding dstEncoding = Encoding.UTF7;

            TextFile dstFile = new TextFile(_dstFilePath, dstEncoding);

            TextFilesAppend append = new TextFilesAppend(dstFile, _textFileAnsiEncoded1, _textFileAnsiEncoded2);
            append.Merge();


            string mergedText = File.ReadAllText(_dstFilePath, dstEncoding);
            Assert.IsFalse(string.IsNullOrEmpty(mergedText));
            Assert.AreEqual("베이시스트빅터 우튼", mergedText);
        }


        [Test]
        public void AppendDelimiterBetweenTwoFiles()
        {
            Encoding dstEncoding = Encoding.UTF32;

            TextFile dstFile = new TextFile(_dstFilePath, dstEncoding);

            TextFilesAppend append = new TextFilesAppend(dstFile, _textFileAnsiEncoded1, _textFileAnsiEncoded2);
            append.Delimiter = " - "; 
            append.Merge();

            string mergedText = File.ReadAllText(_dstFilePath, dstEncoding);
            Assert.IsFalse(string.IsNullOrEmpty(mergedText));
            Assert.AreEqual("베이시스트 - 빅터 우튼", mergedText);
        }

        [Test]
        public void AppendDelimiterBetweenFourFiles()
        {
            Encoding dstEncoding = Encoding.Unicode;

            TextFile dstFile = new TextFile(_dstFilePath, dstEncoding);

            TextFilesAppend append = new TextFilesAppend(dstFile, _textFileAnsiEncoded1, _textFileAnsiEncoded2, _textFileUtf8Encoded1, _textFileUtf8Encoded2);
            append.Delimiter = " - ";
            append.Merge();

            string mergedText = File.ReadAllText(_dstFilePath, dstEncoding);
            Assert.IsFalse(string.IsNullOrEmpty(mergedText));
            Assert.AreEqual("베이시스트 - 빅터 우튼 - 베이시스트 - 안재현", mergedText);
        }
    }
}
