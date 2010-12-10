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

    public class GetRegisteredGroups : AbstractTask
    {
        private List<TaskItem> _names;

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

        [Output]
        public ITaskItem[] Names
        {
            // get { return _names.ToArray(); }
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
