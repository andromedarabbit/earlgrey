using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Reflection;

namespace MSBuild.Earlgrey.Tasks.SqlServer2008
{
    public abstract class AutoResolveTask : AbstractTask
    {
        static AutoResolveTask()
        {
            AppDomain.CurrentDomain.AssemblyResolve += new ResolveEventHandler(MyResolveEventHandler);
        }

        private static Assembly MyResolveEventHandler(object sender, ResolveEventArgs args)
        {
            //This handler is called only when the common language runtime tries to bind to the assembly and fails.

            //Retrieve the list of referenced assemblies in an array of AssemblyName.
            Assembly MyAssembly, objExecutingAssemblies;
            string strTempAssmbPath = "";

            objExecutingAssemblies = Assembly.GetExecutingAssembly();
            AssemblyName[] arrReferencedAssmbNames = objExecutingAssemblies.GetReferencedAssemblies();

            //Loop through the array of referenced assembly names.
            foreach (AssemblyName strAssmbName in arrReferencedAssmbNames)
            {
                //Check for the assembly names that have raised the "AssemblyResolve" event.
                if (strAssmbName.FullName.Substring(0, strAssmbName.FullName.IndexOf(",")) == args.Name.Substring(0, args.Name.IndexOf(",")))
                {
                    //Build the path of the assembly from where it has to be loaded.				
                    // strTempAssmbPath = "C:\\Myassemblies\\" + args.Name.Substring(0, args.Name.IndexOf(",")) + ".dll";

                    strTempAssmbPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, @"ExternalLibs\Microsoft SQL Server 2008 Management Objects");
                    strTempAssmbPath = Path.Combine(strTempAssmbPath, args.Name.Substring(0, args.Name.IndexOf(",")) + ".dll");


                    break;
                }

            }
            //Load the assembly from the specified path. 					
            MyAssembly = Assembly.LoadFrom(strTempAssmbPath);

            //Return the loaded assembly.
            return MyAssembly;	
        }
    }
}
