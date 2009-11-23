using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using TestCommon;

namespace TestClient
{
    public partial class TestClient : Form
    {
        public TestClient()
        {
            InitializeComponent();
            Reporter.AddNode(new ReportRichTextBoxControl(OutputBox));
        }

        private void Resized()
        {
            if (FlowLayoutPane.Visible)
            {
                InputBox.Width = FlowLayoutPane.Width - MyHelpButton.Width - ClearButton.Width - 20;
                OutputBox.Height = ClientRectangle.Height - FlowLayoutPane.Height;
            }
            else
            {
                OutputBox.Height = ClientRectangle.Height;
            }
        }

        private void InputBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                CommandHandler.Instance.OnCommand(InputBox.Text);
                InputBox.Text = "";
            }
            else if (e.KeyChar == 27 || e.KeyChar == 9)
            {
                FlowLayoutPane.Hide();
                Resized();
                OutputBox.Focus();
            }
        }

        private void OutputBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 27)
            {
                FlowLayoutPane.Show();
            }
            if (FlowLayoutPane.Visible)
            {
                InputBox.Focus();
            }
        }

        private void TestClient_SizeChanged(object sender, EventArgs e)
        {
            Resized();
        }

        private void TestClient_Layout(object sender, LayoutEventArgs e)
        {
            Resized();
        }

        private void ClearButton_Click(object sender, EventArgs e)
        {
            OutputBox.Text = "";
        }
    }
}
