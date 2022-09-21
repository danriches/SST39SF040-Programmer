using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        //File Open Dialog for writing to Flash, we'll need to add a File Save one as well for Dumped data once we've also done the read processing
        private System.Windows.Forms.OpenFileDialog openFileDialog1 = new System.Windows.Forms.OpenFileDialog();

        SerialPort serial = new SerialPort();
        bool _continue;

        //Used to store the file we read in before writing to Flash
        byte[] writeData;

        public Form1()
        {
            InitializeComponent();
            openFileDialog1.Title = "Select file to flash to Flash, Flashy eh!?";
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //Find last com port and list it in the control.

        }

        private void AddToMessageLog(string textToAdd)
        {
            txtMessages.Text += textToAdd;
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            //Set default parameters for the serial port and open it
            serial.Parity = Parity.None;
            serial.ReadBufferSize = 256;
            serial.StopBits = StopBits.One;
            serial.WriteBufferSize = 256;
            serial.DataBits = 8;
            serial.BaudRate = 115200;
            serial.Handshake = Handshake.None;
            //serial.ReadTimeout = 1500;
            //serial.WriteTimeout = 1500;
            serial.PortName = txtCOMPort.Text;
            serial.Open();
            _continue = true;

            //Add handler to handle incoming data packets
            serial.DataReceived += new SerialDataReceivedEventHandler(Serial_DataReceived);

            if (serial.IsOpen)
            {
                btnConnect.Enabled = false;
                btnDisconnect.Enabled = true;
                lblStatus.Text = "Status: Connected";

                btnGetID.Enabled = true;
                btnRead.Enabled = true;
                btnWrite.Enabled = true;
                btnErase.Enabled = true;
            } else
            {
                lblStatus.Text = "Status: Unable to connect!";
                btnGetID.Enabled = false;
                btnRead.Enabled = false;
                btnWrite.Enabled = false;
                btnErase.Enabled = false;
            }
        }

        private delegate void SetTextDeleg(string text);            //Hex Data box text
        private delegate void SetMsgTextDeleg(string text);         //Message box text

        private void Serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //try
            //{ 
            string data = serial.ReadLine();        //This line is problematic and maybe should be serial.ReadBytes???
            if (data.StartsWith("0x"))
                this.BeginInvoke(new SetTextDeleg(SerialIn_DataReceived), new object[] { data });
            else
                this.BeginInvoke(new SetMsgTextDeleg(SerialIn_DataReceived2), new object[] { data });
            //} catch(Exception ex)
            //{
            //}
        }

        private void SerialIn_DataReceived(string data) { txtData.Text += data + Environment.NewLine; }
        private void SerialIn_DataReceived2(string data) { txtMessages.Text += data + Environment.NewLine; }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            if (serial.IsOpen)
            {
                serial.Close();
                serial.DataReceived -= new SerialDataReceivedEventHandler(Serial_DataReceived);
            }

            if (!serial.IsOpen)
            {
                btnConnect.Enabled = true;
                btnDisconnect.Enabled = false;
                btnGetID.Enabled = false;
                btnRead.Enabled = false;
                btnWrite.Enabled = false;
                btnErase.Enabled = false;
                lblStatus.Text = "Status: Closed";
            } else
            {
                btnConnect.Enabled = false;
                btnDisconnect.Enabled = true;
                btnGetID.Enabled = true;
                btnRead.Enabled = true;
                btnWrite.Enabled = true;
                btnErase.Enabled = true;
                lblStatus.Text = "Status: Open";
            }
        }

        private void btnGetID_Click(object sender, EventArgs e)
        {
            if(serial.IsOpen)
            {
                serial.WriteLine("<I,0,0>");
            }
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            //This needs the same treatment as the write function, namely streaming data back so it's faster
            if(serial.IsOpen)
            {
                serial.WriteLine("<D," + txtStartAddress.Text + "," + txtEndAddress.Text + ">");
            }
        }

        private void btnWrite_Click(object sender, EventArgs e)
        {
            //Warn that the chip must be erased before hand, do they want to erase it first or attempt an overwrite which may go wrong!?!?!
            MessageBox.Show("The chip should be erased before writing or you'll end up with dodgy data stored in it!", "Warning!");

            //Open a file dialog and allow any file up to a max of 524288 bytes in length.
            openFileDialog1.ShowDialog();

            string filename = openFileDialog1.FileName;
            if (filename == "")
            {
                txtMessages.Text += "Flash was not written to, user cancelled operation!";
                return;
            }
            else
            {
                txtMessages.Text += "File selected was: " + filename;
                string result = WriteFileToFlash(filename);
                txtMessages.Text += result;
            }
            //Construct a base command with address counter and write the data byte by byte as is required by the chip.
        }

        string WriteFileToFlash(string filename)
        {
            string result = "FAILED!";

            //Open the file and read into a byte array
            byte[] dataArray = File.ReadAllBytes(filename);

            //Write the bytes one at a time until done
            if (!serial.IsOpen)
                return "Failed to Write as not connected to Arduino Mega!";

            txtMessages.Text += "Writing Flash starting at address 0x0000. Please Wait...";
            int addressCounter = 0;

            //send <X,0,0> then <Z,x,0>   where x is 256 or the number of bytes to write, lastly send <Y> and then the full amount of bytes in sequence  
            //- Arduino side seems to work just need this side to
            int totalSize = dataArray.Length;

            serial.WriteLine("<X," + txtStartAddress.Text + ",0>");
            serial.WriteLine("<Z," + totalSize.ToString() + ",0>");
            serial.Write("<Y>");

            int offset = 0;
            int block = 256;
            if (block > totalSize)
                block = totalSize;
            
            //This part needs checking before allowing a write to Flash!!!!!!!
            while(offset < totalSize)
            {                
                serial.Write(dataArray, offset, block);
                if (offset + block > totalSize)
                    offset = totalSize - (offset + block);
                else
                    offset += block;
                Application.DoEvents();
            }
            //Read the bytes back and compare, using a dump command, ignore and don't read data past where it was written...

            return "Success: Wrote " + (addressCounter + 1).ToString() + " bytes to Flash." + Environment.NewLine;
        }

        private void btnErase_Click(object sender, EventArgs e)
        {
            if(serial.IsOpen)
            {
                txtMessages.Text += "Erasing entire chip, I hope you wanted to do this as it's now too late!";
                serial.WriteLine("<E,0,0>");
            }
        }

        private void btnClearData_Click(object sender, EventArgs e)
        {
            txtData.Text = "";
        }

        private void btnClearLog_Click(object sender, EventArgs e)
        {
            txtMessages.Text = "";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            serial.WriteLine("<Z,10,0>");
            serial.Write("<Y>");
            byte[] x = new byte[10] { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57 };
            serial.Write(x, 0, 10);
        }
    }
}
