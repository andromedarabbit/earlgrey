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
            return CreateInstanceWithDatabaseName("earlgrey_test");
        }

        private static GenerateSqlScripts CreateInstanceWithDatabaseName(string databaseName)
        {
            GenerateSqlScripts instance = new GenerateSqlScripts();
            instance.BuildEngine = new MockBuildEngine();
            instance.Server = @"localhost\SQLEXPRESS";


            // TODO: 임시
            // instance.Database = "model";
            instance.Database = databaseName;
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
            instance.ScriptTriggers = true;

            Assert.IsTrue(instance.Execute());
        }

        [Test]
        public void IncludeIfNotExists()
        {
            string thisClassName = this.GetType().FullName;
            string thisMethodName = thisClassName + "." + MethodBase.GetCurrentMethod().Name;
            string outputFileName = thisMethodName + ".sql";
            string outputDir = Path.Combine(BuildScripts.TestRootFolder, thisMethodName);

            RunIncludeIfNotExistsTestOnce(outputDir, outputFileName, true, "IF NOT EXISTS (SELECT");
        }

        private static void RunIncludeIfNotExistsTestOnce(string outputDir, string outputFileName, bool includeIfNotExists, string expectedString)
        {
            if (Directory.Exists(outputDir))
                Directory.Delete(outputDir, true);

            GenerateSqlScripts instance = CreateInstance();
            instance.OutputDir = outputDir;
            instance.OutputFileName = outputFileName;
            instance.Overwrite = true;
            instance.IncludeIfNotExists = includeIfNotExists;

            Assert.IsTrue(instance.Execute());
         
            string text = File.ReadAllText(instance.OutputFilePath);
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
            string outputDir = Path.Combine(BuildScripts.TestRootFolder, thisMethodName);

            RunSchemaQualifyTestOnce(outputDir, outputFileName, false, "CREATE TABLE [simple_table_1]");
            RunSchemaQualifyTestOnce(outputDir, outputFileName, true, "CREATE TABLE [dbo].[simple_table_1]");
        }

        private static void RunSchemaQualifyTestOnce(string outputDir, string outputFileName, bool schemaQualify, string expectedString)
        {
            if (Directory.Exists(outputDir))
                Directory.Delete(outputDir, true);

            GenerateSqlScripts instance = CreateInstance();
            instance.OutputDir = outputDir;
            instance.OutputFileName = outputFileName;
            instance.Overwrite = true;
            instance.SchemaQualify = schemaQualify;

            Assert.IsTrue(instance.Execute());

            string text = File.ReadAllText(instance.OutputFilePath);
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
            string outputDir = Path.Combine(BuildScripts.TestRootFolder, thisMethodName);
            string outputFilePath = Path.Combine(outputDir, outputFileName);

            RunOnceNoCollationTest(outputDir, outputFileName, true);

            string text = File.ReadAllText(outputFilePath);
            Assert.IsNotEmpty(text);

            Assert.IsFalse(
                text.Contains("COLLATE")
                );

            RunOnceNoCollationTest(outputDir, outputFileName, false);

            text = File.ReadAllText(outputFilePath);
            Assert.IsNotEmpty(text);

            Assert.IsTrue(
                text.Contains("COLLATE")
                );

        }

        private static void RunOnceNoCollationTest(string outputDir, string outputFileName, bool noCollation)
        {
            if (Directory.Exists(outputDir))
                Directory.Delete(outputDir, true);

            GenerateSqlScripts instance = CreateInstance();
            instance.Overwrite = true;
            instance.OutputDir = outputDir;
            instance.OutputFileName = outputFileName;
            instance.NoCollation = noCollation;
            //instance.CopyAllObjects = true;
            //instance.CopySchema = true;
            //instance.CopyAllDatabaseTriggers = true;

            Assert.IsTrue(instance.Execute());
            Assert.IsTrue(File.Exists(instance.OutputFilePath));
        }

        [Test]
        public void ScriptData()
        {
            string thisClassName = this.GetType().FullName;
            string thisMethodName = thisClassName + "." + MethodBase.GetCurrentMethod().Name;
            string outputFileName = thisMethodName + ".sql";
            string outputDir = Path.Combine(BuildScripts.TestRootFolder, thisMethodName);

            if (Directory.Exists(outputDir))
                Directory.Delete(outputDir, true);

            GenerateSqlScripts instance = CreateInstance();
            instance.Overwrite = true;
            instance.OutputDir = outputDir;
            instance.OutputFileName = outputFileName;
            instance.ScriptSchemas = false;
            instance.CopyData = true;

            Assert.IsTrue(instance.Execute());
            Assert.IsTrue(File.Exists(instance.OutputFilePath));
        }


        [Test]
        public void CombatArms()
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
            instance.ScriptTriggers = true;

            Assert.IsTrue(instance.Execute());
        }


        [Test]
        public void DatabaseNotExists()
        {
            GenerateSqlScripts instance = CreateInstanceWithDatabaseName("Wow");
            instance.Overwrite = true;

            instance.CopyData = true;
            instance.ExtendedProperties = true;
            instance.FullTextIndexes = true;
            instance.IncludeIfNotExists = true;
            instance.Indexes = true;
            instance.NoCollation = true;
            instance.SchemaQualify = true;
            instance.ScriptTriggers = true;

            Assert.IsFalse(instance.Execute());
        }
    }
}