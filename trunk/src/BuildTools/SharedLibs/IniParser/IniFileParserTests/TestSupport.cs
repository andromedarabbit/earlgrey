using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;

namespace IniFileParserTests
{
    internal static class TestSupport
    {
        private static readonly string _TestRootFolder;
        private static readonly string _TestScriptsFolder;

        static TestSupport()
        {
            var assembly = Assembly.GetExecutingAssembly();
            string thisAssemblyName = assembly.GetName().Name;

            _TestRootFolder = Path.Combine(
                ThisAssemblyDirectory
                , @"Temp"
                );
            _TestScriptsFolder = Path.Combine(
                _TestRootFolder
                , thisAssemblyName
                );
        }

        public static string TestRootFolder
        {
            get { return _TestRootFolder; }
        }

        public static string TestScriptsFolder
        {
            get { return _TestScriptsFolder; }
        }

        public static string ThisAssemblyDirectory
        {
            get
            {
                Uri currentAssemblyUri = new Uri(System.Reflection.Assembly.GetExecutingAssembly().CodeBase);
                string currentAssemblyPath = currentAssemblyUri.LocalPath;
                return Directory.GetParent(currentAssemblyPath).FullName;
            }
        }
    }
}
