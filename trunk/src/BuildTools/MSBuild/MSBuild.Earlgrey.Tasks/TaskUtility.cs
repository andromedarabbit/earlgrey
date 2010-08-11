using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey.Tasks
{
    internal class TaskUtility
    {
        public static string BaseDirectory
        {
            get { return AppDomain.CurrentDomain.BaseDirectory; }
        }

        //! \todo 매번 다시 계산하는 건 낭비!
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
