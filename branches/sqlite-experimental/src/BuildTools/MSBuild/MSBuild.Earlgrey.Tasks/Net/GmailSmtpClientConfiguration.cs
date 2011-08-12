using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Mail;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Net
{
    internal class GmailSmtpClientConfiguration : ISmtpClientConfiguration
    {
        public SmtpClient CreateInstance(Email email)
        {
            string smtpServer = email.SmtpServer.Trim();
            if (smtpServer.Equals("smtp.gmail.com", StringComparison.CurrentCultureIgnoreCase) == false)
                return null;

            if (string.IsNullOrEmpty(email.Username))
                throw new Exception("Username is required for accessing your GMail account.");

            if (string.IsNullOrEmpty(email.Password))
                throw new Exception("Password is required for accessing your GMail account.");

            SmtpClient smtp = new SmtpClient();
            smtp.DeliveryMethod = SmtpDeliveryMethod.Network; // 이걸 하지 않으면 Gmail에 인증을 받지 못한다.

            smtp.Host = smtpServer;
            email.SmtpPort = 587;
            smtp.Port = email.SmtpPort;

            smtp.Credentials = new NetworkCredential(email.Username, email.Password);

            email.UseSsl = true;
            smtp.EnableSsl = email.UseSsl;

            return smtp;
        }
    }
}
