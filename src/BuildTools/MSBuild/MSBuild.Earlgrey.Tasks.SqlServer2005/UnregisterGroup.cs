using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{

    public class UnregisterGroup : AbstractTask
    {
        private string _name;
        private string _path;

        public UnregisterGroup()
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

            if (_path == string.Empty)
            {
                DeleteGroup(_name);
                return true;
            }

            try
            {
                ServerGroup group = RegisteredServerHelper.FindGroup(_path, _name);
                group.Drop();
            }
            catch(ApplicationException appEx)
            {
                Log.LogError(appEx.Message);
                return false;
            }
           
            return true;
        }

        private static void DeleteGroup(string name)
        {
            var group = new ServerGroup(name);
            group.Drop();
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
