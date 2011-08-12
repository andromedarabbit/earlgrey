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
            _Reporter.Report(Type, Text + "\n");
        }

        public static void Log(ReportType Type, string Format, params object[] args)
        {
            StringBuilder str = new StringBuilder();
            str.AppendFormat(Format, args);
            str.Append("\n");
            _Reporter.Report(Type, str.ToString());
        }

        public static void Clear()
        {
            _Reporter.ClearAll();
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

        private void ClearAll()
        {
            foreach (ReportNode Node in _ReportNodes)
            {
                Node.Clear();
            }
        }
    }
}
