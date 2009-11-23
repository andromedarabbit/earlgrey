﻿using System;
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
        void Clear();
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
                    TextColor = Color.Red;
                    break;
                case ReportType.Debug:
                    TextColor = Color.DarkGray;
                    break;
                case ReportType.System:
                    TextColor = Color.Red;
                    break;
                case ReportType.Result:
                    TextColor = Color.Black;
                    break;
                case ReportType.Normal:
                    TextColor = Color.Black;
                    break;
            }

            _RichTextBox.ForeColor = TextColor;
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
