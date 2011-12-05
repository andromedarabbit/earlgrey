using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{
    /// <summary>
    /// Unregister a server group from SQL Server Management Studio 2005.
    /// </summary>
    /// <example>
    /// <code title="Register and unregister the target server group in SQL Server Management Studio 2005." lang="xml" source=".\Samples\msbuild-RegisterGroup-UnregisterGroup.xml" />
    /// </example>
    /// <remarks></remarks>
    public class UnregisterGroup : AutoResolveTask
    {
        private string _name;
        private string _path;

        /// <inheritdoc />
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

        /// <summary>
        /// [Required] Gets or sets the name of the server group.
        /// </summary>
        /// <value>The server group name.</value>
        /// <remarks></remarks>
        [Required]
        public string Name
        {
            get { return _name; }
            set { _name = value.Trim(); }
        }

        /// <summary>
        /// Gets or sets the path where the target group <see cref="Name"/> is on.
        /// </summary>
        /// <value>If you do not set <see cref="Path"/>, the target server group will be assumed as the root.</value>
        /// <example>
        /// <code>ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Group2'']</code>
        /// </example>
        /// <remarks>
        /// </remarks>
        public string Path
        {
            get { return _name; }
            set { _path = value.Trim(); }
        }

    }
}
