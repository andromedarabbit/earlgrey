using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks
{
    internal static class TaskItemHelper
    {
        public static bool FileExists(ITaskItem taskItem)
        {
            if (taskItem == null)
                return false;

            return File.Exists(GetFullPath(taskItem));
        }

        public static bool FolderExists(ITaskItem taskItem)
        {
            if (taskItem == null)
                return false;

            return Directory.Exists(GetFullPath(taskItem));
        }

        public static void FileDelete(ITaskItem taskItem)
        {
            if (taskItem == null)
                throw new ArgumentNullException();

            File.Delete(GetFullPath(taskItem));
        }


        public static string GetFullPath(ITaskItem taskItem)
        {
            return taskItem.GetMetadata("FullPath");
        }
    }
}
