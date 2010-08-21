using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using IniParser;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{
    public class GetSection : AbstractIniFile
    {
        private readonly List<TaskItem> _keyValues;

        public GetSection()
        {
            _keyValues = new List<TaskItem>();
        }

        protected override bool ExecuteCommand()
        {
            if(base.ExecuteCommand() == false)
                return false;

            if (Data.Sections.ContainsSection(InternalSection) == false)
                throw new Exception(
                    string.Format("Section named '{0}' not found!", InternalSection)
                    );

            KeyDataCollection section = Data[InternalSection];
            foreach (KeyData keyData in section)
            {
                TaskItem keyValue = GetKeyValue(keyData);
                _keyValues.Add(keyValue);
            }

            return true;
        }

        [Required]
        public ITaskItem Section { get; set; }

        protected string InternalSection
        {
            get
            {
                if (Section.ItemSpec == GlobalSectionName)
                    return StreamIniDataParser.GlobalSectionName;
                return Section.ItemSpec;
            }
        }

        [Output]
        public TaskItem[] KeyValues
        {
            get 
            { 
                return _keyValues.ToArray();
            }
        }

        private static TaskItem GetKeyValue(KeyData keyData)
        {
            TaskItem keyValue = new TaskItem(keyData.KeyName);
            keyValue.SetMetadata("Value", keyData.Value);

            string comments = GetComments(keyData);
            keyValue.SetMetadata("Comments", comments);
            return keyValue;
        }

        private static string GetComments(KeyData keyData)
        {
            StringBuilder sb = new StringBuilder();
            foreach (string comment in keyData.Comments)
            {
                sb.AppendLine(comment);
            }
            return sb.ToString();
        }
    }
}
