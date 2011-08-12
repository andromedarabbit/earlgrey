using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IncludeCounter
{
    class FileNode
    {
        public string Filename
        {
            get;
            set;
        }

        public int Count
        {
            get { return _Count; }
        }

        public List<FileNode> ReferenceFileNodes
        {
            get { return _ReferenceFileNodes; } 
        }

        public bool Recursive
        {
            get;
            set;
        }

        public void IncreaseCount()
        {
            _Count++;
        }

        private int _Count = 0;

        private List<FileNode> _ReferenceFileNodes = new List<FileNode>();
    }
}
