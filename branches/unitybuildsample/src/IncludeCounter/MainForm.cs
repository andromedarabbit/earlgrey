using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using Microsoft.Win32;

namespace IncludeCounter
{
    public partial class MainForm : Form
    {
        private Dictionary<string, FileNode> _fileNodeDictionary = new Dictionary<string, FileNode>();
        private List<string> _filesInPreCompileHeader = new List<string>();
        private int _CppCount = 0;
        private const string keyName = @"Software\Earlgrey\IncludeCounter";

        public MainForm()
        {
            InitializeComponent();
        }

        private void diagnosis_Click(object sender, EventArgs e)
        {
            RegistryKey key = Registry.LocalMachine.OpenSubKey(keyName, true);
            if (key == null)
            {
                key = Registry.LocalMachine.CreateSubKey(keyName);
            }
            else
            {
                string value = key.GetValue("selected") as string;
                if (value.Length > 0)
                {
                    selectFolder.SelectedPath = value;
                }
            }
            selectFolder.ShowDialog();

            if (selectFolder.SelectedPath.Length == 0)
            {
                return;
            }

            key.SetValue("selected", selectFolder.SelectedPath);

            _fileNodeDictionary.Clear();
            _filesInPreCompileHeader.Clear();
            _CppCount = 0;

            MakeReferenceGraph(selectFolder.SelectedPath);

            CountReferences();
            
            FileNode preCompileHeaderNode = GetOrCreateFileNode("stdafx.h");
            _filesInPreCompileHeader.Clear();

            GetReferencingFilenames(preCompileHeaderNode, _filesInPreCompileHeader);

            Display();

        }

        private void CountReferences()
        {
            List<string> traversedFiles = new List<string>();
            foreach (KeyValuePair<string, FileNode> pair in _fileNodeDictionary)
            {
                if (Path.GetExtension(pair.Key) != ".cpp")
                {
                    continue;
                }
                traversedFiles.Clear();
                traversedFiles.Add(pair.Key);

                foreach (FileNode referencingFileNode in pair.Value.ReferenceFileNodes)
                {
                    IncreaseCount(referencingFileNode, traversedFiles);
                }
            }
        }

        private void Display()
        {
            ResultView.Items.Clear();
            foreach (KeyValuePair<string, FileNode> pair in _fileNodeDictionary)
            {
                if (Path.GetExtension(pair.Key) == ".cpp")
                {
                    continue;
                }
                string[] items = {
                                   pair.Key,
                                   pair.Value.Count.ToString(),
                                   (pair.Value.Count * 100 / _CppCount).ToString() + "%",
                                   pair.Value.Recursive ? "Recursive" : ""
                               };

                ListViewItem item;
                if (_filesInPreCompileHeader.Exists(name => name == pair.Key))
                {
                    item = new ListViewItem(items, -1, Color.Gray, Color.White, new Font("Verdana", 8));
                }
                else if (pair.Value.Count >= _CppCount / 2)
                {
                    item = new ListViewItem(items, -1, Color.Red, Color.LightGray, new Font("Verdana", 8));
                }
                else
                {
                    item = new ListViewItem(items);
                }

                
                ResultView.Items.Add(item);
            }

            ResultView.ListViewItemSorter = new ListViewItemComparer();
            this.Text = "Include Counter - cpp files:" + _CppCount;
        }

        private void MakeReferenceGraph(string directoryName)
        {
            string[] files = Directory.GetFiles(directoryName);
            foreach (string path in files)
            {
                string filename = Path.GetFileName(path).ToLower();
                string ext = Path.GetExtension(path).ToLower();

                if (ext != ".h" && ext != ".hpp" && ext != ".c" && ext != ".cpp" && ext != ".inl")
                {
                    continue;
                }

                if (ext == ".cpp")
                {
                    _CppCount++;
                }

                FileNode fileNode = GetOrCreateFileNode(filename);

                AddIncludeFiles(path, fileNode);
            }

            string[] directories = Directory.GetDirectories(directoryName);
            foreach (string directory in directories)
            {
                MakeReferenceGraph(directory);
            }
        }

        private FileNode GetOrCreateFileNode(string filename)
        {
            FileNode fileNode;
            if (_fileNodeDictionary.ContainsKey(filename))
            {
                fileNode = _fileNodeDictionary[filename];
            }
            else
            {
                fileNode = new FileNode();
                fileNode.Filename = filename;
                _fileNodeDictionary.Add(filename, fileNode);
            }

            return fileNode;
        }

        private void GetReferencingFilenames(FileNode parentNode, List<string> filesInPreCompileHeader)
        {
            foreach (FileNode fileNodeInPreCompileHeader in parentNode.ReferenceFileNodes)
            {
                filesInPreCompileHeader.Add(fileNodeInPreCompileHeader.Filename);
                GetReferencingFilenames(fileNodeInPreCompileHeader, filesInPreCompileHeader);
            }
        }

        private void AddIncludeFiles(string path, FileNode fileNode)
        {
            using(StreamReader reader = File.OpenText(path))
            {
                while(!reader.EndOfStream)
                {
                    string line = reader.ReadLine().ToLower();
                    if (!line.Contains("#include"))
                    {
                        continue;
                    }

                    int index = line.IndexOf('\"');
                    char embrace = '\"';
                    if (index < 0)
                    {
                        index = line.IndexOf('<');
                        embrace = '>';
                    }
                    if (index < 0)
                    {
                        continue;
                    }

                    int endIndex = line.IndexOf(embrace, index + 1);
                    Debug.Assert(endIndex >= 0);
                    if (endIndex < 0)
                    {
                        continue;
                    }

                    string filename = line.Substring(index + 1, endIndex - index - 1);
                    FileNode referencingFileNode = GetOrCreateFileNode(Path.GetFileName(filename).ToLower());
                    Debug.Print(filename);

                    fileNode.ReferenceFileNodes.Add(referencingFileNode);
                }
            }
        }

        void IncreaseCount(FileNode fileNode, List<string> traversedFiles)
        {
            if (traversedFiles.Exists(filename => filename == fileNode.Filename))
            {
                fileNode.Recursive = true;
                return;
            }

            fileNode.IncreaseCount();
            traversedFiles.Add(fileNode.Filename);

            foreach (FileNode referencingFileNode in fileNode.ReferenceFileNodes)
            {
                IncreaseCount(referencingFileNode, traversedFiles);
            }
        }

        private void ResultView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            ListViewItemComparer comparer = new ListViewItemComparer((ListViewItemComparer)ResultView.ListViewItemSorter);
            if (comparer.Index == e.Column)
            {
                comparer.Sorting = (comparer.Sorting == SortOrder.Ascending) ? SortOrder.Descending : SortOrder.Ascending;
            }
            else
            {
                comparer.Index = e.Column;
            }

            ResultView.ListViewItemSorter = comparer;
        }

        private void combine_Click(object sender, EventArgs e)
        {
            RegistryKey key = Registry.LocalMachine.OpenSubKey(keyName, true);
            if (key == null)
            {
                key = Registry.LocalMachine.CreateSubKey(keyName);
            }
            else
            {
                string value = key.GetValue("selected") as string;
                if (value.Length > 0)
                {
                    selectFolder.SelectedPath = value;
                }
            }
            selectFolder.ShowDialog();

            if (selectFolder.SelectedPath.Length == 0)
            {
                return;
            }

            key.SetValue("selected", selectFolder.SelectedPath);

            Combine(selectFolder.SelectedPath);
        }

        private void Combine(string directory)
        {
            using (StreamWriter writer = File.CreateText(Path.Combine(directory, "Combine.cpp")))
            {
                _Combine(directory, directory, writer);
            }            
        }

        private void _Combine(string rootDirectory, string baseDirectory, StreamWriter writer)
        {
            string[] files = Directory.GetFiles(baseDirectory);
            foreach (string path in files)
            {
                string filename = Path.GetFileName(path).ToLower();
                string ext = Path.GetExtension(path).ToLower();

                if (ext != ".cpp" || filename == "combine.cpp" || filename == "stdafx.cpp")
                {
                    continue;
                }

                string cppFile = path.ToLower();
                int index = cppFile.IndexOf(rootDirectory.ToLower());
                Debug.Assert(index >= 0);

                index += rootDirectory.Length;

                if (cppFile[index] == '\\' || cppFile[index] == '/')
                {
                    index++;
                }

                writer.WriteLine("#include \"" + cppFile.Substring(index) + "\"");
            }

            string[] directories = Directory.GetDirectories(baseDirectory);
            foreach (string directory in directories)
            {
                _Combine(rootDirectory, directory, writer);
            }
        }
    }

    class ListViewItemComparer : IComparer
    {
        public ListViewItemComparer()
        {
            Sorting = SortOrder.Ascending;
            Index = 1;
        }

        public ListViewItemComparer(int index)
        {
            Sorting = SortOrder.Ascending;
            Index = index;
        }

        public ListViewItemComparer(ListViewItemComparer rhs)
        {
            Sorting = rhs.Sorting;
            Index = rhs.Index;
        }

        public int Compare(object x, object y)
        {
            if (Index == 1)
            {
                if (Sorting == SortOrder.Ascending)
                {
                    return int.Parse(((ListViewItem)y).SubItems[Index].Text) - int.Parse(((ListViewItem)x).SubItems[Index].Text);
                }
                return int.Parse(((ListViewItem)x).SubItems[Index].Text) - int.Parse(((ListViewItem)y).SubItems[Index].Text);
            }
            else
            {
                if (Sorting == SortOrder.Ascending)
                {
                    return String.Compare(((ListViewItem)x).SubItems[Index].Text, ((ListViewItem)y).SubItems[Index].Text);
                }
                return String.Compare(((ListViewItem)y).SubItems[Index].Text, ((ListViewItem)x).SubItems[Index].Text);
            }
            
        }

        public int Index
        {
            get;
            set;
        }

        public SortOrder Sorting
        {
            get;
            set;
        }
    }
}
