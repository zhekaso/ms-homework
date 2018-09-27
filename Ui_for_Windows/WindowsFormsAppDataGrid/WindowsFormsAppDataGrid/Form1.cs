using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace WindowsFormsAppDataGrid
{
    public partial class Form1 : Form
    {
        public BindingSource Data { get; set; }
        public BindingSource FilesData { get; set; }
        Dictionary<Guid, List<Data>> PointsFromFile;
        public Form1()
        {
            Data = new BindingSource();
            FilesData = new BindingSource();
            PointsFromFile = new Dictionary<Guid, List<Data>>();
            Data.Add(new Data() { X = 0, Y = 0 });
            Data.Add(new Data() { X = 1, Y = 1 });
            Data.Add(new Data() { X = 2, Y = 4 });
            Data.ListChanged += new System.ComponentModel.ListChangedEventHandler(Data_CurrentChanged);
            InitializeComponent();
            dataGridView1.DataSource = Data;
            OpenFilesView.DataSource = FilesData;
            comboBox1.SelectedIndex = 0;
            chart1.DataSource = null;
            chart1.Series[0].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chart1.DataSource = Data;
        }

        private void Data_CurrentChanged(object sender, EventArgs e)
        {
            chart1.DataSource = null;
            chart1.DataSource = Data;
            chart1.Invalidate();
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            Data last = (Data)Data.List[Data.List.Count - 1];
            Data.Add(new Data() { X = last.X + 1, Y = (last.X + 1)* (last.X + 1) });
            
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            chart1.DataSource = null;
            if (comboBox1.SelectedIndex == comboBox1.Items.IndexOf("Line"))
            {
                chart1.Series[0].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            }
            else if ((comboBox1.SelectedIndex == comboBox1.Items.IndexOf("Spline")))
            {
                chart1.Series[0].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            }
            chart1.DataSource = Data;
            Invalidate();
        }

        private void загрузитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog download = new OpenFileDialog();
            if (download.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                Data.Clear();
                Guid FileId = Guid.NewGuid();
                var Points = new List<Data>();

                using (var SR = new StreamReader(download.FileName))
                {
                    string line;
                    while ((line = SR.ReadLine()) != null)
                    {
                        var parts = line.Split(' ');
                        double x = Convert.ToDouble(parts[0]);
                        double y = Convert.ToDouble(parts[1]);

                        Points.Add(new Data() { X = x, Y = y });
                        Data.Add(new Data() { X = x, Y = y });
                    }
                }
                PointsFromFile.Add(FileId, Points);
                FilesData.Add(new DataFiles() { FileName = Path.GetFileName(download.FileName), id = FileId });
                FilesData.Position = FilesData.List.Count - 1;
            }
        }

        private void OpenFilesView_RowEnter(object sender, DataGridViewCellEventArgs e)
        {
            Guid FileId = ((DataFiles)FilesData.List[e.RowIndex]).id;
            Data.Clear();
            var Points = PointsFromFile[FileId];

            foreach(var point in Points)
            {
                Data.Add(point);
            }

        }

        private void OpenFilesView_RowsRemoved(object sender, DataGridViewRowsRemovedEventArgs e)
        {

        }

        private void OpenFilesView_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
        {
            var Row = (DataFiles)FilesData.List[e.Row.Index];

            PointsFromFile.Remove(Row.id);
            if(PointsFromFile.Count == 0)
            {
                Data.Clear();
            }
        }

        private void сохранитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            if (saveFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
           
             

                using (var SR = new StreamWriter(saveFileDialog1.FileName))
                {
                    foreach(Data entry in Data.List)
                    {
                        SR.WriteLine($"{entry.X} {entry.Y}");
                    }
                }
            }


        }
    }
    public class Data
    {
        public double X { get; set; }
        public double Y { get; set; }
    }

    public class DataFiles
    {
        public string FileName { get; set; }
        public Guid id;
    }
}
