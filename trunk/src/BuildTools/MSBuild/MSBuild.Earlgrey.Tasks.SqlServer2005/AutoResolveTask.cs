using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Reflection;
using Earlgrey;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{
    public abstract class AutoResolveTask : AbstractTask
    {
        static AutoResolveTask()
        {
            AppDomain.CurrentDomain.AssemblyResolve += new ResolveEventHandler(MyResolveEventHandler);
        }

        // TODO: Earlgrey.TaskUtility 와 중복 코드
        private static string ThisAssemblyDirectory
        {
            get
            {
                Uri currentAssemblyUri = new Uri(System.Reflection.Assembly.GetExecutingAssembly().CodeBase);
                string currentAssemblyPath = currentAssemblyUri.LocalPath;
                return Directory.GetParent(currentAssemblyPath).FullName;
            }
        }

        private static string OSBitnessKeyword
        {
            get
            {
                if (EnvironmentHelper.Is64BitOperatingSystem() == true)
                    return "x64";
                return "x86";
            }
        }
        private static Assembly MyResolveEventHandler(object sender, ResolveEventArgs args)
        {
            //This handler is called only when the common language runtime tries to bind to the assembly and fails.
            

            string assemblyNameMissing = args.Name.Substring(0, args.Name.IndexOf(","));

            string assemblyPath = Path.Combine(ThisAssemblyDirectory, @"ExternalLibs\Microsoft SQL Server 2005 Management Objects\" + OSBitnessKeyword);
            
            assemblyPath = Path.Combine(assemblyPath, assemblyNameMissing + ".dll");

            if (File.Exists(assemblyPath) == false)
                return null;

            //Load the assembly from the specified path. 					
            Assembly assemblyFound = Assembly.LoadFrom(assemblyPath);

            //Return the loaded assembly.
            return assemblyFound;
        }
    }
}
