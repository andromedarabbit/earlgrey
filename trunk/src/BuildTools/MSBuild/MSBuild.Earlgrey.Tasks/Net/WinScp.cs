using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Net
{
    public class WinScp : AbstractToolTask
    {
        [Required]
        public string HostName { get; set; }
        public int Port { get; set; }

        public bool ProtocolFtp { get; set; }
        public bool ProtocolSFtp { get; set; }
        public bool ProtocolScp { get; set; }

        // [/implicit|explicitssl|explicittls]
        public bool Implicit { get; set; }
        public bool ExplicitSsl { get; set; }
        public bool ExplicitTls { get; set; }

        [Required]
        public string UserName { get; set; }
        public string Password { get; set; }

        public string HostKey { get; set; }

        public string ScriptText { get; set; }
        public string ScriptFile { get; set; }

        public bool Passive { get; set; }

        public int TimeoutSeconds { get; set; }
        // public string LogFile { get; set; }

        private string _internalScriptFile;

        private string UrlPrefix
        {
            get
            {
                if (ProtocolSFtp)
                    return "sftp://";

                if (ProtocolScp)
                    return "scp://";

                return "ftp://";
            }
        }

        private string SessionString
        {
            get 
            { 
                StringBuilder sb = new StringBuilder();
                sb.Append(UrlPrefix);

                if (string.IsNullOrEmpty(UserName) == false)
                    sb.Append(UserName);

                if (string.IsNullOrEmpty(Password) == false)
                {
                    sb.Append(":");
                    sb.Append(Password);
                }

                sb.Append("@");
                sb.Append(HostName);

                sb.Append(":");
                sb.Append(InternalPort);

                return sb.ToString();
            }
        }

        private int InternalPort
        {
            get
            {
                if (Port != 0)
                    return Port;

                if (ProtocolSFtp)
                    return 22;
                return 21;
            }
        }

        private string InternalScriptFile
        {
            get
            {
                if (_internalScriptFile == null)
                {
                    _internalScriptFile = Path.GetTempFileName();
                    PrepareScriptFile();
                }

                return _internalScriptFile;
            }
        }

        protected override bool ValidateParameters()
        {
            if(ProtocolFtp == false && ProtocolScp == false && ProtocolSFtp == false)
            {
                ProtocolFtp = true;
                Log.LogMessage(MessageImportance.Low, "Protocol 'FTP' will be used.");
            }

            if( 
                (ProtocolFtp && ProtocolScp && ProtocolSFtp) == true
                || (ProtocolFtp ^ ProtocolScp ^ ProtocolSFtp) == false
                )
            {
                Log.LogError("At least, and only one protocol can set 'true'!");
                return false;
            }
       
            if(ProtocolFtp == false && Passive == true)
            {
                Log.LogError("Passive mode is supported only when you use FTP protocol!");
                return false;
            }

            if(ProtocolFtp == true && (Implicit || ExplicitSsl || ExplicitTls) == true)
            {
                Log.LogError("Implicit/explicit hints can't be used with FTP protocol!");
                return false;
            }

            if (ProtocolSFtp == true || ProtocolScp == true)
            {
                if (
                    (Implicit && ExplicitSsl && ExplicitTls) == true
                    || (Implicit ^ ExplicitSsl ^ ExplicitTls) == false
                    )
                {
                    Implicit = true;
                    Log.LogMessage(MessageImportance.Low, "Protocol hint 'Implicit' will be used.");
                }
            }

            if(TimeoutSeconds < 0)
            {
                Log.LogError("Invalid timeout value!");
                return false;
            }

            if(string.IsNullOrEmpty(UserName) == true && string.IsNullOrEmpty(Password) == false)
            {
                Log.LogError("Password without a user name!");
                return false;
            }

            if(string.IsNullOrEmpty(ScriptFile) == false && string.IsNullOrEmpty(ScriptText) == false)
            {
                Log.LogError("You can't set both of 'ScriptFile' and 'ScriptText' at the same time!");
                return false;
            }

            if (string.IsNullOrEmpty(ScriptFile) == true && string.IsNullOrEmpty(ScriptText) == true)
            {
                Log.LogError("At least one of 'ScriptFile' and 'ScriptText' should be set!");
                return false;
            }

            return base.ValidateParameters();
        }

        protected override string ToolName
        {
            get { return "WinSCP.com"; }
        }

        protected override string GenerateCommandLineCommands()
        {
            Debug.Assert(string.IsNullOrEmpty(InternalScriptFile) == false);

            CommandLineBuilder builder = new CommandLineBuilder();
            builder.AppendSwitch("/console");
            builder.AppendSwitchIfNotNull("/script=", InternalScriptFile);

            if(TimeoutSeconds > 0)
                builder.AppendSwitchIfNotNull("/timeout=", TimeoutSeconds.ToString());
           
            if(Passive)
                builder.AppendSwitch("/passive");

            if(Implicit)
                builder.AppendSwitch("/implicit");

            if (ExplicitSsl)
                builder.AppendSwitch("/explicitssl");

            if (ExplicitTls)
                builder.AppendSwitch("/explicittls");

            string commands = base.GenerateCommandLineCommands();
            return commands + " " + builder.ToString();
        }


        protected override string GenerateFullPathToTool()
        {
            string fullPathToTool = base.GenerateFullPathToTool();

            if (FileSearch.Exists(fullPathToTool))
                return fullPathToTool;

            return ToolsSearch.FindExternalTool(ToolName);
        }

        private string InternalScriptText
        {
            get
            {
                if (string.IsNullOrEmpty(ScriptFile) == false)
                {
                    return File.ReadAllText(ScriptFile);
                }
                return ScriptText;
            }
        }

        private void PrepareScriptFile()
        {
            Debug.Assert(string.IsNullOrEmpty(_internalScriptFile) == false);

            using (
                FileStream fs = File.Open(_internalScriptFile, FileMode.Open, FileAccess.ReadWrite, FileShare.ReadWrite))
            {
                using (StreamWriter sw = new StreamWriter(fs, Encoding.UTF8))
                {
                    sw.Write("open ");
                    sw.WriteLine(SessionString);
                    sw.Write(InternalScriptText);
                    sw.Write("close");
                    sw.Write("exit");
                }
            }
        }

        public override bool Execute()
        {
            return base.Execute();
        }
        
    }
}
