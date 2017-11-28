namespace ModelCreateExample
{
    partial class MainForm
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
          this.btnCreateModel = new System.Windows.Forms.Button();
          this.SuspendLayout();
          // 
          // btnCreateModel
          // 
          this.btnCreateModel.Location = new System.Drawing.Point(90, 12);
          this.btnCreateModel.Name = "btnCreateModel";
          this.btnCreateModel.Size = new System.Drawing.Size(97, 23);
          this.btnCreateModel.TabIndex = 0;
          this.btnCreateModel.Text = "Create Model";
          this.btnCreateModel.UseVisualStyleBackColor = true;
          this.btnCreateModel.Click += new System.EventHandler(this.btnCreateModel_Click);
          // 
          // MainForm
          // 
          this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
          this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
          this.ClientSize = new System.Drawing.Size(286, 65);
          this.Controls.Add(this.btnCreateModel);
          this.Name = "MainForm";
          this.Text = "Create Model Demo";
          this.Load += new System.EventHandler(this.MainForm_Load);
          this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCreateModel;
    }
}

