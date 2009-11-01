using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon
{
    public class Reporter
    {
        private Reporter()
        {
        }

        private static Reporter _Reporter = new Reporter();

        public static void Log(ReportType Type, string Text)
        {
            _Reporter.Report(Type, Text);
        }

        public static void Log(ReportType Type, string Format, params object[] args)
        {
            StringBuilder str = new StringBuilder();
            str.AppendFormat(Format, args);
            _Reporter.Report(Type, str.ToString());
        }

        private List<ReportNode> _ReportNodes = new List<ReportNode>();

        public static void AddNode(ReportNode Node)
        {
           _Reporter._ReportNodes.Add(Node);
        }

        private void Report(ReportType Type, string Text)
        {
            foreach (ReportNode Node in _ReportNodes)
            {
                Node.Report(Type, Text);
            }
        }
    }
}
