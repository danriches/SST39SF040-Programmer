namespace WindowsFormsApp1
{
    partial class Form1
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
            this.txtCOMPort = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnConnect = new System.Windows.Forms.Button();
            this.txtData = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnRead = new System.Windows.Forms.Button();
            this.btnWrite = new System.Windows.Forms.Button();
            this.btnErase = new System.Windows.Forms.Button();
            this.btnGetID = new System.Windows.Forms.Button();
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.lblStatus = new System.Windows.Forms.Label();
            this.txtMessages = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtStartAddress = new System.Windows.Forms.TextBox();
            this.txtEndAddress = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.btnClearData = new System.Windows.Forms.Button();
            this.btnClearLog = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // txtCOMPort
            // 
            this.txtCOMPort.Location = new System.Drawing.Point(17, 38);
            this.txtCOMPort.Name = "txtCOMPort";
            this.txtCOMPort.Size = new System.Drawing.Size(88, 20);
            this.txtCOMPort.TabIndex = 0;
            this.txtCOMPort.Text = "COM5";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "COM Port";
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(111, 38);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(74, 20);
            this.btnConnect.TabIndex = 2;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // txtData
            // 
            this.txtData.AcceptsReturn = true;
            this.txtData.Location = new System.Drawing.Point(17, 94);
            this.txtData.Multiline = true;
            this.txtData.Name = "txtData";
            this.txtData.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtData.Size = new System.Drawing.Size(682, 327);
            this.txtData.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(14, 78);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(338, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Hex Data - Changes here are NOT written to flash, edit the original file!";
            // 
            // btnRead
            // 
            this.btnRead.Enabled = false;
            this.btnRead.Location = new System.Drawing.Point(483, 60);
            this.btnRead.Name = "btnRead";
            this.btnRead.Size = new System.Drawing.Size(68, 28);
            this.btnRead.TabIndex = 5;
            this.btnRead.Text = "Read";
            this.btnRead.UseVisualStyleBackColor = true;
            this.btnRead.Click += new System.EventHandler(this.btnRead_Click);
            // 
            // btnWrite
            // 
            this.btnWrite.Enabled = false;
            this.btnWrite.Location = new System.Drawing.Point(557, 60);
            this.btnWrite.Name = "btnWrite";
            this.btnWrite.Size = new System.Drawing.Size(68, 28);
            this.btnWrite.TabIndex = 6;
            this.btnWrite.Text = "Write";
            this.btnWrite.UseVisualStyleBackColor = true;
            this.btnWrite.Click += new System.EventHandler(this.btnWrite_Click);
            // 
            // btnErase
            // 
            this.btnErase.Enabled = false;
            this.btnErase.Location = new System.Drawing.Point(631, 61);
            this.btnErase.Name = "btnErase";
            this.btnErase.Size = new System.Drawing.Size(68, 27);
            this.btnErase.TabIndex = 7;
            this.btnErase.Text = "Erase";
            this.btnErase.UseVisualStyleBackColor = true;
            this.btnErase.Click += new System.EventHandler(this.btnErase_Click);
            // 
            // btnGetID
            // 
            this.btnGetID.Enabled = false;
            this.btnGetID.Location = new System.Drawing.Point(411, 61);
            this.btnGetID.Name = "btnGetID";
            this.btnGetID.Size = new System.Drawing.Size(66, 27);
            this.btnGetID.TabIndex = 8;
            this.btnGetID.Text = "Get ID";
            this.btnGetID.UseVisualStyleBackColor = true;
            this.btnGetID.Click += new System.EventHandler(this.btnGetID_Click);
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Enabled = false;
            this.btnDisconnect.Location = new System.Drawing.Point(191, 37);
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(74, 20);
            this.btnDisconnect.TabIndex = 9;
            this.btnDisconnect.Text = "Disconnect";
            this.btnDisconnect.UseVisualStyleBackColor = true;
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.Location = new System.Drawing.Point(271, 41);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(75, 13);
            this.lblStatus.TabIndex = 10;
            this.lblStatus.Text = "Status: Closed";
            // 
            // txtMessages
            // 
            this.txtMessages.Location = new System.Drawing.Point(17, 452);
            this.txtMessages.Multiline = true;
            this.txtMessages.Name = "txtMessages";
            this.txtMessages.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtMessages.Size = new System.Drawing.Size(681, 85);
            this.txtMessages.TabIndex = 11;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 433);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(71, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "Message Log";
            // 
            // txtStartAddress
            // 
            this.txtStartAddress.Location = new System.Drawing.Point(411, 31);
            this.txtStartAddress.Name = "txtStartAddress";
            this.txtStartAddress.Size = new System.Drawing.Size(139, 20);
            this.txtStartAddress.TabIndex = 13;
            this.txtStartAddress.Text = "0";
            // 
            // txtEndAddress
            // 
            this.txtEndAddress.Location = new System.Drawing.Point(557, 31);
            this.txtEndAddress.Name = "txtEndAddress";
            this.txtEndAddress.Size = new System.Drawing.Size(139, 20);
            this.txtEndAddress.TabIndex = 14;
            this.txtEndAddress.Text = "524287";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(407, 17);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(70, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Start Address";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(554, 17);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(67, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "End Address";
            // 
            // btnClearData
            // 
            this.btnClearData.Location = new System.Drawing.Point(524, 427);
            this.btnClearData.Name = "btnClearData";
            this.btnClearData.Size = new System.Drawing.Size(77, 22);
            this.btnClearData.TabIndex = 17;
            this.btnClearData.Text = "Clear Data";
            this.btnClearData.UseVisualStyleBackColor = true;
            this.btnClearData.Click += new System.EventHandler(this.btnClearData_Click);
            // 
            // btnClearLog
            // 
            this.btnClearLog.Location = new System.Drawing.Point(622, 427);
            this.btnClearLog.Name = "btnClearLog";
            this.btnClearLog.Size = new System.Drawing.Size(77, 22);
            this.btnClearLog.TabIndex = 18;
            this.btnClearLog.Text = "Clear Log";
            this.btnClearLog.UseVisualStyleBackColor = true;
            this.btnClearLog.Click += new System.EventHandler(this.btnClearLog_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(347, 8);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(51, 29);
            this.button1.TabIndex = 19;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Visible = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(715, 546);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnClearLog);
            this.Controls.Add(this.btnClearData);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.txtEndAddress);
            this.Controls.Add(this.txtStartAddress);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtMessages);
            this.Controls.Add(this.lblStatus);
            this.Controls.Add(this.btnDisconnect);
            this.Controls.Add(this.btnGetID);
            this.Controls.Add(this.btnErase);
            this.Controls.Add(this.btnWrite);
            this.Controls.Add(this.btnRead);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtData);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtCOMPort);
            this.Name = "Form1";
            this.Text = "SST39SF0x0A Flash Programmer";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtCOMPort;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.TextBox txtData;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnRead;
        private System.Windows.Forms.Button btnWrite;
        private System.Windows.Forms.Button btnErase;
        private System.Windows.Forms.Button btnGetID;
        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.TextBox txtMessages;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtStartAddress;
        private System.Windows.Forms.TextBox txtEndAddress;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btnClearData;
        private System.Windows.Forms.Button btnClearLog;
        private System.Windows.Forms.Button button1;
    }
}

