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
    /// <summary>
    /// Provides the interface for classes which operates on <a href="http://winscp.net">WinSCP</a>.
    /// </summary>
    /// <remarks>
    /// <a href="http://winscp.net">WinSCP</a> is the famous and featureful FTP/SFTP client for Windows. 
    /// It has an advantage of automation scripting over FileZila, which is also the well-known open-source one.
    /// </remarks>
    public abstract class AbstractWinScp : AbstractToolTask
    {
        /// <summary>
        /// Gets or sets the name of the destination FTP host.
        /// </summary>
        /// <value>The name of the host.</value>
        /// <remarks></remarks>
        [Required]
        public string HostName { get; set; }

        /// <summary>
        /// Gets or sets the port.
        /// </summary>
        /// <value>The port.</value>
        /// <remarks></remarks>
        public int Port { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to use the protocol FTP.
        /// </summary>
        /// <value>Set <c>true</c> if you want to connect to the <see cref="HostName"/> using the protocol FTP; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// See <a href="http://winscp.net/eng/docs/protocols">Supported Transfer Protocols</a> for more details.
        /// </remarks>
        public bool ProtocolFtp { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to use the protocol SFTP.
        /// </summary>
        /// <value>Set <c>true</c> if you want to connect to the <see cref="HostName"/> using the protocol SFTP; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// See <a href="http://winscp.net/eng/docs/protocols">Supported Transfer Protocols</a> for more details.
        /// </remarks>
        public bool ProtocolSFtp { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to use the protocol SCP.
        /// </summary>
        /// <value>Set <c>true</c> if you want to connect to the <see cref="HostName"/> using the protocol SCP; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// See <a href="http://winscp.net/eng/docs/protocols">Supported Transfer Protocols</a> for more details.
        /// </remarks>
        public bool ProtocolScp { get; set; }

        // [/implicit|explicitssl|explicittls]

        /// <summary>
        /// Gets or sets a value indicating whether to use the [implicit] invocation method.
        /// </summary>
        /// <value>Set <c>true</c> if you need [implicit] invocation method; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// See <a href="http://winscp.net/eng/docs/ftps">FTPS</a> for more details of the difference between two invocation methods 'implicit'/'explicit'.
        /// </remarks>
        public bool Implicit { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to use the [explicit SSL] invocation method.
        /// </summary>
        /// <value>Set <c>true</c> if you need [explicit SSL] invocation method; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// See <a href="http://winscp.net/eng/docs/ftps">FTPS</a> for more details of the difference between two invocation methods 'implicit'/'explicit'.
        /// </remarks>
        public bool ExplicitSsl { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to use the [explicit TLS] invocation method.
        /// </summary>
        /// <value>Set <c>true</c> if you need [explicit TLS] invocation method; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// See <a href="http://winscp.net/eng/docs/ftps">FTPS</a> for more details of the difference between two invocation methods 'implicit'/'explicit'.
        /// </remarks>
        public bool ExplicitTls { get; set; }

        /// <summary>
        /// [Required] Gets or sets the name of the user.
        /// </summary>
        /// <value>The name of the user.</value>
        /// <remarks></remarks>
        [Required]
        public string UserName { get; set; }

        /// <summary>
        /// Gets or sets the password.
        /// </summary>
        /// <value>The password.</value>
        /// <remarks></remarks>
        public string Password { get; set; }

        /// <summary>
        /// Gets or sets the host key.
        /// </summary>
        /// <value>The host key.</value>
        /// <remarks></remarks>
        public string HostKey { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to use the passive transfer mode.
        /// </summary>
        /// <value>Set <c>true</c> if you want to use the passive transfer mode; otherwise, <c>false</c>.</value>
        /// <remarks>FTP protocol only</remarks>
        public bool Passive { get; set; }

        /// <summary>
        /// Gets or sets the timeout in seconds.
        /// </summary>
        /// <value>The timeout in seconds.</value>
        /// <remarks></remarks>
        public int TimeoutSeconds { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to enable echoing of command being executed.
        /// </summary>
        /// <value>Set <c>true</c> if you want to enable echoing of command being executed; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// See <a href="http://winscp.net/eng/docs/scriptcommand_option">Script Commands - option</a> for more details.
        /// </remarks>
        public bool Echo { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to toggle confirmations for overwrite, etc.
        /// </summary>
        /// <value>Set <c>true</c> if you want to toggle confirmations; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// See <a href="http://winscp.net/eng/docs/scriptcommand_option">Script Commands - option</a> for more details.
        /// </remarks>
        public bool Confirm { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to enable the batch mode.
        /// </summary>
        /// <value>Set <c>true</c> if you want to enable the batch mode; otherwise, <c>false</c>.</value>
        /// <remarks>
        /// In batch mode, any choice prompt is automatically replied and any input prompt is cancelled (after short time interval). 
        /// 
        /// <list type="bullet">
        /// <listheader>
        /// <term>off</term>
        /// <description>In batch mode, it is recommended to set confirm to off to allow overwrites. </description>
        /// </listheader>
        /// <item>
        /// <term>on</term>
        /// <description>When batch mode is set to on any choice prompt is automatically replied negatively. Unless the prompt has different default answer (such as default “Reconnect” answer for reconnect prompt), in what case the default answer is used (after short time interval). </description>
        /// </item>
        /// <item>
        /// <term>abort</term>
        /// <description>Value abort is like on. In addition, the script is aborted, when any script command fails or any choice prompt is replied with “Abort” answer (or similar). </description>
        /// </item>
        /// <item>
        /// <term>continue</term>
        /// <description>When set to continue any choice prompt is automatically replied with “Skip”, “Ignore” or similar answer. </description>
        /// </item>        
        /// </list>
        /// 
        /// See <a href="http://winscp.net/eng/docs/scriptcommand_option">Script Commands - option</a> for more details.
        /// </remarks>
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

        /// <inheritdoc />
        protected AbstractWinScp()
        {
            this.Echo = true;
            this.Batch = "abort";
        }

        private bool IsFtpS
        {
            get { return ProtocolFtp && (Implicit || ExplicitSsl || ExplicitTls) == true; }
        }

        /// <inheritdoc />
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

        /// <inheritdoc />
        protected override string ToolName
        {
            get { return "WinSCP.com"; }
        }

        /// <inheritdoc />
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


        /// <inheritdoc />
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

        /// <inheritdoc />
        public override bool Execute()
        {
            Log.LogMessage(MessageImportance.High, InternalScriptText);
            return base.Execute();
        }


        /// <inheritdoc />
        protected override void LogEventsFromTextOutput(string singleLine, MessageImportance messageImportance)
        {
            singleLine = singleLine.Replace("Error", "Failure");
        
            base.LogEventsFromTextOutput(singleLine, messageImportance);
        }
    }
}
