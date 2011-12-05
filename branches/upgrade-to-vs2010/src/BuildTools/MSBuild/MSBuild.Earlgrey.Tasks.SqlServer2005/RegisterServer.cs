using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{
    /// <summary>
    /// Register the target server on the target server group into SQL Server Management Studio 2005.
    /// </summary>
    /// <example>
    ///   <code title="Register and unregister the target server in SQL Server Management Studio 2005." lang="xml" source=".\Samples\msbuild-RegisterServer-UnregisterServer.xml"/>
    /// </example>
    /// <remarks></remarks>
    public class RegisterServer : AutoResolveTask
    {
        private string _name;
        private string _serverInstance;
        private string _description;
        private string _path;

        private string _login;
        private bool _loginSecure;
        private string _password;

        private bool _createRecursively;

        /// <inheritdoc />
        public RegisterServer()
        {
            _name = null;
            _serverInstance = string.Empty;
            _description = string.Empty;
            _path = string.Empty;
            _login = string.Empty;
            _loginSecure = true;
            _password = string.Empty;

            _createRecursively = false;
        }

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            if (string.IsNullOrEmpty(_name))
            {
                Log.LogError("Name is required!", _name);
                return false;
            }

            if (string.IsNullOrEmpty(_serverInstance))
            {
                Log.LogError("ServerInstance is required!", _serverInstance);
                return false;
            }

            return true;
        }

        /// <inheritdoc />
        protected override bool ExecuteCommand()
        {
            Trace.Assert(string.IsNullOrEmpty(_name) == false);

            if (_path == string.Empty)
            {
                CreateServer();
                return true;
            }

            if (_createRecursively == true)
            {
                if (CreateGroupsRecursively() == false) 
                    return false;
            }

            ServerGroup parentGroup = RegisteredServerHelper.FindGroup(_path);
            CreateServer(parentGroup);
            return true;
        }

        private bool CreateGroupsRecursively()
        {
            if (string.IsNullOrEmpty(_path))
                return true;

            string parentGroupPath = RegisteredServerHelper.GetParentGroupPath(_path);
            string lastChildGroupPath = RegisteredServerHelper.GetLastChildGroupName(_path);


            RegisterGroup registerGroup = new RegisterGroup();
            registerGroup.BuildEngine = this.BuildEngine;
            registerGroup.CreateRecursively = true;
            registerGroup.Path = parentGroupPath;
            registerGroup.Name = lastChildGroupPath;

            if(registerGroup.Execute() == false)
            {
                return false;
            }
            return true;
        }

    
        private void CreateServer()
        {
            RegisteredServer server = new RegisteredServer(_name);
            SetRegisteredServer(server);
            server.Create();
        }

        private void CreateServer(ServerGroup parent)
        {
            Debug.Assert(parent != null);
            
            RegisteredServer server = new RegisteredServer(parent, _name);            
            SetRegisteredServer(server);
            server.Create();
        }

        private void SetRegisteredServer(RegisteredServer server)
        {
            server.ServerInstance = _serverInstance;
            server.Description = _description;
            server.Login = _login;
            server.LoginSecure = _loginSecure;
            
            // http://www.vcskicks.com/secure-string.php
            SecureString password = new SecureString();
            for (int i = 0; i < _password.Length; i++)
            {
                password.AppendChar(_password[i]);
            }
            password.MakeReadOnly();
            server.SecurePassword = password;
        }

        /// <summary>
        /// [Required] Gets or sets the server name.
        /// </summary>
        /// <value>The server name.</value>
        /// <remarks></remarks>
        [Required]
        public string Name
        {
            get { return _name; }
            set { _name = value.Trim(); }
        }

        /// <summary>
        /// Gets or sets the path where the target server <see cref="Name"/> is on.
        /// </summary>
        /// <value>If you do not set <see cref="Path"/>, the target server will be created on the root.</value>
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
        /// Gets or sets a textual description of the referenced registered server.
        /// </summary>
        /// <value>A String value that specifies the textual description of the reference registered server.</value>
        /// <remarks></remarks>
        public string Description
        {
            get { return _description; }
            set { _description = value.Trim(); }
        }

        /// <summary>
        /// Gets or sets the instance of SQL Server that is the registered server. 
        /// </summary>
        /// <value>A String value that specifies the instance of SQL Server that is the registered server.</value>
        /// <remarks></remarks>
        [Required]
        public string ServerInstance
        {
            get { return _serverInstance; }
            set { _serverInstance = value.Trim(); }
        }

        /// <summary>
        /// Gets or sets the login used to establish a connection with the registered server. 
        /// </summary>
        /// <value>A String value that specifies the used to establish a connection with the registered server.</value>
        /// <remarks></remarks>
        public string Login
        {
            get { return _login; }
            set { _login = value.Trim(); }
        }

        /// <summary>
        /// Gets or sets the Boolean property value that specifies whether the connection is authenticated by Windows Authentication. 
        /// </summary>
        /// <value>
        /// A Boolean value that specifies whether the connection is authenticated by Windows Authentication.
        /// If <c>true</c> (default), the connection is authenticated by Windows Authentication.
        /// If <c>false</c>, the connection is authenticated by using SQL Server standard authentication.
        /// </value>
        /// <remarks></remarks>
        public bool LoginSecure
        {
            get { return _loginSecure; }
            set { _loginSecure = value; }
        }

        /// <summary>
        /// Gets or sets the password used with SQL Server standard authentication to establish a connection to the registered server. 
        /// </summary>
        /// <value>The password.</value>
        /// <remarks></remarks>
        public string Password
        {
            get { return _password; }
            set { _password = value; }
        }

        /// <summary>
        /// Gets or sets a value indicating whether to create recursively.
        /// </summary>
        /// <value>Set <c>true</c> if you want to create the server <see cref="Name"/> recursively; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool CreateRecursively
        {
            get { return _createRecursively; }
            set { _createRecursively = value; }
        }
    }
}
