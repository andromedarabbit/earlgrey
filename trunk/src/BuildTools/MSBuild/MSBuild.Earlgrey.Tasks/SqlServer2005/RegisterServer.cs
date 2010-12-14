using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.SqlServer.Management.Smo;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{
    public class RegisterServer : AbstractTask
    {
        private string _name;
        private string _serverInstance;
        private string _description;
        private string _path;

        private string _login;
        private bool _loginSecure;
        private string _password;

        private bool _createRecursively;

        public  RegisterServer()
        {
            _name = null;
            _description = string.Empty;
            _path = string.Empty;
            _login = string.Empty;
            _loginSecure = true;
            _password = string.Empty;

            _createRecursively = false;
        }


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
                RegisterGroup registerGroup = new RegisterGroup();
                registerGroup.BuildEngine = this.BuildEngine;
                registerGroup.CreateRecursively = true;
                registerGroup.Path = _path;
                registerGroup.Name = Name;

                if(registerGroup.Execute() == false)
                {
                    return false;
                }
            }
            ServerGroup parentGroup = RegisteredServerHelper.FindParentGroup(_path);
            if (parentGroup == null)
                return false;


            CreateServer(parentGroup);
            return true;
        }

        //private string ParentPath
        //{
        //    get
        //    {
        //        Trace.Assert(string.IsNullOrEmpty(_path) == false);

        //        // "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Group2'']";
        //        int index = _path.LastIndexOf("ServerGroup[@Name=''", StringComparison.CurrentCultureIgnoreCase) + 1;
        //        string 
        //    }
        //}

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
            for (int i = 0; i < password.Length; i++)
            {
                password.AppendChar(_password[i]);
            }
            password.MakeReadOnly();
            server.SecurePassword = password;
            
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

        [Required]
        public string ServerInstance
        {
            get { return _serverInstance; }
            set { _serverInstance = value.Trim(); }
        }
        
        public string Description
        {
            get { return _description; }
            set { _description = value.Trim(); }
        }

        public string Login
        {
            get { return _login; }
            set { _login = value.Trim(); }
        }

        public bool LoginSecure
        {
            get { return _loginSecure; }
            set { _loginSecure = value; }
        }

        public string Password
        {
            get { return _password; }
            set { _password = value; }
        }

        public bool CreateRecursively
        {
            get { return _createRecursively; }
            set { _createRecursively = value; }
        }
    }
}
