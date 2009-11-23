namespace TestClient
{
    partial class TestClient
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.InputBox = new System.Windows.Forms.TextBox();
            this.FlowLayoutPane = new System.Windows.Forms.FlowLayoutPanel();
            this.MyHelpButton = new System.Windows.Forms.Button();
            this.ClearButton = new System.Windows.Forms.Button();
            this.OutputBox = new System.Windows.Forms.RichTextBox();
            this.FlowLayoutPane.SuspendLayout();
            this.SuspendLayout();
            // 
            // InputBox
            // 
            this.InputBox.Location = new System.Drawing.Point(12, 3);
            this.InputBox.Name = "InputBox";
            this.InputBox.Size = new System.Drawing.Size(588, 22);
            this.InputBox.TabIndex = 0;
            this.InputBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.InputBox_KeyPress);
            // 
            // FlowLayoutPane
            // 
            this.FlowLayoutPane.Controls.Add(this.MyHelpButton);
            this.FlowLayoutPane.Controls.Add(this.ClearButton);
            this.FlowLayoutPane.Controls.Add(this.InputBox);
            this.FlowLayoutPane.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.FlowLayoutPane.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.FlowLayoutPane.Location = new System.Drawing.Point(0, 530);
            this.FlowLayoutPane.Name = "FlowLayoutPane";
            this.FlowLayoutPane.Size = new System.Drawing.Size(712, 30);
            this.FlowLayoutPane.TabIndex = 2;
            // 
            // MyHelpButton
            // 
            this.MyHelpButton.Location = new System.Drawing.Point(657, 3);
            this.MyHelpButton.Name = "MyHelpButton";
            this.MyHelpButton.Size = new System.Drawing.Size(52, 25);
            this.MyHelpButton.TabIndex = 2;
            this.MyHelpButton.Text = "&HELP";
            this.MyHelpButton.UseVisualStyleBackColor = true;
            // 
            // ClearButton
            // 
            this.ClearButton.Location = new System.Drawing.Point(606, 3);
            this.ClearButton.Name = "ClearButton";
            this.ClearButton.Size = new System.Drawing.Size(45, 25);
            this.ClearButton.TabIndex = 1;
            this.ClearButton.Text = "&CLR";
            this.ClearButton.UseVisualStyleBackColor = true;
            this.ClearButton.Click += new System.EventHandler(this.ClearButton_Click);
            // 
            // OutputBox
            // 
            this.OutputBox.BackColor = System.Drawing.SystemColors.Window;
            this.OutputBox.Dock = System.Windows.Forms.DockStyle.Top;
            this.OutputBox.Location = new System.Drawing.Point(0, 0);
            this.OutputBox.Name = "OutputBox";
            this.OutputBox.ReadOnly = true;
            this.OutputBox.Size = new System.Drawing.Size(712, 526);
            this.OutputBox.TabIndex = 3;
            this.OutputBox.Text = "";
            this.OutputBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OutputBox_KeyPress);
            // 
            // TestClient
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(712, 560);
            this.Controls.Add(this.OutputBox);
            this.Controls.Add(this.FlowLayoutPane);
            this.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "TestClient";
            this.Text = "TestClient";
            this.SizeChanged += new System.EventHandler(this.TestClient_SizeChanged);
            this.Layout += new System.Windows.Forms.LayoutEventHandler(this.TestClient_Layout);
            this.FlowLayoutPane.ResumeLayout(false);
            this.FlowLayoutPane.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox InputBox;
        private System.Windows.Forms.FlowLayoutPanel FlowLayoutPane;
        private System.Windows.Forms.Button ClearButton;
        private System.Windows.Forms.Button MyHelpButton;
        private System.Windows.Forms.RichTextBox OutputBox;

    }
}

