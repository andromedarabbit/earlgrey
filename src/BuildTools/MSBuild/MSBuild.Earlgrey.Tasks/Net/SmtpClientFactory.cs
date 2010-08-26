using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Mail;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.Net
{
    // TODO: 나중에 플러그인 형식으로 바꾸려고 이렇게 구현했으나 과연....
    internal static class SmtpClientFactory
    {
        private static readonly List<ISmtpClientConfiguration> Configurations;

        static SmtpClientFactory()
        {
            Configurations = new List<ISmtpClientConfiguration>();
            Configurations.Add(new GmailSmtpClientConfiguration());

            // Default 가 항상 마지막에 들어가야 한다.
            Configurations.Add(new DefaultSmtpClientConfiguration());
        }

        public static SmtpClient CreateInstance(Email mail)
        {
            foreach (var configuration in Configurations)
            {
                SmtpClient client = configuration.CreateInstance(mail);
                if (client != null)
                    return client;
            }

            throw new Exception("Unexpected exception!");
        }
    }
}
