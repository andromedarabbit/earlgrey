using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Mail;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Net
{
    internal interface ISmtpClientConfiguration
    {
        SmtpClient CreateInstance(Email email);
    }
}
