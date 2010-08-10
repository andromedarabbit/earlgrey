using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Diagnostics;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using Microsoft.Build.Framework;

    public class SvnDiff : Microsoft.Build.Utilities.ToolTask 
    {
        private readonly SvnDiffWithPlainSummary _plainDiff;
        private readonly SvnDiffWithXmlSummary _xmlDiff;

        private readonly List<AbstractSvnDiff.ItemChanged> _itemsChanged;

        public SvnDiff()
        {
            this._plainDiff = new SvnDiffWithPlainSummary();
            this._xmlDiff = new SvnDiffWithXmlSummary();

            this._itemsChanged = new List<AbstractSvnDiff.ItemChanged>();
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
            diffObj.RepositoryPath = this.RepositoryPath;
            diffObj.LocalPath = this.LocalPath;
            diffObj.OldIsBasePath = this.OldIsBasePath;
        }

        public override bool Execute()
        {
            InitializeInternalDiff(this._plainDiff);
            if (_plainDiff.Execute() == false)
                return false;

            InitializeInternalDiff(this._xmlDiff);
            if (_xmlDiff.Execute() == false)
                return false;

            var plainSummary = _plainDiff.Summary;
            var xmlSummary = _xmlDiff.Summary;

            if (plainSummary.Length != xmlSummary.Length)
                throw new Exception("Svn diffs comparison failed!");

            for (int i = 0; i < plainSummary.Length; i++ )
            {
                var plainItem = plainSummary[i];
                var xmlItem = xmlSummary[i];


                if(plainItem.StateMarked != xmlItem.StateMarked)
                    throw new Exception("Svn diffs comparison failed!");

                _itemsChanged.Add(
                    new AbstractSvnDiff.ItemChanged
                    {
                        KindOf = xmlItem.KindOf,
                        Path = plainItem.Path,
                        StateMarked = plainItem.StateMarked
                    }
                );
            }


            return true;
        }


        public string Old
        {
            get;
            set;
        }

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
    
        private string[] GetPaths(AbstractSvnDiff.State state, AbstractSvnDiff.Kind kind)
        {
            var result = from itemChanged in _itemsChanged
                             where itemChanged.StateMarked == state && itemChanged.KindOf == kind
                             select itemChanged.Path
                             ;
            return result.ToArray();
        }

        private string[] GetPaths(AbstractSvnDiff.State state)
        {
            var result = from itemChanged in _itemsChanged
                         where itemChanged.StateMarked == state
                         select itemChanged.Path
                             ;
            return result.ToArray();
        }

        [Output]
        public string[] ItemsAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added);
            }
        }

        [Output]
        public string[] FoldersAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added, AbstractSvnDiff.Kind.Directory);
            }
        }

        [Output]
        public string[] FilesAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added, AbstractSvnDiff.Kind.File);
                
            }
        }

        [Output]
        public string[] ItemsModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified);
            }
        }

        [Output]
        public string[] FoldersModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified, AbstractSvnDiff.Kind.Directory);
            }
        }

        [Output]
        public string[] FilesModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified, AbstractSvnDiff.Kind.File);
            }
        }

        [Output]
        public string[] ItemsDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted);
            }
        }

        [Output]
        public string[] FoldersDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted, AbstractSvnDiff.Kind.Directory);
            }
        }

        [Output]
        public string[] FilesDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted, AbstractSvnDiff.Kind.File);
            }
        }

        [Output]
        public string[] ItemsInConflict
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Conflict);
            }
        }

        [Output]
        public string[] FoldersInConflict
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Conflict, AbstractSvnDiff.Kind.Directory);
            }
        }

        [Output]
        public string[] FilesInConflict
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Conflict, AbstractSvnDiff.Kind.File);
            }
        }

    
    }
}
