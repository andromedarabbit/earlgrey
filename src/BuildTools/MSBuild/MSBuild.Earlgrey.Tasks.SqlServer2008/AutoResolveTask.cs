using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Reflection;
using Earlgrey;

namespace MSBuild.Earlgrey.Tasks.SqlServer2008
{
    /// <summary>
    /// Resolve the necessary assembly files when Microsoft SQL Server 2008 Management Objects is installed in GAC.
    /// </summary>
    /// <remarks></remarks>
    public abstract class AutoResolveTask : AbstractTask
    {
        static AutoResolveTask()
        {
            AppDomain.CurrentDomain.AssemblyResolve += new ResolveEventHandler(MyResolveEventHandler);
        }

        // TODO: Earlgrey.TaskUtility 와 중복 코드
        /// <summary>
        /// Gets the directory where this assembly resides in.
        /// </summary>
        /// <remarks></remarks>
        private static string ThisAssemblyDirectory
        {
            get
            {
                Uri currentAssemblyUri = new Uri(System.Reflection.Assembly.GetExecutingAssembly().CodeBase);
                string currentAssemblyPath = currentAssemblyUri.LocalPath;
                return Directory.GetParent(currentAssemblyPath).FullName;
            }
        }

        private static Assembly MyResolveEventHandler(object sender, ResolveEventArgs args)
        {
            // This handler is called only when the common language runtime tries to bind to the assembly and fails.
            string assemblyNameMissing = args.Name.Substring(0, args.Name.IndexOf(","));

            string assemblyPath = Path.Combine(ThisAssemblyDirectory, @"ExternalLibs\Microsoft SQL Server 2008 Management Objects\x64");

            assemblyPath = Path.Combine(assemblyPath, assemblyNameMissing + ".dll");

            if (File.Exists(assemblyPath) == false)
                return null;

            // Load the assembly from the specified path. 					
            Assembly assemblyFound = Assembly.LoadFrom(assemblyPath);

            // Return the loaded assembly.
            return assemblyFound;
        }
    }
}
