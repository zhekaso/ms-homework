using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab2
{
    public partial class Form1 : Form
    {
        bool whatColor = false;
        int x, y;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(Color.White);

            Pen brush = new Pen(whatColor ? Color.FromArgb(255,0,0) : Color.FromArgb(0, 155, 0),2);
            Point a = new Point(0, 0);
            Point b = new Point( this.ClientSize.Width, this.ClientSize.Height);
            e.Graphics.DrawLine(brush, a, b);

            Point c = new Point(this.ClientSize.Width, 0);
            Point d = new Point(0, this.ClientSize.Height);
            e.Graphics.DrawLine(brush, c, d);
            e.Graphics.DrawString($"{x},{y}", DefaultFont, new SolidBrush(Color.Black), x, y);

        }

        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            Invalidate();
        }

        private void Form1_Click(object sender, EventArgs e)
        {
           
            whatColor = !whatColor;
            Invalidate();
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            x = e.X;
            y = e.Y;
            Invalidate();
        }
    }
}
