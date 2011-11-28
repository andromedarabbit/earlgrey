using System;
using System.Text;
using Microsoft.Build.Framework;
using MSBuild.Community.Tasks.Subversion;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    /// <summary>
    /// Gives functionality of "svn diff", which displays the differences between two paths.
    /// </summary>
    /// <remarks></remarks>
    public abstract class AbstractSvnDiff : SvnClient
    {
        private const string SwitchBooleanFormat = " --{0}";
        private const string SwitchOldNewFormat = " --{0}=\"{1}\"";

        private string _old;
        private string _new;

        private StringBuilder _outputBuffer;

        /// <summary>
        /// Initializes a new instance of the <see cref="T:AbstractSvnDiff"/> class.
        /// </summary>
        /// <param name="withXmlOption"><c>true</c> if you want to display the result with XML format (--xml); <c>false</c> if you need the summary (--summary).</param>
        /// <inheritdoc />
        protected AbstractSvnDiff(bool withXmlOption)
        {
            base.Command = "diff";
            base.NonInteractive = true;
            base.NoAuthCache = true;

            if(withXmlOption)
            {
                base.Xml = true;
            }

            ResetMemberVariables();
        }

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            if (string.IsNullOrEmpty(this._old))
            {
                Log.LogError(Properties.Resources.ParameterRequired, "SvnDiff", "Old");
                return false;
            }

            if (string.IsNullOrEmpty(this._new))
            {
                Log.LogError(Properties.Resources.ParameterRequired, "SvnDiff", "New");
                return false;
            }

            if (!String.IsNullOrEmpty(RepositoryPath))
            {
                Log.LogError(Properties.Resources.ParameterNotUsed, "SvnCopy", "RepositoryPath");
                return false;
            }

            if (!String.IsNullOrEmpty(LocalPath))
            {
                Log.LogError(Properties.Resources.ParameterNotUsed, "SvnCopy", "LocalPath");
                return false;
            }

            return base.ValidateParameters();
        }

        /// <inheritdoc />
        protected override string GenerateSvnCommand()
        {
            StringBuilder builder = new StringBuilder();

            builder.Append(Command);

            if (!string.IsNullOrEmpty(_old))
                builder.AppendFormat(SwitchOldNewFormat, "old", _old);

            if (!string.IsNullOrEmpty(_new))
                builder.AppendFormat(SwitchOldNewFormat, "new", _new);

            builder.AppendFormat(SwitchBooleanFormat, "summarize");
            // TODO: xml 옵션을 쓰면 버그가 있어서 한글 문자열이 깨진다.
            // builder.AppendFormat(_switchBooleanFormat, "xml");

            return builder.ToString();
        }

        /// <inheritdoc />
        protected override void LogEventsFromTextOutput(string singleLine, MessageImportance messageImportance)
        {
            if (messageImportance == MessageImportance.High)
            {
                base.LogEventsFromTextOutput(singleLine, messageImportance);
            }
            _outputBuffer.AppendLine(singleLine);
        }

        /// <inheritdoc />
        public override bool Execute()
        {
            if (base.Execute() == false)
            {
                ResetMemberVariables();
                return false;
            }

            return true;
        }

        private void ResetMemberVariables()
        {
            RepositoryPath = string.Empty;
            LocalPath = string.Empty;

            _outputBuffer = new StringBuilder();
        }

        protected string OutputText
        {
            get
            {
                return _outputBuffer.ToString();
            }
        }

        /// <summary>
        /// [Required] Gets or sets the old target.
        /// </summary>
        /// <value>The old target.</value>
        /// <remarks></remarks>
        [Required]
        public string Old
        {
            get { return _old; }
            set { _old = value; }
        }

        /// <summary>
        /// [Required] Gets or sets the new target.
        /// </summary>
        /// <value>The new target.</value>
        /// <remarks></remarks>
        [Required]
        public string New
        {
            get { return _new; }
            set { _new = value; }
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

        private static string StripRevisionMark(string repositoryPath)
        {
            int revisionMarkIndex = repositoryPath.LastIndexOf('@');
            return repositoryPath.Remove(revisionMarkIndex);
        }

        protected string GetProperRepositoryPath(string path)
        {
            if(OldIsBasePath)
                return path;

            
            string oldPath = StripRevisionMark(Old);
            string newPath = StripRevisionMark(New);
            return path.Replace(oldPath, newPath);
        }

        internal enum State
        {
            None
            , Added
            , Modified
            , Deleted
            , Conflict
        }

        internal enum Kind
        {
            None
            , File
            , Directory
        }

    

        internal class ItemChanged
        {
            public State StateMarked
            {
                get; 
                set;
            }

            public string Path
            {
                get;
                set;
            }

            public Kind KindOf
            {
                get;
                set;
            }
        }

        internal abstract ItemChanged[] Summary
        {
            get;
        }


    }
}
