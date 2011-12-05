using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Mail;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Net
{
    internal class DefaultSmtpClientConfiguration : ISmtpClientConfiguration
    {
        public SmtpClient CreateInstance(Email email)
        {   
            SmtpClient smtp = new SmtpClient();
            smtp.DeliveryMethod = SmtpDeliveryMethod.Network; // 이걸 하지 않으면 Gmail에 인증을 받지 못한다.

            smtp.Host = email.SmtpServer;
            smtp.Port = email.SmtpPort;

            if (!string.IsNullOrEmpty(email.Username))
                smtp.Credentials = new NetworkCredential(email.Username, email.Password);
            else
                smtp.UseDefaultCredentials = true;

            smtp.EnableSsl = email.UseSsl;

            return smtp;
        }
    }
}
