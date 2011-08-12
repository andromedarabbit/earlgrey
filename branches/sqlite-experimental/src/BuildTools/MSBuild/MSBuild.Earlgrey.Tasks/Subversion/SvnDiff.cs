using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Diagnostics;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using Microsoft.Build.Framework;

    public class SvnDiff : ToolTask 
    {
        private readonly SvnDiffWithPlainSummary _plainDiff;
        private readonly SvnDiffWithXmlSummary _xmlDiff;

        // private readonly List<AbstractSvnDiff.ItemChanged> _itemsChanged;
        private readonly List<ITaskItem> _itemsChanged;

        public SvnDiff()
        {
            this._plainDiff = new SvnDiffWithPlainSummary();
            this._xmlDiff = new SvnDiffWithXmlSummary();

            // this._itemsChanged = new List<AbstractSvnDiff.ItemChanged>();
            this._itemsChanged = new List<ITaskItem>();
        }
        
        protected override string GenerateFullPathToTool()
        {
            Debug.Assert(_plainDiff.ToolPath == _xmlDiff.ToolPath);
            return _plainDiff.ToolPath;
        }

        protected override string ToolName
        {
            get 
            {
                Debug.Assert(_plainDiff.ToolPath == _xmlDiff.ToolPath);
                return Path.GetFileName(_plainDiff.ToolPath); 
            }
        }

        private void InitializeInternalDiff(AbstractSvnDiff diffObj)
        {
            diffObj.BuildEngine = this.BuildEngine;
            diffObj.Old = this.Old;
            diffObj.New = this.New;
            diffObj.Username = this.Username;
            diffObj.Password = this.Password;
            diffObj.RepositoryPath = this.RepositoryPath;
            // TODO: 왜 LocalPath를 설정하면 실패하는 걸까?
            // diffObj.LocalPath = this.LocalPath;
            diffObj.OldIsBasePath = this.OldIsBasePath;
        }

        protected override bool ValidateParameters()
        {
            if(base.ValidateParameters() == false)
                return false;

            if (ResolveLocalPaths == true && this.LocalPath == null)
            {
                Log.LogError(
                    string.Format("Parameter 'LocalPath' required to resolve local paths")
                    );
                return false;
            }
            return true;
        }

        public override bool Execute()
        {
            // Execute
            InitializeInternalDiff(this._plainDiff);
            if (_plainDiff.Execute() == false)
                return false;

            InitializeInternalDiff(this._xmlDiff);
            if (_xmlDiff.Execute() == false)
                return false;

            var plainSummary = _plainDiff.Summary;
            var xmlSummary = _xmlDiff.Summary;

            if (plainSummary.Length != xmlSummary.Length)
            {
                Log.LogError(
                    string.Format("Svn diffs comparison failed! ({0}:{1})", plainSummary.Length, xmlSummary.Length)
                    );
                return false;
            }

            for (int i = 0; i < plainSummary.Length; i++ )
            {
                var plainItem = plainSummary[i];
                var xmlItem = xmlSummary[i];


                if(plainItem.StateMarked != xmlItem.StateMarked)
                    throw new Exception("Svn diffs comparison failed!");

                ITaskItem item = new TaskItem(plainItem.Path);
                item.SetMetadata(
                    "State"
                    , Enum.GetName(typeof (AbstractSvnDiff.State), plainItem.StateMarked)
                    );
                item.SetMetadata(
                    "Kind"
                    , Enum.GetName(typeof(AbstractSvnDiff.Kind), xmlItem.KindOf)
                    );
                _itemsChanged.Add(item);
            }

            return true;
        }

        protected List<ITaskItem> ItemsChanged
        {
            get
            {
                return _itemsChanged; 
            }        
        }


        public string Username
        {
            get;
            set;
        }

        public string Password
        {
            get;
            set;
        }

        [Required]
        public string Old
        {
            get;
            set;
        }

        [Required]
        public string New
        {
            get;
            set;
        }
        
        public string RepositoryPath
        {
            get;
            set;
        }

        public string LocalPath
        {
            get;
            set;
        }

        public bool OldIsBasePath
        {
            get;
            set;
        }

        // TODO: 경로를 잡으려면 로컬 복사본의 리비전이 BasePath의 리비전과 일치해야 한다.
        public bool ResolveLocalPaths { get; set; }
    
        private ITaskItem[] GetPaths(AbstractSvnDiff.State state, AbstractSvnDiff.Kind kind)
        {
            var result = from itemChanged in _itemsChanged
                         where itemChanged.GetMetadata("State") == Enum.GetName(typeof(AbstractSvnDiff.State), state)
                         && itemChanged.GetMetadata("Kind") == Enum.GetName(typeof(AbstractSvnDiff.Kind), kind)
                             select itemChanged
                             ;
            return result.ToArray();
        }

        private ITaskItem[] GetPaths(AbstractSvnDiff.State state)
        {
            var result = from itemChanged in _itemsChanged
                         where itemChanged.GetMetadata("State") == Enum.GetName(typeof(AbstractSvnDiff.State), state)
                         select itemChanged
                             ;
            return result.ToArray();
        }


        [Output]
        public ITaskItem[] Items
        {
            get
            {
                return _itemsChanged.ToArray();
            }
        }

        [Output]
        public ITaskItem[] ItemsAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added);
            }
        }

        [Output]
        public ITaskItem[] FoldersAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added, AbstractSvnDiff.Kind.Directory);
            }
        }

        [Output]
        public ITaskItem[] FilesAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added, AbstractSvnDiff.Kind.File);
                
            }
        }

        [Output]
        public ITaskItem[] ItemsModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified);
            }
        }

        [Output]
        public ITaskItem[] FoldersModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified, AbstractSvnDiff.Kind.Directory);
            }
        }

        [Output]
        public ITaskItem[] FilesModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified, AbstractSvnDiff.Kind.File);
            }
        }

        [Output]
        public ITaskItem[] ItemsDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted);
            }
        }

        [Output]
        public ITaskItem[] FoldersDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted, AbstractSvnDiff.Kind.Directory);
            }
        }

        [Output]
        public ITaskItem[] FilesDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted, AbstractSvnDiff.Kind.File);
            }
        }

        [Output]
        public ITaskItem[] ItemsInConflict
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Conflict);
            }
        }

        [Output]
        public ITaskItem[] FoldersInConflict
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Conflict, AbstractSvnDiff.Kind.Directory);
            }
        }

        [Output]
        public ITaskItem[] FilesInConflict
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Conflict, AbstractSvnDiff.Kind.File);
            }
        }

    
    }
}
