using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Tests
{
    internal class TaskUtility
    {
        public static string  BaseDirectory
        {
            get { return AppDomain.CurrentDomain.BaseDirectory; }
        }
    }
}
