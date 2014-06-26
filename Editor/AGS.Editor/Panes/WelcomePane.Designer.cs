namespace AGS.Editor
{
    partial class WelcomePane
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.webView = new System.Windows.Forms.WebBrowser();
			this.SuspendLayout();
			// 
			// webView
			// 
			this.webView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.webView.Location = new System.Drawing.Point(0, 0);
			this.webView.MinimumSize = new System.Drawing.Size(20, 20);
			this.webView.Name = "webView";
			this.webView.ScrollBarsEnabled = false;
			this.webView.Size = new System.Drawing.Size(697, 502);
			this.webView.TabIndex = 0;
			this.webView.Url = new System.Uri("", System.UriKind.Relative);
			this.webView.Visible = false;
			// 
			// WelcomePane
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.White;
			this.Controls.Add(this.webView);
			this.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "WelcomePane";
			this.Size = new System.Drawing.Size(697, 502);
			this.Load += new System.EventHandler(this.WelcomePane_Load);
			this.Click += new System.EventHandler(this.WelcomePane_Click);
			this.Resize += new System.EventHandler(this.WelcomePane_Resize);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.WebBrowser webView;

    }
}
