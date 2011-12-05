using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Diagnostics;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    using Microsoft.Build.Framework;

    /// <summary>
    /// Display the differences between two paths.
    /// </summary>
    /// <example>
    /// <code title="Display the differences between two paths." lang="xml" source=".\Samples\msbuild-SvnDiff.xml" />
    /// </example>
    /// <remarks>
    /// <see cref="SvnDiff"/> run both of "svn diff --summary" and "svn diff --xml", and then combine two results into the one. 
    /// The reason why this class run the command twice is that some Asian charaters are broken with '--xml' switch.
    /// </remarks>
    public class SvnDiff : ToolTask 
    {
        private readonly SvnDiffWithPlainSummary _plainDiff;
        private readonly SvnDiffWithXmlSummary _xmlDiff;

        private readonly List<ITaskItem> _itemsChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="T:System.Object"/> class.
        /// </summary>
        /// <remarks></remarks>
        /// <inheritdoc />
        public SvnDiff()
        {
            this._plainDiff = new SvnDiffWithPlainSummary();
            this._xmlDiff = new SvnDiffWithXmlSummary();

            this._itemsChanged = new List<ITaskItem>();
        }

        /// <inheritdoc />
        protected override string GenerateFullPathToTool()
        {
            Debug.Assert(_plainDiff.ToolPath == _xmlDiff.ToolPath);
            return _plainDiff.ToolPath;
        }

        /// <inheritdoc />
        protected override string ToolName
        {
            get 
            { 
                if(string.IsNullOrEmpty(ToolPath) == false)
                {
                    string path = Path.Combine(ToolPath, "svn.exe");
                    _plainDiff.ToolPath = path;
                    _xmlDiff.ToolPath = path;
                    return path;
                }

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

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            if(base.ValidateParameters() == false)
                return false;

            if (ResolveLocalPaths == true && this.LocalPath == null)
            {
                Log.LogError(Properties.Resources.ParameterRequired, "SvnDiff", "LocalPath");
                return false;
            }
            return true;
        }

        /// <inheritdoc />
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
                    string.Format("Svn diff comparison failed! ({0}:{1})", plainSummary.Length, xmlSummary.Length)
                    );
                return false;
            }

            for (int i = 0; i < plainSummary.Length; i++ )
            {
                var plainItem = plainSummary[i];
                var xmlItem = xmlSummary[i];


                if(plainItem.StateMarked != xmlItem.StateMarked)
                    throw new Exception("Svn diff comparison failed!");

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


        /// <summary>
        /// Gets or sets the username.
        /// </summary>
        /// <value>The username.</value>
        /// <remarks></remarks>
        public string Username
        {
            get;
            set;
        }

        /// <summary>
        /// Gets or sets the password.
        /// </summary>
        /// <value>The password.</value>
        /// <remarks></remarks>
        public string Password
        {
            get;
            set;
        }

        /// <summary>
        /// [Required] Gets or sets the old target.
        /// </summary>
        /// <value>The old target.</value>
        /// <remarks></remarks>
        [Required]
        public string Old
        {
            get; set;
        }

        /// <summary>
        /// [Required] Gets or sets the new target.
        /// </summary>
        /// <value>The new target.</value>
        /// <remarks></remarks>
        [Required]
        public string New
        {
            get;
            set;
        }

        /// <summary>
        /// Gets or sets a value indicating whether the local working copy is old one.
        /// </summary>
        /// <value>Set <c>true</c> if the local working copy is old one; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool OldIsBasePath
        {
            get;
            set;
        }

        /// <summary>
        /// Gets or sets the repository path.
        /// </summary>
        /// <value>The repository path.</value>
        /// <remarks></remarks>
        public string RepositoryPath
        {
            get;
            set;
        }

        /// <summary>
        /// Gets or sets the local path.
        /// </summary>
        /// <value>The local path.</value>
        /// <remarks></remarks>
        public string LocalPath
        {
            get;
            set;
        }

        // TODO: 경로를 잡으려면 로컬 복사본의 리비전이 BasePath의 리비전과 일치해야 한다.
        /// <summary>
        /// Gets or sets a value indicating whether [resolve local paths].
        /// </summary>
        /// <value><c>true</c> if [resolve local paths]; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool ResolveLocalPaths { get; set; }

        /// <summary>
        /// Gets the paths.
        /// </summary>
        /// <param name="state">The state.</param>
        /// <param name="kind">The kind.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private ITaskItem[] GetPaths(AbstractSvnDiff.State state, AbstractSvnDiff.Kind kind)
        {
            var result = from itemChanged in _itemsChanged
                         where itemChanged.GetMetadata("State") == Enum.GetName(typeof(AbstractSvnDiff.State), state)
                         && itemChanged.GetMetadata("Kind") == Enum.GetName(typeof(AbstractSvnDiff.Kind), kind)
                             select itemChanged
                             ;
            return result.ToArray();
        }

        /// <summary>
        /// Gets the paths.
        /// </summary>
        /// <param name="state">The state.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private ITaskItem[] GetPaths(AbstractSvnDiff.State state)
        {
            var result = from itemChanged in _itemsChanged
                         where itemChanged.GetMetadata("State") == Enum.GetName(typeof(AbstractSvnDiff.State), state)
                         select itemChanged
                             ;
            return result.ToArray();
        }


        /// <summary>
        /// [Output] Gets the items which were added, modified, deleted and conflicted between <see cref="Old"/> and <see cref="New"/>.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] Items
        {
            get
            {
                return _itemsChanged.ToArray();
            }
        }

        /// <summary>
        /// [Output] Gets the items added.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] ItemsAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added);
            }
        }

        /// <summary>
        /// [Output] Gets the folders added.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] FoldersAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added, AbstractSvnDiff.Kind.Directory);
            }
        }

        /// <summary>
        /// [Output] Gets the files added.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] FilesAdded
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Added, AbstractSvnDiff.Kind.File);
                
            }
        }

        /// <summary>
        /// [Output] Gets the items modified.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] ItemsModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified);
            }
        }

        /// <summary>
        /// [Output] Gets the folders modified.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] FoldersModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified, AbstractSvnDiff.Kind.Directory);
            }
        }

        /// <summary>
        /// [Output] Gets the files modified.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] FilesModified
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Modified, AbstractSvnDiff.Kind.File);
            }
        }

        /// <summary>
        /// [Output] Gets the items deleted.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] ItemsDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted);
            }
        }

        /// <summary>
        /// [Output] Gets the folders deleted.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] FoldersDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted, AbstractSvnDiff.Kind.Directory);
            }
        }

        /// <summary>
        /// [Output] Gets the files deleted.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] FilesDeleted
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Deleted, AbstractSvnDiff.Kind.File);
            }
        }

        /// <summary>
        /// [Output] Gets the items in conflict.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] ItemsInConflict
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Conflict);
            }
        }

        /// <summary>
        /// [Output] Gets the folders in conflict.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] FoldersInConflict
        {
            get
            {
                return GetPaths(AbstractSvnDiff.State.Conflict, AbstractSvnDiff.Kind.Directory);
            }
        }

        /// <summary>
        /// [Output] Gets the files in conflict.
        /// </summary>
        /// <remarks></remarks>
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
