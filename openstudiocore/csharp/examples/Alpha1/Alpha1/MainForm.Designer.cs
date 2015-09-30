namespace Alpha1
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
          this.btnGo = new System.Windows.Forms.Button();
          this.txtIdfPath = new System.Windows.Forms.TextBox();
          this.txtSqlPath = new System.Windows.Forms.TextBox();
          this.label1 = new System.Windows.Forms.Label();
          this.label2 = new System.Windows.Forms.Label();
          this.btnIdfBrowse = new System.Windows.Forms.Button();
          this.btnSqlBrowse = new System.Windows.Forms.Button();
          this.SuspendLayout();
          // 
          // btnGo
          // 
          this.btnGo.Location = new System.Drawing.Point(238, 236);
          this.btnGo.Name = "btnGo";
          this.btnGo.Size = new System.Drawing.Size(75, 23);
          this.btnGo.TabIndex = 0;
          this.btnGo.Text = "Go";
          this.btnGo.UseVisualStyleBackColor = true;
          this.btnGo.Click += new System.EventHandler(this.btnGo_Click);
          // 
          // txtIdfPath
          // 
          this.txtIdfPath.Location = new System.Drawing.Point(12, 29);
          this.txtIdfPath.Name = "txtIdfPath";
          this.txtIdfPath.Size = new System.Drawing.Size(263, 20);
          this.txtIdfPath.TabIndex = 1;
          this.txtIdfPath.Text = "";
          // 
          // txtSqlPath
          // 
          this.txtSqlPath.Location = new System.Drawing.Point(12, 72);
          this.txtSqlPath.Name = "txtSqlPath";
          this.txtSqlPath.Size = new System.Drawing.Size(263, 20);
          this.txtSqlPath.TabIndex = 2;
          this.txtSqlPath.Text = "";
          // 
          // label1
          // 
          this.label1.AutoSize = true;
          this.label1.Location = new System.Drawing.Point(13, 13);
          this.label1.Name = "label1";
          this.label1.Size = new System.Drawing.Size(123, 13);
          this.label1.TabIndex = 3;
          this.label1.Text = "Idf Path (required)";
          // 
          // label2
          // 
          this.label2.AutoSize = true;
          this.label2.Location = new System.Drawing.Point(13, 56);
          this.label2.Name = "label2";
          this.label2.Size = new System.Drawing.Size(126, 13);
          this.label2.TabIndex = 4;
          this.label2.Text = "Sql Path (optional)";
          // 
          // btnIdfBrowse
          // 
          this.btnIdfBrowse.Location = new System.Drawing.Point(281, 27);
          this.btnIdfBrowse.Name = "btnIdfBrowse";
          this.btnIdfBrowse.Size = new System.Drawing.Size(32, 23);
          this.btnIdfBrowse.TabIndex = 5;
          this.btnIdfBrowse.Text = "...";
          this.btnIdfBrowse.UseVisualStyleBackColor = true;
          this.btnIdfBrowse.Click += new System.EventHandler(this.btnIdfBrowse_Click);
          // 
          // btnSqlBrowse
          // 
          this.btnSqlBrowse.Location = new System.Drawing.Point(281, 70);
          this.btnSqlBrowse.Name = "btnSqlBrowse";
          this.btnSqlBrowse.Size = new System.Drawing.Size(32, 23);
          this.btnSqlBrowse.TabIndex = 6;
          this.btnSqlBrowse.Text = "...";
          this.btnSqlBrowse.UseVisualStyleBackColor = true;
          this.btnSqlBrowse.Click += new System.EventHandler(this.btnSqlBrowse_Click);
          // 
          // MainForm
          // 
          this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
          this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
          this.ClientSize = new System.Drawing.Size(325, 271);
          this.Controls.Add(this.btnSqlBrowse);
          this.Controls.Add(this.btnIdfBrowse);
          this.Controls.Add(this.label2);
          this.Controls.Add(this.label1);
          this.Controls.Add(this.txtSqlPath);
          this.Controls.Add(this.txtIdfPath);
          this.Controls.Add(this.btnGo);
          this.Name = "MainForm";
          this.Text = "Alpha1";
          this.Load += new System.EventHandler(this.MainForm_Load);
          this.ResumeLayout(false);
          this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnGo;
        private System.Windows.Forms.TextBox txtIdfPath;
        private System.Windows.Forms.TextBox txtSqlPath;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnIdfBrowse;
        private System.Windows.Forms.Button btnSqlBrowse;
    }
}

