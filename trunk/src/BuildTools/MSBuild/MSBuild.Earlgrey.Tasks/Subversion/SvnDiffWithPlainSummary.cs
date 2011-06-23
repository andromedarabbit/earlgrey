using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    public class SvnDiffWithPlainSummary : AbstractSvnDiff
    {
        private List<ItemChanged> _itemsChanged;


        /// <summary>
        /// Initializes a new instance of the <see cref="T:SvnDiffWithPlainSummary"/> class.
        /// </summary>
        public SvnDiffWithPlainSummary()
            : base(false)
        {
            ResetMemberVariables();
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

            ParseOutput();
            return true;
        }

        private void ResetMemberVariables()
        {
            _itemsChanged = new List<ItemChanged>();
        }

        private void ParseOutput()
        {
            string reportText = OutputText;
            
            using (StringReader sr = new StringReader(reportText))
            {
                string line = sr.ReadLine();
                while (string.IsNullOrEmpty(line) == false)
                {
                    string markStr = line.Substring(0, 1);
                    if(line.Length >= 2)
                        markStr = line.Substring(0, 2).Trim();

                    State state = GetState(markStr);

                    string path = line.Substring(1, line.Length - 1).Trim();
                    if (string.IsNullOrEmpty(path) == true)
                        throw new Exception("This 'svn diff' command reported in a wrong format!");

                    path = GetProperRepositoryPath(path);

                    _itemsChanged.Add(
                        new AbstractSvnDiff.ItemChanged()
                        {
                            KindOf = Kind.None,
                            Path = path,
                            StateMarked = state
                        }
                    );

                    line = sr.ReadLine();
                }
            }
        }

        private static State GetState(string markStr)
        {
            if( markStr == "M" )
                return State.Modified;
            if (markStr == "D")
                return State.Deleted;
            if (markStr == "A")
                return State.Added;
            if (markStr == "C")
                return State.Conflict;

            throw new Exception("Unknown marking '" + markStr + "'!");
        }

        internal override ItemChanged[] Summary
        {
            get
            {
                return _itemsChanged.ToArray<ItemChanged>();
            }
        }

    }
}
