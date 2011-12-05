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
    /// Register a server group into SQL Server Management Studio 2005.
    /// </summary>
    /// <example>
    /// <code title="Register and unregister the target server group in SQL Server Management Studio 2005." lang="xml" source=".\Samples\msbuild-RegisterGroup-UnregisterGroup.xml" />
    /// </example>
    /// <remarks></remarks>
    public class RegisterGroup : AutoResolveTask
    {
        private string _name;
        private string _description;
        private string _path;

        private bool _createRecursively;

        /// <inheritdoc />
        public RegisterGroup()
        {
            _name = null;
            _description = string.Empty;
            _path = string.Empty;

            _createRecursively = false;
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
            
            if (_createRecursively == false)
            {
                return CreateGroup();
            }

            // "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Group2'']";
            CreateGroupsRecursively();

            return true;
        }

        private string FullPath
        {
            get
            {
                if (string.IsNullOrEmpty(_path))
                    return "ServerGroup[@Name=''" + _name + "'']";

                return _path + "/ServerGroup[@Name=''" + _name + "'']";
            }
        }

        private void CreateGroupsRecursively()
        {
            string[] pathStrings = FullPath.Split('/');
            string path = string.Empty;
            
            ServerGroup parentGroup = null;
            
            foreach (var pathString in pathStrings)
            {
                Trace.Assert(pathString.StartsWith("ServerGroup"));
                
                // path = path + pathString;
                string groupName = pathString.Replace("ServerGroup[@Name=''", string.Empty).Replace("'']", string.Empty);
                    ;

                ServerGroup group = null;
                if (RegisteredServerHelper.FindGroup(path, groupName, out group) == false)
                {
                    if (parentGroup == null)
                        group = new ServerGroup(groupName);
                    else
                        group = new ServerGroup(parentGroup, groupName);

                    group.Create();
                }

                path = path + "/" + pathString;
                parentGroup = group;
            }
        }

        private bool CreateGroup()
        {
            try
            {
                ServerGroup parentGroup = RegisteredServerHelper.FindParentGroup(_path);
                CreateGroup(parentGroup, _name, _description);
                return true;
            }
            catch (ApplicationException appEx)
            {
                Log.LogError(appEx.Message);
                return false;
            }                       
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
        /// Gets or sets the description.
        /// </summary>
        /// <value>The description.</value>
        /// <remarks></remarks>
        public string Description 
        {
            get { return _description; }
            set { _description = value.Trim(); }
        }

        /// <summary>
        /// Gets or sets the path where the target group <see cref="Name"/> is on.
        /// </summary>
        /// <value>If you do not set <see cref="Path"/>, the target server group will be created on the root.</value>
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

        /// <summary>
        /// Gets or sets a value indicating whether to create recursively.
        /// </summary>
        /// <value>Set <c>true</c> if you want to create the server group <see cref="Name"/> recursively; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool CreateRecursively
        {
            get { return _createRecursively; }
            set { _createRecursively = value; }
        }
    }
}
