using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{
    public class UnregisterServer : AutoResolveTask
    {
        private string _name;
        private string _path;

        public UnregisterServer()
        {
            _name = null;
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
                DeleteServer(_name);                
                return true;
            }

            string parentGroupPath = RegisteredServerHelper.GetParentGroupPath(_path);
            ServerGroup parentGroup = RegisteredServerHelper.FindParentGroup(parentGroupPath);
            if(parentGroup == null)
                return false;

            DeleteServer(parentGroup, _name);
            return true;
        }

        private static void DeleteServer(string name)
        {
            var server = new RegisteredServer(name);
            server.Drop();            
        }

        private static void DeleteServer(ServerGroup parent, string name)
        {
            Debug.Assert(parent != null);

            var server = new RegisteredServer(parent, name);
            server.Drop();
        }

        [Required]
        public string Name
        {
            get { return _name; }
            set { _name = value.Trim(); }
        }

        public string Path
        {
            get { return _name; }
            set { _path = value.Trim(); }
        }

    }
}
