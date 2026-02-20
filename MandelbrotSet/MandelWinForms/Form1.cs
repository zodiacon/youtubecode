using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MandelWinForms {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();

            for (int i = 0; i < m_Rainbow.Length; i++)
                m_Rainbow[i] = Color.FromArgb(
                    (int)((Math.Sin(i * 2 * Math.PI / m_Rainbow.Length + .8762) + 1) / 2 * 255),
                    (int)((Math.Sin(i * 1.8722 * Math.PI / m_Rainbow.Length) + 1) / 2 * 255),
                    (int)((Math.Sin(i * 2.255 * Math.PI / m_Rainbow.Length - 1.11234) + 1) / 2 * 255)
                    );
        }

        Bitmap m_Bitmap;
        Complex m_From = new Complex(-1.5, -1.2);
        Complex m_To = new Complex(0.8, 1.2);
        Rectangle m_Selection;
        Pen m_SelectionPen = new Pen(Color.Red);
        Color[] m_Rainbow = new Color[256];
        bool m_MouseDown;

        int MandelbrotColor(Complex c) {
            int count = 255;
            var z = Complex.Zero;
            while(count > 0 && z.Real * z.Real + z.Imaginary * z.Imaginary <= 2) {
                z = z * z + c;
                count--;
            }
            return count;
        }

        void CalcMandelbrot() {
            m_Bitmap = new Bitmap(Width, Height);
            var dx = (m_To - m_From).Real / Width;
            var dy = (m_To - m_From).Imaginary / Height;
            for (int y = 0; y < Height; y++) {
                for (int x = 0; x < Width; x++) {
                    var c = new Complex(m_From.Real + dx * x,
                        m_From.Imaginary + dy * y);
                    var count = MandelbrotColor(c);
                    m_Bitmap.SetPixel(x, y, count == 0 ? Color.Black : m_Rainbow[count]);
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e) {
            CalcMandelbrot();

        }

        private void Form1_Paint(object sender, PaintEventArgs e) {
            if (m_Bitmap != null) {
                e.Graphics.DrawImage(m_Bitmap, new Rectangle(0, 0, Width, Height));
                if(m_MouseDown) {
                    e.Graphics.DrawRectangle(m_SelectionPen, m_Selection);
                }
            }
        }

        private void Form1_Resize(object sender, EventArgs e) {
            Invalidate();
        }

        private void Form1_MouseDown(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Left) {
                m_MouseDown = true;
                m_Selection.X = e.X;
                m_Selection.Y = e.Y;
            }
        }

        private void Form1_MouseUp(object sender, MouseEventArgs e) {
            if (m_MouseDown) {
                m_MouseDown = false;
                m_Selection.Width = e.X - m_Selection.X;
                m_Selection.Height = e.Y - m_Selection.Y;
                var size = m_To - m_From;
                m_From = new Complex(
                    m_From.Real + Math.Min(m_Selection.Left, m_Selection.Right) * size.Real / Width,
                    m_From.Imaginary + Math.Min(m_Selection.Top, m_Selection.Bottom) * size.Imaginary / Height);
                m_To = new Complex(
                    m_From.Real + size.Real * Math.Abs(m_Selection.Right - m_Selection.Left) / Width,
                    m_From.Imaginary + size.Imaginary * Math.Abs(m_Selection.Bottom - m_Selection.Top) / Height);
                CalcMandelbrot();
                Invalidate();

            }
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e) {
            if (m_MouseDown) {
                m_Selection.Width = e.X - m_Selection.X;
                m_Selection.Height = e.Y - m_Selection.Y;
                Invalidate();
            }
        }
    }
}
