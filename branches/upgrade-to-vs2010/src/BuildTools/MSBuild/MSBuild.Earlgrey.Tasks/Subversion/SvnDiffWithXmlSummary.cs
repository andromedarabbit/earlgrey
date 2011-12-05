using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.Diagnostics;

namespace MSBuild.Earlgrey.Tasks.Subversion
{
    internal class SvnDiffWithXmlSummary : AbstractSvnDiff
    {
        private diff _diffObj;
        private List<ItemChanged> _itemsChanged;


        /// <summary>
        /// Initializes a new instance of the <see cref="T:SvnDiffWithXmlSummary"/> class.
        /// </summary>
        public SvnDiffWithXmlSummary()
            : base(true)
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
            GenerateSummary();
            return true;
        }

        private void ResetMemberVariables()
        {
            _diffObj = null;
            _itemsChanged = new List<ItemChanged>();
        }

        private void ParseOutput()
        {
            string xmlText = OutputText;

            _diffObj = GetObjectFromDiffFile(xmlText);
            if (_diffObj == null)
                throw new Exception("Invalid diff xml file!");
        }

        private static diff GetObjectFromDiffFile(string xmlText)
        {
            XmlParserContext context = new XmlParserContext(null, null, "", XmlSpace.None);
            using (TextReader tr = new StringReader(xmlText))
            {
                XmlSerializer ser = new XmlSerializer(typeof (diff));
                return ser.Deserialize(tr) as diff;
            }
        }

        private static diffPathsPath[] GetPaths(diff diffObj)
        {
            if (diffObj == null)
                return null;

            Trace.Assert(diffObj.Items.Length < 2);
            if (diffObj.Items.Length == 0)
                return null;

            return diffObj.Items[0].path;
        }

        internal override ItemChanged[] Summary
        {
            get 
            {
                return _itemsChanged.ToArray<ItemChanged>();
            }
        }

        private void GenerateSummary() // diff diffObj) // , string pathKind, string pathItem)
        {
            var paths = GetPaths(_diffObj);
            if (paths == null)
                return;

            foreach(var path in paths)
            {
                State state = GetState(path.props);
                if(state == State.None)
                    state = GetState(path.item);

                if(state == State.None)
                    continue;

                var itemChanged = new ItemChanged
                {
                    KindOf = GetKind(path.kind),
                    Path = GetProperRepositoryPath(path.Value),
                    StateMarked = state
                };
                
                this._itemsChanged.Add(itemChanged);
            }            
        }

        private static Kind GetKind(string kindStr)
        {
            if (kindStr == "file")
                return Kind.File;
            if (kindStr == "dir")
                return Kind.Directory;
            throw new Exception("Unknown kind value '" + kindStr + "'!");
        }

        private static State GetProps(string stateStr)
        {
            return GetState(stateStr);
        }

        private static State GetState(string stateStr)
        {
            if (stateStr == "added")
                return State.Added;
            if (stateStr == "deleted")
                return State.Deleted;
            if (stateStr == "modified")
                return State.Modified;
            if (stateStr == "conflict") // TODO: 'conflict'가 맞는지 확인 안 해봤음
                return State.Conflict;
            if (stateStr == "none")
                return State.None;
            throw new Exception("Unknown state value '" + stateStr + "'!");
        }
    }
}
