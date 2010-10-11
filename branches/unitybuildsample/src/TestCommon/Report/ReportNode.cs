using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace TestCommon
{
    public enum ReportType
    {
        Error,
        System,
        Debug,
        Result,
        Normal
    }

    public interface ReportNode
    {
        void Report(ReportType Type, string Text);
        void Clear();
    }

    public class ReportFile : ReportNode
    {
        public ReportFile(string filename)
        {
            _Filename = filename;
        }

        public void Report(ReportType Type, string Text)
        {
            using (StreamWriter writer = File.AppendText(_Filename))
            {
                writer.WriteLine(Text);
            }
        }

        public void Clear()
        {
        }

        private string _Filename = "";
    }

    public class ReportRichTextBoxControl : ReportNode
    {
        public ReportRichTextBoxControl(RichTextBox UserTextBox)
        {
            _RichTextBox = UserTextBox;
        }

        public void Report(ReportType Type, string Text)
        {
            if (_RichTextBox == null)
            {
                return;
            }

            Color TextColor = Color.DarkGray;
            switch(Type)
            {
                case ReportType.Error:
                    _RichTextBox.SelectionFont = new Font(_RichTextBox.SelectionFont, FontStyle.Bold);
                    TextColor = Color.Red;
                    break;
                case ReportType.Debug:
                    TextColor = Color.DarkGray;
                    break;
                case ReportType.System:
                    TextColor = Color.Red;
                    break;
                case ReportType.Result:
                    _RichTextBox.SelectionFont = new Font(_RichTextBox.SelectionFont, FontStyle.Bold);
                    TextColor = Color.Black;
                    break;
                case ReportType.Normal:
                    TextColor = Color.Black;
                    break;
            }

            _RichTextBox.SelectionColor = TextColor;
            _RichTextBox.AppendText(Text);
            _RichTextBox.ScrollToCaret();
        }

        public void Clear()
        {
            _RichTextBox.Text = "";
        }

        private System.Windows.Forms.RichTextBox _RichTextBox = null;
    }

    public class ReportRemoteNode : ReportNode
    {
        public void Report(ReportType Type, string Text)
        {

        }

        public void Clear()
        {

        }
    }
}
