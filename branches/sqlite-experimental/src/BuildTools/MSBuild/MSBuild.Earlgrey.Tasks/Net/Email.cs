using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Mail;
using Microsoft.Build.Utilities;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Net
{
    // TODO: Earlgrey 의 GmailClient와 겹치는 코드가 많다. 
    /// <remarks>Modified MSBuild.Community.Tasks</remarks>
    public class Email : Task
    {
        public Email()
        {
            _priority = MailPriority.Normal;
            _useSsl = false;
            _encoding = Encoding.Unicode;
            _smtpPort = 25;
        }

        #region Properties

        string[] _attachments;

        /// <summary>
        /// List of files to attach to message
        /// </summary>
        public string[] Attachments
        {
            get { return _attachments; }
            set { _attachments = value; }
        }

        private string[] _bcc;

        /// <summary>
        /// List of addresss that contains the blind carbon copy (BCC) recipients for this e-mail message
        /// </summary>
        public string[] Bcc
        {
            get { return _bcc; }
            set { _bcc = value; }
        }

        string[] _cc;

        /// <summary>
        /// List of addresss that contains the carbon copy (CC) recipients for this e-mail message
        /// </summary>
        public string[] Cc
        {
            get { return _cc; }
            set { _cc = value; }
        }

        string _body;

        /// <summary>
        /// The email message body
        /// </summary>
        public string Body
        {
            get { return _body; }
            set { _body = value; }
        }

        string _from;
        /// <summary>
        /// The from address for this e-mail message
        /// </summary>
        /// <remarks>
        /// This property is required.
        /// </remarks>
        [Required]
        public string From
        {
            get { return _from; }
            set { _from = value; }
        }

        bool _isBodyHtml;
        /// <summary>
        /// A value indicating whether the mail message body is in Html
        /// </summary>
        public bool IsBodyHtml
        {
            get { return _isBodyHtml; }
            set { _isBodyHtml = value; }
        }

        MailPriority _priority;
        /// <summary>
        /// The priority of this e-mail message
        /// </summary>
        /// <remarks>
        /// Possible values are High, Normal, and Low
        /// </remarks>
        public string Priority
        {
            get { return Enum.GetName(typeof(MailPriority), _priority); }
            set { _priority = (MailPriority)Enum.Parse(typeof(MailPriority), value); }
        }

        string _subject;
        /// <summary>
        /// The subject line for this e-mail message
        /// </summary>
        /// <remarks>
        /// This property is required.
        /// </remarks>
        [Required]
        public string Subject
        {
            get { return _subject; }
            set { _subject = value; }
        }

        string _smtpServer;
        /// <summary>
        /// The name or IP address of the host used for SMTP transactions
        /// </summary>
        /// <remarks>
        /// This property is required.
        /// </remarks>
        [Required]
        public string SmtpServer
        {
            get { return _smtpServer; }
            set { _smtpServer = value; }
        }

        private int _smtpPort;

        public int SmtpPort
        {
            get { return _smtpPort; }
            set { _smtpPort = value; }
        }

        string[] _to;
        /// <summary>
        /// List of addresss that contains the recipients of this e-mail message
        /// </summary>
        /// <remarks>
        /// This property is required.
        /// </remarks>
        [Required]
        public string[] To
        {
            get { return _to; }
            set { _to = value; }
        }

        private string _username;

        /// <summary>
        /// Gets or sets the username.
        /// </summary>
        /// <value>The username.</value>
        public string Username
        {
            get { return _username; }
            set { _username = value; }
        }

        private string _password;

        /// <summary>
        /// Gets or sets the password.
        /// </summary>
        /// <value>The password.</value>
        public string Password
        {
            get { return _password; }
            set { _password = value; }
        }

        private bool _useSsl;

        public bool UseSsl
        {
            get { return _useSsl; }
            set { _useSsl = value; }
        }

        private Encoding _encoding;

        public string TextEncoding
        {
            get { return _encoding.EncodingName; }
            set { _encoding = Encoding.GetEncoding(value); }
        }

        #endregion


        private MailMessage CreateMailMessage()
        {
            MailMessage message = new MailMessage();

            message.SubjectEncoding = _encoding;
            message.BodyEncoding = _encoding;

            message.From = new MailAddress(_from);
            message.Subject = _subject;
            message.IsBodyHtml = _isBodyHtml;
            message.Body = _body;

            message.Priority = _priority;

            foreach (string to in _to)
            {
                message.To.Add(new MailAddress(to));
            }

            if (_cc != null && _cc.Length > 0)
            {
                foreach (string cc in _cc)
                {
                    message.CC.Add(new MailAddress(cc));
                }
            }

            if (_bcc != null && _bcc.Length > 0)
            {
                foreach (string bcc in _bcc)
                {
                    message.Bcc.Add(new MailAddress(bcc));
                }
            }

            if (_attachments != null && _attachments.Length > 0)
            {
                foreach (string attachment in _attachments)
                {
                    message.Attachments.Add(new Attachment(attachment));
                }
            }

            return message;
        }

        private SmtpClient CreateSmtpClient()
        {
            return SmtpClientFactory.CreateInstance(this);
        }

        public override bool Execute()
        {
            try
            {
                SmtpClient smtp = CreateSmtpClient();
                
                using (MailMessage message = CreateMailMessage())
                {
                    smtp.Send(message);
                    return true;
                }
            }
            catch (Exception ex)
            {
                Log.LogErrorFromException(ex);
                return false;
            }
        }
    }
}
