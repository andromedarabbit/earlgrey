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
    public abstract class AbstractWinScp : AbstractToolTask
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

        public bool Passive { get; set; }

        public int TimeoutSeconds { get; set; }
        // public string LogFile { get; set; }

        public bool Echo { get; set; }
        public bool Confirm { get; set; }

        public string Batch { get; set; }
        
        private string _internalScriptFile;
        private string _options;

        private string UrlPrefix
        {
            get
            {
                if (ProtocolSFtp)
                {
                    if (Implicit)
                        return "ftps://";
                    return "sftp://";
                }

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

                if (string.IsNullOrEmpty(HostKey) == false)
                {
                    sb.Append(" -hostkey=\"");
                    sb.Append(HostKey);
                    sb.Append("\"");
                }


                if (Passive)
                    sb.Append(" -passive");

                if (Implicit)
                    sb.Append(" -implicit");

                if (ExplicitSsl)
                    sb.Append(" -explicitssl");

                if (ExplicitTls)
                    sb.Append(" -explicittls");

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

                if (Implicit)
                    return 990;

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

        protected AbstractWinScp()
        {
            this.Echo = true;
            this.Batch = "abort";
        }

        private bool IsFtpS
        {
            get { return ProtocolFtp && (Implicit || ExplicitSsl || ExplicitTls) == true; }
        }

        protected override bool ValidateParameters()
        {
            if (ProtocolFtp == false && ProtocolScp == false && ProtocolSFtp == false)
            {
                ProtocolFtp = true;
                Log.LogMessage(MessageImportance.Low, "Protocol 'FTP' will be used.");
            }

            if (
                (ProtocolFtp && ProtocolScp && ProtocolSFtp) == true
                || (ProtocolFtp ^ ProtocolScp ^ ProtocolSFtp) == false
                )
            {
                Log.LogError("At least, and only one protocol can set 'true'!");
                return false;
            }

            if (ProtocolFtp == false && (Implicit || ExplicitSsl || ExplicitTls) == true)
            {
                Log.LogError("Implicit/explicit hints can't be used with FTP protocol!");
                return false;
            }

            if ((ProtocolSFtp || IsFtpS) && Passive == true)
            {
                Log.LogError("Passive mode is supported only when you use FTP protocol!");
                return false;
            }

            if (TimeoutSeconds < 0)
            {
                Log.LogError("Invalid timeout value!");
                return false;
            }

            if (string.IsNullOrEmpty(UserName) == true && string.IsNullOrEmpty(Password) == false)
            {
                Log.LogError("Password without a user name!");
                return false;
            }

            if (
                Batch.Equals("on", StringComparison.CurrentCultureIgnoreCase) == false
                && Batch.Equals("abort", StringComparison.CurrentCultureIgnoreCase) == false
                && Batch.Equals("off", StringComparison.CurrentCultureIgnoreCase) == false
                && Batch.Equals("continue", StringComparison.CurrentCultureIgnoreCase) == false
                )
            {
                Log.LogError("Possible batch options are on | off | abort | continue");
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

            if (TimeoutSeconds > 0)
                builder.AppendSwitchIfNotNull("/timeout=", TimeoutSeconds.ToString());

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
                StringBuilder sb = new StringBuilder();
                sb.AppendLine(Options);
                sb.Append("open ");
                sb.AppendLine(SessionString);
                sb.AppendLine(Scripts);
                sb.AppendLine("close");
                sb.AppendLine("exit");
                return sb.ToString();
            }
        }

        protected string Options
        {
            get
            {
                if (_options != null)
                    return _options;


                StringBuilder sb = new StringBuilder();
                if (Echo)
                    sb.AppendLine("option echo on");
                else
                    sb.AppendLine("option echo off");

                sb.Append("option batch ");
                sb.AppendLine(Batch);


                if(Confirm)
                    sb.AppendLine("option confirm on");
                else 
                    sb.AppendLine("option confirm off");


                _options = sb.ToString();
                return _options;
            }
        }

        protected abstract string Scripts
        {
            get;
        }

        private void PrepareScriptFile()
        {
            Debug.Assert(string.IsNullOrEmpty(_internalScriptFile) == false);

            using (
                FileStream fs = File.Open(_internalScriptFile, FileMode.Open, FileAccess.ReadWrite, FileShare.ReadWrite))
            {
                using (StreamWriter sw = new StreamWriter(fs, Encoding.UTF8))
                {
                    sw.WriteLine(InternalScriptText);
                }
            }
        }

        public override bool Execute()
        {
            Log.LogMessage(MessageImportance.High, InternalScriptText);
            return base.Execute();
        }


        protected override void LogEventsFromTextOutput(string singleLine, MessageImportance messageImportance)
        {
            singleLine = singleLine.Replace("Error", "Failure");
        
            base.LogEventsFromTextOutput(singleLine, messageImportance);
        }
    }
}
