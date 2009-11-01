using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

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
            _RichTextBox.Select(_RichTextBox.TextLength, 1);

            Color TextColor = Color.DarkGray;

            switch(Type)
            {
                case ReportType.Error:
                    TextColor = Color.Red;
                    break;
                case ReportType.Debug:
                    TextColor = Color.LightGray;
                    break;
                case ReportType.System:
                    TextColor = Color.Red;
                    break;
                case ReportType.Result:
                    TextColor = Color.Black;
                    break;
            }

            _RichTextBox.SelectionColor = TextColor;
            _RichTextBox.SelectedText = Text;
        }

        private System.Windows.Forms.RichTextBox _RichTextBox = null;
    }

    public class ReportRemoteNode : ReportNode
    {
        public void Report(ReportType Type, string Text)
        {

        }
    }
}
