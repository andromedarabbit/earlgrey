using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using IniParser;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{
    public class GetSections : AbstractIniFile
    {
        private readonly List<TaskItem> _sections;

        public GetSections()
        {
            _sections = new List<TaskItem>();
        }

        protected override bool ExecuteCommand()
        {
            if(base.ExecuteCommand() == false)
                return false;

            foreach (SectionData section in Data.Sections)
            {
                string sectionName = section.SectionName;

                if (sectionName == StreamIniDataParser.GlobalSectionName)
                    sectionName = "[Global]";

                TaskItem item = new TaskItem(sectionName);
                item.SetMetadata("NumberOfKeys", section.Keys.Count.ToString());

                _sections.Add(item);
            }


            return true;
        }

        protected override bool ValidateParameters()
        {
            return base.ValidateParameters();
        }

        [Output]
        public TaskItem[] Sections 
        { 
            get { return _sections.ToArray(); } 
        }
    }
}
