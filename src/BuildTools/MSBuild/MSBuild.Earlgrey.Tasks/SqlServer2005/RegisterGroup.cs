using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.SqlServer.Management.Smo;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;


namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{

    public class RegisterGroup : AbstractTask
    {
        private string _name;
        private string _description;
        private string _path;

        public RegisterGroup()
        {
            _name = null;
            _description = string.Empty;
            _path = string.Empty;
        }

        protected override bool ValidateParameters()
        {
            if(string.IsNullOrEmpty(_name))
            {
                Log.LogError("Name is required!", _name);
                return false;
            }
            return true;
        }

        protected override bool ExecuteCommand()
        {
            Trace.Assert(string.IsNullOrEmpty(_name) == false);

            if(_path == string.Empty)
            {
                CreateGroup(_name, _description);                
                return true;
            }

            ServerGroupCollection groups = SmoApplication.SqlServerRegistrations.ServerGroups;
            ServerGroup parentGroup = FindParentGroup(groups);
            if(parentGroup == null)
                return false;

            CreateGroup(parentGroup, _name, _description);
            return true;
        }

        private static void CreateGroup(string name, string description)
        {
            var group = new ServerGroup(name);
            group.Description = description;
            group.Create();
        }

        private static void CreateGroup(ServerGroup parent, string name, string description)
        {
            Debug.Assert(parent != null);

            var group = new ServerGroup(parent, name);
            group.Description = description;
            group.Create();
        }

        private ServerGroup FindParentGroup(ServerGroupCollection groups)
        {
            if (groups == null)
                return null;

            foreach(ServerGroup group in groups)
            {
                // if (group.Parent == null)
                //    continue;
                
                if(group.Path.Equals(_path, StringComparison.CurrentCultureIgnoreCase))
                    return group.Parent;

                var result = FindParentGroup(group.ServerGroups);
                if(result != null)
                    return result;
            }
            return null;
        }

        [Required]
        public string Name
        {
            get { return _name; }
            set { _name = value.Trim(); }
        }

        public string Description 
        {
            get { return _description; }
            set { _description = value.Trim(); }
        }

        public string Path
        {
            get { return _name; }
            set { _path = value.Trim(); }
        }

    }
}
