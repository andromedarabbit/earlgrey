using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using Microsoft.SqlServer.Management.Smo;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{
    // \ref http://www.sqldbatips.com/print.asp?ID=45

    /// <summary>
    /// Getting all the server groups registered in SQL Server Management Studio 2005.
    /// </summary>
    /// <example>
    /// <code title="Enumerate server groups registered in SQL Server Management Studio 2005." lang="xml" source=".\Samples\msbuild-GetRegisteredGroups.xml" />
    /// </example>
    /// <remarks></remarks>
    public class GetRegisteredGroups : AutoResolveTask
    {
        private List<TaskItem> _names;

        /// <summary>
        /// Initializes a new instance of the <see cref="GetRegisteredGroups"/> class.
        /// </summary>
        /// <remarks></remarks>
        public GetRegisteredGroups()
        {            
            _names = null;
        }

        protected override bool ValidateParameters()
        {
            return true;
        }

        protected override bool ExecuteCommand()
        {
            _names = GetNames();
            return true;
        }
        
        /// <summary>
        /// [Output] Gets the all the names of server groups registered in SQL Server Management Studio 2005.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] Names
        {
            get
            {
                Trace.Assert(_names != null);
                return _names.ToArray();
            }
        }

        private static List<TaskItem> GetNames()
        {   
            List<TaskItem> names = new List<TaskItem>();
            ServerGroupCollection groups = SmoApplication.SqlServerRegistrations.ServerGroups;

            AddGroupNames(groups, names);

            return names;
        }

        private static void AddGroupNames(ServerGroupCollection groups, List<TaskItem> names)
        {
            foreach(ServerGroup group in groups)
            {
                TaskItem name = new TaskItem(group.Name);
                name.SetMetadata("Path", group.Path);
                name.SetMetadata("Description", group.Description);                
                names.Add(name);

                if (group.ServerGroups == null)
                    continue;
                
                AddGroupNames(group.ServerGroups, names);
            }
        }

       
    }
}
