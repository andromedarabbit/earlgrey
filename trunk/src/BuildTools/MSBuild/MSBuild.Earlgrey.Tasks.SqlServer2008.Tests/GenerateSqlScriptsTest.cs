using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using Earlgrey;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2008.Tests
{
    [TestFixture]
    public class GenerateSqlScriptsTest
    {
        private static GenerateSqlScripts CreateInstance()
        {
            GenerateSqlScripts instance = new GenerateSqlScripts();
            instance.BuildEngine = new MockBuildEngine();
            instance.Server = @"localhost\SQLEXPRESS";
            

            // TODO: 임시
            // instance.Database = "model";
            instance.Database = "earlgrey_test";
            // instance.Database = "COMBATARMS";
            return instance;
        }

        [Test]
        public void Run()
        {
            GenerateSqlScripts instance = CreateInstance();
            instance.Overwrite = true;

            instance.CopyData = true;
            instance.ExtendedProperties = true;
            instance.FullTextIndexes = true;
            instance.IncludeIfNotExists = true;
            instance.Indexes = true;
            instance.NoCollation = true;
            instance.SchemaQualify = true;
            instance.Triggers = true;

            Assert.IsTrue(instance.Execute());
        }

        [Test]
        public void IncludeIfNotExists()
        {
            string thisClassName = this.GetType().FullName;
            string thisMethodName = thisClassName + "." + MethodBase.GetCurrentMethod().Name;
            string outputFileName = thisMethodName + ".sql";

            RunIncludeIfNotExistsTestOnce(outputFileName, true, "IF NOT EXISTS (SELECT");
        }

        private static void RunIncludeIfNotExistsTestOnce(string outputFileName, bool includeIfNotExists, string expectedString)
        {
            GenerateSqlScripts instance = CreateInstance();
            instance.OutputFileName = outputFileName;
            instance.Overwrite = true;
            instance.IncludeIfNotExists = includeIfNotExists;

            Assert.IsTrue(instance.Execute());

            string text = File.ReadAllText(outputFileName);
            Assert.IsNotEmpty(text);

            Assert.IsTrue(text.Contains(expectedString));
        }

        [Test]
        public void OutputDir()
        {
            string thisClassName = this.GetType().FullName;
            string thisMethodName = thisClassName + "." + MethodBase.GetCurrentMethod().Name;

            string outputDir = Path.Combine(BuildScripts.TestRootFolder, thisMethodName);
            if (Directory.Exists(outputDir))
                Directory.Delete(outputDir, true);

            GenerateSqlScripts instance = CreateInstance();
            instance.OutputDir = outputDir;
            instance.Overwrite = false;

            Assert.IsTrue(instance.Execute());

            Assert.IsTrue(
                File.Exists(Path.Combine(outputDir, "dbo.simple_table_1.Table.sql"))
                );
        }


        [Test]
        public void SchemaQualify()
        {
            string thisClassName = this.GetType().FullName;
            string thisMethodName = thisClassName + "." + MethodBase.GetCurrentMethod().Name;
            string outputFileName = thisMethodName + ".sql";


            RunSchemaQualifyTestOnce(outputFileName, false, "CREATE TABLE [simple_table_1]");
            RunSchemaQualifyTestOnce(outputFileName, true, "CREATE TABLE [dbo].[simple_table_1]");
        }

        private static void RunSchemaQualifyTestOnce(string outputFileName, bool schemaQualify, string expectedString)
        {
            GenerateSqlScripts instance = CreateInstance();
            instance.OutputFileName = outputFileName;
            instance.Overwrite = true;
            instance.SchemaQualify = schemaQualify;

            Assert.IsTrue(instance.Execute());

            string text = File.ReadAllText(outputFileName);
            Assert.IsNotEmpty(text);

            Assert.IsTrue(text.Contains(expectedString));
        }


        [Test]
        public void Overwrite()
        {
            Assert.IsTrue(RunOnceOverwriteTest(true));
            Assert.IsTrue(RunOnceOverwriteTest(true));
            Assert.IsFalse(RunOnceOverwriteTest(false));
        }

        private static bool RunOnceOverwriteTest(bool overwrite)
        {
            GenerateSqlScripts instance = CreateInstance();
            instance.Overwrite = overwrite;

            return instance.Execute();
        }

        [Test]
        public void NoCollation()
        {
            string thisClassName = this.GetType().FullName;
            string thisMethodName = thisClassName + "." + MethodBase.GetCurrentMethod().Name;
            string outputFileName = thisMethodName + ".sql";

            RunOnceNoCollationTest(outputFileName, true);

            string text = File.ReadAllText(outputFileName);
            Assert.IsNotEmpty(text);

            Assert.IsFalse(
                text.Contains("COLLATE")
                );

            RunOnceNoCollationTest(outputFileName, false);

            text = File.ReadAllText(outputFileName);
            Assert.IsNotEmpty(text);

            Assert.IsTrue(
                text.Contains("COLLATE")
                );

        }

        private static void RunOnceNoCollationTest(string outputFileName, bool noCollation)
        {
            if (File.Exists(outputFileName))
                File.Delete(outputFileName);

            GenerateSqlScripts instance = CreateInstance();
            instance.Overwrite = true;
            instance.OutputFileName = outputFileName;
            instance.NoCollation = noCollation;
            //instance.CopyAllObjects = true;
            //instance.CopySchema = true;
            //instance.CopyAllDatabaseTriggers = true;

            Assert.IsTrue(instance.Execute());
            Assert.IsTrue(File.Exists(instance.OutputFilePath));
        }

    }
}