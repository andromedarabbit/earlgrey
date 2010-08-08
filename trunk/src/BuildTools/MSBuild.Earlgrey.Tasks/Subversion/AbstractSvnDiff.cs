using System;
using System.Text;
using Microsoft.Build.Framework;
using MSBuild.Community.Tasks.Subversion;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
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
        protected AbstractSvnDiff(bool withXmlOption)
        {
            base.Command = "diff";
            base.CommandSwitches = SvnSwitches.NonInteractive | SvnSwitches.NoAuthCache; // | SvnSwitches.Xml;
            if(withXmlOption)
            {
                base.CommandSwitches = base.CommandSwitches | SvnSwitches.Xml;
            }

            ResetMemberVariables();
        }

        /// <summary>
        /// Indicates whether all task paratmeters are valid.
        /// </summary>
        /// <returns>
        /// true if all task parameters are valid; otherwise, false.
        /// </returns>
        protected override bool ValidateParameters()
        {
            if (string.IsNullOrEmpty(this._old))
            {
                Log.LogError(MSBuild.Community.Tasks.Properties.Resources.ParameterRequired, "SvnDiff", "Old");
                return false;
            }

            if (string.IsNullOrEmpty(this._new))
            {
                Log.LogError(MSBuild.Community.Tasks.Properties.Resources.ParameterRequired, "SvnDiff", "New");
                return false;
            }

            if (!String.IsNullOrEmpty(RepositoryPath))
            {
                Log.LogError(MSBuild.Community.Tasks.Properties.Resources.ParameterNotUsed, "SvnCopy", "RepositoryPath");
                return false;
            }

            if (!String.IsNullOrEmpty(LocalPath))
            {
                Log.LogError(MSBuild.Community.Tasks.Properties.Resources.ParameterNotUsed, "SvnCopy", "LocalPath");
                return false;
            }

            return base.ValidateParameters();
        }

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

        /// <summary>
        /// Logs the events from text output.
        /// </summary>
        /// <param name="singleLine">The single line.</param>
        /// <param name="messageImportance">The message importanZce.</param>
        protected override void LogEventsFromTextOutput(string singleLine, MessageImportance messageImportance)
        {
            if (messageImportance == MessageImportance.High)
            {
                base.LogEventsFromTextOutput(singleLine, messageImportance);
            }
            _outputBuffer.AppendLine(singleLine);
        }

        /// <summary>
        /// Execute the task.
        /// </summary>
        /// <returns>true if execution is successful, false if not.</returns>
        public override bool Execute()
        {
            if (base.Execute() == false)
            {
                ResetMemberVariables();
                return false;
            }

            return true;
        }

        /// <summary>
        /// Reset all instance variables to their default (unset) state.
        /// </summary>
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
      
        public string Old
        {
            get { return _old; }
            set { _old = value; }
        }

        public string New
        {
            get { return _new; }
            set { _new = value; }
        }

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
            Added
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
